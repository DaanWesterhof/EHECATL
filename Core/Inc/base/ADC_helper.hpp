//
// Created by daang on 10-2-2022.
//

#ifndef EHECATL_ADC_HELPER_HPP
#define EHECATL_ADC_HELPER_HPP

#include "stm32f4xx_hal.h"

void Add_ADC_Channel(ADC_HandleTypeDef* hadc, uint32_t channel, uint32_t sampling_time) {
    ADC_ChannelConfTypeDef sConfig = {0};
    /** Configure for the selected ADC regular channel to be converted. */
    sConfig.Channel = channel;
    sConfig.Rank = 1;
    sConfig.SamplingTime = sampling_time;
    if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK) { Error_Handler(); }
}

uint32_t Read_ADC_Channel(ADC_HandleTypeDef* hadc, uint32_t channel, uint32_t sampling_time) {
    uint32_t adc_value = 0;
    // clear all channel
    Add_ADC_Channel(hadc, channel, sampling_time);
    // read
    HAL_ADC_Start(hadc);
    HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);
    adc_value = HAL_ADC_GetValue(hadc);
    HAL_ADC_Stop(hadc);
    // clear all channel
    // return
    return adc_value;
}

class ADC_Helper{
private:
public:

};

#endif //EHECATL_ADC_HELPER_HPP
