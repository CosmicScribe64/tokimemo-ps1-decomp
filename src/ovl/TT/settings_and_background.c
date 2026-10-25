/* settings_and_background.c: src/ovl/TT/8013C6D0.c:4: settings/button configuration state plus background renderers; body 8: p[0] = 1; 9: p[1] = 0;. Original object 0x8013C6D0-0x8013E290 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/TT.h"

void tt_button_cursor_init(void) {
    u8 *run = tt_run;
    u8 *sprite = tt_sprites;

    sprite[0] = 1;
    sprite[1] = 0;
    *(s16 *)(sprite + 0x44) = 0;
    *(s16 *)(sprite + 0x14) = -0x60;
    *(s16 *)(sprite + 0x16) = run[0xA] * 16 + 0x60;
    *(s16 *)(sprite + 0x3C) = 8;
    *(s16 *)(sprite + 0x3E) = 8;
    sprite[0x46] = 0;
    sprite[0x47] = 0x18;
    *(s16 *)(sprite + 0x40) = 0xA;
    *(s16 *)(sprite + 0x42) = 0x3C0F;
    sprite[0x3B] = 0x65;
    *(s16 *)(sprite + 0x10) = 0;
}

void tt_button_cursor_update(void) {
    *(s16 *)(tt_sprites + 0x16) = tt_run[0xA] * 16 + 0x60;
}

void tt_button_code_set(u16 arg0, u16 arg1) {
    u8 *sys = tt_sys;
    u8 v = 0;

    if (arg0 != 0) {
        if (arg0 & 0x20) {
            v = 5;
        } else if (arg0 & 0x40) {
            v = 6;
        } else if (arg0 & 0x10) {
            v = 4;
        } else if (arg0 & 0x80) {
            v = 7;
        } else if (arg0 & 4) {
            v = 2;
        } else if (arg0 & 8) {
            v = 3;
        } else if (!(arg0 & 1) && (arg0 & 2)) {
            v = 1;
        }
        if (arg1 == 0) {
            sys[0x44] = v;
        } else {
            sys[0x45] = v;
        }
    }
}

u16 tt_button_code_label(u8 arg0, s8 *arg1) {
    u16 ret;

    switch (arg0) {
    case 5:
        ret = 0x1E;
        *arg1 = 0;
        break;
    case 6:
        ret = 0x1F;
        *arg1 = 0;
        break;
    case 4:
        ret = 0x20;
        *arg1 = 0;
        break;
    case 7:
        ret = 0x21;
        *arg1 = 0;
        break;
    case 2:
        ret = 0x22;
        *arg1 = 0xF;
        break;
    case 3:
        ret = 0x23;
        *arg1 = 0xF;
        break;
    case 0:
        ret = 0x24;
        *arg1 = 0xF;
        break;
    case 1:
        ret = 0x25;
        *arg1 = 0xF;
        break;
    }
    return ret;
}

void tt_button_config_init(void) {
    u8 *sys;
    TtText *p;
    s32 i;

    p = (TtText *)tt_texts;
    sys = tt_sys;
    tt_run[0xA] = 0;
    tt_button_cursor_init();
    p->active = 1;
    p->x = -0x48;
    p->y = 0x30;
    p->text = 0x12;
    p->palette = 0xF;
    p->flags = 0;
    p->layer = 0;
    p++;
    for (i = 0; i < 3; i++, p++) {
        p->active = 1;
        p->x = -0x50;
        p->y = i * 16 + 0x60;
        p->text = tt_menu_text_ids[i];
        p->palette = 0xF;
        p->flags = 0;
        p->layer = 0;
    }
    p = (TtText *)tt_texts + 16;
    p->active = 1;
    p->x = 0x30;
    p->y = 0x60;
    p->text = tt_button_code_label(sys[0x44], (s8 *)&p->palette);
    p->flags = 0;
    p->layer = 0;
    p[1].active = 1;
    p[1].x = 0x30;
    p[1].y = 0x70;
    p++;
    p->text = tt_button_code_label(sys[0x45], (s8 *)&p->palette);
    p->flags = 0;
    p->layer = 0;
}

void tt_button_config_update(void) {
    u8 *sys;
    s32 pad[3]; /* FAKE: unused locals between sys and r, real source unknown (frame layout). T-9130 */
    u8 *r;
    u8 *q;
    u8 *core;
    u8 *run;

    sys = tt_sys;
    core = tt_core;
    run = tt_run;
    q = sys + 0x10;
    if (*(u16 *)(sys + 0x14) & 0x1000) {
        run[0xA] -= 1;
        if ((s8)run[0xA] < 0) {
            run[0xA] = 2;
        }
    } else if (*(u16 *)(q + 4) & 0x4000) {
        run[0xA] += 1;
        if (run[0xA] >= 3U) {
            run[0xA] = 0;
        }
    }
    if ((*(u16 *)(q + 4) & 0x820) && run[0xA] == 2) {
        *(u16 *)(core + 4) = 0;
        *(u16 *)(core + 2) = 0;
    }
    if (run[0xA] == 0 || run[0xA] == 1) {
        if (*(u16 *)(q + 4) & 0xFF) {
            tt_button_code_set(*(u16 *)(q + 4), run[0xA]);
        }
    }
    r = tt_texts + 0x140;
    *(s16 *)(r + 0xE) = tt_button_code_label(sys[0x44], tt_texts + 0x142);
    r += 0x14;
    *(s16 *)(r + 0xE) = tt_button_code_label(sys[0x45], r + 2);
    tt_button_cursor_update();
}

