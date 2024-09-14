#pragma once

#include "GameArea.hpp"
#include "RandomGenerator.hpp"

class ResourceManager {
public:

    static ResourceManager& get_instance() {
        static ResourceManager instance;
        return instance;
    }

    const int32_t& get_width() const  {
        return m_game_area.get_width();
    }

    const int32_t& get_height() const  {
        return m_game_area.get_height();
    }

    const int64_t get_object_id() {
        return m_object_id++;
    }

    // Возвращает случайное значение от 0 до 1.0 (шанс)
    const double random_chance() {
        return m_rnd_gen.random_chance();
    }

    // Случайное целое число в диапазоне [min, max], включая отрицательные числа
    int32_t random_int_range(const int32_t& min, const int32_t& max) {
        return m_rnd_gen.random_int_range(min, max);
    }

private:
    GameArea        m_game_area;
    RandomGenerator m_rnd_gen;
    int64_t         m_object_id = 0;


    ResourceManager() :
        m_game_area(), m_rnd_gen() {
    }

    ~ResourceManager() = default;

    // Delete copy constructor and assignment operator to enforce singleton pattern
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

};
