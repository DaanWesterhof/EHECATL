//
// Created by daang on 14-2-2022.
//
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifdef drone_compile
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MPU6050.h"

#include "drone_main.hpp"
#include "communication.hpp"
#include "drone.hpp"
#include "Barometer.hpp"
#include "DataPrinter.hpp"
#include "ErrorPrinter.hpp"
#include "StateController.hpp"
#include <cstdio>
#include <cstring>
#include <MotorPWMThing.hpp>

#include "dma.h"
#include "gpio.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PI 3.14159265359
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void print_state(uint8_t command, uint8_t * payload, uint8_t len){
    char data[100];
    sprintf(data, "new_state: %u\n", *payload);
    HAL_UART_Transmit(&huart1, data, strlen(data), 100);
}




/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int drone_main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();

    EHECATL::setTimers(&htim2, TIM_CHANNEL_1, &htim2, TIM_CHANNEL_2, &htim2, TIM_CHANNEL_3, &htim4, TIM_CHANNEL_1);
    EHECATL::init_motor_pwm();
    bool armed = false;
    EHECATL::motor_arm();
    armed = true;

    MX_USART1_UART_Init();
    MX_DMA_Init();
    MX_SPI1_Init();
    MX_I2C1_Init();
    MX_I2C3_Init();

    /* USER CODE BEGIN 2 */
    HAL_Delay(3000);
    uint8_t text_buffer[100];
    sprintf((char *)text_buffer, "Het apparaat is opgestart\n");
    HAL_UART_Transmit(&huart1, (char *)text_buffer, strlen((char*)text_buffer), 100);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

    EHECATL::communication comms(hspi1, *GPIOB, GPIO_PIN_0, *GPIOB, GPIO_PIN_1);
    EHECATL::ErrorPrinter error_printer(comms, huart1);
    EHECATL::StateController state_controller(comms);

    EHECATL::PID_Controller pid_controller(comms);
    EHECATL::Motors motors(comms);
    EHECATL::MPU_GYRO mpu(huart1, hi2c1, comms);
    EHECATL::Barometer barometer(comms);
    EHECATL::DataPrinter printer(huart1, comms);
    mpu.init();





    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    HAL_Delay(100);
    volatile uint8_t _true = 1;
    state_controller.setState(EHECATL::DRONE_MODES::SLEEP);
    comms.setDeviceId(2);
    comms.setTargetId(1);
    unsigned int last_update = 0;
    EHECATL::DRONE_MODE current_mode = state_controller.getState();


    int16_t change[4] = {};
    uint16_t motor_speeds[4] = {};
    uint16_t max_value = 1500;


    while (_true)
    {

        if(HAL_GetTick() - last_update > 250){
            uint8_t st = state_controller.getState();
            comms.sendMessage(EHECATL::MSG_COMMANDS::NEW_STATE, &st, 1);
            last_update = HAL_GetTick();
        }
        mpu.update();
        barometer.update();

        if(state_controller.getState() == EHECATL::DRONE_MODES::FLYING){
            motors.getChange(change);
            for(int i = 0; i < 4 ; i++){
                motor_speeds[i] += change[i];
                if(motor_speeds[i] <1000){
                    motor_speeds[i] = 1000;
                }else if(motor_speeds[i] > max_value){
                    motor_speeds[i] = max_value;
                }
            }
            EHECATL::write_motor_speeds(motor_speeds);
        }
    }
    return 0;
    /* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

#endif

