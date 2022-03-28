//
// Created by daang on 25-2-2022.
//

#ifndef EHECATL_MOTORS_HPP
#define EHECATL_MOTORS_HPP
#include "communication.hpp"
#include "dshot.h"
#include "tim.h"
#include "PID.hpp"

namespace EHECATL {

    class Motors {
        int base_speed = 0;
        int current_offset = 0;
        int motor_speeds[4] = {};
        bool PID_Controll_Speed;
        float desired_height = 0;
        float current_height = 0;
        communication &comms;

        PID height_pid = PID(0.3, 0.3, 0.3, 0);

    public:
        Motors(communication &comms);

        /**
         * Set the new motor speeds.
         * @param command The command used to call this function
         * @param data The payload of the message, Should be the desired change is speed per motor and base speed.
         * @param len the length of the payload/data in bytes
         */
        void setMotorSpeedsForRotations(uint8_t command, uint8_t * data, uint8_t len);
        void setBaseSpeed(uint8_t command, uint8_t * data, uint8_t len);

    };

}

#endif //EHECATL_MOTORS_HPP
