#pragma once

#include <emscripten.h>
#include <emscripten/bind.h>

class GameArea {
public:

    GameArea() {
        m_width = EM_ASM_INT({
            return window.innerWidth;
        });
        m_height = EM_ASM_INT({
            return window.innerHeight;
        });
    }

    ~GameArea() = default;

    const int32_t& get_width() const  {
        return m_width;
    }

    const int32_t& get_height() const  {
        return m_height;
    }


private:
    int32_t m_width = 0;
    int32_t m_height = 0;
};
