//
// Created by daang on 20-6-2022.
//

#ifndef EHECATL_TELEMENTRY_H
#define EHECATL_TELEMENTRY_H

#include "stm32f4xx_hal.h"
#include "communication.hpp"
#include <cstring>

namespace EHECATL{

    /**
     * Object used to store telementry data.
     */
    class telementry {

        //the start index of height in the data buffer;
        unsigned int start_height = sizeof(uint8_t);

        //the start index of vertical speed in the data buffer
        unsigned int start_speed = sizeof(double) + start_height;

        //the start index of the speed per motor in the data buffer
        unsigned int start_rpm = sizeof(double) + start_speed;


    public:
        //the total size of the message
        uint8_t width = 1 + sizeof(double) + sizeof(double) + sizeof(uint16_t)*4;

        //the buffer that stores the telementry data, can be passed as an ack package to the communication system.
        uint8_t data[100] = {};


        EHECATL::DRONE_MODE state;
        //variables that store the data parsed from the ack payload in string format
        char height_data[20] = "";
        char speed_data[20] = "";
        //the rpm data should each be displayed on a difrent line of the lcd.
        char rpm_data_1[25] = "";
        char rpm_data_2[25] = "";


        /**
         * update the state value in the data
         * @param dstate new state
         */
        void setState(uint8_t dstate);

        /**
         * update the height value in the data
         * @param altitude the current height of the drone in meters
         */
        void setHeightData(double altitude);

        /**
         * update the vertical speed of the drone in the data
         * @param speed the current vertical speed in meters per second
         */
        void setSpeedData(double speed);

        /**
         * update the speed data per motor in the data buffer
         * @param rpm an array of uint16_t representing the current pwm signal send to each motor
         */
        void setRpmData(uint16_t * rpm);

        /**
         * get the length of the telementry package
         */
        uint8_t getLen();

        /**
         * turn the data of the telementry package into readable/useable data
         * will store all data except the state as a string.
         * @param data_ptr pointer to the ack package
         * @param len total length of the ack package
         */
        void parseFromData(uint8_t * data_ptr, size_t len);

    };
}

#endif //EHECATL_TELEMENTRY_H
