//
// Created by daang on 7-2-2022.
//

#include "screen.hpp"
#include "ST7735_LIB.hpp"

EHECATL::screenManager::screenManager(EHECATL::Canvas &canvas, EHECATL::communication &comms, telementry & tm) : canvas(canvas), comms(comms), tm(tm) {
    comms.addNewCallback(EHECATL::MSG_COMMANDS::ERROR_MESSAGE, COMM_CALLBACK(print_error));
    comms.addNewCallback(EHECATL::MSG_COMMANDS::NEW_STATE, COMM_CALLBACK(print_mode));
    comms.addNewCallback(EHECATL::MSG_COMMANDS::CONNECTION_STATE, COMM_CALLBACK(print_state));


    canvas.writeAndFlushLine(0, error_y, error_text, 7, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
    canvas.writeAndFlushLine(0, state_y, state_text, 7, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
    canvas.writeAndFlushLine(0, mode_y, mode_text, 7, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
    canvas.writeAndFlushLine(0, height_y, height_text, 7, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
    canvas.writeAndFlushLine(0, speed_y, speed_text, 7, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
    canvas.writeAndFlushLine(0, rpm_y, rpm_text, 7, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
}

void EHECATL::screenManager::print_error(uint8_t command, uint8_t *payload, uint8_t len) {
    canvas.writeAndFlushLine(error_x, error_y, (char *)(payload), len, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
}

void EHECATL::screenManager::print_state(uint8_t command, uint8_t *payload, uint8_t len) {
    canvas.writeAndFlushLine(state_x, state_y, (char *)(payload), len, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
}

void EHECATL::screenManager::print_mode(uint8_t command, uint8_t *payload, uint8_t len) {
    char st[4];
    if(payload[0] != state){
        EHECATL::state_to_string(payload[0], mode_s);
        canvas.writeAndFlushLine(mode_x, mode_y, mode_s, 3, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
        state = payload[0];
    }
}

void EHECATL::screenManager::print_data() const {
    canvas.writeAndFlushLine(height_x, height_y, tm.height_data, strlen(tm.height_data), ST7735_COLOR565(0x1f, 0x3f, 0x1f));
    canvas.writeAndFlushLine(speed_x, speed_y, tm.speed_data, strlen(tm.speed_data), ST7735_COLOR565(0x1f, 0x3f, 0x1f));
    canvas.writeAndFlushLine(rpm_x, rpm_y, tm.rpm_data, strlen(tm.rpm_data), ST7735_COLOR565(0x1f, 0x3f, 0x1f));
}


