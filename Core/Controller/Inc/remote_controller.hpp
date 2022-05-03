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
        EHECATL::communication &comms;
        int state = 1;
    public: //variables

    private: //functions

    public: //functions
        void print_state(uint8_t command, uint8_t * payload, uint8_t len){
            char data[100];
            sprintf(data, "new_state: %u\n", *payload);
            HAL_UART_Transmit(&huart1, data, strlen(data), 100);
        }

        controller(TIM_HandleTypeDef &timer, EHECATL::communication &communicator) : timer(timer), comms(communicator){
            comms.addNewCallback(EHECATL::MSG_COMMANDS::NEW_STATE, COMM_CALLBACK(print_state));
        }

    };
}

#endif //EHECATL_REMOTE_CONTROLLER_HPP
