/* title_and_projectile_pools.c: src/ovl/TT/80148DB0.c:4: title logo/menu/ship state plus bomb-target and player-shot pool operations; body 8: i = 0; 9: p = tt_bomb_targets;. Original object 0x80148DB0-0x8014AF00 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/TT.h"

void tt_bomb_targets_clear(void) {
    s32 i;
    u8 *p;

    i = 0;
    p = tt_bomb_targets;
    for (; i < 8; i++) {
        *p = 0;
        p += 0x58;
    }
}

void tt_bomb_targets_reset(void) {
    s32 i;
    u8 *bomb_target;

    for (i = 0, bomb_target = tt_bomb_targets; i < 8; i++, bomb_target += 0x58) {
        bomb_target[0] = 0;
        *(s16 *)(bomb_target + 0x10) = 0xB;
        *(s16 *)(bomb_target + 0x3E) = 0xA;
        *(s16 *)(bomb_target + 0x40) = 0x3C8B;
        bomb_target[0x42] = 0xF0;
        bomb_target[0x43] = 0xD0;
    }
}

void tt_bomb_target_init(arg0, arg1)
u8 *arg0;
s32 arg1;
{
    u8 *src;
    s32 *stage_script;
    u8 *p;
    s32 i;
    s32 d;
    s32 dx;
    s32 e;

    stage_script = tt_stage_script;
    src = tt_player;
    if (arg1 < 2) {
        *(s32 *)(arg0 + 0x20) = *(s16 *)(src + 0x14) << 16;
        *(s32 *)(arg0 + 0x1C) = *stage_script - ((*(s16 *)(src + 0x16) - 0x60) << 16);
        *(u16 *)(arg0 + 0x44) = 0xFFFF;
        i = 0;
        p = tt_ground_targets;
        for (; i != 0x20; i++, p += 0x78) {
            if (p[0] == 0) continue;
            if (*(u16 *)(p + 4) != 0x40) continue;
            if (p[0x64] != 0) continue;
            d = *(s16 *)(src + 0x16) - *(s16 *)(p + 0x16);
            if (d < 0x60 && d >= 0x11) {
                e = *(s16 *)(p + 0x14) - *(s16 *)(src + 0x14);
                if (e < 0) {
                    dx = -e;
                } else {
                    dx = e;
                }
                if (dx < 0x20) {
                    *(s32 *)(arg0 + 0x20) = *(s32 *)(p + 0x20);
                    *(s32 *)(arg0 + 0x1C) = *(s32 *)(p + 0x1C);
                    *(s16 *)(arg0 + 0x44) = i;
                    p[0x64] = 1;
                    break;
                }
            }
        }
    } else {
        src = tt_options + (arg1 >> 1) * 0xA4 - 0xA4;
        *(s32 *)(arg0 + 0x20) = *(s16 *)(src + 0x14) << 16;
        *(s32 *)(arg0 + 0x1C) = *stage_script - ((*(s16 *)(src + 0x16) - 0x60) << 16);
        *(u16 *)(arg0 + 0x44) = 0xFFFF;
        p = tt_ground_targets;
        for (i = 0; i != 0x20; i++, p += 0x78) {
            if (p[0] != 0 && p[0x64] == 0) {
                d = *(s16 *)(src + 0x16) - *(s16 *)(p + 0x16);
                if (d < 0x60 && d >= 0x11) {
                    e = *(s16 *)(p + 0x14) - *(s16 *)(src + 0x14);
                    if (e < 0) {
                        dx = -e;
                    } else {
                        dx = e;
                    }
                    if (dx < 0x20) {
                        *(s32 *)(arg0 + 0x20) = *(s32 *)(p + 0x20);
                        *(s32 *)(arg0 + 0x1C) = *(s32 *)(p + 0x1C);
                        *(s16 *)(arg0 + 0x44) = i;
                        p[0x64] = 1;
                        break;
                    }
                }
            }
        }
    }
    *(s32 *)(arg0 + 0x28) = 0;
    *(s32 *)(arg0 + 0x2C) = 0;
    *(s16 *)(arg0 + 0x46) = 0;
}

void tt_bomb_target_age(u8 *arg0) {
    if ((u32)(u16)++*(u16 *)(arg0 + 0x46) >= 0x79) {
        arg0[0] = 0;
    }
}

void tt_bomb_target_scroll(u8 *arg0) {
    *(s32 *)(arg0 + 0x24) = *tt_stage_script - *(s32 *)(arg0 + 0x1C);
    *(s32 *)(arg0 + 0x20) += *(s32 *)(arg0 + 0x28);
    *(s32 *)(arg0 + 0x24) += *(s32 *)(arg0 + 0x2C);
    *(s16 *)(arg0 + 0x14) = *(s16 *)(arg0 + 0x22);
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x26);
}

void tt_bomb_targets_update(void) {
    s32 index;
    u16 state;
    TtBomb *record;

    record = (TtBomb *)tt_bomb_targets;
    index = 0;
    do {
        if (record->active != 0) {
            state = record->state;
            switch (state) {
            case 0x20:
                tt_bomb_target_init((u8 *)record, index);
                record->state = 0x40U;
                record->step2 = 0;
                record->step = 0;
                record->phase = 0;
                record->tick2 = 0;
                record->tick1 = 0;
                record->tick0 = 0;
                break;
            case 0x40:
                tt_bomb_target_age((u8 *)record);
                break;
            case 0x80:
                record->active = 0;
                break;
            }
            /* FAKE: the dead bound test gives 8 its original saved register;
             * no extra instructions are emitted. T-10020. */
            if (index != 8) {}
            tt_bomb_target_scroll((u8 *)record);
        }
        index += 1;
        record++;
    } while (index != 8);
}

