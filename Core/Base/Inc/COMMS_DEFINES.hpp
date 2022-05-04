//
// Created by daang on 25-2-2022.
//

#ifndef EHECATL_COMMS_DEFINES_HPP
#define EHECATL_COMMS_DEFINES_HPP

#include "stm32f4xx_hal.h"

namespace EHECATL{

    struct MSG_COMMANDS {
        static inline constexpr const uint8_t JOYSTICK_ANGLES       = 0;
        static inline constexpr const uint8_t MOTOR_VALUES          = 1;
        static inline constexpr const uint8_t DESIRED_ANGLES        = 2;
        static inline constexpr const uint8_t CURRENT_ANGLES        = 3;
        static inline constexpr const uint8_t MOTOR_DIFFERENCE      = 4;
        static inline constexpr const uint8_t NEW_BAROMETER_DATA    = 5;
        static inline constexpr const uint8_t BAROMETER_BASE_HEIGHT = 6;
        static inline constexpr const uint8_t ALTITUDE_SPEED        = 7;
        static inline constexpr const uint8_t ERROR_MESSAGE         = 8;
        static inline constexpr const uint8_t NEW_STATE             = 9;
        static inline constexpr const uint8_t DESIRED_STATE         = 10;
    };

    typedef uint8_t DRONE_MODE;
    struct DRONE_MODES {
        static inline constexpr const DRONE_MODE SLEEP          = 0;
        static inline constexpr const DRONE_MODE IDLE           = 1;
        static inline constexpr const DRONE_MODE SETUP          = 2;
        static inline constexpr const DRONE_MODE FLYING         = 3;
        static inline constexpr const DRONE_MODE LANDING        = 4;
        static inline constexpr const DRONE_MODE GOING_TO_SLEEP = 5;
    };

    struct MISC_MESSAGES {
        static inline constexpr const uint8_t BUTTON_STATE_CHANGE = 101;
    };
    
}

#endif //EHECATL_COMMS_DEFINES_HPP
