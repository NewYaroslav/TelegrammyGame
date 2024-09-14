#pragma once

#include "BaseObject.hpp"
#include "../Utils/LowPassFilter.hpp"
#include <random>
#include <chrono>
#include <algorithm>

#define EXPLOSION_ENEMY_PACKAGE_MITOSIS_WIDTH     100
#define EXPLOSION_ENEMY_PACKAGE_MITOSIS_HEIGHT    100
#define EXPLOSION_ENEMY_PACKAGE_MITOSIS_HP        2
#define EXPLOSION_ENEMY_PACKAGE_MITOSIS_SPEED     -5

class ExplosionEnemyPackageMitosisObject final : public BaseObject {
public:
    int32_t mitosis_step = 0;

    ExplosionEnemyPackageMitosisObject(
                const int32_t& x,
                const int32_t& y,
                const float& angle,
                const int32_t& mitosis_step,
                const std::function<void(BaseObject*, const ObjectType&)> on_spawn) :
            BaseObject(x, y,
                EXPLOSION_ENEMY_PACKAGE_MITOSIS_WIDTH,
                EXPLOSION_ENEMY_PACKAGE_MITOSIS_HEIGHT,
                EXPLOSION_ENEMY_PACKAGE_MITOSIS_HP,
                EXPLOSION_ENEMY_PACKAGE_MITOSIS_SPEED,
                angle, std::string(), on_spawn),
            m_filter_x(5.0, 0.05), m_filter_y(5.0, 0.05), m_filter_a(5.0, 0.05),
            mitosis_step(mitosis_step) {
        this->mitosis_step--;
        play_mitosis_start();
    }

    virtual ~ExplosionEnemyPackageMitosisObject() {
        play_mitosis_end();
    }

    ObjectCategory get_category() override final {
        return ObjectCategory::ENEMY;
    }

    ObjectType get_type() override final {
        return ObjectType::ENEMY_PACKAGE_WEAK;
    }

    bool is_online() override final {
        return (x >= -width);
    }

    bool is_destroy() override final {
        return hp <= 0 || !m_is_animation_running;
    }

    void damage(const int32_t& change_hp) override final {
        if (hp > 0) {
            hp -= std::abs(change_hp);
            if (hp <= 0) {
                if (on_spawn) on_spawn(this, ObjectType::EXPLOSION_ENEMY_PACKAGE_WEAK);
            }
        }
    }

    void update() override final {
        ResourceManager& instance = ResourceManager::get_instance();
        const int32_t area_height = instance.get_height();

        static const double pi2 = 2.0 * std::acos(-1);

        // Выбираем случайное направление для движения
        double rnd_angle = instance.random_chance() * pi2; // Угол в диапазоне [0, 2π]
        double dx = std::abs(speed) * std::cos(rnd_angle);
        double dy = 2 * std::abs(speed) * std::sin(rnd_angle);

        // Обновляем позицию частицы
        x += m_filter_x.update(speed + dx);
        y += m_filter_y.update(dy);

        if (dy >= 0) {
            angle += m_filter_a.update(10.0f * rnd_angle/pi2);
        } else {
            angle += m_filter_a.update(-10.0f * rnd_angle/pi2);
        }

        const int32_t min_height = area_height - height;
        const int32_t max_height = height;
        if (y >= min_height) y = min_height;
        if (y <= max_height) y = max_height;

        if (!m_is_animation_running) {
            if (mitosis_step) {
                on_spawn(this, ObjectType::ENEMY_PACKAGE_MITOSIS);
            } else {
                on_spawn(this, ObjectType::ENEMY_PACKAGE_WEAK);
            }
        }
    }

    void draw() override final {
        m_is_animation_running = (bool)EM_ASM_INT({
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
                existing_obj.style.transform = `rotate(${obj_angle}deg) translate(0%, 0%)`;
            } else {
                // Создаем новый объект, если он не существует
                const obj = document.createElement('div');
                obj.className = 'enemy-package-mitosis';
                obj.id = `obj_${obj_id}`;
                obj.style.left = `${obj_x - obj_width / 2}px`;
                obj.style.top = `${obj_y - obj_height / 2}px`;
                obj.style.transform = `rotate(${obj_angle}deg) translate(0%, 0%)`;

                obj.addEventListener('animationstart', function() {
                    obj.setAttribute('data-animation-running', 'true');
                });
                obj.addEventListener('animationend', function() {
                    obj.setAttribute('data-animation-running', 'false');
                });

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

            if (existing_obj) {
                return existing_obj.getAttribute('data-animation-running') === 'true' ? 1 : 0;
            }
            return 1;
        },
        id, x, y, width, height, angle, class_name.c_str(), (bool)GAME_DEBUG);
    }

private:
    LowPassFilter<double> m_filter_x;
    LowPassFilter<double> m_filter_y;
    LowPassFilter<double> m_filter_a;
    bool    m_is_animation_running = true;

    void play_mitosis_start() {
        EM_ASM({
            const shootSound = document.getElementById('sound-package-mitosis-begin');
            shootSound.currentTime = 0;
            shootSound.play().catch(function(error) {
                console.log("Error playing music: " + error);
            });
        });
    }

    void play_mitosis_end() {
        EM_ASM({
            const shootSound = document.getElementById('sound-package-mitosis-end');
            shootSound.currentTime = 0;
            shootSound.play().catch(function(error) {
                console.log("Error playing music: " + error);
            });
        });
    }
};
