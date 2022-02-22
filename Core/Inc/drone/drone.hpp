//
// Created by daang on 14-2-2022.
//

#ifndef EHECATL_DRONE_HPP
#define EHECATL_DRONE_HPP
#include "stm32f4xx_hal.h"
#include "communication.hpp"
#include "PID.hpp"

namespace EHECATL{
    class drone{
    private: //variables

        TIM_HandleTypeDef &timer;
        EHECATL::communication &communicator;




    public: //variables

    private: //functions

        void setPWMSpeeds(uint8_t command, uint8_t * data, uint8_t len){
            uint8_t printer[100] = {};
            timer.Instance->CCR2 = uint32_t(data[0]);
            timer.Instance->CCR3 = uint32_t(data[1]);

            sprintf((char *)printer, "rX:%u rY:%u lX:%u lY:%u\n", uint8_t(data[0]), uint8_t(data[1]), uint8_t(data[2]), uint8_t(data[3]));
            HAL_UART_Transmit(&huart1, (uint8_t*)printer, strlen((char *)printer), 100);

        }

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



    public: //functions
        drone(TIM_HandleTypeDef &timer, EHECATL::communication &communicator) : timer(timer), communicator(communicator) {
            setPWM(timer, TIM_CHANNEL_2, 255, 0);
            setPWM(timer, TIM_CHANNEL_3, 255, 0);
            communicator.addNewCallback('0', COMM_CALLBACK(setPWMSpeeds));
        }

        void update(){
            //y_pid.calulateAction()
        }
    };
}

#endif //EHECATL_DRONE_HPP
