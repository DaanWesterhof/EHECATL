/*
 * dshot.h
 *
 *  Created on: 2021. 1. 27.
 *      Author: mokhwasomssi
 */


#ifndef __DSHOT_H__
#define __DSHOT_H__


#include "stm32f4xx_hal.h"    	// header from stm32cubemx code generate
#include <stdbool.h>	
#include <math.h>		// lrintf


/* User Configuration */
// Timer Clock
#define TIMER_CLOCK				100000000	// 100MHz


//#define MOTOR_1_TIM             (&htim5)
//#define MOTOR_1_TIM_CHANNEL     TIM_CHANNEL_4

// MOTOR 2 (PA2) - TIM2 Channel 3, DMA1 Stream 1
//#define MOTOR_2_TIM             (&htim2)
//#define MOTOR_2_TIM_CHANNEL     TIM_CHANNEL_3

// MOTOR 3 (PA0) - TIM2 Channel 1, DMA1 Stream 5
//#define MOTOR_3_TIM             (&htim2)
//#define MOTOR_3_TIM_CHANNEL     TIM_CHANNEL_1

// MOTOR 4 (PA1) - TIM5 Channel 2, DMA1 Stream 4
//#define MOTOR_4_TIM             (&htim5)
//#define MOTOR_4_TIM_CHANNEL     TIM_CHANNEL_2

// MOTOR 1 (PA3) - TIM5 Channel 4, DMA1 Stream 3
static TIM_HandleTypeDef * MOTOR_1_TIM;
static TIM_HandleTypeDef * MOTOR_2_TIM;
static TIM_HandleTypeDef * MOTOR_3_TIM;
static TIM_HandleTypeDef * MOTOR_4_TIM;

static uint32_t MOTOR_1_TIM_CHANNEL;
static uint32_t MOTOR_2_TIM_CHANNEL;
static uint32_t MOTOR_3_TIM_CHANNEL;
static uint32_t MOTOR_4_TIM_CHANNEL;


/* Definition */
#define MHZ_TO_HZ(x) 			((x) * 1000000)

#define DSHOT600_HZ     		MHZ_TO_HZ(12)
#define DSHOT300_HZ     		MHZ_TO_HZ(6)
#define DSHOT150_HZ     		MHZ_TO_HZ(3)

#define MOTOR_BIT_0            	7
#define MOTOR_BIT_1            	14
#define MOTOR_BITLENGTH        	20

#define DSHOT_FRAME_SIZE       	16
#define DSHOT_DMA_BUFFER_SIZE   18 /* resolution + frame reset (2us) */

#define DSHOT_MIN_THROTTLE      48
#define DSHOT_MAX_THROTTLE     	2047
#define DSHOT_RANGE 			(DSHOT_MAX_THROTTLE - DSHOT_MIN_THROTTLE)


/* Enumeration */
typedef enum
{
    DSHOT150,
    DSHOT300,
    DSHOT600
} dshot_type_e;


/* Functions */
void dshot_setTimers(TIM_HandleTypeDef * tim1, uint32_t tim_1_channel ,TIM_HandleTypeDef * tim2, uint32_t tim_2_channel  ,
                    TIM_HandleTypeDef * tim3, uint32_t tim_3_channel ,TIM_HandleTypeDef * tim4,uint32_t tim_4_channel );
void dshot_init(dshot_type_e dshot_type);
void dshot_write(uint16_t* motor_value);


#endif /* __DSHOT_H__ */