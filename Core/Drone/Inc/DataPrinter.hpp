//
// Created by daang on 19-4-2022.
//

#ifndef EHECATL_DATAPRINTER_HPP
#define EHECATL_DATAPRINTER_HPP

#include "usart.h"
#include "communication.hpp"
#include <cstdio>
#include <cstring>

namespace EHECATL{

    class DataPrinter{
        UART_HandleTypeDef & huart;
        communication & comms;


        void print_barometer(uint8_t command, uint8_t * payload, uint8_t len){
            uint8_t buffer[100];
            sprintf((char *)buffer, "barometer_data: %f\n", ((double *)payload)[0]);
            HAL_UART_Transmit(&huart, (char *)buffer, strlen((char*)buffer), 100);
        }

        void print_gyroscope(uint8_t command, uint8_t * payload, uint8_t len){
            uint8_t buffer[100];
            float * data = (float *)payload;
            sprintf((char *)buffer, "x_axis: %f, \ty_axis: %f, \tz_axis: %f\n", data[0], data[1], data[2]);
            HAL_UART_Transmit(&huart, (char *)buffer, strlen((char*)buffer), 100);
        }


    public:

        DataPrinter(UART_HandleTypeDef & huart, communication & comms) : huart(huart), comms(comms) {
            //comms.addNewCallback(MSG_COMMANDS::NEW_BAROMETER_DATA, COMM_CALLBACK(print_barometer));
            //comms.addNewCallback(MSG_COMMANDS::CURRENT_ANGLES, COMM_CALLBACK(print_gyroscope));
        }
    };
}




#endif //EHECATL_DATAPRINTER_HPP
