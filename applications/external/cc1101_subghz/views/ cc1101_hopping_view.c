#include "cc1101_hopping_view.h"
#include <gui/elements.h>
#include <furi.h>

#define MAX_HOPPING_FREQUENCIES 10
#define MAX_DETECTIONS 20

typedef struct {
    uint32_t frequency;
    int8_t rssi;
    char protocol[16];
    uint32_t timestamp;
} HoppingDetection;

struct CC1101HoppingView {
    View* view;
    CC1101HoppingViewCallback callback;
    void* context;
    FuriTimer* hop_timer;
};

typedef struct {
    HoppingFrequency frequencies[MAX_HOPPING_FREQUENCIES];
    size_t frequency_count;
    size_t current_frequency_index;
    bool hopping;
    uint32_t hop_interval_ms;
    uint32_t hop_count;
    HoppingDetection detections[MAX_DETECTIONS];
    size_t detection_count;
    size_t selected_detection;
    uint32_t start_time;
} CC1101HoppingViewModel;

static void hop_timer_callback(void* context) {
    CC1101HoppingView* hopping_view = context;
    
    with_view_model(
        hopping_view->view,
        CC1101HoppingViewModel * model,
        {
            if(model->hopping && model->frequency_count > 0) {
                // Move to next frequency
                model->current_frequency_index = 
                    (model->current_frequency_index + 1) % model->frequency_count;
                model->hop_count++;
                
                // Simulate signal detection (random for demo)
                if((model->hop_count % 7) == 0) {  // Occasional detection
                    uint32_t current_freq = model->frequencies[model->current_frequency_index].frequency;
                    int8_t rssi = -60 - (rand() % 40);  // Random RSSI between -60 and -100
                    
                    if(model->detection_count < MAX_DETECTIONS) {
                        model->detections[model->detection_count].frequency = current_freq;
                        model->detections[model->detection_count].rssi = rssi;
                        snprintf(
                            model->detections[model->detection_count].protocol,
                            sizeof(model->detections[model->detection_count].protocol),
                            "ASK");
                        model->detections[model->detection_count].timestamp = 
                            furi_get_tick() - model->start_time;
                        model->detection_count++;
                    }
                }
            }
        },
        true);
}

static void cc1101_hopping_view_draw_callback(Canvas* canvas, void* _model) {
    CC1101HoppingViewModel* model = _model;

    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);

    // Header
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignTop, "Frequency Hopping");

    // Current frequency and status
    canvas_set_font(canvas, FontSecondary);
    if(model->frequency_count > 0) {
        uint32_t current_freq = model->frequencies[model->current_frequency_index].frequency;
        FuriString* freq_str = furi_string_alloc();
        furi_string_printf(freq_str, "%.3f MHz (%zu/%zu)", 
            (double)current_freq / 1000000.0,
            model->current_frequency_index + 1,
            model->frequency_count);
        canvas_draw_str_aligned(canvas, 64, 20, AlignCenter, AlignTop, furi_string_get_cstr(freq_str));
        furi_string_free(freq_str);
    } else {
        canvas_draw_str_aligned(canvas, 64, 20, AlignCenter, AlignTop, "No frequencies set");
    }

    // Hopping status
    if(model->hopping) {
        FuriString* status_str = furi_string_alloc();
        furi_string_printf(status_str, "Hopping... (%lu hops)", model->hop_count);
        canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignTop, furi_string_get_cstr(status_str));
        furi_string_free(status_str);
        
        // Simple hop indicator
        uint8_t indicator = (model->hop_count % 4);
        const char* indicators[] = {"|", "/", "-", "\\"};
        canvas_draw_str_aligned(canvas, 100, 32, AlignLeft, AlignTop, indicators[indicator]);
    } else {
        canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignTop, "Press OK to start");
    }

    // Detection count
    FuriString* det_str = furi_string_alloc();
    furi_string_printf(det_str, "Detections: %zu", model->detection_count);
    canvas_draw_str(canvas, 4, 44, furi_string_get_cstr(det_str));
    furi_string_free(det_str);

    // Recent detections list
    if(model->detection_count > 0) {
        canvas_set_font(canvas, FontSecondary);
        
        size_t start_idx = 0;
        if(model->detection_count > 3) {
            start_idx = model->detection_count - 3;  // Show last 3
        }
        
        for(size_t i = start_idx; i < model->detection_count; i++) {
            uint8_t y = 54 + ((i - start_idx) * 10);
            
            // Highlight selected detection
            if(i == model->selected_detection) {
                canvas_draw_box(canvas, 2, y - 8, 124, 9);
                canvas_set_color(canvas, ColorWhite);
            }
            
            FuriString* det_line = furi_string_alloc();
            furi_string_printf(
                det_line,
                "%.2f %s %ddBm %lums",
                (double)model->detections[i].frequency / 1000000.0,
                model->detections[i].protocol,
                model->detections[i].rssi,
                model->detections[i].timestamp);
            
            canvas_draw_str(canvas, 4, y, furi_string_get_cstr(det_line));
            furi_string_free(det_line);
            
            if(i == model->selected_detection) {
                canvas_set_color(canvas, ColorBlack);
            }
        }
    }

    // Instructions
    canvas_set_font(canvas, FontSecondary);
    if(model->hopping) {
        canvas_draw_str_aligned(canvas, 64, 60, AlignCenter, AlignTop, "Back: Stop  Up/Down: Navigate");
    } else {
        canvas_draw_str_aligned(canvas, 64, 60, AlignCenter, AlignTop, "OK: Start  Back: Exit");
    }
}