void tt_button_config_run(void) {
    u8 *core = tt_core;

    switch (*(u16 *)(core + 4)) {
    case 0x0:
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
        tt_button_config_init();
        *(u16 *)(core + 4) = 0x40;
        break;
    case 0x40:
        tt_button_config_update();
        break;
    }
}

void tt_settings_menu_init(void) {
    u8 *sys;
    s32 i;
    TtText *p;

    sys = tt_sys;
    p = (TtText *)tt_texts;
    tt_run[0xA] = 0;
    tt_button_cursor_init();
    p->active = 1;
    p->x = -0x18;
    p->y = 0x30;
    p->text = 4;
    p->palette = 0xF;
    p->flags = 0;
    p->layer = 0;
    p++;
    for (i = 0; i < 4; i++, p++) {
        p->active = 1;
        p->x = -0x50;
        p->y = i * 16 + 0x60;
        p->text = tt_menu_text_ids[i + 4];
        p->palette = 0xF;
        p->flags = 0;
        p->layer = 0;
    }
    p = (TtText *)tt_texts + 16;
    p->active = 1;
    p->x = 0x20;
    p->y = 0x70;
    p->palette = 0xF;
    p->flags = 0;
    p->layer = 0;
    if (sys[0x43] != 0) {
        p->text = 0x18;
    } else {
        p->text = 0x19;
    }
    p[1].active = 1;
    p[1].x = 0x20;
    p[1].y = 0x80;
    p[1].palette = 0xF;
    p[1].flags = 0;
    p[1].layer = 0;
    p++;
    if (sys[0x49] != 0) {
        p->text = 0x16;
    } else {
        p->text = 0x15;
    }
}

void tt_settings_menu_update(void) {
    s32 pad; /* FAKE: unused home; the spill slots of t, run, g sit at sp+0x48/0x44/0x40 only with it. T-11500 */
    u8 *t;
    u8 *run;
    u8 *g;
    u8 *sys;

    sys = tt_sys;
    t = tt_core;
    run = tt_run;
    g = sys + 0x10;
    if (*(u16 *)(sys + 0x14) & 0x1000) {
        run[10] -= 1;
        if ((s8)run[10] < 0) {
            run[10] = 3;
        }
    } else if (*(u16 *)(g + 4) & 0x4000) {
        run[10] += 1;
        if (run[10] >= 4) {
            run[10] = 0;
        }
    }
    if (*(u16 *)(g + 4) & 0x8000) {
        switch (run[10]) {
        case 1:
            sys[0x43] = 0;
            *(s16 *)(tt_texts + 0x14E) = 0x19;
            break;
        case 2:
            sys[0x49] = 0;
            *(s16 *)(tt_texts + 0x162) = 0x15;
            {
                TtFrame *frame;

                frame = (TtFrame *)tt_frames;
                SetDefDispEnv(&frame[0].disp_rect, 0x140, 0, 0x140, 0xF0);
                SetDefDispEnv(&frame[1].disp_rect, 0, 0, 0x140, 0xF0);
            }
            break;
        }
    } else if (*(u16 *)(g + 4) & 0x2000) {
        switch (run[10]) {
        case 1:
            sys[0x43] = 1;
            *(s16 *)(tt_texts + 0x14E) = 0x18;
            break;
        case 2:
            sys[0x49] = 1;
            *(s16 *)(tt_texts + 0x162) = 0x16;
            {
                TtFrame *p2;

                p2 = (TtFrame *)tt_frames;
                SetDefDispEnv(&p2[0].disp_rect, 0x160, 0, 0x100, 0xF0);
                SetDefDispEnv(&p2[1].disp_rect, 0x20, 0, 0x100, 0xF0);
            }
            break;
        }
    }
    if (*(u16 *)(g + 4) & 0x820) {
        switch (run[10]) {
        case 0:
            *(s16 *)(t + 2) = 0x50;
            break;
        case 3:
            *(s16 *)(t + 6) = 0;
            *(s16 *)(t + 4) = 0;
            *(s16 *)(t + 2) = 0;
            t[11] = 0;
            t[10] = 0;
            t[9] = 0;
            *(u16 *)t = 1;
            break;
        }
    }
    tt_button_cursor_update();
}

