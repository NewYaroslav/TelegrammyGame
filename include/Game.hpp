#pragma once

#include "ObjectFactory.hpp"
#include "DialogueManager.hpp"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <chrono>
#include <emscripten.h>
#include <emscripten/bind.h>

#define PLAYER_ID 0

class Game {
public:

    Game() {
        GameArea::get_instance();
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        m_rnd_gen = std::mt19937(seed);
        m_dist_chance = std::uniform_real_distribution<>(0.0, 1.0);
    }

    ~Game() = default;

    static Game& get_instance() {
        static Game instance;
        return instance;
    }

    void start_game() {
        m_is_game_over = false;
        m_is_game_start = false;
        m_is_game_block = false;
        m_game_progress = 0;
        m_game_prev_progress = 0;
        m_objects.clear();
        spawn_player();
        EM_ASM({
            document.getElementById('intro-screen').style.display = 'none';
            document.getElementById('game-area').style.display = 'block';
            document.getElementById('game-over').style.display = 'none';
        });
        show_rkn_start();
        std::cout << "start_game" << std::endl;
    }

    void run() {
        update();
        draw();
    }

    void handle_input(int ch, int mode) {
        m_keys_pressed[ch] = (mode == 1);

        if (!m_is_game_start && m_is_game_intro) {
            m_is_game_start = (mode == 0);
        }

        if (!mode) return;

        if (m_is_game_start || m_is_game_over) {
            switch (ch) {
            case 'R':
            case 'r':
                start_game();
                m_is_game_start = true;
                break;
            case 81: // 'Q' key
                if (mode == 0) {
                    m_is_game_over = true;
                    end_game();
                }
                break;
            default:
                break;
            }
        } else
        if (!m_is_game_intro) {
            switch (ch) {
            case 32: // Space bar
                start_game();
                m_is_game_intro = true;
                break;
            default:
                break;
            }
        }
    }

private:
    std::mt19937                                m_rnd_gen;
    std::uniform_real_distribution<>            m_dist_chance;
    std::vector<std::unique_ptr<BaseObject>>    m_objects;
    std::map<int, bool>                         m_keys_pressed;
    int32_t m_game_progress = 0;
    int32_t m_game_prev_progress = 0;
    bool    m_is_game_progress_change = false;
    bool    m_is_game_over     = false;
    bool    m_is_game_start    = false;
    bool    m_is_game_intro    = false;
    bool    m_is_game_block    = false;

    DialogueManager m_dialogue_manager;

    void update() {
        update_keys_pressed();

        if (!m_is_game_start) return;

        m_dialogue_manager.update();

        // Обновляем состояния объектов
        for (auto& object : m_objects) {
            object->update();
        }

        // Обрабатываем урон
        for (size_t i = 0; i < m_objects.size(); ++i) {
            BaseObject* object_a = m_objects[i].get();

            if (object_a->get_category() == ObjectCategory::UNDEFINED) continue;
            if (!object_a->is_online() || object_a->is_destroy()) continue;

            if (object_a->get_category() == ObjectCategory::BULLET ||
                object_a->get_category() == ObjectCategory::EXPLOSION ||
                object_a->get_category() == ObjectCategory::PLANE) {
                for (size_t j = 0; j < m_objects.size(); ++j) {
                    if (i == j) continue;

                    BaseObject* object_b = m_objects[j].get();
                    if (object_b->get_category() != ObjectCategory::ENEMY) continue;
                    if (!object_b->is_online() || object_b->is_destroy()) continue;

                    if (object_a->check_intersection(*object_b)) {
                        const int32_t object_a_hp = object_a->hp;
                        const int32_t object_b_hp = object_b->hp;
                        if (object_a->get_category() != ObjectCategory::EXPLOSION) {
                            object_a->damage(object_b_hp);
                        }
                        object_b->damage(object_a_hp);
                    }
                } // for j
            }
        } // for i

        // Обрабатываем получение боеприпасов
        for (size_t i = 0; i < m_objects.size(); ++i) {
            BaseObject* object_a = m_objects[i].get();
            if (object_a->get_category() != ObjectCategory::PLANE) continue;
            if (!object_a->is_online() || object_a->is_destroy()) continue;

            PlaneObject* player = dynamic_cast<PlaneObject*>(object_a);

            for (size_t j = 0; j < m_objects.size(); ++j) {
                if (i == j) continue;
                BaseObject* object_b = m_objects[j].get();
                if (object_b->get_category() != ObjectCategory::AMMO) continue;
                if (!object_b->is_online() || object_b->is_destroy()) continue;

                if (player->check_intersection(*object_b)) {
                    const int32_t object_b_hp = object_b->hp;
                    object_b->damage(object_b_hp);
                    player->current_ammo += object_b_hp;
                }
            } // for j

            if (player->is_stop_game()) {
                m_is_game_block = true;
            }

            break;
        } // for i

        // Обрабатываем уничтожение объектов
        for (auto& object : m_objects) {
            if (!object->is_online() && object->get_category() == ObjectCategory::ENEMY) {
                if (!m_is_game_block) ++m_game_progress;
                std::cout << "game_progress " << m_game_progress << std::endl;
            }
            if (!object->is_destroy()) continue;
            if (object->get_category() == ObjectCategory::PLANE) {
                m_is_game_over = true;
                end_game();
            } else
            if (object->get_category() == ObjectCategory::ENEMY) {
                if (!m_is_game_block) ++m_game_progress;
                std::cout << "game_progress " << m_game_progress << std::endl;
            }
        }

        m_objects.erase(std::remove_if(
            m_objects.begin(),
            m_objects.end(),
            [this](std::unique_ptr<BaseObject> &object) {
            return !object->is_online() || object->is_destroy();
        }), m_objects.end());

        // Генерация облаков
        generate_сloud();
        generate_enemy();
        generate_cash_ammo();
    }

