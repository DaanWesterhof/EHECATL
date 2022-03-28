/*******************************************************
* Copyright (C) 2021 Niels Post niels.post.97@gmail.com
*
* This file is part of The Dutch Drift Steering Wheel.
*
* The Dutch Drift Steering Wheel can not be copied and/or distributed without the express
* permission of Niels Post
*******************************************************/

#pragma once

namespace nrf24l01 {
    class startup_test {
    private:
        nrf24l01plus &nrf;
        bool register_success = false;
        bool noack_transmission_success = false;

        void assert_register5_state(uint8_t addr, const uint8_t *state, uint8_t dc_mask = 0x00) {
            uint8_t byte_size = nrf.register_bytes(addr);
            uint8_t register_value[byte_size];
            for (uint8_t i = 0; i < byte_size; i++) {
                register_value[i] = 0;
            }
            nrf.read_register(addr, register_value);
//            hwlib::cout << "addr:" << addr << ",byte_size:" << byte_size ;
            for (uint8_t i = 0; i < byte_size; i++) {
//                hwlib::cout << ",register_value:" << register_value[i] << ",state:" << state[i];
                if (((register_value[i] ^ state[i]) & ~dc_mask) != 0) {
                    hwlib::cout << "Failed register test, " <<
                        "address: " << addr << " - was " << hwlib::hex << register_value[i] << ", but should be "
                                    << state[i] << hwlib::endl;
                    register_success = false;
                    break;
                }
            }
//            hwlib::cout << hwlib::endl;
        }

        void assert_register1_state(uint8_t addr, uint8_t state, uint8_t dc_mask = 0x00) {
            assert_register5_state(addr, &state, dc_mask);
        }

    public:
        startup_test(nrf24l01plus &nrf) : nrf(nrf) {}

        void test_register_reset_states() {
            register_success = true;
            uint8_t test_data[5] = {0};

            assert_register1_state(NRF_REGISTER::CONFIG, 0x08);
            assert_register1_state(NRF_REGISTER::EN_AA, 0x3F);
            assert_register1_state(NRF_REGISTER::EN_RXADDR, 0x03);
            assert_register1_state(NRF_REGISTER::SETUP_AW, 0x03);
            assert_register1_state(NRF_REGISTER::SETUP_RETR, 0x03);
            assert_register1_state(NRF_REGISTER::RF_CH, 0x02);
            assert_register1_state(NRF_REGISTER::RF_SETUP, 0x0E, 1);
            assert_register1_state(NRF_REGISTER::NRF_STATUS, 0x0E);
            assert_register1_state(NRF_REGISTER::NRF_STATUS, nrf.last_status);
            assert_register1_state(NRF_REGISTER::OBSERVE_TX, 0x00);
            assert_register1_state(NRF_REGISTER::RPD, 0x00);


            for (uint8_t i = 0; i < 5; i++) {
                test_data[i] = 0xE7;
            }
            assert_register5_state(NRF_REGISTER::RX_ADDR_P0, test_data);
            assert_register5_state(NRF_REGISTER::TX_ADDR, test_data);
            for (uint8_t i = 0; i < 5; i++) {
                test_data[i] = 0xC2;
            }
            assert_register5_state(NRF_REGISTER::RX_ADDR_P1, test_data);

            assert_register1_state(NRF_REGISTER::RX_ADDR_P2, 0xC3);
            assert_register1_state(NRF_REGISTER::RX_ADDR_P3, 0xC4);
            assert_register1_state(NRF_REGISTER::RX_ADDR_P4, 0xC5);
            assert_register1_state(NRF_REGISTER::RX_ADDR_P5, 0xC6);

            assert_register1_state(NRF_REGISTER::RX_PW_P0, 0x00);
            assert_register1_state(NRF_REGISTER::RX_PW_P1, 0x00);
            assert_register1_state(NRF_REGISTER::RX_PW_P2, 0x00);
            assert_register1_state(NRF_REGISTER::RX_PW_P3, 0x00);
            assert_register1_state(NRF_REGISTER::RX_PW_P4, 0x00);
            assert_register1_state(NRF_REGISTER::RX_PW_P5, 0x00);


            assert_register1_state(NRF_REGISTER::FIFO_STATUS, 0x11);
            assert_register1_state(NRF_REGISTER::DYNPD, 0x00);
            assert_register1_state(NRF_REGISTER::FEATURE, 0x00);
        }

        void test_one_side_transmission() {
            address test_address = {0x34, 0x34, 0x34, 0x34, 0x34};
            uint8_t data[5] = {0};
            nrf.tx_set_address(test_address);
            nrf.rx_set_address(0, test_address);
            nrf.mode(nrf.MODE_PTX);
            nrf.write_register(NRF_REGISTER::FEATURE, 1);
            nrf.tx_write_payload(data, 5, true);
            do {
                nrf.no_operation();
                if ((nrf.last_status & nrf24l01::NRF_STATUS::MAX_RT) > 0) {
                    noack_transmission_success = false;
                }

            } while ((nrf.last_status & nrf24l01::NRF_STATUS::TX_DS) == 0);
            noack_transmission_success = true;
            nrf.mode(nrf.MODE_NONE);
        }

        bool all_successful() {
            return register_success &&
                   noack_transmission_success;
        }
    };


}


