#include "../include/resource_dir.h"
#include "../include/raylib.h"
#include <stddef.h>
#include <unistd.h>

#define WINW 360
#define WINH 720
#define APP_NAME "Calc"
#define APP_NAME_FULL "Calculator"
#define WIN_PADDING 8.0f
#define PADDING (WIN_PADDING * 2.0f)
#define CALC_HEADER 48.0f
#define CALC_DISPLAY_H 92.0f
#define CALC_BUTTON_PADDING_W 2.0f
#define CALC_BUTTON_PADDING_H 8.0f
#define CALC_FUNCTION_BUTTON_H 32.0f
#define CALC_FUNCTION_BUTTON_W ((WINW - (PADDING + CALC_BUTTON_PADDING_W) * 2.0f) / 6.0f) 
#define CALC_NUMBER_BUTTON_H 48.0f
#define CALC_NUMBER_BUTTON_W ((WINW - (PADDING + CALC_BUTTON_PADDING_W) * 2.0f) / 5.0f)
#define CALC_NUMBER_BUTTON_FONT_SIZE 28.0f

typedef struct Button {
    const char *text;
    const char *shift_text;
    const char *alpha_text;
} Button;

static const Vector2 function_button_size = (Vector2){
    .x = CALC_FUNCTION_BUTTON_W,
    .y = CALC_FUNCTION_BUTTON_H,
};

static const Vector2 number_button_size = (Vector2){
    .x = CALC_NUMBER_BUTTON_W,
    .y = CALC_NUMBER_BUTTON_H,
};

static const Button function_buttons[5][6];
static const Button number_buttons[4][5] = {
    {
        (Button){ "0", "Rnd", "" },
        (Button){ ".", "Ran#", "RanInt" },
        (Button){ "x10", "PI", "e" },        // Exponent, Pi and Euler
        (Button){ "Ans", "%", "PreAns" },
        (Button){ "=", "~", "" },             // Equal and approximation
    },
    {
        (Button){ "1", "", "" },
        (Button){ "2", "", "" },
        (Button){ "3", "", "" },
        (Button){ "+", "Pol", "lnt" },
        (Button){ "-", "Pol", "lntg" },
    },
    {
        (Button){ "4", "", "" },
        (Button){ "5", "", "" },
        (Button){ "6", "", "" },
        (Button){ "x", "nPr", "GCD" },
        (Button){ "/", "nCr", "LCM" },
    },
    {
        (Button){ "7", "CONST", "" },
        (Button){ "8", "CONV", "" },
        (Button){ "9", "", "" },
        (Button){ "DEL", "INS", "UNDO" },
        (Button){ "AC", "OFF", "" },
    },
};

static Rectangle function_button_rects[5][6] = {0};
static Rectangle number_button_rects[4][5] = {0};

static Vector2 function_button_text_pos[5][6] = {0};
static Vector2 number_button_text_pos[4][5] = {0};

static Font font;

static const Color CALC_THEME_BASE = (Color){ 30, 30, 46, 255 };
static const Color CALC_THEME_MANTLE = (Color){ 24, 24, 37, 255 };
static const Color CALC_THEME_CRUST = (Color){ 17, 17, 27, 255 };
static const Color CALC_THEME_TEXT = (Color){ 205, 214, 244, 255 };
static const Color CALC_THEME_ACCENT = (Color){ 137, 180, 250, 255 };

static const Rectangle CALC_BG = (Rectangle){ .x      = WIN_PADDING,
                                              .y      = WIN_PADDING,
                                              .width  = WINW - WIN_PADDING * 2.0f,
                                              .height = WINH - WIN_PADDING * 2.0f };
static const Rectangle CALC_DISPLAY = (Rectangle){ .x      = PADDING,
                                                   .y      = PADDING + CALC_HEADER,
                                                   .width  = WINW - PADDING * 2.0f,
                                                   .height = CALC_DISPLAY_H };

