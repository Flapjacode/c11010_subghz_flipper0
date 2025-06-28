#include "../cc1101_subghz_app_i.h"

// Common SubGHz frequencies (in Hz)
static const uint32_t hopper_frequencies[] = {
    315000000,  // 315 MHz
    433920000,  // 433.92 MHz
    868350000,  // 868.35 MHz
    915000000,  // 915 MHz
};

#define HOPPER_FREQ_COUNT (sizeof(hopper_frequencies) / sizeof(hopper_frequencies[0]))
#define HOPPER_DWELL_TIME 50  // Timer ticks to spend on each frequency

static uint32_t current_freq_index = 0;
static uint32_t dwell_counter = 0;
static uint32_t signals_found = 0;
static uint32_t total_hops = 0;

void cc1101_subghz_scene_hopper_scan_on_enter(void* context) {
    CC1101SubGhzApp* app = context;
    
    // Initialize variables
    current_freq_index = 0;
    dwell_counter = 0;
    signals_found = 0;
    total_hops = 0;
    
    // Initialize CC1101
    if(!cc1101_driver_init(app->cc1101, app->external_antenna)) {
        scene_manager_previous_scene(app->scene_manager);
        return;
    }
    
    // Set initial frequency and power
    cc1101_driver_set_frequency(app->cc1101, hopper_frequencies[current_freq_index]);
    cc1101_driver_set_power(app->cc1101, app->power_level);
    
    // Setup widget display
    widget_reset(app->widget);
    widget_add_string_element(app->widget, 64, 8, AlignCenter, AlignTop, FontPrimary, "Hopper Scan");
    widget_add_string_element(app->widget, 64, 20, AlignCenter, AlignTop, FontSecondary, "Press OK to start");
    widget_add_string_element(app->widget, 64, 32, AlignCenter, AlignTop, FontSecondary, "Scanning multiple frequencies");
    widget_add_string_element(app->widget, 64, 50, AlignCenter, AlignTop, FontSecondary, "Back to exit");
    
    view_dispatcher_switch_to_view(app->view_dispatcher, CC1101SubGhzViewWidget);
}

bool cc1101_subghz_scene_hopper_scan_on_event(void* context, SceneManagerEvent event) {
    CC1101SubGhzApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        // Handle custom events here if needed
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->is_scanning) {
            // Update RSSI and check for signals
            float rssi = cc1101_driver_get_rssi(app->cc1101);
            bool signal_detected = cc1101_driver_is_signal_detected(app->cc1101);
            
            if(signal_detected) {
                signals_found++;
                app->signals_detected++;
                notification_message(app->notifications, &sequence_single_vibro);
            }
            
            // Handle frequency hopping
            dwell_counter++;
            if(dwell_counter >= HOPPER_DWELL_TIME) {
                dwell_counter = 0;
                current_freq_index = (current_freq_index + 1) % HOPPER_FREQ_COUNT;
                total_hops++;
                
                // Switch to next frequency
                cc1101_driver_stop_rx(app->cc1101);
                cc1101_driver_set_frequency(app->cc1101, hopper_frequencies[current_freq_index]);
                cc1101_driver_start_rx(app->cc1101);
            }
            
            // Update display
            widget_reset(app->widget);
            widget_add_string_element(app->widget, 64, 8, AlignCenter, AlignTop, FontPrimary, "Hopper Scanning");
            
            furi_string_printf(app->temp_str, "Freq: %.3f MHz (%lu/%d)", 
                              (float)hopper_frequencies[current_freq_index] / 1000000.0f,
                              current_freq_index + 1, HOPPER_FREQ_COUNT);
            widget_add_string_element(app->widget, 64, 20, AlignCenter, AlignTop, FontSecondary, 
                                    furi_string_get_cstr(app->temp_str));
            
            furi_string_printf(app->temp_str, "RSSI: %.1f dBm", rssi);
            widget_add_string_element(app->widget, 64, 32, AlignCenter, AlignTop, FontSecondary, 
                                    furi_string_get_cstr(app->temp_str));
            
            furi_string_printf(app->temp_str, "Signals: %lu  Hops: %lu", signals_found, total_hops);
            widget_add_string_element(app->widget, 64, 44, AlignCenter, AlignTop, FontSecondary, 
                                    furi_string_get_cstr(app->temp_str));
            
            widget_add_string_element(app->widget, 64, 56, AlignCenter, AlignTop, FontSecondary, 
                                    "OK: Stop  Back: Exit");
        }
        consumed = true;
    }
    
    return consumed;
}

void cc1101_subghz_scene_hopper_scan_on_exit(void* context) {
    CC1101SubGhzApp* app = context;
    
    // Stop scanning if active
    if(app->is_scanning) {
        app->is_scanning = false;
        cc1101_driver_stop_rx(app->cc1101);
        notification_message(app->notifications, &sequence_blink_stop);
    }
    
    widget_reset(app->widget);
}
