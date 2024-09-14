#pragma once

#include "BaseObject.hpp"
#include "..\Utils\LowPassFilter.hpp"
#include <random>
#include <chrono>
#include <algorithm>

#define ENEMY_PACKAGE_WEAK_WIDTH    60
#define ENEMY_PACKAGE_WEAK_HEIGHT   100
#define ENEMY_PACKAGE_WEAK_HP       1
#define ENEMY_PACKAGE_WEAK_SPEED    -5

class EnemyPackageWeakObject : public BaseObject {
public:

    EnemyPackageWeakObject(
                const int32_t& x,
                const int32_t& y,
                const float& angle,
                const std::function<void(BaseObject*, const ObjectType&)> on_spawn = nullptr) :
            BaseObject(x, y,
                ENEMY_PACKAGE_WEAK_WIDTH,
                ENEMY_PACKAGE_WEAK_HEIGHT,
                ENEMY_PACKAGE_WEAK_HP,
                ENEMY_PACKAGE_WEAK_SPEED,
                angle, "enemy-package-weak", on_spawn),
            m_filter_x(5.0, 0.05), m_filter_y(5.0, 0.05), m_filter_a(5.0, 0.05) {
    }

    virtual ~EnemyPackageWeakObject() {}

    ObjectCategory get_category() override final {
        return ObjectCategory::ENEMY;
    }

    ObjectType get_type() override {
        return ObjectType::ENEMY_PACKAGE_WEAK;
    }

    bool is_online() override final {
        return (x >= -width);
    }

    void damage(const int32_t& change_hp) override {
        if (hp > 0) {
            hp -= std::abs(change_hp);
            if (hp <= 0) {
                if (on_spawn) on_spawn(this, ObjectType::EXPLOSION_ENEMY_PACKAGE_WEAK);
            }
        }
    }

    void update() override {
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
    }

protected:
    LowPassFilter<double> m_filter_x;
    LowPassFilter<double> m_filter_y;
    LowPassFilter<double> m_filter_a;
};
