//
// Created by daang on 27-6-2022.
//

#include "ErrorPrinter.hpp"
EHECATL::ErrorPrinter::ErrorPrinter(communication &comms, UART_HandleTypeDef &uart) : comms(comms), uart(uart) {
    comms.addNewCallback(MSG_COMMANDS::ERROR_MESSAGE, COMM_CALLBACK(error_printer));
}

void EHECATL::ErrorPrinter::error_printer(uint8_t command, uint8_t *payload, uint8_t len) {
    char * data = (char *)payload;
    HAL_UART_Transmit(&uart, data, len, 100);
}
