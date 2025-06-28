#include "cc1101_scanning_view.h"
#include <gui/elements.h>

#define MAX_DETECTED_SIGNALS 10

typedef struct {
    uint32_t frequency;
    int8_t rssi;
    char protocol[16];
} DetectedSignal;

struct CC1101ScanningView {
    View* view;
    CC1101ScanningViewCallback callback;
    void* context;
};

typedef struct {
    uint32_t frequency;
    bool scanning;
    uint32_t scan_count;
    DetectedSignal signals[MAX_DETECTED_SIGNALS];
    size_t signal_count;
    size_t selected_signal;
} CC1101ScanningViewModel;

static void cc1101_scanning_view_draw_callback(Canvas* canvas, void* _model) {
    CC1101ScanningViewModel* model = _model;

    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);

    // Header
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignTop, "SubGHz Scanner");

    // Frequency display
    canvas_set_font(canvas, FontSecondary);
    FuriString* freq_str = furi_string_alloc();
    furi_string_printf(freq_str, "Freq: %.3f MHz", (double)model->frequency / 1000000.0);
    canvas_draw_str_aligned(canvas, 64, 20, AlignCenter, AlignTop, furi_string_get_cstr(freq_str));
    furi_string_free(freq_str);

    // Scanning status
    if(model->scanning) {
        canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignTop, "Scanning...");
        
        // Simple animation dots
        uint8_t dots = (model->scan_count / 10) % 4;
        char dot_str[5] = "";
        for(uint8_t i = 0; i < dots; i++) {
            strcat(dot_str, ".");
        }
        canvas_draw_str_aligned(canvas, 90, 32, AlignLeft, AlignTop, dot_str);
    } else {
        canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignTop, "Press OK to scan");
    }

    // Detected signals
    if(model->signal_count > 0) {
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 4, 44, "Detected:");
        
        uint8_t start_y = 54;
        for(size_t i = 0; i < model->signal_count && i < 4; i++) {
            uint8_t y = start_y + (i * 10);
            
            // Highlight selected signal
            if(i == model->selected_signal) {
                canvas_draw_box(canvas, 2, y - 8, 124, 9);
                canvas_set_color(canvas, ColorWhite);
            }
            
            FuriString* signal_str = furi_string_alloc();
            furi_string_printf(
                signal_str,
                "%.2fMHz %s %ddBm",
                (double)model->signals[i].frequency / 1000000.0,
                model->signals[i].protocol,
                model->signals[i].rssi);
            
            canvas_draw_str(canvas, 4, y, furi_string_get_cstr(signal_str));
            furi_string_free(signal_str);
            
            if(i == model->selected_signal) {
                canvas_set_color(canvas, ColorBlack);
            }
        }
    } else if(!model->scanning) {
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(canvas, 64, 50, AlignCenter, AlignTop, "No signals detected");
    }

    // Instructions
    canvas_set_font(canvas, FontSecondary);
    if(model->scanning) {
        canvas_draw_str_aligned(canvas, 64, 60, AlignCenter, AlignTop, "Press Back to stop");
    } else {
        canvas_draw_str_aligned(canvas, 64, 60, AlignCenter, AlignTop, "OK: Scan  Back: Exit");
    }
}

