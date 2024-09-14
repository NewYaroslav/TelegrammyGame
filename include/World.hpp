#pragma once

#include "ObjectFactory.hpp"
#include "ResourceManager.hpp"

class World {
public:

    World() = default;
    ~World() = default;

    // Возвращает ссылку на вектор объектов
    std::vector<std::unique_ptr<BaseObject>>& get_objects() {
        return m_objects;
    }

    void clear() {
        m_objects.clear();
    }

    // создание вражеского объекта
    void spawn_enemy(const ObjectType& type) {
        ResourceManager& instance = ResourceManager::get_instance();
        const int32_t max_width = instance.get_width();
        const int32_t max_height = instance.get_height();
        switch (type) {
        case ObjectType::ENEMY_RKN_WEAK:
            spawn_enemy_rkn_week(
                instance.random_int_range(max_width + ENEMY_RKN_WEAK_WIDTH, max_width + 10*ENEMY_RKN_WEAK_WIDTH),
                instance.random_int_range(ENEMY_RKN_WEAK_HEIGHT/2, max_height - ENEMY_RKN_WEAK_HEIGHT/2)
            );
            break;
        case ObjectType::ENEMY_RKN_MIDDLE:
            spawn_enemy_rkn_middle(
                instance.random_int_range(max_width + ENEMY_RKN_WEAK_WIDTH, max_width + 10*ENEMY_RKN_WEAK_WIDTH),
                instance.random_int_range(ENEMY_RKN_WEAK_HEIGHT/2, max_height - ENEMY_RKN_WEAK_HEIGHT/2)
            );
            break;
        case ObjectType::ENEMY_PACKAGE_WEAK:
            spawn_enemy_package_week(
                instance.random_int_range(max_width + ENEMY_PACKAGE_WEAK_WIDTH, max_width + 10*ENEMY_PACKAGE_WEAK_WIDTH),
                instance.random_int_range(ENEMY_PACKAGE_WEAK_HEIGHT/2, max_height - ENEMY_PACKAGE_WEAK_HEIGHT/2),
                0.0f
            );
            break;
        case ObjectType::ENEMY_PACKAGE_MITOSIS:
            spawn_enemy_package_mitosis(
                max_width - 5*ENEMY_PACKAGE_WEAK_WIDTH,
                max_height/2,
                0,
                ENEMY_PACKAGE_MITOSIS_MAX_STEP
            );
            break;
        default:
            break;
        };
    }

