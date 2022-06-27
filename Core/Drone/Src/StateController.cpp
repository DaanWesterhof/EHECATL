//
// Created by daang on 9-5-2022.
//

#include "StateController.hpp"

EHECATL::StateController::StateController(EHECATL::communication &comms) : comms(comms) {
    current_state = DRONE_MODES::SLEEP;
    comms.addNewCallback(EHECATL::MSG_COMMANDS::STATE_UP, COMM_CALLBACK(state_up));
    comms.addNewCallback(EHECATL::MSG_COMMANDS::STATE_DOWN, COMM_CALLBACK(state_down));
}


EHECATL::DRONE_MODE EHECATL::StateController::getState() {
    return current_state;
}

void EHECATL::StateController::setState(EHECATL::DRONE_MODE new_state) {
    current_state = new_state;
    comms.localMessage(MSG_COMMANDS::NEW_STATE, &current_state, 1);
    uint8_t data[1];
    data[0] = current_state;

#ifdef DRONE_DEBUG
    char data1[100];
    sprintf(data1, "we got send_result: %d\n", res);
    HAL_UART_Transmit(&huart1, data1, strlen(data1), 100);
#endif
}

void EHECATL::StateController::state_down(uint8_t command, uint8_t *payload, uint8_t len) {
    if(current_state == EHECATL::DRONE_MODES::FLYING){
        setState(EHECATL::DRONE_MODES::LANDING);
    }
}

void EHECATL::StateController::state_up(uint8_t command, uint8_t * payload, uint8_t len) {
    if(current_state == EHECATL::DRONE_MODES::SLEEP){
        setState(EHECATL::DRONE_MODES::SETUP);
    }
    if(current_state == EHECATL::DRONE_MODES::IDLE){
        setState(EHECATL::DRONE_MODES::FLYING);
    }

}
