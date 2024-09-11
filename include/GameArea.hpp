#pragma once

#include <emscripten.h>
#include <emscripten/bind.h>

class GameArea {
public:

    static GameArea& get_instance() {
        static GameArea instance;
        return instance;
    }

    const int32_t& get_width() const  {
        return m_width;
    }

    const int32_t& get_height() const  {
        return m_height;
    }

    const int64_t get_object_id() {
        return m_object_id++;
    }

private:
    int32_t m_width = 0;
    int32_t m_height = 0;
    int64_t m_object_id = 0;

    GameArea() {
        m_width = EM_ASM_INT({
            return window.innerWidth;
        });
        m_height = EM_ASM_INT({
            return window.innerHeight;
        });
    }

    ~GameArea() = default;

    // Delete copy constructor and assignment operator to enforce singleton pattern
    GameArea(const GameArea&) = delete;
    GameArea& operator=(const GameArea&) = delete;
};
