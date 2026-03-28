#include "../include/resource_dir.h"
#include "../include/raylib.h"

#define WINW 360
#define WINH 720
#define APP_NAME "Calc"
#define APP_NAME_FULL "Calculator"
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
static inline void draw_header(Font font);
static inline void draw_display();
static inline void draw_display_text_result(Font font, const char *text, float font_size);

static inline void draw_app(Font font) {
    ClearBackground(CALC_THEME_BASE);
    draw_background();
    draw_header(font);
    draw_display();
}

static inline void draw_background() {
    DrawRectangleRounded(CALC_BG, .05f, 0, CALC_THEME_MANTLE);
}

static inline void draw_header(Font font) {
    Vector2 text_size = MeasureTextEx(font, APP_NAME_FULL, 32.0f, .0f);
    Vector2 draw_pos = { (WINW - text_size.x) / 2.0f, PADDING + (CALC_HEADER - text_size.y) / 2.0f };
    DrawTextEx(font, APP_NAME_FULL, draw_pos, 32.0f, .0f, CALC_THEME_TEXT); }

static inline void draw_display() {
    DrawRectangleRounded(CALC_DISPLAY, 0.05f, 0, CALC_THEME_CRUST);
}

static inline void draw_display_text_result(Font font, const char *text, float font_size) {
    Vector2 text_size = MeasureTextEx(font, text, font_size, 0.0f);
}

int main() {
    InitWindow(WINW, WINH, APP_NAME);

    if (!SearchAndSetResourceDir("assets"))
        TraceLog(LOG_WARNING, "Failed to set resource directory, fonts won't load correctly");
    Font font = LoadFontEx(TextFormat("%s/%s", GetWorkingDirectory(), "Rubik/Rubik-Regular.ttf"), 72, 0, 0);

    while (!WindowShouldClose()) {
        BeginDrawing();
        draw_app(font);
        DrawTextEx(font, "0", (Vector2){ PADDING + 4.0f, PADDING + CALC_HEADER }, 36.0f, .0f, CALC_THEME_TEXT);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

