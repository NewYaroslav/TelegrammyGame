#pragma once
#include <random>

class RandomGenerator {
public:

    RandomGenerator() {
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        m_rnd_gen = std::mt19937(seed);
        m_dist_chance = std::uniform_real_distribution<>(0.0, 1.0);
    }

    ~RandomGenerator() = default;

    // Случайное целое число в диапазоне [min, max], включая отрицательные числа
    int32_t random_int_range(const int32_t& min, const int32_t& max) {
        std::uniform_int_distribution<> dist(min, max);
        return dist(m_rnd_gen);
    }

    // Возвращает случайное значение от 0 до 1.0 (шанс)
    double random_chance() {
        return m_dist_chance(m_rnd_gen);
    }

private:
    std::mt19937                                m_rnd_gen;
    std::uniform_real_distribution<>            m_dist_chance;
};
