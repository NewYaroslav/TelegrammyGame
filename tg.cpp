#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <emscripten.h>
#include <emscripten/bind.h>


#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

#define CELL_WIDTH          10
#define CELL_HEIGHT         10
#define CLOUD_WIDTH         200
#define CLOUD_HEIGHT        200
#define PLANE_WIDTH         200
#define PLANE_HEIGHT        80
#define BARREL_WIDTH        160
#define BARREL_HEIGHT       80

enum ObjectType {
    PLANE_ID = 0,
    CLOUD_ID = 1,
    BARREL_ID = 2,
    BULLET_ID = 3,
    EXPLOSION_ID = 4,
    BARREL_V2_ID = 5,
};

class Object {
public:
    int32_t x, y, type, w, h, param;
    Object(int32_t x, int32_t y, int32_t type, int32_t w, int32_t h, int32_t param = 0) :
        x(x), y(y), type(type), w(w), h(h), param(param) {
    }
};

class Game {
private:

    int max_width = 0, max_height = 0;
    int plane_x = 0, plane_y = 0;
    int start_plane_x = 0, start_plane_y = 0;
    int64_t tick = 0;
    int64_t shot_tick = 0;
    int64_t level = 0;
    std::vector<Object> objects = {Object(0, 0, ObjectType::PLANE_ID, PLANE_WIDTH, PLANE_HEIGHT)};
    std::map<int,bool> keys_pressed;
    bool game_over = false;
    bool game_start = false;

public:

    Game() = default;

    ~Game() {
        stop_background();
    }

    static Game& get_instance() {
        static Game instance;
        return instance;
    }

    void set_window(int width, int height) {
        max_width = width / CELL_WIDTH;
        max_height = height / CELL_HEIGHT;
        start_plane_x = plane_x = max_width / 3;
        start_plane_y = plane_y = max_height / 2;
        objects[0].x = start_plane_x;
        objects[0].y = start_plane_y;
        std::cout << "width " << width << std::endl;
        std::cout << "height " << height << std::endl;
        play_background();
    }

    void run() {
        update();
        draw();
    }

    void handle_keys_pressed() {
        if (!game_start) return;
        if (game_over) return;
        for (auto& item : keys_pressed) {
            switch (item.first) {
                case 'a':
                case 'A': // Left arrow
                case 37:
                    if (item.second && plane_x > 0) plane_x--;
                    break;
                case 'd':
                case 'D': // Right arrow
                case 39:
                    if (item.second && plane_x < (max_width - 1)) plane_x++;
                    break;
                case 'w':
                case 'W': // Up arrow
                case 38:
                    if (item.second && plane_y > 0) plane_y--;
                    break;
                case 's':
                case 'S': // Down arrow
                case 40:
                    if (item.second && plane_y < (max_height - 1)) plane_y++;
                    break;
                case 32: // Space bar
                    if (item.second) {
                        if (shot_tick == 0) {
                            objects.emplace_back(plane_x + 2, plane_y, ObjectType::BULLET_ID, CELL_WIDTH, CELL_HEIGHT, 1); // Fire bullet
                            play_shoot();
                            shot_tick++;
                        } else
                        if (shot_tick < 5) {
                            shot_tick++;
                        } else {
                            shot_tick = 0;
                        }
                    } else {
                        shot_tick = 0;
                    }
                    break;
                default:
                    break;
            }
        }
        objects[0].x = plane_x;
        objects[0].y = plane_y;
    }

    void handle_input(int ch, int mode) {
        keys_pressed[ch] = (mode == 1);

        if ((game_start || game_over) && mode == 1) {
            switch (ch) {
            case 'R':
            case 'r':
                objects.clear();
                objects = {Object(0, 0, ObjectType::PLANE_ID, PLANE_WIDTH, PLANE_HEIGHT)};
                objects[0].x = start_plane_x;
                objects[0].y = start_plane_y;
                game_over = false;
                game_start = true;
                level = 0;
                restart_game();
                break;
            case 81: // 'Q' key
                if (mode == 0) {
                    game_over = true;
                    end_game();
                }
                break;
            default:
                break;
            }
        }

        if (!game_start && mode == 1) {
            switch (ch) {
            case 32: // Space bar
                game_start = true;
                play_background();
                break;
            case 81: // 'Q' key
                if (mode == 0) {
                    game_over = true;
                    end_game();
                }
                break;
            default:
                break;
            }
        }
    }

private:

    bool has_no_barrel(const std::vector<Object>& objects) {
        for (const auto& obj : objects) {
            if (obj.type == BARREL_ID || obj.type == BARREL_V2_ID) {
                return false; // Найден объект типа BARREL_ID
            }
        }
        return true; // Не найдено объектов типа BARREL_ID
    }

