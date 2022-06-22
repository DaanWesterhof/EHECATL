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
        comms.addNewCallback(MSG_COMMANDS::NEW_STATE, COMM_CALLBACK(StateRecieved));
    }

    void Motors::StateRecieved(uint8_t command, uint8_t *data, uint8_t len) {
        DRONE_MODE state = data[0];
        if (state == DRONE_MODES::FLYING) {
            isFlying = true;
        } else if(state == DRONE_MODES::IDLE) {
            isFlying = false;
//            motor_speeds[0] = 0;
//            motor_speeds[1] = 0;
//            motor_speeds[2] = 0;
//            motor_speeds[3] = 0;
//            write_motor_speeds(motor_speeds);

        } else {
            if(state == DRONE_MODES::LANDING){
                //desired_speed = land_speed;
                //setMotorsForVerticalSpeed(0, (uint8_t *)&land_speed, 4);
            }
        }
    }

    void Motors::temp_motor_tester(uint8_t command, uint8_t *data, uint8_t len) {

        uint8_t text_buffer[100];
        int16_t * mdata = (int16_t * )data;
        sprintf((char *) text_buffer, "angles: \t%d, \t%d, \t%d, \t%d\n", mdata[0], mdata[1], mdata[2], mdata[3]);



       if(isFlying) {
           HAL_UART_Transmit(&huart1, (char *) text_buffer, strlen((char *) text_buffer), 100);
        //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
//            time_since_motor = HAL_GetTick();
           // uint16_t *mdata = (uint16_t *) data;


            current_value = mdata[0];
            if (current_value - last_value > 1500 && !triggered && current_value > 1000){
                last_value = current_value;
                value += 1;
                triggered = true;
            }
            else if(current_value - last_value  < -1500 && !triggered && current_value < 1000){
                last_value = current_value;
                if(value > 0){
                    value -= 1;
                }
                triggered = true;
            }else if(current_value > -150 && current_value < 150 && triggered){
                triggered = false;
            }





//            int16_t speed = mdata[0]/6;
//            if(speed < -100){
//                speed = -100;
//            }
            motor_speeds[0] = 1150 + value * 20;
            motor_speeds[1] = 1150 + value * 20;
            motor_speeds[2] = 1150 + value * 20;
            motor_speeds[3] = 1150 + value * 20;
            write_motor_speeds(motor_speeds);
       }
    }
}