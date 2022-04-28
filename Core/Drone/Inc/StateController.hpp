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

        void newDesiredState(){

        }

        void sendCurrentState(){

        }

    public:
        DRONE_MODE getState(){
            return current_state;
        }

        void setState(DRONE_MODE new_state){
            current_state = new_state;
            comms.localMessage(MSG_COMMANDS::NEW_STATE, &current_state, 1);
            comms.sendMessage(MSG_COMMANDS::NEW_STATE, &current_state, 1);
        }
    };


}

#endif //EHECATL_STATECONTROLLER_HPP
