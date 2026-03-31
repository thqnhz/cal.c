#include "../include/resource_dir.h"
#include "../include/raylib.h"
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

#define WINW 360
#define WINH 720
#define APP_NAME "Calc"
#define APP_NAME_FULL "Calculator"
#define FONT_LOC "Rubik/Rubik-Regular.ttf"
#define WIN_PADDING 8.0f
#define PADDING (WIN_PADDING * 2.0f)
#define CALC_HEADER 48.0f
#define CALC_DISPLAY_H 120.0f
#define CALC_DISPLAY_FONT_SIZE 48.0f
#define CALC_BUTTON_PADDING_W 2.0f
#define CALC_BUTTON_PADDING_H 8.0f
#define CALC_NUMBER_ROWS 4
#define CALC_NUMBER_COLS 5
#define CALC_FUNCTION_ROWS 5
#define CALC_FUNCTION_COLS 6
#define CALC_FUNCTION_BUTTON_H 44.0f
#define CALC_FUNCTION_BUTTON_W ((WINW - (PADDING + CALC_BUTTON_PADDING_W) * 2.0f) / 6.0f)
#define CALC_FUNCTION_BUTTON_FONT_SIZE 20.0f
#define CALC_NUMBER_BUTTON_H 52.0f
#define CALC_NUMBER_BUTTON_W ((WINW - (PADDING + CALC_BUTTON_PADDING_W) * 2.0f) / 5.0f)
#define CALC_NUMBER_BUTTON_FONT_SIZE 28.0f

typedef struct Button {
    const char *text;
    const char *shift_text;
    const char *alpha_text;
    Rectangle rect;
    Vector2 text_pos;
    Vector2 shift_text_pos;
    Vector2 alpha_text_pos;
} Button;

static Button func_btns[CALC_FUNCTION_ROWS][CALC_FUNCTION_COLS] = {
    {
        (Button){ "SHIFT", },
        (Button){ "ALPHA", },
        (Button){ "LEFT",  },
        (Button){ "RIGHT", },
        (Button){ "MODE",  },
        (Button){ "2nd",   },
    },
    {
        (Button){ "OPTN",  },
        (Button){ "CALC",    "SOLVE",   "=",      },
        (Button){ "UP",    },
        (Button){ "DOWN",  },
        (Button){ "Sdx",     "d/dx",    ":"       },
        (Button){ "x",       "Epsilon", "Sigma"   },
    },
    {
        (Button){ "x/y",     "xy/x",    "/R",     },
        (Button){ "SQRT",    "CBRT",    "mod",    },
        (Button){ "x^2",     "x^3",     "neg(x)", },
        (Button){ "x^y",     "xRy",     "Cot",    },
        (Button){ "Logx(y)", "10^y",    "Cot^-1", },
        (Button){ "Ln",      "e^x",     "t",      },
    },
    {
        (Button){ "(-)",     "Log",     "a",      },
        (Button){ "o\"",     "FACT",    "b",      },
        (Button){ "x^-1",    "x!",      "c",      },
        (Button){ "Sin",     "Sin^-1",  "d",      },
        (Button){ "Cos",     "Cos^-1",  "e",      },
        (Button){ "Tan",     "Tan^-1",  "f",      },
    },
    {
        (Button){ "STO",     "RCL",     "CLRv",   },
        (Button){ "ENG",     "ANGL",    "i",      },
        (Button){ "(",       "Abs",     "x",      },
        (Button){ ")",       ",",       "y",      },
        (Button){ "S<=>D",   NULL,      "z",      },
        (Button){ "M+",      "M-",      "m",      },
    },
};

static Button num_btns[CALC_NUMBER_ROWS][CALC_NUMBER_COLS] = {
    {
        (Button){ "0",       "COPY",    "PASTE",  },
        (Button){ ".",       "Ran#",    "RanInt", },
        (Button){ "x10",     "PI",      "e",      },
        (Button){ "Ans",     "%",       "PreAns", },
        (Button){ "=",       "HISTORY", },
    },
    {
        (Button){ "1",       "STAT",    },
        (Button){ "2",       "CMPLX",   },
        (Button){ "3",       "DISTR",   },
        (Button){ "+",       "Pol",     "Ceil",    },
        (Button){ "-",       "Rec",     "Floor",   },
    },
    {
        (Button){ "4",       "MATRIX",  },
        (Button){ "5",       "VECTOR",  },
        (Button){ "6",       "FUNC",    "HELP",    },
        (Button){ "x",       "nPr",     "GCD",     },
        (Button){ "/",       "nCr",     "LCM",     },
    },
    {
        (Button){ "7",       "CONST",   },
        (Button){ "8",       "CONV",    },
        (Button){ "9",       "LIMIT",   "Inf",     },
        (Button){ "DEL",   },
        (Button){ "AC",      "CLR ALL", },
    },
};

static Font font;

typedef struct {
    char *data;
    size_t len;
    size_t cap;
} String;

