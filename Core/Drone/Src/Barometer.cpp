//
// Created by daang on 9-5-2022.
//

#include "Barometer.hpp"
#include <cmath>

BMP3_INTF_RET_TYPE bmp3_interface_init(struct bmp3_dev *bmp3, uint8_t intf) {
    int8_t rslt = BMP3_OK;

    if (bmp3 != NULL) {
        /* Bus configuration : I2C */
        if (intf == BMP3_I2C_INTF) {
            dev_addr = BMP3_ADDR_I2C_SEC;
            bmp3->read = &SensorAPI_I2Cx_Read;
            bmp3->write = &SensorAPI_I2Cx_Write;
            bmp3->intf = BMP3_I2C_INTF;
        }
            /* Bus configuration : SPI */
        else if (intf == BMP3_SPI_INTF) {
            dev_addr = 0;
//            bmp3->read = SensorAPI_SPIx_Read;
//            bmp3->write = SensorAPI_SPIx_Write;
            bmp3->intf = BMP3_SPI_INTF;
        }

        bmp3->delay_us = &bmp3_delay_us;
        bmp3->intf_ptr = &dev_addr;
    } else {
        rslt = BMP3_E_NULL_PTR;
    }

    return rslt;
}

EHECATL::Barometer::Barometer(EHECATL::communication &comms) : comms(comms) {
    /* Interface reference is given as a parameter
     *         For I2C : BMP3_I2C_INTF
     *         For SPI : BMP3_SPI_INTF
     */

    rslt = bmp3_interface_init(&dev, BMP3_I2C_INTF);
    if (rslt != BMP3_OK) {
        char msg[100];
        sprintf(msg, "bad interface init\n");
        comms.localMessage(MSG_COMMANDS::ERROR_MESSAGE, (uint8_t *) msg, strlen(msg));
    }

    bmp3_soft_reset(&dev);

    rslt = bmp3_init(&dev);
    if (rslt != BMP3_OK) {
        char msg[100];
        sprintf(msg, "bad bmp init: %i \n", rslt);
        comms.localMessage(MSG_COMMANDS::ERROR_MESSAGE, (uint8_t *) msg, strlen(msg));
        while (1) {};
    }

    settings.int_settings.drdy_en = BMP3_ENABLE;
    settings.press_en = BMP3_ENABLE;
    settings.temp_en = BMP3_ENABLE;

    settings.odr_filter.press_os = BMP3_OVERSAMPLING_2X;
    settings.odr_filter.temp_os = BMP3_OVERSAMPLING_2X;
    settings.odr_filter.odr = BMP3_ODR_100_HZ;

    settings_sel = BMP3_SEL_PRESS_EN | BMP3_SEL_TEMP_EN | BMP3_SEL_PRESS_OS | BMP3_SEL_TEMP_OS | BMP3_SEL_ODR |
                   BMP3_SEL_DRDY_EN;

    rslt = bmp3_set_sensor_settings(settings_sel, &settings, &dev);
    settings.op_mode = BMP3_MODE_NORMAL;
    rslt = bmp3_set_op_mode(&settings, &dev);
}

void EHECATL::Barometer::setBaseHeight() {
    double base_height = 0;
    for (int i = 0; i < 10; i++) {
        rslt = bmp3_get_status(&status, &dev);
        /* Read temperature and pressure data iteratively based on data ready interrupt */
        if ((rslt == BMP3_OK) && (status.intr.drdy == BMP3_ENABLE)) {
            rslt = bmp3_get_sensor_data(BMP3_PRESS, &data, &dev);

            /* NOTE : Read status register again to clear data ready interrupt status */
            rslt = bmp3_get_status(&status, &dev);
            base_height += data.pressure;
        }
    }
    relative_pressure = base_height / 10.0 + 10000;
    comms.localMessage(MSG_COMMANDS::BAROMETER_BASE_HEIGHT, (uint8_t *) &base_height, 16);
}

double EHECATL::Barometer::pressureToAltitude(double pressure) {
    return 44330.0 * (1.0 - std::pow(pressure / 101325, 0.1903));
}

void EHECATL::Barometer::update(telementry &telem) {
    rslt = bmp3_get_status(&status, &dev);
    /* Read temperature and pressure data iteratively based on data ready interrupt */

    if ((rslt == BMP3_OK) && (status.intr.drdy == BMP3_ENABLE)) {
        /*
         * First parameter indicates the type of data to be read
         * BMP3_PRESS_TEMP : To read pressure and temperature data
         * BMP3_TEMP       : To read only temperature data
         * BMP3_PRESS      : To read only pressure data
         */
        rslt = bmp3_get_sensor_data(BMP3_PRESS, &data, &dev);

        /* NOTE : Read status register again to clear data ready interrupt status */
        rslt = bmp3_get_status(&status, &dev);
        b_data[count] = data.pressure;

        //comms.localMessage(MSG_COMMANDS::NEW_BAROMETER_DATA, (uint8_t *)b_data, 16);
    }
    count++;
    if (count == 15) {
        sending = true;
        count = 0;
    }
    if (sending) {
        sum = 0;
        for (double i: b_data) {
            sum += i;
        }
        sum = sum / 15.0;
        last_altitude = current_altitude;
        current_altitude = pressureToAltitude(sum);

        speed = ((current_altitude - last_altitude) * 1.0f) / ((HAL_GetTick() - last_ticks) * 1.0f) * 1000.0f;
        speed_list[speed_counter] = speed;
        speed_counter++;
        if (speed_counter == 15) {
            send_speed = true;
            speed_counter = 0;
        }


        last_ticks = HAL_GetTick();

        comms.localMessage(MSG_COMMANDS::NEW_BAROMETER_DATA, (uint8_t *) &sum, 8);

        telem.setHeightData( current_altitude);
        if (send_speed) {
            for (double i: speed_list) {
                speed += i;
            }
            speed = speed / 50.0;
            comms.localMessage(MSG_COMMANDS::ALTITUDE_SPEED, (uint8_t *) &speed, 8);

            telem.setSpeedData(speed);
        }
    }
}