void tt_state_settings(void) {
    u8 *core = tt_core;

    switch (*(u16 *)(core + 2)) {
    case 0x0:
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
        core[0x11] = 0;
        core[0x1C] = 0;
        core[0x13] = 0;
        core[0x14] = 0;
        core[0x15] = 0;
        tt_settings_menu_init();
        *(u16 *)(core + 2) = 0x40;
        break;
    case 0x40:
        tt_settings_menu_update();
        break;
    case 0x50:
        tt_button_config_run();
        break;
    }
}

void tt_scroll_speed_update(void) {
    u8 *core;
    s32 *run;
    s32 *stage_script;
    u8 *player;
    u8 mode;
    u16 w;
    s16 h;
    s32 cur;
    s32 diff;

    core = tt_core;
    run = (s32 *)tt_run;
    stage_script = tt_stage_script;
    player = tt_player;
    mode = core[0x1C];
    if (mode == 0x10) {
        *(u16 *)(core + 0x32) = *(u16 *)(core + 0x32) + 1;
        w = *(u16 *)(core + 0x34);
        if (w < 0x101) {
            *(u16 *)(core + 0x34) = 0x100;
        } else {
            *(u16 *)(core + 0x34) = w - 0x20;
        }
    } else if (mode == 0x11) {
        *(u16 *)(core + 0x32) = *(u16 *)(core + 0x32) + 1;
        w = *(u16 *)(core + 0x34);
        if (w >= 0x1000) {
            *(u16 *)(core + 0x34) = 0x1000;
            core[0x1C] = 2;
        } else {
            *(u16 *)(core + 0x34) = w + 0x40;
        }
    }
    if (*(u16 *)((u8 *)stage_script + 2) >= 0x401) {
        run[1] = 0;
    } else {
        h = *(s16 *)(player + 0x16);
        if (h < 0x50) {
            run[1] = 0x30000;
        } else {
            run[1] = 0x28000 - (((h - 0x50) / 32) << 15);
        }
        if (run[1] < 0x8000) {
            run[1] = 0x8000;
        }
        if (run[1] > 0x30000) {
            run[1] = 0x30000;
        }
    }
    if (run[1] < run[0]) {
        run[0] -= 0x400;
    } else {
        run[0] += 0x200;
    }
    diff = run[0] - run[1];
    cur = (diff < 0) ? -diff : diff;
    if (cur < 0x1000) {
        run[0] = run[1];
    }
}

/* The cursor is read again for the increment (as in tt_bg_tilemap_draw); with v + 1 IDO puts v in $a0, the original in $a1. T-11860 */
void tt_bg_flat_draw(void) {
    u8 *core;
    TtDrawBuffer *p;
    TILE *v;

    core = tt_core;
    p = (TtDrawBuffer *)(tt_frames + ((TtVideoView *)tt_video)->index * 0x80B8);
    v = (TILE *)p->cursor;
    p->cursor = (DR_MODE *)((u8 *)p->cursor + 0x10);
    ((TtOtTag *)v)->len = 3, v->code = 0x61;
    v->r0 = core[0x13], v->g0 = core[0x14], v->b0 = core[0x15];
    v->x0 = -0x80, v->y0 = 8;
    v->w = 0x100, v->h = 0xE8;
    ((TtOtTag *)v)->addr = p->ordering[15].addr;
    p->ordering[15].addr = (u32)v;
}

