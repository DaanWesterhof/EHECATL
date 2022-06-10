//
// Created by daang on 7-2-2022.
//

#include "screen.hpp"
#include "ST7735_LIB.hpp"

EHECATL::screenManager::screenManager(EHECATL::Canvas &canvas, EHECATL::communication &comms) : canvas(canvas), comms(comms) {
    comms.addNewCallback(EHECATL::MSG_COMMANDS::ERROR_MESSAGE, COMM_CALLBACK(print_error));
    comms.addNewCallback(EHECATL::MSG_COMMANDS::NEW_STATE, COMM_CALLBACK(print_state));
    comms.addNewCallback(EHECATL::MSG_COMMANDS::DRONE_HEIGHT, COMM_CALLBACK(get_height));

    canvas.writeAndFlushLine(0, error_y, error_text, 7, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
    canvas.writeAndFlushLine(0, state_y, state_text, 7, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
    canvas.writeAndFlushLine(0, height_y, height_text, 7, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
    canvas.writeAndFlushLine(0, speed_y, speed_text, 7, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
}

void EHECATL::screenManager::print_error(uint8_t command, uint8_t *payload, uint8_t len) {
    canvas.writeAndFlushLine(error_x, error_y, (char *)(payload+1), len, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
}

void EHECATL::screenManager::get_height(uint8_t command, uint8_t *payload, uint8_t len) {
    height = ((double *)payload)[0];
}

void EHECATL::screenManager::print_state(uint8_t command, uint8_t *payload, uint8_t len) {
    char st[4];
    if(payload[0] != state){
        EHECATL::state_to_string(payload[0], state_s);
        canvas.writeAndFlushLine(state_x, state_y, state_s, 3, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
        state = payload[0];
    }
}

void EHECATL::screenManager::print_data() const {
    char chight[10] = {};
    sprintf(chight, "%4.2f", height);
    canvas.writeAndFlushLine(height_x, height_y, chight, strlen(chight)-1, ST7735_COLOR565(0x1f, 0x3f, 0x1f));

    char cspeed[10] = {};
    sprintf(cspeed, "%3.1f", speed_val);
    canvas.writeAndFlushLine(speed_x, speed_y, cspeed, strlen(cspeed)-1, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
}
