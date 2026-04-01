#include "../include/resource_dir.h"
#include "../include/raylib.h"
#include <stddef.h>
#include <unistd.h>

#define WINW 360
#define WINH 720
#define APP_NAME "Calc"
#define APP_NAME_FULL "Calculator"
#define FONT_LOC "DejaVu/DejaVuSans.ttf"
#define WIN_PADDING 8.0f
#define PADDING (WIN_PADDING * 2.0f)
#define CALC_HEADER 48.0f
#define CALC_DISPLAY_H 120.0f
#define CALC_DISPLAY_FONT_SIZE 48.0f
#define CALC_DISPLAY_FONT_SIZE_MIN 24.0f
#define CALC_BUTTON_PADDING_W .5f
#define CALC_BUTTON_PADDING_H 8.0f
#define CALC_NUMBER_ROWS 4
#define CALC_NUMBER_COLS 5
#define CALC_FUNCTION_ROWS 5
#define CALC_FUNCTION_COLS 6
#define CALC_FUNCTION_BUTTON_H 44.0f
#define CALC_FUNCTION_BUTTON_W ((WINW - (PADDING + CALC_BUTTON_PADDING_W) * 2.0f) / 6.0f)
#define CALC_FUNCTION_BUTTON_FONT_SIZE 18.0f
#define CALC_NUMBER_BUTTON_H 54.0f
#define CALC_NUMBER_BUTTON_W ((WINW - (PADDING + CALC_BUTTON_PADDING_W) * 2.0f) / 5.0f)
#define CALC_NUMBER_BUTTON_FONT_SIZE 28.0f
#define CALC_BUTTON_SHIFT_ALPHA_FONT_SIZE 14.0f
#define CALC_MAX_INPUT_LEN 128
#define CALC_MAX_OUTPUT_LEN 32

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
        (Button){ "◀",  },
        (Button){ "▶", },
        (Button){ "MODE",  },
        (Button){ "2nd",   },
    },
    {
        (Button){ "OPTN",  },
        (Button){ "CALC",    "SOLVE",   "=",      },
        (Button){ "▲",    },
        (Button){ "▼",  },
        (Button){ "∫dx",     "d/dx",    ":"       },
        (Button){ "x",       "Σ", "∏"   },
    },
    {
        (Button){ "x/y",     "xy/z",    "÷R",     },
        (Button){ "√x",      "∛x",      "mod",    },
        (Button){ "x²",      "x³",      "neg(x)", },
        (Button){ "xʸ",      "xRy",     "Cot",    },
        (Button){ "Logₓ(y)", "10ʸ",     "Cot⁻¹", },
        (Button){ "Ln",      "eˣ",      "t",      },
    },
    {
        (Button){ "(-)",     "Log",     "a",      },
        (Button){ "°'\"",    "FACT",    "b",      },
        (Button){ "x⁻¹",     "x!",      "c",      },
        (Button){ "Sin",     "Sin⁻¹",   "d",      },
        (Button){ "Cos",     "Cos⁻¹",   "e",      },
        (Button){ "Tan",     "Tan⁻¹",   "f",      },
    },
    {
        (Button){ "STO",     "RCL",     "CLRv",   },
        (Button){ "ENG",     "∠",       "i",      },
        (Button){ "(",       "|x|",     "x",      },
        (Button){ ")",       ",",       "y",      },
        (Button){ "S⇔D",     NULL,      "z",      },
        (Button){ "M+",      "M-",      "m",      },
    },
};

static Button num_btns[CALC_NUMBER_ROWS][CALC_NUMBER_COLS] = {
    {
        (Button){ "0",       "COPY",    "PASTE",  },
        (Button){ ".",       "Ran#",    "RanInt", },
        (Button){ "×10",     "π",       "e",      },
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
        (Button){ "×",       "nPr",     "GCD",     },
        (Button){ "÷",       "nCr",     "LCM",     },
    },
    {
        (Button){ "7",       "CONST",   },
        (Button){ "8",       "CONV",    },
        (Button){ "9",       "LIMIT",   "∞",     },
        (Button){ "DEL",   },
        (Button){ "AC",      "CLR ALL", },
    },
};

static Font font;

