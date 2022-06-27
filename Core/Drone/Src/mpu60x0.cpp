//
// Created by daang on 16-2-2022.
//

#include "mpu60x0.hpp"

namespace EHECATL{

    MPU_GYRO::MPU_GYRO(UART_HandleTypeDef &huart1, I2C_HandleTypeDef &hi2C1, communication &comms): huart1(huart1), hi2c1(hi2C1), comms(comms) {}

    void MPU_GYRO::setOffsets() {
        uint8_t text_buffer[100];
        float tempsets[3] = {};
        for (int i = 0; i < 300; i++){
            while (fifoCount < packetSize) {
                //insert here your code
                fifoCount = mpu.getFIFOCount();
            }
            if (fifoCount >= 1024) {
                mpu.resetFIFO();
            }
            else{
                if (fifoCount % packetSize != 0) {
                    mpu.resetFIFO();
                    fifoCount = mpu.getFIFOCount();
                }
                else{
                    while (fifoCount >= packetSize) {
                        mpu.getFIFOBytes(fifoBuffer,packetSize);
                        fifoCount -= packetSize;
                    }
                    mpu.dmpGetQuaternion(&q,fifoBuffer);
                    mpu.dmpGetGravity(&gravity,&q);
                    mpu.dmpGetYawPitchRoll(ypr,&q,&gravity);
                    if(i > 250) {
                        tempsets[1] += ypr[1] * 180 / PI;
                        tempsets[2] += ypr[2] * 180 / PI;
                        tempsets[0] += ypr[0] * 180 / PI;
                    }
                }
            }
        }

        ofsets[1] = tempsets[1]/50;
        ofsets[2] = tempsets[2]/50;
        ofsets[0] = tempsets[0]/50;
    }

    void MPU_GYRO::init() {
        uint8_t text_buffer[100];
        I2Cdev_init(&hi2c1);
        mpu.initialize();
        mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
        mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250);

        uint8_t val = mpu.dmpInitialize();

#ifdef DRONE_DEBUG
        if(val == 0){
            HAL_UART_Transmit(&huart1, (char *)text_buffer, sprintf((char *)text_buffer, "Dmp Initialised\n"), 100);

        }else{

            HAL_UART_Transmit(&huart1, (char *)text_buffer, sprintf((char *)text_buffer, "Something went wrong with dmp initalisation: %u\n", val), 100);
        }
#endif

        mpu.setDMPEnabled(true);
        packetSize = mpu.dmpGetFIFOPacketSize();
        fifoCount = mpu.getFIFOCount();
    }

    void MPU_GYRO::update(telementry &telem) {
        fifoCount = mpu.getFIFOCount();
        if (fifoCount < packetSize) {
            return;
        }

        if (fifoCount >= 1024) {
            mpu.resetFIFO();
        }
        else{
            if (fifoCount % packetSize != 0) {
                mpu.resetFIFO();
                fifoCount = mpu.getFIFOCount();
            }
            else{
                while (fifoCount >= packetSize) {
                    mpu.getFIFOBytes(fifoBuffer,packetSize);
                    fifoCount -= packetSize;
                }
                mpu.dmpGetQuaternion(&q,fifoBuffer);
                mpu.dmpGetGravity(&gravity,&q);
                mpu.dmpGetYawPitchRoll(ypr,&q,&gravity);
                x = ypr[1]*180/PI - ofsets[1];
                y = ypr[2]*180/PI - ofsets[2];
                z = ypr[0]*180/PI - ofsets[0];
            }
        }
        float r_speed = (z - last_r)/(HAL_GetTick()-last_time)*1000;
        last_time = HAL_GetTick();
        last_r = z;
        angles[0] = x;
        angles[1] = y;
        angles[2] = r_speed;
    #ifdef DRONE_DEBUG
        char text_buffer[100];
        sprintf((char *) text_buffer, "x: %2.6f, y: %2.6f, z: %2.6f\t", angles[0], angles[1], angles[2]);
        HAL_UART_Transmit(&huart1, (char *) text_buffer, strlen((char *) text_buffer), 100);
    #endif
        comms.localMessage(MSG_COMMANDS::CURRENT_ANGLES, (uint8_t *)angles, 3*sizeof(float));
    }
}
