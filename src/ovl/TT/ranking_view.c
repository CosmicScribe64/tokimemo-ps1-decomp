/* ranking_view.c: src/ovl/TT/80132130.c:315: ranking display scene (state 7) and its grid background; body 324: control = tt_run; 325: buttons = (TtPadView *)(scene + 0x10);. Original object 0x80132130-0x80132F10 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/tt.h"

void tt_ranking_heading_init(void) {
    u8 *sprite = tt_sprites;

    sprite[0] = 1;
    sprite[1] = 0;
    *(s16 *)(sprite + 0x44) = 8;
    *(s16 *)(sprite + 0x14) = -0x38;
    *(s16 *)(sprite + 0x16) = 0x48;
    *(s16 *)(sprite + 0x3C) = 0x70;
    *(s16 *)(sprite + 0x3E) = 0x10;
    sprite[0x46] = 0;
    sprite[0x47] = 0x60;
    *(s16 *)(sprite + 0x40) = 0xA;
    *(s16 *)(sprite + 0x42) = 0x3C08;
    sprite[0x3B] = 0x65;
    *(s16 *)(sprite + 0x10) = 0;
}

void tt_ranking_rows_init(u8 arg0, u8 arg1, u8 arg2) {
    u8 *sys;
    u8 *text_record;
    s32 i;
    u8 c; /* set by the switch for every i; IDO still loads and stores its stack home */

    sys = tt_sys;
    text_record = tt_texts;
    for (i = 0; i != 3; i++) {
        switch (i) {
        case 0:
            c = arg0;
            break;
        case 1:
            c = arg1;
            break;
        case 2:
            c = arg2;
            break;
        }
        *(s16 *)(text_record + 0xA) = -0x4C;
        *(s16 *)(text_record + 0xC) = i * 0x10 + 0x78;
        *(s16 *)(text_record + 0xE) = i + 0xE;
        text_record[2] = c;
        text_record[0] = 1;
        *(s16 *)(text_record + 6) = 8;
        *(s16 *)(text_record + 0x10) = 0;
        *(s16 *)(text_record + 0x1E) = -0x1C;
        *(s16 *)(text_record + 0x20) = i * 0x10 + 0x78;
        *(s16 *)(text_record + 0x22) = i + 0x28;
        text_record[0x16] = c;
        text_record[0x14] = 1;
        *(s16 *)(text_record + 0x1A) = 8;
        *(s16 *)(text_record + 0x24) = 0;
        text_record += 0x28;
        tt_time_text_set(*(u16 *)(sys + i * 6 + 0x30), i + 0x28);
        tt_name_text_set((s32)(sys + i * 6 + 0x32), i + 0x2B);
        *(s16 *)(text_record + 0xA) = 0x34;
        *(s16 *)(text_record + 0xC) = i * 0x10 + 0x78;
        *(s16 *)(text_record + 0xE) = i + 0x2B;
        text_record[2] = c;
        text_record[0] = 1;
        *(s16 *)(text_record + 6) = 8;
        *(s16 *)(text_record + 0x10) = 0;
        text_record += 0x14;
    }
}

/* Fills the 25 grid records and the corner, frame and panel records after them, then the colour bytes
 * for scene arg0. The u16 stores of 8, 0xD and 0xC give the original register allocation. T-11840 */
