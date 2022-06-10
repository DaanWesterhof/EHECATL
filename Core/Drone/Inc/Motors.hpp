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
        uint16_t motor_speeds[4] = {};

        int16_t desired_change_height[4] = {};
        int16_t desired_change_movement[4] = {};


        bool PID_Controll_Speed = false;
        float desired_height = 0;
        float current_height = 0;
        float base_height = 0;
        double desired_speed = 0;
        communication &comms;

        double land_speed = -0.1;


        PID height_pid = PID(0.3, 0.3, 0.3, 0);
        PID v_speed_pid = PID(0.3, 0.3, 0.3, 0);

    public:

        uint32_t time_since_motor = 0;
        bool isFlying= false;


        void getChange(int16_t * change){
            for(int i = 0; i < 4; i++){
                change[i] += desired_change_height[i];
                change[i] += desired_change_movement[i];
            }
        }


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
        void hoverController(uint8_t command, uint8_t * data, uint8_t len);

        /**
         * Callback function for the communication system. Sets the motor speeds for vertical movement based on user joystick controll.
         * @param command The command used to call this function
         * @param data The payload of the callback message, should be a pointer to an Int value.
         * @param len The length of the payload.
         */
        void setMotorsForVerticalSpeed(uint8_t command, uint8_t *data, uint8_t len);

        void StateRecieved(uint8_t command, uint8_t *data, uint8_t len);

        void temp_motor_tester(uint8_t command, uint8_t *data, uint8_t len);

    };

}

#endif //EHECATL_MOTORS_HPP
