# Drivers
for the drone i use several drivers, some provided by the stm32cube ide and some libraries of other awesome developers i added myself

## Cube Ide Drivers
- CMSIS
- STM32F4xx_HAL_Driver

## Drivers i added myself

### BMP3-Sensor-API
This is a driver writen by bosch for the BMP3xx sensors. It is modified to properly use the stm32 hal using some code of loveley people on the stm32 forum.
read [BMP3-Sensor-API/README.md](BMP3-Sensor-API/README.md) for more information

### cpp_Nrf24l01
this is a library for the nrf24l01+ module written by Niels Post. This is a specific version for the stm32f4 hal.
I have made some minor changes to the library to enable the usage of receiving an ack package.
read [cpp_nrf24l01/README.MD](cpp_nrf24l01/README.MD) for more information about the library.

### MPU6050
This is a driver originaly written by the developers of i2cdev_lib. 
Im using this library as it has an implementation of the dmp for the mpu6050 wich is not feasable to implement myself in this time frame.
I used a fork of this library made by [Pluscrafter](https://github.com/Pluscrafter/i2cdevlib/tree/master/STM32_HAL/Nucleo-144F722ZE) for the implementation of the DMP for the stm32f4.
I but I combined the orriginal stm32hal i2cdevlib and his code to get it working, and some minor changes for the default register.

### STM32F4_HAL_ST7735
This driver is based on the code written by [afiskon](https://github.com/afiskon/stm32-st7735)
i took the most basic part of his code for the st7735 driver and modified it to be object-oriented and fit my needs.