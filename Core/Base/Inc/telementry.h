//
// Created by daang on 20-6-2022.
//

#ifndef EHECATL_TELEMENTRY_H
#define EHECATL_TELEMENTRY_H

#include "stm32f4xx_hal.h"
#include <cstring>

namespace EHECATL{
    class telementry {

        unsigned int start_height = 1;
        unsigned int start_speed = 9;


    public:
        uint8_t width = 1 + 9 + 9;
        uint8_t data[100];

        EHECATL::DRONE_MODE state;
        char height_data[20] = "";
        char speed_data[20] = "";




        void setState(uint8_t dstate){
            data[0] = dstate;
        }

        void setHeightData(double altitude){
            double alt = altitude;
            memcpy(data+start_height, (uint8_t *)(&alt), 8);

        }

        void setSpeedData(double speed){
            double spd = speed;
            memcpy(data+start_speed, (uint8_t *)&spd, 8);

        }

        uint8_t getLen(){
            return width;
        }

        void parseFromData(uint8_t * data_ptr, size_t len){
            state = data_ptr[0];
            double height = 0;
            memcpy(&height, data_ptr+start_height, sizeof(double));
            sprintf(height_data, "%f", height);
            double speed = 0;
            memcpy(&speed, data_ptr+start_speed, sizeof(double));
            sprintf(speed_data, "%f",  speed);
        }

    };
}

#endif //EHECATL_TELEMENTRY_H