    bool has_no_cloud(const std::vector<Object>& objects) {
        for (const auto& obj : objects) {
            if (obj.type == CLOUD_ID) {
                return false; // Найден объект типа BARREL_ID
            }
        }
        return true; // Не найдено объектов типа BARREL_ID
    }

    void update() {
        handle_keys_pressed();

        objects.erase(std::remove_if(objects.begin(), objects.end(), [this](Object &b) { return b.type == ObjectType::EXPLOSION_ID && b.param >= 5;}), objects.end());

        for (auto &object : objects) {
            switch(object.type) {
            case ObjectType::BARREL_ID:
                object.x -= 2;
                break;
            case ObjectType::BARREL_V2_ID:
                object.x -= 1;
                break;
            case ObjectType::BULLET_ID:
                object.x += 2;
                break;
            case ObjectType::CLOUD_ID:
                object.x -= (tick % 3 == 0) ? 1 : 0;
                break;
            case ObjectType::EXPLOSION_ID:
                object.param++;
                break;
            default:
                break;
            }
        }
        objects.erase(std::remove_if(objects.begin(), objects.end(), [this](Object &b) { return b.type == ObjectType::BARREL_ID && b.x < 0;}), objects.end());
        objects.erase(std::remove_if(objects.begin(), objects.end(), [this](Object &b) { return b.type == ObjectType::BARREL_V2_ID && b.x < 0;}), objects.end());
        objects.erase(std::remove_if(objects.begin(), objects.end(), [this](Object &b) { return b.type == ObjectType::BULLET_ID && b.x > max_width; }), objects.end());
        objects.erase(std::remove_if(objects.begin(), objects.end(), [this](Object &b) { return b.type == ObjectType::CLOUD_ID && b.x < -CLOUD_WIDTH; }), objects.end());

        for (auto& barrel: objects) {
            if (barrel.type != ObjectType::BARREL_ID && barrel.type != BARREL_V2_ID) continue;
            for (auto& bullet: objects) {
                if (bullet.type != ObjectType::BULLET_ID) continue;
                const int dist_x = (barrel.x - bullet.x) * CELL_WIDTH;
                const int dist_y = std::abs(barrel.y - bullet.y) * CELL_HEIGHT;
                if (dist_x <= 0 && dist_x > -BARREL_WIDTH && dist_y <= BARREL_HEIGHT) {
                    if (barrel.param <= 0) {
                        barrel.type = ObjectType::EXPLOSION_ID;
                        barrel.w = barrel.h = std::max(barrel.w, barrel.h);
                        play_plane_explosion();
                        --bullet.param;
                    } else {
                        --barrel.param;
                        --bullet.param;
                        play_explosion();
                    }
                    break;
                }
            }
        }

        objects.erase(std::remove_if(objects.begin(), objects.end(), [this](Object &b) { return b.type == ObjectType::BULLET_ID && b.param <= 0; }), objects.end());

        if (!objects.empty() && objects[0].type == ObjectType::PLANE_ID) {
            auto &plane = objects[0];
            for (auto& barrel: objects) {
                if (barrel.type != ObjectType::BARREL_ID && barrel.type != BARREL_V2_ID) continue;
                const int dist_x = (barrel.x - plane.x) * CELL_WIDTH;
                const int dist_y = std::abs(barrel.y - plane.y) * CELL_HEIGHT;
                if (dist_x < (PLANE_WIDTH/2) && dist_x > -(PLANE_WIDTH/2) && dist_y < PLANE_HEIGHT) {
                    plane.type = ObjectType::EXPLOSION_ID;
                    plane.w = plane.h = std::max(plane.w, plane.h);
                    play_plane_explosion();
                    game_over = true;
                    end_game();
                    break;
                }
            }

        }

        /*
        if (rand() % 100 < 5) {
            barrels.emplace_back(max_width, rand() % max_height);
        }
        */
        if (game_start) {
            int p_barrel_v1 = 2;
            int p_barrel_v2 = 1;

            if (level > 100) {
                p_barrel_v1 = 3;
                p_barrel_v2 = 1;
            } else
            if (level > 250) {
                p_barrel_v1 = 4;
                p_barrel_v2 = 2;
            } else
            if (level > 500) {
                p_barrel_v1 = 5;
                p_barrel_v2 = 2;
            } else
            if (level > 1000) {
                p_barrel_v1 = 6;
                p_barrel_v2 = 3;
            } else
            if (level > 5000) {
                p_barrel_v1 = 7;
                p_barrel_v2 = 4;
            } else
            if (level > 10000) {
                p_barrel_v1 = 8;
                p_barrel_v2 = 5;
            } else
            if (level > 25000) {
                p_barrel_v1 = 10;
                p_barrel_v2 = 5;
            } else
            if (level > 50000) {
                p_barrel_v1 = 15;
                p_barrel_v2 = 7;
            } else
            if (level > 100000) {
                p_barrel_v1 = 15;
                p_barrel_v2 = 10;
            } else
            if (level > 100000) {
                p_barrel_v1 = 20;
                p_barrel_v2 = 15;
            }

            if (has_no_barrel(objects) || rand() % 100 < p_barrel_v1) {
                objects.emplace_back(max_width, rand() % max_height, ObjectType::BARREL_ID, BARREL_WIDTH, BARREL_HEIGHT, 2);
                ++level;
            }
            if (has_no_barrel(objects) || rand() % 100 < p_barrel_v2) {
                objects.emplace_back(max_width, rand() % max_height, ObjectType::BARREL_V2_ID, BARREL_WIDTH, BARREL_HEIGHT, 5);
                ++level;
            }
        }

        if (rand() % 1000 < 10 || has_no_cloud(objects)) {
            const int wh = rand() % (CLOUD_WIDTH/2) + CLOUD_WIDTH/2;
            objects.emplace_back(max_width + CLOUD_WIDTH, rand() % (max_height/3), ObjectType::CLOUD_ID, wh, wh);
        }
        ++tick;
    }