void tt_bg_flat_fade_draw(void) {
    u8 *core;
    TtDrawBuffer *frame;
    TILE *v;
    u32 r;
    u32 g;
    u32 b;

    core = tt_core;
    frame = ((TtVideoView *)tt_video)->index + (TtDrawBuffer *)tt_frames;
    v = (TILE *)frame->cursor;
    frame->cursor = (DR_MODE *)(v + 1);
    /* comma expressions as in the libgpu setTile/setWH macros (one source line for as1) */
    ((TtOtTag *)v)->len = 3, v->code = 0x61;
    /* FAKE: dead store; it numbers g before r, so uopt colours g first (original: r in a2, g in a1). T-11630 */
    g = 0;
    r = (core[0x13] * core[0x1D]) >> 7;
    if ((s32)r >= 0x100) {
        r = 0xFF;
    }
    g = (core[0x14] * core[0x1E]) >> 7;
    if ((s32)g >= 0x100) {
        g = 0xFF;
    }
    b = (core[0x15] * core[0x1F]) >> 7;
    if ((s32)b >= 0x100) {
        b = 0xFF;
    }
    v->r0 = r;
    v->g0 = g;
    v->b0 = b;
    v->x0 = -0x80;
    v->y0 = 8;
    v->w = 0x100, v->h = 0xE8;
    ((TtOtTag *)v)->addr = *(u32 *)&frame->ordering[15];
    (&frame->ordering[15])->addr = (u32)v;
}

/* Tile map of tt_core + 0x18, used only by the draw functions below: index (u16 per cell), 3-byte records, flag bytes. */
typedef struct TtTileMap {
    /* 0x00 */ u8 *index;
    /* 0x04 */ u8 *record;
    /* 0x08 */ u8 *flags;
} TtTileMap;

void tt_bg_tilemap_draw(void) {
    TtDrawBuffer *buffer;
    TtRectPacket *q;
    DR_MODE *m;
    u8 *core;
    u8 *r;
    s32 row;
    s32 col;
    s16 scroll;
    TtTileMap *tm;
    TtStageScript *stage_script;

    buffer = (TtDrawBuffer *)(tt_frames + ((TtVideoView *)tt_video)->index * 0x80B8);
    stage_script = (TtStageScript *)tt_stage_script;
    scroll = 0x300 - stage_script->progress;
    core = tt_core;
    for (row = 0; row != 0x100; row += 0x10) {
        for (col = 0; col != 0x10; col++) {
            q = (TtRectPacket *)buffer->cursor;
            buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x10);
            q->code = 0x7D;
            q->tag.len = 3;
            q->x = (col << 4) - 0x80;
            q->y = row - (scroll & 0xF);
            tm = *(TtTileMap **)(core + 0x18);
            r = tm->record + tm->index[(row + col + (scroll & ~0xF)) & 0x3FF] * 3;
            q->u = (r[1] & 7) << 4;
            q->v = (r[0] << 7) + ((r[1] >> 3) << 4);
            q->clut = (((r[2] >> 4) + 0xF0) << 6) + (r[2] & 0xF);
            q->tag.addr = buffer->ordering[13].addr;
            buffer->ordering[13].addr = (u32)q;
        }
    }
    m = buffer->cursor;
    buffer->cursor++;
    func_8009D294((s32)m, 0, 0, 0xE, 0);
    ((TtOtTag *)m)->addr = buffer->ordering[13].addr;
    buffer->ordering[13].addr = (u32)m;
}

