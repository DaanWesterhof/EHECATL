//
// Created by daang on 7-2-2022.
//

#include "joysticks.hpp"

namespace EHECATL {

    joystick::joystick(ADC_HandleTypeDef &hadc, communication &comms, Canvas &canvas) : hadc(hadc), comms(comms), canvas(canvas) {}



    void mapit(){

    }

    void joystick::sendSticks() {
        int16_t temp_data[4];
        temp_data[0] =        Read_ADC_Channel(&hadc, ADC_CHANNEL_3, 200);// speed
        temp_data[2] = 4096 - Read_ADC_Channel(&hadc, ADC_CHANNEL_1, 200);//y angle
        temp_data[1] =        Read_ADC_Channel(&hadc, ADC_CHANNEL_2, 200);//x angle
        temp_data[3] = 4096 - Read_ADC_Channel(&hadc, ADC_CHANNEL_4, 200);//z rotation
        bool equal = true;
        for (int i = 0; i < 4; i++) {
            if (std::abs(temp_data[i] - data[i]) > 30) {
                data[i] = temp_data[i];
                equal = false;
            }
        }

        char line_data[40];

        //change to percentages
        for(int i = 0; i < 4; i++) {
            if (temp_data[i] >= middle_values[i]) {
                temp_data[i] = (float(temp_data[i] - middle_values[i]) / float(4096 - middle_values[i])) * 50;
            } else {
                temp_data[i] = -50 + (float(temp_data[i]) / float(middle_values[i]) * 50);
            }
        }


        sprintf(line_data, "0: %4d, 3: %4d", temp_data[0], temp_data[3]);
        canvas.writeAndFlushLine(0, 6, line_data, strlen(line_data), 0);

        sprintf(line_data, "1: %4d, 2: %4d", temp_data[1], temp_data[2]);
        canvas.writeAndFlushLine(0, 7, line_data, strlen(line_data), 0);

        if (!equal) {
            comms.sendMessage(MSG_COMMANDS::JOYSTICK_ANGLES, (uint8_t *) temp_data, sizeof(int16_t) * 4);
        }
    }

    void joystick::startup() {
        unsigned int buffers[4];
        for (int i = 0; i < 30; i++) {
            if(i > 9) {
                buffers[0] +=        Read_ADC_Channel(&hadc, ADC_CHANNEL_3, 200);
                buffers[2] += 4096 - Read_ADC_Channel(&hadc, ADC_CHANNEL_1, 200);
                buffers[1] +=        Read_ADC_Channel(&hadc, ADC_CHANNEL_2, 200);
                buffers[3] += 4096 - Read_ADC_Channel(&hadc, ADC_CHANNEL_4, 200);
                HAL_Delay(10);
            }
        }
        middle_values[0] = 2000;
        middle_values[1] = buffers[1] / 21;
        middle_values[2] = buffers[2] / 21;
        middle_values[3] = 2000;
    }

    void joystick::update() {
        if (HAL_GetTick() - last_check > 10) {
           sendSticks();
            last_check = HAL_GetTick();
        }


        //check if buttons are pressed, if so send message to system.
        GPIO_PinState pressedl = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14);
        GPIO_PinState pressedr = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15);

        if (pressedl == 0 && !ispressedl) {
            ispressedl = true;
            button_message_data[0] = 1;
            button_message_data[1] = 1;
            comms.localMessage(EHECATL::MISC_MESSAGES::BUTTON_STATE_CHANGE, button_message_data, 2);
        } else if (pressedl == 1 && ispressedl) {
            ispressedl = false;
            button_message_data[0] = 1;
            button_message_data[1] = 0;
            comms.localMessage(EHECATL::MISC_MESSAGES::BUTTON_STATE_CHANGE, button_message_data, 2);
        }

        if (pressedr == 0 && !ispressedr) {
            ispressedr = true;
            button_message_data[0] = 2;
            button_message_data[1] = 1;
            comms.localMessage(EHECATL::MISC_MESSAGES::BUTTON_STATE_CHANGE, button_message_data, 2);
        } else if (pressedr == 1 && ispressedr) {
            ispressedr = false;
            button_message_data[0] = 2;
            button_message_data[1] = 0;
            comms.localMessage(EHECATL::MISC_MESSAGES::BUTTON_STATE_CHANGE, button_message_data, 2);
        }

    }
}
