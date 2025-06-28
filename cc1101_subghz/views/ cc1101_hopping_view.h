#pragma once

#include <gui/view.h>
#include "../helpers/cc1101_subghz_custom_event.h"

typedef struct CC1101HoppingView CC1101HoppingView;

typedef void (*CC1101HoppingViewCallback)(CC1101SubGhzCustomEvent event, void* context);

typedef struct {
    uint32_t frequency;
    uint32_t dwell_time_ms;
    bool enabled;
} HoppingFrequency;

CC1101HoppingView* cc1101_hopping_view_alloc();

void cc1101_hopping_view_free(CC1101HoppingView* hopping_view);

View* cc1101_hopping_view_get_view(CC1101HoppingView* hopping_view);

void cc1101_hopping_view_set_callback(
    CC1101HoppingView* hopping_view,
    CC1101HoppingViewCallback callback,
    void* context);

void cc1101_hopping_view_set_frequencies(
    CC1101HoppingView* hopping_view,
    HoppingFrequency* frequencies,
    size_t count);

void cc1101_hopping_view_start_hopping(CC1101HoppingView* hopping_view);

void cc1101_hopping_view_stop_hopping(CC1101HoppingView* hopping_view);

void cc1101_hopping_view_add_detection(
    CC1101HoppingView* hopping_view,
    uint32_t frequency,
    int8_t rssi,
    const char* protocol,
    uint32_t timestamp);

void cc1101_hopping_view_clear_detections(CC1101HoppingView* hopping_view);

uint32_t cc1101_hopping_view_get_current_frequency(CC1101HoppingView* hopping_view);

void cc1101_hopping_view_set_hop_interval(CC1101HoppingView* hopping_view, uint32_t interval_ms);