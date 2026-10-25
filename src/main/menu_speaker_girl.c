/* menu_speaker_girl.c: src/main/8004F870.c; T-27059: object [0x8004F870,0x80053650); one mixed object for menu/choice navigation, speaker labels and talk-text/voice steps, girl name/score/state queries, birthday and club queries; do not split (reuse reviewed m-menutext filename). Original object 0x8004F870-0x80053650 (config/objects/main.txt). */
#include "common.h"
#include "common.h"
#include "game/game_state.h"
#include "game/girl.h"
#include "game/menu_records.h"
#include "game_enums.h"
#include "libapi.h"
#include "main/audio.h"
#include "main/calendar.h"
#include "main/cd.h"
#include "main/cursor.h"
#include "main/girl.h"
#include "main/hud.h"
#include "main/import_views/scenes.h"
#include "main/menu.h"
#include "main/prim.h"
#include "main/speaker.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"
#include "game/menu_records.h"

/* Menu palette colors, data island 0x800E3680..0x800E36BF. */
u32 menu_bar_style[16] = {0};

/* .data range 0x800B40F0..0x800B438F of this object (T-25005: its own section, tools/data_split.py).
 * The template of sndisp's pos[] is D_800B40F8, so the rest of the range follows sndisp. */
INCLUDE_RODATA("asm/data/main/menu_speaker_girl.data", text_row_y);

/* Accessed menu-selection view at GameState +0x1090. */
typedef struct MenuSelection {
    /* 0x00 */ u8 count[2];
    /* 0x02 */ u8 pad_02;
    /* 0x03 */ s8 current[2];
} MenuSelection; /* size 0x05 */

/* Register a menu: item count and rectangles for menu 0 or 1 */
s32 menu_set(menu, count, x, y, w, h)
    s32 menu;
    u8 count;
    s16 *x;
    s16 *y;
    s16 *w;
    s16 *h;
{
    GameState *g;
    s32 i;
    s16 *px;
    s16 *py;
    s16 *pw;
    s16 *ph;

    g = &game_state;
    for (i = 0; i < 16; i++) {
        ((MenuItem (*)[2])g->menu_items)[i][menu].disabled = 0;
        ((MenuItem (*)[2])g->menu_items)[i][menu].h = 0;
        ((MenuItem (*)[2])g->menu_items)[i][menu].w = 0;
    }
    px = x;
    py = y;
    pw = w;
    ph = h;
    if (count >= 17) {
        return -1;
    }
    ((MenuSelection *)&g->menu_count_0)->count[menu] = count;
    g->menu_hit[menu] = -1;
    if (menu >= 2) {
        return -1;
    }
    for (i = 0; i < count; i++) {
        ((MenuItem (*)[2])g->menu_items)[i][menu].x = *px++;
        ((MenuItem (*)[2])g->menu_items)[i][menu].y = *py++;
        if ((*pw * *ph) == 0) {
            ((MenuItem (*)[2])g->menu_items)[i][menu].disabled = 1;
        } else {
            ((MenuItem (*)[2])g->menu_items)[i][menu].disabled = 0;
        }
        ((MenuItem (*)[2])g->menu_items)[i][menu].w = *pw++;
        ((MenuItem (*)[2])g->menu_items)[i][menu].h = *ph++;
        ((MenuItem (*)[2])g->menu_items)[i][menu].hover = 0;
    }
}

/* Hit-test the pointer or move the cursor of a menu by menu_mode; returns the hit index */
s32 menu_check(checked_menu, check_pointer_x, check_pointer_y)
    s32 checked_menu;
    s16 check_pointer_x;
    s16 check_pointer_y;
{
    u8 control_mode;

    if (checked_menu >= 2) {
        return -1;
    }
    control_mode = game_state.menu_mode;
    if ((control_mode & 3) == 0) {
        return menu_check_1(checked_menu, check_pointer_x, check_pointer_y);
    }
    if ((control_mode & 3) == 1) {
        menu_check_2(checked_menu, check_pointer_x, check_pointer_y);
        return (&game_state.menu_cursor_0)[checked_menu];
    }
    if ((control_mode & 3) == 3) {
        menu_check_2(checked_menu, check_pointer_x, check_pointer_y);
        return (&game_state.menu_cursor_0)[checked_menu];
    }
    if (game_state.pad_pressed & PAD_L1) {
        return menu_check_1(checked_menu, check_pointer_x, check_pointer_y);
    }
    if (game_state.pad_held & PAD_L1) {
        return menu_check_2(checked_menu, check_pointer_x, check_pointer_y);
    }
    return menu_check_1(checked_menu, check_pointer_x, check_pointer_y);
}

/* Hit-tests the selected menu against pointer coordinates. */
s32 menu_check_1(hit_test_menu, hit_test_x, hit_test_y)
    s32 hit_test_menu;
    s16 hit_test_x;
    s16 hit_test_y;
{
    GameState *g;
    s32 hit_count;
    s32 hit_item;
    u32 i;

    g = &game_state;
    hit_count = 0;
    hit_item = -1;
    for (i = 0; i < ((MenuSelection *)&g->menu_count_0)->count[hit_test_menu]; i++) {
        ((MenuItem (*)[2])g->menu_items)[i][hit_test_menu].hover = 0;
        if ((s32)((hit_test_x - ((MenuItem (*)[2])g->menu_items)[i][hit_test_menu].x) * ((hit_test_x - ((MenuItem (*)[2])g->menu_items)[i][hit_test_menu].x) - ((MenuItem (*)[2])g->menu_items)[i][hit_test_menu].w)) < 0) {
            if ((s32)((hit_test_y - ((MenuItem (*)[2])g->menu_items)[i][hit_test_menu].y) * ((hit_test_y - ((MenuItem (*)[2])g->menu_items)[i][hit_test_menu].y) - ((MenuItem (*)[2])g->menu_items)[i][hit_test_menu].h)) < 0) {
                ((MenuItem (*)[2])g->menu_items)[i][hit_test_menu].hover = 1;
                hit_count += 1;
                hit_item = i;
            }
        }
    }
    if (hit_count == 0) {
        ((MenuSelection *)&g->menu_count_0)->current[hit_test_menu] = -1;
    }
    if (!(g->pad_held & PAD_L1) && hit_item != -1) {
        g->menu_hit[hit_test_menu] = hit_item;
    }
    return hit_item;
}

/* Updates menu hover state from the current cursor selection. */
s32 menu_check_2(cursor_menu, arg1, arg2)
    s32 cursor_menu;
    s32 arg1;
    s32 arg2;
{
    GameState *g;
    s32 cursor_item;
    u32 i;

    g = &game_state;
    cursor_item = -1;
    g->menu_hit[cursor_menu] = -1;
    i = 0;
    if (((MenuSelection *)&g->menu_count_0)->count[cursor_menu] != 0) {
        do {
            if (i != ((MenuSelection *)&g->menu_count_0)->current[cursor_menu]) {
                ((MenuItem (*)[2])g->menu_items)[i][cursor_menu].hover = 0;
            } else {
                ((MenuItem (*)[2])g->menu_items)[i][cursor_menu].hover = 10;
                cursor_item = i;
            }
            i += 1;
        } while (i < ((MenuSelection *)&g->menu_count_0)->count[cursor_menu]);
    }
    if (!(g->pad_held & PAD_L1) && cursor_item != -1) {
        g->menu_hit[cursor_menu] = cursor_item;
    }
}

