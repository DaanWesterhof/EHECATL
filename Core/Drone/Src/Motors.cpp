//
// Created by daang on 8-3-2022.
//

#include "Motors.hpp"
#include "MotorPWMThing.hpp"

namespace EHECATL {

    void Motors::setMotorSpeedsForRotations(uint8_t command, uint8_t *data, uint8_t len) {
        int *mdata = (int *) data;
        motor_speeds[0] += mdata[0];
        motor_speeds[1] += mdata[1];
        motor_speeds[2] += mdata[2];
        motor_speeds[3] += mdata[3];
        write_motor_speeds(motor_speeds);
    }



    void Motors::setMotorsForVerticalSpeed(uint8_t command, uint8_t *data, uint8_t len) {
        double current_speed = *(double *) data;
        if (desired_speed = 0) {
            PID_Controll_Speed = true;
        } else {
            PID_Controll_Speed = false;
            int diff = v_speed_pid.calulateAction(desired_speed, current_speed);
            motor_speeds[0] += diff;
            motor_speeds[1] += diff;
            motor_speeds[2] += diff;
            motor_speeds[3] += diff;
            write_motor_speeds(motor_speeds);
        }
    }

    void Motors::hoverController(uint8_t command, uint8_t *data, uint8_t len) {
        current_height = ((float *) data)[0];
        if (PID_Controll_Speed) {
            int diff = height_pid.calulateAction(desired_height, current_height);
            motor_speeds[0] += diff;
            motor_speeds[1] += diff;
            motor_speeds[2] += diff;
            motor_speeds[3] += diff;
            write_motor_speeds(motor_speeds);
        } else {
            desired_height = current_height;
        }
    }

    Motors::Motors(communication &comms) : comms(comms) {
        comms.addNewCallback(MSG_COMMANDS::MOTOR_DIFFERENCE, COMM_CALLBACK(setMotorSpeedsForRotations));
        comms.addNewCallback(MSG_COMMANDS::NEW_BAROMETER_DATA, COMM_CALLBACK(hoverController));
        comms.addNewCallback(MSG_COMMANDS::ALTITUDE_SPEED, COMM_CALLBACK(setMotorsForVerticalSpeed));

        //dshot_type_e type = dshot_type_e::DSHOT600;
        //dshot_init(type);
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
}