#pragma once

#include "BaseObject.hpp"

#define PLANE_WIDTH             180
#define PLANE_HEIGHT            60
#define PLANE_HP                1
#define PLANE_SPEED             15

class PlaneObject final : public BaseObject {
public:
    int32_t current_ammo = 300;

    PlaneObject(
            const int32_t& x,
            const int32_t& y,
            const std::function<void(BaseObject*, const ObjectType&)>& on_spawn) :
        BaseObject(x, y,
            PLANE_WIDTH,
            PLANE_HEIGHT,
            PLANE_HP,
            PLANE_SPEED,
            0.0f, std::string(), on_spawn),
        m_plane_x(x), m_plane_y(y) {
    }

    virtual ~PlaneObject() {}

    ObjectCategory get_category() override final {
        return ObjectCategory::PLANE;
    }

    ObjectType get_type() override final {
        return ObjectType::PLANE;
    }

    bool is_online() override final {
        return true;
    }

    bool is_destroy() override final {
        if (hp <= 0) {
            return m_dive_animation_step == 8;
        }
        return false;
    }

    bool is_stop_game() {
        return hp <= 0;
    }

    void handle_input(const int& ch, const bool& is_key_press) {
        ResourceManager& instance = ResourceManager::get_instance();

        const int32_t max_width = instance.get_width();
        const int32_t max_height = instance.get_height();

        switch (ch) {
            case 'a':
            case 'A': // Left arrow
            case 37:
                if (is_key_press && m_plane_x > 0) m_plane_x -= speed;
                break;
            case 'd':
            case 'D': // Right arrow
            case 39:
                if (is_key_press && m_plane_x < (max_width - 1)) m_plane_x += speed;
                break;
            case 'w':
            case 'W': // Up arrow
            case 38:
                if (is_key_press && m_plane_y > 0) m_plane_y -= speed;
                break;
            case 's':
            case 'S': // Down arrow
            case 40:
                if (is_key_press && m_plane_y < (max_height - 1)) m_plane_y += speed;
                break;
            case 32: // Space bar
                if (is_key_press) {
                    if (current_ammo > 0) {
                        if (m_shot_tick == 0) {
                            //callback(x, y);
                            if (on_spawn) on_spawn(this, ObjectType::BULLET_DOLLAR);
                            play_shoot();
                            m_shot_tick++;
                            current_ammo -= 100;
                        } else
                        if (m_shot_tick < 5) {
                            m_shot_tick++;
                        } else {
                            m_shot_tick = 0;
                        }
                    } else {
                        m_shot_tick = 0;
                    }
                } else {
                    m_shot_tick = 0;
                }
                break;
            default:
                break;
        }
    }

    void damage(const int32_t& change_hp) override final {
        if (hp > 0) {
            hp -= std::abs(change_hp);
            play_damage();
            if (hp <= 0) {
                if (on_spawn) on_spawn(this, ObjectType::EXPLOSION_SMALL);
                play_dive();
            }
        }
    }

    void update() override final {
        if (hp <= 0) {
            const int32_t min_height = 3 * ResourceManager::get_instance().get_height() / 4;
            const int32_t max_height = 1 * ResourceManager::get_instance().get_height() / 6;

            static const double period = 2.5;
            static const double sin_step = 2.0 * std::acos(-1) / period;
            static const double max_offset = 10.0;
            const double offset_angle = std::sin(sin_step * m_angle_swing);
            m_angle_swing += 0.05;
            if (m_angle_swing >= period) m_angle_swing = 0.0;

            // Анимация падения самолета
            switch (m_dive_animation_step) {
            case 0:
                if (y > max_height) {
                    y -= (y - max_height) / 10;
                    y -= 1;
                    x -= 1;
                    if (m_angle > -300.0) m_angle += -5.0;
                    else m_dive_animation_step = 1;
                } else m_dive_animation_step = 2;
                break;
            case 1:
                if (y > max_height) {
                    y -= (y - max_height) / 5;
                    y -= 1;
                    x -= 1;
                    m_angle = -300.0 + 10.0 * offset_angle;
                } else m_dive_animation_step = 2;
                break;
            case 2:
                if (y < min_height) {
                    y += 5;
                    x += 1;
                    if (m_angle > -360.0) m_angle += -2.5;
                    else m_dive_animation_step = 3;
                } else m_dive_animation_step = 4;
                break;
            case 3:
                if (y < min_height) {
                    y += 5;
                    x += 1;
                    m_angle = -360.0 + 5.0 * offset_angle;
                } else m_dive_animation_step = 4;
                break;
            // вторая петля
            case 4:
                if (y > max_height) {
                    const int32_t speed_y = (y - max_height) / 10;
                    y -= std::min(speed_y, 10);
                    y -= 1;
                    x += 2;
                    if (m_angle > -660.0) m_angle += -2.5;
                    else m_dive_animation_step = 5;
                } else m_dive_animation_step = 6;
                break;
            case 5:
                if (y > max_height) {
                    const int32_t speed_y = (y - max_height) / 10;
                    y -= std::min(speed_y, 5);
                    y -= 1;
                    x += 1;
                    m_angle = -660.0 + 10.0 * offset_angle;
                } else m_dive_animation_step = 6;
                break;
            case 6:
                if (y < min_height) {
                    y += 5;
                    x += 3;
                    if (m_angle > -660.0) m_angle += -5.0;
                    else m_dive_animation_step = 7;
                } else {
                    if (on_spawn) on_spawn(this, ObjectType::EXPLOSION_LARGE);
                    m_dive_animation_step = 8;
                }
                break;
            case 7:
                if (y < min_height) {
                    y += 10;
                    x += 4;
                    m_angle = -660.0 + 10.0 * offset_angle;
                } else {
                    if (on_spawn) on_spawn(this, ObjectType::EXPLOSION_LARGE);
                    m_dive_animation_step = 8;
                }
                break;
            default:
                break;
            }

            if (m_dive_explosion_step > 30) {
                if (on_spawn) on_spawn(this, ObjectType::EXPLOSION_SMALL);
                m_dive_explosion_step = 0;
            } else ++m_dive_explosion_step;

            return;
        }

        static const double period = 2.5;
        static const double sin_step = 2.0 * std::acos(-1) / period;
        static const double max_offset = 10.0;
        const int32_t offset = (int32_t)(max_offset * std::sin(sin_step * m_height_swing));

        m_height_swing += 0.05;
        if (m_height_swing >= period) m_height_swing = 0.0;
        y = m_plane_y + offset;
        x = m_plane_x;
    }