void tt_bg_tilemap_fade_draw(void) {
    TtDrawBuffer *buffer;
    TtRectPacket *q;
    DR_MODE *m;
    u8 *core;
    u8 *r;
    s32 row;
    s32 col;
    s16 scroll;
    TtTileMap *tm;
    TtStageScript *stage_script;

    buffer = (TtDrawBuffer *)(tt_frames + ((TtVideoView *)tt_video)->index * 0x80B8);
    stage_script = (TtStageScript *)tt_stage_script;
    scroll = 0x300 - stage_script->progress;
    core = tt_core;
    for (row = 0; row != 0x100; row += 0x10) {
        for (col = 0; col != 0x10; col++) {
            q = (TtRectPacket *)buffer->cursor;
            buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x10);
            q->code = 0x7C;
            q->tag.len = 3;
            q->r = core[0x1D];
            q->g = core[0x1E];
            q->b = core[0x1F];
            q->y = row - (scroll & 0xF);
            q->x = (col << 4) - 0x80;
            tm = *(TtTileMap **)(core + 0x18);
            r = tm->record + tm->index[(row + col + (scroll & ~0xF)) & 0x3FF] * 3;
            q->u = (r[1] & 7) << 4;
            q->v = (r[0] << 7) + ((r[1] >> 3) << 4);
            q->clut = (((r[2] >> 4) + 0xF0) << 6) + (r[2] & 0xF);
            q->tag.addr = buffer->ordering[13].addr;
            buffer->ordering[13].addr = (u32)q;
        }
    }
    m = buffer->cursor;
    buffer->cursor++;
    func_8009D294((s32)m, 0, 0, 0xE, 0);
    ((TtOtTag *)m)->addr = buffer->ordering[13].addr;
    buffer->ordering[13].addr = (u32)m;
}

void tt_bg_tilemap_warp_draw(void) {
    u8 *core;
    s32 pad1; /* FAKE: unused, frame slot under core (0x530). T-11840 */
    TtStageScript *stage_script;
    POLY_FT4 *p;
    TtDrawBuffer *buffer;
    u8 *r;
    s32 j;
    s32 pad2; /* FAKE: unused, frame slot between j and i (0x518). T-11840 */
    s32 i;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    s16 x3;
    s16 y3;
    s16 scroll;
    u8 u;
    u8 v;
    s16 x[17][17];
    s16 y[17][17];

    stage_script = (TtStageScript *)tt_stage_script;
    core = tt_core;
    buffer = (TtDrawBuffer *)(tt_frames + ((TtVideoView *)tt_video)->index * 0x80B8);
    for (i = 0; i < 17; i++) {
        for (j = 0; j < 17; j++) {
            x[j][i] = ((j - 8) * (0x8800 - ((TtCoreWarpView *)core)->warp_divisor)) / 0x780;
            y[j][i] = ((i - 8) * (0x8800 - ((TtCoreWarpView *)core)->warp_divisor)) / 0x780;
            x[j][i] += (s16)(func_800A0140((((TtCoreWarpView *)core)->warp_phase << 5) + i * 0x200) / ((TtCoreWarpView *)core)->warp_divisor);
            y[j][i] += (s16)(rsin((((TtCoreWarpView *)core)->warp_phase << 5) + j * 0x200) / ((TtCoreWarpView *)core)->warp_divisor);
        }
    }
    scroll = 0x300 - stage_script->progress;
    for (i = 0; i != 0x100; i += 0x10) {
        for (j = 0; j < 16; j++) {
            p = (POLY_FT4 *)buffer->cursor;
            buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x28);
            ((TtOtTag *)p)->len = 9, p->code = 0x2D;
            x0 = x[j][i / 16];
            y0 = y[j][i / 16] - (s16)(scroll & 0xF) + 0x80;
            x1 = x[j + 1][i / 16];
            y1 = y[j + 1][i / 16] - (s16)(scroll & 0xF) + 0x80;
            x2 = x[j][i / 16 + 1];
            y2 = y[j][i / 16 + 1] - (s16)(scroll & 0xF) + 0x80;
            x3 = x[j + 1][i / 16 + 1];
            y3 = y[j + 1][i / 16 + 1] - (s16)(scroll & 0xF) + 0x80;
            p->x0 = x0, p->y0 = y0, p->x1 = x1, p->y1 = y1, p->x2 = x2, p->y2 = y2, p->x3 = x3, p->y3 = y3;
            r = (*(TtTileMap **)(core + 0x18))->record + (*(TtTileMap **)(core + 0x18))->index[(i + j + (scroll & ~0xF)) & 0x3FF] * 3;
            u = (r[1] & 7) << 4;
            v = (r[1] >> 3) << 4;
            if (r[0] == 1) {
                v += 0x80;
            }
            p->u0 = u, p->v0 = v, p->u1 = u + 0xF, p->v1 = v, p->u2 = u, p->v2 = v + 0xF, p->u3 = u + 0xF, p->v3 = v + 0xF;
            p->tpage = 0xE;
            p->clut = (((r[2] >> 4) + 0xF0) << 6) + (r[2] & 0xF);
            ((TtOtTag *)p)->addr = buffer->ordering[13].addr;
            buffer->ordering[13].addr = (u32)p;
        }
    }
}

