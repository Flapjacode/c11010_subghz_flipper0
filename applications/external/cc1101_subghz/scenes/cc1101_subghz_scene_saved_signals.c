#include "../cc1101_subghz_app_i.h"

typedef struct {
    const char* name;
    uint32_t frequency;
    const char* data;
} SavedSignal;

// Example saved signals
static const SavedSignal saved_signals[] = {
    {"Gate Remote 1", 433920000, "AM650 -30000 1500 -1500 1500 -1500 3000"},
    {"Gate Remote 2", 315000000, "AM270 -20000 1000 -1000 1000 -1000 2000"},
    {"Car Key Fob", 433920000, "AM650 -25000 2000 -2000 1000 -1000 1500"},
    {"Doorbell", 433920000, "AM650 -30000 500 -500 500 -500 1000"},
};

static const size_t saved_signals_count = COUNT_OF(saved_signals);

void cc1101_subghz_scene_saved_signals_submenu_callback(void* context, uint32_t index) {
    furi_assert(context);
    CC1101SubGhzApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void cc1101_subghz_scene_saved_signals_on_enter(void* context) {
    furi_assert(context);
    CC1101SubGhzApp* app = context;
    Submenu* submenu = app->submenu;

    submenu_reset(submenu);
    submenu_set_header(submenu, "Saved Signals");

    for(size_t i = 0; i < saved_signals_count; i++) {
        submenu_add_item(
            submenu,
            saved_signals[i].name,
            i,
            cc1101_subghz_scene_saved_signals_submenu_callback,
            app);
    }

    submenu_set_selected_item(
        submenu, scene_manager_get_scene_state(app->scene_manager, CC1101SubGhzSceneSavedSignals));

    view_dispatcher_switch_to_view(app->view_dispatcher, CC1101SubGhzViewSubmenu);
}

bool cc1101_subghz_scene_saved_signals_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);
    CC1101SubGhzApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event < saved_signals_count) {
            scene_manager_set_scene_state(
                app->scene_manager, CC1101SubGhzSceneSavedSignals, event.event);
            
            // Store selected signal info for transmission
            app->selected_signal_name = saved_signals[event.event].name;
            app->selected_frequency = saved_signals[event.event].frequency;
            app->selected_data = saved_signals[event.event].data;
            
            scene_manager_next_scene(app->scene_manager, CC1101SubGhzSceneTransmit);
            consumed = true;
        }
    }

    return consumed;
}

void cc1101_subghz_scene_saved_signals_on_exit(void* context) {
    furi_assert(context);
    CC1101SubGhzApp* app = context;
    submenu_reset(app->submenu);
}
