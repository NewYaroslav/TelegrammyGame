#pragma once

#include "BaseObject.hpp"

class ExplosionEnemyPackageObject final : public BaseObject {
public:

    ExplosionEnemyPackageObject(
            const int32_t& id,
            const int32_t& x,
            const int32_t& y,
            const int32_t& width,
            const int32_t& height,
            const int32_t& time,
            const int32_t& speed) :
        BaseObject(id, x, y, width, height, time, speed, std::string()) {
        play_explosion();
    }

    virtual ~ExplosionEnemyPackageObject() {};

    ObjectCategory get_category() override final {
        return ObjectCategory::EXPLOSION;
    }

    ObjectType get_type() override final {
        return ObjectType::EXPLOSION_ENEMY_PACKAGE_WEAK;
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

                const balloon_pop = document.createElement('div');
                balloon_pop.className = 'balloon-pop';
                balloon_pop.style.width = `${obj_width}px`;
                balloon_pop.style.height = `${obj_height}px`;
                explosion.appendChild(balloon_pop);

                const balloon_pop_smoke = document.createElement('div');
                balloon_pop_smoke.className = 'balloon-pop-smoke';
                balloon_pop_smoke.style.width = `${obj_width}px`;
                balloon_pop_smoke.style.height = `${obj_height}px`;
                explosion.appendChild(balloon_pop_smoke);

                // Запуск анимации взрыва
                requestAnimationFrame(() => {
                    requestAnimationFrame(() => {
                        balloon_pop.classList.add('animate');
                        balloon_pop_smoke.classList.add('animate');
                    });
                });

                // Удаление элемента после завершения анимации
                balloon_pop.addEventListener('animationend', () => {
                    balloon_pop.remove();
                });

                balloon_pop_smoke.addEventListener('animationend', () => {
                    balloon_pop_smoke.remove();
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
            const sound = document.getElementById('sound-explosion-bubble');
            sound.currentTime = 0;
            sound.play().catch(function(error) {
                console.log("Error playing music: " + error);
            });
        });
    }

};