static bool cc1101_hopping_view_input_callback(InputEvent* event, void* context) {
    furi_assert(context);
    CC1101HoppingView* hopping_view = context;
    bool consumed = false;

    if(event->type == InputTypeShort) {
        switch(event->key) {
        case InputKeyBack:
            with_view_model(
                hopping_view->view,
                CC1101HoppingViewModel * model,
                {
                    if(model->hopping) {
                        model->hopping = false;
                        furi_timer_stop(hopping_view->hop_timer);
                        consumed = true;
                    } else {
                        if(hopping_view->callback) {
                            hopping_view->callback(
                                CC1101SubGhzCustomEventViewHoppingBack, hopping_view->context);
                        }
                        consumed = true;
                    }
                },
                true);
            break;
        case InputKeyOk:
            with_view_model(
                hopping_view->view,
                CC1101HoppingViewModel * model,
                {
                    if(!model->hopping && model->frequency_count > 0) {
                        model->hopping = true;
                        model->hop_count = 0;
                        model->detection_count = 0;
                        model->current_frequency_index = 0;
                        model->start_time = furi_get_tick();
                        furi_timer_start(hopping_view->hop_timer, model->hop_interval_ms);
                        
                        if(hopping_view->callback) {
                            hopping_view->callback(
                                CC1101SubGhzCustomEventViewHoppingOk, hopping_view->context);
                        }
                    }
                    consumed = true;
                },
                true);
            break;
        case InputKeyUp:
            with_view_model(
                hopping_view->view,
                CC1101HoppingViewModel * model,
                {
                    if(model->detection_count > 0 && model->selected_detection > 0) {
                        model->selected_detection--;
                    }
                    consumed = true;
                },
                true);
            break;
        case InputKeyDown:
            with_view_model(
                hopping_view->view,
                CC1101HoppingViewModel * model,
                {
                    if(model->detection_count > 0 && 
                       model->selected_detection < model->detection_count - 1) {
                        model->selected_detection++;
                    }
                    consumed = true;
                },
                true);
            break;
        default:
            break;
        }
    }

    return consumed;
}

CC1101HoppingView* cc1101_hopping_view_alloc() {
    CC1101HoppingView* hopping_view = malloc(sizeof(CC1101HoppingView));

    hopping_view->view = view_alloc();
    view_allocate_model(hopping_view->view, ViewModelTypeLocking, sizeof(CC1101HoppingViewModel));
    view_set_context(hopping_view->view, hopping_view);
    view_set_draw_callback(hopping_view->view, cc1101_hopping_view_draw_callback);
    view_set_input_callback(hopping_view->view, cc1101_hopping_view_input_callback);

    // Initialize default frequencies (common SubGHz bands)
    with_view_model(
        hopping_view->view,
        CC1101HoppingViewModel * model,
        {
            // Default frequency set
            model->frequencies[0] = (HoppingFrequency){.frequency = 315000000, .dwell_time_ms = 100, .enabled = true};
            model->frequencies[1] = (HoppingFrequency){.frequency = 433920000, .dwell_time_ms = 100, .enabled = true};
            model->frequencies[2] = (HoppingFrequency){.frequency = 868350000, .dwell_time_ms = 100, .enabled = true};
            model->frequencies[3] = (HoppingFrequency){.frequency = 915000000, .dwell_time_ms = 100, .enabled = true};
            model->frequency_count = 4;
            model->current_frequency_index = 0;
            model->hopping = false;
            model->hop_interval_ms = 100;
            model->hop_count = 0;
            model->detection_count = 0;
            model->selected_detection = 0;
            model->start_time = 0;
        },
        true);

    // Create hop timer
    hopping_view->hop_timer = furi_timer_alloc(hop_timer_callback, FuriTimerTypePeriodic, hopping_view);

    return hopping_view;
}

