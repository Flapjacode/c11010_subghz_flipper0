#include "../cc1101_subghz_app_i.h"
#include "../views/cc1101_scanning_view.h"

void cc1101_subghz_scene_static_scan_callback(CC1101SubGhzCustomEvent event, void* context) {
    furi_assert(context);
    CC1101SubGhzApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, event);
}

void cc1101_subghz_scene_static_scan_on_enter(void* context) {
    furi_assert(context);
    CC1101SubGhzApp* app = context;
    CC1101ScanningView* scanning_view = app->scanning_view;

    cc1101_scanning_view_set_callback(
        scanning_view, cc1101_subghz_scene_static_scan_callback, app);

    // Set default frequency (433.92 MHz)
    cc1101_scanning_view_set_frequency(scanning_view, 433920000);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, CC1101SubGhzViewScanning);
}

bool cc1101_subghz_scene_static_scan_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);
    CC1101SubGhzApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
        case CC1101SubGhzCustomEventViewScanningBack:
            scene_manager_search_and_switch_to_previous_scene(
                app->scene_manager, CC1101SubGhzSceneStart);
            consumed = true;
            break;
        case CC1101SubGhzCustomEventViewScanningOk:
            // Handle OK button press during scanning
            consumed = true;
            break;
        default:
            break;
        }
    }

    return consumed;
}

void cc1101_subghz_scene_static_scan_on_exit(void* context) {
    furi_assert(context);
    CC1101SubGhzApp* app = context;
    
    // Stop scanning when exiting
    cc1101_scanning_view_stop_scan(app->scanning_view);
}