void menu_cursor_update(s32 menu) {
    GameState *g;
    s32 direction;
    s32 skip_budget;
    s32 i;

    g = &game_state;
    direction = 0;
    skip_budget = 16;
    if (g->pad_pressed & PAD_SELECT) {
        g->menu_mode++;
        ((MenuSelection *)&g->menu_count_0)->current[menu] = -1;
        return;
    }
    switch (g->menu_mode & 3) {
    case 0:
        hud_cursor_update();
        for (i = 0; i < ((MenuSelection *)&g->menu_count_0)->count[menu]; i++) {
            if (((MenuItem (*)[2])g->menu_items)[i][menu].hover != 0) {
                ((MenuSelection *)&g->menu_count_0)->current[menu] = i;
                break;
            }
        }
        break;
    case 1:
        /* FAKE: empty then-arm; the target branches to the wrap tests from the fall-through, an inverted
         * test gives a different branch shape. Real source unknown. T-25101 */
        if (g->pad_held & 0xF) {
        } else {
            switch (g->pad_pressed & 0xF000) {
            case 0x1000:
            case 0x8000:
                direction = -1;
                ((MenuSelection *)&g->menu_count_0)->current[menu] -= 1;
                break;
            case 0x2000:
            case 0x4000:
                direction = 1;
                ((MenuSelection *)&g->menu_count_0)->current[menu] += 1;
                break;
            }
        }
        if (((MenuSelection *)&g->menu_count_0)->current[menu] >= (s32)((MenuSelection *)&g->menu_count_0)->count[menu]) {
            ((MenuSelection *)&g->menu_count_0)->current[menu] = 0;
        }
        if (((MenuSelection *)&g->menu_count_0)->current[menu] < 0) {
            ((MenuSelection *)&g->menu_count_0)->current[menu] = ((MenuSelection *)&g->menu_count_0)->count[menu] - 1;
        }
        while (((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].disabled == 1) {
            if (skip_budget-- == 0) {
                break;
            }
            if (direction == 1) {
                ((MenuSelection *)&g->menu_count_0)->current[menu] += 1;
            }
            if (direction == -1) {
                ((MenuSelection *)&g->menu_count_0)->current[menu] -= 1;
            }
            if (((MenuSelection *)&g->menu_count_0)->current[menu] >= (s32)((MenuSelection *)&g->menu_count_0)->count[menu]) {
                ((MenuSelection *)&g->menu_count_0)->current[menu] = 0;
            }
            if (((MenuSelection *)&g->menu_count_0)->current[menu] < 0) {
                ((MenuSelection *)&g->menu_count_0)->current[menu] = ((MenuSelection *)&g->menu_count_0)->count[menu] - 1;
            }
        }
        cursor_x = ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].x + ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].w - 4;
        cursor_y = ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].y + 8;
        break;
    case 2:
        if (g->pad_held & PAD_L1) {
            if (g->pad_pressed & PAD_L1) {
                ((MenuSelection *)&g->menu_count_0)->current[menu] = g->menu_hit[menu];
            } else {
                switch (g->pad_pressed & 0xF000) {
                case 0x2000:
                case 0x4000:
                    direction = 1;
                    ((MenuSelection *)&g->menu_count_0)->current[menu] += 1;
                    break;
                case 0x1000:
                case 0x8000:
                    direction = -1;
                    ((MenuSelection *)&g->menu_count_0)->current[menu] -= 1;
                    break;
                }
                if (((MenuSelection *)&g->menu_count_0)->current[menu] >= (s32)((MenuSelection *)&g->menu_count_0)->count[menu]) {
                    ((MenuSelection *)&g->menu_count_0)->current[menu] = 0;
                }
                if (((MenuSelection *)&g->menu_count_0)->current[menu] < 0) {
                    ((MenuSelection *)&g->menu_count_0)->current[menu] = ((MenuSelection *)&g->menu_count_0)->count[menu] - 1;
                }
                while (((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].disabled == 1) {
                    if (skip_budget-- == 0) {
                        break;
                    }
                    if (direction == 1) {
                        ((MenuSelection *)&g->menu_count_0)->current[menu] += 1;
                    }
                    if (direction == -1) {
                        ((MenuSelection *)&g->menu_count_0)->current[menu] -= 1;
                    }
                    if (((MenuSelection *)&g->menu_count_0)->current[menu] >= (s32)((MenuSelection *)&g->menu_count_0)->count[menu]) {
                        ((MenuSelection *)&g->menu_count_0)->current[menu] = 0;
                    }
                    if (((MenuSelection *)&g->menu_count_0)->current[menu] < 0) {
                        ((MenuSelection *)&g->menu_count_0)->current[menu] = ((MenuSelection *)&g->menu_count_0)->count[menu] - 1;
                    }
                }
            }
            cursor_x = ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].x + ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].w - 4;
            cursor_y = ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].y + 8;
        } else {
            hud_cursor_update();
            for (i = 0; i < ((MenuSelection *)&g->menu_count_0)->count[menu]; i++) {
                if (((MenuItem (*)[2])g->menu_items)[i][menu].hover != 0) {
                    ((MenuSelection *)&g->menu_count_0)->current[menu] = i;
                }
            }
        }
        break;
    case 3:
        i = -1;
        switch (g->pad_pressed & 0xF000) {
        case 0x1000:
            i = get_near_menu(1, -1, menu);
            break;
        case 0x4000:
            i = get_near_menu(1, 1, menu);
            break;
        case 0x2000:
            i = get_near_menu(0, 1, menu);
            break;
        case 0x8000:
            i = get_near_menu(0, -1, menu);
            break;
        }
        if (i != -1) {
            ((MenuSelection *)&g->menu_count_0)->current[menu] = i;
        }
        if (((MenuSelection *)&g->menu_count_0)->current[menu] >= (s32)((MenuSelection *)&g->menu_count_0)->count[menu]) {
            ((MenuSelection *)&g->menu_count_0)->current[menu] = menu_cursor_to_edge(menu, 1);
        }
        if (((MenuSelection *)&g->menu_count_0)->current[menu] < 0) {
            ((MenuSelection *)&g->menu_count_0)->current[menu] = menu_cursor_to_edge(menu, 0);
        }
        cursor_x = ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].x + ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].w - 4;
        cursor_y = ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].y + 8;
        break;
    }
    if (cursor_x > 140) {
        cursor_x = 140;
    }
    if (cursor_x < -150) {
        cursor_x = -150;
    }
    if (cursor_y < -100) {
        cursor_y = -100;
    }
    if (cursor_y > 100) {
        cursor_y = 100;
    }
}


s32 menu_cursor_to_edge(s32 menu, s32 reverse) {
    GameState *g;
    s32 i;
    /* The menu entries are interleaved: one record for each of two menus. */
    g = &game_state;
    if (reverse != 0) {
        for (i = ((MenuSelection *)&g->menu_count_0)->count[menu] - 1; i >= 0; i--) {
            if (((MenuItem (*)[2])g->menu_items)[i][menu].w && ((MenuItem (*)[2])g->menu_items)[i][menu].h) {
                ((MenuSelection *)&g->menu_count_0)->current[menu] = i;
                return i;
            }
        }
    } else {
        for (i = 0; (u32)i < ((MenuSelection *)&g->menu_count_0)->count[menu]; i++) {
            if (((MenuItem (*)[2])g->menu_items)[i][menu].w && ((MenuItem (*)[2])g->menu_items)[i][menu].h) {
                ((MenuSelection *)&g->menu_count_0)->current[menu] = i;
                return i;
            }
        }
    }
    return -1;
}

