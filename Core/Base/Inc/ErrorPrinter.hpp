//
// Created by daang on 22-4-2022.
//

#ifndef EHECATL_ERRORPRINTER_HPP
#define EHECATL_ERRORPRINTER_HPP

#include "communication.hpp"
#include "usart.h"
namespace EHECATL{

    /**
     * class used to recieve error messages and print them to the terminal
     */
    class ErrorPrinter{
        communication & comms;
        UART_HandleTypeDef & uart;

    public:
        ErrorPrinter(communication &comms, UART_HandleTypeDef &uart);

    private:
        void error_printer(uint8_t command, uint8_t * payload, uint8_t len);
    };
};

#endif //EHECATL_ERRORPRINTER_HPP
