//
// Created by daang on 7-2-2022.
//

#include "joysticks.hpp"

namespace EHECATL{

    joystick::joystick(ADC_HandleTypeDef &hadc, communication &comms) : hadc(hadc), comms(comms) {}


    void joystick::sendSticks() {
        float temp_data[4];
        temp_data[0] = -float((Read_ADC_Channel(&hadc, ADC_CHANNEL_1, 200)/4094.0*5)-middle_values[0]);
        temp_data[1] = float((Read_ADC_Channel(&hadc, ADC_CHANNEL_2, 200)/4094.0*5)-middle_values[1]);
        temp_data[2] = float((Read_ADC_Channel(&hadc, ADC_CHANNEL_3, 200)/4094.0*5)-middle_values[2]);
        temp_data[3] = -float((Read_ADC_Channel(&hadc, ADC_CHANNEL_4, 200)/4094.0*5)-middle_values[3]);
        bool equal = true;
        for(int i = 0; i < 4; i++){
            if(std::abs(temp_data[i] - data[i]) > 0.01){
                data[i] = temp_data[i];
                equal = false;
            }
        }

        if(!equal) {
            char data[100];
            sprintf(data, "angles: x1:%f \t, x2:%f \t, y1:%f \t, y2:%f\n", temp_data[3], temp_data[1], temp_data[2], temp_data[0]);
            HAL_UART_Transmit(&huart1, data, strlen(data), 100);
            comms.sendMessage(MSG_COMMANDS::JOYSTICK_ANGLES, (uint8_t *) data, 16);
        }
    }

    void joystick::startup() {
        float buffers[4];
        for(int i = 0; i < 100; i++){
            buffers[0] += float(Read_ADC_Channel(&hadc, ADC_CHANNEL_1, 200)/4094.0*5);
            buffers[1] += float(Read_ADC_Channel(&hadc, ADC_CHANNEL_2, 200)/4094.0*5);
            buffers[2] += float(Read_ADC_Channel(&hadc, ADC_CHANNEL_3, 200)/4094.0*5);
            buffers[3] += float(Read_ADC_Channel(&hadc, ADC_CHANNEL_4, 200)/4094.0*5);
            HAL_Delay(10);
        }
        middle_values[0] = float(buffers[0]/100);
        middle_values[1] = float(buffers[1]/100);
        middle_values[2] = float(buffers[2]/100);
        middle_values[3] = float(buffers[3]/100);
    }

    void joystick::update() {
        if(HAL_GetTick() - last_check > 50) {
            sendSticks();
            last_check = HAL_GetTick();
        }


        //check if buttons are pressed, if so send message to system.
        GPIO_PinState pressedl = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14);
        GPIO_PinState pressedr = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15);

        if(pressedl==0 && !ispressedl){
            ispressedl = true;
            button_message_data[0] = 1;
            button_message_data[1] = 1;
            comms.localMessage(EHECATL::MISC_MESSAGES::BUTTON_STATE_CHANGE, button_message_data, 2);
        }
        else if(pressedl == 1 && ispressedl){
            ispressedl = false;
            button_message_data[0] = 1;
            button_message_data[1] = 0;
            comms.localMessage(EHECATL::MISC_MESSAGES::BUTTON_STATE_CHANGE, button_message_data, 2);
        }

        if(pressedr==0 && !ispressedr){
            ispressedr = true;
            button_message_data[0] = 2;
            button_message_data[1] = 1;
            comms.localMessage(EHECATL::MISC_MESSAGES::BUTTON_STATE_CHANGE, button_message_data, 2);
        }
        else if(pressedr == 1 && ispressedr){
            ispressedr = false;
            button_message_data[0] = 2;
            button_message_data[1] = 0;
            comms.localMessage(EHECATL::MISC_MESSAGES::BUTTON_STATE_CHANGE, button_message_data, 2);
        }

    }
}
