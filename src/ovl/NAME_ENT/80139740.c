#define MAIN_API_OVERRIDE_prim_gradient_rect /* Original calls pass promoted integer coordinates. T-10880. */
#define MAIN_API_OVERRIDE_card_start /* The original reads $v0 after the call in func_8013D2AC. T-22023. */
#include "common.h"
#include "ovl/NAME_ENT.h"

void prim_gradient_rect();
s32 card_start();

s32 func_80139740(u32 arg0, s32 arg1, s32 arg2) {
    u8 *p;
    s32 n;
    u32 x;
    u8 *e;

    x = arg0;
    for (n = 0;; n += 8) {
        p = (u8 *)GetWorkBase(0x10, draw_buffer);
        func_8009F090(p);
        SetSemiTrans(p, 0);
        p[4] = 0x80;
        p[5] = 0x80;
        p[6] = 0x80;
        *(s16 *)(p + 8) = game_state.screen_center_x[draw_buffer] + arg1 - n;
        *(s16 *)(p + 0xA) = game_state.screen_center_y[draw_buffer] + arg2;
        e = D_8014D0F4[x % 10];
        p[0xC] = e[0];
        p[0xD] = e[1];
        *(s16 *)(p + 0xE) = func_8009ED74(0, 0x1F0);
        AddPrim(D_800E8CA0 + (draw_buffer << 10) + 0xC, p);
        if (x < 10) {
            dtd_on_tpage(0x200, 0, 3, 0, 1);
            return 1;
        }
        x = x / 10;
    }
}

/* T-25057: record display flags and animated tile coordinates. */
void func_801398EC(void) {
    s32 tmp;
    s32 i;
    u32 tile[1]; /* FAKE: T-25057, memory local orders remainder before frame allocation. */

    i = 0;
    tmp = D_8014D0D4 % 3;
    for (; i < 13; i++) {
        if ((u32)sprite_quads[i].flags < 0) {} /* FAKE: T-25057, erased test orders record induction. */
        tile[0] = game_state.frame_count; /* FAKE: T-25057, dead initialization orders frame before index; removed by uopt. */
        if (tmp == 0) {
            if (((AlbumSlotRecord *)card_block)[D_800E8BEF].flags[i] == 0) {
                sprite_quads[i].flags = 0;
                tile[0] = (game_state.frame_count >> 2) % 3;
                sprite_quads[i].unk_0E = (i / 8) * 0x30 - (0U - tile[0] * 0x10) + 0x80; /* FAKE: T-25057, unsigned identity preserves row-before-tile evaluation. */
            } else {
                sprite_quads[i].flags = 0x80000000;
            }
        } else {
            sprite_quads[i].flags = 0x80000000;
        }
    }
}

void func_80139AD8(s32 arg0, s32 arg1, s32 arg2) {
    u8 *p;

    p = (u8 *)sprite_quads + arg0 * 36;
    p[0x14] = 0x80;
    p[0x15] = 0x80;
    p[0x16] = 0x80;
    *(s16 *)(p + 4) = arg1;
    *(s16 *)(p + 6) = arg2;
    *(s32 *)p = 0;
    *(s16 *)(p + 8) = 0x10;
    *(s16 *)(p + 0xA) = 0x10;
    p[0xE] = arg0 / 8 * 0x30 + game_state.frame_count / 32 % 3 * 0x10 + 0x80;
    p[0xF] = (arg0 & 7) * 0x10;
    *(s16 *)(p + 0xC) = 0x1F;
    *(s16 *)(p + 0x10) = arg0 * 0x10;
    *(s16 *)(p + 0x18) = 8;
    *(s16 *)(p + 0x1A) = 8;
    *(s16 *)(p + 0x12) = 0x1F8;
    game_state.quad_layer[arg0] = 2;
}

void func_80139BA0(void) {
    if (game_state.pad_pressed & PAD_TRIANGLE) {
        D_8014D0D4 += 1;
        return;
    }
    if ((game_state.pad_pressed & PAD_CIRCLE) && (cursor_x < 0x20) && (cursor_x >= -0x3F) && (cursor_y < 0x1C) && (cursor_y >= 0xD)) {
        D_8014D0D4 += 1;
    }
}

void func_80139C24(void) {
    if (D_8014D0D4 < 0) {
        D_8014D0D4 += 3;
    }
    switch (D_8014D0D4 % 3) {
    case 0:
        func_80139F30();
        break;
    case 1:
        func_8013ACA4();
        break;
    case 2:
        func_8013A9C8();
        break;
    }
    func_80139BA0();
}

void func_80139CB4(void) {
    u8 sp30[0x108]; /* FAKE: observed frame and buffer slot, T-9580. */

    if (D_800E8BEE != 0) {
        strncpy(sp30, D_800B3288[D_800E7D36], 5);
        strcat(sp30, "番");
        set_kanji_string(-0x20, -0x5C, 0xF, (s32) sp30, 0);
        set_kanji_string(0x20, -0x4B, 0xF, (s32) system_data.family_name[D_800E7D36], 0);
        set_kanji_string(0x4E, -0x4B, 0xF, (s32) system_data.given_name[D_800E7D36], 0);
        set_kanji_string(0x20, -0x3B, 0xF, (s32) system_data.nickname[D_800E7D36], 0);
        set_kanji_string(0x20, -0x2B, 0xF, (s32) club_name_table[system_data.club[D_800E7D36]], 0);
        return;
    }
    strncpy(sp30, D_800B3288[game_state.save_slot], 5);
    strcat(sp30, "番");
    set_kanji_string(-0x20, -0x5C, 0xF, (s32) sp30, 0);
    set_kanji_string(0x20, -0x4B, 0xF, (s32) ((AlbumSlotRecord *)card_block)[game_state.album_slot].family_name, 0);
    set_kanji_string(0x4E, -0x4B, 0xF, (s32) ((AlbumSlotRecord *)card_block)[game_state.album_slot].given_name, 0);
    set_kanji_string(0x20, -0x3B, 0xF, (s32) ((AlbumSlotRecord *)card_block)[game_state.album_slot].nickname, 0);
    set_kanji_string(0x20, -0x2B, 0xF, (s32) club_name_table[((AlbumSlotRecord *)card_block)[game_state.album_slot].club >> 0xC], 0);
}

void func_80139F30(void) {
    s32 i;
    SystemData *w = &system_data;
    GameState *g = &game_state;

    if (D_800E8BEE != 0) {
        if (w->flags[0] != 0) {
            func_80139740(w->flags[0], -4, 0x1E);
        }
        if (w->flags[1] != 0) {
            func_80139740(w->flags[1], -4, 0x2E);
        }
        if (w->flags[2] != 0) {
            func_80139740(w->flags[2], -4, 0x3E);
        }
        if (w->flags[3] != 0) {
            func_80139740(w->flags[3], -4, 0x4E);
        }
        if (w->flags[4] != 0) {
            func_80139740(w->flags[4], 0x3C, 0x1E);
        }
        if (w->flags[5] != 0) {
            func_80139740(w->flags[5], 0x3C, 0x2E);
        }
        if (w->flags[6] != 0) {
            func_80139740(w->flags[6], 0x3C, 0x3E);
        }
        if (w->flags[7] != 0) {
            func_80139740(w->flags[7], 0x3C, 0x4E);
        }
        if (w->flags[12] != 0) {
            func_80139740(w->flags[12], 0x88, 0xE);
        }
        if (w->flags[8] != 0) {
            func_80139740(w->flags[8], 0x88, 0x1E);
        }
        if (w->flags[9] != 0) {
            func_80139740(w->flags[9], 0x88, 0x2E);
        }
        if (w->flags[10] != 0) {
            func_80139740(w->flags[10], 0x88, 0x3E);
        }
        if (w->flags[11] != 0) {
            func_80139740(w->flags[11], 0x88, 0x4E);
        }
        for (i = 0; i < 4; i++) {
            if (w->flags[i] != 0) {
                func_80139AD8(i, -0x23, (i % 4) * 0x10 + 0x1D);
            }
        }
        for (i = 4; i < 8; i++) {
            if (w->flags[i] != 0) {
                func_80139AD8(i, 0x1E, (i % 4) * 0x10 + 0x1D);
            }
        }
        for (i = 8; i < 12; i++) {
            if (w->flags[i] != 0) {
                func_80139AD8(i, 0x69, (i % 4) * 0x10 + 0x1D);
            }
        }
        if (w->flags[12] != 0) {
            func_80139AD8(12, 0x69, 0xD);
        }
        for (i = 0; i < 8; i++) {
            if (w->flags[i] == 0) {
                ((Actor *)actor_pool)[i + 128].unk_01 = 4;
                ((Actor *)actor_pool)[i + 128].draw_flags |= 0x80;
            } else {
                ((Actor *)actor_pool)[i + 128].draw_flags &= 0x7F;
            }
        }
        for (i = 8; i < 12; i++) {
            if (w->flags[i] == 0) {
                ((Actor *)actor_pool)[i + 129].draw_flags |= 0x80;
                ((Actor *)actor_pool)[i + 129].unk_01 = 4;
            } else {
                ((Actor *)actor_pool)[i + 129].draw_flags &= 0x7F;
            }
        }
        if (w->flags[12] == 0) {
            ((Actor *)actor_pool)[136].draw_flags |= 0x80;
            ((Actor *)actor_pool)[136].unk_01 = 4;
        } else {
            ((Actor *)actor_pool)[136].draw_flags &= 0x7F;
        }
    } else {
        if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[0] != 0) {
            func_80139740(((AlbumSlotRecord *)card_block)[g->album_slot].flags[0], -4, 0x1E);
        }
        if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[1] != 0) {
            func_80139740(((AlbumSlotRecord *)card_block)[g->album_slot].flags[1], -4, 0x2E);
        }
        if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[2] != 0) {
            func_80139740(((AlbumSlotRecord *)card_block)[g->album_slot].flags[2], -4, 0x3E);
        }
        if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[3] != 0) {
            func_80139740(((AlbumSlotRecord *)card_block)[g->album_slot].flags[3], -4, 0x4E);
        }
        if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[4] != 0) {
            func_80139740(((AlbumSlotRecord *)card_block)[g->album_slot].flags[4], 0x3C, 0x1E);
        }
        if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[5] != 0) {
            func_80139740(((AlbumSlotRecord *)card_block)[g->album_slot].flags[5], 0x3C, 0x2E);
        }
        if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[6] != 0) {
            func_80139740(((AlbumSlotRecord *)card_block)[g->album_slot].flags[6], 0x3C, 0x3E);
        }
        if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[7] != 0) {
            func_80139740(((AlbumSlotRecord *)card_block)[g->album_slot].flags[7], 0x3C, 0x4E);
        }
        if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[12] != 0) {
            func_80139740(((AlbumSlotRecord *)card_block)[g->album_slot].flags[12], 0x88, 0xE);
        }
        if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[8] != 0) {
            func_80139740(((AlbumSlotRecord *)card_block)[g->album_slot].flags[8], 0x88, 0x1E);
        }
        if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[9] != 0) {
            func_80139740(((AlbumSlotRecord *)card_block)[g->album_slot].flags[9], 0x88, 0x2E);
        }
        if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[10] != 0) {
            func_80139740(((AlbumSlotRecord *)card_block)[g->album_slot].flags[10], 0x88, 0x3E);
        }
        if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[11] != 0) {
            func_80139740(((AlbumSlotRecord *)card_block)[g->album_slot].flags[11], 0x88, 0x4E);
        }
        for (i = 0; i < 4; i++) {
            if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[i] != 0) {
                func_80139AD8(i, -0x23, (i % 4) * 0x10 + 0x1D);
            }
        }
        for (i = 4; i < 8; i++) {
            if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[i] != 0) {
                func_80139AD8(i, 0x1E, (i % 4) * 0x10 + 0x1D);
            }
        }
        for (i = 8; i < 12; i++) {
            if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[i] != 0) {
                func_80139AD8(i, 0x69, (i % 4) * 0x10 + 0x1D);
            }
        }
        if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[12] != 0) {
            func_80139AD8(12, 0x69, 0xD);
        }
        for (i = 0; i < 8; i++) {
            if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[i] == 0) {
                ((Actor *)actor_pool)[i + 128].unk_01 = 4;
                ((Actor *)actor_pool)[i + 128].draw_flags |= 0x80;
            } else {
                ((Actor *)actor_pool)[i + 128].draw_flags &= 0x7F;
            }
        }
        for (i = 8; i < 12; i++) {
            if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[i] == 0) {
                ((Actor *)actor_pool)[i + 129].draw_flags |= 0x80;
                ((Actor *)actor_pool)[i + 129].unk_01 = 4;
            } else {
                ((Actor *)actor_pool)[i + 129].draw_flags &= 0x7F;
            }
        }
        if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[12] == 0) {
            ((Actor *)actor_pool)[136].draw_flags |= 0x80;
            ((Actor *)actor_pool)[136].unk_01 = 4;
        } else {
            ((Actor *)actor_pool)[136].draw_flags &= 0x7F;
        }
    }
}