void tt_bomb_targets_draw(void) {
    DR_MODE *m;
    TtDrawBuffer *buffer;
    TtSpritePacket *q;
    s32 i;
    u8 *bomb_target;

    bomb_target = tt_bomb_targets;
    buffer = (TtDrawBuffer *)(tt_frames + ((TtVideoView *)tt_video)->index * 0x80B8);
    for (i = 0; i != 8; i++) {
        if (bomb_target[0] != 0) {
            q = (TtSpritePacket *)buffer->cursor;
            buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x14);
            q->tag.len = 4; q->code = 0x65; /* one line (setlen/setcode macro style) */
            q->x = *(s16 *)(bomb_target + 0x14) - 8;
            q->y = *(s16 *)(bomb_target + 0x16) - 8;
            q->h = 0x10;
            q->w = 0x10;
            q->u = bomb_target[0x42];
            q->v = bomb_target[0x43];
            q->clut = *(u16 *)(bomb_target + 0x40);
            q->tag.addr = buffer->ordering[*(u16 *)(bomb_target + 0x10)].addr;
            buffer->ordering[*(u16 *)(bomb_target + 0x10)].addr = (u32)q;
            m = buffer->cursor;
            buffer->cursor++;
            func_8009D294((s32)m, 0, 0, *(u16 *)(bomb_target + 0x3E), 0);
            ((TtOtTag *)m)->addr = buffer->ordering[*(u16 *)(bomb_target + 0x10)].addr;
            buffer->ordering[*(u16 *)(bomb_target + 0x10)].addr = (u32)m;
        }
        if ((s32)bomb_target != 8) {} /* FAKE: a second use of the bound gives it a saved register ahead of i. T-11800 */
        bomb_target += 0x58;
    }
}

void tt_title_code_check(void) {
    s32 pad; /* FAKE: moves the spill slot of core to sp+0x28. T-11800 */
    u8 *core;

    core = tt_core;
    if ((*(u16 *)(tt_sys + 0x14) & tt_title_code_masks[core[0x21]]) == tt_title_code_masks[core[0x21]]) {
        core[0x20] = 0x10;
        if (++core[0x21] == 6) {
            tt_se_request(0x607);
            core[0x21] = 0;
            core[0x24] = 1;
            core[0x25] = 0;
            tt_texts[0x78] = 1;
        }
    } else if (*(u16 *)(*(s32 *)&tt_sys + 0x14) != 0) { /* FAKE: the s32 view stops uopt reusing the first read. T-11800 */
        core[0x21] = 0;
    }
    if (--core[0x20] == 0) {
        core[0x21] = 0;
    }
}

void tt_title_menu_update(void) {
    u8 *scene;
    u8 *state;
    u16 pad1; /* FAKE: pad1..pad3 reproduce the original frame (run at sp+0x44). T-11860 */
    u16 pad2;
    s32 pad3;
    u8 *run;
    u8 *input;
    u8 *sprite;
    u8 *text_record;
    u32 max;

    scene = tt_sys;
    input = scene + 0x10;
    state = tt_core;
    run = tt_run;
    sprite = tt_sprites;
    text_record = tt_texts;
    max = 3;
    if (scene[0x53] == 0) {
        max = 2;
    }
    text_record[run[0xA] * 0x14 + 2] = 0;
    if (*(u16 *)(input + 4) & 0x1000) {
        tt_se_request(0x613);
        *(s16 *)(state + 0x26) = 0;
        if (--run[0xA] == 0xFF) {
            run[0xA] = max;
        }
    } else if (*(u16 *)(input + 4) & 0x4000) {
        tt_se_request(0x613);
        *(s16 *)(state + 0x26) = 0;
        if (++run[0xA] > max) {
            run[0xA] = 0;
        }
    }
    text_record[run[0xA] * 0x14 + 2] = 2;
    sprite[0] = 1;
    sprite[1] = 0;
    *(s16 *)(sprite + 0x44) = 0;
    *(s16 *)(sprite + 0x14) = -0x2C;
    *(s16 *)(sprite + 0x16) = run[0xA] * 8 + 0xA0;
    *(u16 *)(sprite + 0x10) = 1;
    sprite[0x3B] = 0x63;
    *(s16 *)(sprite + 0x3C) = 0x58; *(s16 *)(sprite + 0x3E) = 8; /* FAKE: one line keeps the original li order (as1 schedules by line). T-11860 */
    sprite[0x38] = 0x80;
    sprite[0x39] = 0x40;
    sprite[0x3A] = 0x40;
    if ((*(u16 *)(input + 4) & 0x800) || (*(u16 *)(input + 4) & 0x20)) {
        audio_command(0xB1);
        tt_se_request(0x612);
        state[0x22] = 0;
        *(u16 *)(state + 2) += 1;
        *(s16 *)(state + 4) = 0;
        *(s16 *)(run + 0x10) = 0;
        *(s16 *)(run + 0x12) = 0;
        tt_title_logo_hide();
    }
    if (scene[0x53] == 1 && state[0x24] == 0) {
        tt_title_code_check();
    }
}