    void generate_сloud() {
        // Генерация облаков
        double rnd_value = m_dist_chance(m_rnd_gen);
        if (rnd_value < 0.035) {
            GameArea& game_area = GameArea::get_instance();

            const int32_t area_width = game_area.get_width();
            const int32_t max_height = game_area.get_height() / 3;
            const int32_t min_height = CLOUD_HEIGHT / 2;

            std::uniform_int_distribution<> dist_height(-min_height, max_height);

            const int32_t cloud_rnd_y = dist_height(m_rnd_gen);
            const int32_t cloud_y_ampl = max_height + min_height;
            const int32_t cloud_speed = ((cloud_rnd_y + min_height)  * CLOUD_SPEED) / (cloud_y_ampl == 0 ? 1 : cloud_y_ampl);
            const int32_t cloud_y = max_height - (cloud_rnd_y + min_height);

            const float cloud_scale = (float)cloud_speed / (float)CLOUD_SPEED;

            m_objects.emplace_back(ObjectFactory::make_object(ObjectType::CLOUD, area_width + CLOUD_WIDTH, cloud_y, cloud_speed, cloud_scale));
        }
    }

    // Генерация врагов и боеприпасов
    void generate_enemy() {
        const double enemy_spawn_rate =
            m_game_progress < 5   ? 0.005 :
            m_game_progress < 30  ? 0.015 :
            m_game_progress < 40  ? 0.080 :
            m_game_progress < 100 ? 0.015 :
            m_game_progress < 150 ? 0.035 :
            m_game_progress < 200 ? 0.020 :
            m_game_progress < 250 ? 0.045 :
            m_game_progress < 300 ? 0.025 :
            m_game_progress < 350 ? 0.100 :
            m_game_progress < 400 ? 0.035 :
            m_game_progress < 500 ? 0.050 :
            (m_game_progress/100) % 2 == 0 ? 0.035 :
            0.10;

        if (m_game_progress >= 20 && m_game_prev_progress < 20) {
            show_rkn_filler_2();
        }
        if (m_game_progress >= 40 && m_game_prev_progress < 40) {
            show_rkn_level_1();
        }
        if (m_game_progress >= 80 && m_game_prev_progress < 80) {
            show_rkn_filler_1();
        }
        if (m_game_progress >= 100 && m_game_prev_progress < 100) {
            show_rkn_level_2();
        }
        if (m_game_progress >= 170 && m_game_prev_progress < 170) {
            show_rkn_level_3();
        }
        if (m_game_progress >= 300 && m_game_prev_progress < 300) {
            show_rkn_level_4();
        }

        m_game_prev_progress = m_game_progress;

        if (m_dist_chance(m_rnd_gen) < enemy_spawn_rate) {
            GameArea& game_area = GameArea::get_instance();
            const int32_t max_width = game_area.get_width();
            const int32_t max_height = game_area.get_height();

            if (m_game_progress < 40) {
                spawn_enemy(ObjectType::ENEMY_RKN_WEAK);
            } else {
                if (m_dist_chance(m_rnd_gen) < 0.5) {
                    spawn_enemy(ObjectType::ENEMY_RKN_WEAK);
                } else {
                    spawn_enemy(ObjectType::ENEMY_PACKAGE_WEAK);
                }
            }
        }
    }

