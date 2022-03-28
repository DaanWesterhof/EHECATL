//
// Created by daang on 7-2-2022.
//

#ifndef EHECATL_PID_HPP
#define EHECATL_PID_HPP

#include "stm32f4xx_hal.h"
#include "mpu60x0.hpp"

namespace EHECATL {

    class PID {
    public:
        float KP = 0;
        float KI = 0;
        float KD = 0;

        float bias = 0;
        float value = 0;
        float error = 0;
        int sum_error = 0;
        int previous_error = 0;
        uint32_t dt = 0;
        uint32_t last_dt = 0;

        float calulateAction(float desired_value, float actual_value);

        PID(float kp, float ki, float kd, float bias) : KP(kp), KI(ki), KD(kd), bias(bias) {

        }

    };


    class PID_Controller {
    private:
        communication &comms;
        float target_x_angle = 0;
        float target_y_angle = 0;
        float target_r_speed = 0;
        float target_y_speed = 0;

        float old_r_pos = 0;
        int hal_last_tick = 0;
        uint8_t motor_driver_speeds[4] = {};

        int motor_change_values[5] = {};

        PID x_pid = PID(0.3, 0, 0, 0);
        PID y_pid = PID(0.3, 0, 0, 0);
        PID r_pid = PID(0.3, 0, 0, 0);

        /**
         * Uses the PIDS to calculate new changes for the motor speeds
         * @param x_angle Current X angle of the drone
         * @param y_angle Current Y angle of the drone
         * @param r_speed Current rotating speed of the drone
         */
        void updatePids(float x_angle, float y_angle, float r_speed);

//base = y value

    public:
        PID_Controller(communication &comms);



        /**
         * Function callback called by the communicator if new mpu6050 angles have been recieved
         * @param command The commond with wich the function is called
         * @param data the payload/data of the message, should be the xy angles of the mpu6050 and the z rotating speed
         * @param len the length of the payload in bytes
         */
        void GyroAnglesRecieved(uint8_t command, uint8_t * data, uint8_t len);

        /**
         * Function callback called by the communicator if new target angles have been recieved from the controller
         * @param command The cmmand with wich the callback was called
         * @param data the payload/data of the message.
         * @param len the length of the payload in bytes
         */
        void newTargetAngles(uint8_t command, uint8_t * data, uint8_t len);
    };

}


#endif //EHECATL_PID_HPP
