//
// Created by daang on 7-2-2022.
//

#ifndef EHECATL_COMMUNICATION_HPP
#define EHECATL_COMMUNICATION_HPP

#include "stm32f4xx_hal.h"
#include <functional>
#include <cpp_nrf24l01/include/nrf24l01plus/nrf24l01plus.hpp>
#include <cstring>

#define COMMAND_COUNT 10

#define COMM_CALLBACK(functionname)  [&](uint8_t command, uint8_t * payload, uint8_t len) { this->functionname(command, payload, len);}

namespace EHECATL {


    class communication{
    private: //variables
        unsigned int commands[COMMAND_COUNT];
        std::function<void(uint8_t command, uint8_t * payload, uint8_t len)> callbacks[COMMAND_COUNT];
        unsigned int count = 0;

    public: //variables

    private: //functions


    public:  //functions
        virtual int onMessageReceived(uint8_t command, uint8_t * payload, uint8_t len){
            for (int i = 0; i < count; i++){
                if (command == commands[i]){
                    callbacks[i](command, payload, len);
                }
            }
            return 1;
        }

        virtual int sendMessage(){
            return -1;
        }

        int addNewCallback (uint8_t command, const std::function<void(uint8_t command, uint8_t * payload, uint8_t len)>& callback){
            if(count < COMMAND_COUNT){
                commands[count] = command;
                callbacks[count] = callback;
                count += 1;
                return 1;
            }
            return -1;

        }
    };

    class NrfRunner {
        EHECATL::communication &communicator;
        nrf24l01::nrf24l01plus nrf;
        uint8_t this_device_id = 200;
        uint8_t target_device_id = 2;
        uint8_t session_id = 0;
        unsigned int pongs = 0;
        nrf24l01::address this_device_adress = {0x01, 0x02, 0x03, 0x04, 0x06};
        nrf24l01::address this_device_proxy_address = {0x02, 0x02, 0x03, 0x04, 0x06};
        nrf24l01::address target_device_adress = {0x01, 0x02, 0x03, 0x04, 0x05};
//big one adress(  rx0 0 0 0 1 / tx0 0 1 0 0)
//small one adress( rx0 0 0 1 0 / tx0 0 0 1 0)
        uint8_t receive_buffer[100] = {};
    public:
        NrfRunner(communication &communicator, SPI_HandleTypeDef &bus, GPIO_TypeDef &csnPort, uint16_t csn, GPIO_TypeDef &cePort, uint16_t ce) : communicator(communicator), nrf(bus, csnPort, csn, cePort, ce) {
            communicator.addNewCallback(uint8_t('0'), COMM_CALLBACK(ping));
//            communicator.addNewCallback(uint8_t('1'), COMM_CALLBACK(pong));
//            communicator.addNewCallback(uint8_t('2'), COMM_CALLBACK(pongRecieved));
        }

        void ping(uint8_t command, uint8_t * payload, uint8_t len){
//            nrf.mode(nrf.MODE_PTX);
//            nrf.tx_set_address(target_device_adress);
//            nrf.rx_set_address(0, target_device_adress);

            HAL_UART_Transmit(&huart1, (uint8_t *) "We send the ping\n", 17, HAL_MAX_DELAY);


            uint8_t data[] = "1here is the a ping";

            nrf.no_operation();

            uint8_t result = nrf.tx_send(data, 19);

            uint8_t stringtext[100] = {};

            sprintf((char*)stringtext, "Het resultaat = %u\n", result);
            HAL_UART_Transmit(&huart1, stringtext, strlen((char *)stringtext), HAL_MAX_DELAY);
//            nrf.tx_write_payload(data, 19);
//            nrf.tx_send_payload();

//            nrf.rx_set_address(0, this_device_adress);
//
//            HAL_Delay(100);
//            uint8_t is_sent = nrf.last_status & nrf24l01::NRF_STATUS::TX_DS;
//            nrf.mode(nrf.MODE_PRX);

        }

        void pong(uint8_t command, uint8_t * payload, uint8_t len){
//            nrf.mode(nrf.MODE_PTX);
//            nrf.tx_set_address(target_device_adress);
//            nrf.rx_set_address(0, target_device_adress);


            uint8_t data[] = "2here is the a pong";
            //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
            nrf.tx_send(data, 19);
//            nrf.tx_write_payload(data, 19);
//            nrf.tx_send_payload();
            //return the ping as a pong


//            nrf.rx_set_address(0, this_device_adress);
//
//            HAL_Delay(100);
//            uint8_t is_sent = nrf.last_status & nrf24l01::NRF_STATUS::TX_DS;
//            nrf.mode(nrf.MODE_PRX);

            HAL_UART_Transmit(&huart1, (uint8_t *) "We returned a pong\n", 19, HAL_MAX_DELAY);


        }

        void pongRecieved(uint8_t command, uint8_t * payload, uint8_t len){
            pongs += 1;
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
            HAL_UART_Transmit(&huart1, (uint8_t *) "We received a pong\n", 19, HAL_MAX_DELAY);
        }


        void run() {
            nrf.no_operation();

            while ((nrf.last_status & nrf24l01::NRF_STATUS::RX_DR ) >0) {//there is data to read
                char message[] = "We wanna recieved data\n";
                HAL_UART_Transmit(&huart1, (uint8_t *) message, 23, HAL_MAX_DELAY);
                nrf.no_operation();
                HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);


                uint8_t wd = nrf.rx_payload_width();
                nrf.rx_read_payload(receive_buffer, wd);

                HAL_UART_Transmit(&huart1, receive_buffer, wd, HAL_MAX_DELAY);
                HAL_UART_Transmit(&huart1, (uint8_t *) "\n", 1, HAL_MAX_DELAY);

                communicator.onMessageReceived(receive_buffer[0], &receive_buffer[1], wd - 1);


                if (nrf.last_status & nrf24l01::NRF_STATUS::TX_DS) {
                    nrf.tx_flush();
                }


                nrf.write_register(nrf24l01::NRF_REGISTER::NRF_STATUS,
                                   nrf24l01::NRF_STATUS::RX_DR | nrf24l01::NRF_STATUS::TX_DS);
                nrf.no_operation();
            }
    }

        void setup_nrf(){
            nrf.power(true);
            nrf.write_register(nrf24l01::NRF_REGISTER::CONFIG, 0x0F);

            nrf.rx_set_dynamic_payload_length(true);
            nrf.auto_retransmit(5, 15);
            nrf.write_register(nrf24l01::NRF_REGISTER::FEATURE,
                               nrf24l01::NRF_FEATURE::EN_DPL | nrf24l01::NRF_FEATURE::EN_ACK_PAY);

            nrf.write_register(nrf24l01::NRF_REGISTER::RF_SETUP, 0x26);
            nrf.write_register(nrf24l01::NRF_REGISTER::SETUP_AW, 0x05);
            nrf.channel(20);

            nrf.rx_set_address(0, this_device_adress);
            nrf.rx_set_address(1, this_device_proxy_address);
            nrf.tx_set_address(target_device_adress);

            nrf.rx_auto_acknowledgement(0, true);
            nrf.rx_auto_acknowledgement(1, true);
            nrf.rx_enabled(0, true);
            nrf.rx_enabled(1, true);

            nrf.mode(nrf.MODE_PRX);
        }
    };

}

#endif //EHECATL_COMMUNICATION_HPP