    // Генерация боеприпасов
    void generate_cash_ammo() {
        const double ammo_spawn_rate =
            m_game_progress < 40  ? 0.001 :
            m_game_progress < 50  ? 0.025 :
            m_game_progress < 100 ? 0.001 :
            m_game_progress < 150 ? 0.010 :
            m_game_progress < 200 ? 0.005 :
            m_game_progress < 250 ? 0.012 :
            m_game_progress < 300 ? 0.005 :
            m_game_progress < 350 ? 0.015 :
            m_game_progress < 400 ? 0.007 :
            m_game_progress < 450 ? 0.025 :
            m_game_progress < 500 ? 0.010 :
            0.03;

        if (m_dist_chance(m_rnd_gen) < ammo_spawn_rate) {
            GameArea& game_area = GameArea::get_instance();
            const int32_t max_width = game_area.get_width();
            const int32_t max_height = game_area.get_height();

            std::uniform_int_distribution<> dist_x(CASH_AMMO_300_WIDTH, max_height - CASH_AMMO_300_WIDTH);
            m_objects.emplace_back(ObjectFactory::make_object(ObjectType::CASH_AMMO_300, dist_x(m_rnd_gen), 0 - CASH_AMMO_300_HEIGHT));
        }
    }

    void update_keys_pressed() {
        if (m_is_game_start && !m_is_game_over) {
            for (auto& item : m_keys_pressed) {
                // Ищем объект типа PLANE
                PlaneObject* player = nullptr;
                for (const auto& obj : m_objects) {
                    if (obj->get_category() == ObjectCategory::PLANE) {
                        player = dynamic_cast<PlaneObject*>(obj.get());
                        if (player) {
                            break;
                        }
                    }
                }

                if (player) {
                    player->handle_input(item.first, item.second, [this](const int32_t& x, const int32_t& y) {
                        m_objects.emplace_back(ObjectFactory::make_object(ObjectType::BULLET_DOLLAR, x, y));
                    });
                }
            } // for
        } // if
    }

    void draw() {
        EM_ASM({
            // Очистка игрового пространства
            const gameArea = document.getElementById('game-area');
            const removableClasses = ['box'];
            Array.from(gameArea.children).forEach(element => {
                if (removableClasses.some(cls => element.classList.contains(cls))) {
                    gameArea.removeChild(element);
                }
            });
        });

        for (auto& object : m_objects) {
            object->draw();
        }

        update_hud();
    }

    void update_hud() {
        // Ищем объект типа PLANE
        PlaneObject* player = nullptr;
        for (const auto& obj : m_objects) {
            if (obj->get_category() == ObjectCategory::PLANE) {
                player = dynamic_cast<PlaneObject*>(obj.get());
                if (player) {
                    break;
                }
            }
        }

        if (player) {
            EM_ASM({
                const current_ammo = $0;
                const health = $1;

                // Обновление количества боеприпасов
                const ammoCountLabel = document.getElementById('ammo-count-label');
                const ammoCountElement = ammoCountLabel.getElementsByClassName('ammo-count')[0]; // Берем первый элемент
                if (ammoCountElement) {
                    ammoCountElement.textContent = current_ammo + "$";
                }

                // Обновление количества здоровья
                const healthLabel = document.getElementById('health-label');
                const currentHearts = healthLabel.getElementsByClassName('hud-heart').length;
                if (currentHearts < health) {
                    // Добавляем недостающие сердца
                    for (let i = currentHearts; i < health; i++) {
                        const heart = document.createElement('span');
                        heart.classList.add('hud-heart');
                        healthLabel.appendChild(heart);
                    }
                } else if (currentHearts > health) {
                    // Удаляем лишние сердца
                    for (let i = currentHearts; i > health; i--) {
                        healthLabel.removeChild(healthLabel.lastChild);
                    }
                }

            }, player->current_ammo, player->hp);
        }
    }

