#pragma once

#include "BaseObject.hpp"

class BulletDollarObject final : public BaseObject {
public:

    BulletDollarObject(
            const int32_t& id,
            const int32_t& x,
            const int32_t& y,
            const int32_t& width,
            const int32_t& height,
            const int32_t& hp,
            const int32_t& speed,
            const std::function<void(BaseObject*, const ObjectType&)> on_spawn) :
        BaseObject(id, x, y, width, height, hp, speed, std::string(), on_spawn) {
    }

    virtual ~BulletDollarObject() {}

    ObjectCategory get_category() override final {
        return ObjectCategory::BULLET;
    }

    ObjectType get_type() override final {
        return ObjectType::BULLET_DOLLAR;
    }

    void damage(const int32_t& change_hp) override final {
        if (hp > 0) {
            hp -= std::abs(change_hp);
            if (hp <= 0) {
                if (on_spawn) on_spawn(this, ObjectType::EXPLOSION_SMALL);
            }
        }
    }

    void draw() override final {
        EM_ASM({
            const obj_id = $0;
            const obj_x = $1;
            const obj_y = $2;;
            const obj_width  = $3;
            const obj_height = $4;
            const is_debug = $5;

            // Отображение самолета
            const existing_bullet = document.getElementById(`obj_${obj_id}`);
            if (existing_bullet) {
                // Обновляем положение объекта
                existing_bullet.style.left = `${obj_x - obj_width / 2}px`;
                existing_bullet.style.top = `${obj_y - obj_height / 2}px`;
            } else {
                const bullet = document.createElement('div');
                bullet.className = 'dollar-bullet';
                bullet.id = `obj_${obj_id}`;
                bullet.style.left = `${obj_x - obj_width / 2}px`;
                bullet.style.top = `${obj_y - obj_height / 2}px`;
                bullet.style.width = `${obj_width}px`;
                bullet.style.height = `${obj_height}px`;

                const dollar_airflow = document.createElement('div');
                dollar_airflow.className = 'dollar-bullet-airflow';
                bullet.appendChild(dollar_airflow);

                const dollar_plane = document.createElement('div');
                dollar_plane.className = 'dollar-bullet-plane';
                bullet.appendChild(dollar_plane);

                document.getElementById('game-area').appendChild(bullet);
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
        id, x, y, width, height, (bool)GAME_DEBUG);
    }

};
