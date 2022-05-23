//
// Created by daang on 20-5-2022.
//

#include "MotorPWMThing.hpp"

TIM_HandleTypeDef *MOTOR_1_TIM;
TIM_HandleTypeDef *MOTOR_2_TIM;
TIM_HandleTypeDef *MOTOR_3_TIM;
TIM_HandleTypeDef *MOTOR_4_TIM;

uint32_t MOTOR_1_TIM_CHANNEL;
uint32_t MOTOR_2_TIM_CHANNEL;
uint32_t MOTOR_3_TIM_CHANNEL;
uint32_t MOTOR_4_TIM_CHANNEL;

#define max_motor_value 2000

void EHECATL::setTimers(TIM_HandleTypeDef *tim1, uint32_t tim_1_channel, TIM_HandleTypeDef *tim2, uint32_t tim_2_channel, TIM_HandleTypeDef *tim3,
                        uint32_t tim_3_channel, TIM_HandleTypeDef *tim4, uint32_t tim_4_channel) {
    MOTOR_1_TIM = tim1;
    MOTOR_2_TIM = tim2;
    MOTOR_3_TIM = tim3;
    MOTOR_4_TIM = tim4;

    MOTOR_1_TIM_CHANNEL = tim_1_channel;
    MOTOR_2_TIM_CHANNEL = tim_2_channel;
    MOTOR_3_TIM_CHANNEL = tim_3_channel;
    MOTOR_4_TIM_CHANNEL = tim_4_channel;
}

void EHECATL::write_motor_speeds(uint16_t *motor_value) {
//    for(int i = 0; i < 4; i++){
//        if(motor_value[i] > max_motor_value){
//            motor_value[i] = max_motor_value;
//        }
//    }
    __HAL_TIM_SET_COMPARE(MOTOR_1_TIM, MOTOR_1_TIM_CHANNEL, motor_value[0]);
    __HAL_TIM_SET_COMPARE(MOTOR_2_TIM, MOTOR_2_TIM_CHANNEL, motor_value[1]);
    __HAL_TIM_SET_COMPARE(MOTOR_3_TIM, MOTOR_3_TIM_CHANNEL, motor_value[2]);
    __HAL_TIM_SET_COMPARE(MOTOR_4_TIM, MOTOR_4_TIM_CHANNEL, motor_value[3]);
}

void EHECATL::init_motor_pwm() {
    HAL_TIM_Base_Start(MOTOR_1_TIM); //Starts the TIM Base generation
    HAL_TIM_PWM_Start(MOTOR_1_TIM, MOTOR_1_TIM_CHANNEL);//Starts the PWM signal generation

    HAL_TIM_Base_Start(MOTOR_2_TIM); //Starts the TIM Base generation
    HAL_TIM_PWM_Start(MOTOR_2_TIM, MOTOR_2_TIM_CHANNEL);//Starts the PWM signal generation

    HAL_TIM_Base_Start(MOTOR_3_TIM); //Starts the TIM Base generation
    HAL_TIM_PWM_Start(MOTOR_3_TIM, MOTOR_3_TIM_CHANNEL);//Starts the PWM signal generation

    HAL_TIM_Base_Start(MOTOR_4_TIM); //Starts the TIM Base generation
    HAL_TIM_PWM_Start(MOTOR_4_TIM, MOTOR_4_TIM_CHANNEL);//Starts the PWM signal generation

}

void EHECATL::motor_arm() {
    uint16_t motor_low[4] = {1000,1000, 1000, 1000};
    uint16_t motor_high[4] = {1950, 1950, 1950, 1950};
    write_motor_speeds(motor_low);
    HAL_Delay(100);
    write_motor_speeds(motor_high);
    HAL_Delay(2000);
    write_motor_speeds(motor_low);
}
