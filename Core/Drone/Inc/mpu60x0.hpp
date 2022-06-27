//
// Created by daang on 16-2-2022.
//

#ifndef EHECATL_MPU60X0_HPP
#define EHECATL_MPU60X0_HPP


#include <cstdio>
#include "MPU6050.h"
#include "communication.hpp"

namespace EHECATL{

    class MPU_GYRO{
        //I2cdev MPU6050
        UART_HandleTypeDef &huart1;
        I2C_HandleTypeDef &hi2c1;

        communication & comms;

        MPU6050 mpu;
        uint16_t packetSize;
        uint16_t fifoCount;
        uint8_t fifoBuffer[64];
        Quaternion q;
        VectorFloat gravity;
        float ypr[3];
        float ofsets[3] = {};
        float PI = 3.14159265359;

    public:
        float x;
        float y;
        float z;
        float last_r = 0;
        float angles[3];
        unsigned int last_time;

        MPU_GYRO(UART_HandleTypeDef &huart1, I2C_HandleTypeDef &hi2C1, communication &comms);


        /**
         * Initialise the mpu6050
         */
        void init();

        /**
         * Calclulates wich inputs to set for the ofsett of the mpu6050, this is not the optimal way as we would prefer to use the internal offsets.
         * But thats still a work in porgress
         */
        void setOffsets();


        /**
         * Get the latest rotations of the mpu6050 using the dmp and send them to the PID controller.
         */
        void update(telementry &telem);
    };


}

#endif //EHECATL_MPU60X0_HPP