void tt_title_menu_init(void) {
    u8 *scene;
    u8 *sprite;
    u8 *text_record;
    s32 i;

    scene = tt_sys;
    sprite = tt_sprites;
    text_record = tt_texts;
    func_8009C5E0(1);
    for (i = 0; i < 4; i++, text_record += 0x14) {
        *(s16 *)(text_record + 0xA) = tt_title_menu_x[i];
        *(s16 *)(text_record + 0xC) = i * 8 + 0xA0;
        *(u16 *)(text_record + 0xE) = tt_title_menu_text[i];
        text_record[2] = 0;
        text_record[0] = 1;
        *(s16 *)(text_record + 6) = 0;
        *(s16 *)(text_record + 0x10) = 0;
    }
    if (scene[0x53] == 0) {
        text_record[-0x14] = 0;
    }
    for (i = 0; i != 2; i++, text_record += 0x14) {
        *(s16 *)(text_record + 0xA) = tt_title_hint_x[i];
        *(s16 *)(text_record + 0xC) = i * 8 + 0xD0;
        *(u16 *)(text_record + 0xE) = tt_title_hint_text[i];
        text_record[2] = 0;
        text_record[0] = 1;
        *(s16 *)(text_record + 6) = 0;
        *(s16 *)(text_record + 0x10) = 0;
    }
    *(s16 *)(text_record + 0xA) = -0x70;
    *(s16 *)(text_record + 0xC) = 0x88;
    *(s16 *)(text_record + 0xE) = 0;
    text_record[2] = 3;
    text_record[0] = 0;
    *(s16 *)(text_record + 6) = 0;
    *(s16 *)(text_record + 0x10) = 0;
    sprite += 0x64;
    sprite[0] = 0;
    sprite[1] = 0;
    *(s16 *)(sprite + 0x44) = 0;
    *(s16 *)(sprite + 0x14) = -0x60;
    *(s16 *)(sprite + 0x16) = 0x38;
    *(s16 *)(sprite + 0x3C) = 0x80;
    *(s16 *)(sprite + 0x3E) = 0x30;
    sprite[0x46] = 0;
    sprite[0x47] = 0;
    *(s16 *)(sprite + 0x40) = 0xB;
    *(s16 *)(sprite + 0x42) = 0x3C20;
    if (sprite) {} /* FAKE: splits the uopt block so 0x65 can take v0. T-11820, still needed T-11850 */
    sprite[0x3B] = 0x65;
    *(s16 *)(sprite + 0x10) = 0;
    sprite += 0x64;
    sprite[0] = sprite[1] = 0; /* FAKE: two-store chain; it uses up ugen temp t9, so 0x20/0x40/0x30 get t1/t2/t3 as in the original. T-11850 */
    *(s16 *)(sprite + 0x44) = 0;
    *(s16 *)(sprite + 0x14) = 0x20;
    *(s16 *)(sprite + 0x16) = 0x38;
    *(s16 *)(sprite + 0x3C) = 0x40;
    *(s16 *)(sprite + 0x3E) = 0x30;
    sprite[0x46] = 0;
    sprite[0x47] = 0x30;
    *(s16 *)(sprite + 0x40) = 0xB;
    *(s16 *)(sprite + 0x42) = 0x3C20;
    sprite[0x3B] = 0x65;
    *(s16 *)(sprite + 0x10) = 0;
}

