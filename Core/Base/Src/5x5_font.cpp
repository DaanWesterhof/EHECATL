//
// Created by daang on 9-6-2022.
//

#include "5x5_font.h"

void FONTS::fontToCanvas(int width, char character, uint16_t *dataArray, uint16_t color, int index) {
    for(int i = 0; i < 6; i++){
        for(unsigned int j = 0; j < 8; j++) {
            uint16_t value = (font_6_8[uint8_t(character) - 32][i] & (0b1<<(7-j)) ) ? color : 0;
            dataArray[(8-j)*width + i + index * 6] = value;
        }
    }
}

