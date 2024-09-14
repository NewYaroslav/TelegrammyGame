#pragma once

#include "World.hpp"
#include "PhysicsEngine.hpp"
#include "DialogueManager.hpp"
#include "Dialogues/DialogueRKN.hpp"
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

    Game() :
            m_world(), m_physics(), m_dialogue_manager(),
            m_dialogue_rkn(m_world, m_dialogue_manager) {
        ResourceManager::get_instance();

        m_physics.on_offline = [this](BaseObject* object) {
            if (object->get_category() == ObjectCategory::ENEMY) {
                if (!m_is_game_block) ++m_game_progress;
                std::cout << "game_progress " << m_game_progress << std::endl;
            }
        };

        m_physics.on_destroy = [this](BaseObject* object) {
            if (object->get_category() == ObjectCategory::PLANE) {
                m_is_game_over = true;
                end_game();
            } else
            if (object->get_category() == ObjectCategory::ENEMY) {
                if (!m_is_game_block) ++m_game_progress;
                std::cout << "game_progress " << m_game_progress << std::endl;
            }
        };

        m_physics.on_player = [this](PlaneObject* player) {
            if (player->is_stop_game()) {
                m_is_game_block = true;
            }
        };
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
        m_world.clear();
        m_world.spawn_player();
        EM_ASM({
            document.getElementById('intro-screen').style.display = 'none';
            document.getElementById('game-area').style.display = 'block';
            document.getElementById('game-over').style.display = 'none';
        });

        auto random_chance = ResourceManager::get_instance().random_chance();

        if (random_chance < 0.15) {
            m_dialogue_rkn.show_dialogue_rkn_1();
        } else
        if (random_chance < 0.3) {
            m_dialogue_rkn.show_dialogue_rkn_1_alt_1();
        } else
        if (random_chance < 0.45) {
            m_dialogue_rkn.show_dialogue_rkn_1_alt_2();
        } else
        if (random_chance < 0.6) {
            m_dialogue_rkn.show_dialogue_rkn_1_alt_3();
        } else
        if (random_chance < 0.75) {
            m_dialogue_rkn.show_dialogue_rkn_1_alt_4();
        } else
        if (random_chance < 0.9) {
            m_dialogue_rkn.show_dialogue_rkn_1_alt_5();
        } else {
            m_dialogue_rkn.show_dialogue_rkn_1_alt_6();
        }

        //m_dialogue_rkn.show_dialogue_rkn_5();

        std::cout << "start_game" << std::endl;
    }

    void run() {
        update();
        draw();
    }

    void handle_input(int ch, int mode) {
        bool is_key_press = false;
        auto it_key = m_keys_pressed.find(ch);
        if (it_key != m_keys_pressed.end()) {
            if (!it_key->second && (mode == 1)) {
                is_key_press = true;
            }
        } else {
            is_key_press = (mode == 1);
        }

        m_keys_pressed[ch] = (mode == 1);

        if (!m_is_game_start && m_is_game_intro) {
            m_is_game_start = (mode == 0);
        }

        if (!is_key_press) return;

        if (m_is_game_start || m_is_game_over) {
            switch (ch) {
            case 'R':
            case 'r':
                start_game();
                m_is_game_start = true;
                break;
            case 'T':
            case 't':
                m_world.spawn_enemy(ObjectType::ENEMY_PACKAGE_MITOSIS);
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
    World               m_world;
    PhysicsEngine       m_physics;
    DialogueManager     m_dialogue_manager;
    DialogueRKN         m_dialogue_rkn;
    std::map<int, bool> m_keys_pressed;
    int32_t m_game_progress = 0;
    int32_t m_game_prev_progress = 0;
    bool    m_is_game_progress_change = false;
    bool    m_is_game_over     = false;
    bool    m_is_game_start    = false;
    bool    m_is_game_intro    = false;
    bool    m_is_game_block    = false;

    void update() {
        update_keys_pressed();
        if (!m_is_game_start) return;
        m_dialogue_manager.update();
        m_physics.update(m_world);
        m_world.update();
        generate_enemy();
        generate_cash_ammo();
    }

    // Генерация врагов и боеприпасов
    void generate_enemy() {
        const double enemy_spawn_rate =
            m_game_progress < 100 ? 0.015 : // начальный уровень
            m_game_progress < 200 ? 0.025 : // до появления пакетов
            m_game_progress < 300 ? 0.030 : // после пакетов
            m_game_progress < 400 ? 0.035 : // после пакетов
            (m_game_progress/100) % 2 == 0 ? 0.04 :
            0.08;

        if (m_game_progress >= 100 && m_game_prev_progress < 100) {
            auto random_chance = ResourceManager::get_instance().random_chance();
            if (random_chance < 0.2) {
                m_dialogue_rkn.show_dialogue_rkn_2();
            } else
            if (random_chance < 0.4) {
                m_dialogue_rkn.show_dialogue_rkn_2_alt_1();
            } else
            if (random_chance < 0.6) {
                m_dialogue_rkn.show_dialogue_rkn_2_alt_2();
            } else
            if (random_chance < 0.8) {
                m_dialogue_rkn.show_dialogue_rkn_2_alt_2();
            } else {
                m_dialogue_rkn.show_dialogue_rkn_2_alt_4();
            }
        }
        if (m_game_progress >= 200 && m_game_prev_progress < 200) {
            auto random_chance = ResourceManager::get_instance().random_chance();
            if (random_chance < 0.14) {
                m_dialogue_rkn.show_dialogue_rkn_3();
            } else
            if (random_chance < 0.28) {
                m_dialogue_rkn.show_dialogue_rkn_3_alt_1();
            } else
            if (random_chance < 0.42) {
                m_dialogue_rkn.show_dialogue_rkn_3_alt_2();
            } else
            if (random_chance < 0.56) {
                m_dialogue_rkn.show_dialogue_rkn_3_alt_3();
            } else
            if (random_chance < 0.70) {
                m_dialogue_rkn.show_dialogue_rkn_3_alt_4();
            } else
            if (random_chance < 0.84) {
                m_dialogue_rkn.show_dialogue_rkn_3_alt_5();
            } else {
                m_dialogue_rkn.show_dialogue_rkn_3_alt_6();
            }
        }
        if (m_game_progress >= 300 && m_game_prev_progress < 300) {
            auto random_chance = ResourceManager::get_instance().random_chance();
            if (random_chance < 0.5) {
                m_dialogue_rkn.show_dialogue_rkn_4();
            } else {
                m_dialogue_rkn.show_dialogue_rkn_4_alt_1();
            }
        }
        if (m_game_progress >= 400 && m_game_prev_progress < 400) {
            auto random_chance = ResourceManager::get_instance().random_chance();
            if (random_chance < 0.2) {
                m_dialogue_rkn.show_dialogue_rkn_5();
            } else
            if (random_chance < 0.4) {
                m_dialogue_rkn.show_dialogue_rkn_5_alt_1();
            } else
            if (random_chance < 0.6) {
                m_dialogue_rkn.show_dialogue_rkn_5_alt_2();
            } else
            if (random_chance < 0.8) {
                m_dialogue_rkn.show_dialogue_rkn_5_alt_3();
            } else {
                m_dialogue_rkn.show_dialogue_rkn_5_alt_4();
            }
        }

        m_game_prev_progress = m_game_progress;

        if (ResourceManager::get_instance().random_chance() < enemy_spawn_rate) {
            if (m_game_progress < 200) {
                auto random_chance = ResourceManager::get_instance().random_chance();
                if (random_chance < 0.1) {
                    auto random_chance_grid = ResourceManager::get_instance().random_chance();
                    if (random_chance_grid < 0.2) {
                        std::vector<std::vector<int>> bit_grid = {
                            {0, 0, 1, 0, 0},
                            {0, 1, 0, 0, 0},
                            {1, 0, 0, 0, 0},
                            {0, 1, 0, 0, 0},
                            {0, 0, 1, 0, 0}
                        };
                        m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
                    } else
                    if (random_chance_grid < 0.4) {
                        std::vector<std::vector<int>> bit_grid = {
                            {0, 0, 1, 0, 1},
                            {0, 1, 1, 0, 1},
                            {1, 0, 1, 1, 1},
                            {0, 1, 1, 0, 1},
                            {0, 0, 1, 0, 1}
                        };
                        m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
                    } else
                    if (random_chance_grid < 0.6) {
                        std::vector<std::vector<int>> bit_grid = {
                            {0, 0, 0, 0, 0},
                            {0, 0, 1, 0, 0},
                            {1, 1, 1, 0, 0},
                            {0, 0, 1, 0, 0},
                            {0, 0, 0, 0, 0}
                        };
                        m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
                    } else
                    if (random_chance_grid < 0.8) {
                        std::vector<std::vector<int>> bit_grid = {
                            {1, 0, 1, 0, 1},
                            {1, 0, 1, 0, 1},
                            {1, 0, 1, 0, 1},
                            {1, 0, 1, 0, 1},
                            {1, 0, 1, 0, 1}
                        };
                        m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
                    } else {
                        std::vector<std::vector<int>> bit_grid = {
                            {1, 0, 1, 0, 1},
                            {0, 1, 0, 1, 0},
                            {0, 0, 1, 0, 0},
                            {0, 1, 0, 1, 0},
                            {1, 0, 1, 0, 1}
                        };
                        m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
                    }
                } else
                if (random_chance < 0.6) {
                    m_world.spawn_enemy(ObjectType::ENEMY_RKN_WEAK);
                } else {
                    m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
                }
            } else
            if (m_game_progress < 300) {
                auto random_chance = ResourceManager::get_instance().random_chance();
                if (random_chance < 0.1) {
                    auto random_chance_grid = ResourceManager::get_instance().random_chance();
                    if (random_chance_grid < 0.2) {
                        std::vector<std::vector<int>> bit_grid = {
                            {0, 0, 1, 0, 0},
                            {0, 1, 0, 0, 0},
                            {1, 0, 0, 0, 0},
                            {0, 1, 0, 0, 0},
                            {0, 0, 1, 0, 0}
                        };
                        m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
                    } else
                    if (random_chance_grid < 0.4) {
                        std::vector<std::vector<int>> bit_grid = {
                            {0, 0, 1, 0, 1},
                            {0, 1, 1, 0, 1},
                            {1, 0, 1, 1, 1},
                            {0, 1, 1, 0, 1},
                            {0, 0, 1, 0, 1}
                        };
                        m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
                    } else
                    if (random_chance_grid < 0.6) {
                        std::vector<std::vector<int>> bit_grid = {
                            {0, 0, 0, 0, 0},
                            {0, 0, 1, 0, 0},
                            {1, 1, 1, 0, 0},
                            {0, 0, 1, 0, 0},
                            {0, 0, 0, 0, 0}
                        };
                        m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
                    } else
                    if (random_chance_grid < 0.8) {
                        std::vector<std::vector<int>> bit_grid = {
                            {1, 0, 1, 0, 1},
                            {1, 0, 1, 0, 1},
                            {1, 0, 1, 0, 1},
                            {1, 0, 1, 0, 1},
                            {1, 0, 1, 0, 1}
                        };
                        m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
                    } else {
                        std::vector<std::vector<int>> bit_grid = {
                            {1, 0, 1, 0, 1},
                            {0, 1, 0, 1, 0},
                            {0, 0, 1, 0, 0},
                            {0, 1, 0, 1, 0},
                            {1, 0, 1, 0, 1}
                        };
                        m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
                    }
                } else
                if (random_chance < 0.25) {
                    m_world.spawn_enemy(ObjectType::ENEMY_RKN_WEAK);
                } else
                if (random_chance < 0.5) {
                    m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
                } else {
                    m_world.spawn_enemy(ObjectType::ENEMY_PACKAGE_WEAK);
                }
            } else
            if (m_game_progress < 400) {
                auto random_chance = ResourceManager::get_instance().random_chance();
                if (random_chance < 0.1) {
                    auto random_chance_grid = ResourceManager::get_instance().random_chance();
                    if (random_chance_grid < 0.2) {
                        std::vector<std::vector<int>> bit_grid = {
                            {0, 0, 1, 0, 0},
                            {0, 1, 0, 0, 0},
                            {1, 0, 0, 0, 0},
                            {0, 1, 0, 0, 0},
                            {0, 0, 1, 0, 0}
                        };
                        m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
                    } else
                    if (random_chance_grid < 0.4) {
                        std::vector<std::vector<int>> bit_grid = {
                            {0, 0, 1, 0, 1},
                            {0, 1, 1, 0, 1},
                            {1, 0, 1, 1, 1},
                            {0, 1, 1, 0, 1},
                            {0, 0, 1, 0, 1}
                        };
                        m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
                    } else
                    if (random_chance_grid < 0.6) {
                        std::vector<std::vector<int>> bit_grid = {
                            {0, 0, 0, 0, 0},
                            {0, 0, 1, 0, 0},
                            {1, 1, 1, 0, 0},
                            {0, 0, 1, 0, 0},
                            {0, 0, 0, 0, 0}
                        };
                        m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
                    } else
                    if (random_chance_grid < 0.8) {
                        std::vector<std::vector<int>> bit_grid = {
                            {1, 0, 1, 0, 1},
                            {1, 0, 1, 0, 1},
                            {1, 0, 1, 0, 1},
                            {1, 0, 1, 0, 1},
                            {1, 0, 1, 0, 1}
                        };
                        m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
                    } else {
                        std::vector<std::vector<int>> bit_grid = {
                            {1, 0, 1, 0, 1},
                            {0, 1, 0, 1, 0},
                            {0, 0, 1, 0, 0},
                            {0, 1, 0, 1, 0},
                            {1, 0, 1, 0, 1}
                        };
                        m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
                    }
                } else
                if (random_chance < 0.35) {
                    m_world.spawn_enemy(ObjectType::ENEMY_RKN_WEAK);
                } else
                if (random_chance < 0.6) {
                    m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
                } else {
                    m_world.spawn_enemy(ObjectType::ENEMY_PACKAGE_WEAK);
                }
            } else {
                auto random_chance = ResourceManager::get_instance().random_chance();
                if (random_chance < 0.1) {
                    auto random_chance_grid = ResourceManager::get_instance().random_chance();
                    if (random_chance_grid < 0.2) {
                        std::vector<std::vector<int>> bit_grid = {
                            {0, 0, 1, 0, 0},
                            {0, 1, 0, 0, 0},
                            {1, 0, 0, 0, 0},
                            {0, 1, 0, 0, 0},
                            {0, 0, 1, 0, 0}
                        };
                        m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
                    } else
                    if (random_chance_grid < 0.4) {
                        std::vector<std::vector<int>> bit_grid = {
                            {0, 0, 1, 0, 1},
                            {0, 1, 1, 0, 1},
                            {1, 0, 1, 1, 1},
                            {0, 1, 1, 0, 1},
                            {0, 0, 1, 0, 1}
                        };
                        m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
                    } else
                    if (random_chance_grid < 0.6) {
                        std::vector<std::vector<int>> bit_grid = {
                            {0, 0, 0, 0, 0},
                            {0, 0, 1, 0, 0},
                            {1, 1, 1, 0, 0},
                            {0, 0, 1, 0, 0},
                            {0, 0, 0, 0, 0}
                        };
                        m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
                    } else
                    if (random_chance_grid < 0.8) {
                        std::vector<std::vector<int>> bit_grid = {
                            {1, 0, 1, 0, 1},
                            {1, 0, 1, 0, 1},
                            {1, 0, 1, 0, 1},
                            {1, 0, 1, 0, 1},
                            {1, 0, 1, 0, 1}
                        };
                        m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
                    } else {
                        std::vector<std::vector<int>> bit_grid = {
                            {1, 0, 1, 0, 1},
                            {0, 1, 0, 1, 0},
                            {0, 0, 1, 0, 0},
                            {0, 1, 0, 1, 0},
                            {1, 0, 1, 0, 1}
                        };
                        m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
                    }
                } else
                if (random_chance < 0.35) {
                    m_world.spawn_enemy(ObjectType::ENEMY_RKN_WEAK);
                } else
                if (random_chance < 0.6) {
                    m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
                } else {
                    m_world.spawn_enemy(ObjectType::ENEMY_PACKAGE_MITOSIS);
                }
            }
        }
    }

    // Генерация боеприпасов
    void generate_cash_ammo() {
        const double ammo_spawn_rate =
            m_game_progress < 100 ? 0.015 :
            m_game_progress < 200 ? 0.025 :
            m_game_progress < 300 ? 0.030 :
            m_game_progress < 400 ? 0.035 :
            0.04;

        if (ResourceManager::get_instance().random_chance() < ammo_spawn_rate) {
            m_world.spawn_cash(ObjectType::CASH_AMMO_300);
        }
    }

    void update_keys_pressed() {
        if (m_is_game_start && !m_is_game_over) {
            auto& objects = m_world.get_objects();

            for (auto& item : m_keys_pressed) {
                // Ищем объект типа PLANE
                PlaneObject* player = nullptr;
                for (const auto& obj : objects) {
                    if (obj->get_category() == ObjectCategory::PLANE) {
                        player = dynamic_cast<PlaneObject*>(obj.get());
                        if (player) {
                            break;
                        }
                    }
                }

                if (player) {
                    player->handle_input(item.first, item.second);
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

        // Отрисовываем объекты
        auto& objects = m_world.get_objects();
        for (auto& object : objects) {
            object->draw();
        }

        update_hud();
    }

    void update_hud() {
        auto& objects = m_world.get_objects();

        // Ищем объект типа PLANE
        PlaneObject* player = nullptr;
        for (const auto& obj : objects) {
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
                } else
                if (currentHearts > health) {
                    for (let i = currentHearts; i > health; i--) {
                        const lastChild = healthLabel.lastChild;
                        // Проверяем, чтобы не удалить текст "Здоровье:"
                        if (lastChild && lastChild.classList && lastChild.classList.contains('hud-heart')) {
                            healthLabel.removeChild(lastChild);
                        }
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


    void show_rkn_level_1() {
        m_dialogue_manager.add_dialogue(u8"Пытаешься увернуться от блокировок? Это бесполезно =)", "images/hero_rkn_1.png", "sound-dialogue-rkn-4", 2000);
        m_dialogue_manager.add_dialogue(u8"Посмотрим, как ты справишься с нашими ПАКЕТАМИ! =)", "images/hero_rkn_1.png", "sound-dialogue-rkn-5", 2500, [this](){
            std::vector<std::vector<int>> bit_grid = {
                {1, 1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_WEAK);
        });
        m_dialogue_manager.start();
    }

    void show_rkn_level_2() {
        m_dialogue_manager.add_dialogue(u8"От чего так в России...", "images/hero_rkn_1.png", "sound-dialogue-rkn-6", 2000);
        m_dialogue_manager.add_dialogue(u8"Пакеты шуршат... Ха-ха-ха! =)", "images/hero_rkn_1.png", "sound-dialogue-rkn-7", 3000, [this](){
            std::vector<std::vector<int>> bit_grid = {
                {1, 0, 1, 0, 1, 0},
                {1, 0, 1, 0, 1, 0},
                {1, 0, 1, 0, 1, 0}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_WEAK);
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_WEAK);
        });
        m_dialogue_manager.start();
    }

    void show_rkn_level_3() {
        m_dialogue_manager.add_dialogue(u8"Я и не таких блокировала!", "images/hero_rkn_1.png", "sound-dialogue-rkn-8", 4000, [this](){
            std::vector<std::vector<int>> bit_grid = {
                {1, 0, 1, 0, 1, 0},
                {1, 1, 0, 1, 0, 1},
                {1, 0, 1, 0, 1, 0}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_WEAK);
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_WEAK);

            std::vector<std::vector<int>> bit_grid_2 = {
                {0, 0, 1},
                {0, 1, 0},
                {1, 0, 1},
                {0, 1, 0},
                {0, 0, 1}
            };
            m_world.spawn_enemy(bit_grid_2, ObjectType::ENEMY_RKN_WEAK);
            m_world.spawn_enemy(bit_grid_2, ObjectType::ENEMY_RKN_WEAK);
        });
        m_dialogue_manager.start();
    }

    void show_rkn_level_4() {
        m_dialogue_manager.add_dialogue(u8"Да ты ЗАДОЛБАЛ! ЗАБАНЮ!!!", "images/hero_rkn_2.png", "sound-dialogue-rkn-9", 4000, [this](){
            for (int i = 0; i < 20; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_PACKAGE_WEAK);
            }
            for (int i = 0; i < 20; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_WEAK);
            }
        });
        m_dialogue_manager.start();
    }

	void show_rkn_level_5() {
        m_dialogue_manager.add_dialogue(u8"Ты уже пробовал яндекс браузер? НЕТ?! Тогда сейчас узнаешь Всю... Его... МОЩЬ!", "images/hero_rkn_2.png", "sound-dialogue-rkn-10", 4000, [this](){
            for (int i = 0; i < 20; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_PACKAGE_WEAK);
            }
            for (int i = 0; i < 20; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_WEAK);
            }
        });
        m_dialogue_manager.start();
    }

    void show_rkn_filler_1() {
        m_dialogue_manager.add_dialogue(u8"Отдай ключи шифрования, Дуров! А то будет а-та-та!", "images/hero_rkn_1.png", "sound-dialogue-rkn-filler-1", 3000,
            [this](){
        });
        m_dialogue_manager.start();
    }

    void show_rkn_filler_2() {
        m_dialogue_manager.add_dialogue(u8"It's блокировка time!", "images/hero_rkn_1.png", "sound-dialogue-rkn-filler-2", 3000, [this](){
            for (int i = 0; i < 3; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_WEAK);
            }
        });
        m_dialogue_manager.start();
    }

};
