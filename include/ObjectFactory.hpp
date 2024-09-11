#pragma once

#include "Objects/PlaneObject.hpp"
#include "Objects/CloudObject.hpp"
#include "Objects/ExplosionObject.hpp"
#include "Objects/ExplosionEnemyRknWeakObject.hpp"
#include "Objects/ExplosionEnemyPackageObject.hpp"
#include "Objects/BulletDollarObject.hpp"
#include "Objects/EnemyRknWeakObject.hpp"
#include "Objects/EnemyPackageWeakObject.hpp"
#include "Objects/CashAmmo300Object.hpp"
#include <memory>

#define PLANE_WIDTH             180 // 200
#define PLANE_HEIGHT            60  // 80
#define PLANE_HP                1
#define PLANE_SPEED             10

#define CLOUD_WIDTH             400
#define CLOUD_HEIGHT            200
#define CLOUD_SPEED             -10

#define EXPLOSION_WIDTH         100
#define EXPLOSION_HEIGHT        100
#define EXPLOSION_TIME          20
#define EXPLOSION_SPEED         0

#define EXPLOSION_ENEMY_RKN_WEAK_TIME       60
#define EXPLOSION_ENEMY_RKN_WEAK_SPEED      -2

#define EXPLOSION_ENEMY_PACKAGE_WEAK_TIME   15
#define EXPLOSION_ENEMY_PACKAGE_WEAK_SPEED  0

#define DOLLAR_BULLET_WIDTH     70
#define DOLLAR_BULLET_HEIGHT    25
#define DOLLAR_BULLET_HP        1
#define DOLLAR_BULLET_SPEED     15

#define ENEMY_RKN_WEAK_WIDTH    80
#define ENEMY_RKN_WEAK_HEIGHT   80
#define ENEMY_RKN_WEAK_HP       1
#define ENEMY_RKN_WEAK_SPEED    -5

#define ENEMY_PACKAGE_WEAK_WIDTH    60
#define ENEMY_PACKAGE_WEAK_HEIGHT   100
#define ENEMY_PACKAGE_WEAK_HP       1
#define ENEMY_PACKAGE_WEAK_SPEED    -5

#define CASH_AMMO_300_WIDTH     120
#define CASH_AMMO_300_HEIGHT    25 //50
#define CASH_AMMO_300_HP        300
#define CASH_AMMO_300_SPEED     4

class ObjectFactory {
public:

    static std::unique_ptr<BaseObject> make_object(
            const ObjectType& type,
            const int32_t& x = 0,
            const int32_t& y = 0,
            const int32_t& speed = 0,
            const float& scale = 0,
            const int32_t& w = 0,
            const int32_t& h = 0,
            const std::function<void(BaseObject*, const ObjectType&)> on_spawn = nullptr) {
        GameArea& game_area = GameArea::get_instance();
        switch(type) {
        case ObjectType::CLOUD:
            return std::make_unique<CloudObject>(
                game_area.get_object_id(),
                x,
                y,
                CLOUD_WIDTH,
                CLOUD_HEIGHT,
                speed,
                scale);
        // взрывы
        case ObjectType::EXPLOSION_SMALL:
            return std::make_unique<ExplosionObject>(
                game_area.get_object_id(),
                x,
                y,
                w,
                h,
                EXPLOSION_TIME,
                EXPLOSION_SPEED,
                1);
        case ObjectType::EXPLOSION_MEDIUM:
            return std::make_unique<ExplosionObject>(
                game_area.get_object_id(),
                x,
                y,
                w,
                h,
                EXPLOSION_TIME,
                EXPLOSION_SPEED,
                2);
        case ObjectType::EXPLOSION_LARGE:
            return std::make_unique<ExplosionObject>(
                game_area.get_object_id(),
                x,
                y,
                w,
                h,
                EXPLOSION_TIME,
                EXPLOSION_SPEED,
                3);
        case ObjectType::EXPLOSION_ENEMY_RKN_WEAK:
            return std::make_unique<ExplosionEnemyRknWeakObject>(
                game_area.get_object_id(),
                x,
                y,
                w,
                h,
                EXPLOSION_ENEMY_RKN_WEAK_TIME,
                EXPLOSION_ENEMY_RKN_WEAK_SPEED);
        case ObjectType::EXPLOSION_ENEMY_PACKAGE_WEAK:
            return std::make_unique<ExplosionEnemyPackageObject>(
                game_area.get_object_id(),
                x,
                y,
                w,
                h,
                EXPLOSION_ENEMY_PACKAGE_WEAK_TIME,
                EXPLOSION_ENEMY_PACKAGE_WEAK_SPEED);
        // самолет
        case ObjectType::PLANE:
            return std::make_unique<PlaneObject>(
                game_area.get_object_id(),
                game_area.get_width()/2,
                game_area.get_height()/4,
                PLANE_WIDTH,
                PLANE_HEIGHT,
                PLANE_HP,
                PLANE_SPEED,
                on_spawn);
        // снаряды
        case ObjectType::BULLET_DOLLAR:
            return std::make_unique<BulletDollarObject>(
                game_area.get_object_id(),
                x,
                y,
                DOLLAR_BULLET_WIDTH,
                DOLLAR_BULLET_HEIGHT,
                DOLLAR_BULLET_HP,
                DOLLAR_BULLET_SPEED,
                on_spawn);
        // враги
        case ObjectType::ENEMY_RKN_WEAK:
            return std::make_unique<EnemyRknWeakObject>(
                game_area.get_object_id(),
                x,
                y,
                ENEMY_RKN_WEAK_WIDTH,
                ENEMY_RKN_WEAK_HEIGHT,
                ENEMY_RKN_WEAK_HP,
                ENEMY_RKN_WEAK_SPEED,
                on_spawn);
        case ObjectType::ENEMY_PACKAGE_WEAK:
            return std::make_unique<EnemyPackageWeakObject>(
                game_area.get_object_id(),
                x,
                y,
                ENEMY_PACKAGE_WEAK_WIDTH,
                ENEMY_PACKAGE_WEAK_HEIGHT,
                ENEMY_PACKAGE_WEAK_HP,
                ENEMY_PACKAGE_WEAK_SPEED,
                on_spawn);
        // патроны
        case ObjectType::CASH_AMMO_300:
            return std::make_unique<CashAmmo300Object>(
                game_area.get_object_id(),
                x,
                y,
                CASH_AMMO_300_WIDTH,
                CASH_AMMO_300_HEIGHT,
                CASH_AMMO_300_HP,
                CASH_AMMO_300_SPEED);
        default:
            return nullptr;
        }
    }

};
