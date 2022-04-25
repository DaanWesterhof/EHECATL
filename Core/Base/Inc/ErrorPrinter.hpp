//
// Created by daang on 22-4-2022.
//

#ifndef EHECATL_ERRORPRINTER_HPP
#define EHECATL_ERRORPRINTER_HPP

#include "communication.hpp"
#include "usart.h"
namespace EHECATL{
    class ErrorPrinter{
        communication & comms;
        UART_HandleTypeDef & uart;

    public:
        ErrorPrinter(communication &comms, UART_HandleTypeDef &uart) : comms(comms), uart(uart) {
            comms.addNewCallback(MSG_COMMANDS::ERROR_MESSAGE, COMM_CALLBACK(error_printer));
        }

    private:
        void error_printer(uint8_t command, uint8_t * payload, uint8_t len){
            char * data = (char *)payload;
            HAL_UART_Transmit(&uart, data, len, 100);
        }
    };
};

#endif //EHECATL_ERRORPRINTER_HPP
