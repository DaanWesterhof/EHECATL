//
// Created by daang on 27-6-2022.
//

#include "droneController.h"

EHECATL::motorStateController::motorStateController(EHECATL::Motors &motors, EHECATL::communication &comms, EHECATL::StateController &stateController)
        : motors(motors), comms(comms), stateController(stateController)
{
    comms.addNewCallback(EHECATL::MSG_COMMANDS::ALTITUDE_SPEED, COMM_CALLBACK(setSpeed));
    comms.addNewCallback(EHECATL::MSG_COMMANDS::JOYSTICK_ANGLES, COMM_CALLBACK(joystickListener));
    comms.addNewCallback(EHECATL::MSG_COMMANDS::NEW_STATE, COMM_CALLBACK(state_listener));
}

void EHECATL::motorStateController::joystickListener(uint8_t command, uint8_t *payload, uint8_t len) {
    if (stateController.getState() == EHECATL::DRONE_MODES::LANDING) {
        stateController.setState(EHECATL::DRONE_MODES::FLYING);
        landing = false;
    }
}

void EHECATL::motorStateController::setSpeed(uint8_t command, uint8_t *payload, uint8_t len) {
    drone_speed = *((double *) payload);
}

void EHECATL::motorStateController::state_listener(uint8_t command, uint8_t *payload, uint8_t len) {
    if (*payload == EHECATL::DRONE_MODES::LANDING && !landing) {
        landing = true;
        float angles[4] = {-10, 0, 0, 0};
        send_joysticks = true;
        comms.localMessage(EHECATL::MSG_COMMANDS::JOYSTICK_ANGLES_LOCALE, (uint8_t *) angles, sizeof(float) * 4);
        time_since_landing = HAL_GetTick();
    }
}

void EHECATL::motorStateController::update(EHECATL::Barometer &barometer, EHECATL::MPU_GYRO &mpu, EHECATL::telementry &telem) {
    if (stateController.getState() == EHECATL::DRONE_MODES::SLEEP) {

    }
    if (stateController.getState() == EHECATL::DRONE_MODES::SETUP) {
        mpu.setOffsets();
        motor_speeds[0] = 1330;
        motor_speeds[1] = 1330;
        motor_speeds[2] = 1330;
        motor_speeds[3] = 1330;
        EHECATL::write_motor_speeds(motor_speeds);
        stateController.setState(EHECATL::DRONE_MODES::FLYING);
    }
    if (stateController.getState() == EHECATL::DRONE_MODES::FLYING) {

        //get the base speed of the motors
        //add on the change values from the mpu.
        mpu.update(telem);
        barometer.update(telem);


        motors.getChange(change);
        uint16_t write_speeds[4];
        for (int i = 0; i < 4; i++) {

            write_speeds[i] = motors.motor_speeds[i] + change[i];
            if (write_speeds[i] < 1000) {
                write_speeds[i] = 1000;
            }
            if (write_speeds[i] > max_value) {
                write_speeds[i] = max_value;
            }
        }
        telem.setRpmData(write_speeds);

    #ifdef DRONE_DEBUG
        char text_buffer[100];
        sprintf((char *) text_buffer, "1: %4d, 2: %4d, 3: %4d, 4: %4d\n", write_speeds[0], write_speeds[1], write_speeds[2],
                write_speeds[3]);
        HAL_UART_Transmit(&huart1, (char *) text_buffer, strlen((char *) text_buffer), 100);
    #endif
        EHECATL::write_motor_speeds(write_speeds);

    } else if (stateController.getState() == EHECATL::DRONE_MODES::LANDING) {
        mpu.update();
        barometer.update(telem);
        if (HAL_GetTick() - time_since_landing > 3000) {
            if (drone_speed < 0.3 && drone_speed > -0.3) {
                stateController.setState(EHECATL::DRONE_MODES::IDLE);
                uint16_t speeds[4] = {500, 500, 500, 500};
                EHECATL::write_motor_speeds(speeds);
                landing = false;
            }
        }
    }
}
