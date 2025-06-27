#include "../cc1101_subghz_app_i.h"

void cc1101_subghz_scene_frequency_input_text_input_callback(void* context) {
    CC1101SubGhzApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, 0);
}

void cc1101_subghz_scene_frequency_input_on_enter(void* context) {
    CC1101SubGhzApp* app = context;
    TextInput* text_input = app->text_input;
    
    // Set current frequency as default text
    snprintf(app->text_buffer, sizeof(app->text_buffer), "%.3f", 
             (float)app->frequency / 1000000.0f);
    
    text_input_set_header_text(text_input, "Enter Frequency (MHz)");
    text_input_set_result_callback(
        text_input,
        cc1101_subghz_scene_frequency_input_text_input_callback,
        app,
        app->text_buffer,
        sizeof(app->text_buffer),
        true);
    
    // Validator to ensure valid frequency input
    ValidatorIsFloat* validator_context = validator_is_float_alloc(300.0f, 1000.0f, 3);
    text_input_set_validator(text_input, validator_is_float_cb, validator_context);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, CC1101SubGhzViewTextInput);
}

bool cc1101_subghz_scene_frequency_input_on_event(void* context, SceneManagerEvent event) {
    CC1101SubGhzApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == 0) {
            // Parse frequency input
            float freq_mhz = strtof(app->text_buffer, NULL);
            uint32_t freq_hz = (uint32_t)(freq_mhz * 1000000.0f);
            
            // Validate frequency range
            if(freq_hz >= 300000000 && freq_hz <= 1000000000) {
                app->frequency = freq_hz;
                
                // Show confirmation popup
                popup_set_header(app->popup, "Frequency Set");
                furi_string_printf(app->temp_str, "%.3f MHz", freq_mhz);
                popup_set_text(app->popup, furi_string_get_cstr(app->temp_str));
                popup_set_icon(app->popup, 32, 5, &I_DolphinNice_96x59);
                popup_set_timeout(app->popup, 1500);
                popup_enable_timeout(app->popup);
                
                view_dispatcher_switch_to_view(app->view_dispatcher, CC1101SubGhzViewPopup);
                
                // Auto-return to main menu after popup
                scene_manager_set_scene_state(app->scene_manager, CC1101SubGhzSceneFrequencyInput, 1);
            } else {
                // Invalid frequency - show error
                popup_set_header(app->popup, "Invalid Frequency");
                popup_set_text(app->popup, "Valid range:\n300-1000 MHz");
                popup_set_icon(app->popup, 32, 5, &I_DolphinCommon_56x48);
                popup_set_timeout(app->popup, 2000);
                popup_enable_timeout(app->popup);
                
                view_dispatcher_switch_to_view(app->view_dispatcher, CC1101SubGhzViewPopup);
                
                // Return to input after error
                scene_manager_set_scene_state(app->scene_manager, CC1101SubGhzSceneFrequencyInput, 0);
            }
            consumed = true;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        // Handle back from popup
        uint32_t state = scene_manager_get_scene_state(app->scene_manager, CC1101SubGhzSceneFrequencyInput);
        if(state == 1) {
            // Success case - return to main menu
            scene_manager_search_and_switch_to_previous_scene(app->scene_manager, CC1101SubGhzSceneStart);
            consumed = true;
        } else {
            // Error case - return to input
            view_dispatcher_switch_to_view(app->view_dispatcher, CC1101SubGhzViewTextInput);
            consumed = true;
        }
    }
    
    return consumed;
}

void cc1101_subghz_scene_frequency_input_on_exit(void* context) {
    CC1101SubGhzApp* app = context;
    
    // Clean up text input
    void* validator_context = text_input_get_validator_callback_context(app->text_input);
    text_input_set_validator(app->text_input, NULL, NULL);
    if(validator_context) {
        validator_is_float_free((ValidatorIsFloat*)validator_context);
    }
    
    text_input_reset(app->text_input);
    popup_reset(app->popup);
}