/* Finds the nearest menu item in the requested direction. */
s32 get_near_menu(s32 axis, s32 direction, s32 menu) {
    GameState *g;
    s32 item_count;
    s16 best_distance;
    s32 best_item;
    s16 axis_distances[16];
    s32 i;

    g = &game_state;
    item_count = 16;
    best_distance = 320;
    if (((MenuSelection *)&g->menu_count_0)->count[menu] < 16) {
        item_count = ((MenuSelection *)&g->menu_count_0)->count[menu];
    }
    switch (axis) {
    case 0:
        for (i = 0; i < item_count; i++) {
            axis_distances[i] = ((MenuItem (*)[2])g->menu_items)[i][menu].x - ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].x;
        }
        break;
    case 1:
        for (i = 0; i < item_count; i++) {
            axis_distances[i] = ((MenuItem (*)[2])g->menu_items)[i][menu].y - ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].y;
        }
        break;
    }
    switch (direction) {
    case -1:
        for (i = 0; i < item_count; i++) {
            if (axis_distances[i] > 0) {
                axis_distances[i] = 0;
            } else {
                axis_distances[i] = -axis_distances[i];
            }
        }
        break;
    case 1:
        for (i = 0; i < item_count; i++) {
            if (axis_distances[i] < 0) {
                axis_distances[i] = 0;
            }
        }
        break;
    }
    best_item = -1;
    for (i = 0; i < item_count; i++) {
        if (((MenuItem (*)[2])g->menu_items)[i][menu].disabled == 0 && axis_distances[i] != 0) {
            if (axis == 0 && ((MenuItem (*)[2])g->menu_items)[i][menu].y == ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].y) {
                if (best_distance > 0) {
                    best_distance = -320;
                }
                if (best_distance < -abs(((MenuItem (*)[2])g->menu_items)[i][menu].x - ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].x)) {
                    best_distance = -abs(((MenuItem (*)[2])g->menu_items)[i][menu].x - ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].x);
                    best_item = i;
                }
            } else if (axis == 1 && ((MenuItem (*)[2])g->menu_items)[i][menu].x == ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].x) {
                if (best_distance > 0) {
                    best_distance = -320;
                }
                if (best_distance < -abs(((MenuItem (*)[2])g->menu_items)[i][menu].y - ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].y)) {
                    best_distance = -abs(((MenuItem (*)[2])g->menu_items)[i][menu].y - ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].y);
                    best_item = i;
                }
            } else if (best_distance > 0) {
                if (axis_distances[i] < best_distance) {
                    best_distance = axis_distances[i];
                    best_item = i;
                } else if (best_distance == axis_distances[i]) {
                    switch (axis) {
                    case 0:
                        if (abs(((MenuItem (*)[2])g->menu_items)[best_item][menu].y - ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].y) > abs(((MenuItem (*)[2])g->menu_items)[i][menu].y - ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].y)) {
                            best_item = i;
                        }
                        break;
                    case 1:
                        if (abs(((MenuItem (*)[2])g->menu_items)[best_item][menu].x - ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].x) > abs(((MenuItem (*)[2])g->menu_items)[i][menu].x - ((MenuItem (*)[2])g->menu_items)[((MenuSelection *)&g->menu_count_0)->current[menu]][menu].x)) {
                            best_item = i;
                        }
                        break;
                    }
                }
            }
        }
    }
    /* FAKE: redundant test (best_item is -1 or an index); it gives bltz with `li v0,-1` in the delay slot as in the target. T-25101 */
    if (best_item >= 0) {
        return best_item;
    }
    return -1;
}

/* Encodes menu-bar ordering, color and style flags for a menu item. */
void menu_bar_color(s32 menu_index, s32 rgb_color, s32 ordering_slot, s32 style_flags) {
    if (menu_index >= 0x10) {
        menu_bar_style[0] = 0x832F7FAF;
        menu_bar_style[1] = 0x822F7F2F;
        return;
    }
    /* FAKE: redundant nibble mask preserves the original temporary numbering. T-24501 */
    menu_bar_style[menu_index] = ((ordering_slot & 0xF) << 0x18) + rgb_color + ((style_flags & 0xF) << 0x1C);
}

/* Draw the highlight bar of the hovered menu item */
void menu_bar_show(menu)
    s32 menu;
{
    s32 box_style;
    s32 ordering_slot;
    s32 transparency_flag;
    s8 cursor_item;
    ordering_slot = (menu_bar_style[menu] >> 24) & 15;
    box_style = (menu_bar_style[menu] >> 28) & 7;
    transparency_flag = (menu_bar_style[menu] >> 31) << 7;
    cursor_item = ((MenuSelection *)&game_state.menu_count_0)->current[menu];
    if (cursor_item >= 0 && (u32)cursor_item < ((MenuSelection *)&game_state.menu_count_0)->count[menu]) {
        prim_gradient_rect(((MenuItem (*)[2])game_state.menu_items)[cursor_item][menu].x, ((MenuItem (*)[2])game_state.menu_items)[cursor_item][menu].y, ((MenuItem (*)[2])game_state.menu_items)[cursor_item][menu].w, ((MenuItem (*)[2])game_state.menu_items)[cursor_item][menu].h, ordering_slot, menu_bar_style[menu] & 0xFFFFFF, transparency_flag | box_style);
        dtd_on_tpage(0, 0, ordering_slot, 0, 1);
    }
}

/* Configures the centered two- or three-choice menu rows. */
void x_taku_menu_set(s32 menu, s32 count)
{
    s32 pad; /* FAKE: unused local shifts array homes down by 8 bytes within the 0x70 frame. T-24111 */
    s32 i;
    s16 x[4];
    s16 width[4];
    s16 height[4];

    for (i = 0; i < count; i++) {
        x[i] = -120;
        width[i] = 240;
        height[i] = 16;
    }
    if (count == 3) {
        menu_set(menu, 3, x, text_row_y, width, height);
        return;
    }
    menu_set(menu, 2, x, text_row_y, width, height);
}

/* Places up to three choice labels in the centered choice menu. */
void x_taku_string_set(s32 first_label, s32 second_label, s32 third_label, s32 choice_count) {
    set_kanji_string(-0x78, 0x32, 0, first_label, 0);
    set_kanji_string(-0x78, 0x42, 0, second_label, 0);
    if (choice_count == 3) {
        if (third_label != 0) {
            set_kanji_string(-0x78, 0x52, 0, third_label, 0);
        }
    }
}

/* Draw a name string centered by its length (2, 4 or 6 bytes) */
void gnsx(u8 *speaker_name) {
    switch (strlen(speaker_name)) {
    case 2:
        set_kanji_string(-0x72, 0x32, 0, speaker_name, 0);
        return;
    case 4:
        set_kanji_string(-0x79, 0x32, 0, speaker_name, 0);
        return;
    default:
    case 6:
        set_kanji_string(-0x80, 0x32, 0, speaker_name, 0);
        return;
    }
}

/* Wrap and queue a narration string at row y with color col */
void sndisp(u8 *text, s32 first_row, s32 speaker_style) {
    s32 pad[2]; /* FAKE: T-25001, two unused home words at the top of the 0x168 frame */
    s32 i;
    s32 byte_phase;
    s32 line_break_count;
    u8 line_break_offsets[5] = { 0, 0, 0, 0, 0 };
    u8 block_text[256];

    strncpy(block_text, text, 256);
    byte_phase = 0;
    line_break_count = 0;
    for (i = 0; block_text[i] != 0; i++, byte_phase++) {
        if (block_text[i] == ' ' && !(byte_phase & 1)) {
            line_break_offsets[line_break_count] = i;
            block_text[i] = 0;
            byte_phase = -1;
            line_break_count++;
        }
        if (block_text[i] == 'P' && !(byte_phase & 1)) {
            byte_phase = -1;
        }
        if (block_text[i] == 'G' && !(byte_phase & 1)) {
            byte_phase = -1;
        }
    }
    game_state.text_first_line = check_set_k();
    sndi(block_text, first_row, speaker_style);
    for (i = 0; i < line_break_count; i++) {
        sndi(&block_text[line_break_offsets[i]] + 1, first_row + 1 + i, speaker_style);
    }
    game_state.text_lines_total = check_set_k();
    game_state.text_scroll_state = 0x80;
}

