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
        virtual void write_pixels(int start_x, int start_y, int end_x, int end_y, uint8_t ** data) = 0;
    };

    template<int width, int height>
    class Canvas{
        Base_Screen & screen;


    public:
        Canvas(Base_Screen &screen) : screen(screen) {}

    private:
    public:
        template<typename t>
        void writeAndFlushLine(int x, int y, char * text, int len, t color){
            t print_buffer[8][width*6] = {};
            for(int i = 0; i < len; i++){
                FONTS::fontToCanvas<t>(text[i], print_buffer, color, i*6);
            }
            screen.write_pixels(x * 6, y * 8, x + len*6, y + len*8, (uint8_t **)print_buffer);
        }
    };
}

#endif //EHECATL_SCREEN_HPP
