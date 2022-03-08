//
// Created by daang on 8-3-2022.
//

#include "Motors.hpp"

namespace EHECATL{

    void Motors::setMotorSpeeds(uint8_t command, uint8_t *data, uint8_t len) {
        int * mdata = (int *)data;
        if(mdata[4] == offset){
            motor_speeds[0] += mdata[0];
            motor_speeds[1] += mdata[1];
            motor_speeds[2] += mdata[2];
            motor_speeds[3] += mdata[3];
        }else{
            offset = mdata[4];
            motor_speeds[0] = base_speed + offset + mdata[0];
            motor_speeds[1] = base_speed + offset + mdata[1];
            motor_speeds[2] = base_speed + offset + mdata[2];
            motor_speeds[3] = base_speed + offset + mdata[3];

        }
    }

    Motors::Motors(communication &comms) : comms(comms) {
        comms.addNewCallback(MSG_COMMANDS::MOTOR_DIFFERENCE, COMM_CALLBACK(setMotorSpeeds));
    }
}