    void end_game() {
        EM_ASM({
            document.getElementById('game-over').style.display = 'block';
            document.getElementById('hero-dialogue').style.display = 'none';

            const heroSound = document.getElementById('sound-dialogue-rkn-end-1');
            heroSound.currentTime = 0;
            heroSound.play().catch(function(error) {
                console.log("Error playing music: " + error);
            });
        });
    }

    // Создание объекта игрока
    void spawn_player() {
        m_objects.emplace_back(ObjectFactory::make_object(
                ObjectType::PLANE,0,0,0,0,0,0,
                [this](BaseObject* object, const ObjectType& type) {
            switch (type) {
            case ObjectType::EXPLOSION_LARGE:
                m_objects.emplace_back(ObjectFactory::make_object(
                    ObjectType::EXPLOSION_LARGE,
                    object->x,
                    object->y,
                    object->speed,
                    0,
                    object->width*2,
                    object->height*2));
                break;
            case ObjectType::EXPLOSION_SMALL:
                m_objects.emplace_back(ObjectFactory::make_object(
                    ObjectType::EXPLOSION_SMALL,
                    object->x,
                    object->y,
                    object->speed,
                    0,
                    object->width,
                    object->height));
                break;
            case ObjectType::BULLET_DOLLAR:
                m_objects.emplace_back(ObjectFactory::make_object(
                    ObjectType::BULLET_DOLLAR,
                    object->x,
                    object->y,
                    0, 0, 0, 0,
                    [this](BaseObject* object, const ObjectType& type) {
                        m_objects.emplace_back(ObjectFactory::make_object(
                            ObjectType::EXPLOSION_SMALL,
                            object->x,
                            object->y,
                            object->speed,
                            0,
                            object->width,
                            object->height));
                }));
                break;
            default:
                break;
            };
        }));
    }

    // создание объектов врагов
    void spawn_enemy(const ObjectType& type) {
        GameArea& game_area = GameArea::get_instance();
        const int32_t max_width = game_area.get_width();
        const int32_t max_height = game_area.get_height();
        switch (type) {
        case ObjectType::ENEMY_RKN_WEAK: {
                std::uniform_int_distribution<> dist_y(ENEMY_RKN_WEAK_HEIGHT/2, max_height - ENEMY_RKN_WEAK_HEIGHT/2);
                m_objects.emplace_back(ObjectFactory::make_object(
                        ObjectType::ENEMY_RKN_WEAK,
                        max_width + ENEMY_RKN_WEAK_HEIGHT,
                        dist_y(m_rnd_gen),
                        0, 0, 0, 0,
                        [this](BaseObject* object, const ObjectType& type) {
                    m_objects.emplace_back(ObjectFactory::make_object(
                        ObjectType::EXPLOSION_ENEMY_RKN_WEAK,
                        object->x,
                        object->y,
                        object->speed,
                        0,
                        object->width,
                        object->height));
                }));
            }
            break;
        case ObjectType::ENEMY_PACKAGE_WEAK: {
                std::uniform_int_distribution<> dist_y(ENEMY_PACKAGE_WEAK_HEIGHT/2, max_height - ENEMY_PACKAGE_WEAK_HEIGHT/2);
                m_objects.emplace_back(ObjectFactory::make_object(
                        ObjectType::ENEMY_PACKAGE_WEAK,
                        max_width + ENEMY_RKN_WEAK_HEIGHT,
                        dist_y(m_rnd_gen),
                        0, 0, 0, 0,
                        [this](BaseObject* object, const ObjectType& type) {
                    m_objects.emplace_back(ObjectFactory::make_object(
                        ObjectType::EXPLOSION_ENEMY_PACKAGE_WEAK,
                        object->x,
                        object->y,
                        object->speed,
                        0,
                        object->width,
                        object->height));
                }));
            }
            break;
        default:
            break;
        };
    }

