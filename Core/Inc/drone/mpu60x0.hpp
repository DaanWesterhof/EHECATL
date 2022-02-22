////
//// Created by daang on 16-2-2022.
////
//
//#ifndef EHECATL_MPU60X0_HPP
//#define EHECATL_MPU60X0_HPP
//
////#include "mpu_6050_defines.hpp"
////#include "MPU6050.hpp"
//
//namespace EHECATL{
//    class MPU_60x0{
//    public:
//
//        uint16_t mpu_data[7]       = {};
//        uint8_t  external_data[24] = {};
//
//        virtual void writeRegister(uint32_t reg, uint8_t data) = 0;
//
//        virtual uint8_t reagRegister(uint32_t reg) = 0;
//
//        virtual void readMultipleRegs(uint32_t start_adress, uint8_t amount, uint8_t * target_array) = 0;
//
//        virtual void writeMultipleRegs(uint32_t start_adress, uint8_t * data, uint8_t amount) = 0;
//
//
//    };
//
//    class MPU_6050 : private MPU_60x0 {
//        I2C_HandleTypeDef & i2c_bus;
//    private:
//        void readMultipleRegs(uint32_t start_adress, uint8_t amount, uint8_t * target_array) override{
//            uint8_t adr = uint8_t(start_adress);
//            HAL_I2C_Master_Transmit(&i2c_bus, MPU6050::CONFIG_2_REGISTERS::CHIP_ID, &adr, 1, HAL_MAX_DELAY);
//            HAL_I2C_Master_Receive(&i2c_bus, MPU6050::CONFIG_2_REGISTERS::CHIP_ID, target_array, amount, HAL_MAX_DELAY);
//        }
//
//        void writeMultipleRegs(uint32_t start_adress, uint8_t * data, uint8_t amount) override{
//            uint8_t payload[100];
//            payload[0] = uint8_t(start_adress);
//            for (int i = 0; i < amount; i++){
//                payload[i+1] = data[i];
//            }
//
//            HAL_I2C_Master_Transmit(&i2c_bus, MPU6050::CONFIG_2_REGISTERS::CHIP_ID, payload, amount+1, HAL_MAX_DELAY);
//        }
//    public:
//        MPU_6050(I2C_HandleTypeDef &i2CBus) : i2c_bus(i2CBus) {}
//
//        void writeRegister(uint32_t reg, uint8_t data) override{
//
//        }
//
//        void setup(){
//            writeRegister(MPU6050::CONFIG_2_REGISTERS::PWR_MGMT_1, 0b00000000);
//        }
//
//        void update(){
//            readMultipleRegs(MPU6050::DATA_OUT_REGISTERS::ACCEL_XOUT_H, 14, (uint8_t *)mpu_data);
//        }
//
//
//    };
//}
//
//#endif //EHECATL_MPU60X0_HPP
