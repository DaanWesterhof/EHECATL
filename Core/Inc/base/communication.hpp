//
// Created by daang on 7-2-2022.
//

#ifndef EHECATL_COMMUNICATION_HPP
#define EHECATL_COMMUNICATION_HPP

#include "stm32f4xx_hal.h"
#include <functional>

#define COMM_CALLBACK(functionname)  [&](unsigned int command, unsigned int * payload) { this->functionname(command, payload);}

namespace EHECATL {

    template<int COMMAND_COUNT>
    class communication{
    private: //variables
        unsigned int commands[COMMAND_COUNT];
        std::function<void(unsigned int command, unsigned int * payload)> callbacks[COMMAND_COUNT];
        unsigned int count = 0;

    public: //variables

    private: //functions


    public:  //functions
        virtual int onMessageReceived(unsigned int command, unsigned int * payload){
            for (int i = 0; i < count; i++){
                if (command == commands[i]){
                    callbacks[i](command, payload);
                }
            }
            return 1;
        }

        virtual int sendMessage(){
            return -1;
        }

        int addNewCallback (unsigned int command, const std::function<void(unsigned int command, unsigned int * payload)>& callback){
            if(count < COMMAND_COUNT){
                commands[count] = command;
                callbacks[count] = callback;
                count += 1;
                return 1;
            }
            return -1;

        }
    };

}

#endif //EHECATL_COMMUNICATION_HPP
