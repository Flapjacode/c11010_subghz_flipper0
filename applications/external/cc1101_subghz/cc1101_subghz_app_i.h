#pragma once

#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_input.h>
#include <gui/modules/popup.h>
#include <gui/modules/variable_item_list.h>
#include <gui/modules/widget.h>
#include <notification/notification_messages.h>
#include <storage/storage.h>
#include <lib/subghz/subghz_device.h>

#include "scenes/cc1101_subghz_scene.h"
#include "views/cc1101_scanning_view.h"
#include "cc1101_driver.h"

#define CC1101_SUBGHZ_APP_FOLDER "/ext/subghz"
#define CC1101_SUBGHZ_APP_EXTENSION ".sub"

typedef struct CC1101SubGhzApp {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    SceneManager* scene_manager;
    
    Submenu* submenu;
    TextInput* text_input;
    Popup* popup;
    VariableItemList* variable_item_list;
    Widget* widget;
    CC1101ScanningView* scanning_view;
    
    Storage* storage;
    NotificationApp* notifications;
    
    // App state
    uint32_t frequency;
    uint8_t power_level;
    bool external_antenna;
    bool is_scanning;
    uint32_t signals_detected;
    
    // Strings
    FuriString* temp_str;
    FuriString* file_path;
    char text_buffer[64];
    
    // CC1101 driver
    CC1101Driver* cc1101;
} CC1101SubGhzApp;

typedef enum {
    CC1101SubGhzViewSubmenu,
    CC1101SubGhzViewTextInput,
    CC1101SubGhzViewPopup,
    CC1101SubGhzViewVariableItemList,
    CC1101SubGhzViewWidget,
    CC1101SubGhzViewScanning,
} CC1101SubGhzView;
