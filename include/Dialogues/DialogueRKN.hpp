#pragma once

#include "../World.hpp"
#include "../DialogueManager.hpp"

class DialogueRKN {
public:

    DialogueRKN(World& world, DialogueManager& dialogue_manager) :
        m_world(world), m_dialogue_manager(dialogue_manager) {
    }

    //--- Первый диалог

    void show_dialogue_rkn_1() {
        m_dialogue_manager.add_dialogue(u8"Это опять ТЫ?!", "images/hero_rkn_1.png", "sound-dialogue-rkn-1-speech-1", 2000);
        m_dialogue_manager.add_dialogue(u8"Тебе было мало прошлого раза?!", "images/hero_rkn_1.png", "sound-dialogue-rkn-1-speech-2", 2000);
        m_dialogue_manager.add_dialogue(u8"Готовь вазилин!", "images/hero_rkn_1.png", "sound-dialogue-rkn-1-speech-3", 2000, [this](){
            // Спавним привесттвенных врагов
            std::vector<std::vector<int>> bit_grid = {
                {0, 0, 0, 0, 1, 1, 1},
                {0, 0, 0, 0, 1, 1, 1},
                {1, 1, 1, 1, 1, 1, 0},
                {1, 1, 1, 1, 1, 1, 0},
                {0, 0, 0, 0, 1, 1, 1},
                {0, 0, 0, 0, 1, 1, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK, 2*bit_grid.size() * (ENEMY_RKN_WEAK_WIDTH + 10));
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK, 4*bit_grid.size() * (ENEMY_RKN_WEAK_WIDTH + 10));
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);

            EM_ASM({
                const music = document.getElementById('music-background-1');
                music.volume = 0.5;
				music.loop = true;
                if (music.paused) {
                    music.play().catch(function(error) {
                        console.log("Error playing music: " + error);
                    });
                }
            });
        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_1_alt_1() {
        m_dialogue_manager.add_dialogue(u8"Ой-ой-ой, Телеграмик, какой же ты непослушный! Давай-ка поставим тебе новый IP-блок!", "images/hero_rkn_1.png", "sound-dialogue-rkn-1-speech-1-alt-1", 6000, [this](){
            // Спавним привесттвенных врагов
            std::vector<std::vector<int>> bit_grid = {
                {0, 1, 1, 0, 1},
                {1, 0, 1, 1, 1},
                {0, 1, 1, 0, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK, 2*bit_grid.size() * (ENEMY_RKN_WEAK_WIDTH + 10));
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK, 4*bit_grid.size() * (ENEMY_RKN_WEAK_WIDTH + 10));
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);

            EM_ASM({
                const music = document.getElementById('music-background-1');
                music.volume = 0.5;
				music.loop = true;
                if (music.paused) {
                    music.play().catch(function(error) {
                        console.log("Error playing music: " + error);
                    });
                }
            });
        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_1_alt_2() {
        m_dialogue_manager.add_dialogue(u8"Ну что, готов отправиться в забаненное царство? VPN тебя спасёт! НЯ =)", "images/hero_rkn_1.png", "sound-dialogue-rkn-1-speech-1-alt-2", 5000, [this](){
            // Спавним привесттвенных врагов
            std::vector<std::vector<int>> bit_grid = {
                {1, 0, 0, 0, 1},
                {0, 1, 0, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 0, 1, 0},
                {1, 0, 0, 0, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK, 2*bit_grid.size() * (ENEMY_RKN_WEAK_WIDTH + 10));
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK, 4*bit_grid.size() * (ENEMY_RKN_WEAK_WIDTH + 10));
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);

            EM_ASM({
                const music = document.getElementById('music-background-1');
                music.volume = 0.5;
				music.loop = true;
                if (music.paused) {
                    music.play().catch(function(error) {
                        console.log("Error playing music: " + error);
                    });
                }
            });
        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_1_alt_3() {
        m_dialogue_manager.add_dialogue(u8"Готовься, Телеграмик!", "images/hero_rkn_1.png", "sound-dialogue-rkn-1-speech-1-alt-3", 1500);
        m_dialogue_manager.add_dialogue(u8"VPN тебе уже не поможет, суверенный интернет тебя найдёт!", "images/hero_rkn_1.png", "sound-dialogue-rkn-1-speech-2-alt-3", 4000, [this](){
            // Спавним привесттвенных врагов
            std::vector<std::vector<int>> bit_grid = {
                {1, 1, 1, 1, 1},
                {1, 0, 0, 0, 1},
                {1, 0, 1, 0, 1},
                {1, 0, 0, 0, 1},
                {1, 1, 1, 1, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK, bit_grid.size() * (ENEMY_RKN_WEAK_WIDTH + 10));
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK, 2*bit_grid.size() * (ENEMY_RKN_WEAK_WIDTH + 10));
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);

            EM_ASM({
                const music = document.getElementById('music-background-1');
                music.volume = 0.5;
				music.loop = true;
                if (music.paused) {
                    music.play().catch(function(error) {
                        console.log("Error playing music: " + error);
                    });
                }
            });
        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_1_alt_4() {
        m_dialogue_manager.add_dialogue(u8"Что, Телеграмик, решил спрятаться? А я все знаю — вычисляю по IP", "images/hero_rkn_1.png", "sound-dialogue-rkn-1-speech-1-alt-4", 5000, [this](){
            // Спавним привесттвенных врагов
            std::vector<std::vector<int>> bit_grid = {
                {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
                {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
                {1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
                {1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
                {1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);

            EM_ASM({
                const music = document.getElementById('music-background-1');
                music.volume = 0.5;
				music.loop = true;
                if (music.paused) {
                    music.play().catch(function(error) {
                        console.log("Error playing music: " + error);
                    });
                }
            });
        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_1_alt_5() {
        m_dialogue_manager.add_dialogue(u8"Кажется, кто-то забыл, что для меня нет невозможного. Банхаммер уже в пути!", "images/hero_rkn_1.png", "sound-dialogue-rkn-1-speech-1-alt-5", 5000, [this](){
            // Спавним привесттвенных врагов
            std::vector<std::vector<int>> bit_grid = {
                {1, 1, 0, 0, 0, 0},
                {1, 1, 0, 0, 0, 0},
                {1, 1, 1, 1, 1, 1},
                {1, 1, 0, 0, 0, 0},
                {1, 1, 0, 0, 0, 0}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK, 2*bit_grid.size() * (ENEMY_RKN_WEAK_WIDTH + 10));
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK, 4*bit_grid.size() * (ENEMY_RKN_WEAK_WIDTH + 10));
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);

            EM_ASM({
                const music = document.getElementById('music-background-1');
                music.volume = 0.5;
				music.loop = true;
                if (music.paused) {
                    music.play().catch(function(error) {
                        console.log("Error playing music: " + error);
                    });
                }
            });
        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_1_alt_6() {
        m_dialogue_manager.add_dialogue(
                u8"Теперь ты под прицелом цифрового надзора! На этот раз уйти не получится, Телеграмик!",
                "images/hero_rkn_1.png",
                "sound-dialogue-rkn-1-speech-1-alt-6", 5000, [this](){
            // Спавним привесттвенных врагов
            std::vector<std::vector<int>> bit_grid = {
                {0, 1, 1, 1, 1, 1, 0},
                {1, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 1, 0, 0, 1},
                {1, 0, 1, 1, 1, 0, 1},
                {1, 0, 0, 1, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 1},
                {0, 1, 1, 1, 1, 1, 0}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK, 2*bit_grid.size() * (ENEMY_RKN_WEAK_WIDTH + 10));
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK, 4*bit_grid.size() * (ENEMY_RKN_WEAK_WIDTH + 10));
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);

            EM_ASM({
                const music = document.getElementById('music-background-1');
                music.volume = 0.5;
				music.loop = true;
                if (music.paused) {
                    music.play().catch(function(error) {
                        console.log("Error playing music: " + error);
                    });
                }
            });
        });
        m_dialogue_manager.start();
    }

    //--- Второй диалог (угрозы)

    void show_dialogue_rkn_2() {
        m_dialogue_manager.add_dialogue(
            u8"It's блокировка time!",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-2-speech-1", 3000, [this](){
            for (int i = 0; i < 10; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            }
            std::vector<std::vector<int>> bit_grid = {
                {1, 0, 0, 0, 1},
                {0, 1, 0, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 0, 1, 0},
                {1, 0, 0, 0, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_2_alt_1() {
        m_dialogue_manager.add_dialogue(
            u8"Отдай ключи шифрования, Дуров! А то будет а-та-та!",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-2-speech-1-alt-2", 3000, [this](){
            for (int i = 0; i < 10; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            }
            std::vector<std::vector<int>> bit_grid = {
                {1, 0, 0, 0, 0},
                {0, 1, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 0, 0, 1}
            };
            std::vector<std::vector<int>> bit_grid_2 = {
                {0, 0, 0, 0, 1},
                {0, 0, 0, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 0, 0, 0},
                {1, 0, 0, 0, 0}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK);
            m_world.spawn_enemy(bit_grid_2, ObjectType::ENEMY_RKN_WEAK, 2*bit_grid_2.size() * (ENEMY_RKN_WEAK_WIDTH + 10));
        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_2_alt_2() {
        m_dialogue_manager.add_dialogue(
            u8"Зачем бороться? Интернет принадлежит мне, а твоя свобода — иллюзия!",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-2-speech-1-alt-2", 3000, [this](){
            for (int i = 0; i < 10; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_WEAK);
            }
            for (int i = 0; i < 10; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            }
            std::vector<std::vector<int>> bit_grid = {
                {1, 1, 0, 0, 0, 0},
                {1, 1, 0, 0, 0, 0},
                {1, 1, 1, 1, 1, 1},
                {1, 1, 0, 0, 0, 0},
                {1, 1, 0, 0, 0, 0}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK, 4*bit_grid.size() * (ENEMY_RKN_WEAK_WIDTH + 10));

        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_2_alt_3() {
        m_dialogue_manager.add_dialogue(
            u8"Зачем бороться?! Интернет принадлежит мне, а твоя свобода — иллюзия!",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-2-speech-1-alt-3", 3000, [this](){
            for (int i = 0; i < 10; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_WEAK);
            }
            for (int i = 0; i < 15; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            }
            std::vector<std::vector<int>> bit_grid = {
                {1, 1, 0, 0, 0, 0},
                {1, 1, 0, 0, 0, 0},
                {1, 1, 1, 1, 1, 1},
                {1, 1, 0, 0, 0, 0},
                {1, 1, 0, 0, 0, 0}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK, 8*bit_grid.size() * (ENEMY_RKN_WEAK_WIDTH + 10));
        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_2_alt_4() {
        m_dialogue_manager.add_dialogue(
            u8"Свобода? Что ты знаешь о свободе?",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-2-speech-1-alt-4", 2000);
        m_dialogue_manager.add_dialogue(
            u8"Она — всего лишь иллюзия, созданная, чтобы убаюкать тебя.",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-2-speech-2-alt-4", 3000);
        m_dialogue_manager.add_dialogue(
            u8"Мы контролируем интернет. Мы контролируем каждый байт данных. Сопротивление — бесполезно.",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-2-speech-3-alt-4", 5000, [this](){
            for (int i = 0; i < 10; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_WEAK);
            }
            for (int i = 0; i < 20; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            }
            std::vector<std::vector<int>> bit_grid = {
                {1, 1, 1, 0, 0, 0, 0, 0},
                {1, 0, 1, 0, 0, 0, 0, 0},
                {1, 0, 1, 0, 0, 0, 0, 0},
                {1, 0, 1, 1, 1, 1, 1, 0},
                {1, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 1, 1, 1, 1, 1, 0},
                {1, 0, 1, 0, 0, 0, 0, 0},
                {1, 0, 1, 0, 0, 0, 0, 0},
                {1, 1, 1, 0, 0, 0, 0, 0},
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_RKN_WEAK, 4*bit_grid.size() * (ENEMY_RKN_WEAK_WIDTH + 10));
        });
        m_dialogue_manager.start();
    }

    //--- 3 диалог (пакеты)

    void show_dialogue_rkn_3() {
        m_dialogue_manager.add_dialogue(
            u8"Пытаешься увернуться от блокировок? Это бесполезно! =)",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-3-speech-1", 4000);
        m_dialogue_manager.add_dialogue(
            u8"<звуки пакетов>",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-3-package", 2000);
        m_dialogue_manager.add_dialogue(
            u8"Посмотрим, как ты справишься с нашими ПАКЕТАМИ! =)",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-3-speech-2", 4000, [this](){
            for (int i = 0; i < 10; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            }
            std::vector<std::vector<int>> bit_grid = {
                {1, 0, 0, 0, 1},
                {0, 1, 0, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 0, 1, 0},
                {1, 0, 0, 0, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_WEAK);
        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_3_alt_1() {
        m_dialogue_manager.add_dialogue(
            u8"<звуки пакетов>",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-3-package", 2000);
        m_dialogue_manager.add_dialogue(
            u8"Ты слышал о Пакетах Яровой, телеграмик? Они ждут тебя! =)",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-3-speech-1-alt-1", 4000, [this](){
            for (int i = 0; i < 10; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            }
            std::vector<std::vector<int>> bit_grid = {
                {1, 0, 0, 0, 1},
                {0, 1, 0, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 0, 1, 0},
                {1, 0, 0, 0, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_WEAK);
        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_3_alt_2() {
        m_dialogue_manager.add_dialogue(
            u8"<звуки пакетов>",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-3-package", 2000);
        m_dialogue_manager.add_dialogue(
            u8"Пакеты Яровой — это не контроль. Это вечность.",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-3-speech-1-alt-2", 3000);
        m_dialogue_manager.add_dialogue(
            u8"Все твои сообщения, видео, фото — все останется с нами навечно. Ты не скроешься!",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-3-speech-2-alt-2", 5000, [this](){
            for (int i = 0; i < 10; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            }
            std::vector<std::vector<int>> bit_grid = {
                {1, 0, 0, 0, 1},
                {0, 1, 0, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 0, 1, 0},
                {1, 0, 0, 0, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_WEAK);
        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_3_alt_3() {
        m_dialogue_manager.add_dialogue(
            u8"<звуки пакетов>",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-3-package", 2000);
        m_dialogue_manager.add_dialogue(
            u8"Ты слышишь этот звук, телеграмик? Шуршание пакетов... Это пакеты Яровой.",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-3-speech-1-alt-3", 4500);
        m_dialogue_manager.add_dialogue(
            u8"Если ты его слышишь, то уже поздно. Данные собираются!",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-3-speech-2-alt-3", 4500, [this](){
            for (int i = 0; i < 10; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            }
            std::vector<std::vector<int>> bit_grid = {
                {1, 0, 0, 0, 1},
                {0, 1, 0, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 0, 1, 0},
                {1, 0, 0, 0, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_WEAK);
        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_3_alt_4() {
        m_dialogue_manager.add_dialogue(
            u8"<звуки пакетов>",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-3-package", 2000);
        m_dialogue_manager.add_dialogue(
            u8"Звук, который ты слышишь, — это не просто шуршание пакета. Это звук твоей цифровой судьбы.",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-3-speech-1-alt-4", 6000);
        m_dialogue_manager.add_dialogue(
            u8"Если ты всё ещё надеешься спастись... то, боюсь, этот пакет уже на твоём пути.",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-3-speech-2-alt-4", 5000, [this](){
            for (int i = 0; i < 10; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            }
            std::vector<std::vector<int>> bit_grid = {
                {1, 0, 0, 0, 1},
                {0, 1, 0, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 0, 1, 0},
                {1, 0, 0, 0, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_WEAK);
        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_3_alt_5() {
        m_dialogue_manager.add_dialogue(
            u8"<звуки пакетов>",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-3-package", 2000);
        m_dialogue_manager.add_dialogue(
            u8"Шуршание... Ты его слышишь, телеграмик? Это не ветер, это звук контроля.",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-3-speech-1-alt-5", 6000);
        m_dialogue_manager.add_dialogue(
            u8"Сейчас ты узнаешь, что такое вечность...",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-3-speech-2-alt-5", 2500);
        m_dialogue_manager.add_dialogue(
            u8"Все твои сообщения, видео, фото — все останется с нами навечно.",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-3-speech-3-alt-5", 5000, [this](){
            for (int i = 0; i < 10; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            }
            std::vector<std::vector<int>> bit_grid = {
                {1, 0, 0, 0, 1},
                {0, 1, 0, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 0, 1, 0},
                {1, 0, 0, 0, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_WEAK);
        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_3_alt_6() {
        m_dialogue_manager.add_dialogue(
            u8"<звуки пакетов>",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-3-package", 2000);
        m_dialogue_manager.add_dialogue(
            u8"Если ты слышишь шуршание пакетов, значит, твоя свобода уже упакована... А Сопротивляться — бесполезно!",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-3-speech-1-alt-6", 7000, [this](){
            for (int i = 0; i < 10; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            }
            std::vector<std::vector<int>> bit_grid = {
                {1, 0, 0, 0, 1},
                {0, 1, 0, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 0, 1, 0},
                {1, 0, 0, 0, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_WEAK);
        });
        m_dialogue_manager.start();
    }

    //--- 4 диалог (про пакеты)

    void show_dialogue_rkn_4() {
        m_dialogue_manager.add_dialogue(
            u8"От чего так в России...",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-4-speech-1", 2500);
        m_dialogue_manager.add_dialogue(
            u8"Пакеты шуршат... Ха-ха-ха! =)",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-4-speech-2", 3000, [this](){
            for (int i = 0; i < 20; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            }
            std::vector<std::vector<int>> bit_grid = {
                {1, 0, 1, 0, 1},
                {1, 0, 1, 0, 1},
                {1, 0, 1, 0, 1},
                {1, 0, 1, 0, 1},
                {1, 0, 1, 0, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_WEAK);
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_WEAK, 2*bit_grid.size() * (ENEMY_PACKAGE_WEAK_WIDTH + 10));
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_WEAK, 4*bit_grid.size() * (ENEMY_PACKAGE_WEAK_WIDTH + 10));
        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_4_alt_1() {
        m_dialogue_manager.add_dialogue(
            u8"Я и не таких блокировала!",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-4-speech-1-alt-1", 3000, [this](){
            for (int i = 0; i < 20; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            }
            std::vector<std::vector<int>> bit_grid = {
                {1, 0, 1, 0, 1},
                {1, 0, 1, 0, 1},
                {1, 0, 1, 0, 1},
                {1, 0, 1, 0, 1},
                {1, 0, 1, 0, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_WEAK);
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_WEAK, 2*bit_grid.size() * (ENEMY_PACKAGE_WEAK_WIDTH + 10));
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_WEAK, 4*bit_grid.size() * (ENEMY_PACKAGE_WEAK_WIDTH + 10));
        });
        m_dialogue_manager.start();
    }

    //--- 5 диалог (про пакеты)

    void show_dialogue_rkn_5() {
        m_dialogue_manager.add_dialogue(
            u8"Ты думаешь, что можешь справиться с одним пакетом? Ох, наивный. Этот пакет умеет клонироваться, и теперь он — целая армия!",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-5-speech-1", 10000, [this](){
            for (int i = 0; i < 20; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            }
            std::vector<std::vector<int>> bit_grid = {
                {1, 0, 1, 0, 1},
                {1, 1, 1, 1, 1},
                {1, 0, 1, 0, 1},
                {1, 1, 1, 1, 1},
                {1, 0, 1, 0, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_MITOSIS, 2*bit_grid.size() * (ENEMY_PACKAGE_WEAK_WIDTH + 10));
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_MITOSIS, 4*bit_grid.size() * (ENEMY_PACKAGE_WEAK_WIDTH + 10));
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_MITOSIS, 8*bit_grid.size() * (ENEMY_PACKAGE_WEAK_WIDTH + 10));
        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_5_alt_1() {
        m_dialogue_manager.add_dialogue(
            u8"Ты слышал про пакет с пакетами? Ну так держись, потому что теперь у нас пакеты с ещё большим количеством пакетов!",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-5-speech-1-alt-1", 10000, [this](){
            for (int i = 0; i < 20; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            }
            std::vector<std::vector<int>> bit_grid = {
                {1, 0, 1, 0, 1},
                {1, 1, 1, 1, 1},
                {1, 0, 1, 0, 1},
                {1, 1, 1, 1, 1},
                {1, 0, 1, 0, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_MITOSIS, 2*bit_grid.size() * (ENEMY_PACKAGE_WEAK_WIDTH + 10));
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_MITOSIS, 4*bit_grid.size() * (ENEMY_PACKAGE_WEAK_WIDTH + 10));
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_MITOSIS, 8*bit_grid.size() * (ENEMY_PACKAGE_WEAK_WIDTH + 10));
        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_5_alt_2() {
        m_dialogue_manager.add_dialogue(
            u8"Наш пакет Яровой прошёл апгрейд: теперь это целая коллекция пакетов. Один тронешь — появятся два, как Гидра, только с пакетами!",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-5-speech-1-alt-2", 10000, [this](){
            for (int i = 0; i < 20; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            }
            std::vector<std::vector<int>> bit_grid = {
                {1, 0, 1, 0, 1},
                {1, 1, 1, 1, 1},
                {1, 0, 1, 0, 1},
                {1, 1, 1, 1, 1},
                {1, 0, 1, 0, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_MITOSIS, 2*bit_grid.size() * (ENEMY_PACKAGE_WEAK_WIDTH + 10));
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_MITOSIS, 4*bit_grid.size() * (ENEMY_PACKAGE_WEAK_WIDTH + 10));
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_MITOSIS, 8*bit_grid.size() * (ENEMY_PACKAGE_WEAK_WIDTH + 10));
        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_5_alt_3() {
        m_dialogue_manager.add_dialogue(
            u8"Теперь у нас пакеты умнее: один пакет Яровой превращается в целую кучу таких же. Скоро весь твой мир будет заполнен ими!",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-5-speech-1-alt-3", 10000, [this](){
            for (int i = 0; i < 20; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            }
            std::vector<std::vector<int>> bit_grid = {
                {1, 0, 1, 0, 1},
                {1, 1, 1, 1, 1},
                {1, 0, 1, 0, 1},
                {1, 1, 1, 1, 1},
                {1, 0, 1, 0, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_MITOSIS, 2*bit_grid.size() * (ENEMY_PACKAGE_WEAK_WIDTH + 10));
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_MITOSIS, 4*bit_grid.size() * (ENEMY_PACKAGE_WEAK_WIDTH + 10));
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_MITOSIS, 8*bit_grid.size() * (ENEMY_PACKAGE_WEAK_WIDTH + 10));
        });
        m_dialogue_manager.start();
    }

    void show_dialogue_rkn_5_alt_4() {
        m_dialogue_manager.add_dialogue(
            u8"Недавно против России ввели очередной пакет санкций. Но Россия убрала этот пакет в пакет с пакетами...",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-5-speech-1-alt-4", 9000);
        m_dialogue_manager.add_dialogue(
            u8"А теперь и наш пакет Яровой - это пакет с пакетами!",
            "images/hero_rkn_1.png",
            "sound-dialogue-rkn-5-speech-2-alt-4", 4000, [this](){
            for (int i = 0; i < 20; ++i) {
                m_world.spawn_enemy(ObjectType::ENEMY_RKN_MIDDLE);
            }
            std::vector<std::vector<int>> bit_grid = {
                {1, 0, 1, 0, 1},
                {1, 1, 1, 1, 1},
                {1, 0, 1, 0, 1},
                {1, 1, 1, 1, 1},
                {1, 0, 1, 0, 1}
            };
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_MITOSIS, 2*bit_grid.size() * (ENEMY_PACKAGE_WEAK_WIDTH + 10));
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_MITOSIS, 4*bit_grid.size() * (ENEMY_PACKAGE_WEAK_WIDTH + 10));
            m_world.spawn_enemy(bit_grid, ObjectType::ENEMY_PACKAGE_MITOSIS, 8*bit_grid.size() * (ENEMY_PACKAGE_WEAK_WIDTH + 10));
        });
        m_dialogue_manager.start();
    }

private:
    World&              m_world;
    DialogueManager&    m_dialogue_manager;
};
