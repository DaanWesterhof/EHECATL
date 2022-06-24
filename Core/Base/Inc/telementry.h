//
// Created by daang on 20-6-2022.
//

#ifndef EHECATL_TELEMENTRY_H
#define EHECATL_TELEMENTRY_H

#include "stm32f4xx_hal.h"
#include <cstring>

namespace EHECATL{
    class telementry {

        unsigned int start_height = sizeof(uint8_t);
        unsigned int start_speed = sizeof(double) + start_height;
        unsigned int start_rpm = sizeof(double) + start_speed;


    public:
        uint8_t width = 1 + sizeof(double) + sizeof(double) + sizeof(uint16_t)*4;
        uint8_t data[100] = {};

        EHECATL::DRONE_MODE state;
        char height_data[20] = "";
        char speed_data[20] = "";
        char rpm_data_1[25] = "";
        char rpm_data_2[25] = "";


        void setState(uint8_t dstate){
            data[0] = dstate;
        }

        void setHeightData(double altitude){
            double alt = altitude;
            memcpy(data+start_height, (uint8_t *)(&alt), sizeof(double));

        }

        void setSpeedData(double speed){
            double spd = speed;
            memcpy(data+start_speed, (uint8_t *)&spd, sizeof(double));

        }

        void setRpmData(uint16_t * rpm){
            memcpy(data+start_rpm, (uint8_t *)rpm, sizeof(uint16_t)*4);
        }

        uint8_t getLen(){
            return width;
        }

        void parseFromData(uint8_t * data_ptr, size_t len){
            state = data_ptr[0];
            double height = 0;
            memcpy(&height, data_ptr+start_height, sizeof(double));
            sprintf(height_data, "%3.8f", height);
            double speed = 0;
            memcpy(&speed, data_ptr+start_speed, sizeof(double));
            sprintf(speed_data, "%3.8f",  speed);


            uint16_t * rpm = (uint16_t *) (data_ptr + start_rpm);
            //memcpy(rpm, data_ptr+start_rpm, sizeof(uint16_t)*4);
            sprintf(rpm_data_1, "1: %4d, 2: %4d", rpm[0], rpm[1]);
            sprintf(rpm_data_2, "3: %4d, 4: %4d", rpm[2], rpm[3]);
        }

    };
}

#endif //EHECATL_TELEMENTRY_H