    void draw() override final {
        EM_ASM({
            const obj_id = $0;
            const obj_x = $1;
            const obj_y = $2;;
            const obj_width  = $3;
            const obj_height = $4;
            const obj_angle = $5;
            const is_debug = $6;

            // Отображение самолета
            const existing_plane = document.getElementById(`obj_${obj_id}`);
            if (existing_plane) {
                // Обновляем положение объекта
                existing_plane.style.left = `${obj_x - obj_width / 2}px`;
                existing_plane.style.top = `${obj_y - obj_height / 2}px`;
                existing_plane.style.transform = `rotate(${obj_angle}deg)`;
            } else {
                // Создаем новый объект, если он не существует
                const plane = document.createElement('div');
                plane.className = 'plane';
                plane.id = `obj_${obj_id}`;
                plane.style.left = `${obj_x - obj_width / 2}px`;
                plane.style.top = `${obj_y - obj_height / 2}px`;
                plane.style.width = `${obj_width}px`;
                plane.style.height = `${obj_height}px`;

                const airflow = document.createElement('div');
                airflow.className = 'airflow';
                plane.appendChild(airflow);

                const plane_body = document.createElement('div');
                plane_body.className = 'plane-body';
                plane.appendChild(plane_body);

                const plane_wings = document.createElement('div');
                plane_wings.className = 'plane-wings';
                plane.appendChild(plane_wings);

                document.getElementById('game-area').appendChild(plane);

                console.log("create plane");
            }

            if (is_debug) {
                const box = document.createElement('div');
                box.className = 'box';
                box.style.left = `${obj_x - obj_width / 2}px`;
                box.style.top = `${obj_y - obj_height / 2}px`;
                box.style.width = `${obj_width}px`;
                box.style.height = `${obj_height}px`;
                box.style.transform = `rotate(${obj_angle}deg)`;

                document.getElementById('game-area').appendChild(box);
            }
        },
        id, x, y, width, height, m_angle, (bool)GAME_DEBUG);
    }

private:
    double  m_height_swing = 0;
    double  m_angle_swing = 0;
    float   m_angle = 0;
    int32_t m_shot_tick = 0;
    int32_t m_plane_x = 0;
    int32_t m_plane_y = 0;
    int32_t m_dive_animation_step = 0;
    int32_t m_dive_explosion_step = 0;

    void play_shoot() {
        EM_ASM({
            const shootSound = document.getElementById('sound-shoot-dollar');
            shootSound.currentTime = 0; // Обнулить, чтобы звук начинался заново при каждом нажатии
            shootSound.play().catch(function(error) {
                console.log("Error playing music: " + error);
            });
        });
    }

    void play_damage() {
        EM_ASM({
            const damageSound = document.getElementById('sound-plane-damage');
            damageSound.currentTime = 0;
            damageSound.play().catch(function(error) {
                console.log("Error playing music: " + error);
            });
        });
    }

    void play_dive() {
        EM_ASM({
            const music = document.getElementById('music-background-1');
            if (music) {
                music.pause(); // Остановить музыку
                music.currentTime = 0; // Сбросить время воспроизведения
            } else {
                console.log("Element with id 'music-background-1' not found.");
            }

            const diveSound = document.getElementById('sound-plane-dive');
            diveSound.currentTime = 0;
            diveSound.play().catch(function(error) {
                console.log("Error playing music: " + error);
            });
        });
    }
};
