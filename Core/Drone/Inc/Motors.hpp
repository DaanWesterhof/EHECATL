//
// Created by daang on 25-2-2022.
//

#ifndef EHECATL_MOTORS_HPP
#define EHECATL_MOTORS_HPP
#include "communication.hpp"
#include "tim.h"
#include "PID.hpp"

namespace EHECATL {

    class Motors {
    public:
        uint16_t motor_speeds[4] = {};
    private:

        int16_t desired_change_height[4] = {};
        int16_t desired_change_movement[4] = {};


        bool PID_Controll_Speed = false;
        float desired_height = 0;
        float current_height = 0;
        float base_height = 0;
        double desired_speed = 0;
        communication &comms;

        uint16_t base_speed = 330;

        uint16_t max_added_speed = 150;
        uint16_t max_removed_speed = 150;

        double land_speed = -0.1;

        telementry & telm;


        PID height_pid = PID(0.3, 0.3, 0.3, 0);
        PID v_speed_pid = PID(0.3, 0.3, 0.3, 0);

        int value = 0;
        int last_value = 0;
        int current_value = 0;
        bool triggered = false;
        int16_t max_change = 100;
        int16_t min_change = -100;

    public:

        uint32_t time_since_motor = 0;
        bool isFlying= false;


        void getChange(int16_t * change){
            for(int i = 0; i < 4; i++){
                change[i] = desired_change_movement[i];
                if(change[i] > max_change){
                    change[i] = max_change;
                }else if(change[i] < min_change){
                    change[i] = min_change;
                }
            }
        }


        Motors(communication &comms, telementry & telm);

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
         * Callback function for the communication system. Sets the motor speeds for vertical movement based on user joystick controll in combination with a PID
         * @param command The command used to call this function
         * @param data The payload of the callback message, should be a pointer to an Int value.
         * @param len The length of the payload.
         */
        void setMotorsForVerticalSpeed(uint8_t command, uint8_t *data, uint8_t len);

        /**
         * Callback function that listens to the new_state command. if the state is flying, sets internal bool to true, if its idle it sets it to false
         * @param command The command used to call this function
         * @param data The payload of the callback message, should be a uint8_t indicating the new state
         * @param len The length of the payload.
         */
        void StateRecieved(uint8_t command, uint8_t *data, uint8_t len);

        /**
         * Callback function for the communictaion system, sets the motor speed for vertical movement directly based ont he joystick angles.
         * @param command The command used to call this function
         * @param data The payload of the callback message, should be a pointer to a list of floats of length 4
         * @param len The length of the payload.
         */
        void basicSpeedController(uint8_t command, uint8_t *data, uint8_t len);

    };

}

#endif //EHECATL_MOTORS_HPP