static inline void init();
static inline void load_font();
static inline void init_number_button_rects();
static inline void init_number_button_text_pos();
static inline void draw_background();
static inline void draw_header();
static inline void draw_display();
static inline void draw_display_text_result(const char *text, float font_size);
static inline void draw_function_buttons();
static inline void draw_number_buttons();

static inline void init() {
    load_font();
    init_number_button_rects();
    init_number_button_text_pos();
}

static inline void load_font() {
    // TODO: Find a way to load the math symbols
    // int codepoints[512] = {0};
    // int count = 0;
    //
    // for (size_t i = 0; i < 95; ++i) codepoints[count++] = i + 32;      // ASCII
    // for (size_t i = 0; i < 256; ++i) codepoints[count++] = i + 0x2200; // Math symbols
    font = LoadFontEx(TextFormat("%s/%s", GetWorkingDirectory(), "Rubik/Rubik-Regular.ttf"), 144, 0, 0);
}

static inline void draw_app() {
    draw_background();
    draw_header();
    draw_display();
    draw_number_buttons();
}

static inline void draw_background() {
    ClearBackground(CALC_THEME_BASE);
    DrawRectangleRounded(CALC_BG, .05f, 0, CALC_THEME_MANTLE);
}

static inline void draw_header() {
    Vector2 text_size = MeasureTextEx(font, APP_NAME_FULL, 32.0f, .0f);
    Vector2 draw_pos = { (WINW - text_size.x) / 2.0f, PADDING + (CALC_HEADER - text_size.y) / 2.0f };
    DrawTextEx(font, APP_NAME_FULL, draw_pos, 32.0f, .0f, CALC_THEME_TEXT);
}

static inline void draw_display() {
    DrawRectangleRounded(CALC_DISPLAY, 0.05f, 0, CALC_THEME_CRUST);
}

static inline void draw_display_text_result(const char *text, float font_size) {}

static inline void draw_function_buttons() {}

static inline void init_number_button_text_pos() {
    for (size_t row = 4; row > 0; --row) {
        for (size_t col = 5; col > 0; --col) {
            Rectangle *r = &number_button_rects[row - 1][col - 1];
            Vector2 text_size = MeasureTextEx(font, number_buttons[row - 1][col - 1].text, CALC_NUMBER_BUTTON_FONT_SIZE, .0f);
            number_button_text_pos[row - 1][col - 1] = (Vector2){
                .x = r->x + (r->width - text_size.x) / 2.0f,
                .y = r->y + (r->height - text_size.y) / 2.0f
            };
        }
    }
}

static inline void init_number_button_rects() {
    for (size_t row = 4; row > 0; --row) {
        for (size_t col = 5; col > 0; --col) {
            number_button_rects[row - 1][col - 1] = (Rectangle){
                .x = PADDING + (col - 1) * (number_button_size.x + CALC_BUTTON_PADDING_W),
                .y = WINH - PADDING * 4.0f - (row - 1) * (number_button_size.y + CALC_BUTTON_PADDING_H),
                .width = number_button_size.x - CALC_BUTTON_PADDING_W * 2.0f,
                .height = number_button_size.y - CALC_BUTTON_PADDING_H
            };
        }
    }
}

static inline void draw_number_buttons() {
    for (size_t row = 4; row > 0; --row) {
        for (size_t col = 5; col > 0; --col) {
            DrawRectangleRounded(
                number_button_rects[row - 1][col - 1],
                .05f,
                0,
                CALC_THEME_CRUST
            );
            DrawTextEx(font, number_buttons[row - 1][col - 1].text, number_button_text_pos[row - 1][col - 1], CALC_NUMBER_BUTTON_FONT_SIZE, .0f, CALC_THEME_TEXT);
        }
    }
}

int main() {
    InitWindow(WINW, WINH, APP_NAME);

    if (!SearchAndSetResourceDir("assets"))
        TraceLog(LOG_WARNING, "Failed to set resource directory, fonts won't load correctly");
    
    init();
    while (!WindowShouldClose()) {
        BeginDrawing();
        draw_app();
        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}