    void play_background() {
        EM_ASM({
            const music = document.getElementById('background-music');
            console.log("background-music");
            if (music.paused) {
                music.play().catch(function(error) {
                    console.log("Error playing music: " + error);
                });
            }
        });
    }

    void play_shoot() {
        EM_ASM({
            const shootSound = document.getElementById('shoot-sound');
            shootSound.currentTime = 0; // Обнулить, чтобы звук начинался заново при каждом нажатии
            console.log("shoot-sound");
            shootSound.play().catch(function(error) {
                console.log("Error playing music: " + error);
            });
        });
    }

    void play_explosion() {
        EM_ASM({
            const shootSound = document.getElementById('explosion-sound');
            shootSound.currentTime = 0; // Обнулить, чтобы звук начинался заново при каждом нажатии
            console.log("explosion-sound");
            shootSound.play().catch(function(error) {
                console.log("Error playing music: " + error);
            });
        });
    }

    void play_plane_explosion() {
        EM_ASM({
            const shootSound = document.getElementById('plane-explosion-sound');
            shootSound.currentTime = 0; // Обнулить, чтобы звук начинался заново при каждом нажатии
            console.log("plane-explosion-sound");
            shootSound.play().catch(function(error) {
                console.log("Error playing music: " + error);
            });
        });
    }

    void stop_background() {
        EM_ASM({
            const music = document.getElementById('background-music');
            music.pause();
        });
    }

