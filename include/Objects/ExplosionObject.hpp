#pragma once

#include "BaseObject.hpp"

class ExplosionObject final : public BaseObject {
public:

    ExplosionObject(
                const int32_t& id,
                const int32_t& x,
                const int32_t& y,
                const int32_t& width,
                const int32_t& height,
                const int32_t& time,
                const int32_t& speed,
                const int32_t& power = 0) :
            BaseObject(id, x, y, width, height, time, speed, std::string()),
            m_power(power) {
        play_explosion();
    }

    virtual ~ExplosionObject() {};

    ObjectCategory get_category() override final {
        return ObjectCategory::EXPLOSION;
    }

    ObjectType get_type() override final {
        if (m_power <= 1) return ObjectType::EXPLOSION_SMALL;
        if (m_power == 2) return ObjectType::EXPLOSION_MEDIUM;
        return ObjectType::EXPLOSION_LARGE;
    }

    bool is_online() override final {
        return true;
    }

    void update() override final {
        hp--;
        x += speed;
    }

    void draw() override final {
        EM_ASM({
            const obj_id = $0;
            const obj_x = $1;
            const obj_y = $2;;
            const obj_width  = $3;
            const obj_height = $4;
            const is_debug = $5;

            const existing_explosion = document.getElementById(`obj_${obj_id}`);
            if (existing_explosion) {
                existing_explosion.style.left = `${obj_x - obj_width / 2}px`;
                existing_explosion.style.top = `${obj_y - obj_height / 2}px`;
            } else {
                const explosion = document.createElement('div');
                explosion.className = 'explosion';
                explosion.id = `obj_${obj_id}`;
                explosion.style.width = `${obj_width}px`;
                explosion.style.height = `${obj_height}px`;
                explosion.style.left = `${obj_x - obj_width / 2}px`;
                explosion.style.top = `${obj_x - obj_height / 2}px`;

                const explosion_flash = document.createElement('div');
                explosion_flash.className = 'explosion-flash';
                explosion_flash.style.width = `${obj_width}px`;
                explosion_flash.style.height = `${obj_height}px`;
                explosion.appendChild(explosion_flash);

                const existing_smoke = document.createElement('div');
                existing_smoke.className = 'explosion-smoke';
                existing_smoke.style.width = `${obj_width}px`;
                existing_smoke.style.height = `${obj_height}px`;
                explosion.appendChild(existing_smoke);

                // Запуск анимации взрыва
                requestAnimationFrame(() => {
                    requestAnimationFrame(() => {
                        explosion_flash.classList.add('animate');
                        existing_smoke.classList.add('animate');
                    });
                });

                // Удаление элемента после завершения анимации
                explosion_flash.addEventListener('animationend', () => {
                    explosion_flash.remove();
                });

                existing_smoke.addEventListener('animationend', () => {
                    existing_smoke.remove();
                });

                document.getElementById('game-area').appendChild(explosion);
            }

            if (is_debug) {
                const box = document.createElement('div');
                box.className = 'box';
                box.style.left = `${obj_x - obj_width / 2}px`;
                box.style.top = `${obj_y - obj_height / 2}px`;
                box.style.width = `${obj_width}px`;
                box.style.height = `${obj_height}px`;

                document.getElementById('game-area').appendChild(box);
            }
        },
        id, x, y, std::max(width, height), std::max(width, height), (bool)GAME_DEBUG);
    }

private:
    int32_t m_power = 0;

    void play_explosion() {
        EM_ASM({
            const power = $0;
            let sound_type;

            if (power <= 1) {
                sound_type = 'sound-explosion-small';
            } else
            if (power == 2) {
                sound_type = 'sound-explosion-medium';
            } else
            if (power >= 3) {
                sound_type = 'sound-explosion-large';
            }

            if (sound_type) {
                const sound = document.getElementById(sound_type);
                if (sound) {
                    sound.currentTime = 0;
                    sound.play().catch(function(error) {
                        console.log("Error playing sound: " + error);
                    });
                } else {
                    console.log("Sound element not found: " + sound_type);
                }
            } else {
                console.log("Sound type is undefined");
            }
        }, m_power);
    }

};
