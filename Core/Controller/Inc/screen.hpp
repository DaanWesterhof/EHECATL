//
// Created by daang on 7-2-2022.
//

#ifndef EHECATL_SCREEN_HPP
#define EHECATL_SCREEN_HPP

#include "5x5_font.h"
#include "communication.hpp"

namespace EHECATL{

    /**
     * base class for screens to be used by the canvas
     */
    class Base_Screen{
    public:
        /**
         * function used by the canvas to write a rectangle of data to the display
         * @param start_x the start location on the x axis
         * @param start_y the start location on the y axis
         * @param len the length/width of the data rectangle
         * @param height the height of the data rectangle
         * @param data the data to be written
         */
        virtual void write_pixels(int start_x, int start_y, int len, int height, uint8_t * data) = 0;
    };



    /**
     * canvas used to write characters/strings to a display
     */
    class Canvas{
        //the width of the display
        int width;
        //the height of the display
        int height;
        //the display we want to use
        Base_Screen & screen;
    public:

        Canvas(int width, int height, Base_Screen &screen) : width(width), height(height), screen(screen) {}
    /**
     * write a line of characters/string to the screen/display
     * @param x the horizontal position on the display in characters
     * @param y the vertical position on the display in characters
     * @param text the text to write to the diplay
     * @param len the length of the text
     * @param color the color the text should have
     */
        void writeAndFlushLine(int x, int y, char * text, int len, uint16_t color);

        /**
         * clear an area on the display
         * @param x the horizontal position of the display in characters
         * @param y the vertical position on the display in characters
         * @param len the amount of character positions to clear.
         */
        void clearArea(int x, int y, int len);
    };



    /**
     * a class to manage the information displayed on the screen
     */
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

        uint8_t rpm_1_x = 0;
        uint8_t rpm_1_y = 10;
        uint8_t rpm_2_x = 0;
        uint8_t rpm_2_y = 11;


        double speed_val = 0;
        telementry & tm;
        char mode_s[4] = {};
        EHECATL::DRONE_MODE state = 232;

        char error_text [8] = "error :";
        char state_text [8] = "state :";
        char mode_text  [8] = "mode  :";
        char height_text[8] = "height:";
        char speed_text [8] = "speed :";
        char rpm_text   [8] = "rpm   :";

    public:

        screenManager(EHECATL::Canvas &canvas, EHECATL::communication &comms, telementry & tm);


        /**
         * callback function that prints an error on the display, listens to the EHECATL::MSG_COMMANDS::ERROR_MESSAGE command
         * @param command the command that called the function
         * @param payload the payload of the command, this should be the string you want to display
         * @param len the length of the payload/string
         */
        void print_error(uint8_t command, uint8_t *payload, uint8_t len);

        /**
         * callback function that prints the state to the display, listens to the EHECATL::MSG_COMMANDS::CONNECTION_STATE command
         * @param command the command that called the function
         * @param payload the payload of the command, this should be the string you want to display
         * @param len the length of the payload
         */
        void print_state(uint8_t command, uint8_t *payload, uint8_t len);

        /**
         * callback function that prints the drone mode to the display, listens to the EHECATL::MSG_COMMANDS::NEW_STATE command
         * @param command the command that called the function
         * @param payload the payload of the command, this is the state you want to display
         * @param len the length of the payload
         */
        void print_mode(uint8_t command, uint8_t *payload, uint8_t len);


        /**
         * function that prints the acuired data from the telementry data to the display
         */
        void print_data() const;
    };

}



#endif //EHECATL_SCREEN_HPP
