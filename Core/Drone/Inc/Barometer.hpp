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


BMP3_INTF_RET_TYPE bmp3_interface_init(struct bmp3_dev *bmp3, uint8_t intf);

namespace EHECATL{
    class Barometer{
    private: //variables
        int8_t rslt = 0;
        uint8_t settings_sel;
        struct bmp3_dev dev;
        struct bmp3_data data = { 0 };
        double b_data[15] = {};
        double speed_list[15] = {};
        bool send_speed = false;
        uint8_t speed_counter = 0;


        double sum = 0;

        double relative_pressure = 0;

        double current_altitude = 0;
        double speed = 0;

        double last_altitude;
        double last_ticks = 0;

        struct bmp3_settings settings = { 0 };
        struct bmp3_status status = { { 0 } };

        int count = 0;
        bool sending = false;

        communication &comms;
    public:  // variables

    private: //functions

    public: //functions
        Barometer(communication &comms);


        void setBaseHeight();


        double pressureToAltitude(double pressure);


        /**
         * Update function of the barometer, Takes the moving average of 5 measurements and sends it to the communication system.
         */
        void update();
    };
}



#endif //EHECATL_BAROMETER_HPP
