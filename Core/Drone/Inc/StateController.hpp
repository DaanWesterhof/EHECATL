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

        void state_up(uint8_t command, uint8_t * payload, uint8_t len);
        void state_down(uint8_t command, uint8_t * payload, uint8_t len);

        void speedListener(double speed);

    public:
        DRONE_MODE getState();

        void setState(DRONE_MODE new_state);
    };


}

#endif //EHECATL_STATECONTROLLER_HPP
