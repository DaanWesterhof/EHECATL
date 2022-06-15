//
// Created by daang on 7-2-2022.
//

#ifndef EHECATL_SCREEN_HPP
#define EHECATL_SCREEN_HPP

#include "5x5_font.h"
#include "communication.hpp"

namespace EHECATL{



    class Base_Screen{

    public:
        virtual void write_pixels(int start_x, int start_y, int len, int height, uint8_t * data) = 0;
    };

    class Canvas{
        int width;
        int height;
        Base_Screen & screen;


    public:

        Canvas(int width, int height, Base_Screen &screen) : width(width), height(height), screen(screen) {}

    private:
    public:
        void writeAndFlushLine(int x, int y, char * text, int len, uint16_t color){

            uint16_t print_buffer[8*128] = {};
            for(int i = 0; i < len; i++){
                FONTS::fontToCanvas(len*6+1, text[i], print_buffer, 65000, i);
            }
            screen.write_pixels(x*6, y * 8, len*6, 8, (uint8_t *)print_buffer);
        }

        void clearArea(int x, int y, int len){
            uint16_t print_buffer[8*128] = {0};
            screen.write_pixels(x*6, y*8, len*6, 8, (uint8_t *) print_buffer);
        }
    };



    class screenManager {
        EHECATL::Canvas & canvas;
        EHECATL::communication & comms;
        uint8_t error_x = 9;
        uint8_t error_y = 0;

        uint8_t height_x = 8;
        uint8_t height_y = 3;

        uint8_t state_x  = 8;
        uint8_t state_y  = 1;

        uint8_t mode_x  = 8;
        uint8_t mode_y  = 2;

        uint8_t speed_x = 8;
        uint8_t speed_y = 4;


        double speed_val = 0;
        char height_s[21] = "";
        char  speed_s[21] = "";
        char mode_s[4] = {};
        EHECATL::DRONE_MODE state = 232;

        char error_text [8] = "error :";
        char state_text [8] = "state :";
        char mode_text  [8] = "mode  :";
        char height_text[8] = "height:";
        char speed_text [8] = "speed :";



    public:

        screenManager(EHECATL::Canvas &canvas, EHECATL::communication &comms);


        void print_error(uint8_t command, uint8_t *payload, uint8_t len);

        void print_state(uint8_t command, uint8_t *payload, uint8_t len);


        void get_height(uint8_t command, uint8_t *payload, uint8_t len);

        void get_speed(uint8_t command, uint8_t * payload, uint8_t len);

        void print_mode(uint8_t command, uint8_t *payload, uint8_t len);


        void print_data() const;
    };

}



#endif //EHECATL_SCREEN_HPP