void tt_ranking_panel_init(u8 arg0, u8 arg1) {
    u8 *state;
    u8 *p;
    u8 *transform;
    s32 i;

    state = tt_core;
    /* FAKE: p, transform and the loop header on one line put the hoisted (arg0 >> 4) chain ahead of the p load (as1 order). T-11840 */
    p = tt_sprites + 0x1900; transform = tt_transforms; for (i = 0; i < 25; i++) {
        *(s16 *)(transform + 4) = (i / 5) * 0x48 - 0x5C;
        *(s16 *)(transform + 6) = (i % 5) * 0x48 - 0x48;
        *(s16 *)(transform + 0) = 0x1000;
        *(s16 *)(transform + 2) = 0x1000;
        p[0] = 1;
        p[1] = 0;
        *(u16 *)(p + 0x44) = 8;
        p[0x3B] = 0x80;
        *(u8 **)(p + 0x48) = transform;
        p[0x4D] = 0;
        p[0x4E] = 1;
        p[0x3A] = 0x40;
        p[0x39] = 0x40;
        p[0x38] = 0x40;
        p[0x50] = 0;
        *(s16 *)(p + 0x14) = *(s16 *)(transform + 4) - 0x24;
        *(s16 *)(p + 0x16) = *(s16 *)(transform + 6) - 0x24;
        p[0x46] = 0x38; p[0x47] = 0xB8; /* FAKE: one line for the original li order. T-11840 */
        p[0x4F] = arg1;
        *(s16 *)(p + 0x3C) = 0x48;
        *(s16 *)(p + 0x3E) = 0x48;
        *(s16 *)(p + 0x40) = 0xA;
        *(s16 *)(p + 0x42) = (arg0 & 0xF) + (((arg0 >> 4) + 0xF0) << 6);
        p[0x4C] = 0;
        *(u16 *)(p + 0x10) = 0xD;
        p += 0x64;
        transform += 0x10;
    }
    for (i = 0; i < 4; i++, p += 0x64) {
        p[0] = 1;
        p[1] = 0;
        *(u16 *)(p + 0x44) = 8;
        if (i & 1) {
            *(s16 *)(p + 0x14) = 0x58;
        } else {
            *(s16 *)(p + 0x14) = -0x60;
        }
        if (i >> 1) {
            *(s16 *)(p + 0x16) = 0xB8;
        } else {
            *(s16 *)(p + 0x16) = 0x30;
        }
        *(s16 *)(p + 0x3C) = 8;
        *(s16 *)(p + 0x3E) = 8;
        p[0x46] = (i << 3) + 0x40;
        p[0x47] = 0x18;
        *(s16 *)(p + 0x40) = 0xA;
        *(s16 *)(p + 0x42) = 0x3C0D;
        p[0x3B] = 0x65;
        *(s16 *)(p + 0x10) = 0;
    }
    for (i = 0; i < 2; i++, p += 0xC8) {
        p[0] = 1;
        p[1] = 0;
        *(u16 *)(p + 0x44) = 8;
        *(s16 *)(p + 0x14) = -0x58;
        *(s16 *)(p + 0x16) = (i == 0 ? 1 : 2) + i * 0x8C + 0x30;
        *(s16 *)(p + 0x3C) = 0xB0;
        *(s16 *)(p + 0x3E) = 1;
        p[0x38] = 0xF8;
        p[0x39] = 0xD8;
        p[0x3A] = 0;
        *(s16 *)(p + 0x10) = 0;
        p[0x3B] = 0x61;
        p += 0x64;
        p[0] = 1;
        p[1] = 0;
        *(u16 *)(p + 0x44) = 8;
        *(s16 *)(p + 0x14) = -0x58;
        *(s16 *)(p + 0x16) = (i == 0 ? 2 : 1) + i * 0x8C + 0x30;
        *(s16 *)(p + 0x3C) = 0xB0;
        *(s16 *)(p + 0x3E) = 1;
        p[0x38] = 0xF8;
        p[0x39] = 0xA8;
        p[0x3A] = 0;
        *(s16 *)(p + 0x10) = 0;
        p[0x3B] = 0x61;
        p += 0x64;
        p[0] = 1;
        p[1] = 0;
        *(u16 *)(p + 0x44) = 8;
        *(s16 *)(p + 0x14) = -0x58;
        *(s16 *)(p + 0x16) = i * 0x8C + 0x30;
        *(s16 *)(p + 0x3C) = 0xB0;
        *(s16 *)(p + 0x3E) = 4;
        p[0x38] = 0xC0;
        p[0x39] = 0x70;
        p[0x3A] = 0;
        *(s16 *)(p + 0x10) = 0;
        p[0x3B] = 0x61;
    }
    for (i = 0; i < 2; i++, p += 0xC8) {
        p[0] = 1;
        p[1] = 0;
        *(u16 *)(p + 0x44) = 8;
        *(s16 *)(p + 0x14) = (i == 0 ? 1 : 2) + i * 0xBC - 0x60;
        *(s16 *)(p + 0x16) = 0x38;
        *(s16 *)(p + 0x3C) = 1;
        *(s16 *)(p + 0x3E) = 0x80;
        p[0x38] = 0xF8;
        p[0x39] = 0xD8;
        p[0x3A] = 0;
        *(s16 *)(p + 0x10) = 0;
        p[0x3B] = 0x61;
        p += 0x64;
        p[0] = 1;
        p[1] = 0;
        *(u16 *)(p + 0x44) = 8;
        *(s16 *)(p + 0x14) = (i == 0 ? 2 : 1) + i * 0xBC - 0x60;
        *(s16 *)(p + 0x16) = 0x38;
        *(s16 *)(p + 0x3C) = 1;
        *(s16 *)(p + 0x3E) = 0x80;
        p[0x38] = 0xF8;
        p[0x39] = 0xA8;
        p[0x3A] = 0;
        *(s16 *)(p + 0x10) = 0;
        p[0x3B] = 0x61;
        p += 0x64;
        p[0] = 1;
        p[1] = 0;
        *(u16 *)(p + 0x44) = 8;
        *(s16 *)(p + 0x14) = i * 0xBC - 0x60;
        *(s16 *)(p + 0x16) = 0x38;
        *(s16 *)(p + 0x3C) = 4;
        *(s16 *)(p + 0x3E) = 0x80;
        p[0x38] = 0xC0;
        p[0x39] = 0x70;
        p[0x3A] = 0;
        *(s16 *)(p + 0x10) = 0;
        p[0x3B] = 0x61;
    }
    p[0] = 1;
    p[1] = 0;
    *(s16 *)(p + 0x14) = -0x5E;
    *(s16 *)(p + 0x16) = 0x32;
    *(s16 *)(p + 0x3C) = 0xBC;
    *(s16 *)(p + 0x3E) = 0x8C;
    p[0x3A] = 0;
    p[0x39] = 0;
    p[0x38] = 0;
    *(u16 *)(p + 0x10) = 0xC;
    p[0x3B] = 0x63;
    switch (arg0) {
    case 12:
        state[0x13] = 0x7B;
        state[0x14] = 0xD5;
        state[0x15] = 0x94;
        break;
    case 13:
        state[0x13] = 0;
        state[0x14] = 0xCD;
        state[0x15] = 0xEE;
        break;
    case 14:
        state[0x13] = 0xFF;
        state[0x14] = 0x9C;
        state[0x15] = 0xCD;
        break;
    }
}