void func_8013A9C8(void) {
    s32 i;
    SystemData *w = &system_data;
    GameState *g = &game_state;

    k_sub_reset_point_set();
    k_sub_disp_start(0);
    for (i = 0; i < 13; i++) {
        ((Actor *)actor_pool)[i + 128].draw_flags |= 0x80;
    }
    if (D_800E8BEE != 0) {
        for (i = 0; i < 8; i++) {
            if (w->unk_CC8[w->selected_slot][i] == 0) {
                ((Actor *)actor_pool)[i + 128].draw_flags |= 0x80;
                ((Actor *)actor_pool)[i + 128].unk_01 = 4;
                ((Actor *)actor_pool)[i + 141].draw_flags &= 0x7F;
            } else {
                ((Actor *)actor_pool)[i + 128].draw_flags &= 0x7F;
                ((Actor *)actor_pool)[i + 141].draw_flags |= 0x80;
                ((Actor *)actor_pool)[i + 141].unk_18 = (w->unk_CC8[w->selected_slot][i] & 7) + 0x58;
            }
        }
        for (i = 8; i < 11; i++) {
            if (w->unk_CC8[w->selected_slot][i] == 0) {
                ((Actor *)actor_pool)[i + 129].draw_flags |= 0x80;
                ((Actor *)actor_pool)[i + 129].unk_01 = 4;
                ((Actor *)actor_pool)[i + 141].draw_flags &= 0x7F;
            } else {
                ((Actor *)actor_pool)[i + 129].draw_flags &= 0x7F;
                ((Actor *)actor_pool)[i + 141].draw_flags |= 0x80;
                ((Actor *)actor_pool)[i + 141].unk_18 = (w->unk_CC8[w->selected_slot][i] & 7) + 0x58;
            }
        }
    } else {
        for (i = 0; i < 8; i++) {
            if (((GirlFlagBits *)&((AlbumSlotRecord *)card_block)[g->album_slot].unk_2E8[0x2C + i * 0x38])->met == 0 ||
                ((AlbumSlotRecord *)card_block)[g->album_slot].unk_2E8[0x37C + i] == 0) {
                ((Actor *)actor_pool)[i + 128].draw_flags |= 0x80;
                ((Actor *)actor_pool)[i + 128].unk_01 = 4;
                ((Actor *)actor_pool)[i + 141].draw_flags &= 0x7F;
            } else {
                ((Actor *)actor_pool)[i + 128].draw_flags &= 0x7F;
                ((Actor *)actor_pool)[i + 141].draw_flags |= 0x80;
                if (((AlbumSlotRecord *)card_block)[g->album_slot].unk_2E8[0x37C + i] != 0) {
                    ((Actor *)actor_pool)[i + 141].unk_18 = (((AlbumSlotRecord *)card_block)[g->album_slot].unk_2E8[0x37C + i] & 7) + 0x58;
                }
            }
        }
        for (i = 8; i < 11; i++) {
            if (((GirlFlagBits *)&((AlbumSlotRecord *)card_block)[g->album_slot].unk_2E8[0x2C + i * 0x38])->met == 0 ||
                ((AlbumSlotRecord *)card_block)[g->album_slot].unk_2E8[0x37C + i] == 0) {
                ((Actor *)actor_pool)[i + 129].draw_flags |= 0x80;
                ((Actor *)actor_pool)[i + 129].unk_01 = 4;
                ((Actor *)actor_pool)[i + 141].draw_flags &= 0x7F;
            } else {
                ((Actor *)actor_pool)[i + 129].draw_flags &= 0x7F;
                ((Actor *)actor_pool)[i + 141].draw_flags |= 0x80;
                if (((AlbumSlotRecord *)card_block)[g->album_slot].unk_2E8[0x37C + i] != 0) {
                    ((Actor *)actor_pool)[i + 141].unk_18 = (((AlbumSlotRecord *)card_block)[g->album_slot].unk_2E8[0x37C + i] & 7) + 0x58;
                }
            }
        }
    }
    ((Actor *)actor_pool)[140].draw_flags |= 0x80;
    ((Actor *)actor_pool)[140].unk_01 = 4;
    ((Actor *)actor_pool)[136].draw_flags |= 0x80;
    ((Actor *)actor_pool)[136].unk_01 = 4;
}

void func_8013ACA4(void) {
    if (D_800E8BEE != 0) {
        func_80139740(system_data.metrics[0][D_800E7D36].unk_02, 16, 30);
        func_80139740(system_data.metrics[1][D_800E7D36].unk_02, 16, 46);
        func_80139740(system_data.metrics[2][D_800E7D36].unk_02, 16, 62);
        func_80139740(system_data.metrics[3][D_800E7D36].unk_02, 16, 78);
        func_80139740(system_data.metrics[4][D_800E7D36].unk_02, 128, 14);
        func_80139740(system_data.metrics[5][D_800E7D36].unk_02, 128, 30);
        func_80139740(system_data.metrics[6][D_800E7D36].unk_02, 128, 46);
        func_80139740(system_data.metrics[7][D_800E7D36].unk_02, 128, 62);
        func_80139740(system_data.metrics[8][D_800E7D36].unk_02, 128, 78);
        return;
    }
    func_80139740(((AlbumSlotRecord *)card_block)[game_state.album_slot].metrics[0].unk_02, 16, 30);
    func_80139740(((AlbumSlotRecord *)card_block)[game_state.album_slot].metrics[1].unk_02, 16, 46);
    func_80139740(((AlbumSlotRecord *)card_block)[game_state.album_slot].metrics[2].unk_02, 16, 62);
    func_80139740(((AlbumSlotRecord *)card_block)[game_state.album_slot].metrics[3].unk_02, 16, 78);
    func_80139740(((AlbumSlotRecord *)card_block)[game_state.album_slot].metrics[4].unk_02, 128, 14);
    func_80139740(((AlbumSlotRecord *)card_block)[game_state.album_slot].metrics[5].unk_02, 128, 30);
    func_80139740(((AlbumSlotRecord *)card_block)[game_state.album_slot].metrics[6].unk_02, 128, 46);
    func_80139740(((AlbumSlotRecord *)card_block)[game_state.album_slot].metrics[7].unk_02, 128, 62);
    func_80139740(((AlbumSlotRecord *)card_block)[game_state.album_slot].metrics[8].unk_02, 128, 78);
}

s32 func_8013AF78(s32 arg0) {
    s32 a;
    s32 b;
    s32 sel[3];
    s32 c;
    /* FAKE: T-25301, a register local stored from an unsigned global before the first branch is
     * what tools/cvt_pass.py reads as a switch temporary; it turns IDO's global copy propagation
     * off for the function, so the copies below stay apart from their masks as in the original.
     * The store is dead (the loop sets d) and emits nothing. */
    register s32 d = D_80120C2B;

    /* FAKE: T-25301, the empty test gives arg0 the first optimizer bit, so it keeps $a0 */
    if (arg0) {
    }
    for (d = 0; d < 3; d++) {
        sel[d] = 0;
    }
    /* FAKE: T-25301, copies and masks each on one line (as1 schedules a line as one group); the
     * second empty test ends the block, so the copies give d, c, b, a their optimizer bits */
    d = D_80120C2B; c = D_80120BA3; b = D_80120B5F; a = D_80120BE7;
    if (arg0) {
    }
    b &= 0x7F; c &= 0x7F; a &= 0x7F; d &= 0x7F;
    if (arg0 == 0) {
        D_80120BE7 = a;
        D_80120B5F = b;
        D_80120BA3 = c;
        D_80120C2B = d;
        /* FAKE: T-25301, the label puts `return 0` in a block of its own; uopt forbids $v0 to a
         * range used in the block that sets the return value, and d must stay in $v0 */
        goto ret0;
    ret0:
        return 0;
    }
    sel[D_8014D0D4 % 3] = 1;
    D_80120BE7 = a;
    if (sel[0] != 0) {
        a = (u8)(a | 0x80);
        c = (u8)(c | 0x80);
        D_80120BE7 = a;
    }
    D_80120B5F = b;
    if (sel[1] != 0) {
        b = (u8)(b | 0x80);
        D_80120B5F = b;
    }
    D_80120C2B = d;
    D_80120BA3 = c;
    if (sel[2] != 0) {
        d = (u8)(d | 0x80);
        D_80120C2B = d;
        D_80120BA3 = c = (u8)(c | 0x80);
    }
}

s32 func_8013B0A0(void) {
    s32 a;
    s32 b;
    s32 c;
    s32 d;

    a = 0;
    b = 0;
    if (game_state.stage == 0) {
        return 0;
    }
    c = 0;
    d = 0;
    if (game_state.step < 0xA0U) {
        a = 1;
    } else if (game_state.step < 0xC0U) {
        b = 1;
    } else if (game_state.step < 0xE0U) {
        c = 1;
    } else {
        d = 1;
    }
    if (a != 0) {
        ((Actor *)actor_pool_96)[3].draw_flags |= 0x80;
    } else {
        ((Actor *)actor_pool_96)[3].draw_flags &= ~0x80;
    }
    if (b != 0) {
        ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    } else {
        ((Actor *)actor_pool_96)[1].draw_flags &= ~0x80;
    }
    if (c != 0) {
        ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    } else {
        ((Actor *)actor_pool_96)->draw_flags &= ~0x80;
    }
    if (d != 0) {
        ((Actor *)actor_pool_96)[2].draw_flags |= 0x80;
    } else {
        ((Actor *)actor_pool_96)[2].draw_flags &= ~0x80;
    }
}

void func_8013B1D8(void) {
    /* FAKE: unused words preserve the 0x40 stack frame (T-24112) */
    s32 pad[3];
    s32 i;
    s32 idx;
    s32 count;
    SystemData *w = &system_data;
    GameState *g = &game_state;

    if (D_800E8BEE != 0) {
        count = 0;
        for (i = 0; i < 13; i++) {
            if (w->flags[i] != 0) {
                count++;
            }
        }
        for (i = 0; i < 14; i++) {
            if (i == w->selected_slot - 1) {
                ((Actor *)actor_pool_96)[i + 4].unk_43 = 0x11;
            } else {
                ((Actor *)actor_pool_96)[i + 4].unk_43 = 0;
            }
        }
        idx = g->menu_cursor_0;
        if (g->menu_cursor_0 != -1) {
            D_80120C6F |= 0x80;
            D_80120C92 = idx / 7 * 0x1F - 0x82;
            D_80120C96 = idx % 7 * 0x10 - 0xB;
        }
        func_80139740(w->year[w->selected_slot], 0x30, -0x60);
        func_80139740(w->month[w->selected_slot], 0x50, -0x60);
        func_80139740(w->day[w->selected_slot], 0x70, -0x60);
        func_80139740(count, 0x5A, -0x1D);
        return;
    }
    count = 0;
    for (i = 0; i < 13; i++) {
        if (((AlbumSlotRecord *)card_block)[g->album_slot].flags[i] != 0) {
            count++;
        }
    }
    for (i = 0; i < 14; i++) {
        if (i == g->save_slot - 1) {
            ((Actor *)actor_pool_96)[i + 4].unk_43 = 0x11;
        } else {
            ((Actor *)actor_pool_96)[i + 4].unk_43 = 0;
        }
    }
    idx = g->menu_cursor_0;
    if (g->menu_cursor_0 != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = idx / 7 * 0x1F - 0x82;
        D_80120C96 = idx % 7 * 0x10 - 0xB;
    }
    func_80139740(((AlbumSlotRecord *)card_block)[g->album_slot].year, 0x30, -0x60);
    func_80139740(((AlbumSlotRecord *)card_block)[g->album_slot].month, 0x50, -0x60);
    func_80139740(((AlbumSlotRecord *)card_block)[g->album_slot].day, 0x70, -0x60);
    func_80139740(count, 0x5A, -0x1D);
}

void func_8013B51C(s32 arg0) {
    s16 x;
    s16 y;

    x = (arg0 / 7) * 0x1F - 0x82;
    ((Actor *)actor_pool)->x = x;
    y = (arg0 % 7) * 0x10 - 0xB;
    ((Actor *)actor_pool)->y = y;
    ((Actor *)actor_pool)[119].draw_flags |= 0x80;
    ((Actor *)actor_pool)[119].x = x;
    ((Actor *)actor_pool)[119].y = y;
}

void func_8013B59C(void) {
    text_system_reset();
    func_80139CB4();
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    D_8014D0D4 = 1;
    func_8013B51C(game_state.save_slot - 1);
}

void func_8013B5F0(void) {
    if ((game_state.stage != 0 || game_state.step != 0) && D_8014D0E0 < 0x4001) {
        D_8014D0E0 = (D_8014D0E0 * 16 + 0x70) / 15;
        D_8014D0E4 += D_8014D0EC;
        D_8014D0E8 += D_8014D0F0;
        if (D_8014D0E4 >= 0xA1) {
            D_8014D0EC = -(rand() & 0xF);
        }
        if (D_8014D0E8 >= 0x79) {
            D_8014D0F0 = -(rand() & 0xF);
        }
        if (D_8014D0E4 < -0xA0) {
            D_8014D0EC = rand() & 0xF;
        }
        if (D_8014D0E8 < -0x78) {
            D_8014D0F0 = rand() & 0xF;
        }
    }
    func_80046AC8(0x1A, 0, 0x1F7, D_8014D0E0, D_8014D0E4, D_8014D0E8, 0x01DF7FCF, 2);
}

