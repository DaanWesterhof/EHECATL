//
// Created by daang on 8-3-2022.
//

#include "Motors.hpp"

namespace EHECATL {

    void Motors::setMotorSpeedsForRotations(uint8_t command, uint8_t *data, uint8_t len) {
        int *mdata = (int *) data;
        motor_speeds[0] += mdata[0];
        motor_speeds[1] += mdata[1];
        motor_speeds[2] += mdata[2];
        motor_speeds[3] += mdata[3];
    }

    void Motors::setMotorSpeedsYSpeed(uint8_t command, uint8_t *data, uint8_t len) {
        int *mdata = (int *) data;
        if (mdata[0] == 0) {
            PID_Controll_Speed = true;
        } else {
            if (data[0] != current_offset) {
                int dif = data[0] - current_offset;
                current_offset = data[0];
                motor_speeds[0] += dif;
                motor_speeds[1] += dif;
                motor_speeds[2] += dif;
                motor_speeds[3] += dif;
            }
            PID_Controll_Speed = false;
        }
    }

    void Motors::setBaseSpeed(uint8_t command, uint8_t *data, uint8_t len) {
        current_height = ((float *) data)[0];
        if (PID_Controll_Speed) {
            int diff = height_pid.calulateAction(desired_height, current_height);
            motor_speeds[0] += diff;
            motor_speeds[1] += diff;
            motor_speeds[2] += diff;
            motor_speeds[3] += diff;
        } else {
            desired_height = current_height;
        }
    }

    Motors::Motors(communication &comms) : comms(comms) {
        comms.addNewCallback(MSG_COMMANDS::MOTOR_DIFFERENCE, COMM_CALLBACK(setMotorSpeedsForRotations));
        comms.addNewCallback(MSG_COMMANDS::NEW_BAROMETER_DATA, COMM_CALLBACK(setBaseSpeed));
        comms.addNewCallback(MSG_COMMANDS::BAROMETER_BASE_HEIGHT, COMM_CALLBACK(recieveBaseHeight));
        dshot_setTimers(&htim2, TIM_CHANNEL_1, &htim2, TIM_CHANNEL_2, &htim3, TIM_CHANNEL_1, &htim4, TIM_CHANNEL_1);
        dshot_type_e type = dshot_type_e::DSHOT600;
        dshot_init(type);
    }

    void Motors::StateRecieved(uint8_t command, uint8_t *data, uint8_t len) {
        DRONE_MODE state = data[0];
        if (state == DRONE_MODES::FLYING) {
            isFlying = true;
        } else {
            isFlying = false;
            if(state == DRONE_MODES::LANDING){
                setMotorSpeedsForRotations(0, (uint8_t *)&land_speed, 4);
            }
        }
    }


}