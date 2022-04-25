//
// Created by daang on 28-3-2022.
//

#ifndef EHECATL_BAROMETER_HPP
#define EHECATL_BAROMETER_HPP


#include "stm32f4xx_hal.h"
#include "i2c.h"
#include "communication.hpp"
#include "usart.h"
#include "common_porting.h"
static uint8_t dev_addr;
static uint8_t GTXBuffer[512];
static uint8_t GRXBuffer[2048];
//
///*!
// * I2C read function map to STM32 HAL platform
// */
//BMP3_INTF_RET_TYPE SensorAPI_I2Cx_Read(uint8_t subaddress, uint8_t *pBuffer, uint32_t ReadNumbr, void *intf_ptr)
//{
//    uint8_t dev_addr = *(uint8_t*)intf_ptr;
//    uint16_t DevAddress = dev_addr << 1;
//
//// send register address
//    HAL_I2C_Master_Transmit(&hi2c3, dev_addr, &subaddress, 1, 100);
//    HAL_I2C_Master_Receive(&hi2c3, dev_addr, pBuffer, ReadNumbr, 100);
//    return 0;
//}
//
///*!
// * I2C write function map to STM32 HAL platform
// */
//int8_t SensorAPI_I2Cx_Write(uint8_t subaddress, const uint8_t *pBuffer, uint32_t WriteNumbr, void *intf_ptr)
//{
//    uint8_t dev_addr = *(uint8_t*)intf_ptr;
//    uint16_t DevAddress = dev_addr << 1;
//
//    GTXBuffer[0] = subaddress;
//    memcpy(&GTXBuffer[1], pBuffer, WriteNumbr);
//
//// send register address
//    HAL_I2C_Master_Transmit(&hi2c3, dev_addr, GTXBuffer, WriteNumbr, 100);
//    return 0;
//}
//
//
///*!
// * Delay function map to STM32 HAL platform
// */
//void bmp3_delay_us(uint32_t period, void *intf_ptr)
//{
//    HAL_Delay(period);
//}

BMP3_INTF_RET_TYPE bmp3_interface_init(struct bmp3_dev *bmp3, uint8_t intf)
{
    int8_t rslt = BMP3_OK;

    if(bmp3 != NULL)
    {
        /* Bus configuration : I2C */
        if (intf == BMP3_I2C_INTF)
        {
            dev_addr = BMP3_ADDR_I2C_SEC;
            bmp3->read = &SensorAPI_I2Cx_Read;
            bmp3->write = &SensorAPI_I2Cx_Write;
            bmp3->intf = BMP3_I2C_INTF;
        }
            /* Bus configuration : SPI */
        else if (intf == BMP3_SPI_INTF)
        {
            dev_addr = 0;
//            bmp3->read = SensorAPI_SPIx_Read;
//            bmp3->write = SensorAPI_SPIx_Write;
            bmp3->intf = BMP3_SPI_INTF;
        }

        bmp3->delay_us = &bmp3_delay_us;
        bmp3->intf_ptr = &dev_addr;
    }
    else
    {
        rslt = BMP3_E_NULL_PTR;
    }

    return rslt;
}

namespace EHECATL{
    class Barometer{
    private: //variables
        int8_t rslt = 0;
        uint8_t settings_sel;
        struct bmp3_dev dev;
        struct bmp3_data data = { 0 };
        double b_data[5] = {};
        double sum = 0;

        struct bmp3_settings settings = { 0 };
        struct bmp3_status status = { { 0 } };

        int count = 0;
        bool sending = false;

        communication &comms;
    public:  // variables

    private: //functions

    public: //functions
        Barometer(communication &comms) : comms(comms) {
            /* Interface reference is given as a parameter
             *         For I2C : BMP3_I2C_INTF
             *         For SPI : BMP3_SPI_INTF
             */

            rslt = bmp3_interface_init(&dev, BMP3_I2C_INTF);
            if(rslt != BMP3_OK){
                char msg[100];
                sprintf(msg, "bad interface init\n");
                comms.localMessage(MSG_COMMANDS::ERROR_MESSAGE, (uint8_t *)msg, strlen(msg));
            }

            bmp3_soft_reset(&dev);

            rslt = bmp3_init(&dev);
            if(rslt != BMP3_OK){
                char msg[100];
                sprintf(msg, "bad bmp init: %i \n", rslt);
                comms.localMessage(MSG_COMMANDS::ERROR_MESSAGE, (uint8_t *)msg, strlen(msg));
                while(1){};
            }

            settings.int_settings.drdy_en = BMP3_ENABLE;
            settings.press_en = BMP3_ENABLE;
            settings.temp_en = BMP3_ENABLE;

            settings.odr_filter.press_os = BMP3_OVERSAMPLING_2X;
            settings.odr_filter.temp_os = BMP3_OVERSAMPLING_2X;
            settings.odr_filter.odr = BMP3_ODR_100_HZ;

            settings_sel = BMP3_SEL_PRESS_EN | BMP3_SEL_TEMP_EN | BMP3_SEL_PRESS_OS | BMP3_SEL_TEMP_OS | BMP3_SEL_ODR |
                           BMP3_SEL_DRDY_EN;

            rslt = bmp3_set_sensor_settings(settings_sel, &settings, &dev);
            settings.op_mode = BMP3_MODE_NORMAL;
            rslt = bmp3_set_op_mode(&settings, &dev);
        }


        /**
         * Update function of the barometer, Takes the moving average of 5 measurements and sends it to the communication system.
         */
        void update(){
            rslt = bmp3_get_status(&status, &dev);
            /* Read temperature and pressure data iteratively based on data ready interrupt */

            if ((rslt == BMP3_OK) && (status.intr.drdy == BMP3_ENABLE)) {
                /*
                 * First parameter indicates the type of data to be read
                 * BMP3_PRESS_TEMP : To read pressure and temperature data
                 * BMP3_TEMP       : To read only temperature data
                 * BMP3_PRESS      : To read only pressure data
                 */
                rslt = bmp3_get_sensor_data(BMP3_PRESS, &data, &dev);

                /* NOTE : Read status register again to clear data ready interrupt status */
                rslt = bmp3_get_status(&status, &dev);
                b_data[count] = data.pressure;

                //comms.localMessage(MSG_COMMANDS::NEW_BAROMETER_DATA, (uint8_t *)b_data, 16);
            }
            count++;
            if (count == 5) {
                sending = true;
                count = 0;
            }
            if(sending) {
                sum = 0;
                for (int i = 0; i < 5; i++){
                    sum += b_data[i];
                }
                sum = sum/5.0;

                comms.localMessage(MSG_COMMANDS::NEW_BAROMETER_DATA, (uint8_t *) &sum, 16);
            }
        }
    };
}



#endif //EHECATL_BAROMETER_HPP
