#pragma once

#include "../GameArea.hpp"
#include <string>

enum class ObjectCategory {
    UNDEFINED,
    BULLET,
    AMMO,
    PLANE,
    ENEMY,
    EXPLOSION
};

enum class ObjectType {
    UNDEFINED,
    PLANE,
    CLOUD,
    EXPLOSION_SMALL,
    EXPLOSION_MEDIUM,
    EXPLOSION_LARGE,
    EXPLOSION_ENEMY_RKN_WEAK,
    EXPLOSION_ENEMY_PACKAGE_WEAK,
    BULLET_DOLLAR,
    ENEMY_RKN_WEAK,
    ENEMY_PACKAGE_WEAK,
    CASH_AMMO_300,
};

class BaseObject {
public:
    int32_t     id = 0;
    int32_t     x;
    int32_t     y;
    int32_t     width;
    int32_t     height;
    int32_t     hp = 0;
    int32_t     speed = 0;
    float       angle = 0;
    std::string class_name;
    std::function<void(BaseObject*, const ObjectType&)> on_spawn;

    BaseObject(
            const int32_t& id,
            const int32_t& x,
            const int32_t& y,
            const int32_t& width,
            const int32_t& height,
            const int32_t& hp = 0,
            const int32_t& speed = 0,
            const std::string& class_name = std::string(),
            const std::function<void(BaseObject*, const ObjectType&)> on_spawn = nullptr) :
        id(id), x(x), y(y), width(width), height(height), hp(hp), speed(speed),
        class_name(class_name), on_spawn(on_spawn) {
    }

    virtual ~BaseObject() {
        EM_ASM({
            const obj_id = $0;
            const element = document.getElementById(`obj_${obj_id}`);
            if (element) {
                element.remove();
            }
        }, id);
    }

    virtual ObjectCategory get_category() {
        return ObjectCategory::UNDEFINED;
    }

    virtual ObjectType get_type() {
        return ObjectType::UNDEFINED;
    }

    virtual bool is_online() {
        GameArea& game_area = GameArea::get_instance();
        const int32_t max_width = game_area.get_width();
        return (x >= -width && x <= (max_width + width));
    }


    virtual bool is_destroy() {
        return hp <= 0;
    }

    virtual void update() {
        x += speed;
    }

    virtual void damage(const int32_t& change_hp) {
        hp -= std::abs(change_hp);
    }

    virtual void draw() {
        EM_ASM({
            const obj_id = $0;
            const obj_x = $1;
            const obj_y = $2;;
            const obj_width  = $3;
            const obj_height = $4;
            const obj_angle = $5;
            const obj_class_name = UTF8ToString($6);
            const is_debug = $7;

            const existing_obj = document.getElementById(`obj_${obj_id}`);
            if (existing_obj) {
                // Обновляем положение объекта
                existing_obj.style.left = `${obj_x - obj_width / 2}px`;
                existing_obj.style.top = `${obj_y - obj_height / 2}px`;
                existing_obj.style.transform = `rotate(${obj_angle}deg)`;
            } else {
                // Создаем новый объект, если он не существует
                const obj = document.createElement('div');
                obj.className = obj_class_name;
                obj.id = `obj_${obj_id}`;
                obj.style.left = `${obj_x - obj_width / 2}px`;
                obj.style.top = `${obj_y - obj_height / 2}px`;
                //obj.style.width = `${obj_width}px`;
                //obj.style.height = `${obj_height}px`;
                obj.style.transform = `rotate(${obj_angle}deg)`;

                document.getElementById('game-area').appendChild(obj);
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
        id, x, y, width, height, angle, class_name.c_str(), (bool)GAME_DEBUG);
    }

    bool check_intersection(const BaseObject& other) const {
        // Преобразуем углы в радианы
        static const float pi = std::acos(-1);
        float rad_angle1 = angle * pi / 180.0f;
        float rad_angle2 = other.angle * pi / 180.0f;

        // Вычисляем центр обоих прямоугольников
        float center_x1 = static_cast<float>(x);
        float center_y1 = static_cast<float>(y);
        float center_x2 = static_cast<float>(other.x);
        float center_y2 = static_cast<float>(other.y);

        // Определяем размеры прямоугольников
        float w1 = static_cast<float>(width);
        float h1 = static_cast<float>(height);
        float w2 = static_cast<float>(other.width);
        float h2 = static_cast<float>(other.height);

        // Вершины первого прямоугольника
        float x1[4] = {
            -w1 / 2,
            w1 / 2,
            w1 / 2,
            -w1 / 2
        };
        float y1[4] = {
            -h1 / 2,
            -h1 / 2,
            h1 / 2,
            h1 / 2
        };

        // Вершины второго прямоугольника
        float x2[4] = {
            -w2 / 2,
            w2 / 2,
            w2 / 2,
            -w2 / 2
        };
        float y2[4] = {
            -h2 / 2,
            -h2 / 2,
            h2 / 2,
            h2 / 2
        };

        // Применяем вращение к вершинам первого прямоугольника
        for (int i = 0; i < 4; ++i) {
            float temp_x = x1[i];
            float temp_y = y1[i];
            x1[i] = center_x1 + (temp_x * std::cos(rad_angle1) - temp_y * std::sin(rad_angle1));
            y1[i] = center_y1 + (temp_x * std::sin(rad_angle1) + temp_y * std::cos(rad_angle1));
        }

        // Применяем вращение к вершинам второго прямоугольника
        for (int i = 0; i < 4; ++i) {
            float temp_x = x2[i];
            float temp_y = y2[i];
            x2[i] = center_x2 + (temp_x * std::cos(rad_angle2) - temp_y * std::sin(rad_angle2));
            y2[i] = center_y2 + (temp_x * std::sin(rad_angle2) + temp_y * std::cos(rad_angle2));
        }

        // Проверка пересечения прямоугольников
        auto [min_x1, max_x1] = get_min_max_impl(x1);
        auto [min_y1, max_y1] = get_min_max_impl(y1);
        auto [min_x2, max_x2] = get_min_max_impl(x2);
        auto [min_y2, max_y2] = get_min_max_impl(y2);

        // Проверяем, пересекаются ли прямоугольники
        return (max_x1 >= min_x2 && min_x1 <= max_x2 &&
                max_y1 >= min_y2 && min_y1 <= max_y2);
    }

private:

    std::pair<float, float> get_min_max_impl(float* arr) const {
        float min_val = arr[0];
        float max_val = arr[0];
        for (int i = 1; i < 4; ++i) {
            if (arr[i] < min_val) min_val = arr[i];
            if (arr[i] > max_val) max_val = arr[i];
        }
        return std::pair<float, float>(min_val, max_val);
    }

}; // BaseObject