void tt_title_logo_init(void) {
    u8 *q;
    u8 *run;
    u8 *o;
    s32 i;

    run = tt_run;
    q = tt_transforms + 0x10;
    *(s16 *)(q + 4) = -0x100;
    *(s16 *)(q + 6) = 0x44;
    *(s16 *)q = 0x1000;
    *(s16 *)(q + 2) = 0x1000;
    *(s16 *)(q + 0x14) = 0x100;
    *(s16 *)(q + 0x16) = 0x44;
    *(s16 *)(q + 0x10) = 0x1000;
    *(s16 *)(q + 0x12) = 0x1000;
    o = tt_sprites + 0xFA0;
    for (i = 0; i < 4; i++, o += 0x64) {
        o[0] = 1;
        o[1] = 0;
        *(s16 *)(o + 0x44) = 0;
        o[0x3B] = 0x80;
        o[0x4D] = 0;
        o[0x4E] = 1;
        o[0x4F] = 1;
        o[0x50] = 0;
        *(s16 *)(o + 0x40) = 0x19;
        *(s16 *)(o + 0x42) = 0x3C07;
        o[0x4C] = 0;
        *(s16 *)(o + 0x10) = 0;
    }
    o = tt_sprites + 0xFA0;
    o[0x46] = 0;
    o[0x47] = 0x80;
    *(s16 *)(o + 0x3C) = 0x50;
    *(s16 *)(o + 0x3E) = 0x20;
    *(s32 *)(o + 0x20) = (*(s16 *)(q + 0x14) - 0x6C) << 16;
    *(s32 *)(o + 0x24) = 0x500000;
    *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
    *(s16 *)(o + 0x16) = *(s16 *)(o + 0x26);
    *(s32 *)(o + 0x28) = 0;
    *(s32 *)(o + 0x2C) = 0;
    *(u8 **)(o + 0x48) = q + 0x10;
    o += 0x64;
    o[0x47] = 0xA0;
    o[0x46] = 0;
    *(s16 *)(o + 0x3C) = 0x80;
    *(s16 *)(o + 0x3E) = 0x20;
    *(s32 *)(o + 0x20) = (*(s16 *)(q + 0x14) - 0x18) << 16;
    *(s32 *)(o + 0x24) = 0x500000;
    *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
    *(s16 *)(o + 0x16) = *(s16 *)(o + 0x26);
    *(s32 *)(o + 0x28) = 0;
    *(s32 *)(o + 0x2C) = 0;
    *(u8 **)(o + 0x48) = q + 0x10;
    o += 0x64;
    o[0x47] = 0;
    o[0x46] = 0;
    *(s16 *)(o + 0x3C) = 0x80;
    *(s16 *)(o + 0x3E) = 0x40;
    *(s32 *)(o + 0x20) = (*(s16 *)(q + 4) - 0x60) << 16;
    *(s32 *)(o + 0x24) = 0x180000;
    *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
    *(s16 *)(o + 0x16) = *(s16 *)(o + 0x26);
    *(s32 *)(o + 0x28) = 0;
    *(s32 *)(o + 0x2C) = 0;
    *(u8 **)(o + 0x48) = q;
    o += 0x64;
    o[0x47] = 0x40;
    o[0x46] = 0;
    *(s16 *)(o + 0x3C) = 0x40;
    *(s16 *)(o + 0x3E) = 0x40;
    *(s32 *)(o + 0x20) = (*(s16 *)(q + 4) + 0x20) << 16;
    *(s32 *)(o + 0x24) = 0x180000;
    *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
    *(s16 *)(o + 0x16) = *(s16 *)(o + 0x26);
    *(s32 *)(o + 0x28) = 0;
    *(s32 *)(o + 0x2C) = 0;
    *(u8 **)(o + 0x48) = q;
    *(s16 *)(run + 0x12) = 0;
}

void tt_title_logo_hide(void) {
    u8 *q;
    u8 *o;

    q = tt_transforms + 0x10;
    *(s16 *)(q + 4) = 0;
    *(s16 *)q = *(s16 *)(q + 2) = 0x1000;
    *(s16 *)(q + 0x14) = 0;
    *(s16 *)(q + 0x10) = *(s16 *)(q + 0x12) = 0x1000;
    o = tt_sprites + 0xFA0;
    *(s32 *)(o + 0x20) = 0xFF940000;
    o[0x4F] = 0;
    *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
    o += 0x64;
    *(s32 *)(o + 0x20) = 0xFFE80000;
    o[0x4F] = 0;
    *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
    o += 0x64;
    *(s32 *)(o + 0x20) = 0xFFA00000;
    o[0x4F] = 0;
    *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
    o += 0x64;
    *(s32 *)(o + 0x20) = 0x200000;
    o[0x4F] = 0;
    *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
}

/* T-11800: matched by walking the four 0x64-byte records (tt_sprites + 0xFA0, then o += 0x64 three times); uopt folds the walk into the original tt_sprites + 0x10CC rebase. */
void tt_title_logo_update(void) {
    u8 *core;
    u8 *q;
    u8 *run;
    u8 *o;
    s32 ang;

    core = tt_core;
    q = tt_transforms + 0x10;
    run = tt_run;
    switch (*(u16 *)(core + 4)) {
    case 0x40:
        ang = *(u16 *)(run + 0x12);
        *(s16 *)(q + 4) = -0x80 - func_800A0140(ang) / 32;
        *(s16 *)q = *(s16 *)(q + 2) = 0x1000 - rsin(ang) / 2;
        *(s16 *)(q + 0x14) = func_800A0140(ang) / 32 + 0x80;
        *(s16 *)(q + 0x10) = *(s16 *)(q + 0x12) = rsin(ang) / 2 + 0x1000;
        o = tt_sprites + 0xFA0;
        *(s32 *)(o + 0x20) = (*(s16 *)(q + 0x14) - 0x6C) << 16;
        *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
        o += 0x64;
        *(s32 *)(o + 0x20) = (*(s16 *)(q + 0x14) - 0x18) << 16;
        *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
        o += 0x64;
        *(s32 *)(o + 0x20) = (*(s16 *)(q + 4) - 0x60) << 16;
        *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
        o += 0x64;
        *(s32 *)(o + 0x20) = (*(s16 *)(q + 4) + 0x20) << 16;
        *(s16 *)(o + 0x14) = *(s16 *)(o + 0x22);
        *(u16 *)(run + 0x12) += 0x18;
        if (*(u16 *)(run + 0x12) >= 0x800) {
            *(s16 *)(core + 4) = 0xA0;
        }
        break;
    case 0xA0:
        tt_title_logo_hide();
        break;
    }
}

