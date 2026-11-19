#pragma once

#include <lvgl.h>
#include <zmk/hid.h>
#include "util.h"

struct modifiers_status_state {
    zmk_mod_flags_t mods;
};

void draw_modifier_status(lv_obj_t *canvas, const struct status_state *state);
