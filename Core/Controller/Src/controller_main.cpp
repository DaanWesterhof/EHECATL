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

    EHECATL::communication comms(hspi1, *GPIOB, GPIO_PIN_0, *GPIOB, GPIO_PIN_1);
    EHECATL::controller controll(htim2, comms);
    EHECATL::joystick joystick(hadc1, comms);
    ST7735::ST7735 screen(GPIO_PIN_11, *GPIOA, GPIO_PIN_12, *GPIOA,  GPIO_PIN_0, *GPIOA, hspi2);
    EHECATL::Canvas canvas(128, 160, screen);
    EHECATL::screenManager manager(canvas, comms);
    comms.setDeviceId(1);
    comms.setTargetId(2);
    joystick.startup();
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    HAL_Delay(100);
    volatile uint8_t _true = 1;
    unsigned int last_time = HAL_GetTick();
    unsigned int count = 0;
    char _1[] = "00001";
    char _2[] = "00010";
    char _3[] = "00100";
    char _4[] = "01000";
    char _5[] = "10000";

    char count_text[] = "1111";

    while (_true)
    {
        //screen.FillRectangle(0, 0, 128, 160, 23030);
        //count_text[0] = char(count+32);

        count++;
        if(count == 1){
            canvas.writeAndFlushLine(0, 4, _1, 5, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
            canvas.writeAndFlushLine(0, 5, _1, 5, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
            canvas.writeAndFlushLine(0, 6, _1, 5, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
            HAL_Delay(100);
        }else if(count  == 2){
            canvas.writeAndFlushLine(0, 4, _2, 5, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
            canvas.writeAndFlushLine(0, 5, _2, 5, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
            canvas.writeAndFlushLine(0, 6, _2, 5, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
            HAL_Delay(100);
        }else if(count  == 3){
            canvas.writeAndFlushLine(0, 4, _3, 5, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
            canvas.writeAndFlushLine(0, 5, _3, 5, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
            canvas.writeAndFlushLine(0, 6, _3, 5, ST7735_COLOR565(0x1f, 0x3f, 0x1f));

            HAL_Delay(100);
        }else if(count  == 4){
            canvas.writeAndFlushLine(0, 4, _4, 5, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
            canvas.writeAndFlushLine(0, 5, _4, 5, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
            canvas.writeAndFlushLine(0, 6, _4, 5, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
            HAL_Delay(100);
        }else if(count  == 5){
            canvas.writeAndFlushLine(0, 4, _5, 5, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
            canvas.writeAndFlushLine(0, 5, _5, 5, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
            canvas.writeAndFlushLine(0, 6, _5, 5, ST7735_COLOR565(0x1f, 0x3f, 0x1f));
            HAL_Delay(100);
            count = 0;
        }

        canvas.writeAndFlushLine(0, 7, count_text, 4, ST7735_COLOR565(0x1f, 0x3f, 0x1f));



        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

//        if(HAL_GetTick() - last_time > 50){
//            //manager.print_data();
//            last_time = HAL_GetTick();
//        }

//        joystick.update();
//        comms.update();

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

