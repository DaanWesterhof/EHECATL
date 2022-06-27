//
// Created by daang on 7-2-2022.
//

#ifndef EHECATL_COMMUNICATION_HPP
#define EHECATL_COMMUNICATION_HPP

#include "stm32f4xx_hal.h"
#include <functional>
#include "nrf24l01plus/nrf24l01plus.hpp"
#include <cstring>
#include "COMMS_DEFINES.hpp"
#include "telementry.h"

//the amound of commands supported by the communication class

// the maximum number of callbacks function on the system.
#ifndef COMMAND_COUNT
#define COMMAND_COUNT 30
#endif

// a macro function to turn a public member function into a lambda call that can be used by the communication system
#define COMM_CALLBACK(functionname)  [&](uint8_t command, uint8_t * payload, uint8_t len) { this->functionname(command, payload, len);}

namespace EHECATL {

    /**
     * Class used for communication between components/modules.
     */
    class communication{
    private: //variables
        //variables used for storing and accesing the functions
        uint8_t commands[COMMAND_COUNT] = {};
        std::function<void(uint8_t command, uint8_t * payload, uint8_t len)> callbacks[COMMAND_COUNT];
        unsigned int count = 0;


        nrf24l01::nrf24l01plus nrf;
        uint8_t this_device_id = 1;
        uint8_t target_device_id = 2;
        nrf24l01::address this_device_adress = {0x01, 0x02, 0x03, 0x04, 0x06};
        nrf24l01::address this_device_proxy_address = {0x02, 0x02, 0x03, 0x04, 0x06};
        nrf24l01::address target_device_adress = {0x01, 0x02, 0x03, 0x04, 0x05};

        //data receive buffer
        uint8_t receive_buffer[100] = {};

        //data send buffer
        uint8_t send_buffer[200] = {};

        telementry &telm;



    private: //functions

        /**
         * Setup the nrf chip for communication
         */
        void setup_nrf();

        /**
         * Updates the nrf communication, checks if new messages are available, and if so it will execture the callbacks associated with them.
         */
        void update_nrf(telementry &tm, bool is_host = false);

        /**
         * Function that is exetuted when a message is recieved, it checks if there is a callback for the command and executes those callbacks.
         * @param command The command that is recieved
         * @param payload The payload of the message
         * @param len The length of the payload in bytes
         * @return 1 if a callback has been found for the command, 0 if no callback was found.
         */
        virtual int onMessageReceived(uint8_t command, uint8_t * payload, uint8_t len);

    public:  //functions

        /**
         * prints the adress writen to the nrf chip and the adres read from it.
         */
        void check();


        /**
         * function to be called when the ping command is recieved. Uncomment the codeblock to enable printing to the terminal for debugging
         * @param command the command that called the function
         * @param payload the payload of the message
         * @param len the length of the message
         */
        void pong(uint8_t command, uint8_t *payload, uint8_t len);


        /**
         * update the ack package with new data from the telementry data.
         */
        void updateAckPackage();

        /**
         * function to handle the payload from an ack message
         * @param data the data from the payload
         * @param len the length of the payload
         * @param tm reference to a telementry packate where we can store the data
         */
        void handleAckPackage(uint8_t * data, size_t len, telementry & tm);


        /**
         * Constructor for the communication class
         * @param bus A reference to a SPI handle from the stm32 HAL
         * @param csnPort The HAL gpio port of the pin used for csn
         * @param csn the pin number used for the csn pin
         * @param cePort The HAL gpio port of the pin used for CE
         * @param ce the pin number used for the ce pin
         */
        communication(SPI_HandleTypeDef &bus, GPIO_TypeDef &csnPort, uint16_t csn, GPIO_TypeDef &cePort, uint16_t ce, telementry &telm);

        /**
         * Send a message to the local communicator to communicate with other modules on the same device.
         * @param command The command/identifier for the message
         * @param payload The payload of the message
         * @param len The byte length of the message
         * @return 1 if the message has been used succesfully, 0 if no callback relating to that command has been found.
         */
        int localMessage(uint8_t command, uint8_t * payload, uint8_t len);

        /**
         * Send a message to the target device
         * @param command The command/indentifier of the message
         * @param payload The payload of the message
         * @param len The length of the payload in bytes
         * @return 1 if the message has been send correctly, 0 if a problem occured.
         */
        virtual int sendMessage(uint8_t command, uint8_t * payload, uint8_t len);

        /**
         * Set the id of the target device (the device you want to send messages to)
         * @param id The id of the target device
         */
        void setTargetId(uint8_t id);

        /**
         * Set the id of this device (the id on what you want to recieve on).
         * @param id The id of this device
         */
        void setDeviceId(uint8_t id);


        /**
         * Update the communications with other devices/recieve messages. Should be called often/in the main loop of the program.
         * @param tm reference to the telementry object, is used to either store or send telementry data
         * @param is_host boolean to tell if this device is the host of communication, defaults to false.
         */
        void update(telementry & tm, bool is_host = false);

        /**
         * Add a new callback to the communicator class. This can be a member function by callig it with a lambda, you can use the COMM_CALLBACK define for this functionality
         * addNewCallback(0, COMM_CALLBACK(memberfunction_name))
         * @param command The command on wich the function should be executed
         * @param callback The function that needs to be executed on the given command
         * @return 1 if callback is added successfully, 0 if there is not enough space for more callbacks.
         */
        int addNewCallback (uint8_t command, const std::function<void(uint8_t command, uint8_t * payload, uint8_t len)>& callback);
    };

    /**
     * max message length = 12
     * @param comms
     * @param error_type
     * @param message
     * @param len
     */
    void displayError(communication & comms, uint8_t error_type, char * message, unsigned int len);
}

#endif //EHECATL_COMMUNICATION_HPP