typedef struct {
    String input;
    String output;
} Calc;

static Calc calc;

static const Color CALC_THEME_BASE    = (Color){  30,  30,  46, 255 };
static const Color CALC_THEME_MANTLE  = (Color){  24,  24,  37, 255 };
static const Color CALC_THEME_CRUST   = (Color){  17,  17,  27, 255 };
static const Color CALC_THEME_TEXT    = (Color){ 205, 214, 244, 255 };
static const Color CALC_THEME_ACCENT  = (Color){ 137, 180, 250, 255 };
static const Color CALC_THEME_OVERLAY = (Color){ 108, 112, 134, 255 };

static const Rectangle CALC_BG_RECT = (Rectangle){ .x      = WIN_PADDING,
                                                   .y      = WIN_PADDING,
                                                   .width  = WINW - WIN_PADDING * 2.0f,
                                                   .height = WINH - WIN_PADDING * 2.0f };
static const Rectangle CALC_DISPLAY_RECT = (Rectangle){ .x      = PADDING,
                                                        .y      = PADDING + CALC_HEADER,
                                                        .width  = WINW - PADDING * 2.0f,
                                                        .height = CALC_DISPLAY_H };

static inline void init();
static inline void load_font();
static inline void init_number_buttons();
static inline void init_function_buttons();
static inline void draw_background();
static inline void draw_header();
static inline void draw_display();
static inline void draw_display_text_input();
static inline void draw_display_text_result();
static inline void draw_number_buttons();
static inline void draw_function_buttons();

static inline void update_user_input();
static inline void add_input_string(Rectangle *r, const char *str);

static inline void init() {
    InitWindow(WINW, WINH, APP_NAME);

    if (!SearchAndSetResourceDir("assets"))
        TraceLog(LOG_WARNING, "Failed to set resource directory, fonts won't load correctly");

    // TODO: about this mess
    int c;
    int *cpoints = LoadCodepoints(
        "0123456789"
        "+-/()'\",.<=>"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz",
        &c
    );
    font = LoadFontEx(TextFormat("%s/%s", GetWorkingDirectory(), FONT_LOC), 144, cpoints, c);
    GenTextureMipmaps(&font.texture);
    SetTextureFilter(font.texture, TEXTURE_FILTER_TRILINEAR);

    calc.input  = (String){ .data = malloc(1024), .len = 0, .cap = 1 };
    calc.output = (String){ .data = malloc(1024), .len = 0, .cap = 1 };

    init_number_buttons();
    init_function_buttons();
}

static inline void draw_app() {
    draw_background();
    draw_header();
    draw_display();
    draw_display_text_input();
    draw_display_text_result();
    draw_number_buttons();
    draw_function_buttons();
}

static inline void update() {
    update_user_input();
}

static inline void draw_background() {
    ClearBackground(CALC_THEME_BASE);
    DrawRectangleRounded(CALC_BG_RECT, .05f, 0, CALC_THEME_MANTLE);
}

static inline void draw_header() {
    Vector2 text_size = MeasureTextEx(font, APP_NAME_FULL, 32.0f, .0f);
    Vector2 draw_pos = { (WINW - text_size.x) / 2.0f, PADDING + (CALC_HEADER - text_size.y) / 2.0f };
    DrawTextEx(font, APP_NAME_FULL, draw_pos, 32.0f, .0f, CALC_THEME_TEXT);
}

static inline void draw_display() {
    DrawRectangleRounded(CALC_DISPLAY_RECT, 0.05f, 0, CALC_THEME_CRUST);
}

static inline void draw_display_text_input() {
    if (calc.input.len == 0) return;
    Vector2 text_size = MeasureTextEx(font, calc.input.data, CALC_DISPLAY_FONT_SIZE, .0f);
    Vector2 draw_pos = {
        .x = CALC_DISPLAY_RECT.x + WIN_PADDING,
        .y = CALC_DISPLAY_RECT.y,
    };
    DrawTextEx(font, calc.input.data, draw_pos, CALC_DISPLAY_FONT_SIZE, .0f, CALC_THEME_TEXT);
}

static inline void draw_display_text_result() {
    if (calc.output.len == 0) calc.output.data = "0";
    Vector2 text_size = MeasureTextEx(font, calc.output.data, CALC_DISPLAY_FONT_SIZE, .0f);
    Vector2 draw_pos = {
        .x = CALC_DISPLAY_RECT.x + CALC_DISPLAY_RECT.width  - text_size.x - WIN_PADDING,
        .y = CALC_DISPLAY_RECT.y + CALC_DISPLAY_RECT.height - text_size.y,
    };
    DrawTextEx(font, calc.output.data, draw_pos, CALC_DISPLAY_FONT_SIZE, .0f, CALC_THEME_TEXT);
}