void func_8013B77C(void) {
    if (D_8014D0AC != 0) {
        printf("option  kidou back ");
        if (D_8014D0AC & 0x80) {
            k_speed_set(D_8014D0B0);
        }
        printf("kanji_speed%d \n", D_8014D0B0);
        if (D_8014D0B4 >= 3) {
            game_state.icon_mode = D_8014D0B4 % 3;
        }
        printf("icon_mode %d \n", D_8014D0B4);
        if (D_8014D0B8 >= 4) {
            game_state.menu_mode = D_8014D0B8 & 3;
        }
        printf("menu_mode %d \n", D_8014D0B8);
        if (D_8014D0AC & 8) {
            game_state.cursor_speed = D_8014D0BC;
        }
        printf("cursor_speed %d \n", D_8014D0BC);
        if (D_8014D0C0 >= 3) {
            game_state.voice_mode = D_8014D0C0 % 3;
        }
        printf("sound_mode %d \n", D_8014D0C0);
        if (D_8014D0AC & 0x40) {
            game_state.click_mode = D_8014D0C4;
        }
        printf("click_mode %d \n", D_8014D0C4);
        if (D_8014D0AC & 0x20) {
            game_state.double_click_interval = D_8014D0C8;
        }
        printf("click_speed %d \n", D_8014D0C8);
        if (D_8014D0AC & 0x10) {
            game_state.confirm_button = D_8014D0CC;
        }
        printf("mouse_ok %d \n", D_8014D0CC);
        if (*(u8 *)&game_state.icon_mode == 2) {
            game_state.icon_count = 0xF;
        } else {
            game_state.icon_count = 0xE;
        }
        func_8006509C();
        icon_disp_switch(0);
    }
    D_8014D0A8 = 0;
    func_8013BB1C();
    func_8013B994();
}

void func_8013B994(void) {
    s32 i;
    /* FAKE: unused words preserve the 0x48 stack frame and place err/zero/saved spills at sp+0x34/0x30/0x2C. T-24106 */
    s32 pad[3];
    s32 err;
    s32 zero;
    s32 saved;

    err = 0;
    zero = 0;
    /* FAKE: pointer view on unk_F6C emits lbu (declared s8 in header). T-24106 */
    if (*(u8 *)&game_state.icon_mode == 2) {
        for (i = 0; i < 16; i++) {
            if (((s16 *)D_80125C10)[i * 2] >= 0x7D) {
                err++;
            }
            if (((s16 *)D_80125C10)[i * 2] < -0x86) {
                err++;
            }
            if (((s16 *)D_80125C10)[i * 2 + 1] < -0x54) {
                err++;
            }
            if (((s16 *)D_80125C10)[i * 2 + 1] >= 0x55) {
                err++;
            }
            if (((s16 *)D_80125C10)[i * 2] == 0 && ((s16 *)D_80125C10)[i * 2 + 1] == 0) {
                zero++;
            }
        }
    }
    printf("err %d \n", err);
    printf("zero %d \n", zero);
    if (zero >= 2) {
        if (err != 0) {
            saved = *(u8 *)&game_state.icon_mode;
            *(u8 *)&game_state.icon_mode = 0;
            func_8006509C();
            icon_disp_switch(0);
            *(u8 *)&game_state.icon_mode = saved;
        }
    }
}

void func_8013BB10(void) {
    D_8014D0A8 = 0;
}

void func_8013BB1C(void) {
}

void func_8013BB24(void) {
    u8 speed;

    switch (game_state.substep) {
    case 0:
        D_800B3C6C = 0;
        draw2d3d(1, 0);
        actors_draw_enable(1);
        sprite_tables_clear();
        *(s32 *)&game_state.menu_hit[3] = 0xA0;
        actor_clear_upper();
        if (game_state.options_dirty != 0) {
            printf("option setting!");
            D_8014D0AC = game_state.options_dirty;
            speed = get_k_speed();
            /* FAKE: single-line store+call schedules sb D_8014D0B0 before jal printf. T-11570 */
            D_8014D0B0 = speed; printf("kanji_speed%d \n", speed);
            /* FAKE: pointer views avoid cvt_pass widening on D_8014D0B4..D_8014D0CC so uopt CSEs the stores and schedules lui $a0 before lui $at. T-24106 */
            D_8014D0B4 = game_state.icon_mode;
            printf("icon_mode %d \n", *(u8 *)&D_8014D0B4);
            D_8014D0B8 = game_state.menu_mode;
            printf("menu_mode %d \n", *(u8 *)&D_8014D0B8);
            D_8014D0BC = game_state.cursor_speed;
            printf("cursor_speed %d \n", *(u8 *)&D_8014D0BC);
            D_8014D0C0 = game_state.voice_mode;
            printf("sound_mode %d \n", *(u8 *)&D_8014D0C0);
            D_8014D0C4 = game_state.click_mode;
            printf("click_mode %d \n", *(u8 *)&D_8014D0C4);
            D_8014D0C8 = game_state.double_click_interval;
            printf("click_speed %d \n", *(u8 *)&D_8014D0C8);
            D_8014D0CC = game_state.confirm_button;
            printf("mouse_ok %d \n", *(u8 *)&D_8014D0CC);
        } else {
            D_8014D0AC = 0;
        }
        game_state.options_dirty = 0;
        D_8014D0A8 = 1;
        D_8014D0A4 = 0;
        srand(system_data.seed);
        k_speed_set(1);
        back_clear_switch(1);
        func_8006764C(0);
        tpage_buf_clear();
        game_state.tile_layer_enabled = 0;
        message_window_title_show(0);
        text_system_reset();
        text_set_advance(0xE);
        game_state.screen_brightness = 0;
        D_801230D0 = 0xFF;
        cursor_actor_init(0);
        cursor_show(0);
        D_8014D0E0 = 1;
        D_8014D0E4 = -0x50;
        D_8014D0E8 = -0x50;
        D_8014D0EC = 8;
        D_8014D0F0 = 8;
        make_color_bar16(game_state.scratch_words, 0xDF7FCF, 0xDF7FCF, 0x10, 0);
        load_palette(game_state.scratch_words, 0x17, 1, 1, 0);
        func_80046754(0x80180000, 0x1A, 0);
        snd_bank_load_start(0, 0xBF98, 0xBF79, 0xCD21, 0xCCD9, 0xCCD1);
        game_state.substep++;
        break;
    case 1:
        if (snd_bank_load_poll() == 1) {
            main_step_next();
        }
        break;
    }
}

void func_8013BE18(void) {
    /* FAKE: the fields are read through a GameState pointer declared before the rect. Written as direct game_state reads the
     * rect sat at sp+0x38, the original has it at sp+0x34 with the same frame (T-11570); found by autotweak gstate, T-20830. T-21520 */
    GameState *g = &game_state;
    RECT rect;

    g->screen_brightness += 4;
    if (g->screen_brightness >= 0x80) {
        g->screen_brightness = 0x80;
    }
    switch (g->substep) {
    case 0:
        snd_bank_start(0, 0);
        cd_read_sectors(5, 0x801E0000, 0x9DBA);
        g->substep++;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x89, 0x80180000, 0x9CB5);
            g->substep++;
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            rect.x = 0x3E0;
            rect.y = 0x100;
            rect.h = 0x80;
            rect.w = 0x20;
            func_8009C884(&rect, (void *)0x801E0200);
            load_palette(0x801E0000, 0x18, 1, 2, 0);
            g->screen_brightness = 0x80;
            g->substep = 3;
        }
        break;
    case 3:
        main_step_next();
        break;
    }
}

void func_8013BF74(void) {
    s32 i;
    s32 column, row;
    s16 x[16], y[16], width[16], height[16];
    s16 x2[4], y2[4], width2[4], height2[4];
    s32 addresses[6], addresses2[6];
    Actor *record;

    for (i = 0; i < 6; i++) {
        addresses[i] = 0x80182000 + i * 0x4000;
        addresses2[i] = 0x801A4000 + i * 0x4000;
    }
    for (i = 0; i < 14; i++) {
        column = i / 7;
        row = i % 7;
        x[i] = column * 32 - 144;
        y[i] = row * 16 - 18;
        width[i] = 32;
        height[i] = 16;
    }
    menu_set(0, 14, x, y, width, height);
    for (i = 0; i < 4; i++) {
        column = i & 1;
        row = i & 2;
        x2[i] = column * 44 - 144;
        y2[i] = row * 8 - 96;
        width2[i] = 40;
        height2[i] = 20;
    }
    menu_set(1, 4, x2, y2, width2, height2);
    game_state.menu_cursor_0 = 0;
    game_state.menu_cursor_1 = -1;
    D_8014D0D0 = 0;
    D_8014D0D4 = 1;
    D_8014D0D8 = 0;
    D_8014D0DC = 0;
    record = (Actor *)actor_pool_96;
    for (i = 0; i < 45; i++) {
        record[i].unk_18 = i;
        record[i].unk_01 = 5;
        record[i].flags = 64;
        record[i].quad_flags = 0x01000000;
        record[i].draw_flags = 36;
        record[i].frame_table = (u8 *)0x801C048C;
        record[i].sequence_table = (u8 *)0x801C0540;
        record[i].tpage_table = (u8 *)0x801C047C;
        record[i].unk_04 = 0;
        record[i].unk_43 = 0;
        record[i].unk_05 = 128;
        record[i].x = -160;
        record[i].y = -120;
        record[i].brightness = 128;
    }
    for (i = 0; i < 11; i++) {
        record[i + 45].unk_01 = 4;
        record[i + 45].flags = 64;
        record[i + 45].quad_flags = 0x40000000;
        record[i + 45].draw_flags = 36;
        record[i + 45].frame_table = sprite_frame_table;
        record[i + 45].sequence_table = D_800C9A14;
        record[i + 45].tpage_table = D_800C9730;
        record[i + 45].unk_04 = 8;
        record[i + 45].unk_18 = 89;
        record[i + 45].unk_43 = 0;
        record[i + 45].unk_05 = 1;
        record[i + 45].x = i / 4 * 65 - 15;
        record[i + 45].y = i % 4 * 16 + 36;
    }
    load_tpage_buf_lock(5, ((s32 *)D_80120684)[0], 1);
    load_tpage_buf_lock(6, ((s32 *)D_80120684)[1], 1);
    load_tpage_buf_lock(7, ((s32 *)D_80120684)[2], 1);
    load_tpage_buf_lock(8, ((s32 *)D_80120684)[3], 1);
    ((Actor *)actor_pool)[114].draw_flags = 0;
    ((Actor *)actor_pool)[100].draw_flags |= 0x80;
    ((Actor *)actor_pool)[101].draw_flags |= 0x80;
    ((Actor *)actor_pool)[102].draw_flags |= 0x80;
    record = &((Actor *)actor_pool)[103];
    for (i = 0; i < 12; i++) {
        record[i].draw_flags |= 128;
    }
    if (D_800E8BEE != 0) {
        ((Actor *)actor_pool)[126].unk_43 = 0x11;
    } else {
        ((Actor *)actor_pool)[127].unk_43 = 0x11;
    }
    ((Actor *)actor_pool)[126].draw_flags |= 0x80;
    ((Actor *)actor_pool)[127].draw_flags |= 0x80;
    load_palette(0x801A2000, 0x10, 1, 1, 0);
    load_palette(0x801A2000, 0x11, 1, 1, 1);
    load_palette(0x80180000, 0x1F, 1, 1, 0);
    func_8004A1A0((s32) addresses, 0, 6, 3, 2, 0x1FF, -0xA0, -0x78, 0xA, 0xD);
    func_8004A1A0((s32) addresses2, 1, 6, 3, 2, 0x1F0, -0xA0, -0x78, 8, 0xA);
    actor_clear(0x72);
    main_stage_next();
}

void func_8013C4A8(void) {
    switch (game_state.step) {
    case 0:
        func_8013BB24();
        return;
    case 1:
        func_8013BE18();
        return;
    case 2:
        func_8013BF74();
        return;
    }
}
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014B7B4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014B7C8);

void func_8013C510(void) {
    func_80056284();
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014B7B4, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014B7C8, 0);
    k_disp_start(2);
    main_step_next();
}

void func_8013C580(void) {
    u8 sel;
    GameState *g; /* FAKE: g and the u8 copy of the selector only move it to $v1 (T-11120, permuter) */

    if (func_80053F04() != 0) {
        cursor_show(1);
        card_stop();
        g = &game_state;
        sel = g->card_result;
        if (sel == 1) {
            printf("EXIST CARD!");
            main_stage_next();
        } else if (sel == 0xF1) {
            printf("FILE_SEL UNFORMAT FIND!");
            main_stage_set(0xC0);
        } else if (sel == 0xFF) {
            printf("NO CARD!");
            main_step_next();
        }
    } else if ((u32)game_state.card_timer >= 0x201U) {
        cursor_show(1);
        card_stop();
        printf("NO COMUNICATION!");
        main_step_next();
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014B824);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014B838);