/* rest of the 0x800B40F0 range, after the template of sndisp */
INCLUDE_RODATA("asm/data/main/menu_speaker_girl.data", girl_tokimeki_weight);
INCLUDE_RODATA("asm/data/main/menu_speaker_girl.data", girl_bonus_param_mask);
INCLUDE_RODATA("asm/data/main/menu_speaker_girl.data", girl_param_ladder_kind);
INCLUDE_RODATA("asm/data/main/menu_speaker_girl.data", girl_param_bonus);
INCLUDE_RODATA("asm/data/main/menu_speaker_girl.data", girl_param_ladder_base);
INCLUDE_RODATA("asm/data/main/menu_speaker_girl.data", D_800B419C);
INCLUDE_RODATA("asm/data/main/menu_speaker_girl.data", girl_affection_weight);
INCLUDE_RODATA("asm/data/main/menu_speaker_girl.data", girl_closeness_grid);
INCLUDE_RODATA("asm/data/main/menu_speaker_girl.data", girl_syoushin_weight);
INCLUDE_RODATA("asm/data/main/menu_speaker_girl.data", girl_points_mask_strong);
INCLUDE_RODATA("asm/data/main/menu_speaker_girl.data", girl_points_mask_normal);
INCLUDE_RODATA("asm/data/main/menu_speaker_girl.data", girl_points_param_curve);
INCLUDE_RODATA("asm/data/main/menu_speaker_girl.data", girl_points_stress_curve);

/* Queue a narration string with the speaker name codes expanded */
void sndi(u8 *text, s32 row, s32 speaker_style) {
    u8 *scan_text;
    u8 original[64];
    /* FAKE: the sizes below 64 are inferred from the target'scan_text frame offsets (each buffer ends
     * where the next one starts); the real declarations are unknown. T-25101 */
    u8 decorated_text[40];
    u8 player_expanded_text[40];
    u8 expanded_text[40];
    u8 expanded_name[20];
    u8 *name_code_at;
    s32 i;
    s32 name_code_found;

    strncpy(original, text, 64);
    game_state.text_scroll_state = 0;
    name_code_found = 0;
    if ((speaker_style & 15) == 15) {
        for (i = 0, scan_text = original; scan_text[i] != 0; i++) {
            name_code_at = scan_text + i;
            if (*name_code_at == 'P' && (i & 1) == 0) {
                *name_code_at = 0;
                name_code_found = 1;
                strcpy(player_expanded_text, original);
                get_p_name(expanded_name, speaker_style & 15);
                strcat(player_expanded_text, expanded_name);
                strcat(player_expanded_text, name_code_at + 1);
                break;
            }
        }
        if (name_code_found == 0) {
            strcpy(player_expanded_text, original);
        }
        name_code_found = 0;
        for (i = 0, scan_text = player_expanded_text; scan_text[i] != 0; i++) {
            name_code_at = scan_text + i;
            if (*name_code_at == 'G' && (i & 1) == 0) {
                *name_code_at = 0;
                name_code_found = 1;
                strcpy(expanded_text, player_expanded_text);
                get_g_name(expanded_name, game_state.current_girl);
                strcat(expanded_text, expanded_name);
                strcat(expanded_text, name_code_at + 1);
                break;
            }
        }
        if (name_code_found == 0) {
            strcpy(expanded_text, player_expanded_text);
        }
    } else {
        for (i = 0, scan_text = original; scan_text[i] != 0; i++) {
            name_code_at = scan_text + i;
            if (*name_code_at == 'P' && (i & 1) == 0) {
                *name_code_at = 0;
                name_code_found = 1;
                strcpy(expanded_text, original);
                get_p_name(expanded_name, game_state.current_girl);
                strcat(expanded_text, expanded_name);
                strcat(expanded_text, name_code_at + 1);
                break;
            }
        }
        if (name_code_found == 0) {
            strcpy(expanded_text, original);
        }
    }
    switch (row) {
    case 0:
        if (speaker_style == 0xF) {
            strcpy(decorated_text, "『");
            strcat(decorated_text, expanded_text);
        } else if (speaker_style == 0x1F) {
            strcpy(decorated_text, "（");
            strcat(decorated_text, expanded_text);
        } else if (speaker_style == 0x2F) {
            strcpy(decorated_text, expanded_text);
        } else {
            strcpy(decorated_text, "「");
            strcat(decorated_text, expanded_text);
        }
        if (speaker_style == 0xFF) {
            set_kanji_string(-0x80, 0x32, 0, decorated_text, 0);
            return;
        }
        set_kanji_string(-0x56, 0x32, 0, decorated_text, 0);
        return;
    case 1:
        if (speaker_style == 0xFF) {
            set_kanji_string(-0x80, 0x42, 0, expanded_text, 0);
            return;
        }
        set_kanji_string(-0x48, 0x42, 0, expanded_text, 0);
        return;
    default:
    case 2:
        if (speaker_style == 0xFF) {
            set_kanji_string(-0x80, 0x52, 0, expanded_text, 0);
            return;
        }
        set_kanji_string(-0x48, 0x52, 0, expanded_text, 0);
        return;
    }
}

/* Stores the supplied character ID in the current girl slot. */
void set_c_girl(u8 girl_id) {
    game_state.current_girl = girl_id;
}

/* Copies the character name form selected by the supplied girl ID. */
s32 get_g_name(u8 *name_buf, s32 girl_id) {
    if (girl_id == 0) {
        strcpy(name_buf, "詩織");
    } else if (girl_id == 0xA) {
        strcpy(name_buf, girl_short_name_yumi_given);
        strcat(name_buf, "ちゃん");
    } else if (girl_id == 0xC) {
        strcpy(name_buf, girl_short_name_rei);
    } else {
        strcpy(name_buf, girl_short_name_table[girl_id & 0xF]);
        strcat(name_buf, "さん");
    }
    if (girl_id >= 0x10 || girl_id < 0) {
        return -1;
    }
    return 1;
}

