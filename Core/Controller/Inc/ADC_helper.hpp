//
// Created by daang on 10-2-2022.
//

#ifndef EHECATL_ADC_HELPER_HPP
#define EHECATL_ADC_HELPER_HPP

#include "stm32f4xx_hal.h"

/**
 * Function used by read_adc_channel to changed the adc channel
 * @param hadc The HAL adc handle
 * @param channel the channel wich you want to read
 * @param sampling_time the duraion of the sammpling time
 */
void Add_ADC_Channel(ADC_HandleTypeDef* hadc, uint32_t channel, uint32_t sampling_time);

/**
 * Read the Analof value of an adc channel
 * @param hadc The HAL handle of the adc
 * @param channel The channel wich you want to read
 * @param sampling_time the duration of the sammpling
 * @return the value read by the adc
 */
uint32_t Read_ADC_Channel(ADC_HandleTypeDef* hadc, uint32_t channel, uint32_t sampling_time);

#endif //EHECATL_ADC_HELPER_HPP
