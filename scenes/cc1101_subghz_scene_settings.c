#include "../cc1101_subghz_app_i.h"

typedef enum {
    CC1101SettingsIndexFrequency,
    CC1101SettingsIndexPower,
    CC1101SettingsIndexAntenna,
    CC1101SettingsIndexReset,
} CC1101SettingsIndex;

const char* const power_level_names[] = {
    "-30 dBm",
    "-20 dBm", 
    "-15 dBm",
    "-10 dBm",
    "0 dBm",
    "5 dBm",
    "7 dBm",
    "10 dBm",
};

const char* const antenna_names[] = {
    "Internal",
    "External",
};

static void cc1101_subghz_scene_settings_set_frequency(VariableItem* item) {
    CC1101SubGhzApp* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);
    
    const uint32_t frequencies[] = {315000000, 433920000, 868350000, 915000000};
    const char* freq_names[] = {"315.0 MHz", "433.92 MHz", "868.35 MHz", "915.0 MHz"};
    
    app->frequency = frequencies[index];
    variable_item_set_current_value_text(item, freq_names[index]);
}

static void cc1101_subghz_scene_settings_set_power(VariableItem* item) {
    CC1101SubGhzApp* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);
    
    app->power_level = index;
    variable_item_set_current_value_text(item, power_level_names[index]);
}

static void cc1101_subghz_scene_settings_set_antenna(VariableItem* item) {
    CC1101SubGhzApp* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);
    
    app->external_antenna = index;
    variable_item_set_current_value_text(item, antenna_names[index]);
}

static void cc1101_subghz_scene_settings_reset_callback(void* context, uint32_t index) {
    UNUSED(index);
    CC1101SubGhzApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, CC1101SettingsIndexReset);
}

void cc1101_subghz_scene_settings_on_enter(void* context) {
    CC1101SubGhzApp* app = context;
    VariableItemList* variable_item_list = app->variable_item_list;
    VariableItem* item;
    
    // Frequency setting
    item = variable_item_list_add(variable_item_list, "Frequency", 4, cc1101_subghz_scene_settings_set_frequency, app);
    uint8_t freq_index = 1; // Default to 433.92 MHz
    if(app->frequency == 315000000) freq_index = 0;
    else if(app->frequency == 433920000) freq_index = 1;
    else if(app->frequency == 868350000) freq_index = 2;
    else if(app->frequency == 915000000) freq_index = 3;
    
    const char* freq_names[] = {"315.0 MHz", "433.92 MHz", "868.35 MHz", "915.0 MHz"};
    variable_item_set_current_value_index(item, freq_index);
    variable_item_set_current_value_text(item, freq_names[freq_index]);
    
    // Power level setting
    item = variable_item_list_add(variable_item_list, "Power Level", 8, cc1101_subghz_scene_settings_set_power, app);
    variable_item_set_current_value_index(item, app->power_level);
    variable_item_set_current_value_text(item, power_level_names[app->power_level]);
    
    // Antenna setting
    item = variable_item_list_add(variable_item_list, "Antenna", 2, cc1101_subghz_scene_settings_set_antenna, app);
    variable_item_set_current_value_index(item, app->external_antenna ? 1 : 0);
    variable_item_set_current_value_text(item, antenna_names[app->external_antenna ? 1 : 0]);
    
    // Reset statistics
    variable_item_list_add(variable_item_list, "Reset Statistics", 0, NULL, NULL);
    variable_item_list_set_enter_callback(variable_item_list, cc1101_subghz_scene_settings_reset_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, CC1101SubGhzViewVariableItemList);
}

bool cc1101_subghz_scene_settings_on_event(void* context, SceneManagerEvent event) {
    CC1101SubGhzApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == CC1101SettingsIndexReset) {
            // Reset statistics
            app->signals_detected = 0;
            
            // Show confirmation popup
            popup_set_header(app->popup, "Statistics Reset");
            popup_set_text(app->popup, "All counters cleared");
            popup_set_icon(app->popup, 32, 5, &I_DolphinNice_96x59);
            popup_set_timeout(app->popup, 1500);
            popup_enable_timeout(app->popup);
            
            view_dispatcher_switch_to_view(app->view_dispatcher, CC1101SubGhzViewPopup);
            consumed = true;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        // Return to settings view after popup
        view_dispatcher_switch_to_view(app->view_dispatcher, CC1101SubGhzViewVariableItemList);
        consumed = true;
    }
    
    return consumed;
}

void cc1101_subghz_scene_settings_on_exit(void* context) {
    CC1101SubGhzApp* app = context;
    variable_item_list_reset(app->variable_item_list);
    popup_reset(app->popup);
}
