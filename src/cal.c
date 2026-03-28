#include "../include/raylib.h"

#define WINW 360
#define WINH 720
#define APP_NAME "Calc"
#define WIN_PADDING 8.0f
#define PADDING (WIN_PADDING * 2.0f)
#define CALC_HEADER 40.0f
#define CALC_DISPLAY_H 72.0f

static const Color CALC_THEME_BASE = (Color){ 30, 30, 46, 255 };
static const Color CALC_THEME_MANTLE = (Color){ 24, 24, 37, 255 };
static const Color CALC_THEME_CRUST = (Color){ 17, 17, 27, 255 };
static const Color CALC_THEME_TEXT = (Color){ 205, 214, 244, 255 };

static const Rectangle CALC_BG = (Rectangle){ .x      = WIN_PADDING,
                                              .y      = WIN_PADDING,
                                              .width  = WINW - WIN_PADDING * 2.0f,
                                              .height = WINH - WIN_PADDING * 2.0f };
static const Rectangle CALC_DISPLAY = (Rectangle){ .x      = PADDING,
                                                   .y      = PADDING + CALC_HEADER,
                                                   .width  = WINW - PADDING * 2.0f,
                                                   .height = CALC_DISPLAY_H };

static inline void draw_background();
static inline void draw_display();

static inline void draw_app() {
    ClearBackground(CALC_THEME_BASE);
    draw_background();
    draw_display();
}

static inline void draw_background() {
    DrawRectangleRounded(CALC_BG, .05f, 0, CALC_THEME_MANTLE);
}

static inline void draw_display() {
    DrawRectangleRounded(CALC_DISPLAY, 0.05f, 0, CALC_THEME_CRUST);
}

int main() {
    InitWindow(WINW, WINH, APP_NAME);

    while (!WindowShouldClose()) {
        BeginDrawing();
        draw_app();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