    void spawn_enemy(
            const std::vector<std::vector<int>>& bit_grid,
            const ObjectType& obj_type,
            const int32_t& offset_x = 0) {
        ResourceManager& instance = ResourceManager::get_instance();
        const int32_t max_width = instance.get_width();
        const int32_t max_height = instance.get_height();
        int32_t center_x_min = offset_x;
        int32_t center_x_max = offset_x;
        int32_t center_y_min = 0;
        int32_t center_y_max = 0;
        switch (obj_type) {
        case ObjectType::ENEMY_RKN_WEAK:
            center_x_min += max_width + bit_grid.size()*(ENEMY_RKN_WEAK_WIDTH + 10)/2 + 10;
            center_x_max += max_width + bit_grid.size()*(ENEMY_RKN_WEAK_WIDTH + 10);
            center_y_min = (bit_grid[0].size()/2) * (ENEMY_RKN_WEAK_HEIGHT + 10);
            center_y_max = max_height - (bit_grid[0].size()/2) * (ENEMY_RKN_WEAK_HEIGHT + 10);
            process_bit_grid(bit_grid,
                    std::max(ENEMY_RKN_WEAK_WIDTH, ENEMY_RKN_WEAK_HEIGHT) + 10,
                    instance.random_int_range(center_x_min, center_x_max),
                    instance.random_int_range(std::min(center_y_min, center_y_max), std::max(center_y_min, center_y_max)),
                    [this](const int32_t& x, const int32_t& y) {
                spawn_enemy_rkn_week(x, y);
            });
            break;
        case ObjectType::ENEMY_PACKAGE_WEAK:
            center_x_min += max_width + bit_grid.size()*(ENEMY_PACKAGE_WEAK_WIDTH + 10)/2 + 10;
            center_x_max += max_width + bit_grid.size()*(ENEMY_PACKAGE_WEAK_WIDTH + 10);
            center_y_min = (bit_grid[0].size()/2) * (ENEMY_PACKAGE_WEAK_HEIGHT + 10);
            center_y_max = max_height - (bit_grid[0].size()/2) * (ENEMY_PACKAGE_WEAK_HEIGHT + 10);
            process_bit_grid(bit_grid,
                    std::max(ENEMY_PACKAGE_WEAK_WIDTH, ENEMY_PACKAGE_WEAK_HEIGHT) + 10,
                    instance.random_int_range(center_x_min, center_x_max),
                    instance.random_int_range(std::min(center_y_min, center_y_max), std::max(center_y_min, center_y_max)),
                    [this](const int32_t& x, const int32_t& y) {
                spawn_enemy_package_week(x, y, 0.0f);
            });
            break;
        case ObjectType::ENEMY_PACKAGE_MITOSIS:
            center_x_min += max_width + bit_grid.size()*(ENEMY_PACKAGE_WEAK_WIDTH + 10)/2 + 10;
            center_x_max += max_width + bit_grid.size()*(ENEMY_PACKAGE_WEAK_WIDTH + 10);
            center_y_min = (bit_grid[0].size()/2) * (ENEMY_PACKAGE_WEAK_HEIGHT + 10);
            center_y_max = max_height - (bit_grid[0].size()/2) * (ENEMY_PACKAGE_WEAK_HEIGHT + 10);
            process_bit_grid(bit_grid,
                    std::max(ENEMY_PACKAGE_WEAK_WIDTH, ENEMY_PACKAGE_WEAK_HEIGHT) + 10,
                    instance.random_int_range(center_x_min, center_x_max),
                    instance.random_int_range(std::min(center_y_min, center_y_max), std::max(center_y_min, center_y_max)),
                    [this](const int32_t& x, const int32_t& y) {
                spawn_enemy_package_mitosis(x, y, 0.0f, ENEMY_PACKAGE_MITOSIS_MAX_STEP);
            });
            break;
        default:
            break;
        };
    }

    // создание игрока
    void spawn_player() {
        ResourceManager& instance = ResourceManager::get_instance();
        const int32_t max_width = instance.get_width();
        const int32_t max_height = instance.get_height();
        spawn_plane(max_width/3, max_height/2);
    }

    void spawn_cash(const ObjectType& type) {
        ResourceManager& instance = ResourceManager::get_instance();

        ObjectParams obj_param;
        switch (type) {
        case ObjectType::CASH_AMMO_300:

            obj_param.x = instance.random_int_range(
            CASH_AMMO_300_WIDTH,
            instance.get_height() - CASH_AMMO_300_WIDTH);
            obj_param.y = 0 - CASH_AMMO_300_HEIGHT;

            m_objects.emplace_back(ObjectFactory::make_object(ObjectType::CASH_AMMO_300, obj_param));
            break;
        default:
            break;
        };
    }

    void update() {
        generate_сloud();
    }

private:
    std::vector<std::unique_ptr<BaseObject>> m_objects;

    void generate_сloud() {
        // Генерация облаков
        ResourceManager& instance = ResourceManager::get_instance();
        if (instance.random_chance() < 0.035) {
            ObjectParams obj_param;
            obj_param.x = instance.get_width() + CLOUD_WIDTH;

            const int32_t max_height = instance.get_height() / 3;
            const int32_t min_height = CLOUD_HEIGHT / 2;

            const int32_t cloud_rnd_y = instance.random_int_range(-min_height, max_height);
            const int32_t cloud_y_ampl = max_height + min_height;

            obj_param.speed = ((cloud_rnd_y + min_height)  * CLOUD_SPEED) / (cloud_y_ampl == 0 ? 1 : cloud_y_ampl);
            obj_param.y = max_height - (cloud_rnd_y + min_height);
            obj_param.scale = (float)obj_param.speed / (float)CLOUD_SPEED;

            m_objects.emplace_back(ObjectFactory::make_object(ObjectType::CLOUD, obj_param));
        }
    }