    void draw() {
        EM_ASM({
            const cellWidht = $0;
            const cellHeight = $1;

            const objectsDataPointer = $2;
            const objectsDataSize = $3;

            // Создаем Int32Array для данных
            const objectsDataArray = new Int32Array(HEAP32.buffer, objectsDataPointer, objectsDataSize / Int32Array.BYTES_PER_ELEMENT);

            const objects = [];
            for (let i = 0; i < objectsDataArray.length; i += 6) {
                objects.push({
                    x: objectsDataArray[i] * cellWidht,
                    y: objectsDataArray[i + 1] * cellHeight,
                    type: objectsDataArray[i + 2],
                    w: objectsDataArray[i + 3],
                    h: objectsDataArray[i + 4],
                    param: objectsDataArray[i + 5],
                });
            }

            // Очистка игрового пространства
            const gameArea = document.getElementById('game-area');
            /*
            const clouds = gameArea.querySelectorAll('.cloud');
            const explosions = gameArea.querySelectorAll('.explosion');
            const smokes = gameArea.querySelectorAll('.smoke');
            gameArea.innerHTML = '';
            clouds.forEach(cloud => gameArea.appendChild(cloud));
            explosions.forEach(explosion => gameArea.appendChild(explosion));
            smokes.forEach(smoke => gameArea.appendChild(smoke));
            */
            const elements = gameArea.children;
            for (let i = elements.length - 1; i >= 0; i--) {
                if (elements[i].classList.contains('plane')) {
                    gameArea.removeChild(elements[i]);
                } else
                if (elements[i].classList.contains('plane-body')) {
                    gameArea.removeChild(elements[i]);
                } else
                if (elements[i].classList.contains('barrel')) {
                    gameArea.removeChild(elements[i]);
                } else
                if (elements[i].classList.contains('barrel-v2')) {
                    gameArea.removeChild(elements[i]);
                } else
                if (elements[i].classList.contains('bullet')) {
                    gameArea.removeChild(elements[i]);
                } else
                if (elements[i].classList.contains('box')) {
                    gameArea.removeChild(elements[i]);
                } else
                if (elements[i].classList.contains('cloud')) {
                    gameArea.removeChild(elements[i]);
                }
            }

            for (let i = 0; i < objects.length; i++) {
                if (objects[i].type == 0) {
                    // Отображение самолета
                    const plane = document.createElement('div');
                    const plane_body = document.createElement('div');
                    plane.className = 'plane';
                    plane.style.left = objects[i].x + 'px';
                    plane.style.top = objects[i].y + 'px';

                    plane_body.className = 'plane-body';
                    plane_body.style.left = objects[i].x + 'px';
                    plane_body.style.top = objects[i].y + 'px';
                    gameArea.appendChild(plane_body);
                    gameArea.appendChild(plane);
                } else
                if (objects[i].type == 1) {
                    // отображение облаков
                    const cloud = document.createElement('div');
                    cloud.className = 'cloud';
                    cloud.style.left = objects[i].x + 'px';
                    cloud.style.top = objects[i].y + 'px';
                    cloud.style.width = objects[i].w + 'px';
                    cloud.style.height = objects[i].h + 'px';
                    gameArea.appendChild(cloud);
                } else
                if (objects[i].type == 2) {
                    // Отображение бочек
                    const barrel = document.createElement('div');
                    barrel.className = 'barrel';
                    barrel.style.left = objects[i].x - objects[i].w/2 + 'px';
                    barrel.style.top = objects[i].y - objects[i].h/2 + 'px';
                    gameArea.appendChild(barrel);
                } else
                if (objects[i].type == 5) {
                    // Отображение бочек
                    const barrel = document.createElement('div');
                    barrel.className = 'barrel-v2';
                    barrel.style.left = objects[i].x - objects[i].w/2 + 'px';
                    barrel.style.top = objects[i].y - objects[i].h/2 + 'px';
                    gameArea.appendChild(barrel);
                } else
                if (objects[i].type == 3) {
                    // Отображение пуль
                    const bullet = document.createElement('div');
                    bullet.className = 'bullet';
                    bullet.style.left = objects[i].x - objects[i].w/2 + 'px';
                    bullet.style.top = objects[i].y - objects[i].h/2 + 'px';
                    gameArea.appendChild(bullet);
                } else
                if (objects[i].type == 4) {
                    if (objects[i].param == 0) {
                        // Взрыв
                        const explosion = document.createElement('div');
                        explosion.className = 'explosion';
                        explosion.style.width = objects[i].w + 'px';
                        explosion.style.height = objects[i].h + 'px';
                        explosion.style.left = objects[i].x - objects[i].w/2 + 'px';
                        explosion.style.top = objects[i].y - objects[i].h/2 + 'px';
                        gameArea.appendChild(explosion);

                        const smoke = document.createElement('div');
                        smoke.className = 'smoke';
                        smoke.style.left = objects[i].x - objects[i].w/2 + 'px';
                        smoke.style.top = objects[i].y - objects[i].h/2 + 'px';
                        gameArea.appendChild(smoke);

                        // Запуск анимации взрыва
                        requestAnimationFrame(() => {
                            requestAnimationFrame(() => {
                                explosion.classList.add('animate');
                                smoke.classList.add('animate');
                            });
                        });

                        // Удаление элемента после завершения анимации
                        explosion.addEventListener('animationend', () => {
                            explosion.remove();
                        });

                        smoke.addEventListener('animationend', () => {
                            smoke.remove();
                        });
                    }
                }
                //const box = document.createElement('div');
                //box.className = 'box';
                //box.style.width = objects[i].w + 'px';
                //box.style.height = objects[i].h + 'px';
                //box.style.left = objects[i].x - objects[i].w/2 + 'px';
                //box.style.top = objects[i].y - objects[i].h/2 + 'px';
                //gameArea.appendChild(box);
            }
        },
        CELL_WIDTH, CELL_HEIGHT,
        reinterpret_cast<int32_t*>(objects.data()), objects.size() * sizeof(Object));
    }

    void end_game() {
        EM_ASM({
            document.getElementById('game-over').style.display = 'block';
        });
    }

    void restart_game() {
        EM_ASM({
            document.getElementById('game-over').style.display = 'none';
        });
    }
};


EXTERN EMSCRIPTEN_KEEPALIVE
void init_game(int width, int height) {
    Game::get_instance().set_window(width, height);
}

EXTERN EMSCRIPTEN_KEEPALIVE
void update_game() {
    Game::get_instance().run();
}

EXTERN EMSCRIPTEN_KEEPALIVE
void on_key_press(int key, int mode) {
    Game::get_instance().handle_input(key, mode);
}

EXTERN EMSCRIPTEN_KEEPALIVE
void cleanup_game() {

}

