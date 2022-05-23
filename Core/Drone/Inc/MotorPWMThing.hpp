//
// Created by daang on 20-5-2022.
//

#ifndef EHECATL_MOTORPWMTHING_HPP
#define EHECATL_MOTORPWMTHING_HPP
#include "stm32f4xx_hal.h"

namespace EHECATL{

    void write_motor_speeds(uint16_t* motor_value);


    void setTimers(TIM_HandleTypeDef * tim1, uint32_t tim_1_channel ,TIM_HandleTypeDef * tim2, uint32_t tim_2_channel  ,
                   TIM_HandleTypeDef * tim3, uint32_t tim_3_channel ,TIM_HandleTypeDef * tim4,uint32_t tim_4_channel );

    void motor_arm();


    void init_motor_pwm();

}

#endif //EHECATL_MOTORPWMTHING_HPP
