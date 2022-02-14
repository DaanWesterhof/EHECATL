//
// Created by daang on 7-2-2022.
//

#ifndef EHECATL_REMOTE_CONTROLLER_HPP
#define EHECATL_REMOTE_CONTROLLER_HPP


#include "stm32f4xx_hal.h"
#include "communication.hpp"


namespace EHECATL{





    class controller{
    private: //variables
        //screen
        //joysticks
        TIM_HandleTypeDef &timer;
        EHECATL::communication &communicator;
        int state = 1;

    public: //variables

    private: //functions
        void setPWM(TIM_HandleTypeDef timer, uint32_t channel, uint32_t period, uint32_t pulse) {
            HAL_TIM_PWM_Stop(&timer, channel); // stop generation of pwm
            TIM_OC_InitTypeDef sConfigOC;
            timer.Init.Period = period; // set the period duration
            HAL_TIM_PWM_Init(&timer); // reinititialise with new period value
            sConfigOC.OCMode = TIM_OCMODE_PWM1;
            sConfigOC.Pulse = pulse; // set the pulse duration
            sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
            sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
            HAL_TIM_PWM_ConfigChannel(&timer, &sConfigOC, channel);
            HAL_TIM_PWM_Start(&timer, channel); // start pwm generation
        }

        void test_function(uint8_t command, uint8_t * payload, uint8_t len){
            setPWM(timer, TIM_CHANNEL_2,  4096, ((unsigned int *)payload)[0]);
            setPWM(timer, TIM_CHANNEL_3,  4096, ((unsigned int *)payload)[1]);
        }

        void second_function(uint8_t command, uint8_t * payload, uint8_t len){
            if(state == 1){
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
                state = 0;
            }else{
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
                state = 1;
            }
        }

        int init(){
            communicator.addNewCallback(005, COMM_CALLBACK(test_function));
            communicator.addNewCallback(006, COMM_CALLBACK(second_function));
            return 1;
        }

    public: //functions
        controller(TIM_HandleTypeDef &timer, EHECATL::communication &communicator) : timer(timer), communicator(communicator) {
            init();
        }

    };
}

#endif //EHECATL_REMOTE_CONTROLLER_HPP