void tt_title_ship_init(void) {
    u8 *player;
    u8 *arm;
    u32 i;

    player = tt_player;
    player[0] = 1;
    player[0xC] = 0;
    *(s16 *)(player + 4) = 0;
    player[0xD] = 0;
    player[0xE] = 0;
    player[0xF] = 0;
    *(s16 *)(player + 6) = 0;
    *(s16 *)(player + 8) = 0;
    *(s16 *)(player + 0xA) = 0;
    *(s16 *)(player + 0x10) = 2;
    *(s32 *)(player + 0x20) = 0x480000;
    *(s32 *)(player + 0x24) = 0x840000;
    *(s16 *)(player + 0x14) = 0x48;
    *(s16 *)(player + 0x16) = 0x9C;
    *(s32 *)(player + 0x28) = 0;
    *(s32 *)(player + 0x2C) = 0;
    *(u8 **)(player + 0x38) = tt_sprite_set_table;
    player[0x3C] = 0;
    player[0x3D] = 0xB;
    player[0x3E] = 4;
    player[0x40] = 0x60;
    player[0x48] = 0;
    tt_sprite_animation_step(player + 0x38);
    *(s16 *)(player + 0x50) = 0;
    *(s16 *)(player + 0x6A) = 0;
    arm = tt_arms;
    i = 0;
    do {
        i++;
        arm += 0x60;
        arm[-0x60] = 0;
        *(s32 *)(arm - 0x28) = 0;
    } while (i != 2);
}

void tt_title_ship_bob(void) {
    u8 *player = tt_player;

    *(s32 *)(player + 0x24) = rsin(*(u16 *)(tt_player + 0x6A)) * 0x78 + 0x9C0000;
    *(s16 *)(player + 0x16) = *(s16 *)(player + 0x26);
    *(u16 *)(player + 0x6A) += 0x10;
}

void tt_title_ship_start_sequence(void) {
    u8 *player;
    u8 *run;
    u8 *core;
    u8 *text_record;

    player = tt_player;
    core = tt_core;
    run = tt_run;
    text_record = tt_texts;
    /* abs(x - 0x9C0000) written as the usual ABS() expansion; locals for x or the difference give other registers. T-11810 */
    if ((*(s32 *)(player + 0x24) - 0x9C0000 < 0 ? -(*(s32 *)(player + 0x24) - 0x9C0000) : *(s32 *)(player + 0x24) - 0x9C0000) < 0x8001) {
        *(s32 *)(player + 0x24) = 0x9C0000;
    } else if (*(s32 *)(player + 0x24) < 0x9C0000) {
        *(s32 *)(player + 0x24) += 0x8000;
    } else {
        *(s32 *)(player + 0x24) -= 0x8000;
    }
    *(s16 *)(player + 0x16) = *(s16 *)(player + 0x26);
    if (*(u16 *)(run + 0x10) >= 0x1E) {
        *(u16 *)(run + 0x10) = 0;
        text_record[0] = 1;
        *(u16 *)(core + 2) += 1;
    }
    switch (*(u16 *)(run + 0x10)) {
    case 0:
        player[0x3E] = 5;
        break;
    case 1:
        tt_se_request(0x601);
        break;
    case 6:
        player[0x3E] = 6;
        break;
    case 30:
        player[0x3E] = 7;
        break;
    }
    *(u16 *)(run + 0x10) += 1;
}

void tt_title_ship_exit_sequence(void) {
    s32 i;
    u8 *a64;
    u8 *a6C;
    u8 *a74;
    u8 *p;
    u8 *aA8;

    a64 = tt_core;
    a6C = tt_run;
    a74 = tt_player;
    p = tt_sprites + 0xFA0;
    aA8 = tt_texts;
    if (*(u16 *) (a6C + 0x10) >= 0x2C) {
        *(u16 *) (a6C + 0x10) = 0;
        *(u16 *) (a64 + 2) += 1;
    }
    switch (*(u16 *) (a6C + 0x10)) {
    case 8:
        a74[0x3E] = 8;
        break;
    case 16:
        a74[0x3E] = 9;
        break;
    case 22:
        a74[0x3E] = 0xA;
        break;
    case 28:
        a74[0x3E] = 0xB;
        break;
    case 34:
        a74[0x3E] = 0xC;
        break;
    case 40:
        a74[0x3E] = 0xD;
        break;
    case 42:
        a74[0x3E] = 0xE;
        break;
    case 44:
        a74[0x3E] = 0xF;
        break;
    }
    for (i = 0; i < 4; i++) {
        *(s32 *) (p + 0x24) += 0xFFFC0000;
        *(s16 *) (p + 0x16) = *(s16 *) (p + 0x26);
        p += 0x64;
    }
    for (i = 0; i < 7; i++) {
        *(s16 *) (aA8 + 0xC) += 4;
        aA8 += 0x14;
    }
    tt_texts[0x78] = 0;
    *(s16 *) (tt_sprites + 0x16) += 4;
    *(u16 *) (a6C + 0x10) += 1;
}