static bool cc1101_scanning_view_input_callback(InputEvent* event, void* context) {
    furi_assert(context);
    CC1101ScanningView* scanning_view = context;
    bool consumed = false;

    if(event->type == InputTypeShort) {
        switch(event->key) {
        case InputKeyBack:
            with_view_model(
                scanning_view->view,
                CC1101ScanningViewModel * model,
                {
                    if(model->scanning) {
                        model->scanning = false;
                        consumed = true;
                    } else {
                        if(scanning_view->callback) {
                            scanning_view->callback(
                                CC1101SubGhzCustomEventViewScanningBack, scanning_view->context);
                        }
                        consumed = true;
                    }
                },
                true);
            break;
        case InputKeyOk:
            with_view_model(
                scanning_view->view,
                CC1101ScanningViewModel * model,
                {
                    if(!model->scanning) {
                        model->scanning = true;
                        model->scan_count = 0;
                        model->signal_count = 0;
                        model->selected_signal = 0;
                        if(scanning_view->callback) {
                            scanning_view->callback(
                                CC1101SubGhzCustomEventViewScanningOk, scanning_view->context);
                        }
                    }
                    consumed = true;
                },
                true);
            break;
        case InputKeyUp:
            with_view_model(
                scanning_view->view,
                CC1101ScanningViewModel * model,
                {
                    if(model->signal_count > 0 && model->selected_signal > 0) {
                        model->selected_signal--;
                    }
                    consumed = true;
                },
                true);
            break;
        case InputKeyDown:
            with_view_model(
                scanning_view->view,
                CC1101ScanningViewModel * model,
                {
                    if(model->signal_count > 0 && 
                       model->selected_signal < model->signal_count - 1) {
                        model->selected_signal++;
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

CC1101ScanningView* cc1101_scanning_view_alloc() {
    CC1101ScanningView* scanning_view = malloc(sizeof(CC1101ScanningView));

    scanning_view->view = view_alloc();
    view_allocate_model(scanning_view->view, ViewModelTypeLocking, sizeof(CC1101ScanningViewModel));
    view_set_context(scanning_view->view, scanning_view);
    view_set_draw_callback(scanning_view->view, cc1101_scanning_view_draw_callback);
    view_set_input_callback(scanning_view->view, cc1101_scanning_view_input_callback);

    with_view_model(
        scanning_view->view,
        CC1101ScanningViewModel * model,
        {
            model->frequency = 433920000;
            model->scanning = false;
            model->scan_count = 0;
            model->signal_count = 0;
            model->selected_signal = 0;
        },
        true);

    return scanning_view;
}

void cc1101_scanning_view_free(CC1101ScanningView* scanning_view) {
    furi_assert(scanning_view);
    view_free(scanning_view->view);
    free(scanning_view);
}

View* cc1101_scanning_view_get_view(CC1101ScanningView* scanning_view) {
    furi_assert(scanning_view);
    return scanning_view->view;
}

void cc1101_scanning_view_set_callback(
    CC1101ScanningView* scanning_view,
    CC1101ScanningViewCallback callback,
    void* context) {
    furi_assert(scanning_view);
    scanning_view->callback = callback;
    scanning_view->context = context;
}

void cc1101_scanning_view_set_frequency(CC1101ScanningView* scanning_view, uint32_t frequency) {
    furi_assert(scanning_view);
    with_view_model(
        scanning_view->view,
        CC1101ScanningViewModel * model,
        { model->frequency = frequency; },
        true);
}

void cc1101_scanning_view_start_scan(CC1101ScanningView* scanning_view) {
    furi_assert(scanning_view);
    with_view_model(
        scanning_view->view,
        CC1101ScanningViewModel * model,
        {
            model->scanning = true;
            model->scan_count = 0;
            model->signal_count = 0;
        },
        true);
}

void cc1101_scanning_view_stop_scan(CC1101ScanningView* scanning_view) {
    furi_assert(scanning_view);
    with_view_model(
        scanning_view->view,
        CC1101ScanningViewModel * model,
        { model->scanning = false; },
        true);
}

void cc1101_scanning_view_add_signal(
    CC1101ScanningView* scanning_view,
    uint32_t frequency,
    int8_t rssi,
    const char* protocol) {
    furi_assert(scanning_view);
    
    with_view_model(
        scanning_view->view,
        CC1101ScanningViewModel * model,
        {
            if(model->signal_count < MAX_DETECTED_SIGNALS) {
                model->signals[model->signal_count].frequency = frequency;
                model->signals[model->signal_count].rssi = rssi;
                strncpy(
                    model->signals[model->signal_count].protocol,
                    protocol,
                    sizeof(model->signals[model->signal_count].protocol) - 1);
                model->signals[model->signal_count].protocol[15] = '\0';
                model->signal_count++;
            }
            model->scan_count++;
        },
        true);
}

void cc1101_scanning_view_clear_signals(CC1101ScanningView* scanning_view) {
    furi_assert(scanning_view);
    with_view_model(
        scanning_view->view,
        CC1101ScanningViewModel * model,
        {
            model->signal_count = 0;
            model->selected_signal = 0;
        },
        true);
}
