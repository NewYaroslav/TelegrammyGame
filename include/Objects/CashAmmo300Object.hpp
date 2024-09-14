#pragma once

#include "BaseObject.hpp"

#define CASH_AMMO_300_WIDTH     120
#define CASH_AMMO_300_HEIGHT    25
#define CASH_AMMO_300_HP        300
#define CASH_AMMO_300_SPEED     4

class CashAmmo300Object final : public BaseObject {
public:

    CashAmmo300Object(
            const int32_t& x,
            const int32_t& y) :
        BaseObject(x, y,
            CASH_AMMO_300_WIDTH,
            CASH_AMMO_300_HEIGHT,
            CASH_AMMO_300_HP,
            CASH_AMMO_300_SPEED,
            0,
            "cash-ammo-300-bucks"),
        m_pos_x(x) {
    }

    virtual ~CashAmmo300Object() {
        if (is_destroy()) {
            play_pickup();
        }
    }

    ObjectCategory get_category() override final {
        return ObjectCategory::AMMO;
    }

    ObjectType get_type() override final {
        return ObjectType::CASH_AMMO_300;
    }

    bool is_online() override final {
        ResourceManager& instance = ResourceManager::get_instance();
        const int32_t area_height = instance.get_height();
        return (y < (area_height + height));
    }

    void update() override final {
        static const double step = 2.0 * std::acos(-1) / 45.0;
        const double sin_val = std::sin(m_angle);
        angle = 10.0 * sin_val;
        y += speed;
        x = m_pos_x + (20.0 * sin_val);
        m_angle += step;
    }

private:
    double  m_angle = 0.0;
    int32_t m_pos_x = 0;

    void play_pickup() {
        EM_ASM({
            const soundСashRegister = new Audio('sounds/cash_register.mp3');
            soundСashRegister.loop = false;
            soundСashRegister.addEventListener('ended', function() {
                this.remove();
            });
            soundСashRegister.play().catch(function(error) {
                console.log("Error playing sound: " + error);
            });

            const sound300Bucks = document.getElementById('sound-cash-ammo-300-bucks');
            if (sound300Bucks.paused || sound300Bucks.ended) {
                sound300Bucks.currentTime = 0; // Обнулить, чтобы звук начинался заново при каждом нажатии
                sound300Bucks.play().catch(function(error) {
                    console.log("Error playing sound: " + error);
                });
            }
        });
    }
};
