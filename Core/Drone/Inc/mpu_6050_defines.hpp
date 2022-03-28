//
// Created by daang on 16-2-2022.
//

#ifndef EHECATL_MPU_6050_DEFINES_HPP
#define EHECATL_MPU_6050_DEFINES_HPP

#include "stm32f4xx_hal.h"

namespace MPU6050 {

    struct SELF_TEST_REGISTERS {
        static inline constexpr const uint32_t SELF_TEST_X = 0x0D;
        static inline constexpr const uint32_t SELF_TEST_Y = 0x0E;
        static inline constexpr const uint32_t SELF_TEST_Z = 0x0F;
        static inline constexpr const uint32_t SELF_TEST_A = 0x10;
    };

    struct CONFIG_1_REGISTERS {
        static inline constexpr const uint32_t SMPLRT_DIV = 0x19;
        static inline constexpr const uint32_t CONFIG = 0x1A;
        static inline constexpr const uint32_t GYRO_CONFIG = 0x1B;
        static inline constexpr const uint32_t ACCEL_CONFIG = 0x1C;
        static inline constexpr const uint32_t FIFO_EN = 0x23;
    };

    struct I2C_CONFIG_REGISTERS {
        static inline constexpr const uint32_t I2C_MST_CTRL = 0x24;
        static inline constexpr const uint32_t I2C_SLV0_ADDR = 0x25;
        static inline constexpr const uint32_t I2C_SLV0_REG = 0x26;
        static inline constexpr const uint32_t I2C_SLV0_CTRL = 0x27;
        static inline constexpr const uint32_t I2C_SLV1_ADDR = 0x28;
        static inline constexpr const uint32_t I2C_SLV1_REG = 0x29;
        static inline constexpr const uint32_t I2C_SLV1_CTRL = 0x2A;
        static inline constexpr const uint32_t I2C_SLV2_ADDR = 0x2B;
        static inline constexpr const uint32_t I2C_SLV2_REG = 0x2C;
        static inline constexpr const uint32_t I2C_SLV2_CTRL = 0x2D;
        static inline constexpr const uint32_t I2C_SLV3_ADDR = 0x2E;
        static inline constexpr const uint32_t I2C_SLV3_REG = 0x2F;
        static inline constexpr const uint32_t I2C_SLV3_CTRL = 0x30;
        static inline constexpr const uint32_t I2C_SLV4_ADDR = 0x31;
        static inline constexpr const uint32_t I2C_SLV4_REG = 0x32;
        static inline constexpr const uint32_t I2C_SLV4_CTRL = 0x33;
        static inline constexpr const uint32_t I2C_SLV4_DO = 0x34;
        static inline constexpr const uint32_t I2C_SLV4_DI = 0x35;
        static inline constexpr const uint32_t I2C_MST_STATUS = 0x36;
        static inline constexpr const uint32_t INT_PIN_CFG = 0x37;
        static inline constexpr const uint32_t INT_ENABLE = 0x38;
        static inline constexpr const uint32_t INT_STATUs = 0x3A;
    };

    struct DATA_OUT_REGISTERS {
        static inline constexpr const uint32_t ACCEL_XOUT_H = 0x3B;
        static inline constexpr const uint32_t ACCEL_XOUT_L = 0x3C;
        static inline constexpr const uint32_t ACCEL_YOUT_H = 0x3D;
        static inline constexpr const uint32_t ACCEL_YOUT_L = 0x3E;
        static inline constexpr const uint32_t ACCEL_ZOUT_H = 0x3F;
        static inline constexpr const uint32_t ACCEL_ZOUT_L = 0x40;
        static inline constexpr const uint32_t TEMP_OUT_H = 0x41;
        static inline constexpr const uint32_t TEMP_OUT_L = 0x42;
        static inline constexpr const uint32_t GYRO_XOUT_H = 0x43;
        static inline constexpr const uint32_t GYRO_XOUT_L = 0x44;
        static inline constexpr const uint32_t GYRO_YOUT_H = 0x45;
        static inline constexpr const uint32_t GYRO_YOUT_L = 0x46;
        static inline constexpr const uint32_t GYRO_ZOUT_H = 0x47;
        static inline constexpr const uint32_t GYRO_ZOUT_L = 0x48;
    };

