//
// Created by daang on 30-3-2022.
//

#include "ADC_helper.hpp"

void Add_ADC_Channel(ADC_HandleTypeDef *hadc, uint32_t channel, uint32_t sampling_time) {
    ADC_ChannelConfTypeDef sConfig = {0};
    /** Configure for the selected ADC regular channel to be converted. */
    sConfig.Channel = channel;
    sConfig.Rank = 1;
    sConfig.SamplingTime = sampling_time;
    HAL_ADC_ConfigChannel(hadc, &sConfig);
}

uint32_t Read_ADC_Channel(ADC_HandleTypeDef *hadc, uint32_t channel, uint32_t sampling_time) {
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
