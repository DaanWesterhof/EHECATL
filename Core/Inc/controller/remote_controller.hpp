//
// Created by daang on 7-2-2022.
//

#ifndef EHECATL_REMOTE_CONTROLLER_HPP
#define EHECATL_REMOTE_CONTROLLER_HPP


#include "stm32f4xx_hal.h"
#include "communication.hpp"
#include "ADC_helper.hpp"


namespace EHECATL{

    class controller{
    private: //variables

        TIM_HandleTypeDef &timer;
        EHECATL::communication &communicator;
        ADC_HandleTypeDef &hadc;
        int state = 1;
        uint8_t data[4] = {};

    public: //variables

    private: //functions

        void sendSticks(){
            data[0] = uint8_t((Read_ADC_Channel(&hadc, ADC_CHANNEL_1, 200)*0.062255));
            data[1] = uint8_t((Read_ADC_Channel(&hadc, ADC_CHANNEL_2, 200)*0.062255));
            data[2] = uint8_t((Read_ADC_Channel(&hadc, ADC_CHANNEL_3, 200)*0.062255));
            data[3] = uint8_t((Read_ADC_Channel(&hadc, ADC_CHANNEL_4, 200)*0.062255));
            communicator.sendMessage('0', data, 4);
        }


    public: //functions
        controller(TIM_HandleTypeDef &timer, EHECATL::communication &communicator, ADC_HandleTypeDef &hadc) : timer(timer), communicator(communicator), hadc(hadc) {}

        void update(){
            sendSticks();
        }

    };
}

#endif //EHECATL_REMOTE_CONTROLLER_HPP
