//
// Created by daang on 8-3-2022.
//

#include "Motors.hpp"
#include "MotorPWMThing.hpp"

namespace EHECATL {

    void Motors::setMotorSpeedsForRotations(uint8_t command, uint8_t *data, uint8_t len) {
        int *mdata = (int *) data;
        for(int i = 0; i < 4; i++){
            desired_change_movement[i] += mdata[i];
            if(desired_change_movement[i] > max_change){
                desired_change_movement[i] = max_change;
            }else if(desired_change_movement[i] < min_change){
                desired_change_movement[i] = min_change;
            }


            //change[i] += desired_change_height[i];
        }
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

    Motors::Motors(communication &comms, telementry & telm) : comms(comms), telm(telm) {
        comms.addNewCallback(MSG_COMMANDS::MOTOR_DIFFERENCE, COMM_CALLBACK(setMotorSpeedsForRotations));
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
        sprintf((char *) text_buffer, "angles: \t%d, \t%d, \t%d, \t%d\t\n", mdata[0], mdata[1], mdata[2], mdata[3]);
       if(isFlying) {
           HAL_UART_Transmit(&huart1, (char *) text_buffer, strlen((char *) text_buffer), 100);
//############################################################# DIRECT MOTOR CONTROLL ##############################################################
//            current_value = mdata[0];
//            if (current_value > 2000  && !triggered){
//                value += 1;
//                triggered = true;
//            }
//            else if(current_value < -1500 && !triggered ){
//                last_value = current_value;
//                if(value > 0){
//                    value -= 1;
//                }
//                triggered = true;
//            }else if(current_value > -300 && current_value < 300 && triggered){
//                triggered = false;
//            }
//           sprintf((char *) text_buffer, "speed_val: %d, \t rpm: %d\n", value, 1150 + (value * 5));
//           HAL_UART_Transmit(&huart1, (char *) text_buffer, strlen((char *) text_buffer), 100);
//#############################################################                        ##############################################################


            uint16_t speed = 0;
            if(mdata[0] > 0){
                speed = 330 + (max_added_speed/50*mdata[0]);
            }else{
                speed = 330 + (max_removed_speed/50*mdata[0]);
            }
            //base speed is 330;

            motor_speeds[0] = 1050 + speed;
            motor_speeds[1] = 1050 + speed;
            motor_speeds[2] = 1050 + speed;
            motor_speeds[3] = 1050 + speed;
            //write_motor_speeds(motor_speeds);
       }
    }
}