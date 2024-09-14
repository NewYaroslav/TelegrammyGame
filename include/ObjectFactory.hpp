#pragma once

#include "Objects\PlaneObject.hpp"
#include "Objects\CloudObject.hpp"
#include "Objects\ExplosionObject.hpp"
#include "Objects\ExplosionEnemyRknWeakObject.hpp"
#include "Objects\ExplosionEnemyPackageObject.hpp"
#include "Objects\ExplosionEnemyPackageMitosisObject.hpp"
#include "Objects\BulletDollarObject.hpp"
#include "Objects\EnemyRknWeakObject.hpp"
#include "Objects\EnemyRknMiddleObject.hpp"
#include "Objects\EnemyPackageWeakObject.hpp"
#include "Objects\EnemyPackageMitosisObject.hpp"
#include "Objects\CashAmmo300Object.hpp"
#include <memory>

struct ObjectParams {
    int32_t x       = 0;
    int32_t y       = 0;
    int32_t width   = 0;
    int32_t height  = 0;
    int32_t size    = 0;
    int32_t health  = 0;
    int32_t speed   = 0;
    int32_t obj_state = 0;
    float angle     = 0.0f;
    float scale     = 1.0f;
    std::function<void(BaseObject*, const ObjectType&)> on_spawn = nullptr;
};

class ObjectFactory {
public:

    template<typename... Args>
    static std::unique_ptr<BaseObject> make_object(const ObjectType& type, const ObjectParams& params) {
        switch (type) {
        case ObjectType::PLANE:
            return std::make_unique<PlaneObject>(params.x, params.y, params.on_spawn);
        case ObjectType::CLOUD:
            return std::make_unique<CloudObject>(params.x, params.y, params.speed, params.scale);
        // взрывы
        case ObjectType::EXPLOSION_SMALL:
            return std::make_unique<ExplosionSmallObject>(params.x, params.y, params.size);
        case ObjectType::EXPLOSION_MEDIUM:
            return std::make_unique<ExplosionMediumObject>(params.x, params.y, params.size);
        case ObjectType::EXPLOSION_LARGE:
            return std::make_unique<ExplosionLargeObject>(params.x, params.y, params.size);
        case ObjectType::EXPLOSION_ENEMY_RKN_WEAK:
            return std::make_unique<ExplosionEnemyRknWeakObject>(params.x, params.y);
        case ObjectType::EXPLOSION_ENEMY_PACKAGE_WEAK:
            return std::make_unique<ExplosionEnemyPackageObject>(params.x, params.y, params.angle);
        case ObjectType::EXPLOSION_ENEMY_PACKAGE_MITOSIS:
            return std::make_unique<ExplosionEnemyPackageMitosisObject>(params.x, params.y, params.angle, params.obj_state, params.on_spawn);
        // снаряды
        case ObjectType::BULLET_DOLLAR:
            return std::make_unique<BulletDollarObject>(params.x, params.y, params.on_spawn);
        // враги
        case ObjectType::ENEMY_RKN_WEAK:
            return std::make_unique<EnemyRknWeakObject>(params.x, params.y, params.on_spawn);
        case ObjectType::ENEMY_RKN_MIDDLE:
            return std::make_unique<EnemyRknMiddleObject>(params.x, params.y, params.on_spawn);
        case ObjectType::ENEMY_PACKAGE_WEAK:
            return std::make_unique<EnemyPackageWeakObject>(params.x, params.y, params.angle, params.on_spawn);
        case ObjectType::ENEMY_PACKAGE_MITOSIS:
            return std::make_unique<EnemyPackageMitosisObject>(params.x, params.y, params.angle, params.obj_state, params.on_spawn);
        // патроны
        case ObjectType::CASH_AMMO_300:
            return std::make_unique<CashAmmo300Object>(params.x, params.y);
        default:
            return nullptr;
        }
    }
};
