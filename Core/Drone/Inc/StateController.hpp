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
        StateController(communication &comms);

    private:

        /**
         * callback for the state up command in the communication system, checks the current state and to what state the drone should move
         * @param command the command that called the function
         * @param payload the payload of the message
         * @param len the length of the payload
         */
        void state_up(uint8_t command, uint8_t * payload, uint8_t len);


        /**
         * callback for the state up command in the communication system, checks the current state and to what state the drone should move
         * @param command the command that called the function
         * @param payload the payload of the message
         * @param len the length of the payload
         */
        void state_down(uint8_t command, uint8_t * payload, uint8_t len);

    public:
        /**
         * returns the current state of the drone
         * @return the current state
         */
        DRONE_MODE getState();

        /**
         * sets the current state of the drone
         * @param new_state the new state
         */
        void setState(DRONE_MODE new_state);
    };


}

#endif //EHECATL_STATECONTROLLER_HPP