    void show_rkn_start() {
        m_dialogue_manager.add_dialogue(u8"Это опять ТЫ?!", "images/hero_rkn_1.png", "sound-dialogue-rkn-1", 2000);
        m_dialogue_manager.add_dialogue(u8"Тебе было мало прошлого раза?!", "images/hero_rkn_1.png", "sound-dialogue-rkn-2", 2000);
        m_dialogue_manager.add_dialogue(u8"Готовь вазилин!", "images/hero_rkn_1.png", "sound-dialogue-rkn-3", 2000, [this](){
            spawn_enemy(ObjectType::ENEMY_RKN_WEAK);
            spawn_enemy(ObjectType::ENEMY_RKN_WEAK);
            spawn_enemy(ObjectType::ENEMY_RKN_WEAK);
            EM_ASM({
                const music = document.getElementById('music-background-1');
                music.volume = 0.5;
                if (music.paused) {
                    music.play().catch(function(error) {
                        console.log("Error playing music: " + error);
                    });
                }
            });
        });
        m_dialogue_manager.start();
    }

    void show_rkn_level_1() {
        m_dialogue_manager.add_dialogue(u8"Пытаешься увернуться от блокировок? Это бесполезно =)", "images/hero_rkn_1.png", "sound-dialogue-rkn-4", 3000);
        m_dialogue_manager.add_dialogue(u8"Посмотрим, как ты справишься с нашими ПАКЕТАМИ! =)", "images/hero_rkn_1.png", "sound-dialogue-rkn-5", 3000, [this](){
            for (int i = 0; i < 3; ++i) {
                spawn_enemy(ObjectType::ENEMY_PACKAGE_WEAK);
            }
        });
        m_dialogue_manager.start();
    }

    void show_rkn_level_2() {
        m_dialogue_manager.add_dialogue(u8"От чего так в России...", "images/hero_rkn_1.png", "sound-dialogue-rkn-6", 3000);
        m_dialogue_manager.add_dialogue(u8"Пакеты шуршат... Ха-ха-ха! =)", "images/hero_rkn_1.png", "sound-dialogue-rkn-7", 4000, [this](){
            for (int i = 0; i < 10; ++i) {
                spawn_enemy(ObjectType::ENEMY_PACKAGE_WEAK);
            }
        });
        m_dialogue_manager.start();
    }

    void show_rkn_level_3() {
        m_dialogue_manager.add_dialogue(u8"Я и не таких блокировала!", "images/hero_rkn_1.png", "sound-dialogue-rkn-8", 4000, [this](){
            for (int i = 0; i < 10; ++i) {
                spawn_enemy(ObjectType::ENEMY_PACKAGE_WEAK);
            }
            for (int i = 0; i < 10; ++i) {
                spawn_enemy(ObjectType::ENEMY_RKN_WEAK);
            }
        });
        m_dialogue_manager.start();
    }

    void show_rkn_level_4() {
        m_dialogue_manager.add_dialogue(u8"Да ты ЗАДОЛБАЛ! ЗАБАНЮ!!!", "images/hero_rkn_2.png", "sound-dialogue-rkn-9", 4000, [this](){
            for (int i = 0; i < 20; ++i) {
                spawn_enemy(ObjectType::ENEMY_PACKAGE_WEAK);
            }
            for (int i = 0; i < 20; ++i) {
                spawn_enemy(ObjectType::ENEMY_RKN_WEAK);
            }
        });
        m_dialogue_manager.start();
    }

    void show_rkn_filler_1() {
        m_dialogue_manager.add_dialogue(u8"Отдай ключи шифрования, Дуров! А то будет а-та-та!", "images/hero_rkn_1.png", "sound-dialogue-rkn-filler-1", 3000, [this](){
        });
        m_dialogue_manager.start();
    }

    void show_rkn_filler_2() {
        m_dialogue_manager.add_dialogue(u8"It's блокировка time!", "images/hero_rkn_1.png", "sound-dialogue-rkn-filler-2", 3000, [this](){
            for (int i = 0; i < 3; ++i) {
                spawn_enemy(ObjectType::ENEMY_RKN_WEAK);
            }
        });
        m_dialogue_manager.start();
    }

};
