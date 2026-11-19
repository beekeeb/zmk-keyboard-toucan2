#include <zephyr/kernel.h>
#include <dt-bindings/zmk/modifiers.h>

#include "modifiers.h"

#define GLYPH_SIZE      16
#define CELL_SIZE       22
#define CELL_PADDING    ((CELL_SIZE - GLYPH_SIZE) / 2)
#define CELL_PITCH      32
#define ROW_X           12      // aligned with the left edge of the style 2 WPM chart
#define ROW_Y           78

struct modifier_glyph {
    zmk_mod_flags_t mask;
    const char *rows[GLYPH_SIZE];
};

// '#' marks a foreground pixel.
static const struct modifier_glyph glyphs[] = {
    {
        .mask = MOD_LCTL | MOD_RCTL, // ^
        .rows = {
            "................",
            "................",
            "................",
            "................",
            ".......##.......",
            "......####......",
            ".....##..##.....",
            "....##....##....",
            "...##......##...",
            "..##........##..",
            ".##..........##.",
            "................",
            "................",
            "................",
            "................",
            "................",
        },
    },
    {
        .mask = MOD_LALT | MOD_RALT, // ⌥
        .rows = {
            "................",
            "................",
            "................",
            ".#####...######.",
            ".#####...######.",
            ".....##.........",
            "......##........",
            ".......##.......",
            "........##......",
            ".........##.....",
            "..........##....",
            "..........#####.",
            "..........#####.",
            "................",
            "................",
            "................",
        },
    },
    {
        .mask = MOD_LGUI | MOD_RGUI, // ⌘
        .rows = {
            "................",
            "...##......##...",
            "..#..#....#..#..",
            ".#...#....#...#.",
            ".#...#....#...#.",
            "..############..",
            ".....#....#.....",
            ".....#....#.....",
            ".....#....#.....",
            ".....#....#.....",
            "..############..",
            ".#...#....#...#.",
            ".#...#....#...#.",
            "..#..#....#..#..",
            "...##......##...",
            "................",
        },
    },
    {
        .mask = MOD_LSFT | MOD_RSFT, // ⇧
        .rows = {
            "................",
            ".......##.......",
            "......#..#......",
            ".....#....#.....",
            "....#......#....",
            "...#........#...",
            "..#..........#..",
            ".#####....#####.",
            ".....#....#.....",
            ".....#....#.....",
            ".....#....#.....",
            ".....#....#.....",
            ".....#....#.....",
            ".....######.....",
            "................",
            "................",
        },
    },
};

static void draw_glyph(lv_obj_t *canvas, const struct modifier_glyph *glyph, lv_coord_t x,
                       lv_coord_t y, lv_color_t color) {
    for (int row = 0; row < GLYPH_SIZE; row++) {
        for (int col = 0; col < GLYPH_SIZE; col++) {
            if (glyph->rows[row][col] == '#') {
                lv_canvas_set_px_color(canvas, x + col, y + row, color);
            }
        }
    }
}

static void draw_indicator(lv_obj_t *canvas, const struct modifier_glyph *glyph, lv_coord_t x,
                           lv_coord_t y, bool active) {
    // Held modifiers are shown inverted: a filled box with the glyph cut out of it.
    if (active) {
        lv_draw_rect_dsc_t rect_dsc;
        init_rect_dsc(&rect_dsc, LVGL_FOREGROUND);
        rect_dsc.radius = 3;
        lv_canvas_draw_rect(canvas, x, y, CELL_SIZE, CELL_SIZE, &rect_dsc);
    }
    draw_glyph(canvas, glyph, x + CELL_PADDING, y + CELL_PADDING,
               active ? LVGL_BACKGROUND : LVGL_FOREGROUND);
}

void draw_modifier_status(lv_obj_t *canvas, const struct status_state *state) {
    for (size_t i = 0; i < ARRAY_SIZE(glyphs); i++) {
        const bool active = (state->mods & glyphs[i].mask) != 0;
        draw_indicator(canvas, &glyphs[i], ROW_X + i * CELL_PITCH, ROW_Y, active);
    }
}
