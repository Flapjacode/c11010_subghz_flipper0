#pragma once

#include <gui/scene_manager.h>

// Generate scene id and total number
#define ADD_SCENE(prefix, name, id) CC1101SubGhzScene##id,
typedef enum {
#include "cc1101_subghz_scene_config.h"
    CC1101SubGhzSceneNum,
} CC1101SubGhzScene;
#undef ADD_SCENE

extern const SceneManagerHandlers cc1101_subghz_scene_handlers;

// Generate scene on_enter handlers declaration
#define ADD_SCENE(prefix, name, id) void prefix##_scene_##name##_on_enter(void*);
#include "cc1101_subghz_scene_config.h"
#undef ADD_SCENE

// Generate scene on_event handlers declaration  
#define ADD_SCENE(prefix, name, id) \
    bool prefix##_scene_##name##_on_event(void* context, SceneManagerEvent event);
#include "cc1101_subghz_scene_config.h"
#undef ADD_SCENE

// Generate scene on_exit handlers declaration
#define ADD_SCENE(prefix, name, id) void prefix##_scene_##name##_on_exit(void* context);
#include "cc1101_subghz_scene_config.h"
#undef ADD_SCENE
