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
        explicit Motors(communication &comms);

        /**
         * Callback function for the communication system. Sets the motor speeds for rotations based on data from the joysticks and mpu6050.
         * @param command The command used to call this function
         * @param data The payload of the message, Should be the desired change of speed per motor and base speed, should be a pointer to a list of 4 ints.
         * @param len the length of the payload/data in bytes
         */
        void setMotorSpeedsForRotations(uint8_t command, uint8_t * data, uint8_t len);

        /**
         * Callback function for the communication system. Sets the motor speeds when not modifying the Y speed.
         * It listens to data from the barometer, and uses a PID to calulate the base speed to stay at the desired height.
         * @param command The command used to call this function
         * @param data The payload of the callback message, should be a pointer to (a list of) 1 float.
         * @param len the length of the payload/data in bytes
         */
        void setBaseSpeed(uint8_t command, uint8_t * data, uint8_t len);

        /**
         * Callback function for the communication system. Sets the motor speeds for vertical movement based on user joystick controll.
         * @param command The command used to call this function
         * @param data The payload of the callback message, should be a pointer to an Int value.
         * @param len The length of the payload.
         */
        void setMotorSpeedsYSpeed(uint8_t command, uint8_t *data, uint8_t len);

    };

}

#endif //EHECATL_MOTORS_HPP