void cc1101_hopping_view_free(CC1101HoppingView* hopping_view) {
    furi_assert(hopping_view);
    
    furi_timer_free(hopping_view->hop_timer);
    view_free(hopping_view->view);
    free(hopping_view);
}

View* cc1101_hopping_view_get_view(CC1101HoppingView* hopping_view) {
    furi_assert(hopping_view);
    return hopping_view->view;
}

void cc1101_hopping_view_set_callback(
    CC1101HoppingView* hopping_view,
    CC1101HoppingViewCallback callback,
    void* context) {
    furi_assert(hopping_view);
    hopping_view->callback = callback;
    hopping_view->context = context;
}

void cc1101_hopping_view_set_frequencies(
    CC1101HoppingView* hopping_view,
    HoppingFrequency* frequencies,
    size_t count) {
    furi_assert(hopping_view);
    furi_assert(count <= MAX_HOPPING_FREQUENCIES);
    
    with_view_model(
        hopping_view->view,
        CC1101HoppingViewModel * model,
        {
            for(size_t i = 0; i < count; i++) {
                model->frequencies[i] = frequencies[i];
            }
            model->frequency_count = count;
            model->current_frequency_index = 0;
        },
        true);
}

void cc1101_hopping_view_start_hopping(CC1101HoppingView* hopping_view) {
    furi_assert(hopping_view);
    
    with_view_model(
        hopping_view->view,
        CC1101HoppingViewModel * model,
        {
            if(model->frequency_count > 0) {
                model->hopping = true;
                model->hop_count = 0;
                model->detection_count = 0;
                model->start_time = furi_get_tick();
                furi_timer_start(hopping_view->hop_timer, model->hop_interval_ms);
            }
        },
        true);
}

void cc1101_hopping_view_stop_hopping(CC1101HoppingView* hopping_view) {
    furi_assert(hopping_view);
    
    furi_timer_stop(hopping_view->hop_timer);
    with_view_model(
        hopping_view->view,
        CC1101HoppingViewModel * model,
        { model->hopping = false; },
        true);
}

void cc1101_hopping_view_add_detection(
    CC1101HoppingView* hopping_view,
    uint32_t frequency,
    int8_t rssi,
    const char* protocol,
    uint32_t timestamp) {
    furi_assert(hopping_view);
    
    with_view_model(
        hopping_view->view,
        CC1101HoppingViewModel * model,
        {
            if(model->detection_count < MAX_DETECTIONS) {
                model->detections[model->detection_count].frequency = frequency;
                model->detections[model->detection_count].rssi = rssi;
                model->detections[model->detection_count].timestamp = timestamp;
                strncpy(
                    model->detections[model->detection_count].protocol,
                    protocol,
                    sizeof(model->detections[model->detection_count].protocol) - 1);
                model->detections[model->detection_count].protocol[15] = '\0';
                model->detection_count++;
            }
        },
        true);
}

void cc1101_hopping_view_clear_detections(CC1101HoppingView* hopping_view) {
    furi_assert(hopping_view);
    
    with_view_model(
        hopping_view->view,
        CC1101HoppingViewModel * model,
        {
            model->detection_count = 0;
            model->selected_detection = 0;
        },
        true);
}

uint32_t cc1101_hopping_view_get_current_frequency(CC1101HoppingView* hopping_view) {
    furi_assert(hopping_view);
    
    uint32_t frequency = 0;
    with_view_model(
        hopping_view->view,
        CC1101HoppingViewModel * model,
        {
            if(model->frequency_count > 0) {
                frequency = model->frequencies[model->current_frequency_index].frequency;
            }
        },
        false);
    
    return frequency;
}

void cc1101_hopping_view_set_hop_interval(CC1101HoppingView* hopping_view, uint32_t interval_ms) {
    furi_assert(hopping_view);
    
    with_view_model(
        hopping_view->view,
        CC1101HoppingViewModel * model,
        { model->hop_interval_ms = interval_ms; },
        true);
}