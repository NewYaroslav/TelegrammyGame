#pragma once

#include "BaseObject.hpp"

#define ENEMY_RKN_WEAK_WIDTH    80
#define ENEMY_RKN_WEAK_HEIGHT   80
#define ENEMY_RKN_WEAK_HP       1
#define ENEMY_RKN_WEAK_SPEED    -5

class EnemyRknWeakObject final : public BaseObject {
public:

    EnemyRknWeakObject(
            const int32_t& x,
            const int32_t& y,
            const std::function<void(BaseObject*, const ObjectType&)> on_spawn = nullptr) :
        BaseObject(x, y,
            ENEMY_RKN_WEAK_WIDTH,
            ENEMY_RKN_WEAK_HEIGHT,
            ENEMY_RKN_WEAK_HP,
            ENEMY_RKN_WEAK_SPEED,
            0, "enemy-rkn-weak", on_spawn) {
    }

    virtual ~EnemyRknWeakObject() {}

    ObjectCategory get_category() override final {
        return ObjectCategory::ENEMY;
    }

    ObjectType get_type() override final {
        return ObjectType::ENEMY_RKN_WEAK;
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
};
