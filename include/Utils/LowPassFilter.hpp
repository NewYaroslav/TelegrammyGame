#pragma once

#include <cmath>

template <typename T>
class LowPassFilter {
private:
    T m_alfa = 0;
    T m_beta = 0;
    T m_prev = 0;
    bool m_is_update = false;
    bool m_is_init = false;
public:

    LowPassFilter() = default;

    /// \brief Инициализация фильтра низкой частоты
    /// \param dt время переходного процесса
    /// \param period период дискретизации
    /// \param error_signal ошибка сигнала
    LowPassFilter(
            const T dt,
            const T period = 1.0,
            const T error_signal = 0.03) {
        T N = dt / period;
        T Ntay = std::log(1.0 / error_signal);
        m_alfa = std::exp(-Ntay / N);
        m_beta = 1.0 - m_alfa;
        m_is_init = true;
    }


    bool update(const T& in, T& out) {
        if(!m_is_init) {
            out = in;
            return false;
        }
        if (!m_is_update) {
            m_prev = in;
            m_is_update = true;
            out = in;
            return false;
        }
        out = m_alfa * m_prev + m_beta * in;
        m_prev = out;
        return true;
    }

    T update(const T& in) {
        if(!m_is_init) {
            return in;
        }
        if (!m_is_update) {
            m_prev = in;
            m_is_update = true;
            return in;
        }
        m_prev = m_alfa * m_prev + m_beta * in;
        return m_prev;
    }

    void reset() {
        m_is_update = false;
    }
};
