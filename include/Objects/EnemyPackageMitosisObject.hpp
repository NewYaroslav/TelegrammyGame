#pragma once

#include "EnemyPackageWeakObject.hpp"

#define ENEMY_PACKAGE_MITOSIS_MAX_STEP  4
#define ENEMY_PACKAGE_MITOSIS_CHANCE    0.002

class EnemyPackageMitosisObject final : public EnemyPackageWeakObject {
public:
    int32_t mitosis_step;

    EnemyPackageMitosisObject(
                const int32_t& x,
                const int32_t& y,
                const float& angle,
                const int32_t& mitosis_step,
                const std::function<void(BaseObject*, const ObjectType&)> on_spawn = nullptr) :
            EnemyPackageWeakObject(x, y, angle, on_spawn),
            mitosis_step(mitosis_step) {
    }

    virtual ~EnemyPackageMitosisObject() {
    }

    ObjectType get_type() override final {
        return ObjectType::ENEMY_PACKAGE_MITOSIS;
    }

    void damage(const int32_t& change_hp) override {
        if (hp > 0) {
            hp -= std::abs(change_hp);
            if (hp <= 0) {
                if (on_spawn) on_spawn(this, ObjectType::EXPLOSION_ENEMY_PACKAGE_MITOSIS);
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
        x += EnemyPackageWeakObject::m_filter_x.update(speed + dx);
        y += EnemyPackageWeakObject::m_filter_y.update(dy);

        if (dy >= 0) {
            angle += EnemyPackageWeakObject::m_filter_a.update(10.0f * rnd_angle/pi2);
        } else {
            angle += EnemyPackageWeakObject::m_filter_a.update(-10.0f * rnd_angle/pi2);
        }

        const int32_t min_height = area_height - height;
        const int32_t max_height = height;
        if (y >= min_height) y = min_height;
        if (y <= max_height) y = max_height;

        if (instance.random_chance() < ENEMY_PACKAGE_MITOSIS_CHANCE) {
            hp = 0;
            if (on_spawn) on_spawn(this, ObjectType::EXPLOSION_ENEMY_PACKAGE_MITOSIS);
        }
    }
};