void tt_bg_sky_draw(void) {
    s32 pad1; /* FAKE: unused, with pad2 reproduces the 0x78 frame. T-11510 */
    s32 pad2; /* FAKE: see pad1. T-11510 */
    TtDrawBuffer *buffer;
    TtRectPacket *q;
    POLY_G4 *g;
    DR_MODE *m;
    u8 *core;
    u8 *r;
    s32 row;
    s32 col;

    core = tt_core;
    buffer = (TtDrawBuffer *)(tt_frames + ((TtVideoView *)tt_video)->index * 0x80B8);
    for (row = 0; row != 0x80; row += 0x10) {
        for (col = 0; col != 0x10; col++) {
            q = (TtRectPacket *)buffer->cursor;
            buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x10);
            q->code = 0x7D;
            q->tag.len = 3;
            q->x = (col << 4) - 0x80;
            q->y = row + 0x70;
            r = ((TtTileMap *)*(u8 **)(core + 0x18))->record + ((TtTileMap *)*(u8 **)(core + 0x18))->index[(row + col) & 0x3FF] * 3;
            /* Operand order of both sums and of the flags address sets the ugen temps (t6/t7); found with
               tools/order_search.py. T-11540 */
            q->u = ((r[1] & 7) << 4) + (*(r[0] + ((TtTileMap *)*(u8 **)(core + 0x18))->flags) & 0x80);
            q->v = ((r[1] >> 3) << 4) + ((*(r[0] + ((TtTileMap *)*(u8 **)(core + 0x18))->flags) & 0x40) * 2);
            q->clut = (((r[2] >> 4) + 0xF0) << 6) + (r[2] & 0xF);
            q->tag.addr = buffer->ordering[13].addr;
            buffer->ordering[13].addr = (u32)q;
        }
    }
    m = buffer->cursor;
    buffer->cursor++;
    func_8009D294((s32)m, 0, 0, 0x18, 0);
    ((TtOtTag *)m)->addr = buffer->ordering[13].addr;
    buffer->ordering[13].addr = (u32)m;
    m = buffer->cursor;
    buffer->cursor++;
    func_8009D294((s32)m, 0, 0, 0x18, 0);
    ((TtOtTag *)m)->addr = buffer->ordering[15].addr;
    buffer->ordering[15].addr = (u32)m;
    g = (POLY_G4 *)buffer->cursor;
    buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x24);
    ((TtOtTag *)g)->len = 8; g->code = 0x39;
    g->x0 = -0x80; g->y0 = 0; g->x1 = 0x80; g->y1 = 0; g->x2 = -0x80; g->y2 = 0xA8; g->x3 = 0x80; g->y3 = 0xA8;
    g->r0 = 0; g->g0 = 0x68; g->b0 = 0xF0;
    g->r1 = 0; g->g1 = 0x68; g->b1 = 0xF0;
    g->r2 = 0xB0; g->g2 = 0xF0; g->b2 = 0xD8;
    g->r3 = 0xB0; g->g3 = 0xF0; g->b3 = 0xD8;
    ((TtOtTag *)g)->addr = buffer->ordering[15].addr;
    buffer->ordering[15].addr = (u32)g;
    m = buffer->cursor;
    buffer->cursor++;
    func_8009D294((s32)m, 0, 1, 0x18, 0);
    ((TtOtTag *)m)->addr = buffer->ordering[15].addr;
    buffer->ordering[15].addr = (u32)m;
}

void tt_bg_draw(void) {
    switch (tt_core[0x1C]) {
    case 0:
        tt_bg_flat_draw();
        return;
    case 1:
        tt_bg_flat_fade_draw();
        return;
    case 2:
        tt_bg_tilemap_draw();
        return;
    case 3:
        tt_bg_tilemap_fade_draw();
        return;
    case 0x10:
    case 0x11:
        tt_bg_tilemap_warp_draw();
        return;
    case 0x20:
    case 0x21:
        tt_bg_sky_draw();
        /* fallthrough */
    default:
        return;
    }
}
