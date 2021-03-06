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

#ifdef controller_compile
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


#include "controller_main.hpp"
#include "communication.hpp"
#include "remote_controller.hpp"
#include "ADC_helper.hpp"
#include <stdio.h>
#include <string.h>
#include "ST7735_LIB.hpp"
#include "screen.hpp"

#include "adc.h"
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

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int controller_main(void)
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
    MX_ADC1_Init();
    MX_TIM2_Init();
    MX_USART1_UART_Init();
    MX_DMA_Init();
    MX_SPI1_Init();
    MX_SPI2_Init();

    /* USER CODE BEGIN 2 */
    char text[] = "Het apparaat is opgestart\n";
    HAL_UART_Transmit(&huart1, text, strlen(text), 100);
    EHECATL::telementry tm;
    EHECATL::communication comms(hspi1, *GPIOB, GPIO_PIN_0, *GPIOB, GPIO_PIN_1, tm);

    ST7735::ST7735 screen(GPIO_PIN_11, *GPIOA, GPIO_PIN_12, *GPIOA,  GPIO_PIN_0, *GPIOA, hspi2);
    EHECATL::Canvas canvas(128, 160, screen);
    EHECATL::screenManager manager(canvas, comms, tm);
    EHECATL::controller controll(htim2, comms);
    EHECATL::joystick joystick(hadc1, comms, canvas);

    comms.setDeviceId(1);
    comms.setTargetId(2);
    joystick.startup();
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    HAL_Delay(100);
    volatile uint8_t _true = 1;
    unsigned int last_time = HAL_GetTick();
    unsigned int count = 100;

    char count_text[10];

    while (_true)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

        if(HAL_GetTick() - last_time > 100){
            manager.print_data();
            last_time = HAL_GetTick();
            comms.sendMessage(EHECATL::MSG_COMMANDS::PING, nullptr, 0);
        }
        controll.update();

        joystick.update();
        //uncomment when the drone is to also take initative of communication
        //comms.update(tm, true);

        sprintf(count_text, "%d", count);
        canvas.writeAndFlushLine(0, 15, count_text, strlen(count_text), ST7735_GREEN);
        count = count * 1.05;
        if(count > 1234567){
            count = 100;
        }


    }
    /* USER CODE END 3 */
    return 0;
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