void tt_title_logo_zoom(void) {
    u8 *sprite;
    u8 *core;
    u8 *transform;
    u8 *p;
    s16 t;

    sprite = tt_sprites;
    core = tt_core;
    p = sprite + 0xC80;
    transform = tt_transforms;
    if (*(u16 *)(sprite + 0xCDA) >= 0x1000) {
        *(u16 *)(core + 2) += 1;
    }
    *(u32 *)(p + 0x20) = 0x18000000 / *(u16 *)(p + 0x5A);
    *(s32 *)(p + 0x24) = 0x6C0000 - 0x3600 / *(u16 *)(p + 0x5A);
    *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
    *(s16 *)(transform + 4) = *(s16 *)(p + 0x14);
    *(s16 *)(transform + 6) = *(s16 *)(p + 0x16) + 0x18;
    t = 0x180000 / *(u16 *)(p + 0x5A);
    *(s16 *)(transform + 2) = t;
    *(s16 *)(transform + 0) = t;
    if (*(u16 *)(p + 0x52) & 1) {
        p[0x46] = 0x80;
        p[0x47] = 0;
    } else {
        p[0x46] = 0;
        p[0x47] = 0x80;
    }
    *(u16 *)(p + 0x52) += 1;
    *(u16 *)(p + 0x5A) += 0x40;
}

void tt_state_title(void) {
    u8 *s;
    u8 *e;
    u8 *sys;
    s32 pad1; /* FAKE: stack slot of the original frame. T-11840 */
    u8 *run;
    u8 *y;
    u8 *p;
    u8 *f;
    s32 pad3; /* FAKE: stack slot of the original frame. T-11840 */
    u32 k; /* one unsigned local for both selectors: $v1 in cases 2 and 9, compare constant 1 shared. T-11840 */

    s = tt_core;
    p = tt_sprites + 0xC80;
    sys = tt_sys;
    run = tt_run;
    e = tt_player;
    y = tt_transforms;
    switch (*(u16 *)(s + 2)) {
    case 0:
        if (*(u16 *)(tt_sys + 0xA4) != 0) {
            tt_cd_read_start(0x80016000, 0xC812, 0x2420);
            tt_cd_read_start(0x8002E800, 0xC7C1, 0x69E4);
            tt_cd_read_start(0x8019A000, 0xC7C5, 0x27D60);
            tt_cd_read_start(0x8001A000, 0xBFD1, 0xE20);
            tt_cd_read_start(0x801C2000, 0xBF9C, 0x1A680);
        }
        *(u16 *)(s + 2) = 1;
        break;
    case 1:
        audio_command(0x22);
        audio_command(0x2F);
        audio_command(0x205);
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
        s[0x11] = 0;
        s[0x24] = 0;
        *(u8 ***)(s + 0x18) = tt_title_tile_map;
        s[0x1C] = 0x20;
        tt_title_menu_init();
        tt_title_logo_init();
        tt_title_ship_init();
        *(s16 *)(run + 0x10) = 0;
        run[0xA] = 0;
        *(u16 *)(s + 0x26) = 0;
        *(u16 *)(s + 2) += 1;
        *(s16 *)(s + 4) = 0x40;
        break;
    case 2:
        if (*(u16 *)(tt_sys + 0xA4) != 0) {
            k = cd_done_count();
            if ((s32)((k - tt_cd_read_counter_before() + 0x100) & 0xFF) >= 5) {
                audio_command(0x12);
                *(u16 *)(s + 2) += 1;
            }
        } else {
            *(u16 *)(s + 2) = 4;
        }
        tt_title_ship_bob();
        tt_title_logo_update();
        break;
    case 3:
        if (*(u16 *)(tt_sys + 0xA4) != 0) {
            *(u16 *)(sys + 0xA4) = 0;
            if (func_80044F94(1) != 0) {
                *(u16 *)(s + 2) += 1;
            }
        }
        tt_title_ship_bob();
        tt_title_logo_update();
        break;
    case 4:
        if (*(u16 *)(tt_core + 0x26) >= 0x708) {
            s[0x24] = 0;
            s[0x22] = 1;
            sys[0x52] = 0xFF;
            *(u8 **)(sys + 0x4C) = tt_demo_input_attract;
            *(s16 *)s = 2;
            s[9] = 0;
            s[0xA] = 0;
            s[0xB] = 0;
            *(u16 *)(s + 2) = 0;
            *(s16 *)(s + 4) = 0;
            *(s16 *)(s + 6) = 0;
        }
        tt_title_ship_bob();
        tt_title_logo_update();
        tt_title_menu_update();
        *(u16 *)(s + 0x26) += 1;
        break;
    case 5:
        f = tt_texts + run[0xA] * 0x14;
        *f = *(u16 *)(sys + 8) & 1;
        if (run[0xA] == 0) {
            tt_title_ship_start_sequence();
        } else {
            if (*(u16 *)(run + 0x10) >= 0x1E) {
                *(u16 *)(run + 0x10) = 0;
                *(u16 *)(s + 2) = 9;
            }
            *(u16 *)(run + 0x10) += 1;
        }
        break;
    case 6:
        tt_title_ship_exit_sequence();
        break;
    case 7:
        /* Record init (as in tt_title_logo_init), then the position: the original ugen order. T-11870 */
        *e = 0;
        *(s16 *)(p + 0x52) = 0;
        *(s16 *)(p + 0x5A) = 0x180;
        p[0] = 1;
        p[1] = 0;
        *(s16 *)(p + 0x44) = 0;
        p[0x3B] = 0x80;
        p[0x4D] = 0;
        p[0x4E] = 1;
        p[0x50] = 0;
        p[0x46] = 0x80;
        p[0x47] = 0;
        p[0x4F] = 1;
        *(s16 *)(p + 0x3C) = 0x80;
        *(s16 *)(p + 0x3E) = 0x80;
        *(s16 *)(p + 0x40) = 0x1A;
        *(s16 *)(p + 0x42) = 0x3D04;
        p[0x4C] = 0;
        *(s16 *)(p + 0x10) = 2;
        *(s32 *)(p + 0x20) = 0x100000;
        *(s32 *)(p + 0x24) = 0x6C0000;
        *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x2C) = 0;
        *(s16 *)(y + 4) = *(s16 *)(p + 0x14);
        *(s16 *)(y + 6) = *(s16 *)(p + 0x16);
        *(s16 *)(y + 2) = 0x1000;
        *(s16 *)y = *(s16 *)(y + 2);
        *(u8 **)(p + 0x48) = y;
        tt_se_request(0x614);
        *(u16 *)(s + 2) += 1;
        break;
    case 8:
        tt_title_logo_zoom();
        break;
    case 9:
        k = tt_run[0xA];
        switch (k) {
        case 0:
            *(s16 *)s = 2;
            break;
        case 1:
            *(s16 *)s = 7;
            break;
        case 2:
            *(s16 *)s = 3;
            break;
        case 3:
            *(s16 *)s = 8;
            break;
        }
        s[9] = 1;
        break;
    }
    if (s[9] != 0) {
        *(s16 *)(s + 6) = 0;
        *(s16 *)(s + 4) = 0;
        *(u16 *)(s + 2) = 0;
        s[0xB] = 0;
        s[0xA] = 0;
        s[9] = 0;
    }
}

