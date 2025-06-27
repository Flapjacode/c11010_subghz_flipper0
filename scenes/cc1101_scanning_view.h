#pragma once

#include <gui/view.h>
#include "../helpers/cc1101_subghz_custom_event.h"

typedef struct CC1101ScanningView CC1101ScanningView;

typedef void (*CC1101ScanningViewCallback)(CC1101SubGhzCustomEvent event, void* context);

CC1101ScanningView* cc1101_scanning_view_alloc();

void cc1101_scanning_view_free(CC1101ScanningView* scanning_view);

View* cc1101_scanning_view_get_view(CC1101ScanningView* scanning_view);

void cc1101_scanning_view_set_callback(
    CC1101ScanningView* scanning_view,
    CC1101ScanningViewCallback callback,
    void* context);

void cc1101_scanning_view_set_frequency(CC1101ScanningView* scanning_view, uint32_t frequency);

void cc1101_scanning_view_start_scan(CC1101ScanningView* scanning_view);

void cc1101_scanning_view_stop_scan(CC1101ScanningView* scanning_view);

void cc1101_scanning_view_add_signal(
    CC1101ScanningView* scanning_view,
    uint32_t frequency,
    int8_t rssi,
    const char* protocol);

void cc1101_scanning_view_clear_signals(CC1101ScanningView* scanning_view);