/* Builds the player-name form used by the selected character to address the player. */
void get_p_name(u8 *name_buf, u32 girl_form) {
    switch (girl_form) {
    case 0:
        if (get_g_zyotai_s((s32) girl_form) & 0x80) {
            strcpy(name_buf, &game_state.player_nickname[0]);
            return;
        }
        strcpy(name_buf, &game_state.player_given_name[0]);
        strcat(name_buf, &girl_name_suffix_familiar);
        return;
    case 1:
        if (get_g_zyotai_s((s32) girl_form) & 0x80) {
            strcpy(name_buf, &game_state.player_family_name[0]);
            strcat(name_buf, &girl_name_suffix_familiar);
            return;
        }
        strcpy(name_buf, &game_state.player_family_name[0]);
        strcat(name_buf, &girl_name_suffix_polite);
        return;
    case 2:
        if (get_g_zyotai_s((s32) girl_form) & 0x80) {
            strcpy(name_buf, &game_state.player_given_name[0]);
            strcat(name_buf, &girl_name_suffix_familiar);
            return;
        }
        strcpy(name_buf, &game_state.player_family_name[0]);
        strcat(name_buf, &girl_name_suffix_familiar);
        return;
    case 3:
        if (get_g_zyotai_s((s32) girl_form) & 0x80) {
            strcpy(name_buf, &game_state.player_nickname[0]);
            return;
        }
        strcpy(name_buf, &game_state.player_family_name[0]);
        strcat(name_buf, &girl_name_suffix_familiar);
        return;
    case 4:
        if (get_g_zyotai_s((s32) girl_form) & 0x80) {
            strcpy(name_buf, &game_state.player_given_name[0]);
            strcat(name_buf, &girl_name_suffix_familiar);
            return;
        }
        if ((get_g_zyotai_s((s32) girl_form) & 0x7F) == 4) {
            strcpy(name_buf, &game_state.player_family_name[0]);
            strcat(name_buf, &girl_name_suffix_familiar);
            return;
        }
        strcpy(name_buf, &game_state.player_family_name[0]);
        strcat(name_buf, &girl_name_suffix_familiar);
        return;
    case 5:
        if (get_g_zyotai_s((s32) girl_form) & 0x80) {
            strcpy(name_buf, &game_state.player_given_name[0]);
            strcat(name_buf, &girl_name_suffix_polite);
            return;
        }
        if ((get_g_zyotai_s((s32) girl_form) & 0x7F) == 4) {
            strcpy(name_buf, &game_state.player_family_name[0]);
            strcat(name_buf, &girl_name_suffix_polite);
            return;
        }
        strcpy(name_buf, &game_state.player_family_name[0]);
        strcat(name_buf, &girl_name_suffix_polite);
        return;
    case 6:
        if (get_g_zyotai_s((s32) girl_form) & 0x80) {
            strcpy(name_buf, &game_state.player_nickname[0]);
            return;
        }
        strcpy(name_buf, &game_state.player_family_name[0]);
        strcat(name_buf, &girl_name_suffix_familiar);
        return;
    case 7:
        if (get_g_zyotai_s((s32) girl_form) & 0x80) {
            strcpy(name_buf, &game_state.player_given_name[0]);
            strcat(name_buf, &girl_name_suffix_familiar);
            return;
        }
        strcpy(name_buf, &game_state.player_family_name[0]);
        strcat(name_buf, &girl_name_suffix_familiar);
        return;
    case 8:
        if (get_g_zyotai_s((s32) girl_form) & 0x80) {
            strcpy(name_buf, &game_state.player_nickname[0]);
            return;
        }
        strcpy(name_buf, &game_state.player_given_name[0]);
        strcat(name_buf, &girl_name_suffix_familiar);
        return;
    case 9:
        if (get_g_zyotai_s((s32) girl_form) & 0x80) {
            strcpy(name_buf, &game_state.player_family_name[0]);
            strcat(name_buf, &girl_name_suffix_familiar);
            return;
        }
        strcpy(name_buf, &game_state.player_family_name[0]);
        strcat(name_buf, &girl_name_suffix_polite);
        return;
    case 10:
        if (get_g_zyotai_s((s32) girl_form) & 0x80) {
            strcpy(name_buf, &game_state.player_given_name[0]);
            strcat(name_buf, &girl_name_suffix_polite);
            return;
        }
        strcpy(name_buf, &game_state.player_family_name[0]);
        strcat(name_buf, &girl_name_suffix_polite);
        return;
    case 13:
        strcpy(name_buf, &game_state.player_given_name[0]);
        return;
    case 11:
        strcpy(name_buf, &game_state.player_given_name[0]);
        strcat(name_buf, &girl_name_suffix_familiar);
        return;
    case 12:
        strcpy(name_buf, &game_state.player_family_name[0]);
        strcat(name_buf, &girl_name_suffix_familiar);
        return;
    case 15:
        strcpy(name_buf, &game_state.player_family_name[0]);
        strncat(name_buf, &game_state.player_given_name[0], 6);
        return;
    default:
        strcpy(name_buf, &game_state.player_family_name[0]);
        strcat(name_buf, &girl_name_suffix_familiar);
        return;
    }
}

/* Girl state rank 0-4 for scenes (4 worst); bit 7 set when affection is high */
s32 get_g_zyotai_s(s32 girl_id) {
    if (get_h_yuukou(girl_id) < 0xA) {
        return 4;
    }
    if (get_h_yuukou(girl_id) < 0x1E) {
        return 3;
    }
    if (get_h_yuukou(girl_id) < 0x32) {
        if (get_h_tokimeki(girl_id) < 0x32) {
            return 3;
        }
        return 1;
    }
    if (get_h_yuukou(girl_id) < 0x50) {
        if (get_h_tokimeki(girl_id) < 0x32) {
            return 2;
        }
        return 1;
    }
    if (get_h_tokimeki(girl_id) < 0x32) {
        return 0x82;
    }
    return 0x80;
}

/* Girl state rank 0-4 from weighted affection and tokimeki (4 if affection < 10, 3 if < 50, 2 if tokimeki < 50, 1 if
 * < 80, else 0) */
s32 get_g_zyotai_h(s32 girl_id) {
    if (get_h_yuukou(girl_id) < 0xA) {
        return 4;
    }
    if (get_h_yuukou(girl_id) < 0x32) {
        return 3;
    }
    if (get_h_tokimeki(girl_id) < 0x32) {
        return 2;
    }
    if (get_h_tokimeki(girl_id) < 0x50) {
        return 1;
    }
    return 0;
}

/* Populates a menu with eligible heroine entries and their positions. */
void menu_girl_taku_set(s32 menu, s32 *last_text_entry_count)
{
    s32 i;
    s32 girl_id;
    GirlRecord *girl_record;
    s16 x[12];
    s16 y[12];
    s16 w[12];
    s16 h[12];

    i = 0;
    do {
        if (i == 11) {
            girl_id = 12;
        } else {
            girl_id = i;
        }
        girl_record = &game_state.girls[girl_id];
        if (girl_record->flags.f.met) {
            *last_text_entry_count = set_kanji_string((i / 3) * 64 - 0x80, (i % 3) * 16 + 0x32, 0, girl_short_name_table[girl_id], 0);
        }
        x[i] = (i / 3) * 64 - 0x80;
        y[i] = (i % 3) * 16 + 0x32;
        if (girl_record->flags.f.met) {
            w[i] = 0x30;
        } else {
            w[i] = 0;
        }
        h[i] = 0x10;
        menu_set(menu, 12, x, y, w, h);
        i++;
    } while (i != 12);
}

/* Typewriter plus voice wait step used by dialogue lines */
void xa_wait(void) {
    /* FAKE: two unused home words above the spills; the original frame is 8 bytes larger. T-17018 */
    s32 pad[2];
    s32 text_complete;
    u8 *middle_entry;
    u8 *bottom_entry;
    s32 revealed_entry;
    s32 scroll_phase;
    u32 first_visible_entry;

    text_complete = 0;
    revealed_entry = get_now_k();
    if (game_state.text_scroll_state & 0x80) {
        scroll_phase = game_state.text_scroll_state & 0x7F;
        if (game_state.text_first_line + 2 >= (u32)revealed_entry) {
            text_complete = k_disp_inc();
            revealed_entry = get_now_k();
            first_visible_entry = game_state.text_first_line;
            if (revealed_entry == first_visible_entry + 3) {
                if (game_state.text_lines_total >= first_visible_entry + 3) {
                    k_disp_switch(revealed_entry, 0);
                }
            }
        } else if (scroll_phase == 0) {
            k_disp_switch(game_state.text_first_line, 0);
            middle_entry = get_k_work(game_state.text_first_line + 1);
            bottom_entry = get_k_work(game_state.text_first_line + 2);
            *(s16 *)(middle_entry + 2) -= 2;
            *(s16 *)(bottom_entry + 2) -= 2;
            game_state.text_scroll_state += 1;
        } else if (scroll_phase != 0) {
            /* FAKE: redundant test; the second use makes scroll_phase middle_entry uopt variable in $v0 as in the original (middle_entry plain else gives middle_entry $t9 temporary). T-17018 */
            middle_entry = get_k_work(game_state.text_first_line + 1);
            bottom_entry = get_k_work(game_state.text_first_line + 2);
            *(s16 *)(middle_entry + 2) -= 2;
            *(s16 *)(bottom_entry + 2) -= 2;
            if (*(s16 *)(middle_entry + 2) < 0x33) {
                *(s16 *)(middle_entry + 2) = 0x32;
                *(s16 *)(bottom_entry + 2) = 0x42;
                game_state.text_scroll_state -= 1;
                game_state.text_first_line += 1;
                k_disp_switch(game_state.text_first_line + 2, 1);
            }
        }
    } else {
        text_complete = k_disp_inc();
        get_now_k();
    }
    if (text_complete == 1) {
        if (D_8011ED5B & 0x80) {
            hud_text_marker_bob();
        } else {
            hud_text_marker_set(1);
        }
    }
    if (game_state.pad_pressed & 0x60) {
        if (text_complete == 1) {
            hud_text_marker_set(0);
            game_state.substep += 1;
            game_state.text_scroll_state = 0;
            return;
        }
        k_disp_goto_line_end();
    }
}