    void spawn_plane(const int32_t& x, const int32_t& y) {
        ObjectParams obj_param;
        obj_param.x = x;
        obj_param.y = y;
        obj_param.on_spawn = [this](BaseObject* object, const ObjectType& type) {
            switch (type) {
            case ObjectType::EXPLOSION_LARGE:
                spawn_large_explosion(object->x, object->y, object->width*2);
                break;
            case ObjectType::EXPLOSION_SMALL:
                spawn_small_explosion(object->x, object->y, object->width);
                break;
            case ObjectType::BULLET_DOLLAR:
                spawn_bullet_dollar(object->x, object->y);
                break;
            default:
                break;
            };
        };

        m_objects.emplace_back(ObjectFactory::make_object(ObjectType::PLANE, obj_param));
    }

    void spawn_large_explosion(const int32_t& x, const int32_t& y, const int32_t& size) {
        ObjectParams obj_param;
        obj_param.x = x;
        obj_param.y = y;
        obj_param.size = size;
        m_objects.emplace_back(ObjectFactory::make_object(ObjectType::EXPLOSION_LARGE, obj_param));
    }

    void spawn_small_explosion(const int32_t& x, const int32_t& y, const int32_t& size) {
        ObjectParams obj_param;
        obj_param.x = x;
        obj_param.y = y;
        obj_param.size = size;
        m_objects.emplace_back(ObjectFactory::make_object(ObjectType::EXPLOSION_SMALL, obj_param));
    }

    void spawn_bullet_dollar(const int32_t& x, const int32_t& y) {
        ObjectParams obj_param;
        obj_param.x = x;
        obj_param.y = y;
        obj_param.on_spawn = [this](BaseObject* object, const ObjectType& type) {
            spawn_small_explosion(object->x, object->y, object->width);
        };
        m_objects.emplace_back(ObjectFactory::make_object(ObjectType::BULLET_DOLLAR, obj_param));
    }

    void spawn_enemy_rkn_week(const int32_t& x, const int32_t& y) {
        ObjectParams obj_param;
        obj_param.x = x;
        obj_param.y = y;
        obj_param.on_spawn = [this](BaseObject* object, const ObjectType& type) {
            ObjectParams obj_param_l2;
            obj_param_l2.x = object->x;
            obj_param_l2.y = object->y;
            m_objects.emplace_back(ObjectFactory::make_object(
                ObjectType::EXPLOSION_ENEMY_RKN_WEAK,
                obj_param_l2));
        };

        m_objects.emplace_back(ObjectFactory::make_object(
            ObjectType::ENEMY_RKN_WEAK,
            obj_param));
    }

    void spawn_enemy_rkn_middle(const int32_t& x, const int32_t& y) {
        ObjectParams obj_param;
        obj_param.x = x;
        obj_param.y = y;
        obj_param.on_spawn = [this](BaseObject* object, const ObjectType& type) {
            ObjectParams obj_param_l2;
            obj_param_l2.x = object->x;
            obj_param_l2.y = object->y;
            m_objects.emplace_back(ObjectFactory::make_object(
                ObjectType::EXPLOSION_ENEMY_RKN_WEAK,
                obj_param_l2));
        };

        m_objects.emplace_back(ObjectFactory::make_object(
            ObjectType::ENEMY_RKN_MIDDLE,
            obj_param));
    }

    void spawn_enemy_package_week(const int32_t& x, const int32_t& y, const float& angle) {
        ObjectParams obj_param;
        obj_param.x = x;
        obj_param.y = y;
        obj_param.angle = angle;
        obj_param.on_spawn = [this](BaseObject* object, const ObjectType& type) {
            ObjectParams obj_param_l2;
            obj_param_l2.x = object->x;
            obj_param_l2.y = object->y;
            obj_param_l2.angle = object->angle;
            m_objects.emplace_back(ObjectFactory::make_object(
                ObjectType::EXPLOSION_ENEMY_PACKAGE_WEAK,
                obj_param_l2));
        };

        m_objects.emplace_back(ObjectFactory::make_object(
                ObjectType::ENEMY_PACKAGE_WEAK,
                obj_param));
    }

