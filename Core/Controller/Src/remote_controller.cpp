//
// Created by daang on 27-6-2022.
//

#include "remote_controller.hpp"

EHECATL::controller::controller(EHECATL::communication &communicator) :  comms(communicator){
    comms.addNewCallback(EHECATL::MSG_COMMANDS::NEW_STATE, COMM_CALLBACK(print_state));
    comms.addNewCallback(EHECATL::MSG_COMMANDS::BUTTON_STATE_CHANGE, COMM_CALLBACK(state_switcher));
    char error[] ="Disconected";
    comms.localMessage(EHECATL::MSG_COMMANDS::CONNECTION_STATE, (uint8_t *)error, strlen(error));
}

void EHECATL::controller::print_state(uint8_t command, uint8_t *payload, uint8_t len) {
    char data[10];
    recieved_state = *payload;
#ifdef DRONE_DEBUG
    sprintf(data, "new_state: %u\n", *payload);
    HAL_UART_Transmit(&huart1, data, strlen(data), 100);
#endif
    time_since_state = HAL_GetTick();
    if(disconected) {
        char error[] = "Connected  ";
        comms.localMessage(EHECATL::MSG_COMMANDS::CONNECTION_STATE, (uint8_t *) error, strlen(error));
        disconected = false;
    }
}

void EHECATL::controller::state_switcher(uint8_t command, uint8_t *payload, uint8_t len) {
    if(payload[1] == 1){//a button has been pressed
        if(payload[0] == 2){ //its the right button
            uint8_t p_data[1] = {0};
            comms.sendMessage(EHECATL::MSG_COMMANDS::STATE_UP, p_data, 1);
        }
        if(payload[0] == 1){ //its the left button
            uint8_t p_data[1] = {0};
            comms.sendMessage(EHECATL::MSG_COMMANDS::STATE_DOWN, p_data, 1);
        }
    }

}

void EHECATL::controller::update() {
    if(HAL_GetTick() - time_since_state > 3000){
        char error[] ="Disconected";
        comms.localMessage(EHECATL::MSG_COMMANDS::CONNECTION_STATE, (uint8_t *)error, strlen(error));
        disconected = true;
    }
#ifdef DRONE_DEBUG
    if(disconected){
        char text[] = "disconected\n";
        HAL_UART_Transmit(&huart1, text, strlen(text), 100);
    }else{
        char text2[] = "conected\n";
        HAL_UART_Transmit(&huart1, text2, strlen(text2), 100);
    }
#endif
}
