//
// Created by daang on 25-2-2022.
//

#ifndef EHECATL_COMMS_DEFINES_HPP
#define EHECATL_COMMS_DEFINES_HPP

#include "stm32f4xx_hal.h"

namespace EHECATL{

    /**
     * struct with commands used by the communcation system between the drone and controller and internal communication
     */
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
        static inline constexpr const uint8_t STATE_UP              = 11;
        static inline constexpr const uint8_t STATE_DOWN            = 12;
        static inline constexpr const uint8_t DRONE_HEIGHT          = 13;
        static inline constexpr const uint8_t CONNECTION_STATE      = 14;
        static inline constexpr const uint8_t JOYSTICK_ANGLES_LOCALE= 15;
        static inline constexpr const uint8_t PING                  = 16;
        static inline constexpr const uint8_t DRONE_RPM             = 17;
        static inline constexpr const uint8_t BUTTON_STATE_CHANGE   = 18;
    };



    typedef uint8_t DRONE_MODE;
    /**
     * the difrent modes of the drone
     */
    struct DRONE_MODES {
        static inline constexpr const DRONE_MODE SLEEP          = 0;
        static inline constexpr const DRONE_MODE SETUP          = 1;
        static inline constexpr const DRONE_MODE IDLE           = 2;
        static inline constexpr const DRONE_MODE FLYING         = 3;
        static inline constexpr const DRONE_MODE LANDING        = 4;
    };



    /**
     * turn a given state into a string
     * @param state the state
     * @param state_s a pointer to the char array/string where you want to store the text
     */
    void state_to_string(DRONE_MODE state, char * state_s);

    /**
     *
     */
    struct MISC_MESSAGES {

    };
    
}

#endif //EHECATL_COMMS_DEFINES_HPP