    void spawn_enemy_package_mitosis(const int32_t& x, const int32_t& y, const float& angle, const int32_t& mitosis_step) {
        ObjectParams obj_param;
        obj_param.x = x;
        obj_param.y = y;
        obj_param.angle = angle;
        obj_param.obj_state = mitosis_step;
        obj_param.on_spawn = [this](BaseObject* object, const ObjectType& type) {
            EnemyPackageMitosisObject *enemy = dynamic_cast<EnemyPackageMitosisObject*>(object);
            spawn_explosion_enemy_package_mitosis(enemy->x, enemy->y, enemy->angle, enemy->mitosis_step);
        };

        m_objects.emplace_back(ObjectFactory::make_object(
            ObjectType::ENEMY_PACKAGE_MITOSIS,
            obj_param));
    }

    void spawn_explosion_enemy_package_mitosis(const int32_t& x, const int32_t& y, const float& angle, const int32_t& mitosis_step) {
        ObjectParams obj_param;
        obj_param.x = x;
        obj_param.y = y;
        obj_param.angle = angle;
        obj_param.obj_state = mitosis_step;
        obj_param.on_spawn = [this](BaseObject* object, const ObjectType& type) {
            ExplosionEnemyPackageMitosisObject *enemy = dynamic_cast<ExplosionEnemyPackageMitosisObject*>(object);
            if (type == ObjectType::EXPLOSION_ENEMY_PACKAGE_WEAK) {
                ObjectParams obj_param_l2;
                obj_param_l2.x = enemy->x;
                obj_param_l2.y = enemy->y;
                obj_param_l2.angle = enemy->angle;
                m_objects.emplace_back(ObjectFactory::make_object(
                    ObjectType::EXPLOSION_ENEMY_PACKAGE_WEAK,
                    obj_param_l2));
            } else
            if (type == ObjectType::ENEMY_PACKAGE_MITOSIS) {
                spawn_enemy_package_mitosis(
                    enemy->x - ENEMY_PACKAGE_WEAK_WIDTH/2,
                    enemy->y,
                    enemy->angle,
                    enemy->mitosis_step
                );
                spawn_enemy_package_mitosis(
                    object->x + ENEMY_PACKAGE_WEAK_WIDTH/2,
                    object->y,
                    enemy->angle,
                    enemy->mitosis_step
                );
            } else
            if (type == ObjectType::ENEMY_PACKAGE_WEAK) {
                spawn_enemy_package_week(
                    object->x - ENEMY_PACKAGE_WEAK_WIDTH/2,
                    object->y,
                    object->angle
                );
                spawn_enemy_package_week(
                    object->x + ENEMY_PACKAGE_WEAK_WIDTH/2,
                    object->y,
                    object->angle
                );
            }
        };

        m_objects.emplace_back(ObjectFactory::make_object(
            ObjectType::EXPLOSION_ENEMY_PACKAGE_MITOSIS,
            obj_param));
    }

    void process_bit_grid(const std::vector<std::vector<int>>& bit_grid,
                          const int32_t& step_size,
                          const int32_t& center_x,
                          const int32_t& center_y,
                          const std::function<void(const int32_t&, const int32_t&)>& callback) {
        // Получаем размеры сетки
        int32_t rows = bit_grid.size();
        if (rows == 0) return;
        int32_t cols = bit_grid[0].size();

        // Проходим по каждому элементу сетки
        for (int32_t i = 0; i < rows; ++i) {
            for (int32_t j = 0; j < cols; ++j) {
                // Пропускаем ячейки, где бит равен 0
                if (bit_grid[i][j] == 0) continue;

                // Вычисляем координаты центра текущей ячейки
                int32_t object_x = center_x + (j - cols / 2) * step_size;
                int32_t object_y = center_y + (i - rows / 2) * step_size;

                // Вызываем callback с заданным типом объекта и его координатами
                callback(object_x, object_y);
            }
        }
    }

};
