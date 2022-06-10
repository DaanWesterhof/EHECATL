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

//    void PID_Controller::StateRecieved(uint8_t command, uint8_t *data, uint8_t len){
//        DRONE_MODE state = data[0];
//        switch(state){
//            case DRONE_MODES::LANDING:
//                target_x_angle = 0;
//                target_y_angle = 0;
//                target_r_speed = 0;
//                target_y_speed = 0;
//                isFlying = false;
//        }
//    }

    PID_Controller::PID_Controller(communication &comms) : comms(comms) {
        comms.addNewCallback(MSG_COMMANDS::CURRENT_ANGLES, COMM_CALLBACK(GyroAnglesRecieved));
        comms.addNewCallback(MSG_COMMANDS::JOYSTICK_ANGLES, COMM_CALLBACK(newTargetAngles));
    }

    void PID_Controller::updatePids(float x_angle, float y_angle, float r_speed) {
        float x_angle_correction, y_angle_correction, rotation_angle_correction = 0;
        x_angle_correction = x_pid.calulateAction(x_angle, target_x_angle);
        y_angle_correction = y_pid.calulateAction(y_angle, target_y_angle);
        rotation_angle_correction = r_pid.calulateAction(r_speed, target_r_speed);
        //do calculation for motors for x and y z
        // add base onto it to keep flying, y height things
        motor_change_values[0] = int(x_angle_correction);
        motor_change_values[1] = int(x_angle_correction);
        motor_change_values[2] = -int(x_angle_correction);
        motor_change_values[3] = -int(x_angle_correction);

        motor_change_values[0] += int(y_angle_correction);
        motor_change_values[1] += int(y_angle_correction);
        motor_change_values[2] += -int(y_angle_correction);
        motor_change_values[3] += -int(y_angle_correction);

        motor_change_values[0] += int(rotation_angle_correction);
        motor_change_values[1] += -int(rotation_angle_correction);
        motor_change_values[2] += int(rotation_angle_correction);
        motor_change_values[3] += -int(rotation_angle_correction);

        comms.localMessage(MSG_COMMANDS::MOTOR_DIFFERENCE, (uint8_t* ) motor_change_values, 5*4);
    }

    void PID_Controller::GyroAnglesRecieved(uint8_t command, uint8_t *data, uint8_t len) {
        float *gdata = (float *) data;
        float current_speed = (gdata[2] - old_r_pos) / (HAL_GetTick() - hal_last_tick) * 1000;
        hal_last_tick = HAL_GetTick();
        old_r_pos = gdata[2];
        updatePids(gdata[0], gdata[1], current_speed);
    }

    void PID_Controller::newTargetAngles(uint8_t command, uint8_t *data, uint8_t len) {
        target_x_angle = data[0];
        target_y_angle = data[1];
        target_r_speed = data[2];
        target_y_speed = data[3];
        isFlying = true;
    }

}