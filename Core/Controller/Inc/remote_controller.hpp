//
// Created by daang on 7-2-2022.
//

#ifndef EHECATL_REMOTE_CONTROLLER_HPP
#define EHECATL_REMOTE_CONTROLLER_HPP


#include "stm32f4xx_hal.h"
#include "communication.hpp"
#include "ADC_helper.hpp"
#include "joysticks.hpp"


namespace EHECATL{

    class controller{
    private: //variables

        TIM_HandleTypeDef &timer;
        EHECATL::communication &communicator;
        int state = 1;
    public: //variables

    private: //functions

    public: //functions
        controller(TIM_HandleTypeDef &timer, EHECATL::communication &communicator) : timer(timer), communicator(communicator){}
    };
}

#endif //EHECATL_REMOTE_CONTROLLER_HPP
