#pragma once

#include <furi_hal.h>

typedef struct CC1101Driver CC1101Driver;

typedef struct {
    uint32_t frequency;
    float rssi;
    uint32_t timestamp;
    uint8_t* data;
    size_t data_size;
} CC1101Signal;

CC1101Driver* cc1101_driver_alloc(void);
void cc1101_driver_free(CC1101Driver* driver);

bool cc1101_driver_init(CC1101Driver* driver, bool external_antenna);
void cc1101_driver_deinit(CC1101Driver* driver);

void cc1101_driver_set_frequency(CC1101Driver* driver, uint32_t frequency);
void cc1101_driver_set_power(CC1101Driver* driver, uint8_t power);

void cc1101_driver_start_rx(CC1101Driver* driver);
void cc1101_driver_stop_rx(CC1101Driver* driver);
void cc1101_driver_start_tx(CC1101Driver* driver);
void cc1101_driver_stop_tx(CC1101Driver* driver);

float cc1101_driver_get_rssi(CC1101Driver* driver);
bool cc1101_driver_is_signal_detected(CC1101Driver* driver);

