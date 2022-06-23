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
        uint32_t time_since_state = 0;
        bool disconected = true;

        DRONE_MODE recieved_state = DRONE_MODES::SLEEP;
    public: //variables

    private: //functions

        void sendNewState(DRONE_MODE desired_state){
            uint8_t state_data[1] = {desired_state};
            comms.sendMessage(EHECATL::MSG_COMMANDS::DESIRED_STATE, state_data, 1);
            char data[30];
            sprintf(data, "send_state: %u\n", desired_state);
            HAL_UART_Transmit(&huart1, data, strlen(data), 100);
        }

    public: //functions
        void print_state(uint8_t command, uint8_t * payload, uint8_t len){
            char data[10];
            recieved_state = *payload;
            sprintf(data, "new_state: %u\n", *payload);
            HAL_UART_Transmit(&huart1, data, strlen(data), 100);
            time_since_state = HAL_GetTick();
            if(disconected) {
                char error[] = "Connected";
                comms.localMessage(EHECATL::MSG_COMMANDS::CONNECTION_STATE, (uint8_t *) error, strlen(error));
                disconected = false;
            }
        }

        controller(TIM_HandleTypeDef &timer, EHECATL::communication &communicator) : timer(timer), comms(communicator){
            comms.addNewCallback(EHECATL::MSG_COMMANDS::NEW_STATE, COMM_CALLBACK(print_state));
            comms.addNewCallback(EHECATL::MISC_MESSAGES::BUTTON_STATE_CHANGE, COMM_CALLBACK(state_switcher));
            char error[] ="Disconected";
            comms.localMessage(EHECATL::MSG_COMMANDS::CONNECTION_STATE, (uint8_t *)error, strlen(error));
        }

        void state_switcher(uint8_t command, uint8_t * payload, uint8_t len){
            if(payload[1] == 1){//a button has been pressed
                if(payload[0] == 2){ //its the right button
                    uint8_t p_data[1] = {0};
                    comms.sendMessage(EHECATL::MSG_COMMANDS::STATE_UP, p_data, 1);
                }
                if(payload[0] == 1){ //its the left button
                    uint8_t p_data[1] = {0};
                    comms.sendMessage(EHECATL::MSG_COMMANDS::STATE_DOWN, p_data, 1);
                }
            }

        }

        void update(){
            if(HAL_GetTick() - time_since_state > 3000){
                char error[] ="Disconected";
                comms.localMessage(EHECATL::MSG_COMMANDS::CONNECTION_STATE, (uint8_t *)error, strlen(error));
                disconected = true;
            }
            if(disconected){
                char text[] = "disconected\n";
                HAL_UART_Transmit(&huart1, text, strlen(text), 100);
            }else{
                char text2[] = "conected\n";
                HAL_UART_Transmit(&huart1, text2, strlen(text2), 100);
            }
        }

    };
}

#endif //EHECATL_REMOTE_CONTROLLER_HPP
