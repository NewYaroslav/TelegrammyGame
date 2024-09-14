#pragma once

#include "BaseObject.hpp"

#define CLOUD_WIDTH             400
#define CLOUD_HEIGHT            200
#define CLOUD_SPEED             -10
#define CLOUD_HP                1

class CloudObject final : public BaseObject {
public:

    CloudObject(
            const int32_t& x,
            const int32_t& y,
            const int32_t& speed,
            const float& scale) :
        BaseObject(x, y,
            CLOUD_WIDTH,
            CLOUD_HEIGHT,
            CLOUD_HP,
            speed),
        m_scale(scale) {
    }

    ObjectType get_type() override final {
        return ObjectType::CLOUD;
    }

    bool is_online() override final {
        return (x >= -width);
    }

    void draw() override final {
        EM_ASM({
            const obj_id = $0;
            const obj_x = $1;
            const obj_y = $2;;
            const obj_width  = $3;
            const obj_height = $4;
            const obj_scale = $5;

            // Отображение облака
            const existing_cloud = document.getElementById(`obj_${obj_id}`);
            if (existing_cloud) {
                // Обновляем положение объекта
                existing_cloud.style.left = `${obj_x - obj_width / 2}px`;
                existing_cloud.style.top = `${obj_y - obj_height / 2}px`;
            } else {
                // Создаем новый объект, если он не существует
                const cloud = document.createElement('div');
                cloud.className = 'cloud';
                cloud.id = `obj_${obj_id}`;
                cloud.style.left = `${obj_x - obj_width / 2}px`;
                cloud.style.top = `${obj_y - obj_height / 2}px`;

                cloud.style.transform = `scale(${obj_scale})`;
                cloud.style.webkitTransform = `scale(${obj_scale})`; // для поддержки WebKit
                cloud.style.mozTransform = `scale(${obj_scale})`;    // для поддержки Mozilla

                const circle_1 = document.createElement('div');
                circle_1.className = 'largeCircle';
                circle_1.id = 'circ1';

                const circle_2 = document.createElement('div');
                circle_2.className = 'middleCircle';
                circle_2.id = 'circ2';

                const circle_3 = document.createElement('div');
                circle_3.className = 'middleCircle';
                circle_3.id = 'circ3';

                const circle_4 = document.createElement('div');
                circle_4.className = 'smallCircle';
                circle_4.id = 'circ4';

                const circle_5 = document.createElement('div');
                circle_5.className = 'smallCircle';
                circle_5.id = 'circ5';
                const shadow_5 = document.createElement('div');
                shadow_5.className = 'smallCircle';
                shadow_5.id = 'circ5shadow';
                circle_5.appendChild(shadow_5);

                const circle_6 = document.createElement('div');
                circle_6.className = 'smallCircle';
                circle_6.id = 'circ6';
                const shadow_6 = document.createElement('div');
                shadow_6.className = 'smallCircle';
                shadow_6.id = 'circ6shadow';
                circle_6.appendChild(shadow_6);

                cloud.appendChild(circle_1);
                cloud.appendChild(circle_2);
                cloud.appendChild(circle_3);
                cloud.appendChild(circle_4);
                cloud.appendChild(circle_5);
                cloud.appendChild(circle_6);

                document.getElementById('game-area').appendChild(cloud);
            }
        },
        id, x, y, width, height, m_scale);
    }

private:
    float m_scale = 1.0;
};
