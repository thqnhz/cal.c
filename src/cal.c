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
#define CALC_BUTTON_PADDING 2.0f
#define CALC_FUNCTION_BUTTON_H 32.0f
#define CALC_FUNCTION_BUTTON_W ((WINW - (PADDING + CALC_BUTTON_PADDING) * 2.0f) / 6.0f) 
#define CALC_NUMBER_BUTTON_H 48.0f
#define CALC_NUMBER_BUTTON_W ((WINW - (PADDING + CALC_BUTTON_PADDING) * 2.0f) / 5.0f) 

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
        (Button){ "7", "CONST", "" },
        (Button){ "8", "CONV", "" },
        (Button){ "9", "", "" },
        (Button){ "DEL", "INS", "UNDO" },
        (Button){ "AC", "OFF", "" },
    },
    {
        (Button){ "4", "", "" },
        (Button){ "5", "", "" },
        (Button){ "6", "", "" },
        (Button){ "×", "nPr", "GCD" },
        (Button){ "÷", "nCr", "LCM" },
    },
    {
        (Button){ "1", "", "" },
        (Button){ "2", "", "" },
        (Button){ "3", "", "" },
        (Button){ "+", "Pol", "lnt" },
        (Button){ "-", "Pol", "lntg" },
    },
    {
        (Button){ "0", "Rnd", "" },
        (Button){ ".", "Ran#", "RanInt" },
        (Button){ "⏨", "ℼ", "ℯ" },        // Exponent, Pi and Euler
        (Button){ "Ans", "%", "PreAns" },
        (Button){ "=", "≈", "" },         // Equal and approximation
    },
};

static Rectangle function_button_rects[5][6] = {0};
static Rectangle number_button_rects[4][5] = {0};

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

static inline void init_number_button_rects();
static inline void draw_background();
static inline void draw_header();
static inline void draw_display();
static inline void draw_display_text_result(const char *text, float font_size);
static inline void draw_function_buttons();
static inline void draw_number_buttons();

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

static inline void init_number_button_rects() {
    for (size_t row = 4; row > 0; --row) {
        for (size_t col = 5; col > 0; --col) {
            number_button_rects[row - 1][col - 1] = (Rectangle){
                .x = PADDING + (col - 1) * (number_button_size.x + CALC_BUTTON_PADDING),
                .y = WINH - PADDING * 4.0f - (row - 1) * (number_button_size.y + CALC_BUTTON_PADDING),
                .width = number_button_size.x - CALC_BUTTON_PADDING * 2.0f,
                .height = number_button_size.y - CALC_BUTTON_PADDING * 2.0f
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
        }
    }
}

int main() {
    InitWindow(WINW, WINH, APP_NAME);

    if (!SearchAndSetResourceDir("assets"))
        TraceLog(LOG_WARNING, "Failed to set resource directory, fonts won't load correctly");
    font = LoadFontEx(TextFormat("%s/%s", GetWorkingDirectory(), "Rubik/Rubik-Regular.ttf"), 144, 0, 0);
    init_number_button_rects();
    while (!WindowShouldClose()) {
        BeginDrawing();
        draw_app();
        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}