void func_8013C664(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014B824, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014B838, 0);
    k_disp_start(2);
    main_step_next();
}

void func_8013C6CC(void) {
    if (game_state.pad_pressed & 0x860) {
        main_step_next();
    }
}

void func_8013C6FC(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, "新たにゲームを始めますか？", 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_800B3C70, 0);
    set_kanji_string(0x14, 0x30, 1, func_8006CA9C(), 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_800B3C88, 0);
    set_kanji_string(0x14, 0x40, 2, func_8006CAE0(), 0);
    k_disp_start(5);
    main_step_next();
}

void func_8013C7C0(void) {
    if (game_state.pad_pressed & PAD_START) {
        func_8013B77C();
        main_state_set(MODE_NAME_ENT_3);
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        func_8013B77C();
        main_state_set(MODE_NAME_ENT_3);
    } else if (game_state.pad_pressed & PAD_CROSS) {
        main_step_next();
    }
}

void func_8013C834(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, "メモリーカードを", 0);
    set_kanji_string(-0x40, 0x30, 1, "差し込み口１", 0);
    set_kanji_string(-0x40, 0x30, 0xF, "　　　　　　にセットして、", 0);
    set_kanji_string(-0x40, 0x40, 0xF, "　ボタンを押してください。", 0);
    set_kanji_string(-0x40, 0x40, 1, func_8006CA9C(), 0);
    card_stop();
    k_disp_start(5);
    main_step_next();
}

void func_8013C8FC(void) {
    if (game_state.pad_pressed & PAD_CIRCLE) {
        func_80056284();
        main_step_set(0);
    }
}

void func_8013C934(void) {
    switch (game_state.step) {
    case 0:
        func_8013C510();
        break;
    case 1:
        func_8013C580();
        break;
    case 2:
        func_8013C664();
        break;
    case 3:
        func_8013C6CC();
        break;
    case 4:
        func_8013C6FC();
        break;
    case 5:
        func_8013C7C0();
        break;
    case 6:
        func_8013C834();
        break;
    case 7:
        func_8013C8FC();
        break;
    }
}

/* Falls through without a return statement. */
s32 func_8013C9F0(void) {
    s32 pad[3]; /* FAKE: unused words above count, the original frame has them (T-9020, T-11120) */
    s32 count;
    s32 i;

    switch (game_state.substep) {
    case 0:
        card_start();
        game_state.substep++;
        break;
    case 1:
        if (func_80054140() == 2) {
            printf("SEARCH_CARD");
            game_state.substep++;
        } else if (game_state.step_timer.u >= 0x201U) {
            func_80056284();
            main_stage_set(1);
            card_stop();
        }
        break;
    case 2:
        count = 0;
        for (i = 0; i < 14; i++) {
            if (D_800E7D11[i + 4] != 0) {
                count++;
            }
        }
        text_system_reset();
        if (D_800E8BEE != 0) {
            main_step_next();
        } else if (count != 0) {
            main_step_set(0x20);
        } else {
            main_step_set(0x40);
        }
        break;
    case 3:
        break;
    }
}

void func_8013CB44(void) {
    /* FAKE: unused words preserve the 0x40 stack frame. T-22023 */
    s32 pad[2];
    s32 slot;
    s32 i;
    GameState *g = &game_state;

    switch (g->substep) {
    case 0:
        card_start();
        g->substep++;
        break;
    case 1:
        if (func_80054284() == 3) {
            card_stop();
            g->substep++;
        } else if (((u32) g->card_timer >= 0x201U) || (g->card_result == 0xF0)) {
            card_stop();
            func_80056284();
            main_stage_set(1);
        }
        break;
    case 2:
        if (func_800552DC(0) == -1) {
            g->substep++;
        } else {
            text_system_reset();
            func_8013BB10();
            slot = 0xFF;
            if (system_data.slot_status[system_data.selected_slot] == 1) {
                slot = system_data.selected_slot - 1;
            } else {
                for (i = 1; i < 14; i++) {
                    if (system_data.slot_status[i] == 1) {
                        system_data.selected_slot = i;
                        slot = i - 1;
                        break;
                    }
                }
            }
            if (slot != 0xFF) {
                func_8013B51C(slot);
            }
            func_80139CB4();
            k_sub_reset_point_set();
            k_sub_disp_start(0);
            D_8014D0D4 = 1;
            main_step_next();
        }
        break;
    case 3:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "システムファイルは", 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "壊れています。", 0);
        k_disp_start(2);
        card_stop();
        g->substep++;
        break;
    case 4:
        if (g->pad_pressed & 0x60) {
            g->substep++;
        }
        break;
    case 5:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "システムファイルを", 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "作り直しますか？", 0);
        set_kanji_string(-0x40, 0x40, 0xF, (s32) D_800B3C70, 0);
        set_kanji_string(0x14, 0x40, 1, (s32) func_8006CA9C(), 0);
        set_kanji_string(-0x40, 0x50, 0xF, (s32) D_800B3C88, 0);
        set_kanji_string(0x14, 0x50, 2, (s32) func_8006CAE0(), 0);
        k_disp_start(6);
        g->substep++;
        break;
    case 6:
        if (g->pad_pressed & PAD_CIRCLE) {
            g->substep++;
        } else if (g->pad_pressed & PAD_CROSS) {
            for (i = 0; i < 4; i++) {
                if (func_80054704(0) == 1) {
                    break;
                }
            }
            main_stage_set(1);
        }
        break;
    case 7:
        main_stage_set(0xD0);
        break;
    }
}

void func_8013CEC8(void) {
    s32 pad[2]; /* FAKE: unused, gives the original's 0x38 frame; real source unknown. T-16920 */
    s32 n;
    GameState *g = &game_state;

    menu_check(0, cursor_x, cursor_y);
    g->pad_pressed &= ~0x100;
    menu_cursor_update(0);
    n = g->menu_cursor_0;
    if (g->menu_cursor_0 != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = n / 7 * 0x1F - 0x82;
        D_80120C96 = n % 7 * 0x10 - 0xB;
    }
    if ((g->pad_pressed & 0x820) && (g->menu_cursor_0 != -1) && D_800E7D15[n] == 1) {
        if (D_800E7D36 == n + 1) {
            snd_command(0x501);
            main_step_set(3);
            g->save_slot = D_800E7D36;
        } else {
            D_800E7D36 = g->menu_cursor_0 + 1;
            text_system_reset();
            func_80139CB4();
            k_sub_reset_point_set();
            k_sub_disp_start(0);
            func_8013B51C(n);
        }
    }
    func_8013AF78(1);
    func_8013B1D8();
    func_80139C24();
    func_80142880();
}

void func_8013D054(void) {
    u8 state;

    switch (game_state.substep) {
    case 0:
        card_start();
        game_state.substep++;
        break;
    case 1:
        if (func_8005448C() == 4) {
            card_stop();
            game_state.substep++;
        } else if ((state = game_state.card_result) == 0xF0) {
            func_80056284();
            main_stage_set(1);
        } else if (((u32) game_state.card_timer >= 0x201U) || (state == 0xD0)) {
            card_stop();
            main_stage_set(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            game_state.substep++;
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            main_stage_set(0xF0);
        }
        break;
    case 3:
        if (func_80055AFC(0) == 1) {
            func_8013BB10();
            printf("SYS0 DATA OK!");
            card_stop();
            snd_command(0x501);
            card_start();
            D_800E7D36 = game_state.save_slot;
            func_80054E6C();
            game_state.substep++;
        } else {
            state = game_state.card_result;
            printf("s->mem_stat %d \n", state);
            D_800E7D14[game_state.save_slot] = 0xFF;
            main_stage_set(0xF0);
        }
        break;
    case 4:
        if (func_80054388() == 0x13) {
            card_stop();
            main_stage_set(3);
        } else if (((u32) game_state.card_timer >= 0x201U) || ((state = game_state.card_result) == 0xF0)) {
            card_stop();
            func_80056284();
            main_stage_set(1);
        }
        break;
    }
    func_8013B1D8();
}

void func_8013D2AC(void) {
    s32 pad[2]; /* FAKE: unused words above the buffer, the original frame has them. T-22023 */
    u8 text[256];
    s32 i;
    u8 state;
    GameState *g = &game_state;

    switch (g->substep) {
    case 0:
        card_start();
        for (i = 1; i < 15; i++) {
            if (system_data.slot_status[i] == 1) {
                g->save_slot = i;
                break;
            }
        }
        if (i < 15) {
            g->substep++;
        } else {
            card_stop();
            main_step_set(0x40);
        }
        printf("load file %d \n", g->save_slot);
        break;
    case 1:
        if (func_8005448C() == 4) {
            g->substep++;
        } else if ((state = g->card_result) == 0xF0) {
            func_80056284();
            main_stage_set(1);
        } else if (((u32) g->card_timer >= 0x201U) || (state == 0xD0)) {
            card_stop();
            func_80056284();
            main_stage_set(1);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            func_8013B59C();
            main_step_next();
        } else {
            text_system_reset();
            strcpy(text, D_800B3288[g->save_slot]);
            strcat(text, "番アルバムのデータは");
            set_kanji_string(-0x40, 0x20, 0xF, (s32) text, 0);
            set_kanji_string(-0x40, 0x30, 0xF, (s32) "壊れています。", 0);
            D_800E7D14[g->save_slot] = 0xFF;
            card_stop();
            g->substep = 0;
        }
        break;
    }
}

void func_8013D4CC(void) {
    s32 idx;
    GameState *g = &game_state;

    menu_check(0, cursor_x, cursor_y);
    g->pad_pressed &= ~0x100;
    menu_cursor_update(0);
    idx = g->menu_cursor_0;
    if (g->menu_cursor_0 != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = idx / 7 * 0x1F - 0x82;
        D_80120C96 = idx % 7 * 0x10 - 0xB;
    }
    if ((g->pad_pressed & 0x820) && (g->menu_cursor_0 != -1) && D_800E7D15[idx] == 1) {
        if (g->save_slot == idx + 1) {
            main_step_set(0x24);
        } else {
            main_step_set(0x22);
            g->save_slot = g->menu_cursor_0 + 1;
        }
    }
    func_8013AF78(1);
    func_8013B1D8();
    func_80139C24();
    func_80142880();
}

void func_8013D62C(void) {
    s32 pad; /* FAKE: unused, declared before buf for its stack slot (T-3330) */
    u8 buf[0x100];

    text_system_reset();
    strcpy(buf, D_800B3288[game_state.save_slot]);
    strcat(buf, "番アルバムを");
    set_kanji_string(-0x40, 0x20, 0xF, buf, 0);
    set_kanji_string(-0x40, 0x30, 0xF, "ロードしています。", 0);
    k_disp_start(2);
    main_step_next();
}

/* Original implicit-int dispatch falls through without a return statement. */
s32 func_8013D6C0(void) {
    switch (game_state.substep) {
    case 0:
        card_start();
        game_state.substep += 1;
        break;
    case 1:
        if (func_8005448C() == 4) {
            game_state.substep += 1;
        } else if (game_state.card_result == 0xF0) {
            func_80056284();
            main_stage_set(1);
        } else if (((u32) game_state.card_timer >= 0x201U) || (game_state.card_result == 0xD0)) {
            card_stop();
            main_stage_set(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            func_8013B59C();
            main_step_set(0x21);
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            main_stage_set(0xFF);
        }
        break;
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014B998);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014B9A4);

void func_8013D7F4(void) {
    if (func_80055AFC(0) == 1) {
        func_8013BB10();
        printf(D_8014B998);
        card_stop();
        snd_command(0x501);
        main_stage_set(3);
    } else {
        printf(D_8014B9A4);
        func_80056284();
        main_stage_set(1);
    }
    func_8013B1D8();
}

void func_8013D874(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, "このゲームのデータがありません", 0);
    set_kanji_string(-0x40, 0x30, 0xF, "新たにアルバムを作成し、", 0);
    set_kanji_string(-0x40, 0x40, 0xF, "ゲームをプレイしますか？", 0);
    set_kanji_string(-0x40, 0x50, 0xF, "　　はい…　　いいえ…　", 0);
    set_kanji_string(6, 0x50, 1, func_8006CA9C(), 0);
    set_kanji_string(0x5A, 0x50, 2, func_8006CAE0(), 0);
    k_disp_start(6);
    main_step_next();
}

void func_8013D954(void) {
    if (game_state.pad_pressed & PAD_CIRCLE) {
        main_step_set(0x44);
    } else if (game_state.pad_pressed & PAD_CROSS) {
        main_step_next();
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BA28);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BA44);

void func_8013D9A0(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BA28, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BA44, 0);
    k_disp_start(2);
    main_step_next();
}

