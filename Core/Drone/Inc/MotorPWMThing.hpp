//
// Created by daang on 20-5-2022.
//

#ifndef EHECATL_MOTORPWMTHING_HPP
#define EHECATL_MOTORPWMTHING_HPP
#include "stm32f4xx_hal.h"

namespace EHECATL{

    /**
     * function to write speeds to the motors,
     * @param motor_value uint16_t * to a list of 4 values bewteen 0 and 2000;
     */
    void write_motor_speeds(uint16_t* motor_value);

    /**
     * setup the timers used for the motors
     * @param tim1 timer for motor 1
     * @param tim_1_channel channel for motor 1
     * @param tim2 timer for motor 2
     * @param tim_2_channel channel for motor 2
     * @param tim3 timer for motor 3
     * @param tim_3_channel channel for motor 3
     * @param tim4 timer for motor 4
     * @param tim_4_channel channel for motor 4
     */
    void setTimers(TIM_HandleTypeDef * tim1, uint32_t tim_1_channel ,TIM_HandleTypeDef * tim2, uint32_t tim_2_channel  ,
                   TIM_HandleTypeDef * tim3, uint32_t tim_3_channel ,TIM_HandleTypeDef * tim4,uint32_t tim_4_channel );

    /**
     * Arm the esc's of the motors, first writes low value to motors, then high, then low again.
     */
    void motor_arm();


    /**
     * Initialise the pwm generation for the motors
     */
    void init_motor_pwm();

}

#endif //EHECATL_MOTORPWMTHING_HPP