void text_wait_confirm(void) {
    k_disp_inc();
    if (game_state.pad_pressed & 0x860) {
        k_disp_goto_line_end();
        if (check_end_k() != 0) {
            game_state.substep += 1;
        }
    }
}

void talk_voice_play(s32 voice_source_a, s32 voice_source_b, s32 clip_index, s32 voice_selector) {
    s32 speaker_code;

    if (clip_index == -1) {
        speaker_code = voice_selector & 15;
        if (speaker_code != 15) {
            if ((u8)game_state.voice_mode != VOICE_OFF) {
                cd_audio_request_set(voice_source_a, voice_source_b, voice_selector);
                audio_command(0x300);
            }
        }
    } else {
        speaker_code = voice_selector & 15;
        if (speaker_code != 15) {
            if ((u8)game_state.voice_mode != VOICE_OFF) {
                cd_audio_request_set(((s32 *)voice_source_a)[clip_index * 2], ((s32 *)voice_source_a)[clip_index * 2 + 1], voice_selector);
                audio_command(0x300);
            }
        }
    }
    speaker_voice_source_a = voice_source_a;
    speaker_voice_source_b = voice_source_b;
    speaker_voice_index = clip_index;
    if (speaker_code == 15) {
        speaker_voice_state = SPEAKER_VOICE_DONE;
        return;
    }
    speaker_voice_state = SPEAKER_VOICE_PENDING;
}

void talk_text_step(void) {
    /* FAKE: two unused home words above the spills; the original frame is 8 bytes larger, as in xa_wait. T-25101 */
    s32 pad[2];
    s32 text_complete;
    u8 *middle_entry;
    u8 *bottom_entry;
    s32 *cd_request_words;
    s32 revealed_entry;
    s32 scroll_phase;
    u32 first_visible_entry;

    text_complete = 0;
    revealed_entry = get_now_k();
    if (game_state.text_scroll_state & 0x80) {
        scroll_phase = game_state.text_scroll_state & 0x7F;
        if (game_state.text_first_line + 2 >= (u32)revealed_entry) {
            text_complete = k_disp_inc();
            revealed_entry = get_now_k();
            first_visible_entry = game_state.text_first_line;
            if (revealed_entry == first_visible_entry + 3) {
                if (game_state.text_lines_total >= first_visible_entry + 3) {
                    k_disp_switch(revealed_entry, 0);
                }
            }
        } else if (scroll_phase == 0) {
            /* FAKE: `+ 0` keeps this argument off the F61 range, which then has no a0 preference and takes v1 (F62 a0) as in the original; the plain argument gives a2. T-25101 */
            k_disp_switch(game_state.text_first_line + 0, 0);
            middle_entry = get_k_work(game_state.text_first_line + 1);
            bottom_entry = get_k_work(game_state.text_first_line + 2);
            *(s16 *)(middle_entry + 2) -= 2;
            *(s16 *)(bottom_entry + 2) -= 2;
            game_state.text_scroll_state += 1;
        } else if (scroll_phase != 0) {
            /* FAKE: redundant test, as in xa_wait; the second use keeps scroll_phase in $v0. T-25101 */
            middle_entry = get_k_work(game_state.text_first_line + 1);
            bottom_entry = get_k_work(game_state.text_first_line + 2);
            *(s16 *)(middle_entry + 2) -= 2;
            *(s16 *)(bottom_entry + 2) -= 2;
            if (*(s16 *)(middle_entry + 2) < 0x33) {
                *(s16 *)(middle_entry + 2) = 0x32;
                *(s16 *)(bottom_entry + 2) = 0x42;
                game_state.text_scroll_state -= 1;
                game_state.text_first_line += 1;
                k_disp_switch(game_state.text_first_line + 2, 1);
            }
        }
    } else {
        /* FAKE: the s8 mode field is read through middle_entry u8 address view: lbu, and uopt keeps the byte as one promoted global
         * reloaded after each call; `(u8)game_state.voice_mode` adds conversion ranges that cache it. T-25101 */
        if (((*(u8 *)&game_state.voice_mode) == 1 && speaker_voice_state != SPEAKER_VOICE_PENDING) || ((*(u8 *)&game_state.voice_mode) == 2 && speaker_voice_state != SPEAKER_VOICE_PENDING)) {
            text_complete = k_disp_inc();
        }
        if ((*(u8 *)&game_state.voice_mode) == 0) {
            text_complete = k_disp_inc();
        }
        get_now_k();
    }
    if (speaker_voice_state == SPEAKER_VOICE_PENDING && (cd_audio_state() & 4)) {
        speaker_voice_state = SPEAKER_VOICE_PLAYING;
    }
    cd_request_words = (s32 *)cd_audio_request_get();
    if (speaker_voice_state == SPEAKER_VOICE_PLAYING) {
        if (cd_audio_state() & 2) {
            if (speaker_voice_index != -1) {
                if (((s32 *)speaker_voice_source_a)[speaker_voice_index * 2] != ((s32 *)speaker_voice_source_b)[speaker_voice_index * 2] && ((s32 *)speaker_voice_source_a)[speaker_voice_index * 2] == cd_request_words[2]) {
                    cd_audio_request_set(((s32 *)speaker_voice_source_b)[speaker_voice_index * 2], ((s32 *)speaker_voice_source_b)[speaker_voice_index * 2 + 1], game_state.current_girl);
                    audio_command(0x300);
                    speaker_voice_state = SPEAKER_VOICE_PENDING;
                } else {
                    speaker_voice_state = SPEAKER_VOICE_DONE;
                }
            } else {
                speaker_voice_state = SPEAKER_VOICE_DONE;
            }
        }
    }
    if (text_complete == 1 && speaker_voice_state == SPEAKER_VOICE_DONE) {
        if (D_8011ED5B & 0x80) {
            hud_text_marker_bob();
        } else {
            hud_text_marker_set(1);
        }
    }
    if (text_complete == 1 && (*(u8 *)&game_state.voice_mode) == 0) {
        if (D_8011ED5B & 0x80) {
            hud_text_marker_bob();
        } else {
            hud_text_marker_set(1);
        }
    }
    if (game_state.pad_pressed & PAD_CROSS) {
        if (text_complete == 1) {
            if ((*(u8 *)&game_state.voice_mode) == 2) {
                speaker_voice_state = SPEAKER_VOICE_DONE;
                audio_command(0x74);
                cd_audio_command(9, 0, 0);
            }
            game_state.text_scroll_state = 0;
        } else {
            k_disp_goto_line_end();
        }
    }
    if (game_state.pad_pressed & 0x60) {
        if (text_complete == 1) {
            if ((*(u8 *)&game_state.voice_mode) == 1) {
                if (speaker_voice_state == SPEAKER_VOICE_DONE) {
                    hud_text_marker_set(0);
                    game_state.substep += 1;
                }
                game_state.text_scroll_state = 0;
                return;
            }
            if ((*(u8 *)&game_state.voice_mode) == 2) {
                if (speaker_voice_state == SPEAKER_VOICE_DONE) {
                    hud_text_marker_set(0);
                    game_state.substep += 1;
                }
                game_state.text_scroll_state = 0;
                return;
            }
            hud_text_marker_set(0);
            game_state.substep += 1;
            game_state.text_scroll_state = 0;
            return;
        }
        k_disp_goto_line_end();
    }
}

/* Love level of girl arg0: base value from her record, plus one bonus from girl_param_bonus for each
 * parameter that her mask in girl_bonus_param_mask selects, by the threshold ladder of her kind (girl_param_ladder_kind).
 * FAKE: the u32 level locals lv0..lv6 (copies of girl_param_ladder_base) give the frame and the colour order of
 * the thresholds, and reading the parameters through the gs pointer gives the bonus row s1 and the
 * parameter value s2; the original source of both is unknown. T-25101 */
