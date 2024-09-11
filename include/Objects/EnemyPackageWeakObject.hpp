#pragma once

#include "BaseObject.hpp"
#include "../Utils/LowPassFilter.hpp"
#include <random>
#include <chrono>
#include <algorithm>

class EnemyPackageWeakObject final : public BaseObject {
public:

    EnemyPackageWeakObject(
                const int32_t& id,
                const int32_t& x,
                const int32_t& y,
                const int32_t& width,
                const int32_t& height,
                const int32_t& hp,
                const int32_t& speed,
                const std::function<void(BaseObject*, const ObjectType&)> on_spawn = nullptr) :
            BaseObject(id, x, y, width, height, hp, speed, "enemy-package-weak", on_spawn),
            filter_x(5.0, 0.05), filter_y(5.0, 0.05), filter_a(5.0, 0.05) {
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        m_rnd_gen = std::mt19937(seed);
    }

    virtual ~EnemyPackageWeakObject() {}

    ObjectCategory get_category() override final {
        return ObjectCategory::ENEMY;
    }

    ObjectType get_type() override final {
        return ObjectType::ENEMY_PACKAGE_WEAK;
    }

    bool is_online() override final {
        return (x >= -width);
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
        GameArea& game_area = GameArea::get_instance();
        const int32_t area_height = game_area.get_height();

        static const double pi2 = 2.0 * std::acos(-1);
        std::uniform_real_distribution<> dist_angle(0.0, pi2);

        // Выбираем случайное направление для движения
        double rnd_angle = dist_angle(m_rnd_gen); // Угол в диапазоне [0, 2π]
        double dx = std::abs(speed) * std::cos(rnd_angle);
        double dy = 2 * std::abs(speed) * std::sin(rnd_angle);

        // Обновляем позицию частицы
        x += filter_x.update(speed + dx);
        y += filter_y.update(dy);

        if (dy >= 0) {
            angle += filter_a.update(10.0f * rnd_angle/pi2);
        } else {
            angle += filter_a.update(-10.0f * rnd_angle/pi2);
        }

        const int32_t min_height = area_height - height;
        const int32_t max_height = height;
        if (y >= min_height) y = min_height;
        if (y <= max_height) y = max_height;
    }

private:
    std::mt19937 m_rnd_gen;
    LowPassFilter<double> filter_x;
    LowPassFilter<double> filter_y;
    LowPassFilter<double> filter_a;

    void play_explosion() {
        EM_ASM({
            const shootSound = document.getElementById('sound-explosion-bubble');
            shootSound.currentTime = 0;
            shootSound.play().catch(function(error) {
                console.log("Error playing music: " + error);
            });
        });
    }
};