void func_8013DA08(void) {
    if (game_state.pad_pressed & 0x820) {
        func_8013B77C();
        main_state_set(MODE_NAME_ENT_3);
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BA60);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BA78);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BA88);

void func_8013DA40(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BA60, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BA78, 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_8014BA88, 0);
    k_disp_start(3);
    main_step_next();
}

void func_8013DAC4(void) {
    s32 idx;
    GameState *g = &game_state;

    menu_check(0, cursor_x, cursor_y);
    g->pad_pressed &= ~0x100;
    menu_cursor_update(0);
    idx = g->menu_cursor_0;
    if (g->menu_cursor_0 != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = idx / 7 * 0x1F - 0x82;
        D_80120C96 = idx % 7 * 0x10 - 0xB;
    }
    if ((g->pad_pressed & 0x820) && (g->menu_cursor_0 != -1)) {
        main_step_next();
        g->save_slot = g->menu_cursor_0 + 1;
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BAA4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BAC4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BAE0);

void func_8013DBD0(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BAA4, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BAC4, 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_8014BAE0, 0);
    k_disp_start(3);
    main_step_next();
}

void func_8013DC54(void) {
    if (game_state.pad_pressed & 0x860) {
        main_step_next();
    }
}

void func_8013DC84(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, "システムファイルを作成しますか", 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_800B3C70, 0);
    set_kanji_string(0x14, 0x30, 1, func_8006CA9C(), 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_800B3C88, 0);
    set_kanji_string(0x14, 0x40, 2, func_8006CAE0(), 0);
    k_disp_start(5);
    main_step_next();
}

void func_8013DD48(void) {
    if (game_state.pad_pressed & 0x820) {
        main_step_set(0x4C);
    } else if (game_state.pad_pressed & PAD_CROSS) {
        main_step_next();
    }
}

s32 func_8013DD94(void) {
    u8 sp38[0x108]; /* FAKE: observed frame and buffer slot, T-9580. */
    u8 temp_v0;

    switch (game_state.substep) {
    case 0:
        text_system_reset();
        strcpy(sp38, D_800B32CC[game_state.save_slot - 1]);
        strcat(sp38, "番アルバムを作成しています。");
        set_kanji_string(-0x40, 0x20, 0xF, (s32) sp38, 0);
        k_disp_start(2);
        temp_v0 = D_800E7D14[game_state.save_slot];
        if (temp_v0 == 0xFF) {
            game_state.substep += 1;
        } else if (temp_v0 == 1) {
            game_state.substep += 1;
        } else if (func_80054694((s32) game_state.save_slot) == 1) {
            game_state.substep += 1;
        } else {
            main_stage_set(4);
        }
        break;
    case 1:
        card_start();
        func_80041878();
        func_80042058();
        game_state.substep += 1;
        break;
    case 2:
        if (func_80054590() == 0x14) {
            game_state.substep += 1;
        } else if (((u32) game_state.card_timer >= 0x201U) || (game_state.card_result == 0xF0)) {
            card_stop();
            func_80056284();
            main_stage_set(1);
        }
        break;
    case 3:
        card_stop();
        func_8013B77C();
        main_state_set(MODE_NAME_ENT_3);
        break;
    }
}


void func_8013DF88(void) {
    /* FAKE: unused words, the original frame has them. T-22023 */
    s32 pad[2];
    s32 i;
    u8 text[256];
    switch (game_state.substep) {
    case 0:
        text_system_reset();
        strcpy(text, D_800B32CC[game_state.save_slot - 1]);
        strcat(text, "番アルバム　と、");
        set_kanji_string(-0x40, 0x20, 0xF, (s32) text, 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "システムファイルを", 0);
        set_kanji_string(-0x40, 0x40, 0xF, (s32) "作成しています。", 0);
        k_disp_start(3);
        game_state.substep += 1;
    default:
        break;
    case 1:
        if (func_80054694((s32) game_state.save_slot) == 1) {
            game_state.substep += 1;
        } else {
            main_stage_set(4);
        }
        break;
    case 2:
        if (func_80054694(0) == 1) {
            game_state.substep += 1;
        } else {
            func_80054704((s32) game_state.save_slot);
            main_stage_set(4);
        }
        break;
    case 3:
        card_stop();
        D_800E7D36 = game_state.save_slot;
        i = 0;
        while (i < 15) {
            system_data.slot_status[i] = 0;
            ++i;
        }
        D_800E7D14[game_state.save_slot] = 1;
        *D_800E7D14 = 1;
        game_state.save_slot = 0;
        card_start();
        func_80054E6C();
        game_state.substep += 1;
        break;
    case 4:
        if (func_80054388() == 0x13) {
            card_stop();
            game_state.substep += 1;
        } else if (((u32) (*(u32 *)&game_state.card_timer) >= 0x201U) || (game_state.card_result == 0xF0)) {
            card_stop();
            func_80056284();
            main_stage_set(1);
        }
        break;
    case 5:
        card_start();
        func_80041878();
        func_80042058();
        func_80042058();
        game_state.save_slot = D_800E7D36;
        /* FAKE: pointer view avoids cvt_pass widening on unk_1118 so uopt keeps unk_1118 in $a1 across the switch. T-24106 */
        printf("s->file_no %x \n", *(u8 *)&game_state.save_slot);
        game_state.substep += 1;
        break;
    case 6:
        if (func_80054590() == 0x14) {
            D_800E7D14[game_state.save_slot] = 1;
            game_state.substep += 1;
        } else if (((u32) (*(u32 *)&game_state.card_timer) >= 0x201U) || (game_state.card_result == 0xF0)) {
            card_stop();
            func_80056284();
            main_stage_set(1);
        }
        break;
    case 7:
        card_stop();
        func_8013B77C();
        main_state_set(MODE_NAME_ENT_3);
        break;
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BB8C);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BBA8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BBC0);

void func_8013E30C(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BB8C, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BBA8, 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_8014BBC0, 0);
    k_disp_start(3);
    main_step_next();
}

void func_8013E390(void) {
    if (game_state.pad_pressed & 0x860) {
        main_step_next();
    }
    func_80142880();
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BBD4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BBEC);

void func_8013E3C8(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BBD4, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BBEC, 0);
    k_disp_start(2);
    main_step_next();
}

void func_8013E430(void) {
    s32 pad[2]; /* FAKE: unused, gives the original's 0x38 frame; real source unknown. T-16920 */
    s32 n;
    GameState *g = &game_state;

    menu_check(0, cursor_x, cursor_y);
    g->pad_pressed &= ~0x100;
    menu_cursor_update(0);
    n = g->menu_cursor_0;
    if (g->menu_cursor_0 != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = n / 7 * 0x1F - 0x82;
        D_80120C96 = n % 7 * 0x10 - 0xB;
    }
    if ((g->pad_pressed & 0x820) && (g->menu_cursor_0 != -1)) {
        snd_command(0x501);
        g->save_slot = n + 1;
        if (D_800E7D15[n] != 0) {
            main_step_next();
            g->save_slot = g->menu_cursor_0 + 1;
        } else if (D_800E8BEE != 0) {
            main_step_set(0xA8);
        } else {
            main_step_set(0xA9);
        }
    }
    func_80142880();
}

void func_8013E59C(void) {
    s32 pad; /* FAKE: unused, declared before buf for its stack slot (T-3330) */
    u8 buf[0x100];

    text_system_reset();
    strcpy(buf, D_800B3288[game_state.save_slot]);
    strcat(buf, "番アルバムを");
    set_kanji_string(-0x40, 0x20, 0xF, buf, 0);
    set_kanji_string(-0x40, 0x30, 0xF, "やりなおします。", 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_800B3C70, 0);
    set_kanji_string(0x14, 0x40, 1, func_8006CA9C(), 0);
    set_kanji_string(-0x40, 0x50, 0xF, D_800B3C88, 0);
    set_kanji_string(0x14, 0x50, 2, func_8006CAE0(), 0);
    k_disp_start(6);
    main_step_next();
}

void func_8013E6A8(void) {
    if (game_state.pad_pressed & PAD_CROSS) {
        snd_command(0x501);
        main_step_set(0);
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        if (D_800E8BEE) {
            main_step_set(0xA6);
        } else {
            main_step_set(0xA7);
        }
    }
}

/* Packed date word: day in bits 0-3, month in bits 4-8, week in bits 9-10. */

void func_8013E71C(void) {
    memcpy(D_8014D070, &game_state.player_family_name, 8);
    memcpy(D_8014D078, &game_state.player_given_name, 8);
    memcpy(D_8014D080, &game_state.player_nickname, 0xC);
    D_8014D08C = ((BirthdayBits *)&game_state.player_birthday)->month - 1;
    D_8014D090 = ((BirthdayBits *)&game_state.player_birthday)->day - 1;
    D_8014D094 = (((BirthdayBits *)&game_state.player_birthday)->blood_type + 3) & 3;
    D_8014D098 = ((BirthdayBits *)&game_state.girls[0].birthday.w)->month - 1;
    D_8014D09C = ((BirthdayBits *)&game_state.girls[0].birthday.w)->day - 1;
    D_8014D0A0 = (((BirthdayBits *)&game_state.girls[0].birthday.w)->blood_type + 3) & 3;
    D_8014D0A4 = 1;
}

void func_8013E80C(void) {
    switch (game_state.substep) {
    case 0:
        card_start();
        printf("NEW FILE %d \n", game_state.save_slot);
        game_state.substep += 1;
    default:
        break;
    case 1:
        if (func_8005448C() == 4) {
            card_stop();
            game_state.substep += 1;
        } else if (game_state.card_result == 0xF0) {
            func_80056284();
            main_stage_set(1);
        } else if (((u32) game_state.card_timer >= 0x201U) || (game_state.card_result == 0xD0)) {
            card_stop();
            main_stage_set(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            game_state.substep += 1;
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            main_stage_set(0xFF);
        }
        break;
    case 3:
        if (func_80055AFC(0) == 1) {
            func_8013E71C();
            func_8013BB10();
            game_state.substep += 1;
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            main_stage_set(0xFF);
        }
        break;
    case 4:
        printf("%d file replay", game_state.save_slot);
        func_80041878();
        func_80042058();
        func_8013B77C();
        main_state_set(MODE_NAME_ENT_3);
        break;
    }
}

void func_8013E9FC(void) {
    switch (game_state.substep) {
    case 0:
        card_start();
        printf("NEW FILE %d \n", game_state.save_slot);
        game_state.substep += 1;
    default:
        break;
    case 1:
        if (func_8005448C() == 4) {
            card_stop();
            game_state.substep += 1;
        } else if (game_state.card_result == 0xF0) {
            func_80056284();
            main_stage_set(1);
        } else if (((u32) game_state.card_timer >= 0x201U) || (game_state.card_result == 0xD0)) {
            card_stop();
            main_stage_set(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            game_state.substep += 1;
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            main_stage_set(0xFF);
        }
        break;
    case 3:
        if (func_80055AFC(0) == 1) {
            func_8013E71C();
            func_8013BB10();
            game_state.substep += 1;
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            main_stage_set(0xFF);
        }
        break;
    case 4:
        printf("%d file replay", game_state.save_slot);
        func_80041878();
        func_80042058();
        func_8013B77C();
        main_state_set(MODE_NAME_ENT_3);
        break;
    }
}

void func_8013EBEC(void) {
    u8 state;
    s32 result;
    u8 text[256];

    state = game_state.substep;
    switch (state) {
    case 0:
        text_system_reset();
        strcpy(text, D_800B32CC[game_state.save_slot - 1]);
        strcat(text, "番アルバム　を、");
        set_kanji_string(-0x40, 0x20, 0xF, (s32) text, 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "作成しています。", 0);
        k_disp_start(2);
        game_state.substep += 1;
    default:
        break;
    case 1:
        if ((result = func_80054694((s32) game_state.save_slot)) == 1) {
            game_state.substep += 1;
        } else {
            main_stage_set(4);
        }
        break;
    case 2:
        card_start();
        func_80041878();
        func_80042058();
        game_state.substep += 1;
        break;
    case 3:
        if ((result = func_80054590()) == 0x14) {
            D_800E7D14[game_state.save_slot] = 1;
            game_state.substep += 1;
        } else if (((u32) (*(u32 *)&game_state.card_timer) >= 0x201U) || (game_state.card_result == 0xF0)) {
            card_stop();
            func_80056284();
            main_stage_set(1);
        }
        break;
    case 4:
        card_stop();
        card_start();
        D_800E7D36 = game_state.save_slot;
        func_80054E6C();
        game_state.substep += 1;
        break;
    case 5:
        if ((result = func_80054388()) == 0x13) {
            card_stop();
            game_state.substep += 1;
        } else if (((u32) (*(u32 *)&game_state.card_timer) >= 0x201U) || (game_state.card_result == 0xF0)) {
            card_stop();
            func_80056284();
            main_stage_set(1);
        }
        break;
    case 6:
        func_8013B77C();
        main_state_set(MODE_NAME_ENT_3);
        break;
    }
}

void func_8013EE7C(void) {
    u8 state;
    s32 result;
    u8 text[256];

    state = game_state.substep;
    switch (state) {
    case 0:
        text_system_reset();
        strcpy(text, D_800B32CC[game_state.save_slot - 1]);
        strcat(text, "番アルバム　を、");
        set_kanji_string(-0x40, 0x20, 0xF, (s32) text, 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "作成しています。", 0);
        k_disp_start(2);
        game_state.substep += 1;
    default:
        break;
    case 1:
        if ((result = func_80054694((s32) game_state.save_slot)) == 1) {
            game_state.substep += 1;
        } else {
            main_stage_set(4);
        }
        break;
    case 2:
        card_start();
        func_80041878();
        func_80042058();
        game_state.substep += 1;
        break;
    case 3:
        if ((result = func_80054590()) == 0x14) {
            D_800E7D14[game_state.save_slot] = 1;
            game_state.substep += 1;
        } else if (((u32) (*(u32 *)&game_state.card_timer) >= 0x201U) || (game_state.card_result == 0xF0)) {
            card_stop();
            func_80056284();
            main_stage_set(1);
        }
        break;
    case 4:
        func_8013B77C();
        main_state_set(MODE_NAME_ENT_3);
        break;
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BCB4);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BCCC);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BCE0);

void func_8013F058(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BCB4, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BCCC, 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_8014BCE0, 0);
    k_disp_start(3);
    main_step_next();
}

