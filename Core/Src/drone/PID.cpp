//
// Created by daang on 7-2-2022.
//

#include "PID.hpp"

namespace EHECATL{

    float PID::calulateAction(float desired_value, float actual_value) {
        dt = HAL_GetTick() - last_dt;
        error = actual_value - desired_value;
        sum_error += error;
        int actie = (KP * error) + (KI * (sum_error * (dt / 1000))) + (KD * ((error - previous_error) / (dt / 1000)));
        previous_error = error;
        last_dt = HAL_GetTick();
        return actie;
    }

    PID_Controller::PID_Controller(communication &comms) : comms(comms) {
        comms.addNewCallback(MSG_COMMANDS::CURRENT_ANGLES, COMM_CALLBACK(GyroAnglesRecieved));
        comms.addNewCallback(MSG_COMMANDS::JOYSTICK_ANGLES, COMM_CALLBACK(newTargetAngles));
    }

    void PID_Controller::updatePids(float x_angle, float y_angle, float r_speed) {
        float x, y, r = 0;
        x = x_pid.calulateAction(x_angle, target_x_angle);
        y = y_pid.calulateAction(y_angle, target_y_angle);
        r = r_pid.calulateAction(r_speed, target_r_speed);
        //do calculation for motors for x and y z
        // add base onto it to keep flying, y height things
        motor_change_values[0] = int(x/2);
        motor_change_values[1] = int(x/2);
        motor_change_values[2] = -int(x/2);
        motor_change_values[3] = -int(x/2);

        motor_change_values[0] += int(y/2);
        motor_change_values[1] += int(y/2);
        motor_change_values[2] += -int(y/2);
        motor_change_values[3] += -int(y/2);

        motor_change_values[0] += int(r/2);
        motor_change_values[1] += -int(r/2);
        motor_change_values[2] += int(r/2);
        motor_change_values[3] += -int(r/2);

        motor_change_values[0] += int(r/2);
        motor_change_values[1] += -int(r/2);
        motor_change_values[2] += int(r/2);
        motor_change_values[3] += -int(r/2);

        motor_change_values[4] = int(target_y_speed/4);

        comms.localMessage(MSG_COMMANDS::MOTOR_DIFFERENCE, (uint8_t* ) motor_change_values, 5*4);
    }

    void PID_Controller::GyroAnglesRecieved(uint8_t command, uint8_t *data, uint8_t len) {
        float * gdata = (float*)data;
        updatePids(gdata[0], gdata[1], gdata[2]);
    }

    void PID_Controller::newTargetAngles(uint8_t command, uint8_t *data, uint8_t len) {
        target_x_angle = data[0];
        target_y_angle = data[1];
        target_r_speed = data[2];
        target_y_speed = data[3];
    }
}