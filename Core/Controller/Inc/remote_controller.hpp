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
    private:

        EHECATL::communication &comms;
        uint32_t time_since_state = 0;
        bool disconected = true;
        DRONE_MODE recieved_state = DRONE_MODES::SLEEP;

    public:


        controller(EHECATL::communication &communicator);

        /**
         * callback function that prints the new state received by the drone via the ack message and updates the connection status
         * @param command the command that called the function
         * @param payload the payload of the command
         * @param len the length of the command
         */
        void print_state(uint8_t command, uint8_t * payload, uint8_t len);

        /**
         * callback function that listens to button presses,
         * sends a state up command if button 2/the right button is pressed
         * sends a state down if button 1 /the left button is pressed
         * @param command the command that called the function
         * @param payload the payload of the command
         * @param len length of the payload
         */
        void state_switcher(uint8_t command, uint8_t * payload, uint8_t len);

        /**
         * update the controller, if more then 3 seconds have passed since the last ack package, set state to disconnected
         * and print the current state of connected/disconnected to the terminal.
         */
        void update();

    };
}

#endif //EHECATL_REMOTE_CONTROLLER_HPP
