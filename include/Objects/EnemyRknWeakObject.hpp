#pragma once

#include "BaseObject.hpp"

class EnemyRknWeakObject final : public BaseObject {
public:

    EnemyRknWeakObject(
            const int32_t& id,
            const int32_t& x,
            const int32_t& y,
            const int32_t& width,
            const int32_t& height,
            const int32_t& hp,
            const int32_t& speed,
            const std::function<void(BaseObject*, const ObjectType&)> on_spawn = nullptr) :
        BaseObject(id, x, y, width, height, hp, speed, "enemy-rkn-weak", on_spawn) {
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
