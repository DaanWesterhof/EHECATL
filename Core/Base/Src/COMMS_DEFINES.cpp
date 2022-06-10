//
// Created by daang on 3-6-2022.
//
#include "COMMS_DEFINES.hpp"
#include <cstdio>


char state_strings[5][4]{
        "SLP",
        "SET",
        "IDL",
        "FLY",
        "LND"
};

void EHECATL::state_to_string(DRONE_MODE state, char *state_s) {
    sprintf(state_s, "%s", state_strings[state]);
}