typedef struct {
    char input[CALC_MAX_INPUT_LEN];
    char *output;
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
static inline void calc_text_size(Rectangle *r, Button *b, float size);
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
        "+-/()'\",.=^|:%!"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        u8"π⁻⁰¹²³⁴⁵⁶⁷⁸⁹∠°Σ∏▼▲◀▶√∛×÷∫⇔ₓˣʸ∞",
        &c
    );
    font = LoadFontEx(TextFormat("%s/%s", GetWorkingDirectory(), FONT_LOC), 144, cpoints, c);
    GenTextureMipmaps(&font.texture);
    SetTextureFilter(font.texture, TEXTURE_FILTER_TRILINEAR);

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
    if (TextLength(calc.input) == 0) return;
    Vector2 text_size = MeasureTextEx(font, calc.input, CALC_DISPLAY_FONT_SIZE, .0f);
    Vector2 draw_pos = {
        .x = CALC_DISPLAY_RECT.x + WIN_PADDING,
        .y = CALC_DISPLAY_RECT.y,
    };
    DrawTextEx(font, calc.input, draw_pos, CALC_DISPLAY_FONT_SIZE, .0f, CALC_THEME_TEXT);
}

static inline void draw_display_text_result() {
    if (TextLength(calc.output) == 0) calc.output = "0";
    Vector2 text_size = MeasureTextEx(font, calc.output, CALC_DISPLAY_FONT_SIZE, .0f);
    Vector2 draw_pos = {
        .x = CALC_DISPLAY_RECT.x + CALC_DISPLAY_RECT.width  - text_size.x - WIN_PADDING,
        .y = CALC_DISPLAY_RECT.y + CALC_DISPLAY_RECT.height - text_size.y,
    };
    DrawTextEx(font, calc.output, draw_pos, CALC_DISPLAY_FONT_SIZE, .0f, CALC_THEME_TEXT);
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
            calc_text_size(&r, b, CALC_FUNCTION_BUTTON_FONT_SIZE);
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
            calc_text_size(&r, b, CALC_NUMBER_BUTTON_FONT_SIZE);
        }
    }
}

static inline void calc_text_size(Rectangle *r, Button *b, float size) {
    Vector2 text_size = MeasureTextEx(font, b->text, size, .0f);
    b->text_pos = (Vector2){
        .x = r->x + (r->width  - text_size.x) / 2.0f,
        .y = r->y + (r->height - text_size.y) / 2.0f
    };
    Vector2 alpha_text_size = MeasureTextEx(font, b->alpha_text, CALC_BUTTON_SHIFT_ALPHA_FONT_SIZE, .0f);
    b->alpha_text_pos = (Vector2){
        .x = r->x + r->width - alpha_text_size.x,
        .y = r->y - alpha_text_size.y
    };

    Vector2 shift_text_size = MeasureTextEx(font, b->shift_text, CALC_BUTTON_SHIFT_ALPHA_FONT_SIZE, .0f);
    b->shift_text_pos = (Vector2){
        .x = r->x,
        .y = r->y - shift_text_size.y
    };
}

static inline void draw_function_buttons() {
    for (size_t row = 0; row < CALC_FUNCTION_ROWS; ++row) {
        for (size_t col = 0; col < CALC_FUNCTION_COLS; ++col) {
            Button *b = &func_btns[row][col];
            Rectangle *r = &b->rect;
            Color c = CALC_THEME_CRUST;
            if (CheckCollisionPointRec(GetMousePosition(), *r)) c = CALC_THEME_OVERLAY;
            DrawRectangleRounded(*r, .2f, 0, c);
            Color text_c = TextIsEqual(b->text, "SHIFT") ? YELLOW
                : TextIsEqual(b->text, "ALPHA")          ? RED
                : CALC_THEME_TEXT;
            DrawTextEx(font, b->text, b->text_pos, CALC_FUNCTION_BUTTON_FONT_SIZE, .0f, text_c);
            if (b->alpha_text) DrawTextEx(font, b->alpha_text, b->alpha_text_pos, CALC_BUTTON_SHIFT_ALPHA_FONT_SIZE, .0f, RED);
            if (b->shift_text) DrawTextEx(font, b->shift_text, b->shift_text_pos, CALC_BUTTON_SHIFT_ALPHA_FONT_SIZE, .0f, YELLOW);
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
            if (b->alpha_text) DrawTextEx(font, b->alpha_text, b->alpha_text_pos, CALC_BUTTON_SHIFT_ALPHA_FONT_SIZE, .0f, RED);
            if (b->shift_text) DrawTextEx(font, b->shift_text, b->shift_text_pos, CALC_BUTTON_SHIFT_ALPHA_FONT_SIZE, .0f, YELLOW);
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
                    if (TextLength(calc.input) + TextLength(bn->text) <= CALC_MAX_INPUT_LEN) {
                        TextCopy(calc.input, TextFormat("%s%s", calc.input, bn->text));
                    }
                    break;
                }
            } else if (CheckCollisionPointRec(GetMousePosition(), *rf)) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if (TextLength(calc.input) + TextLength(bf->text) <= CALC_MAX_INPUT_LEN) {
                        TextCopy(calc.input, TextFormat("%s%s", calc.input, bf->text));
                    }
                    break;
                }
            }
        }
    }
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

