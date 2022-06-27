//
// Created by daang on 27-6-2022.
//

#ifndef EHECATL_DRONECONTROLLER_H
#define EHECATL_DRONECONTROLLER_H

#include "communication.hpp"
#include "Barometer.hpp"
#include "ErrorPrinter.hpp"
#include "StateController.hpp"
#include "Motors.hpp"
#include <cstdio>
#include <cstring>
#include <MotorPWMThing.hpp>

namespace EHECATL{

    /**
     * class to control the state of the drone
     */
        class motorStateController {
            public:
            motorStateController(EHECATL::Motors & motors, EHECATL::communication & comms, EHECATL::StateController & stateController);


            private:
            EHECATL::Motors &motors;
            EHECATL::communication &comms;
            EHECATL::StateController &stateController;

            int16_t change[4] = {};

            uint16_t motor_speeds[4] = {};
            uint16_t max_value = 1500;
            bool send_joysticks = false;
            double drone_speed = 0;
            unsigned int time_since_landing = 0;
            bool landing = false;

            public:

            /**
             * callback that listens to the joystick data, if it recieves data while in landing mode it will switch to flying mode
             * @param command the command that called the function
             * @param payload the payload of the command
             * @param len the length of the payload
             */
            void joystickListener(uint8_t command, uint8_t *payload, uint8_t len);

            /**
             * callback that gets the vertical speed of the drone, this speed is used to check if we have landed.
             * @param command the command that called the function
             * @param payload the payload of the command
             * @param len the length of the payload
             */
            void setSpeed(uint8_t command, uint8_t *payload, uint8_t len);

            /**
             * callback that listens to the NEW_STATE state, if the new state is landing set landing motor speeds
             * @param command the command that called the function
             * @param payload the payload of the command
             * @param len the length of the payload
             */
            void state_listener(uint8_t command, uint8_t *payload, uint8_t len);

            /**
             * update the drone systems based on the current mode. Checks if the drone has landed, performs setup and flying systems depending on the mode.
             * @param barometer a reference to the barometer class
             * @param mpu a reference to the mpu class
             * @param telem a reference tot the telementry object
             */
            void update(EHECATL::Barometer &barometer, EHECATL::MPU_GYRO &mpu, EHECATL::telementry &telem);
        };
}


#endif //EHECATL_DRONECONTROLLER_H
