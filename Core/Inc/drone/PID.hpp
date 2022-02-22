//
// Created by daang on 7-2-2022.
//

#ifndef EHECATL_PID_HPP
#define EHECATL_PID_HPP

#include "stm32f4xx_hal.h"
#include "MPU6050.hpp"

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

        int calulateAction(int desired_value, int actual_value) {
            dt = HAL_GetTick() - last_dt;
            error = actual_value - desired_value;
            sum_error += error;
            int actie = (KP * error) + (KI * (sum_error * (dt / 1000))) + (KD * ((error - previous_error) / (dt / 1000)));
            previous_error = error;
            last_dt = HAL_GetTick();
            return actie;
        }

        PID(float kp, float ki, float kd, float bias) : KP(kp), KI(ki), KD(kd), bias(bias) {

        }

    };


    class PID_Controller {
    private:
        int target_x_speed = 0;
        int target_z_speed = 0;
        int target_y_speed = 0;
        int target_r_speed = 0;
        uint8_t motor_driver_speeds[4] = {};

        PID y_pid = PID(0.3, 0, 0, 0);
        PID x_pid = PID(0.3, 0, 0, 0);
        PID z_pid = PID(0.3, 0, 0, 0);
        PID r_pid = PID(0.3, 0, 0, 0);

//base = y value

    public:

        void update(int x_speed, int z_speed, int y_speed, int r_speed){
            int x, y, z, r = 0;
            x = x_pid.calulateAction(x_speed, target_x_speed);
            y = y_pid.calulateAction(y_speed, target_y_speed);
            z = z_pid.calulateAction(z_speed, target_z_speed);
            r = r_pid.calulateAction(r_speed, target_r_speed);
        }

    };

}


#endif //EHECATL_PID_HPP
