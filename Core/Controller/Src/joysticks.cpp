//
// Created by daang on 7-2-2022.
//

#include "joysticks.hpp"

namespace EHECATL{

    joystick::joystick(ADC_HandleTypeDef &hadc, communication &comms) : hadc(hadc), comms(comms) {}


    void joystick::sendSticks() {
        data[0] = float((Read_ADC_Channel(&hadc, ADC_CHANNEL_1, 200)/4094.0*5)-middle_values[0]);
        data[1] = float((Read_ADC_Channel(&hadc, ADC_CHANNEL_2, 200)/4094.0*5)-middle_values[1]);
        data[2] = 1024.0-float((Read_ADC_Channel(&hadc, ADC_CHANNEL_3, 200)/4094.0*1)-middle_values[2]);
        data[3] = 1024.0-float((Read_ADC_Channel(&hadc, ADC_CHANNEL_4, 200)/4094.0*20)-middle_values[3]);
        comms.sendMessage(MSG_COMMANDS::JOYSTICK_ANGLES, (uint8_t*)data, 16);
    }

    void joystick::startup() {
        float buffers[4];
        for(int i = 0; i < 100; i++){
            buffers[0] += float(Read_ADC_Channel(&hadc, ADC_CHANNEL_1, 200)/4094.0*5);
            buffers[1] += float(Read_ADC_Channel(&hadc, ADC_CHANNEL_2, 200)/4094.0*5);
            buffers[2] += float(Read_ADC_Channel(&hadc, ADC_CHANNEL_3, 200)/4094.0*1);
            buffers[3] += float(Read_ADC_Channel(&hadc, ADC_CHANNEL_4, 200)/4094.0*20);
            HAL_Delay(10);
        }
        middle_values[0] = float(buffers[0]/100);
        middle_values[1] = float(buffers[1]/100);
        middle_values[2] = float(buffers[2]/100);
        middle_values[3] = float(buffers[3]/100);
    }

    void joystick::update() {
        sendSticks();
    }
}