/* Weighted tokimeki for girl % 13 with extra terms from the player parameters */
u32 get_h_tokimeki(s32 girl_id) {
    s32 girl;
    s32 weighted_tokimeki;
    s32 i;
    u8 bonus_param_mask;
    u8 ladder_kind;
    u32 ladder_base_0;
    u32 ladder_base_1;
    u32 ladder_base_2;
    u32 ladder_base_3;
    u32 ladder_base_4;
    u32 ladder_base_5;
    u32 ladder_base_6;
    s32 param_bonus_enabled[8];
    s8 *param_bonus_by_step;
    GameState *gs;

    gs = &game_state;
    girl = girl_id % 13;
    weighted_tokimeki = gs->girls[girl].tokimeki * girl_tokimeki_weight[girl];
    weighted_tokimeki /= 100;
    bonus_param_mask = girl_bonus_param_mask[girl];
    for (i = 1; i < 8; i++) {
        if (bonus_param_mask & (1 << i)) {
            param_bonus_enabled[i] = 1;
        } else {
            param_bonus_enabled[i] = 0;
        }
    }
    ladder_kind = girl_param_ladder_kind[girl];
    ladder_base_0 = girl_param_ladder_base[0];
    ladder_base_1 = girl_param_ladder_base[1];
    ladder_base_2 = girl_param_ladder_base[2];
    ladder_base_3 = girl_param_ladder_base[3];
    ladder_base_4 = girl_param_ladder_base[4];
    ladder_base_5 = girl_param_ladder_base[5];
    ladder_base_6 = girl_param_ladder_base[6];
    for (i = 1; i < 7; i++) {
        if (param_bonus_enabled[i]) {
            switch (ladder_kind) {
            case 1:
                param_bonus_by_step = girl_param_bonus[girl];
                if ((&gs->param_condition)[i].value > ladder_base_0 + 50) {
                    weighted_tokimeki += param_bonus_by_step[0];
                } else if ((&gs->param_condition)[i].value > ladder_base_1 + 35) {
                    weighted_tokimeki += param_bonus_by_step[1];
                } else if ((&gs->param_condition)[i].value > ladder_base_2 + 20) {
                    weighted_tokimeki += param_bonus_by_step[2];
                } else if ((&gs->param_condition)[i].value > ladder_base_3 + 20) {
                    weighted_tokimeki += param_bonus_by_step[3];
                } else if ((&gs->param_condition)[i].value > ladder_base_4 + 5) {
                    weighted_tokimeki += param_bonus_by_step[4];
                } else if ((&gs->param_condition)[i].value > ladder_base_5 + 5) {
                    weighted_tokimeki += param_bonus_by_step[5];
                } else if ((&gs->param_condition)[i].value > ladder_base_6) {
                    weighted_tokimeki += param_bonus_by_step[6];
                } else {
                    weighted_tokimeki += param_bonus_by_step[7];
                }
                break;
            case 2:
                param_bonus_by_step = girl_param_bonus[girl];
                if ((&gs->param_condition)[i].value > ladder_base_0) {
                    weighted_tokimeki += param_bonus_by_step[0];
                } else if ((&gs->param_condition)[i].value > ladder_base_1) {
                    weighted_tokimeki += param_bonus_by_step[1];
                } else if ((&gs->param_condition)[i].value > ladder_base_2) {
                    weighted_tokimeki += param_bonus_by_step[2];
                } else if ((&gs->param_condition)[i].value > ladder_base_3) {
                    weighted_tokimeki += param_bonus_by_step[3];
                } else if ((&gs->param_condition)[i].value > ladder_base_4) {
                    weighted_tokimeki += param_bonus_by_step[4];
                } else if ((&gs->param_condition)[i].value > ladder_base_5) {
                    weighted_tokimeki += param_bonus_by_step[5];
                } else if ((&gs->param_condition)[i].value > ladder_base_6) {
                    weighted_tokimeki += param_bonus_by_step[6];
                } else {
                    weighted_tokimeki += param_bonus_by_step[7];
                }
                break;
            case 3:
                param_bonus_by_step = girl_param_bonus[girl];
                if ((&gs->param_condition)[i].value > ladder_base_0 + 50) {
                    weighted_tokimeki += param_bonus_by_step[0];
                } else if ((&gs->param_condition)[i].value > ladder_base_1 + 50) {
                    weighted_tokimeki += param_bonus_by_step[1];
                } else if ((&gs->param_condition)[i].value > ladder_base_2 + 50) {
                    weighted_tokimeki += param_bonus_by_step[2];
                } else if ((&gs->param_condition)[i].value > ladder_base_3 + 20) {
                    weighted_tokimeki += param_bonus_by_step[3];
                } else if ((&gs->param_condition)[i].value > ladder_base_4 + 20) {
                    weighted_tokimeki += param_bonus_by_step[4];
                } else if ((&gs->param_condition)[i].value > ladder_base_5 + 20) {
                    weighted_tokimeki += param_bonus_by_step[5];
                } else if ((&gs->param_condition)[i].value > ladder_base_6 + 20) {
                    weighted_tokimeki += param_bonus_by_step[6];
                } else {
                    weighted_tokimeki += param_bonus_by_step[7];
                }
                break;
            case 4:
                param_bonus_by_step = girl_param_bonus[girl];
                if ((&gs->param_condition)[i].value > ladder_base_0 + 10) {
                    weighted_tokimeki += param_bonus_by_step[0];
                } else if ((&gs->param_condition)[i].value > ladder_base_1 + 10) {
                    weighted_tokimeki += param_bonus_by_step[1];
                } else if ((&gs->param_condition)[i].value > ladder_base_2 + 10) {
                    weighted_tokimeki += param_bonus_by_step[2];
                } else if ((&gs->param_condition)[i].value > ladder_base_3 + 10) {
                    weighted_tokimeki += param_bonus_by_step[3];
                } else if ((&gs->param_condition)[i].value > ladder_base_4) {
                    weighted_tokimeki += param_bonus_by_step[4];
                } else if ((&gs->param_condition)[i].value > ladder_base_5) {
                    weighted_tokimeki += param_bonus_by_step[5];
                } else if ((&gs->param_condition)[i].value > ladder_base_6) {
                    weighted_tokimeki += param_bonus_by_step[6];
                } else {
                    weighted_tokimeki += param_bonus_by_step[7];
                }
                break;
            case 5:
                param_bonus_by_step = girl_param_bonus[girl];
                if ((&gs->param_condition)[i].value > ladder_base_0 + 20) {
                    weighted_tokimeki += param_bonus_by_step[0];
                } else if ((&gs->param_condition)[i].value > ladder_base_1 + 20) {
                    weighted_tokimeki += param_bonus_by_step[1];
                } else if ((&gs->param_condition)[i].value > ladder_base_2 + 20) {
                    weighted_tokimeki += param_bonus_by_step[2];
                } else if ((&gs->param_condition)[i].value > ladder_base_3 + 20) {
                    weighted_tokimeki += param_bonus_by_step[3];
                } else if ((&gs->param_condition)[i].value > ladder_base_4) {
                    weighted_tokimeki += param_bonus_by_step[4];
                } else if ((&gs->param_condition)[i].value > ladder_base_5 - 5) {
                    weighted_tokimeki += param_bonus_by_step[5];
                } else if ((&gs->param_condition)[i].value > ladder_base_6) {
                    weighted_tokimeki += param_bonus_by_step[6];
                } else {
                    weighted_tokimeki += param_bonus_by_step[7];
                }
                break;
            case 6:
                param_bonus_by_step = girl_param_bonus[girl];
                if ((&gs->param_condition)[i].value > ladder_base_0) {
                    weighted_tokimeki += param_bonus_by_step[0];
                } else if ((&gs->param_condition)[i].value > ladder_base_1) {
                    weighted_tokimeki += param_bonus_by_step[1];
                } else if ((&gs->param_condition)[i].value > ladder_base_2) {
                    weighted_tokimeki += param_bonus_by_step[2];
                } else if ((&gs->param_condition)[i].value > ladder_base_3) {
                    weighted_tokimeki += param_bonus_by_step[3];
                } else if ((&gs->param_condition)[i].value > ladder_base_4 - 5) {
                    weighted_tokimeki += param_bonus_by_step[4];
                } else if ((&gs->param_condition)[i].value > ladder_base_5 - 5) {
                    weighted_tokimeki += param_bonus_by_step[5];
                } else if ((&gs->param_condition)[i].value > ladder_base_6 - 5) {
                    weighted_tokimeki += param_bonus_by_step[6];
                } else {
                    weighted_tokimeki += param_bonus_by_step[7];
                }
                break;
            }
        }
    }
    if (weighted_tokimeki >= 0x65) {
        return 0x64;
    }
    if (weighted_tokimeki < 0) {
        return 0;
    }
    return weighted_tokimeki & 0xFF;
}

