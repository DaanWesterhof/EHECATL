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
void print_state(uint8_t command, uint8_t *payload, uint8_t len) {
    char data[100];
    sprintf(data, "new_state: %u\n", *payload);
    HAL_UART_Transmit(&huart1, data, strlen(data), 100);
}


class motorStateController {
public:
    motorStateController(EHECATL::Motors &motors, EHECATL::communication &comms, EHECATL::StateController &stateController)
            : motors(motors), comms(comms), stateController(stateController) {
        comms.addNewCallback(EHECATL::MSG_COMMANDS::ALTITUDE_SPEED, COMM_CALLBACK(setSpeed));
        comms.addNewCallback(EHECATL::MSG_COMMANDS::JOYSTICK_ANGLES, COMM_CALLBACK(joystickListener));
        comms.addNewCallback(EHECATL::MSG_COMMANDS::NEW_STATE, COMM_CALLBACK(state_listener));
    }


private:
    EHECATL::Motors &motors;
    EHECATL::communication &comms;
    EHECATL::StateController &stateController;

    int16_t change[4] = {};

    uint16_t motor_speeds[4] = {};
    uint16_t max_value = 1500;
    bool send_joysticks = false;
    double drone_speed = 0;
    unsigned int time_since_landing = 0;
    bool landing = false;

public:

    void joystickListener(uint8_t command, uint8_t *payload, uint8_t len) {
        if (stateController.getState() == EHECATL::DRONE_MODES::LANDING) {
            stateController.setState(EHECATL::DRONE_MODES::FLYING);
            landing = false;
        }
    }

    void setSpeed(uint8_t command, uint8_t *payload, uint8_t len) {
        drone_speed = *((double *) payload);
    }

    void state_listener(uint8_t command, uint8_t *payload, uint8_t len) {
        if (*payload == EHECATL::DRONE_MODES::LANDING && !landing) {
            landing = true;
            float angles[4] = {-10, 0, 0, 0};
            send_joysticks = true;
            comms.localMessage(EHECATL::MSG_COMMANDS::JOYSTICK_ANGLES_LOCALE, (uint8_t *) angles, 4 * 4);
            time_since_landing = HAL_GetTick();
            EHECATL::write_motor_speeds(motor_speeds);
        }
    }

    void update(EHECATL::Barometer &barometer, EHECATL::MPU_GYRO &mpu, EHECATL::telementry &telem) {
        if (stateController.getState() == EHECATL::DRONE_MODES::SLEEP) {

        }
        if (stateController.getState() == EHECATL::DRONE_MODES::SETUP) {

            HAL_Delay(200);
            //barometer.setBaseHeight();
            mpu.setOffsets();

            motor_speeds[0] = 1050;
            motor_speeds[1] = 1050;
            motor_speeds[2] = 1050;
            motor_speeds[3] = 1050;
            EHECATL::write_motor_speeds(motor_speeds);
            stateController.setState(EHECATL::DRONE_MODES::FLYING);
        }
        if (stateController.getState() == EHECATL::DRONE_MODES::FLYING) {

            //get the base speed of the motors
            //add on the change values from the mpu.
            mpu.update(telem);
            barometer.update(telem);


            motors.getChange(change);
            uint16_t write_speeds[4];
            for (int i = 0; i < 4; i++) {

                write_speeds[i] = motors.motor_speeds[i] + change[i];
                if (write_speeds[i] < 1000) {
                    write_speeds[i] = 1000;
                }
                if (write_speeds[i] > max_value) {
                    write_speeds[i] = max_value;
                }
            }
            telem.setRpmData(write_speeds);
            char text_buffer[100];
            sprintf((char *) text_buffer, "1: %4d, 2: %4d, 3: %4d, 4: %4d\n", write_speeds[0], write_speeds[1], write_speeds[2], write_speeds[3]);
            HAL_UART_Transmit(&huart1, (char *) text_buffer, strlen((char *) text_buffer), 100);
            EHECATL::write_motor_speeds(write_speeds);

        } else if (stateController.getState() == EHECATL::DRONE_MODES::LANDING) {
            //mpu.update();
            barometer.update(telem);
            if (HAL_GetTick() - time_since_landing > 3000) {
                if (drone_speed < 0.3 && drone_speed > -0.3) {
                    stateController.setState(EHECATL::DRONE_MODES::IDLE);
                    uint16_t speeds[4] = {500, 500, 500, 500};
                    EHECATL::write_motor_speeds(speeds);
                    landing = false;
                }
            }
        }
    }
};




/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int drone_main(void) {
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

    HAL_Delay(100);
    EHECATL::motor_arm();

    uint16_t motor_speeds[4] = {};
    motor_speeds[0] = 1050;
    motor_speeds[1] = 1050;
    motor_speeds[2] = 1050;
    motor_speeds[3] = 1050;
    EHECATL::write_motor_speeds(motor_speeds);



    MX_USART1_UART_Init();
    MX_DMA_Init();
    MX_SPI1_Init();
    MX_I2C1_Init();
    MX_I2C3_Init();

    /* USER CODE BEGIN 2 */
    HAL_Delay(3000);
    uint8_t text_buffer[100];
    sprintf((char *) text_buffer, "Het apparaat is opgestart\n");
    HAL_UART_Transmit(&huart1, (char *) text_buffer, strlen((char *) text_buffer), 100);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    EHECATL::telementry teledata;
    EHECATL::communication comms(hspi1, *GPIOB, GPIO_PIN_0, *GPIOB, GPIO_PIN_1, teledata);
    EHECATL::ErrorPrinter error_printer(comms, huart1);
    EHECATL::StateController state_controller(comms);

    EHECATL::PID_Controller pid_controller(comms);
    EHECATL::Motors motors(comms, teledata);
    EHECATL::MPU_GYRO mpu(huart1, hi2c1, comms);
    EHECATL::Barometer barometer(comms);
    EHECATL::DataPrinter printer(huart1, comms);


    motorStateController controller(motors, comms, state_controller);
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

    while (_true) {

        if (HAL_GetTick() - last_update > 50) {
            //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
            uint8_t st = state_controller.getState();
            teledata.setState(st);
            last_update = HAL_GetTick();
            comms.updateAckPackage();
            //sprintf((char *) text_buffer, "testtest\n");
            //HAL_UART_Transmit(&huart1, (char *) text_buffer, strlen((char *) text_buffer), 100);

        }

        controller.update(barometer, mpu, teledata);
        comms.update(teledata, false);



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

