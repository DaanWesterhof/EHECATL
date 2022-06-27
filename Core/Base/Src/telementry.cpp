//
// Created by daang on 27-6-2022.
//

#include "telementry.h"

void EHECATL::telementry::setState(uint8_t dstate) {
    data[0] = dstate;
}

void EHECATL::telementry::setHeightData(double altitude) {
    double alt = altitude;
    memcpy(data+start_height, (uint8_t *)(&alt), sizeof(double));

}

void EHECATL::telementry::setSpeedData(double speed) {
    double spd = speed;
    memcpy(data+start_speed, (uint8_t *)&spd, sizeof(double));

}

void EHECATL::telementry::setRpmData(uint16_t *rpm) {
    memcpy(data+start_rpm, (uint8_t *)rpm, sizeof(uint16_t)*4);
}

uint8_t EHECATL::telementry::getLen() {
    return width;
}

void EHECATL::telementry::parseFromData(uint8_t *data_ptr, size_t len) {
    state = data_ptr[0];
    double height = 0;
    memcpy(&height, data_ptr+start_height, sizeof(double));
    sprintf(height_data, "%3.8f", height);
    double speed = 0;
    memcpy(&speed, data_ptr+start_speed, sizeof(double));
    sprintf(speed_data, "%3.8f",  speed);


    uint16_t * rpm = (uint16_t *) (data_ptr + start_rpm);
    sprintf(rpm_data_1, "1: %4d, 2: %4d", rpm[0], rpm[1]);
    sprintf(rpm_data_2, "3: %4d, 4: %4d", rpm[2], rpm[3]);
}