void tt_shots_clear(void) {
    s32 i;
    u8 *p;

    for (i = 0, p = tt_shots; i < 12; i++, p += 0x54) {
        *p = 0;
    }
}

void tt_shots_reset(void) {
    s32 i;
    u8 *shot;
    int new_var; /* FAKE: the dead test of tt_shots reserves two registers (a0/a1) so the hoisted constants land in a2..t2 as in the original; found with decomp-permuter. T-10910 */

    for (i = 0, shot = tt_shots; i < 12; i++, shot += 0x54) {
        shot[0] = 0;
        new_var = tt_shots && tt_shots;
        *(s16 *)(shot + 0x10) = 3;
        *(s16 *)(shot + 0x3E) = 0xA;
        *(s16 *)(shot + 0x40) = 0x3C87;
        *(s16 *)(shot + 4) = 0x40;
        *(s16 *)(shot + 10) = 0;
        *(s16 *)(shot + 8) = 0;
        *(s16 *)(shot + 6) = 0;
        shot[0xF] = 0;
        if (new_var && tt_shots) { /* FAKE: see above. T-10910 */
        }
        shot[0xE] = 0;
        shot[0xD] = 0;
    }
}

void tt_shot_set_kind(u8 *arg0, u16 arg1) {
    *(u16 *)(arg0 + 2) = arg1;
    switch (arg1) {
    case 8:
    default:
        arg0[0x42] = 0x80;
        arg0[0x43] = 0xC0;
        break;
    case 9:
        arg0[0x42] = 0x90;
        arg0[0x43] = 0xC0;
        break;
    case 10:
        arg0[0x42] = 0x80;
        arg0[0x43] = 0xD0;
        break;
    }
}

