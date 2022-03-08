//
// Created by daang on 25-2-2022.
//

#ifndef EHECATL_COMMS_DEFINES_HPP
#define EHECATL_COMMS_DEFINES_HPP

#include "stm32f4xx_hal.h"

namespace EHECATL{

    struct MSG_COMMANDS {
        static inline constexpr const uint8_t JOYSTICK_ANGLES = 0;
        static inline constexpr const uint8_t MOTOR_VALUES = 0;
        static inline constexpr const uint8_t DESIRED_ANGLES = 0;
        static inline constexpr const uint8_t CURRENT_ANGLES = 0;
        static inline constexpr const uint8_t MOTOR_DIFFERENCE = 0;

    };
//
//    static inline constexpr const uint8_t JOYSTICK_ANGLES = 0;
//    static inline constexpr const uint8_t JOYSTICK_ANGLES = 0;
//    static inline constexpr const uint8_t JOYSTICK_ANGLES = 0;
//    static inline constexpr const uint8_t JOYSTICK_ANGLES = 0;
//    static inline constexpr const uint8_t JOYSTICK_ANGLES = 0;
//    static inline constexpr const uint8_t JOYSTICK_ANGLES = 0;
//    static inline constexpr const uint8_t JOYSTICK_ANGLES = 0;
    
    
    
}

#endif //EHECATL_COMMS_DEFINES_HPP
