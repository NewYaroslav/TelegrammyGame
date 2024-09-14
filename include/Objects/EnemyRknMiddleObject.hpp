#pragma once

#include "BaseObject.hpp"

#define ENEMY_RKN_MIDDLE_WIDTH    80
#define ENEMY_RKN_MIDDLE_HEIGHT   80
#define ENEMY_RKN_MIDDLE_HP       2
#define ENEMY_RKN_MIDDLE_SPEED    -10

class EnemyRknMiddleObject final : public BaseObject {
public:

    EnemyRknMiddleObject(
            const int32_t& x,
            const int32_t& y,
            const std::function<void(BaseObject*, const ObjectType&)> on_spawn = nullptr) :
        BaseObject(x, y,
            ENEMY_RKN_MIDDLE_WIDTH,
            ENEMY_RKN_MIDDLE_HEIGHT,
            ENEMY_RKN_MIDDLE_HP,
            ENEMY_RKN_MIDDLE_SPEED,
            0, "enemy-rkn-middle", on_spawn) {
    }

    virtual ~EnemyRknMiddleObject() {}

    ObjectCategory get_category() override final {
        return ObjectCategory::ENEMY;
    }

    ObjectType get_type() override final {
        return ObjectType::ENEMY_RKN_MIDDLE;
    }

    bool is_online() override final {
        return (x >= -width);
    }

    void damage(const int32_t& change_hp) override final {
        if (hp > 0) {
            hp -= std::abs(change_hp);
            if (hp <= 0) {
                if (on_spawn) on_spawn(this, ObjectType::EXPLOSION_ENEMY_RKN_WEAK);
            }
        }
    }

    void update() override final {
        static const double period = 10.0;
        static const double sin_step = 2.0 * std::acos(-1) / period;
        const double offset_angle = std::sin(sin_step * m_angle_swing);
        m_angle_swing += 0.05;
        angle += 10.0 + 5.0 * offset_angle;
        x += speed + ((double)(speed/2) * offset_angle);
    }
private:
    double  m_angle_swing = 0;
};