/* Affection * weight / 100 clamped 0..100 for girl % 13 */
u32 get_h_yuukou(s32 girl_id) {
    s32 girl_index;
    s32 weighted_affection;

    girl_index = girl_id % 13;
    weighted_affection = game_state.girls[girl_index].affection * girl_affection_weight[girl_index];
    weighted_affection /= 100;
    if (weighted_affection >= 0x65) {
        return 0x64;
    }
    if (weighted_affection < 0) {
        return 0;
    }
    return weighted_affection & 0xFF;
}

/* Fills the eleven-entry table with heroine tokimeki scores. */
void get_h_tokimeki_table(u8 *scores_out) {
    s32 i;

    for (i = 0; i < 11; i++) {
        scores_out[i] = get_h_tokimeki(i);
    }
}

/* Fills the eleven-entry table with heroine affection scores. */
void get_h_yuukou_table(u8 *scores_out) {
    s32 i;

    for (i = 0; i < 11; i++) {
        scores_out[i] = get_h_yuukou(i);
    }
}

/* Checks whether the current date matches the selected birthday. */
void birth_day_check(s32 birthday_subject) {
    birth_day_check_days(birthday_subject, game_state.month, game_state.day);
}

/* Return 1 when the girl's (or, for 0xF, the player's) birthday is the given month and day */
s32 birth_day_check_days(s32 birthday_subject, s32 month, s32 day) {
    s32 subject_id;
    s32 birthday_word;

    if (month == 0xD) {
        month = 1;
    }
    subject_id = birthday_subject & 0xF;
    if (subject_id == 0xF) {
        birthday_word = game_state.player_birthday;
        if (month == (birthday_word & 0xF)) {
            if (day == (u32)(birthday_word << 23) >> 27) {
                return 1;
            }
        }
    } else {
        birthday_word = game_state.girls[subject_id].birthday.w;
        if (month == (birthday_word & 0xF)) {
            if (day == (u32)(birthday_word << 23) >> 27) {
                return 1;
            }
        }
    }
    return 0;
}

s32 girl_points_shiori(u8 *closeness_by_girl, s16 *points_by_girl) {
    s32 i;
    s32 scene_count;
    u8 *param_minimum_tail;

    if (*closeness_by_girl < 0x55U) {
        return 0;
    }
    if (game_state.player_flags.b[1] & 0xF) {
        return 0;
    }
    i = 1;
    param_minimum_tail = girl_shiori_param_minimum_tail;
    do {
        if ((u32) (&game_state.param_condition)[i].value < param_minimum_tail[-1]) {
            return 0;
        }
        i++;
        param_minimum_tail++;
    } while (i < 8);
    scene_count = 0;
    /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
    for (i = 1; i < 36; i++) scene_count += game_state.girls[0].scene_counters[i];
    if (scene_count < 8) {
        return 0;
    }
    if (game_state.girls[0].syoushin >= 0x33) {
        return 0;
    }
    *points_by_girl = 0x311;
}

s32 girl_points_others(u8 *closeness_by_girl, s16 *points_by_girl) {
    GameState *g = &game_state;
    s32 i;
    /* FAKE: T-25307, j, k and x are never used; their three stack homes give the original's
     * frame (0x80, girl_points at sp+0x48). The original's real locals are unknown. */
    s32 j;
    s32 k;
    s32 m;
    s32 curve_bucket;
    s32 score_input;
    s32 x;
    s16 girl_points[11];

    for (m = 1; m < 11; m++) {
        girl_points[m] = closeness_by_girl[m] * 4;
        girl_points[m] += g->girls[m].syoushin * girl_syoushin_weight[m];
    }
    score_input = g->param_stress.value;
    if (score_input >= 141) {
        score_input = 140;
    }
    girl_points[2] += girl_points_stress_curve[score_input / 10];
    for (i = 1; i < 11; i++) {
        for (m = 0; m < 7; m++) {
            if ((girl_points_mask_strong[i] >> (7 - m)) & 1) {
                curve_bucket = (&g->param_condition)[7 - m].value;
                /* m never reaches 7; the original has this test and the divide by 20 too */
                if (m == 7) {
                    curve_bucket /= 20;
                } else {
                    curve_bucket /= 10;
                }
                if (curve_bucket >= 15) {
                    curve_bucket = 14;
                }
                girl_points[i] += girl_points_param_curve[curve_bucket];
                girl_points[i] += girl_points_param_curve[curve_bucket] / 2;
            }
            if ((girl_points_mask_normal[i] >> (7 - m)) & 1) {
                curve_bucket = (&g->param_condition)[7 - m].value;
                if (m == 7) {
                    curve_bucket /= 20;
                } else {
                    curve_bucket /= 10;
                }
                if (curve_bucket >= 15) {
                    curve_bucket = 14;
                }
                girl_points[i] += girl_points_param_curve[curve_bucket];
            }
        }
    }
    for (m = 1; m < 11; m++) {
        score_input = 0;
        for (i = 1; i < 36; i++) {
            score_input += g->girls[m].scene_counters[i];
        }
        if (score_input >= 4) {
            girl_points[m] += score_input * 10;
        } else {
            girl_points[m] = 0;
        }
    }
    for (m = 1; m < 11; m++) {
        points_by_girl[m] = girl_points[m];
    }
}

s32 girl_scores(u8 *closeness_by_girl, s16 *points_by_girl) {
    s32 i;
    u8 weighted_tokimeki[16];
    u8 weighted_affection[16];
    s32 j;
    get_h_yuukou_table(weighted_affection);
    get_h_tokimeki_table(weighted_tokimeki);
    for (i = 0, j = 0; j != 32; j += 2, i++) {
        points_by_girl[i] = 0;
        weighted_affection[i] /= 5U;
        if (weighted_affection[i] >= 20U) {
            weighted_affection[i] = 19;
        }
        weighted_tokimeki[i] /= 5U;
        if (weighted_tokimeki[i] >= 20U) {
            weighted_tokimeki[i] = 19;
        }
        closeness_by_girl[i] = girl_closeness_grid[weighted_tokimeki[i]][weighted_affection[i]];
    }
    girl_points_shiori(closeness_by_girl, points_by_girl);
    return girl_points_others(closeness_by_girl, points_by_girl);
}

s32 girl_player_sports_club_check(void) {
    u16 player_flags_word;

    player_flags_word = game_state.player_flags.h;
    if (((player_flags_word >> 0xC) >= 6) && ((player_flags_word >> 0xC) < 0xB)) {
        return 1;
    }
    return 0;
}

s32 girl_find_by_club(s32 club_id) {
    s32 i;

    for (i = 0; i < 11; i++) {
        if (club_id == (u32)game_state.girls[i].flags.b[2] >> 4) {
            return i;
        }
    }
    return -1;
}
