//
// Created by daang on 28-4-2022.
//

#ifndef EHECATL_STATECONTROLLER_HPP
#define EHECATL_STATECONTROLLER_HPP

#include "communication.hpp"
namespace EHECATL {

    class StateController {
        DRONE_MODE current_state;
        communication & comms;
    public:
        StateController(communication &comms) : comms(comms) {
            current_state = DRONE_MODES::SETUP;
        }

    private:

        void newDesiredState(uint8_t command, uint8_t * data, uint8_t len){
            DRONE_MODE state = data[0];
            switch(state){
                case DRONE_MODES::SLEEP:
                case DRONE_MODES::GOING_TO_SLEEP:
                    if(current_state == DRONE_MODES::IDLE){
                        setState(DRONE_MODES::GOING_TO_SLEEP);
                    }
                    break;

                case DRONE_MODES::IDLE:
                    if(current_state == DRONE_MODES::LANDING){
                        setState(state);
                    }else if(current_state == DRONE_MODES::SLEEP){
                        setState(DRONE_MODES::SETUP);
                    }
                    break;

                case DRONE_MODES::FLYING:
                    if(current_state == DRONE_MODES::IDLE){
                        setState(state);
                    }
                    break;

                case DRONE_MODES::LANDING:
                    if(current_state == DRONE_MODES::FLYING){
                        setState(state);
                    }
                    break;

                case DRONE_MODES::SETUP:
                    if(current_state == DRONE_MODES::IDLE || current_state == DRONE_MODES::SLEEP){
                        setState(state);
                    }
                    break;

                default:
                    break;


            }

        }

        void speedListener(double speed){
            if(current_state == DRONE_MODES::LANDING) {
                if (speed < 0.005 && speed > -0.005) {
                    setState(DRONE_MODES::IDLE);
                }
            }
        }

    public:
        DRONE_MODE getState(){
            return current_state;
        }

        void setState(DRONE_MODE new_state){
            current_state = new_state;
            comms.localMessage(MSG_COMMANDS::NEW_STATE, &current_state, 1);
            uint8_t data[1];
            data[0] = current_state;
            int res = comms.sendMessage(MSG_COMMANDS::NEW_STATE, data, 1);
            char data1[100];
            sprintf(data1, "we got send_result: %d\n", res);
            HAL_UART_Transmit(&huart1, data1, strlen(data1), 100);
        }
    };


}

#endif //EHECATL_STATECONTROLLER_HPP