void tt_ranking_grid_scroll(void) {
    s32 i;
    TtCoreGridView *state;
    TtTransformView *grid;
    TtSpriteView *tiles;
    state = (TtCoreGridView *)tt_core;
    i = 0;
    tiles = (TtSpriteView *)(tt_sprites + 0x1900);
    grid = (TtTransformView *)tt_transforms;
    for (; i < 25; i++) {
        grid->x = (i / 5) * 0x48 - state->scroll_offset - 0x5C;
        grid->y = state->scroll_offset + (i % 5) * 0x48 - 0x48;
        tiles->x = grid->x - 0x24;
        tiles->y = grid->y - 0x24;
        tiles++;
        grid++;
    }
    if (++state->scroll_offset >= 0x48U) {
        state->scroll_offset = 0;
    }
}

void tt_ranking_fade_in(void) {
    u8 *core;
    u8 *q;
    s32 i;

    core = tt_core;
    core[0x1D] += 4;
    core[0x1E] = core[0x1F] = core[0x1D];
    if (core[0x1D] >= 0x80U) {
        q = tt_texts;
        for (i = 0; i < 0x80; i++) {
            q += 0x14;
            *(s16 *)(q - 0xE) = 0;
        }
        q = tt_sprites;
        for (i = 0; i < 0x80; i++) {
            *(s16 *)(q + 0x44) = 0;
            q += 0x64;
        }
        *(s16 *)(core + 2) = 0x40;
    }
    tt_ranking_grid_scroll();
}

