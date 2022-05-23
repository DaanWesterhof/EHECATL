//
// Created by daang on 7-2-2022.
//

#ifndef EHECATL_SCREEN_HPP
#define EHECATL_SCREEN_HPP

#include "5x5_font.h"

namespace EHECATL{



    class Base_Screen{
        int width;
        int height;

    public:
        void write_pixels(int start_x, int start_y, int end_x, int end_y, uint8_t * data){

        }
    };

    template<int width, int height>
    class Character{
        char char_data[width] = {};
    public:
        Character(char character){
            FONTS::getPixelData(character, char_data);
        }

    };

    template<int width, int height>
    class Canvas{
        Character<6, 8> segments[width][height];
        bool updated[width][height] = {};

        Base_Screen & screen;


        void writeText(int x, int y, char * text, int len){
            for(int i = 0; i < len; i++){
                segments[x][y+i] = Character<6, 8>(text[i]);
                updated[x][y] = true;
            }
        }

        void writeAndFlushLine(int x, int y, char * text, int len){
            uint8_t print_buffer[8][width] = {};
            for(int i = 0; i < len; i++){
                FONTS::fontToCanvas(text[i], print_buffer,i*6);
            }
            screen.write_pixels(x * 6, y * 8, x + len*6, y + len*8, print_buffer);
        }
    };
}

#endif //EHECATL_SCREEN_HPP
