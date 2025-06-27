#include "../cc1101_subghz_app_i.h"

typedef enum {
    SubmenuIndexTransmitContinuous,
    SubmenuIndexTransmitBurst,
    SubmenuIndexTransmitPattern,
} TransmitSubmenuIndex;

void cc1101_subghz_scene_transmit_submenu_callback(void* context, uint32_t index) {
    CC1101SubGhzApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void cc1101_subghz_scene_transmit_on_enter(void* context) {
    CC1101SubGhzApp* app = context;
    Submenu* submenu = app->submenu;
    
    submenu_add_item(submenu, "Continuous TX", SubmenuIndexTransmitContinuous, 
                     cc1101_subghz_scene_transmit_submenu_callback, app);
    submenu_add_item(submenu, "Burst TX", SubmenuIndexTransmitBurst, 
                     cc1101_subghz_scene_transmit_submenu_callback, app);
    submenu_add_item(submenu, "Pattern TX", SubmenuIndexTransmitPattern, 
                     cc1101_subghz_scene_transmit_submenu_callback, app);
    
    submenu_set_selected_item(submenu, 
        scene_manager_get_scene_state(app->scene_manager, CC1101SubGhzSceneTransmit));
    
    view_dispatcher_switch_to_view(app->view_dispatcher, CC1101SubGhzViewSubmenu);
}

bool cc1101_subghz_scene_transmit_on_event(void* context, SceneManagerEvent event) {
    CC1101SubGhzApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, CC1101SubGhzSceneTransmit, event.event);
        
        if(event.event == SubmenuIndexTransmitContinuous) {
            // Initialize CC1101 for transmission
            if(!cc1101_driver_init(app->cc1101, app->external_antenna)) {
                popup_set_header(app->popup, "Error");
                popup_set_text(app->popup, "CC1101 Init Failed");
                popup_set_icon(app->popup, 32, 5, &I_DolphinCommon_56x48);
                popup_set_timeout(app->popup, 2000);
                popup_enable_timeout(app->popup);
                view_dispatcher_switch_to_view(app->view_dispatcher, CC1101SubGhzViewPopup);
                consumed = true;
                return consumed;
            }
            
            // Set frequency and power
            cc1101_driver_set_frequency(app->cc1101, app->frequency);
            cc1101_driver_set_power(app->cc1101, app->power_level);
            
            // Show continuous TX interface
            widget_reset(app->widget);
            widget_add_string_element(app->widget, 64, 8, AlignCenter, AlignTop, FontPrimary, "Continuous TX");
            
            furi_string_printf(app->temp_str, "Freq: %.3f MHz", (float)app->frequency / 1000000.0f);
            widget_add_string_element(app->widget, 64, 20, AlignCenter, AlignTop, FontSecondary, 
                                    furi_string_get_cstr(app->temp_str));
            
            furi_string_printf(app->temp_str, "Power: %d", app->power_level);
            widget_add_string_element(app->widget, 64, 32, AlignCenter, AlignTop, FontSecondary, 
                                    furi_string_get_cstr(app->temp_str));
            
            widget_add_string_element(app->widget, 64, 44, AlignCenter, AlignTop, FontSecondary, 
                                    "Hold OK to transmit");
            widget_add_string_element(app->widget, 64, 56, AlignCenter, AlignTop, FontSecondary, 
                                    "Back to exit");
            
            view_dispatcher_switch_to_view(app->view_dispatcher, CC1101SubGhzViewWidget);
            consumed = true;
            
        } else if(event.event == SubmenuIndexTransmitBurst) {
            // Burst transmission mode
            if(!cc1101_driver_init(app->cc1101, app->external_antenna)) {
                popup_set_header(app->popup, "Error");
                popup_set_text(app->popup, "CC1101 Init Failed");
                popup_set_icon(app->popup, 32, 5, &I_DolphinCommon_56x48);
                popup_set_timeout(app->popup, 2000);
                popup_enable_timeout(app->popup);
                view_dispatcher_switch_to_view(app->view_dispatcher, CC1101SubGhzViewPopup);
                consumed = true;
                return consumed;
            }
            
            cc1101_driver_set_frequency(app->cc1101, app->frequency);
            cc1101_driver_set_power(app->cc1101, app->power_level);
            
            widget_reset(app->widget);
            widget_add_string_element(app->widget, 64, 8, AlignCenter, AlignTop, FontPrimary, "Burst TX");
            
            furi_string_printf(app->temp_str, "Freq: %.3f MHz", (float)app->frequency / 1000000.0f);
            widget_add_string_element(app->widget, 64, 20, AlignCenter, AlignTop, FontSecondary, 
                                    furi_string_get_cstr(app->temp_str));
            
            widget_add_string_element(app->widget, 64, 32, AlignCenter, AlignTop, FontSecondary, 
                                    "Press OK for 1s burst");
            widget_add_string_element(app->widget, 64, 44, AlignCenter, AlignTop, FontSecondary, 
                                    "Up/Down: Adjust timing");
            widget_add_string_element(app->widget, 64, 56, AlignCenter, AlignTop, FontSecondary, 
                                    "Back to exit");
            
            view_dispatcher_switch_to_view(app->view_dispatcher, CC1101SubGhzViewWidget);
            consumed = true;
            
        } else if(event.event == SubmenuIndexTransmitPattern) {
            // Pattern transmission mode
            widget_reset(app->widget);
            widget_add_string_element(app->widget, 64, 8, AlignCenter, AlignTop, FontPrimary, "Pattern TX");
            widget_add_string_element(app->widget, 64, 20, AlignCenter, AlignTop, FontSecondary, 
                                    "Not implemented yet");
            widget_add_string_element(app->widget, 64, 32, AlignCenter, AlignTop, FontSecondary, 
                                    "Coming soon...");
            widget_add_string_element(app->widget, 64, 56, AlignCenter, AlignTop, FontSecondary, 
                                    "Back to return");
            
            view_dispatcher_switch_to_view(app->view_dispatcher, CC1101SubGhzViewWidget);
            consumed = true;
        }
    }
    
    return consumed;
}

void cc1101_subghz_scene_transmit_on_exit(void* context) {
    CC1101SubGhzApp* app = context;
    
    // Stop any ongoing transmission
    cc1101_driver_stop_tx(app->cc1101);
    
    submenu_reset(app->submenu);
    widget_reset(app->widget);
    popup_reset(app->popup);
}
