//
// Created by daang on 7-2-2022.
//

#include "screen.hpp"
#include "ST7735_LIB.hpp"

void EHECATL::Canvas::writeAndFlushLine(int x, int y, char *text, int len, uint16_t color) {

    uint16_t print_buffer[8*128] = {};
    for(int i = 0; i < len; i++){
        FONTS::fontToCanvas(len*6+1, text[i], print_buffer, 65000, i);
    }
    screen.write_pixels(x*6, y * 8, len*6, 8, (uint8_t *)print_buffer);
}

void EHECATL::Canvas::clearArea(int x, int y, int len) {
    uint16_t print_buffer[8*128] = {0};
    screen.write_pixels(x*6, y*8, len*6, 8, (uint8_t *) print_buffer);
}


EHECATL::screenManager::screenManager(EHECATL::Canvas &canvas, EHECATL::communication &comms, telementry & tm) : canvas(canvas), comms(comms), tm(tm) {
    comms.addNewCallback(EHECATL::MSG_COMMANDS::ERROR_MESSAGE, COMM_CALLBACK(print_error));
    comms.addNewCallback(EHECATL::MSG_COMMANDS::NEW_STATE, COMM_CALLBACK(print_mode));
    comms.addNewCallback(EHECATL::MSG_COMMANDS::CONNECTION_STATE, COMM_CALLBACK(print_state));


    canvas.writeAndFlushLine(0, error_y, error_text, 7, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
    canvas.writeAndFlushLine(0, state_y, state_text, 7, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
    canvas.writeAndFlushLine(0, mode_y, mode_text, 7, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
    canvas.writeAndFlushLine(0, height_y, height_text, 7, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
    canvas.writeAndFlushLine(0, speed_y, speed_text, 7, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
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
    canvas.writeAndFlushLine(rpm_1_x, rpm_1_y, tm.rpm_data_1, strlen(tm.rpm_data_1), ST7735_COLOR565(0x1f, 0x3f, 0x1f));
    canvas.writeAndFlushLine(rpm_2_x, rpm_2_y, tm.rpm_data_2, strlen(tm.rpm_data_2), ST7735_COLOR565(0x1f, 0x3f, 0x1f));
}


