//
// Created by daang on 7-2-2022.
//

#include "communication.hpp"
namespace EHECATL{


    void EHECATL::communication::setup_nrf() {
        nrf.power(true);
        nrf.write_register(nrf24l01::NRF_REGISTER::CONFIG, 0x0F);

        nrf.rx_set_dynamic_payload_length(true);
        nrf.auto_retransmit(5, 15);
        nrf.write_register(nrf24l01::NRF_REGISTER::FEATURE,
                           nrf24l01::NRF_FEATURE::EN_DPL | nrf24l01::NRF_FEATURE::EN_ACK_PAY);

        //high power low speed for long range
        nrf.write_register(nrf24l01::NRF_REGISTER::RF_SETUP, nrf24l01::NRF_RF_SETUP::RF_DR_LOW | nrf24l01::NRF_RF_SETUP::RF_PWR);

        nrf.write_register(nrf24l01::NRF_REGISTER::RF_SETUP, 0x26);
        nrf.write_register(nrf24l01::NRF_REGISTER::SETUP_AW, 0x05);
        nrf.channel(20);

        nrf.rx_set_address(0, this_device_adress);
        nrf.rx_set_address(1, this_device_proxy_address);


        nrf.tx_set_address(target_device_adress);

        char str[100];
        sprintf(str, "writing_adress: %u, %u, %u, %u, %u\n", target_device_adress.address_bytes[0], target_device_adress.address_bytes[1], target_device_adress.address_bytes[2], target_device_adress.address_bytes[3], target_device_adress.address_bytes[4]);
        HAL_UART_Transmit(&huart1, str, strlen(str), 100);
        check();

        nrf.rx_auto_acknowledgement(0, true);
        nrf.rx_auto_acknowledgement(1, true);
        nrf.rx_enabled(0, true);
        nrf.rx_enabled(1, true);

        nrf.mode(nrf.MODE_PRX);
    }

    void communication::update_nrf(telementry &tm, bool is_host) {

        nrf.no_operation();

        while ((nrf.last_status & nrf24l01::NRF_STATUS::RX_DR ) >0) {//there is data to read
            nrf.no_operation();
            //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

            uint8_t wd = nrf.rx_payload_width();
            nrf.rx_read_payload(receive_buffer, wd);

            if (nrf.last_status & nrf24l01::NRF_STATUS::TX_DS) {
                nrf.tx_flush();
            }


            nrf.write_register(nrf24l01::NRF_REGISTER::NRF_STATUS,
                               nrf24l01::NRF_STATUS::RX_DR | nrf24l01::NRF_STATUS::TX_DS);
            nrf.no_operation();

            if(!is_host) {
                onMessageReceived(receive_buffer[0], &receive_buffer[1], wd - 1);
                updateAckPackage();
            }else{
                //handleAckPackage(receive_buffer, wd, tm);
            }
        }
    }

    int communication::onMessageReceived(uint8_t command, uint8_t *payload, uint8_t len) {
        int val = 0;
        for (int i = 0; i < count; i++){
            if (command == commands[i]){
                callbacks[i](command, payload, len);
                val = 1;
            }
        }
        return val;
    }


    communication::communication(SPI_HandleTypeDef &bus, GPIO_TypeDef &csnPort, uint16_t csn, GPIO_TypeDef &cePort, uint16_t ce, telementry &telm)
            : nrf(bus, csnPort, csn, cePort, ce), telm(telm) {
        setup_nrf();
    }

    int communication::localMessage(uint8_t command, uint8_t *payload, uint8_t len) {
        return onMessageReceived(command, payload, len);
    }

    int communication::sendMessage(uint8_t command, uint8_t *payload, uint8_t len) {
        send_buffer[0] = command;
        for(int i = 0; i < len; i++){
            send_buffer[i+1] = payload[i];
        }
        nrf.no_operation();
        uint8_t width;
        int res = nrf.tx_send(send_buffer, len+1, false, receive_buffer, &width);
        if(res){
            handleAckPackage(receive_buffer, width, telm);
        }
        return res;
    }

    void communication::setTargetId(uint8_t id) {
        target_device_id = id;
        target_device_adress.address_bytes[4] = id * 0x05;
        nrf.tx_set_address(target_device_adress);
    }

    void communication::setDeviceId(uint8_t id) {
        this_device_id = id;
        this_device_adress.address_bytes[4] = id*0x05;
        this_device_proxy_address.address_bytes[4] = id*0x05;
        nrf.rx_set_address(0, this_device_adress);
        nrf.rx_set_address(1, this_device_proxy_address);
    }

    void communication::update(telementry & tm, bool is_host) {
        update_nrf(tm, is_host);
    }

    int communication::addNewCallback(uint8_t command, const std::function<void(uint8_t, uint8_t *, uint8_t)> &callback) {
        if(count < COMMAND_COUNT){
            commands[count] = command;
            callbacks[count] = callback;
            count += 1;
            return 1;
        }
        return 0;

    }

    void displayError(communication &comms, uint8_t error_type, char *message, unsigned int len) {
        uint8_t error_message[12+1];
        error_message[0] = error_type;
        memcpy(error_message+1, message, len);
        comms.sendMessage(EHECATL::MSG_COMMANDS::ERROR_MESSAGE, (uint8_t *)message, len);
    }
}
