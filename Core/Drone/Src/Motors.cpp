//
// Created by daang on 8-3-2022.
//

#include "Motors.hpp"
#include "MotorPWMThing.hpp"

namespace EHECATL {

    void Motors::setMotorSpeedsForRotations(uint8_t command, uint8_t *data, uint8_t len) {
        int *mdata = (int *) data;
        desired_change_movement[0] = mdata[0];
        desired_change_movement[1] = mdata[1];
        desired_change_movement[2] = mdata[2];
        desired_change_movement[3] = mdata[3];
    }



    void Motors::setMotorsForVerticalSpeed(uint8_t command, uint8_t *data, uint8_t len) {
        double current_speed = *(double *) data;
        if (desired_speed = 0) {
            PID_Controll_Speed = true;
        } else {
            PID_Controll_Speed = false;
            int diff = v_speed_pid.calulateAction(desired_speed, current_speed);
            desired_change_height[0] = diff;
            desired_change_height[1] = diff;
            desired_change_height[2] = diff;
            desired_change_height[3] = diff;
        }
    }

    void Motors::hoverController(uint8_t command, uint8_t *data, uint8_t len) {
        current_height = ((float *) data)[0];
        if (PID_Controll_Speed) {
            int diff = height_pid.calulateAction(desired_height, current_height);
            desired_change_height[0] = diff;
            desired_change_height[1] = diff;
            desired_change_height[2] = diff;
            desired_change_height[3] = diff;
        } else {
            desired_height = current_height;
        }
    }

    Motors::Motors(communication &comms) : comms(comms) {
        //comms.addNewCallback(MSG_COMMANDS::MOTOR_DIFFERENCE, COMM_CALLBACK(setMotorSpeedsForRotations));
        //comms.addNewCallback(MSG_COMMANDS::NEW_BAROMETER_DATA, COMM_CALLBACK(hoverController));
        //comms.addNewCallback(MSG_COMMANDS::ALTITUDE_SPEED, COMM_CALLBACK(setMotorsForVerticalSpeed));
        comms.addNewCallback(MSG_COMMANDS::JOYSTICK_ANGLES, COMM_CALLBACK(temp_motor_tester));
    }

    void Motors::StateRecieved(uint8_t command, uint8_t *data, uint8_t len) {
        DRONE_MODE state = data[0];
        if (state == DRONE_MODES::FLYING) {
            isFlying = true;
        } else if(state == DRONE_MODES::IDLE) {
            isFlying = false;
            motor_speeds[0] = 0;
            motor_speeds[1] = 0;
            motor_speeds[2] = 0;
            motor_speeds[3] = 0;
            write_motor_speeds(motor_speeds);

        } else {
            isFlying = false;
            if(state == DRONE_MODES::LANDING){
                setMotorsForVerticalSpeed(0, (uint8_t *)&land_speed, 4);
            }
        }
    }

    void Motors::temp_motor_tester(uint8_t command, uint8_t *data, uint8_t len) {
       //  if(isFlying) {
        //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
            time_since_motor = HAL_GetTick();
            uint16_t *mdata = (uint16_t *) data;
            motor_speeds[0] = 1050 + mdata[0];
            motor_speeds[1] = 1050 + mdata[0];
            motor_speeds[2] = 1050 + mdata[0];
            motor_speeds[3] = 1050 + mdata[0];
            write_motor_speeds(motor_speeds);
       // }
    }
}