static inline void init_function_buttons() {
    for (size_t row = 0; row < CALC_FUNCTION_ROWS; ++row) {
        for (size_t col = 0; col < CALC_FUNCTION_COLS; ++col) {
            Button *b = &func_btns[row][col];
            Rectangle r = (Rectangle){
                .x = PADDING + col * (CALC_FUNCTION_BUTTON_W + CALC_BUTTON_PADDING_W),
                .y = PADDING + row * (CALC_FUNCTION_BUTTON_H + CALC_BUTTON_PADDING_H) + CALC_HEADER + CALC_DISPLAY_H + PADDING * 1.5f,
                .width  = CALC_FUNCTION_BUTTON_W - CALC_BUTTON_PADDING_W * 4.0f,
                .height = CALC_FUNCTION_BUTTON_H - CALC_BUTTON_PADDING_H
            };
            b->rect = r;

            Vector2 text_size = MeasureTextEx(font, b->text, CALC_FUNCTION_BUTTON_FONT_SIZE, .0f);
            b->text_pos = (Vector2){
                .x = r.x + (r.width - text_size.x) / 2.0f,
                .y = r.y + (r.height - text_size.y) / 2.0f
            };

        }
    }
}

static inline void init_number_buttons() {
    for (size_t row = 0; row < CALC_NUMBER_ROWS; ++row) {
        for (size_t col = 0; col < CALC_NUMBER_COLS; ++col) {
            Button *b = &num_btns[row][col];
            Rectangle r = (Rectangle){
                .x = PADDING + col * (CALC_NUMBER_BUTTON_W + CALC_BUTTON_PADDING_W),
                .y = WINH    - row * (CALC_NUMBER_BUTTON_H + CALC_BUTTON_PADDING_H) - PADDING * 4.0f,
                .width  = CALC_NUMBER_BUTTON_W - CALC_BUTTON_PADDING_W * 2.0f,
                .height = CALC_NUMBER_BUTTON_H - CALC_BUTTON_PADDING_H
            };
            b->rect = r;

            Vector2 text_size = MeasureTextEx(font, b->text, CALC_NUMBER_BUTTON_FONT_SIZE, .0f);
            b->text_pos = (Vector2){
                .x = r.x + (r.width  - text_size.x) / 2.0f,
                .y = r.y + (r.height - text_size.y) / 2.0f
            };
        }
    }
}

static inline void draw_function_buttons() {
    for (size_t row = 0; row < CALC_FUNCTION_ROWS; ++row) {
        for (size_t col = 0; col < CALC_FUNCTION_COLS; ++col) {
            Button *b = &func_btns[row][col];
            Rectangle *r = &b->rect;
            Color c = CALC_THEME_CRUST;
            if (CheckCollisionPointRec(GetMousePosition(), *r)) c = CALC_THEME_OVERLAY;
            DrawRectangleRounded(*r, .2f, 0, c);
            DrawTextEx(font, b->text, b->text_pos, CALC_FUNCTION_BUTTON_FONT_SIZE, .0f, CALC_THEME_TEXT);
        }
    }
}

static inline void draw_number_buttons() {
    for (size_t row = 0; row < CALC_NUMBER_ROWS; ++row) {
        for (size_t col = 0; col < CALC_NUMBER_COLS; ++col) {
            Button *b = &num_btns[row][col];
            Rectangle *r = &b->rect;
            Color c = CALC_THEME_CRUST;
            if (CheckCollisionPointRec(GetMousePosition(), *r)) c = CALC_THEME_OVERLAY;
            DrawRectangleRounded(*r, .2f, 0, c);
            DrawTextEx(font, b->text, b->text_pos, CALC_NUMBER_BUTTON_FONT_SIZE, .0f, CALC_THEME_TEXT);
        }
    }
}

static inline void update_user_input() {
    for (size_t row = 0; row < CALC_FUNCTION_ROWS; ++row) {
        for (size_t col = 0; col < CALC_FUNCTION_COLS; ++col) {
            Button *bf = &func_btns[row][col];
            Button *bn = (row < CALC_NUMBER_ROWS && col < CALC_NUMBER_COLS) ? &num_btns[row][col] : NULL;
            Rectangle *rf = &bf->rect;
            Rectangle *rn = (bn) ? &bn->rect : NULL;

            if (rn && CheckCollisionPointRec(GetMousePosition(), *rn)) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    add_input_string(rf, bn->text);
                    break;
                }
            } else if (CheckCollisionPointRec(GetMousePosition(), *rf)) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    add_input_string(rf, bf->text);
                    break;
                }
            }
        }
    }
}

static inline void add_input_string(Rectangle *r, const char *str) {
    if (calc.input.len + strlen(str) >= calc.input.cap) {
        calc.input.cap *= 2;
        calc.input.data = realloc(calc.input.data, calc.input.cap);
    }
    memcpy(calc.input.data + calc.input.len, str, strlen(str));
    calc.input.len += strlen(str);
    calc.input.data[calc.input.len] = '\0';
}

int main() {
    init();
    while (!WindowShouldClose()) {
        BeginDrawing();
        draw_app();
        update();
        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}

