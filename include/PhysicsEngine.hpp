#pragma once

#include "World.hpp"

class PhysicsEngine {
public:

    std::function<void(BaseObject* object)>     on_offline;
    std::function<void(BaseObject* object)>     on_destroy;
    std::function<void(PlaneObject* player)>    on_player;

    void update(World& world) {
        // Получаем массив объектов из World
        auto& objects = world.get_objects();

        // Обновляем состояния объектов
        for (auto& object : objects) {
            object->update();
        }

        // Обрабатываем урон
        for (size_t i = 0; i < objects.size(); ++i) {
            BaseObject* object_a = objects[i].get();

            if (object_a->get_category() == ObjectCategory::UNDEFINED) continue;
            if (!object_a->is_online() || object_a->is_destroy()) continue;

            if (object_a->get_category() == ObjectCategory::BULLET ||
                (object_a->get_category() == ObjectCategory::EXPLOSION &&
                object_a->get_type() != ObjectType::EXPLOSION_SMALL &&
                object_a->get_type() != ObjectType::EXPLOSION_ENEMY_PACKAGE_WEAK)  ||
                object_a->get_category() == ObjectCategory::PLANE) {
                for (size_t j = 0; j < objects.size(); ++j) {
                    if (i == j) continue;

                    BaseObject* object_b = objects[j].get();
                    if (object_b->get_category() != ObjectCategory::ENEMY) continue;
                    if (!object_b->is_online() || object_b->is_destroy()) continue;

                    if (object_a->check_intersection(*object_b)) {
                        const int32_t object_a_hp = object_a->hp;
                        const int32_t object_b_hp = object_b->hp;
                        if (object_a->get_category() != ObjectCategory::EXPLOSION) {
                            object_a->damage(object_b_hp);
                        }
                        object_b->damage(object_a_hp);
                    }
                } // for j
            }
        } // for i

        // Обрабатываем игрока
        for (size_t i = 0; i < objects.size(); ++i) {
            BaseObject* object_a = objects[i].get();
            if (object_a->get_category() != ObjectCategory::PLANE) continue;
            if (!object_a->is_online() || object_a->is_destroy()) continue;

            PlaneObject* player = dynamic_cast<PlaneObject*>(object_a);

            for (size_t j = 0; j < objects.size(); ++j) {
                if (i == j) continue;
                BaseObject* object_b = objects[j].get();
                if (object_b->get_category() != ObjectCategory::AMMO) continue;
                if (!object_b->is_online() || object_b->is_destroy()) continue;

                if (player->check_intersection(*object_b)) {
                    const int32_t object_b_hp = object_b->hp;
                    object_b->damage(object_b_hp);
                    player->current_ammo += object_b_hp;
                }
            } // for j

            on_player(player);
            break;
        } // for i

        // Обрабатываем уничтожение объектов
        for (auto& object : objects) {
            if (!object->is_online() && !object->is_destroy()) {
                on_offline(object.get());
            }
            if (!object->is_destroy()) continue;
            on_destroy(object.get());
        }

        // Удаляем лишние объекты
        objects.erase(std::remove_if(
            objects.begin(),
            objects.end(),
            [this](std::unique_ptr<BaseObject> &object) {
            return !object->is_online() || object->is_destroy();
        }), objects.end());
    }

};