void tt_ranking_fade_out(void) {
    u8 *core;
    u8 *q;
    s32 i;

    core = tt_core;
    if (core[0x1D] == 0x80U) {
        i = 0;
        q = tt_texts;
        for (; i < 0x80; i++) {
            q += 0x14;
            *(s16 *)(q - 0xE) = 8;
        }
        q = tt_sprites;
        for (i = 0; i < 0x80; i++) {
            *(s16 *)(q + 0x44) = 8;
            q += 0x64;
        }
    }
    core[0x1D] -= 4;
    core[0x1E] = core[0x1F] = core[0x1D];
    if (core[0x1D] == 0) {
        core[9] = 1;
    }
    tt_ranking_grid_scroll();
}

void tt_state_ranking_view(void) {
    u8 *scene = tt_sys;
    /* FAKE (T-9780): retain the unused selector home between the saved pointers. */
    u16 pad_selector;
    u8 *control;
    TtPadView *buttons;
    u16 flags;
    TtCore *state = (TtCore *)tt_core;

    control = tt_run;
    buttons = (TtPadView *)(scene + 0x10);
    switch (state->sub_state) {
    case 0:
        audio_command(0x22);
        audio_command(0x2F);
        audio_command(4);
        audio_command(0x204);
        tt_sprites_clear();
        tt_texts_clear();
        tt_player_clear();
        tt_shots_clear();
        tt_bombs_clear();
        tt_bomb_targets_clear();
        tt_options_clear();
        tt_air_enemies_clear();
        tt_ground_targets_clear();
        tt_enemy_shots_clear();
        tt_bells_clear();
        tt_clouds_clear();
        tt_boss_sprites_clear();
        state->pause_allowed = 0;
        state->bg_mode = 1;
        state->fade_r = 0;
        state->fade_g = 0;
        state->fade_b = 0;
        tt_ranking_heading_init();
        tt_ranking_rows_init(2, 6, 4);
        tt_ranking_panel_init(0xD, 0);
        state->fade_level = 0;
        if (state->autoplay != 0) {
            state->timer = 0x12CU;
        } else {
            state->timer = 0xE10U;
        }
        state->sub_state = 0x20;
        break;
    case 0x20:
        tt_ranking_fade_in((u8 *)buttons);
        break;
    case 0x40:
        tt_ranking_grid_scroll((u8 *)buttons);
        if ((state->timer -= 1) == 0) {
            state->sub_state = 0x80;
        }
        if ((state->autoplay == 0) && ((flags = buttons->pressed, ((flags & 0x20) != 0)) || (flags & 0x800))) {
            state->fade_r = 0x80;
            state->fade_g = 0x80;
            state->fade_b = 0x80;
            state->sub_state = 0x80;
        }
        break;
    case 0x80:
        audio_command(0xB1);
        tt_ranking_fade_out();
        break;
    }
    if (state->request_next != 0) {
        if (scene[83] == 0) {
            if (control[15] != 0) {
                state->state = 8;
            } else {
                state->state = 1;
            }
        } else {
            state->state = 1;
        }
        state->phase = 0;
        state->sub_step = 0;
        state->sub_state = 0;
        state->flag_b = 0;
        state->flag_a = 0;
        state->request_next = 0U;
    }
}
