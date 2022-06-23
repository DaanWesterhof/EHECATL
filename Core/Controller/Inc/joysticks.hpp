//
// Created by daang on 7-2-2022.
//

#ifndef EHECATL_JOYSTICKS_HPP
#define EHECATL_JOYSTICKS_HPP

#include "communication.hpp"
#include "ADC_helper.hpp"
#include "Screen.hpp"

namespace EHECATL{

    /**
     * Class for the joysticks of the drone controller, will read and send the new positions to the drone
     */
    class joystick{

        ADC_HandleTypeDef &hadc;
        communication &comms;

        Canvas & canvas;
        int16_t data[4] = {};
        int16_t middle_values[4] = {};

        bool ispressedl = false;
        bool ispressedr = false;
        uint8_t button_message_data[2];

        unsigned int last_check = 0;


    private://functions

        /**
         * Send joystick data to the drone
         */
        void sendSticks();

    public: //functions
        /**
         * Constructor for the joystick class
         * @param hadc A reference to the ADC handle
         * @param comms a reference to the communication class
         */
        joystick(ADC_HandleTypeDef &hadc, communication &comms, Canvas &canvas);

        /**
         * Startup for the joystick class, used to find the resting positions of the controller, needs to be called once, before update
         */
        void startup();

        /**
         * Update the joystick values, will send new values to the drone
         */
        void update();
    };

}

#endif //EHECATL_JOYSTICKS_HPP
