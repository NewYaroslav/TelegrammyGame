#pragma once

#include "BaseObject.hpp"

#define EXPLOSION_ENEMY_RKN_WEAK_WIDTH    80
#define EXPLOSION_ENEMY_RKN_WEAK_HEIGHT   80
#define EXPLOSION_ENEMY_RKN_WEAK_TIME       60
#define EXPLOSION_ENEMY_RKN_WEAK_SPEED      -2

class ExplosionEnemyRknWeakObject final : public BaseObject {
public:

    ExplosionEnemyRknWeakObject(
            const int32_t& x,
            const int32_t& y) :
            BaseObject(x, y,
                EXPLOSION_ENEMY_RKN_WEAK_WIDTH,
                EXPLOSION_ENEMY_RKN_WEAK_HEIGHT,
                EXPLOSION_ENEMY_RKN_WEAK_TIME,
                EXPLOSION_ENEMY_RKN_WEAK_SPEED) {
        play_explosion();
    }

    virtual ~ExplosionEnemyRknWeakObject() {};

    ObjectCategory get_category() override final {
        return ObjectCategory::EXPLOSION;
    }

    ObjectType get_type() override final {
        return ObjectType::EXPLOSION_ENEMY_RKN_WEAK;
    }

    bool is_online() override final {
        return true;
    }

    virtual void update() override final {
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

                const electric_flash = document.createElement('div');
                electric_flash.className = 'explosion-electric-flash';
                electric_flash.style.width = `${obj_width}px`;
                electric_flash.style.height = `${obj_height}px`;
                explosion.appendChild(electric_flash);

                const electric_glow = document.createElement('div');
                electric_glow.className = 'explosion-electric-glow';
                electric_glow.style.width = `${obj_width}px`;
                electric_glow.style.height = `${obj_height}px`;
                explosion.appendChild(electric_glow);

                // Запуск анимации взрыва
                requestAnimationFrame(() => {
                    requestAnimationFrame(() => {
                        electric_flash.classList.add('animate');
                        electric_glow.classList.add('animate');
                    });
                });

                // Удаление элемента после завершения анимации
                electric_flash.addEventListener('animationend', () => {
                    electric_flash.remove();
                });

                electric_glow.addEventListener('animationend', () => {
                    electric_glow.remove();
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

    void play_explosion() {
        EM_ASM({
            const sound = document.getElementById('sound-explosion-electric');
            sound.currentTime = 0;
            sound.play().catch(function(error) {
                console.log("Error playing music: " + error);
            });
        });
    }
};