void func_8013F0DC(void) {
    s32 pad[2]; /* FAKE: unused, gives the original's 0x38 frame; real source unknown. T-16920 */
    s32 idx;
    GameState *g = &game_state;

    menu_check(0, cursor_x, cursor_y);
    g->pad_pressed &= ~0x100;
    menu_cursor_update(0);
    idx = g->menu_cursor_0;
    if (g->menu_cursor_0 != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = idx / 7 * 0x1F - 0x82;
        D_80120C96 = idx % 7 * 0x10 - 0xB;
    }
    if ((g->pad_pressed & 0x820) && (g->menu_cursor_0 != -1) && D_800E7D15[idx] == 1) {
        snd_command(0x501);
        main_step_next();
        g->save_slot = idx + 1;
    }
    func_80142880();
}

/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* The u8 mode copy and the s32 state give the original 0x138 frame (text at sp+0x30). T-11080 */
s32 func_8013F208(void) {
    s32 state;
    u8 mode;
    u8 text[0x100];

    state = game_state.substep;
    switch (state) {
    case 0:
        card_start();
        game_state.substep++;
    default:
        break;
    case 1:
        if (func_8005448C() == 4) {
            game_state.substep++;
        } else if ((mode = game_state.card_result) == 0xF0) {
            func_80056284();
            main_stage_set(1);
        } else if (((u32) game_state.card_timer >= 0x201U) || (mode == 0xD0)) {
            card_stop();
            main_stage_set(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            func_8013B59C();
            game_state.substep++;
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            main_stage_set(0xFF);
        }
        break;
    case 3:
        if (func_80055AFC(0) == 1) {
            func_8013BB10();
            D_8014D0A8 = 1;
            printf("COPY LOAD SUM !\n");
            game_state.substep++;
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            main_stage_set(0xFF);
        }
        break;
    case 4:
        text_system_reset();
        strcpy(text, D_800B3288[game_state.save_slot]);
        strcat(text, "番アルバムを");
        set_kanji_string(-0x40, 0x20, 0xF, text, 0);
        set_kanji_string(-0x40, 0x30, 0xF, "どのアルバムにコピーしますか？", 0);
        set_kanji_string(-0x40, 0x40, 0xF, "カーソルで選んで下さい。", 0);
        k_disp_start(3);
        main_step_next();
        break;
    }
}


void func_8013F454(void) {
    s32 idx;
    GameState *g = &game_state;

    func_80142880();
    menu_check(0, cursor_x, cursor_y);
    g->pad_pressed &= ~0x100;
    menu_cursor_update(0);
    idx = g->menu_cursor_0;
    if (g->menu_cursor_0 != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = idx / 7 * 0x1F - 0x82;
        D_80120C96 = idx % 7 * 0x10 - 0xB;
    }
    if (g->pad_pressed & 0x820) {
        if (g->menu_cursor_0 != -1) {
            g->save_slot = g->menu_cursor_0 + 1;
            if (D_800E7D15[idx] != 0) {
                main_step_next();
            } else if (func_80054694(g->save_slot) == 1) {
                snd_command(0x501);
                main_step_set(0xC6);
            } else {
                main_stage_set(4);
            }
        }
    }
}

void func_8013F5B0(void) {
    u8 buf[0x108]; /* FAKE: 8 bytes more than the neighbours give the original frame (0x140). T-9170 */

    text_system_reset();
    strcpy(buf, D_800B3288[game_state.save_slot]);
    strcat(buf, "番アルバムに");
    set_kanji_string(-0x40, 0x20, 0xF, buf, 0);
    set_kanji_string(-0x40, 0x30, 0xF, "上書きしてもいいですか？", 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_800B3C70, 0);
    set_kanji_string(0x14, 0x40, 1, func_8006CA9C(), 0);
    set_kanji_string(-0x40, 0x50, 0xF, D_800B3C88, 0);
    set_kanji_string(0x14, 0x50, 2, func_8006CAE0(), 0);
    k_disp_start(6);
    main_step_next();
}

void func_8013F6BC(void) {
    if (game_state.pad_pressed & PAD_CIRCLE) {
        snd_command(0x501);
        main_step_next();
    } else if (game_state.pad_pressed & PAD_CROSS) {
        main_step_set(0);
        game_state.substep = 2;
    }
}

void func_8013F71C(void) {
    if (D_800E8BEE) {
        main_step_set(0xC7);
    } else {
        main_step_set(0xC8);
    }
}

s32 func_8013F758(void) {
    u8 sp38[0x108]; /* FAKE: observed frame and buffer slot, T-9580. */

    switch (game_state.substep) {
    case 0:
        text_system_reset();
        strcpy(sp38, D_800B3288[game_state.save_slot]);
        strcat(sp38, "番アルバムに");
        set_kanji_string(-0x40, 0x20, 0xF, (s32) sp38, 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "コピーします。", 0);
        k_disp_start(2);
        card_start();
        game_state.substep += 1;
        break;
    case 1:
        if (func_80054590() == 0x14) {
            D_800E7D14[game_state.save_slot] = 1;
            game_state.substep += 1;
            card_stop();
        } else if (((u32) game_state.card_timer >= 0x201U) || (game_state.card_result == 0xF0)) {
            printf("NO MEM CARD \n");
            card_stop();
            func_80056284();
            main_stage_set(1);
        }
        break;
    case 2:
        card_stop();
        D_800E7D36 = game_state.save_slot;
        game_state.save_slot = 0;
        card_start();
        func_80054E6C();
        game_state.substep += 1;
        break;
    case 3:
        if (func_80054388() == 0x13) {
            card_stop();
            main_step_set(0);
            game_state.substep = 2;
        } else if (((u32) game_state.card_timer >= 0x201U) || (game_state.card_result == 0xF0)) {
            card_stop();
            func_80056284();
            main_stage_set(1);
        }
        break;
    }
}


s32 func_8013F984(void) {
    u8 sp38[0x108]; /* FAKE: observed frame and buffer slot, T-9580. */

    switch (game_state.substep) {
    case 0:
        text_system_reset();
        strcpy(sp38, D_800B3288[game_state.save_slot]);
        strcat(sp38, "番アルバムに");
        set_kanji_string(-0x40, 0x20, 0xF, (s32) sp38, 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "コピーします。", 0);
        k_disp_start(2);
        card_start();
        game_state.substep += 1;
        break;
    case 1:
        if (func_80054590() == 0x14) {
            D_800E7D14[game_state.save_slot] = 1;
            game_state.substep += 1;
            card_stop();
        } else if (((u32) game_state.card_timer >= 0x201U) || (game_state.card_result == 0xF0)) {
            printf("NO MEM CARD \n");
            card_stop();
            func_80056284();
            main_stage_set(1);
        }
        break;
    case 2:
        main_step_set(0);
        game_state.substep = 2;
        break;
    }
}


INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BDE8);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BE00);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BE10);

void func_8013FB08(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BDE8, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BE00, 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_8014BE10, 0);
    k_disp_start(3);
    main_step_next();
}

void func_8013FB8C(void) {
    s32 idx;
    GameState *g = &game_state;

    menu_check(0, cursor_x, cursor_y);
    g->pad_pressed &= ~0x100;
    menu_cursor_update(0);
    idx = g->menu_cursor_0;
    if (g->menu_cursor_0 != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = idx / 7 * 0x1F - 0x82;
        D_80120C96 = idx % 7 * 0x10 - 0xB;
    }
    if ((g->pad_pressed & 0x820) && (g->menu_cursor_0 != -1) && D_800E7D15[idx] != 0) {
        main_step_next();
        snd_command(0x501);
        g->save_slot = g->menu_cursor_0 + 1;
    }
    func_80142880();
}

void func_8013FCBC(void) {
    s32 pad; /* FAKE: unused, declared before buf for its stack slot (T-3330) */
    u8 buf[0x100];

    text_system_reset();
    strcpy(buf, D_800B3288[game_state.save_slot]);
    strcat(buf, "番アルバムを");
    set_kanji_string(-0x40, 0x20, 0xF, buf, 0);
    set_kanji_string(-0x40, 0x30, 0xF, "本当に消去していいですか？", 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_800B3C70, 0);
    set_kanji_string(0x14, 0x40, 1, func_8006CA9C(), 0);
    set_kanji_string(-0x40, 0x50, 0xF, D_800B3C88, 0);
    set_kanji_string(0x14, 0x50, 2, func_8006CAE0(), 0);
    k_disp_start(6);
    main_step_next();
}

void func_8013FDC8(void) {
    if (game_state.pad_pressed & PAD_CIRCLE) {
        snd_command(0x501);
        if (D_800E8BEE) {
            main_step_set(0xE4);
        } else {
            main_step_set(0xE5);
        }
    } else if (game_state.pad_pressed & PAD_CROSS) {
        main_step_set(0);
    }
}

void func_8013FE40(void) {
    switch (game_state.substep) {
    case 0:
        if (func_80054704((s32) game_state.save_slot) == 1) {
            printf("DELETE OK");
            D_800E7D14[game_state.save_slot] = 0;
            game_state.substep += 1;
        } else {
            card_stop();
            func_80056284();
            main_stage_set(1);
        }
        break;
    case 1:
        card_start();
        D_800E7D36 = game_state.save_slot;
        game_state.save_slot = 0;
        game_state.substep += 1;
        break;
    case 2:
        if (func_80054388() == 0x13) {
            card_stop();
            func_80056284();
            main_stage_set(1);
        } else if (((u32) game_state.card_timer >= 0x201U) || (game_state.card_result == 0xF0)) {
            card_stop();
            func_80056284();
            main_stage_set(1);
        }
        break;
    }
}

void func_8013FF98(void) {
    if (game_state.substep == 0) {
        if (func_80054704(game_state.save_slot) == 1) {
            printf("DELETE OK");
            D_800E7D14[game_state.save_slot] = 0;
            main_step_set(0);
            game_state.substep = 2;
            return;
        }
        card_stop();
        func_80056284();
        main_stage_set(1);
    }
}

void func_80140024(void) {
    main_step_set(0);
}

void func_80140044(void) {
    u8 state;
    game_state.step_timer.w += 1;
    state = game_state.step;
    switch (state) {
    case 0x0:
        func_8013C9F0();
        return;
    case 0x1:
        func_8013CB44();
        return;
    case 0x2:
        func_8013CEC8();
        return;
    case 0x3:
        func_8013D054();
        return;
    case 0x20:
        func_8013D2AC();
        return;
    case 0x21:
        func_8013D4CC();
        return;
    case 0x22:
        func_8013D62C();
        return;
    case 0x23:
        func_8013D6C0();
        return;
    case 0x24:
        func_8013D7F4();
        return;
    case 0x40:
        func_8013D874();
        return;
    case 0x41:
        func_8013D954();
        return;
    case 0x42:
        func_8013D9A0();
        return;
    case 0x43:
        func_8013DA08();
        return;
    case 0x44:
        func_8013DA40();
        return;
    case 0x45:
        func_8013DAC4();
        return;
    case 0x46:
        func_8013DBD0();
        return;
    case 0x47:
        func_8013DC54();
        return;
    case 0x48:
        func_8013DC84();
        return;
    case 0x49:
        func_8013DD48();
        return;
    case 0x4A:
        func_8013DD94();
        return;
    case 0x4C:
        func_8013DF88();
        return;
    case 0xA0:
        func_8013E30C();
        return;
    case 0xA1:
        func_8013E390();
        return;
    case 0xA2:
        func_8013E3C8();
        return;
    case 0xA3:
        func_8013E430();
        return;
    case 0xA4:
        func_8013E59C();
        return;
    case 0xA5:
        func_8013E6A8();
        return;
    case 0xA6:
        func_8013E80C();
        return;
    case 0xA7:
        func_8013E9FC();
        return;
    case 0xA8:
        func_8013EBEC();
        return;
    case 0xA9:
        func_8013EE7C();
        return;
    case 0xC0:
        func_8013F058();
        return;
    case 0xC1:
        func_8013F0DC();
        return;
    case 0xC2:
        func_8013F208();
        return;
    case 0xC3:
        func_8013F454();
        return;
    case 0xC4:
        func_8013F5B0();
        return;
    case 0xC5:
        func_8013F6BC();
        return;
    case 0xC6:
        func_8013F71C();
        return;
    case 0xC7:
        func_8013F758();
        return;
    case 0xC8:
        func_8013F984();
        return;
    case 0xE0:
        func_8013FB08();
        return;
    case 0xE1:
        func_8013FB8C();
        return;
    case 0xE2:
        func_8013FCBC();
        return;
    case 0xE3:
        func_8013FDC8();
        return;
    case 0xE4:
        func_8013FE40();
        return;
    case 0xE5:
        func_8013FF98();
        return;
    case 0xE6:
        func_80140024();
        return;
    default:
        func_8013B77C();
        main_state_set(MODE_START);
        return;
    }
}