    struct EXERNAL_SENSORS_REGISTERS {
        static inline constexpr const uint32_t EXT_SENS_DATA_00 = 0x49;
        static inline constexpr const uint32_t EXT_SENS_DATA_01 = 0x4A;
        static inline constexpr const uint32_t EXT_SENS_DATA_02 = 0x4B;
        static inline constexpr const uint32_t EXT_SENS_DATA_03 = 0x4C;
        static inline constexpr const uint32_t EXT_SENS_DATA_04 = 0x4D;
        static inline constexpr const uint32_t EXT_SENS_DATA_05 = 0x4E;
        static inline constexpr const uint32_t EXT_SENS_DATA_06 = 0x4F;
        static inline constexpr const uint32_t EXT_SENS_DATA_07 = 0x50;
        static inline constexpr const uint32_t EXT_SENS_DATA_08 = 0x51;
        static inline constexpr const uint32_t EXT_SENS_DATA_09 = 0x52;
        static inline constexpr const uint32_t EXT_SENS_DATA_10 = 0x53;
        static inline constexpr const uint32_t EXT_SENS_DATA_11 = 0x54;
        static inline constexpr const uint32_t EXT_SENS_DATA_12 = 0x55;
        static inline constexpr const uint32_t EXT_SENS_DATA_13 = 0x56;
        static inline constexpr const uint32_t EXT_SENS_DATA_14 = 0x57;
        static inline constexpr const uint32_t EXT_SENS_DATA_15 = 0x58;
        static inline constexpr const uint32_t EXT_SENS_DATA_16 = 0x59;
        static inline constexpr const uint32_t EXT_SENS_DATA_17 = 0x5A;
        static inline constexpr const uint32_t EXT_SENS_DATA_18 = 0x5B;
        static inline constexpr const uint32_t EXT_SENS_DATA_19 = 0x5C;
        static inline constexpr const uint32_t EXT_SENS_DATA_20 = 0x5D;
        static inline constexpr const uint32_t EXT_SENS_DATA_21 = 0x5E;
        static inline constexpr const uint32_t EXT_SENS_DATA_22 = 0x5F;
        static inline constexpr const uint32_t EXT_SENS_DATA_23 = 0x60;
    };

    struct I2C_SLAVE_REGISTERS {
        static inline constexpr const uint32_t I2C_SLV0_DO = 0x63;
        static inline constexpr const uint32_t I2C_SLV1_DO = 0x64;
        static inline constexpr const uint32_t I2C_SLV2_DO = 0x65;
        static inline constexpr const uint32_t I2C_SLV3_DO = 0x66;
    };


    struct CONFIG_2_REGISTERS {
        static inline constexpr const uint32_t I2C_MST_DELAY_CTRL = 0x67;
        static inline constexpr const uint32_t SIGNAL_PATH_RESET = 0x68;
        static inline constexpr const uint32_t USER_CTRL = 0x6A;
        static inline constexpr const uint32_t PWR_MGMT_1 = 0x6B;
        static inline constexpr const uint32_t PWR_MGMT_2 = 0x6C;
        static inline constexpr const uint32_t FIFO_COUNTH = 0x72;
        static inline constexpr const uint32_t FIFO_COUNTL = 0x73;
        static inline constexpr const uint32_t FIFO_R_W = 0x74;
        static inline constexpr const uint32_t WHO_AM_I = 0x75;

        static inline constexpr const uint32_t CHIP_ID = 0x68;
    };
}

#endif //EHECATL_MPU_6050_DEFINES_HPP
