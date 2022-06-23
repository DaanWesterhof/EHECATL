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
        temp_data[0] = Read_ADC_Channel(&hadc, ADC_CHANNEL_3, 200);// speed
        temp_data[1] = Read_ADC_Channel(&hadc, ADC_CHANNEL_1, 200);//y angle
        temp_data[2] = Read_ADC_Channel(&hadc, ADC_CHANNEL_2, 200);//x angle
        temp_data[3] = Read_ADC_Channel(&hadc, ADC_CHANNEL_4, 200);//z rotation
        bool equal = true;
        for (int i = 0; i < 4; i++) {
            if (std::abs(temp_data[i] - data[i]) > 30) {
                data[i] = temp_data[i];
                equal = false;
            }
        }

        char line_data[40];
        sprintf(line_data, "0: %d, 3: %d", temp_data[0], temp_data[3]);
        canvas.writeAndFlushLine(0, 9, line_data, strlen(line_data), 0);

        //change to percentages
        for(int i = 0; i < 4; i++) {
            if (temp_data[i] >= middle_values[i]) {
                temp_data[i] = (float(temp_data[i] - middle_values[i]) / float(4096 - middle_values[i] + 30)) * 50;
            } else {
                temp_data[i] = -50 + (float(temp_data[i]) / float(middle_values[i]) * 50);
            }
        }

        if (!equal) {
            comms.sendMessage(MSG_COMMANDS::JOYSTICK_ANGLES, (uint8_t *) temp_data, sizeof(int16_t) * 4);
        }
    }

    void joystick::startup() {
        unsigned int buffers[4];
        for (int i = 0; i < 100; i++) {
            buffers[0] += Read_ADC_Channel(&hadc, ADC_CHANNEL_3, 200);
            buffers[1] += Read_ADC_Channel(&hadc, ADC_CHANNEL_1, 200);
            buffers[2] += Read_ADC_Channel(&hadc, ADC_CHANNEL_2, 200);
            buffers[3] += Read_ADC_Channel(&hadc, ADC_CHANNEL_4, 200);
            HAL_Delay(10);
        }
        middle_values[0] = buffers[0] / 100;
        middle_values[1] = buffers[1] / 100;
        middle_values[2] = buffers[2] / 100;
        middle_values[3] = buffers[3] / 100;
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