/* Falls through without a return statement. */
s32 func_80140430(void) {
    u32 i;

    switch (game_state.substep) {
    case 0:
        i = game_state.save_slot - 1;
        cd_audio_request_set(D_800B3A3C[i * 2], D_800B3A3C[i * 2 + 1], i % 13);
        snd_command(0x300);
        printf("CALL XA");
        game_state.substep++;
        break;
    case 1:
        if (cd_audio_state() & 4) {
            game_state.substep++;
        }
        break;
    case 2:
        if (cd_audio_state() & 2) {
            game_state.substep++;
        }
        break;
    case 3:
        card_stop();
        if (D_800E8BEE != 0) {
            main_stage_set(2);
            main_step_set(2);
        } else {
            func_80056284();
            main_stage_set(1);
        }
        break;
    }
}

/* Falls through without a return statement. */
s32 func_80140574(void) {
    u32 i;

    switch (game_state.substep) {
    case 0:
        i = game_state.save_slot - 1;
        cd_audio_request_set(D_800B394C[i * 2], D_800B394C[i * 2 + 1], i % 13);
        snd_command(0x300);
        printf("CALL XA");
        game_state.substep++;
        break;
    case 1:
        if (cd_audio_state() & 4) {
            game_state.substep++;
        }
        break;
    case 2:
        if (cd_audio_state() & 2) {
            game_state.substep++;
        }
        break;
    case 3:
        card_stop();
        func_80056284();
        main_stage_set(1);
        break;
    }
}

void func_8014068C(void) {
    /* FAKE: unused words preserve the 0x38 stack frame (T-24112) */
    s32 pad[3];
    u8 i;
    u8 girl;
    u8 unk;
    s32 j;
    s32 *pair;

    /* FAKE: unk_110D and unk_03E are already u8; the (u8 *) views emit Ilod/Istr so as1 schedules the base address into the delay slot (T-24112) */
    switch (*(u8 *)&game_state.substep) {
    case 0:
        dec_bg_reset();
        dec_bg_show_switch(2);
        func_80057640(0xF0);
        cursor_show(0);
        sprite_tables_clear();
        game_state.event_log_count = func_8006C988(&girl, &unk, 1);
        i = girl;
        printf("girl %x \n", girl);
        printf("i %x \n", i);
        if (i >= 13) {
            if (*(u8 *)&game_state.year == 0x62 && game_state.month == 3 && game_state.day == 1) {
            } else if (*(u8 *)&game_state.year == 0x5F && game_state.month == 4 && game_state.day == 4) {
            } else {
                snd_command(5);
                snd_command(0xC1);
            }
            *(u8 *)&game_state.substep = 4;
        } else if (i == 11) {
            if (game_state.event_flags[0x4D] != 0) {
                j = 1;
                pair = D_800B3B9C + i * 4 + j * 2;
                cd_audio_request_set(pair[0], pair[1], i);
                snd_command(0x300);
                *(u8 *)&game_state.substep += 1;
            } else {
                j = 0;
                pair = D_800B3B9C + i * 4 + j * 2;
                cd_audio_request_set(pair[0], pair[1], i);
                snd_command(0x300);
                *(u8 *)&game_state.substep += 1;
            }
        } else if (i == 12) {
            if (system_data.flags[12] != 0 || game_state.girl_cleared[12] != 0) {
                if ((u8)game_state.girls[12].flags.b[3] >= 0x29U) {
                    j = 0;
                } else {
                    j = 1;
                }
                pair = D_800B3B9C + i * 4 + j * 2;
                cd_audio_request_set(pair[0], pair[1], i);
                printf("伊集院A");
                snd_command(0x300);
                *(u8 *)&game_state.substep += 1;
            } else if ((u8)game_state.girls[12].flags.b[3] >= 0x29U) {
                j = 1;
                pair = D_800B3B9C + i * 4 + j * 2;
                cd_audio_request_set(pair[0], pair[1], i);
                snd_command(0x300);
                printf("伊集院B");
                *(u8 *)&game_state.substep += 1;
            } else {
                printf("伊集院C");
                *(u8 *)&game_state.substep = 4;
            }
        } else {
            if ((get_g_zyotai_s(i) & 0x7F) == 0) {
                j = 1;
            } else {
                j = 0;
            }
            pair = D_800B3B9C + i * 4 + j * 2;
            cd_audio_request_set(pair[0], pair[1], i);
            snd_command(0x300);
            *(u8 *)&game_state.substep += 1;
        }
        break;
    case 1:
        game_state.screen_brightness = 0x80;
        set_dec_bri(0x80);
        sprite_tables_clear();
        if (cd_audio_state() & 4) {
            *(u8 *)&game_state.substep += 1;
        }
        break;
    case 2:
        if (cd_audio_state() & 2) {
            *(u8 *)&game_state.substep += 1;
            if (*(u8 *)&game_state.year == 0x62 && game_state.month == 3 && game_state.day == 1) {
            } else if (*(u8 *)&game_state.year == 0x5F && game_state.month == 4 && game_state.day == 4) {
            } else {
                snd_command(5);
                snd_command(0xC1);
            }
        }
        break;
    case 3:
        *(u8 *)&game_state.substep += 1;
        break;
    case 4:
        if ((u8)game_state.screen_brightness < 4U) {
            printf("A\n");
            game_state.clear_color_g = 0;
            game_state.clear_color_r = 0;
            game_state.clear_color_b = 0;
            main_step_next();
        }
        break;
    }
    func_80140B20();
    dec_bg_draw(2);
    D_8011ECA4 = 1;
}

/* FAKE: s32 with no return value; the void form keeps the six fade stores in $t6 where the original copies the value to $v0 (T-22023) */
s32 func_80140B20(void) {
    /* FAKE: unk_03A is already u8; the (u8 *) view only moves the global to $a0 like the original (T-11570) */
    if (*(u8 *)&game_state.screen_brightness >= 3) {
        *(u8 *)&game_state.screen_brightness -= 2;
        /* the chain order sets the store order: D_801220E0 is stored last, the others from D_80122106 outward */
        D_801220E1 = D_801220E2 = D_80122104 = D_80122105 = D_80122106 = D_801220E0 = *(u8 *)&game_state.screen_brightness;
        if (*(u8 *)&game_state.screen_brightness < 0x80) {
            set_dec_bri(*(u8 *)&game_state.screen_brightness);
        }
    }
}

void func_80140BA4(void) {
    if (game_state.substep++ >= 0x41U) {
        func_8013B77C();
        set_dec_bri(0);
        if (game_state.year == 0x62 && game_state.month == 3 && game_state.day == 1) {
            func_8013E71C();
            func_80041878();
            func_80041F48();
            game_state.event_log_count = 0;
            main_state_set(MODE_NAME_ENT_3);
        } else if (game_state.year == 0x5F && game_state.month == 4 && game_state.day == 4) {
            func_8013E71C();
            func_80041878();
            func_80041F48();
            game_state.event_log_count = 0;
            main_state_set(MODE_NAME_ENT_3);
        } else {
            func_800674B0();
            main_state_set(MODE_COMMAND_SCREEN);
            main_stage_set(0xFF);
        }
    }
    D_8011ECA4 = 1;
}

s32 func_80140CCC(void) {
    switch (game_state.step) {
    case 0:
        func_8014068C();
        break;
    case 1:
        func_80140BA4();
        break;
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata", D_8014BEB0);

void func_80140D20(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BEB0, 0);
    k_disp_start(2);
    main_step_next();
}

/* FAKE: biased byte cursor, alternate end expression and statement layout preserve addressing and scheduling, T-9580. */
void func_80140D6C(void)
{
  u8 *p;
  u8 *new_var;
  if (game_state.pad_pressed & 0x860)
  {
    p = (u8 *) (&system_data); do
    {
      if (p[4] == 1)
      {
        break;
      }
      p++;
    }
    while (p != (&D_800E7D1F));
    new_var = ((u8 *) (&system_data)) - -15;
    if (p == new_var)
    {
      main_step_next();
      return;
    }
    main_stage_set(2);
    if (D_800E8BEE != 0)
    {
      main_step_set(1);
      return;
    }
    main_step_set(0x20);
  }
}

void func_80140E18(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, "別のメモリーカードを差し込み口", 0);
    set_kanji_string(-0x40, 0x30, 0xF, "１にセットするか、他のゲームの", 0);
    set_kanji_string(-0x40, 0x40, 0xF, "データを消去してください。", 0);
    k_disp_start(2);
    main_step_next();
}

void func_80140E9C(void) {
    if (game_state.pad_pressed & 0x860) {
        main_stage_set(1);
        main_step_set(6);
    }
}

void func_80140ED4(void) {
    switch (game_state.step) {
    case 0:
        func_80140D20();
        return;
    case 1:
        func_80140D6C();
        return;
    case 2:
        func_80140E18();
        return;
    case 3:
        func_80140E9C();
        return;
    }
}

void func_80140F54(void) {
    if (D_800E8BEE) {
        main_step_set(1);
    } else {
        main_step_set(2);
    }
}

void func_80140F90(void) {
    s32 i;
    u8 state;
    SystemData *w = &system_data;
    GameState *g = &game_state;

    switch (game_state.substep) {
    case 0:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "システムファイルを作り直して", 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "いいですか？", 0);
        set_kanji_string(-0x40, 0x40, 0xF, (s32) D_800B3C70, 0);
        set_kanji_string(0x14, 0x40, 1, func_8006CA9C(), 0);
        set_kanji_string(-0x40, 0x50, 0xF, (s32) D_800B3C88, 0);
        set_kanji_string(0x14, 0x50, 2, func_8006CAE0(), 0);
        k_disp_start(6);
        for (i = 0; i < 16; i++) D_8014D158[i] = 0;
        game_state.substep++;
        break;
    case 1:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            for (i = 0; i < 16; i++) {
                D_8014D158[i] = w->flags[i];
            }
            game_state.substep++;
        } else if (game_state.pad_pressed & PAD_CROSS) {
            func_80056284();
            main_stage_set(1);
        }
        break;
    case 2:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "約１分ほど時間がかかりますが、", 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "途中でメモリーカードを", 0);
        set_kanji_string(-0x40, 0x40, 0xF, (s32) "抜かないで下さい。", 0);
        k_disp_start(3);
        game_state.substep++;
        break;
    case 3:
        if ((game_state.pad_pressed != 0) || ((u32) game_state.step_timer.w >= 0x41U)) {
            game_state.substep++;
        }
        break;
    case 4:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "準備が出来たら、", 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "何かボタンを押してください。", 0);
        k_disp_start(2);
        game_state.step_timer.w = 0;
        game_state.substep++;
        break;
    case 5:
        if (game_state.pad_pressed != 0) {
            game_state.substep++;
        }
        break;
    case 6:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "しばらくお待ち下さい。", 0);
        k_disp_start(1);
        for (i = 0; i < 16; i++) D_8014D060[i] = 0;
        game_state.save_slot = 1;
        game_state.substep++;
        break;
    case 7:
        if (game_state.save_slot >= 15) {
            for (i = 0; i < 16; i++) {
                if (D_8014D158[i] < D_8014D060[i]) {
                    w->flags[i] = D_8014D060[i];
                } else {
                    w->flags[i] = D_8014D158[i];
                }
                if (D_8014D060[i] != 0 && D_8014D060[i] != D_8014D158[i]) {
                    w->flags[i] = 1;
                }
            }
            game_state.substep = 32;
        } else {
            while (w->slot_status[g->save_slot] == 0) {
                game_state.save_slot++;
                if (game_state.save_slot >= 15) {
                    for (i = 0; i < 16; i++) {
                        if (D_8014D158[i] < D_8014D060[i]) {
                            w->flags[i] = D_8014D060[i];
                        } else {
                            w->flags[i] = D_8014D158[i];
                        }
                        if (D_8014D060[i] != 0 && D_8014D060[i] != D_8014D158[i]) {
                            w->flags[i] = 1;
                        }
                    }
                    game_state.substep = 32;
                    break;
                }
            }
            if (game_state.substep != 32) {
                w->selected_slot = game_state.save_slot;
                game_state.substep++;
                card_start();
            }
        }
        break;
    case 8:
        if (func_8005448C() == 4) {
            card_stop();
            game_state.substep++;
        } else if ((state = game_state.card_result) == 0xF0) {
            func_80056284();
            main_stage_set(1);
        } else if (((u32) game_state.card_timer >= 0x201U) || (state == 0xD0)) {
            card_stop();
            main_stage_set(0xFF);
        }
        break;
    case 9:
        if (func_80055A38(0) == 1) {
            game_state.substep++;
        } else {
            w->slot_status[game_state.save_slot] = 0xFF;
            game_state.save_slot++;
            game_state.substep = 7;
        }
        break;
    case 10:
        if (func_80055AFC(0) == 1) {
            func_80054E6C();
            game_state.save_slot++;
            game_state.substep = 7;
            for (i = 0; i < 16; i++) {
                if (g->girl_cleared[i] != 0) {
                    D_8014D060[i]++;
                }
            }
        } else {
            w->slot_status[game_state.save_slot] = 0xFF;
            game_state.save_slot++;
            game_state.substep = 7;
        }
        break;
    case 32:
        card_start();
        w->selected_slot = 0;
        game_state.substep++;
        break;
    case 33:
        if (func_80054388() == 0x13) {
            card_stop();
            main_stage_set(2);
            main_step_set(0);
            game_state.substep = 2;
            snd_command(0x50F);
        } else if (((u32) game_state.card_timer >= 0x201U) || ((state = game_state.card_result) == 0xF0)) {
            card_stop();
            func_80056284();
            main_stage_set(1);
        }
        break;
    }
}

