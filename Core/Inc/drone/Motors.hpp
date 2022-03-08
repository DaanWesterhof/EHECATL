//
// Created by daang on 25-2-2022.
//

#ifndef EHECATL_MOTORS_HPP
#define EHECATL_MOTORS_HPP
#include "communication.hpp"
namespace EHECATL {

    class Motors {
        int base_speed;
        int offset;
        int motor_speeds[4];
        communication &comms;
    public:
        Motors(communication &comms);

        /**
         * Set the new motor speeds.
         * @param command The command used to call this function
         * @param data The payload of the message, Should be the desired change is speed per motor and base speed.
         * @param len the length of the payload/data in bytes
         */
        void setMotorSpeeds(uint8_t command, uint8_t * data, uint8_t len);
    };

}

#endif //EHECATL_MOTORS_HPP
