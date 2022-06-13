//
// Created by daang on 16-5-2022.
//

#ifndef EHECATL_ST7735_LIB_HPP
#define EHECATL_ST7735_LIB_HPP

#include "stm32f4xx_hal.h"
#include "ST7735_DEFINES.hpp"
#include "screen.hpp"

#define DELAY 0x80



namespace ST7735{

    typedef enum {
        GAMMA_10 = 0x01,
        GAMMA_25 = 0x02,
        GAMMA_22 = 0x04,
        GAMMA_18 = 0x08
    } GammaDef;

    class ST7735 : public EHECATL::Base_Screen{
        uint16_t ST7735_RES_Pin;
        GPIO_TypeDef&  ST7735_RES_GPIO_Port;
        uint16_t ST7735_CS_Pin;
        GPIO_TypeDef & ST7735_CS_GPIO_Port;
        uint16_t ST7735_DC_Pin;
        GPIO_TypeDef & ST7735_DC_GPIO_Port;

        SPI_HandleTypeDef & ST7735_SPI_PORT;
    public:
        ST7735(uint16_t st7735ResPin, GPIO_TypeDef &st7735ResGpioPort,
                uint16_t st7735CsPin, GPIO_TypeDef &st7735CsGpioPort,
                uint16_t st7735DcPin, GPIO_TypeDef &st7735DcGpioPort,
                SPI_HandleTypeDef &st7735SpiPort);

    private:

        void Select();

        void Unselect();

        void ExecuteCommandList(const uint8_t *addr);

    public:

        void Reset();

        void WriteCommand(uint8_t cmd);

        void WriteData(uint8_t* buff, size_t buff_size);

        void SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

        void InvertColors(bool invert);

        void SetGamma(GammaDef gamma);

        void write_pixels(int start_x, int start_y, int len, int height, uint8_t * data) override;

        void FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
    };
}


#endif //EHECATL_ST7735_LIB_HPP