/* Original implicit-int state machine falls through without a return statement. */
s32 func_80141820(void) {
    s32 i;
    u8 state;
    SystemData *w = &system_data;
    GameState *g = &game_state;

    switch (game_state.substep) {
    case 0:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "システムファイルを作成して", 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "いいですか？", 0);
        set_kanji_string(-0x40, 0x40, 0xF, (s32) D_800B3C70, 0);
        set_kanji_string(0x14, 0x40, 1, func_8006CA9C(), 0);
        set_kanji_string(-0x40, 0x50, 0xF, (s32) D_800B3C88, 0);
        set_kanji_string(0x14, 0x50, 2, func_8006CAE0(), 0);
        k_disp_start(6);
        game_state.substep++;
        break;
    case 1:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            game_state.substep++;
        } else if (game_state.pad_pressed & PAD_CROSS) {
            func_80056284();
            main_stage_set(1);
        }
        break;
    case 2:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "約１分ほど時間がかかりますが、", 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "途中でメモリーカードを", 0);
        set_kanji_string(-0x40, 0x40, 0xF, (s32) "抜かないで下さい。", 0);
        k_disp_start(3);
        game_state.step_timer.w = 0;
        game_state.substep++;
        break;
    case 3:
        if ((game_state.pad_pressed != 0) || ((u32) game_state.step_timer.w >= 0x41U)) {
            game_state.substep++;
        }
        break;
    case 4:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "準備が出来たら、", 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "何かボタンを押してください。", 0);
        k_disp_start(2);
        game_state.substep++;
        break;
    case 5:
        if (game_state.pad_pressed != 0) {
            game_state.substep++;
        }
        break;
    case 6:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "しばらくお待ち下さい。", 0);
        k_disp_start(1);
        for (i = 0; i < 16; i++) D_8014D060[i] = 0;
        game_state.save_slot = 1;
        game_state.substep++;
        break;
    case 7:
        if (func_80054694(0) == 1) {
            D_800E8BEE = 1;
            game_state.substep++;
        } else {
            main_stage_set(4);
        }
        break;
    case 8:
        if (game_state.save_slot >= 15) {
            game_state.substep = 32;
            for (i = 0; i < 16; i++) {
                w->flags[i] = D_8014D060[i];
            }
        } else {
            while (w->slot_status[g->save_slot] == 0) {
                game_state.save_slot++;
                if (game_state.save_slot >= 15) {
                    for (i = 0; i < 16; i++) {
                        w->flags[i] = D_8014D060[i];
                    }
                    game_state.substep = 32;
                    break;
                }
            }
            if (game_state.substep != 32) {
                w->selected_slot = game_state.save_slot;
                game_state.substep++;
                card_start();
            }
        }
        break;
    case 9:
        if (func_8005448C() == 4) {
            card_stop();
            game_state.substep++;
        } else if ((state = game_state.card_result) == 0xF0) {
            func_80056284();
            main_stage_set(1);
        } else if (((u32) game_state.card_timer >= 0x201U) || (state == 0xD0)) {
            card_stop();
            main_stage_set(0xFF);
        }
        break;
    case 10:
        if (func_80055A38(0) == 1) {
            game_state.substep++;
        } else {
            w->slot_status[game_state.save_slot] = 0xFF;
            game_state.save_slot++;
            game_state.substep = 8;
        }
        break;
    case 11:
        if (func_80055AFC(0) == 1) {
            func_80054E6C();
            game_state.save_slot++;
            game_state.substep = 8;
            for (i = 0; i < 16; i++) {
                if (g->girl_cleared[i] != 0) {
                    D_8014D060[i]++;
                }
            }
        } else {
            w->slot_status[game_state.save_slot] = 0xFF;
            game_state.save_slot++;
            game_state.substep = 8;
        }
        break;
    case 32:
        card_start();
        w->selected_slot = 0;
        game_state.substep++;
        break;
    case 33:
        if (func_80054388() == 0x13) {
            card_stop();
            main_stage_set(2);
            main_step_set(0);
            game_state.substep = 2;
            snd_command(0x50F);
        } else if (((u32) game_state.card_timer >= 0x201U) || ((state = game_state.card_result) == 0xF0)) {
            card_stop();
            func_80056284();
            main_stage_set(1);
        }
        break;
    }
}

s32 func_80141FA4(void) {
    game_state.step_timer.w += 1;
    switch (game_state.step) {
    case 0:
        func_80140F54();
        break;
    case 1:
        func_80140F90();
        break;
    case 2:
        func_80141820();
        break;
    }
    game_state.menu_mode = MENU_MODE_MOUSE_LIKE;
}

s32 func_80142028(void) {
    switch (game_state.substep) {
    case 0:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, "未初期化のメモリーカードです。", 0);
        k_disp_start(1);
        game_state.substep++;
    default:
        break;
    case 1:
        if (game_state.pad_pressed & 0x860) {
            game_state.substep++;
        }
        break;
    case 2:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, "初期化していいですか？", 0);
        set_kanji_string(-0x40, 0x30, 0xF, D_800B3C70, 0);
        set_kanji_string(0x14, 0x30, 1, func_8006CA9C(), 0);
        set_kanji_string(-0x40, 0x40, 0xF, D_800B3C88, 0);
        set_kanji_string(0x14, 0x40, 2, func_8006CAE0(), 0);
        k_disp_start(5);
        game_state.substep++;
        break;
    case 3:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            if (func_80054108() == 1) {
                main_stage_set(2);
            } else {
                main_stage_set(1);
            }
        } else if (game_state.pad_pressed & PAD_CROSS) {
            game_state.substep++;
        }
        break;
    case 4:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, "別のメモリーカードを", 0);
        set_kanji_string(-0x40, 0x30, 1, "差し込み口１", 0);
        set_kanji_string(-0x40, 0x30, 0xF, "　　　　　　にセットして、", 0);
        set_kanji_string(-0x40, 0x40, 0xF, "　ボタンを押してください。", 0);
        set_kanji_string(-0x40, 0x40, 1, func_8006CA9C(), 0);
        k_disp_start(5);
        game_state.substep++;
        break;
    case 5:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            main_stage_set(1);
        }
        break;
    }
}

void func_801422F0(void) {
    if (game_state.step == 0) {
        func_80142028();
    }
}

/* Records 128..140, then 141..151 (flag byte +3). The shared counter keeps the first loop rolled; the second (11 trips) is unrolled by four. T-11080 */
void func_8014231C(void) {
    s32 i;

    for (i = 0; i < 13; i++) {
        ((Actor *)actor_pool)[i + 128].draw_flags &= 0x7F;
    }
    for (i = 0; i < 11; i++) {
        ((Actor *)actor_pool)[i + 141].draw_flags &= 0x7F;
    }
    func_8013AF78(0);
    game_state.stage_timer += 1;
    switch (game_state.stage) {
    case 0x0:
        func_8013C4A8();
        break;
    case 0x1:
        func_8013C934();
        break;
    case 0x2:
        func_80140044();
        break;
    case 0x3:
        func_80140CCC();
        break;
    case 0x4:
        func_80140ED4();
        break;
    case 0xC0:
        func_801422F0();
        break;
    case 0xD0:
        func_80141FA4();
        break;
    case 0xF0:
        func_80140430();
        break;
    case 0xFF:
        func_80140574();
        break;
    }
    func_8013B5F0();
    func_801398EC();
    k_disp_inc();
    cursor_update();
    func_8013B0A0();
    func_801425C0();
    func_80142560();
    if (D_8014D0A8 != 0) {
        game_state.menu_mode = MENU_MODE_MOUSE_LIKE;
    }
}

void func_80142560(void) {
    if ((game_state.stage | game_state.step) != 0) {
        D_801230D0 -= 1;
    }
    if (D_801230D0 > 0) {
        func_8004AC18(D_801230D0);
    }
}

s32 func_801425C0(void) {
    s32 temp_t3;
    s32 temp_t7;
    s32 var_s0;

    for (var_s0 = 0; var_s0 < 14; var_s0++) {
        if (system_data.slot_status[var_s0 + 1] == 0) {
            prim_gradient_rect(((var_s0 / 7) * 0x1F) - 0x90, ((var_s0 % 7) * 0x10) - 0x12, 0x1E, 0xF, 2, 0x808080, 0x81);
        }
        if (system_data.slot_status[var_s0 + 1] == 0xFF) {
            prim_gradient_rect(((var_s0 / 7) * 0x1F) - 0x90, ((var_s0 % 7) * 0x10) - 0x12, 0x1E, 0xF, 2, 0x8080, 0x81);
        }
    }
    dtd_on_tpage(0, 0, 2, 2, 0);
    if (game_state.stage != 0) {
        if (D_800E8BEE != 0) {
            temp_t7 = game_state.frame_count & 0x1F;
            if (temp_t7 != 0) {
                if (temp_t7 == 0x10) {
                    ((Actor *)actor_pool)[126].draw_flags &= 0x7F;
                }
            } else {
                ((Actor *)actor_pool)[126].draw_flags |= 0x80;
            }
            ((Actor *)actor_pool)[126].unk_43 = 0x11;
            ((Actor *)actor_pool)[127].unk_43 = 0;
        } else {
            temp_t3 = game_state.frame_count & 0x1F;
            if (temp_t3 != 0) {
                if (temp_t3 == 0x10) {
                    ((Actor *)actor_pool)[127].draw_flags &= 0x7F;
                }
            } else {
                ((Actor *)actor_pool)[127].draw_flags |= 0x80;
            }
            ((Actor *)actor_pool)[126].unk_43 = 0;
            ((Actor *)actor_pool)[127].unk_43 = 0x11;
        }
    }
}

void func_80142880(void) {
    s8 *sel; /* FAKE: the switch reads the selector through a pointer so the later read stays a separate range; permuter idiom, T-11120 */

    menu_check(1, cursor_x, cursor_y);
    menu_cursor_update(1);
    sel = &game_state.menu_cursor_1;
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (*sel) {
        case 0:
            func_80142A3C();
            main_step_set(0xC0);
            printf("0");
            break;
        case 1:
            func_80142A3C();
            main_step_set(0xA0);
            printf("1");
            break;
        case 2:
            func_80142A3C();
            main_step_set(0xE0);
            printf("2");
            break;
        case 3:
            main_step_set(0);
            printf("3");
            break;
        }
        sprite_quads_clear(0, 0x40);
        if (game_state.menu_cursor_1 >= 0) {
            D_8014D0D4 = 1;
        }
    }
    if (func_801429BC() != 0) {
        sprite_quads_clear(0, 0x40);
    }
}

s32 func_801429BC(void) {
    if (game_state.pad_pressed & PAD_CIRCLE) {
        if (cursor_x < -0x50 && cursor_x >= -0x8F) {
            if (cursor_y < -0x18 && cursor_y >= -0x27) {
                snd_command(0x50F);
                main_stage_set(0xD0);
                return 1;
            }
        }
    }
    return 0;
}

void func_80142A3C(void) {
    s32 i;

    /* FAKE: loop body on the for line; as1 schedules by source line and then fills the delay slot like the original. T-9170 */
    for (i = 0; i < 13; i++) sprite_quads[i].flags = 0x80000000;
}
