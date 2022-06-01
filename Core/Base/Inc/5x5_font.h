#include "stm32f4xx_hal.h"

namespace FONTS {
    const constexpr unsigned char font_6_8[96][6] = {
            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, //
            {0x5c, 0x00, 0x00, 0x00, 0x00, 0x00}, // !
            {0x06, 0x00, 0x06, 0x00, 0x00, 0x00}, // "
            {0x28, 0x7c, 0x28, 0x7c, 0x28, 0x00}, // #
            {0x5c, 0x54, 0xfe, 0x54, 0x74, 0x00}, // $
            {0x44, 0x20, 0x10, 0x08, 0x44, 0x00}, // %
            {0x28, 0x54, 0x54, 0x20, 0x50, 0x00}, // &
            {0x06, 0x00, 0x00, 0x00, 0x00, 0x00}, // '
            {0x38, 0x44, 0x00, 0x00, 0x00, 0x00}, // (
            {0x44, 0x38, 0x00, 0x00, 0x00, 0x00}, // )
            {0x02, 0x07, 0x02, 0x00, 0x00, 0x00}, // *
            {0x10, 0x10, 0x7c, 0x10, 0x10, 0x00}, // +
            {0xc0, 0x00, 0x00, 0x00, 0x00, 0x00}, // ,
            {0x10, 0x10, 0x10, 0x10, 0x10, 0x00}, // -
            {0x40, 0x00, 0x00, 0x00, 0x00, 0x00}, // .
            {0x60, 0x10, 0x0c, 0x00, 0x00, 0x00}, // /
            {0x7c, 0x64, 0x54, 0x4c, 0x7c, 0x00}, // 0
            {0x48, 0x7c, 0x40, 0x00, 0x00, 0x00}, // 1
            {0x64, 0x54, 0x54, 0x54, 0x48, 0x00}, // 2
            {0x44, 0x54, 0x54, 0x54, 0x6c, 0x00}, // 3
            {0x3c, 0x20, 0x70, 0x20, 0x20, 0x00}, // 4
            {0x5c, 0x54, 0x54, 0x54, 0x24, 0x00}, // 5
            {0x7c, 0x54, 0x54, 0x54, 0x74, 0x00}, // 6
            {0x04, 0x04, 0x64, 0x14, 0x0c, 0x00}, // 7
            {0x7c, 0x54, 0x54, 0x54, 0x7c, 0x00}, // 8
            {0x5c, 0x54, 0x54, 0x54, 0x7c, 0x00}, // 9
            {0x44, 0x00, 0x00, 0x00, 0x00, 0x00}, // :
            {0xc4, 0x00, 0x00, 0x00, 0x00, 0x00}, // ;
            {0x10, 0x28, 0x44, 0x00, 0x00, 0x00}, // <
            {0x28, 0x28, 0x28, 0x28, 0x28, 0x00}, // =
            {0x44, 0x28, 0x10, 0x00, 0x00, 0x00}, // >
            {0x08, 0x04, 0x54, 0x08, 0x00, 0x00}, // ?
            {0x7c, 0x44, 0x54, 0x54, 0x5c, 0x00}, // @
            {0x7c, 0x24, 0x24, 0x24, 0x7c, 0x00}, // A
            {0x7c, 0x54, 0x54, 0x54, 0x6c, 0x00}, // B
            {0x7c, 0x44, 0x44, 0x44, 0x44, 0x00}, // C
            {0x7c, 0x44, 0x44, 0x44, 0x38, 0x00}, // D
            {0x7c, 0x54, 0x54, 0x54, 0x44, 0x00}, // E
            {0x7c, 0x14, 0x14, 0x14, 0x04, 0x00}, // F
            {0x7c, 0x44, 0x44, 0x54, 0x74, 0x00}, // G
            {0x7c, 0x10, 0x10, 0x10, 0x7c, 0x00}, // H
            {0x44, 0x44, 0x7c, 0x44, 0x44, 0x00}, // I
            {0x60, 0x40, 0x40, 0x44, 0x7c, 0x00}, // J
            {0x7c, 0x10, 0x10, 0x28, 0x44, 0x00}, // K
            {0x7c, 0x40, 0x40, 0x40, 0x40, 0x00}, // L
            {0x7c, 0x08, 0x10, 0x08, 0x7c, 0x00}, // M
            {0x7c, 0x08, 0x10, 0x20, 0x7c, 0x00}, // N
            {0x38, 0x44, 0x44, 0x44, 0x38, 0x00}, // O
            {0x7c, 0x14, 0x14, 0x14, 0x08, 0x00}, // P
            {0x3c, 0x24, 0x64, 0x24, 0x3c, 0x00}, // Q
            {0x7c, 0x14, 0x14, 0x14, 0x68, 0x00}, // R
            {0x5c, 0x54, 0x54, 0x54, 0x74, 0x00}, // S
            {0x04, 0x04, 0x7c, 0x04, 0x04, 0x00}, // T
            {0x7c, 0x40, 0x40, 0x40, 0x7c, 0x00}, // U
            {0x0c, 0x30, 0x40, 0x30, 0x0c, 0x00}, // V
            {0x3c, 0x40, 0x30, 0x40, 0x3c, 0x00}, // W
            {0x44, 0x28, 0x10, 0x28, 0x44, 0x00}, // X
            {0x0c, 0x10, 0x60, 0x10, 0x0c, 0x00}, // Y
            {0x44, 0x64, 0x54, 0x4c, 0x44, 0x00}, // Z
            {0x7c, 0x44, 0x00, 0x00, 0x00, 0x00}, // [
            {0x0c, 0x10, 0x60, 0x00, 0x00, 0x00}, // "\"
            {0x44, 0x7c, 0x00, 0x00, 0x00, 0x00}, // ]
            {0x00, 0x01, 0x00, 0x01, 0x00, 0x00}, // ^
            {0x40, 0x40, 0x40, 0x40, 0x40, 0x40}, // _
            {0x00, 0x01, 0x00, 0x00, 0x00, 0x00}, // `
            {0x7c, 0x24, 0x24, 0x24, 0x7c, 0x00}, // a
            {0x7c, 0x54, 0x54, 0x54, 0x6c, 0x00}, // b
            {0x7c, 0x44, 0x44, 0x44, 0x44, 0x00}, // c
            {0x7c, 0x44, 0x44, 0x44, 0x38, 0x00}, // d
            {0x7c, 0x54, 0x54, 0x54, 0x44, 0x00}, // e
            {0x7c, 0x14, 0x14, 0x14, 0x04, 0x00}, // f
            {0x7c, 0x44, 0x44, 0x54, 0x74, 0x00}, // g
            {0x7c, 0x10, 0x10, 0x10, 0x7c, 0x00}, // h
            {0x44, 0x44, 0x7c, 0x44, 0x44, 0x00}, // i
            {0x60, 0x40, 0x40, 0x44, 0x7c, 0x00}, // j
            {0x7c, 0x10, 0x10, 0x28, 0x44, 0x00}, // k
            {0x7c, 0x40, 0x40, 0x40, 0x40, 0x00}, // l
            {0x7c, 0x08, 0x10, 0x08, 0x7c, 0x00}, // m
            {0x7c, 0x08, 0x10, 0x20, 0x7c, 0x00}, // n
            {0x38, 0x44, 0x44, 0x44, 0x38, 0x00}, // o
            {0x7c, 0x14, 0x14, 0x14, 0x08, 0x00}, // p
            {0x3c, 0x24, 0x64, 0x24, 0x3c, 0x00}, // q
            {0x7c, 0x14, 0x14, 0x14, 0x68, 0x00}, // r
            {0x5c, 0x54, 0x54, 0x54, 0x74, 0x00}, // s
            {0x04, 0x04, 0x7c, 0x04, 0x04, 0x00}, // t
            {0x7c, 0x40, 0x40, 0x40, 0x7c, 0x00}, // u
            {0x0c, 0x30, 0x40, 0x30, 0x0c, 0x00}, // v
            {0x3c, 0x40, 0x30, 0x40, 0x3c, 0x00}, // w
            {0x44, 0x28, 0x10, 0x28, 0x44, 0x00}, // x
            {0x0c, 0x10, 0x60, 0x10, 0x0c, 0x00}, // y
            {0x44, 0x64, 0x54, 0x4c, 0x44, 0x00}, // z
            {0x10, 0x7c, 0x44, 0x00, 0x00, 0x00}, // {
            {0x6c, 0x00, 0x00, 0x00, 0x00, 0x00}, // |
            {0x44, 0x7c, 0x10, 0x00, 0x00, 0x00}, // }
            {0x02, 0x01, 0x02, 0x01, 0x00, 0x00}, // ~
            {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
    };


    template<typename t>
    void fontToCanvas(char character, t ** dataArray, t color, int index = 0){
        for(int i = 0; i < 6; i++){
            for(unsigned int j = 0; j < 8; j++) {
                dataArray[j][index+i] = (font_6_8[uint8_t(character) - 32][i] & (1u<<(j)) ) ? color : 0;
            }
        }
    }
}