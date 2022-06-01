//
// Created by daang on 9-5-2022.
//

#include "StateController.hpp"

EHECATL::StateController::StateController(EHECATL::communication &comms) : comms(comms) {
    current_state = DRONE_MODES::SETUP;
    comms.addNewCallback(EHECATL::MSG_COMMANDS::STATE_UP, COMM_CALLBACK(newDesiredState));
    comms.addNewCallback(EHECATL::MSG_COMMANDS::STATE_DOWN, COMM_CALLBACK(newDesiredState));
}


void EHECATL::StateController::speedListener(double speed) {
    if(current_state == DRONE_MODES::LANDING) {
        if (speed < 0.005 && speed > -0.005) {
            setState(DRONE_MODES::IDLE);
        }
    }
}

EHECATL::DRONE_MODE EHECATL::StateController::getState() {
    return current_state;
}

void EHECATL::StateController::setState(EHECATL::DRONE_MODE new_state) {
    current_state = new_state;
    comms.localMessage(MSG_COMMANDS::NEW_STATE, &current_state, 1);
    uint8_t data[1];
    data[0] = current_state;
    int res = comms.sendMessage(MSG_COMMANDS::NEW_STATE, data, 1);
    char data1[100];
    sprintf(data1, "we got send_result: %d\n", res);
    HAL_UART_Transmit(&huart1, data1, strlen(data1), 100);
}

void EHECATL::StateController::state_down(uint8_t command, uint8_t *payload, uint8_t len) {


}

void EHECATL::StateController::state_up(uint8_t command, uint8_t * payload, uint8_t len) {

}
