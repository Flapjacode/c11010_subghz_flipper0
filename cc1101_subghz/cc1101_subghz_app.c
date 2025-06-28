#include "cc1101_subghz_app_i.h"

static bool cc1101_subghz_app_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    CC1101SubGhzApp* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

static bool cc1101_subghz_app_back_event_callback(void* context) {
    furi_assert(context);
    CC1101SubGhzApp* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

static void cc1101_subghz_app_tick_event_callback(void* context) {
    furi_assert(context);
    CC1101SubGhzApp* app = context;
    scene_manager_handle_tick_event(app->scene_manager);
}

CC1101SubGhzApp* cc1101_subghz_app_alloc() {
    CC1101SubGhzApp* app = malloc(sizeof(CC1101SubGhzApp));

    // Initialize default values
    app->frequency = 433920000; // 433.92 MHz
    app->power_level = 7;
    app->external_antenna = false;
    app->is_scanning = false;
    app->signals_detected = 0;
    
    // Allocate strings
    app->temp_str = furi_string_alloc();
    app->file_path = furi_string_alloc();
    
    // GUI
    app->gui = furi_record_open(RECORD_GUI);
    app->view_dispatcher = view_dispatcher_alloc();
    app->scene_manager = scene_manager_alloc(&cc1101_subghz_scene_handlers, app);
    
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, cc1101_subghz_app_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, cc1101_subghz_app_back_event_callback);
    view_dispatcher_set_tick_event_callback(app->view_dispatcher, cc1101_subghz_app_tick_event_callback, 100);
    
    // Views
    app->submenu = submenu_alloc();
    view_dispatcher_add_view(app->view_dispatcher, CC1101SubGhzViewSubmenu, submenu_get_view(app->submenu));
    
    app->text_input = text_input_alloc();
    view_dispatcher_add_view(app->view_dispatcher, CC1101SubGhzViewTextInput, text_input_get_view(app->text_input));
    
    app->popup = popup_alloc();
    view_dispatcher_add_view(app->view_dispatcher, CC1101SubGhzViewPopup, popup_get_view(app->popup));
    
    app->variable_item_list = variable_item_list_alloc();
    view_dispatcher_add_view(app->view_dispatcher, CC1101SubGhzViewVariableItemList, variable_item_list_get_view(app->variable_item_list));
    
    app->widget = widget_alloc();
    view_dispatcher_add_view(app->view_dispatcher, CC1101SubGhzViewWidget, widget_get_view(app->widget));
    
    // Custom scanning view
    app->scanning_view = cc1101_scanning_view_alloc();
    view_dispatcher_add_view(app->view_dispatcher, CC1101SubGhzViewScanning, cc1101_scanning_view_get_view(app->scanning_view));
    
    // Storage and notifications
    app->storage = furi_record_open(RECORD_STORAGE);
    app->notifications = furi_record_open(RECORD_NOTIFICATION);
    
    // Initialize CC1101 driver
    app->cc1101 = cc1101_driver_alloc();
    
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);
    scene_manager_next_scene(app->scene_manager, CC1101SubGhzSceneStart);
    
    return app;
}

void cc1101_subghz_app_free(CC1101SubGhzApp* app) {
    furi_assert(app);
    
    // Stop any ongoing operations
    if(app->is_scanning) {
        cc1101_driver_stop_rx(app->cc1101);
    }
    
    // Free views
    view_dispatcher_remove_view(app->view_dispatcher, CC1101SubGhzViewSubmenu);
    view_dispatcher_remove_view(app->view_dispatcher, CC1101SubGhzViewTextInput);
    view_dispatcher_remove_view(app->view_dispatcher, CC1101SubGhzViewPopup);
    view_dispatcher_remove_view(app->view_dispatcher, CC1101SubGhzViewVariableItemList);
    view_dispatcher_remove_view(app->view_dispatcher, CC1101SubGhzViewWidget);
    view_dispatcher_remove_view(app->view_dispatcher, CC1101SubGhzViewScanning);
    
    submenu_free(app->submenu);
    text_input_free(app->text_input);
    popup_free(app->popup);
    variable_item_list_free(app->variable_item_list);
    widget_free(app->widget);
    cc1101_scanning_view_free(app->scanning_view);
    
    // Free scene manager and view dispatcher
    view_dispatcher_free(app->view_dispatcher);
    scene_manager_free(app->scene_manager);
    
    // Free CC1101 driver
    cc1101_driver_free(app->cc1101);
    
    // Free strings
    furi_string_free(app->temp_str);
    furi_string_free(app->file_path);
    
    // Close records
    furi_record_close(RECORD_GUI);
    furi_record_close(RECORD_STORAGE);
    furi_record_close(RECORD_NOTIFICATION);
    
    free(app);
}

int32_t cc1101_subghz_app(void* p) {
    UNUSED(p);
    
    CC1101SubGhzApp* app = cc1101_subghz_app_alloc();
    view_dispatcher_run(app->view_dispatcher);
    cc1101_subghz_app_free(app);
    
    return 0;
}
