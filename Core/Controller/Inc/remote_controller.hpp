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

        DRONE_MODE recieved_state = DRONE_MODES::SLEEP;
    public: //variables

    private: //functions

        void sendNewState(DRONE_MODE desired_state){
            uint8_t state_data[1] = {desired_state};
            comms.sendMessage(EHECATL::MSG_COMMANDS::DESIRED_STATE, state_data, 1);
            char data[100];
            sprintf(data, "send_state: %u\n", desired_state);
            HAL_UART_Transmit(&huart1, data, strlen(data), 100);
        }

    public: //functions
        void print_state(uint8_t command, uint8_t * payload, uint8_t len){
            char data[100];
            recieved_state = *payload;
            sprintf(data, "new_state: %u\n", *payload);
            HAL_UART_Transmit(&huart1, data, strlen(data), 100);
        }

        controller(TIM_HandleTypeDef &timer, EHECATL::communication &communicator) : timer(timer), comms(communicator){
            comms.addNewCallback(EHECATL::MSG_COMMANDS::NEW_STATE, COMM_CALLBACK(print_state));
            comms.addNewCallback(EHECATL::MISC_MESSAGES::BUTTON_STATE_CHANGE, COMM_CALLBACK(state_switcher));
        }

        void state_switcher(uint8_t command, uint8_t * payload, uint8_t len){
            if(payload[1] == 1){
                if(payload[0] == 2){
                    switch(recieved_state) {
                        case DRONE_MODES::SLEEP:
                            sendNewState(DRONE_MODES::SETUP);
                            break;
                        case DRONE_MODES::IDLE:
                        case DRONE_MODES::LANDING:
                            sendNewState(DRONE_MODES::FLYING);
                            break;
                        case DRONE_MODES::GOING_TO_SLEEP:
                        case DRONE_MODES::FLYING:
                        case DRONE_MODES::SETUP:
                        default:
                            break;//none
                    }
                }
                if(payload[0] == 1){
                    switch(recieved_state) {
                        case DRONE_MODES::IDLE:
                            sendNewState(DRONE_MODES::SLEEP);
                            break; //send desired_state sleep
                        case DRONE_MODES::FLYING:
                            sendNewState(DRONE_MODES::LANDING);
                            break; //send desired_state landing
                        case DRONE_MODES::SETUP:
                        case DRONE_MODES::LANDING:
                        case DRONE_MODES::SLEEP:
                        case DRONE_MODES::GOING_TO_SLEEP:
                        default:
                            break;//none
                    }
                }
            }

        }

    };
}

#endif //EHECATL_REMOTE_CONTROLLER_HPP