void tt_shot_launch_init(u8 *arg0, s32 arg1) {
    u8 *player;
    u8 *q;
    u8 *r;

    player = tt_player;
    q = tt_options;
    if (arg1 < 6) {
        tt_shot_set_kind(arg0, player[0x55]);
    } else {
        tt_shot_set_kind(arg0, q[0x92]);
    }
    if (arg1 < 6) {
        *(s32 *)(arg0 + 0x20) = *(s16 *)(player + 0x14) << 16;
        *(s32 *)(arg0 + 0x24) = *(s16 *)(player + 0x16) << 16;
    } else if (arg1 < 8) {
        q = tt_options;
        *(s32 *)(arg0 + 0x20) = *(s16 *)(q + 0x50) << 16;
        *(s32 *)(arg0 + 0x24) = *(s16 *)(q + 0x70) << 16;
    } else if (arg1 < 10) {
        q = tt_options;
        r = q + 0xA4;
        *(s32 *)(arg0 + 0x20) = *(s16 *)(q + 0xF4) << 16;
        *(s32 *)(arg0 + 0x24) = *(s16 *)(r + 0x70) << 16;
    } else {
        q = tt_options;
        r = q + 0x148;
        *(s32 *)(arg0 + 0x20) = *(s16 *)(q + 0x198) << 16;
        *(s32 *)(arg0 + 0x24) = *(s16 *)(r + 0x70) << 16;
    }
    if (arg1 >= 2 && arg1 < 4) {
        *(s32 *)(arg0 + 0x28) = 0xFFFD0000;
    } else if (arg1 >= 4 && arg1 < 6) {
        *(s32 *)(arg0 + 0x28) = 0x30000;
    } else {
        *(s32 *)(arg0 + 0x28) = 0;
    }
    *(s32 *)(arg0 + 0x2C) = 0xFFFA0000;
}

void tt_shot_move(u8 *arg0) {
    *(s32 *)(arg0 + 0x20) += *(s32 *)(arg0 + 0x28);
    *(s32 *)(arg0 + 0x24) += *(s32 *)(arg0 + 0x2C);
    *(s16 *)(arg0 + 0x14) = *(s16 *)(arg0 + 0x22);
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x26);
    if (*(s16 *)(arg0 + 0x16) < 0) {
        *(s16 *)(arg0 + 4) = 0xA0;
        *(s16 *)(arg0 + 0xA) = 0;
        *(s16 *)(arg0 + 8) = 0;
        *(s16 *)(arg0 + 6) = 0;
        arg0[0xF] = 0;
        arg0[0xE] = 0;
        arg0[0xD] = 0;
    }
}

void tt_shots_update(void) {
    s32 i;
    u32 limit;
    u16 state;
    TtShot *record;

    record = (TtShot *)tt_shots;
    i = 0;
    for (limit = 12; i != limit;) {
        if (record->active != 0) {
            state = record->state;
            switch (state) {
            case 0x20:
                tt_shot_launch_init((u8 *)record, i);
                record->state = 0x40U;
                record->step2 = 0;
                record->step = 0;
                record->phase = 0;
                record->tick2 = 0;
                record->tick1 = 0;
                record->tick0 = 0;
                break;
            case 0x80:
                record->u = 0xD0;
                record->v = 0xD0;
                record->hit_timer = 1U;
                record->state = 0x81U;
                record->step2 = 0;
                record->step = 0;
                record->phase = 0;
                record->tick2 = 0;
                record->tick1 = 0;
                record->tick0 = 0;
                record->velocity_y = 0;
                record->velocity_x = 0;
                break;
            case 0x81:
                if (--record->hit_timer == 0) {
                    record->state = 0x82U;
                }
                break;
            case 0x82:
                record->u = 0xE0;
                record->v = 0xD0;
                record->hit_timer = 1U;
                record->state = 0x83U;
                break;
            case 0x83:
                if (--record->hit_timer == 0) {
                    record->state = 0xA0U;
                }
                break;
            case 0xA0:
                record->active = 0;
                break;
            }
            tt_shot_move((u8 *)record);
        }
        i += 1;
        record++;
    }
}

void tt_shots_draw(void) {
    DR_MODE *m;
    TtDrawBuffer *buffer;
    TtSpritePacket *q;
    s32 i;
    u8 *shot;

    shot = tt_shots;
    buffer = (TtDrawBuffer *)(tt_frames + ((TtVideoView *)tt_video)->index * 0x80B8);
    for (i = 0; i != 12; i++) {
        if (shot[0] != 0) {
            q = (TtSpritePacket *)buffer->cursor;
            buffer->cursor = (DR_MODE *)((u8 *)buffer->cursor + 0x14);
            q->tag.len = 4; q->code = 0x65; /* one line (setlen/setcode macro style) */
            q->x = *(s16 *)(shot + 0x22) - 8;
            q->y = *(s16 *)(shot + 0x26) - 8;
            q->h = 0x10;
            q->w = 0x10;
            q->u = shot[0x42];
            q->v = shot[0x43];
            q->clut = *(u16 *)(shot + 0x40);
            q->tag.addr = buffer->ordering[*(u16 *)(shot + 0x10)].addr;
            buffer->ordering[*(u16 *)(shot + 0x10)].addr = (u32)q;
            m = buffer->cursor;
            buffer->cursor++;
            func_8009D294((s32)m, 0, 0, *(u16 *)(shot + 0x3E), 0);
            ((TtOtTag *)m)->addr = buffer->ordering[*(u16 *)(shot + 0x10)].addr;
            buffer->ordering[*(u16 *)(shot + 0x10)].addr = (u32)m;
        }
        if ((s32)shot != 12) {} /* FAKE: a second use of the bound gives it a saved register ahead of i. T-11800 */
        shot += 0x54;
    }
}
