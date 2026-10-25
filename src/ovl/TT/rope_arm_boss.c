/* rope_arm_boss.c: src/ovl/TT/801374C0.c:408: boss rope contact, side/core damage, rope extension/breakage, ring expansion and attacks; body 417: c = tt_boss; 418: s = tt_boss_rope_buf;. Original object 0x801374C0-0x8013A040 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/TT.h"

/* Fields used by TT's phase update. T-9720. */
typedef struct TtBossSpinView {
    /* 0x000 */ u8 pad000[0x52];
    /* 0x052 */ u8 frame;
    /* 0x053 */ u8 pad053[0x357];
    /* 0x3AA */ u16 phase;
    /* 0x3AC */ u16 step;
} TtBossSpinView; /* size 0x3AE */

/* The zero-store chain pair on a (stores 0xA, 8, 6 and 0xF, 0xE, 0xD) gives a the reloads that rank it above &pt and tt_boss1_hitbox_rope; the one-line loop head and step fix the order of the i/q home stores. T-11860 */
void tt_boss1_rope_contact(u8 *arg0) {
    u8 *player;
    u8 *shot;
    s32 j;
    s32 n;
    /* FAKE: unused homes (pa/pm) place pt at sp+0x68 and the spilled q, i at sp+0x54/0x58 in the 152-byte frame. T-11000. */
    s32 pa[7];
    s16 pt[2];
    s32 pm[3];
    s32 i;
    u8 *q;

    player = tt_player;
    i = 0; q = arg0; do { /* FAKE: one line, so as1 puts the i/q home stores after the hoisted constants. T-11860 */
        pt[0] = *(s16 *)(q + 0x1E);
        pt[1] = *(s16 *)(q + 0x9E);
        if (player[0x52] == 0 && player[0] != 0 && tt_box_overlap(player + 0x14, tt_hitboxes, (u8 *)pt, tt_boss1_hitbox_rope) != 0) {
            tt_player_hit();
            *(u16 *)(player + 4) = 0x60;
            /* FAKE: TT's zero-store chain pair; the reloads raise player's priority above &pt and tt_boss1_hitbox_rope (s2, s3, s4). T-11860 */
            *(u16 *)(player + 6) = *(u16 *)(player + 8) = *(u16 *)(player + 0xA) = 0;
            player[0xD] = player[0xE] = player[0xF] = 0;
        }
        shot = tt_shots;
        for (j = 0, n = 12; j != n; j++, shot += 0x54) {
            if (shot[0] != 0 && *(u16 *)(shot + 4) == 0x40 && tt_box_overlap(shot + 0x14, &tt_hitbox_shot, (u8 *)pt, tt_boss1_hitbox_rope) != 0) {
                *(u16 *)(shot + 4) = 0x80;
                /* FAKE: TT's zero-store chain pair; same idiom as the record above. T-11860 */
                *(u16 *)(shot + 6) = *(u16 *)(shot + 8) = *(u16 *)(shot + 0xA) = 0;
                shot[0xD] = shot[0xE] = shot[0xF] = 0;
            }
        }
        i += 4; q += 4; /* FAKE: one line, so as1 stores q before i. T-11860 */
    } while (i != 0x20);
}

/* Sibling of tt_boss1_core_shot_damage for the four player slots; the c + 0x24C + i * 2 spelling with c's field first gives the original's load and temp order. T-11860 */
void tt_boss1_side_parts_shot_damage(void) {
    /* FAKE: unused homes (top, frame_pad) place boss at sp+0x90, pt at sp+0x6C and i at sp+0x64 in the 152-byte frame. T-11860 */
    s32 top;
    u8 *boss;
    u8 *core;
    s32 frame_pad[7];
    s16 pt[2];
    u8 *shot;
    s32 i;
    s32 j;

    boss = tt_boss;
    core = tt_core;
    for (i = 0; i < 4; i++) {
        if (boss[i + 0x353] != 0) {
            (boss + i * 0x14)[0x6A] = 0;
            pt[0] = *(s16 *)(boss + 0x14) + *(s16 *)(boss + 0x24C + i * 2);
            pt[1] = *(s16 *)(boss + 0x16) + *(s16 *)(boss + 0x27C + i * 2);
            shot = tt_shots;
            for (j = 0; j != 12; j++, shot += 0x54) {
                if (j == 12) {} /* FAKE: a second use of 12 keeps the constant in s6 (hoisted to entry) instead of &pt. T-11860 */
                if (shot[0] != 0 && *(u16 *)(shot + 4) == 0x40 && tt_box_overlap(shot + 0x14, &tt_hitbox_shot, (u8 *)pt, &tt_boss1_hitbox_side_part) != 0) {
                    core[0x25] = core[0x25] + 1;
                    if (core[0x24] != 0) {
                        if (!(core[0x25] & 3)) {
                            tt_enemy_shot_fire_aimed(*(s16 *)(shot + 0x14), *(s16 *)(shot + 0x16), 0x20, 0xA0);
                        }
                    }
                    tt_se_request(0x507);
                    if (boss[i + 0x353] != 0) {
                        if (j >= 6) {
                            *(s16 *)(boss + i * 2 + 0x2D8) -= 1;
                        } else if (*(u16 *)(shot + 2) == 9) {
                            *(s16 *)(boss + i * 2 + 0x2D8) -= 4;
                        } else {
                            *(s16 *)(boss + i * 2 + 0x2D8) -= 2;
                        }
                        if (*(s16 *)(boss + i * 2 + 0x2D8) < 0) {
                            tt_se_request(0x505);
                            boss[i + 0x353] = 0;
                            (boss + i * 0x14)[0x69] = 0;
                        }
                    }
                    (boss + i * 0x14)[0x6A] = 1;
                    *(u16 *)(shot + 4) = 0x80;
                    shot[13] = 0;
                    shot[14] = 0;
                    shot[15] = 0;
                    *(s16 *)(shot + 6) = 0;
                    *(s16 *)(shot + 8) = 0;
                    *(s16 *)(shot + 10) = 0;
                }
            }
        }
    }
}

void tt_boss1_core_shot_damage(void) {
    u8 *boss;
    u8 *core;
    /* FAKE: unused homes retain the observed frame. T-11000. */
    s32 frame_pad[8];
    s16 pt[2];
    u8 *shot;
    s32 i;
    s32 n;

    boss = tt_boss;
    pt[0] = *(s16 *)(boss + 0x14) + *(s16 *)(boss + 0x254);
    core = tt_core;
    pt[1] = *(s16 *)(boss + 0x16) + *(s16 *)(boss + 0x284);
    boss[0xBA] = 0;
    shot = tt_shots;
    for (i = 0, n = 12; i != n; i++, shot += 0x54) {
        if (shot[0] != 0 && *(u16 *)(shot + 4) == 0x40 && tt_box_overlap(shot + 0x14, &tt_hitbox_shot, (u8 *)pt, &tt_boss1_hitbox_core_shot) != 0) {
            if (*(u16 *)(boss + 0x30E) == 2) {
                core[0x25] = core[0x25] + 1;
                if (core[0x24] != 0) {
                    if (!(core[0x25] & 3)) {
                        if (!(core[0x25] & 7)) {
                            tt_enemy_shot_fire_fan4_aimed(*(s16 *)(shot + 0x14), *(s16 *)(shot + 0x16), 0x100, 0x20, 0xA0);
                        } else {
                            tt_enemy_shot_fire_3way_aimed(*(s16 *)(shot + 0x14), *(s16 *)(shot + 0x16), 0x100, 0x20, 0xA0);
                        }
                    }
                }
                tt_se_request(0x507);
                if (i >= 6) {
                    *(s16 *)(boss + 0x30C) -= 1;
                } else if (*(u16 *)(shot + 2) == 9) {
                    *(s16 *)(boss + 0x30C) -= 4;
                } else {
                    *(s16 *)(boss + 0x30C) -= 2;
                }
            } else {
                tt_se_request(0x506);
            }
            boss[0xBA] = 1;
            *(u16 *)(shot + 4) = 0x80;
            shot[13] = 0;
            shot[14] = 0;
            shot[15] = 0;
            *(s16 *)(shot + 6) = 0;
            *(s16 *)(shot + 8) = 0;
            *(s16 *)(shot + 10) = 0;
        }
    }
}

/* t = s; s += 0xEFC; before the test keeps s in a2 (spilled only around calls) and puts the addiu in the branch delay slot; the comma-for header orders i = 0 before the tt_sprites load. T-11860 */
void tt_boss1_collision(void) {
    u8 *boss;
    u8 *rope;
    u8 *p;
    s32 i;
    u8 *t;

    boss = tt_boss;
    rope = tt_boss_rope_buf;
    if (boss[0] != 0) {
        for (i = 0; i < 24; i++) {
            boss[i * 0x14 + 0x42] = 0;
        }
        if (boss[0x342] == 0) {
            if (tt_boss_check_hits() != 0) {
                tt_se_request(0x508);
                for (i = 0, p = tt_sprites + 0xFA0; i < 7; i++) {
                    p -= 0x64;
                    *(s16 *)(p + 0xA6) = 0x3CDF;
                }
                p = tt_sprites + 0x12C0;
                for (i = 0; i < 7; i++, p -= 0x64) {
                    *(s16 *)(p + 0x42) = 0x3CDF;
                }
            } else {
                for (i = 0, p = tt_sprites + 0xFA0; i < 7; i++) {
                    p -= 0x64;
                    *(s16 *)(p + 0xA6) = 0x3CD9;
                }
                p = tt_sprites + 0x12C0;
                for (i = 0; i < 7; i++, p -= 0x64) {
                    *(s16 *)(p + 0x42) = 0x3CD9;
                }
            }
        }
        t = rope;
        rope += 0xEFC;
        if (boss[0x343] == 0) {
            tt_boss1_rope_contact(t);
            tt_boss1_rope_contact(rope);
        }
        if (*(u16 *)(boss + 0x30E) == 1) {
            tt_boss1_side_parts_shot_damage();
            tt_boss1_core_shot_damage();
        } else if (*(u16 *)(boss + 0x30E) == 2) {
            tt_boss1_core_shot_damage();
        }
    }
}

/* The u16 store of 8 lets the constant share t5 with the s[0x1A] store; the 0x11C ring row is spelled like the packet loops' s + i * 4 loads so both share one induction variable (a3), and the 2D-array rows keep i * 4 as the loop test; the packet loops' grouped stores share a source line because as1 schedules the stores of one line together. T-11840 */
void tt_boss1_init(void) {
    u8 *boss;
    u8 *s;
    u8 *p;
    s32 i;
    s32 j;

    boss = tt_boss;
    s = tt_boss_rope_buf;
    tt_boss_parts_hide_all();
    tt_boss_hitboxes_clear();
    boss[0] = 1;
    boss[0x342] = 0;
    *(s32 *)(boss + 0x20) = 0;
    *(s32 *)(boss + 0x24) = 0xFFC00000;
    *(s16 *)(boss + 0x14) = 0;
    *(s16 *)(boss + 0x16) = -0x40;
    *(s32 *)(boss + 0x28) = 0;
    *(s32 *)(boss + 0x2C) = 0x20000;
    for (i = 0; i < 4; i++) {
        ((TtPartAnimation *)(boss + 0x38))[i].sets = tt_sprite_set_table;
        ((TtPartAnimation *)(boss + 0x38))[i].flip = 0;
        ((TtPartAnimation *)(boss + 0x38))[i].set = tt_boss1_part_set[i];
        ((TtPartAnimation *)(boss + 0x38))[i].frame = tt_boss1_part_frame[i];
        ((TtPartAnimation *)(boss + 0x38))[i].flags = 0x60;
        ((TtPartAnimation *)(boss + 0x38))[i].clut = 0xF;
        ((TtPartAnimation *)(boss + 0x38))[i].transform_enable = 0;
        boss[0x310 + i] = 1;
        if (i < 2) {
            *(s16 *)(boss + 0x2A8 + i * 2) = 10;
        } else {
            *(u16 *)(boss + 0x2A8 + i * 2) = 8;
        }
        *(s16 *)(boss + 0x248 + i * 2) = 0;
        *(s16 *)(boss + 0x278 + i * 2) = 0;
    }
    *(s16 *)(boss + 0x278) = -6;
    *(s16 *)(boss + 0x24C) = 0x18;
    *(s16 *)(boss + 0x27C) = 0x34;
    *(s16 *)(boss + 0x24E) = -0x18;
    *(s16 *)(boss + 0x27E) = 0x34;
    *(s16 *)(boss + 0x30C) = 0x60;
    *(s16 *)(boss + 0x30E) = 0;
    *(s16 *)(boss + 0x3A4) = 0;
    *(s16 *)(boss + 0x3A6) = 0;
    *(s16 *)(boss + 0x3A8) = 0;
    *(s16 *)(boss + 0x3AA) = 0;
    *(s16 *)(boss + 0x3AC) = 0;
    *(s16 *)(boss + 0x364) = 0;
    *(s16 *)(boss + 0x366) = 0;
    *(s16 *)(boss + 0x368) = 0;
    *(s16 *)(boss + 0x36A) = 0;
    *(s16 *)(boss + 0x36C) = 0;
    boss[0x343] = 0;
    *(s16 *)s = *(s16 *)(boss + 0x14) + 0x1C;
    *(s16 *)(s + 2) = *(s16 *)(boss + 0x16) - 0x22;
    *(s16 *)(s + 0x10) = 0xC00;
    *(s16 *)(s + 0x14) = 0xC00;
    *(s16 *)(s + 0x12) = 0xC00;
    *(s16 *)(s + 0x16) = 0xC00;
    *(s16 *)(s + 0x18) = 0x100;
    s[0x1A] = 8;
    s[0x1B] = 4;
    *(s32 *)(s + 0xC) = 0x380;
    *(s32 *)(s + 8) = 0x1C0;
    for (i = 0; i < 32; i++) {
        *(s32 *)(s + 0x11C + i * 4) = 0x380;
        ((s16 *)(s + 0x99C))[i] = 0xC00;
        ((s16 *)(s + 0xDDC))[i] = 0;
        for (j = 0; j < 16; j++) {
            ((s32 (*)[16])(s + 0x19C))[i][j] = 0x380;
            ((s16 (*)[16])(s + 0x9DC))[i][j] = 0xC00;
        }
    }
    tt_rope_update(s);
    *(s16 *)(s + 0xEFC) = *(s16 *)(boss + 0x14) - 0x1C;
    s += 0xEFC;
    *(s16 *)(s + 2) = *(s16 *)(boss + 0x16) - 0x22;
    *(s16 *)(s + 0x10) = 0xC00;
    *(s16 *)(s + 0x14) = 0xC00;
    *(s16 *)(s + 0x12) = 0xC00;
    *(s16 *)(s + 0x16) = 0xC00;
    *(s16 *)(s + 0x18) = 0x100;
    s[0x1A] = 8;
    s[0x1B] = 4;
    *(s32 *)(s + 0xC) = 0x380;
    *(s32 *)(s + 8) = 0x1C0;
    for (i = 0; i < 32; i++) {
        *(s32 *)(s + 0x11C + i * 4) = 0x380;
        ((s16 *)(s + 0x99C))[i] = 0xC00;
        ((s16 *)(s + 0xDDC))[i] = 0;
        for (j = 0; j < 16; j++) {
            ((s32 (*)[16])(s + 0x19C))[i][j] = 0x380;
            ((s16 (*)[16])(s + 0x9DC))[i][j] = 0xC00;
        }
    }
    tt_rope_update(s);
    p = tt_sprites + 0xFA0;
    for (i = 0; i < 7; i++) {
        p[0] = 1;
        p[1] = 0;
        *(s16 *)(p + 0x44) = 0;
        *(s16 *)(p + 0x14) = *(s16 *)(boss + 0x14) + 0x14;
        *(s16 *)(p + 0x16) = *(s16 *)(boss + 0x16) + i * 8 - 0x2A;
        *(s16 *)(p + 0x14) = *(s16 *)(s + 0x1E + i * 4) - 8;
        p -= 0x64;
        *(s16 *)(p + 0x7A) = *(s16 *)(s + 0x9E + i * 4) - 8;
        *(s16 *)(p + 0xA0) = 0x10; *(s16 *)(p + 0xA2) = 0x10; /* FAKE: stores grouped on one line keep the original as1 order. T-11840 */
        p[0xAA] = 0x90; p[0xAB] = 0x80; /* FAKE: one line, as above. T-11840 */
        p[0x9C] = 0x80; p[0x9D] = 0x80; p[0x9E] = 0x80; /* FAKE: one line, as above. T-11840 */
        *(s16 *)(p + 0xA4) = 0x12;
        *(s16 *)(p + 0xA6) = 0x3CD9;
        p[0x9F] = 0x65;
        *(s16 *)(p + 0x74) = 9;
    }
    p = tt_sprites + 0x12C0;
    for (i = 0; i < 7; i++, p -= 0x64) {
        p[0] = 1;
        p[1] = 0;
        *(s16 *)(p + 0x44) = 0;
        *(s16 *)(p + 0x14) = *(s16 *)(boss + 0x14) - 0x24;
        *(s16 *)(p + 0x16) = *(s16 *)(boss + 0x16) + i * 8 - 0x2A;
        *(s16 *)(p + 0x14) = *(s16 *)(s + 0x1E + i * 4) - 8;
        *(s16 *)(p + 0x16) = *(s16 *)(s + 0x9E + i * 4) - 8;
        *(s16 *)(p + 0x3C) = 0x10; *(s16 *)(p + 0x3E) = 0x10; /* FAKE: one line, as above. T-11840 */
        p[0x46] = 0x90; p[0x47] = 0x80; /* FAKE: one line, as above. T-11840 */
        p[0x38] = 0x80; p[0x39] = 0x80; p[0x3A] = 0x80; /* FAKE: one line, as above. T-11840 */
        *(s16 *)(p + 0x40) = 0x12;
        *(s16 *)(p + 0x42) = 0x3CD9;
        p[0x3B] = 0x65;
        *(s16 *)(p + 0x10) = 9;
    }
    *(u8 **)(boss + 0x238) = tt_boss1_hitbox_core;
}

void tt_boss1_enter(void) {
    u8 *boss = tt_boss;

    *(s32 *)(boss + 0x20) = 0;
    *(s32 *)(boss + 0x24) += 0x10000;
    *(s16 *)(boss + 0x14) = *(s16 *)(boss + 0x22);
    *(s16 *)(boss + 0x16) = *(s16 *)(boss + 0x26);
    if (*(s16 *)(boss + 0x16) >= 0x50) {
        *(s32 *)(boss + 0x24) = 0x500000;
        *(s16 *)(boss + 0x16) = *(s16 *)(boss + 0x26);
        *(s16 *)(boss + 0xA) = 0;
        *(s16 *)(boss + 8) = 0;
        *(s16 *)(boss + 6) = 0;
        boss[0xF] = 0;
        boss[0xE] = 0;
        boss[0xD] = 0;
        *(s16 *)(boss + 4) = 0x30;
    }
}

void tt_boss1_intro_animation(void) {
    u8 *boss = tt_boss;

    tt_boss_part_animation_step(tt_boss1_intro_script, 0);
    if (boss[0x40] & 1) {
        *(u8 **)(boss + 0x218) = tt_boss1_hitbox_body;
        *(s16 *)(boss + 4) = 0x40;
    }
}

void tt_boss1_track_player_x(void) {
    u8 *boss = tt_boss;
    u8 *player = tt_player;
    s32 d = *(s16 *)(player + 0x14) - *(s16 *)(boss + 0x14);
    s32 ad;
    s16 y;

    if (d < 0) {
        ad = -d;
    } else {
        ad = d;
    }
    if (ad < 2) {
        *(s32 *)(boss + 0x20) = *(s16 *)(player + 0x14) << 16;
    } else if (d < 0) {
        *(s32 *)(boss + 0x20) -= 0x4000;
    } else {
        *(s32 *)(boss + 0x20) += 0x4000;
    }
    y = *(s16 *)(boss + 0x22);
    if (y >= 0x51) {
        *(s32 *)(boss + 0x20) = 0x500000;
        y = *(s16 *)(boss + 0x22);
    } else if (y < -0x50) {
        *(s32 *)(boss + 0x20) = 0xFFB00000;
        y = *(s16 *)(boss + 0x22);
    }
    *(s16 *)(boss + 0x14) = y;
}

/* Operand order of the sums (x + f() / 64, table lookups) sets the temp numbering; p -= 0x64 before the second store of the loop gives the original delay-slot fill; s += 0xEFC reuses s for the second half. T-11520 */
void tt_boss1_rope_update(void) {
    u8 *boss;
    u8 *s;
    u8 *player;
    u8 *p;
    s32 i;
    s16 x;
    u16 v;

    boss = tt_boss;
    s = tt_boss_rope_buf;
    player = tt_player;
    *(s16 *)s = *(s16 *)(boss + 0x14) + 0x1C;
    *(s16 *)(s + 2) = *(s16 *)(boss + 0x16) - 0x22;
    if (*(u16 *)(boss + 0x36A) < 0x100) {
        *(s16 *)(s + 4) = *(s16 *)(player + 0x14);
        *(s16 *)(s + 6) = *(s16 *)(player + 0x16);
        if (*(u16 *)(boss + 0x3A4) != 0) {
            *(u16 *)(boss + 0x3A8) = 0x400;
        } else {
            *(u16 *)(boss + 0x3A8) = 0x2400;
        }
    } else {
        *(s16 *)(s + 4) = *(s16 *)s + func_800A0140(*(u16 *)(boss + 0x3A8)) / 64;
        *(s16 *)(s + 6) = *(s16 *)(s + 2) + rsin(*(u16 *)(boss + 0x3A8)) / 64;
        if (*(u16 *)(boss + 0x3A4) != 0) {
            *(u16 *)(boss + 0x3A8) += 0x30;
            if (*(u16 *)(boss + 0x3A8) >= 0x2400) {
                *(u16 *)(boss + 0x36A) = 0;
            }
        } else {
            *(u16 *)(boss + 0x3A8) -= 0x30;
            if (*(u16 *)(boss + 0x3A8) < 0x401) {
                *(u16 *)(boss + 0x36A) = 0;
            }
        }
    }
    *(u16 *)(boss + 0x36A) += 1;
    tt_rope_update(s);
    p = tt_sprites + 0xFA0;
    for (i = 0; i < 7; i++) {
        *(s16 *)(p + 0x14) = *(s16 *)(s + 0x1E + i * 4) - 8;
        p -= 0x64;
        *(s16 *)(p + 0x7A) = *(s16 *)(s + 0x9E + i * 4) - 8;
    }
    x = *(s16 *)(boss + 0x14);
    *(s16 *)(boss + 0x24C) = *(s16 *)(s + 0x3A) - x;
    *(s16 *)(boss + 0x27C) = *(s16 *)(s + 0xBA) - *(s16 *)(boss + 0x16);
    boss[0x66] = tt_boss1_arm_frame_by_angle[((*(s16 *)(s + 0x9AA) + 0x80) & 0xFFF) / 256] + tt_boss1_arm_frame_cycle[(*(u16 *)(boss + 0x364) >> 3) & 3];
    boss[0x64] = tt_boss1_arm_flip_by_angle[((*(s16 *)(s + 0x9AA) + 0x80) & 0xFFF) / 256];
    s += 0xEFC;
    *(s16 *)s = x - 0x1C;
    *(s16 *)(s + 2) = *(s16 *)(boss + 0x16) - 0x22;
    if (*(u16 *)(boss + 0x368) < 0x100) {
        *(s16 *)(s + 4) = *(s16 *)(player + 0x14);
        *(s16 *)(s + 6) = *(s16 *)(player + 0x16);
        if (*(u16 *)(boss + 0x3A4) != 0) {
            *(u16 *)(boss + 0x3A6) = 0x2400;
        } else {
            *(u16 *)(boss + 0x3A6) = 0x400;
        }
    } else {
        *(s16 *)(s + 4) = *(s16 *)s + func_800A0140(*(u16 *)(boss + 0x3A6)) / 64;
        *(s16 *)(s + 6) = *(s16 *)(s + 2) + rsin(*(u16 *)(boss + 0x3A6)) / 64;
        if (*(u16 *)(boss + 0x3A4) != 0) {
            *(u16 *)(boss + 0x3A6) -= 0x30;
            if (*(u16 *)(boss + 0x3A6) < 0x401) {
                *(u16 *)(boss + 0x368) = 0;
                *(u16 *)(boss + 0x3A4) += 1;
                *(u16 *)(boss + 0x3A4) &= 1;
            }
        } else {
            *(u16 *)(boss + 0x3A6) += 0x30;
            if (*(u16 *)(boss + 0x3A6) >= 0x2400) {
                *(u16 *)(boss + 0x368) = 0;
                *(u16 *)(boss + 0x3A4) += 1;
                *(u16 *)(boss + 0x3A4) &= 1;
            }
        }
    }
    *(u16 *)(boss + 0x368) += 1;
    tt_rope_update(s);
    p = tt_sprites + 0x12C0;
    for (i = 0; i < 7; i++) {
        *(s16 *)(p + 0x14) = *(s16 *)(s + 0x1E + i * 4) - 8;
        *(s16 *)(p + 0x16) = *(s16 *)(s + 0x9E + i * 4) - 8;
        p -= 0x64;
    }
    *(s16 *)(boss + 0x24E) = *(s16 *)(s + 0x3A) - *(s16 *)(boss + 0x14);
    v = *(u16 *)(boss + 0x364);
    *(s16 *)(boss + 0x27E) = *(s16 *)(s + 0xBA) - *(s16 *)(boss + 0x16);
    boss[0x7A] = tt_boss1_arm_frame_by_angle[((*(s16 *)(s + 0x9AA) + 0x80) & 0xFFF) / 256] + tt_boss1_arm_frame_cycle[(v >> 3) & 3];
    boss[0x78] = tt_boss1_arm_flip_by_angle[((*(s16 *)(s + 0x9AA) + 0x80) & 0xFFF) / 256];
    *(u16 *)(boss + 0x364) = v + 1;
}

/* Sibling of tt_boss1_side_parts_init: the s16 0x3D4 store lets 8 share t0 with the loop stores; the 2D-array view of the s rows keeps 0x18 in the strength-reduced pointer of the unrolled loop. T-11860 */
void tt_boss1_rope_break(void) {
    u8 *boss;
    u8 *rope;
    u8 *sprite;
    s32 i;
    s32 j;

    boss = tt_boss;
    rope = tt_boss_rope_buf;
    sprite = tt_sprites;
    switch (*(u16 *)(boss + 6)) {
    case 0:
        boss[0x343] = 1;
        for (i = 2; i << 0 < 4; i++) { /* FAKE: an opaque test keeps IDO'rope unroller off this loop (slti on i, as in the original). T-11860 */
            ((TtPartAnimation *)(boss + 0x38))[i].flags = 0x80;
            ((TtPartAnimation *)(boss + 0x38))[i].flip = 0;
            ((TtPartAnimation *)(boss + 0x38))[i].set = 2;
            ((TtPartAnimation *)(boss + 0x38))[i].frame = tt_boss1_break_script[0];
        }
        for (j = 0; j * 4 < 0x1C; j++) { /* FAKE: same; the original unrolls the second record loop but not this one. T-11860 */
            ((TtPartAnimationView *)(boss + 0xD8))[j].sets = tt_sprite_set_table;
            ((TtPartAnimationView *)(boss + 0xD8))[j].flip = 0;
            ((TtPartAnimationView *)(boss + 0xD8))[j].set = 2;
            ((TtPartAnimationView *)(boss + 0xD8))[j].unk_0A = 0;
            ((TtPartAnimationView *)(boss + 0xD8))[j].transform_enable = 0;
            boss[0x318 + j] = 0;
            ((s16 *)(boss + 0x2B8))[j] = 8;
            ((s16 *)(boss + 0x258))[j] = ((s16 (*)[2])(rope + 0x1C))[6 - j][1] - *(s16 *)(boss + 0x14);
            ((s16 *)(boss + 0x288))[j] = ((s16 (*)[2])(rope + 0x9C))[6 - j][1] - *(s16 *)(boss + 0x16) - 8;
            ((TtPartAnimationView *)(boss + 0xD8))[j].flags = 0x80;
        }
        rope += 0xEFC;
        for (i = 0; i != 7; i++) {
            ((TtPartAnimationView *)(boss + 0x178))[i].sets = tt_sprite_set_table;
            ((TtPartAnimationView *)(boss + 0x178))[i].flip = 0;
            ((TtPartAnimationView *)(boss + 0x178))[i].set = 2;
            ((TtPartAnimationView *)(boss + 0x178))[i].unk_0A = 0;
            ((TtPartAnimationView *)(boss + 0x178))[i].transform_enable = 0;
            boss[0x320 + i] = 0;
            ((s16 *)(boss + 0x2C8))[i] = 8; /* FAKE: the array spelling drops a phantom range so rope keeps t5. T-11860 */
            ((s16 *)(boss + 0x268))[i] = ((s16 (*)[2])(rope + 0x1C))[6 - i][1] - *(s16 *)(boss + 0x14);
            ((s16 *)(boss + 0x298))[i] = ((s16 (*)[2])(rope + 0x9C))[6 - i][1] - *(s16 *)(boss + 0x16) - 8;
            ((TtPartAnimationView *)(boss + 0x178))[i].flags = 0x80;
        }
        *(u16 *)(boss + 0x36C) = 0;
        *(s16 *)(boss + 0x3D4) = 8;
        *(s16 *)(boss + 6) = 0x40;
        break;
    case 0x40:
        tt_boss_part_animation_step(tt_boss1_break_script, 2);
        tt_boss_part_animation_step(tt_boss1_break_script, 3);
        if ((*(u16 *)(boss + 0x36C) & 7) == 7) {
            if (*(u16 *)(boss + 0x3D4) < 0xF) {
                tt_se_request(0x50A);
                ((TtPartAnimation *)(boss + 0x38))[*(u16 *)(boss + 0x3D4)].flags = 0x80;
                boss[0x310 + *(u16 *)(boss + 0x3D4)] = 1;
                ((TtPartAnimation *)(boss + 0xD8))[*(u16 *)(boss + 0x3D4)].flags = 0x80;
                boss[0x318 + *(u16 *)(boss + 0x3D4)] = 1;
                (sprite + 0xD48)[*(u16 *)(boss + 0x3D4) * 0x64] = 0;
                (sprite + 0xA28)[*(u16 *)(boss + 0x3D4) * 0x64] = 0;
            }
            if (*(u16 *)(boss + 0x3D4) >= 0x13) {
                *(s16 *)(boss + 4) = 0x51;
                *(s16 *)(boss + 6) = 0;
            }
            *(u16 *)(boss + 0x3D4) += 1;
        }
        for (i = 8; i < 15; i++) {
            tt_boss_part_animation_step(tt_boss1_break_script, i);
        }
        for (i = 16; i != 23; i++) {
            tt_boss_part_animation_step(tt_boss1_break_script, i);
        }
        break;
    }
    *(u16 *)(boss + 0x36C) += 1;
}

void tt_boss1_side_parts_init(void) {
    u8 *boss;
    s32 i;

    boss = tt_boss;
    /* IDO unrolls this by two after peeling i = 2; the typed record index folds the second copy into offsets */
    for (i = 2; i < 7; i++) {
        ((TtPartAnimation *)(boss + 0x38))[i].sets = tt_sprite_set_table;
        ((TtPartAnimation *)(boss + 0x38))[i].flip = 0;
        ((TtPartAnimation *)(boss + 0x38))[i].set = 6;
        if (i == 6) {
            ((TtPartAnimation *)(boss + 0x38))[i].frame = 0x14;
        } else {
            ((TtPartAnimation *)(boss + 0x38))[i].frame = 0x1F;
        }
        ((TtPartAnimation *)(boss + 0x38))[i].flags = 0x60;
        ((TtPartAnimation *)(boss + 0x38))[i].hold = 0;
        ((TtPartAnimation *)(boss + 0x38))[i].clut = 0xF;
        ((TtPartAnimation *)(boss + 0x38))[i].transform_enable = 0;
        boss[0x310 + i] = 1;
        *(s16 *)(boss + 0x2A8 + i * 2) = 9;
        if (i == 6) {
            *(s16 *)(boss + 0x248 + i * 2) = 0;
        } else {
            *(s16 *)(boss + 0x248 + i * 2) = i * 16 - 0x38;
        }
        *(s16 *)(boss + 0x278 + i * 2) = tt_boss1_core_part_y[i - 2];
    }
}

void tt_boss1_core_animation(void) {
    switch (*(u16 *)(tt_boss + 0x30E)) {
    case 1:
        tt_boss_part_animation_step(tt_boss1_core_script_1, 6);
        break;
    case 2:
        tt_boss_part_animation_step(tt_boss1_core_script_2, 6);
        break;
    case 3:
        tt_boss_part_animation_step(tt_boss1_core_script_3, 6);
        break;
    }
}

void tt_boss1_side_part_animation(void) {
    u8 *base;
    s32 i;

    base = tt_boss;
    for (i = 2; i < 6; i++) {
        if (base[i + 0x351] != 0) {
            tt_boss_part_animation_step(tt_boss1_side_part_script_alive, i);
        } else {
            tt_boss_part_animation_step(tt_boss1_side_part_script_broken, i);
        }
    }
}

/* T-11510 near miss: structure matches; differences are the register numbers of the hoisted constants in the case-0 slot loop (the original ranks 8, the table pointer, 4, then the bound 18; here 18 comes first) and the delay-slot fill of the sltiu before the 0x4B test; about 73 differing instructions (permuter 8 min: no gain). */
void tt_boss1_expand_ring(void) {
    u8 *boss;
    u8 *transform;
    u8 *p;
    u8 *e;
    s16 *t;
    s16 j;
    s32 i;
    u16 k;

    k = *(u16 *)(tt_boss + 6);
    boss = tt_boss;
    transform = tt_transforms;
    switch (k) {
    case 0:
        *(u16 *)(boss + 0x364) = 0;
        *(u16 *)(boss + 0x3D4) = 0;
        /* The unsigned bound is a constant of its own; an int 0x12 would share the hoisted register of the 0x12 stored in the 0x4B block and change the register order. T-11830 */
        for (i = 0x10; i != 0x12U; i++) {
            ((TtPartAnimationView *)(boss + 0x38))[i].sets = tt_sprite_set_table;
            ((TtPartAnimationView *)(boss + 0x38))[i].flip = 0;
            ((TtPartAnimationView *)(boss + 0x38))[i].set = 4;
            ((TtPartAnimationView *)(boss + 0x38))[i].unk_0A = 0;
            ((TtPartAnimationView *)(boss + 0x38))[i].transform_enable = 0;
            boss[0x310 + i] = 0;
            *(s16 *)(boss + 0x2A8 + i * 2) = 8;
            *(s16 *)(boss + 0x248 + i * 2) = 0;
            *(s16 *)(boss + 0x278 + i * 2) = 0;
        }
        *(s16 *)(boss + 6) = 0x40;
        return;
    case 0x40:
        t = tt_boss1_expand_timeline;
        e = tt_boss;
        if (t[*(u16 *)(e + 0x3D4)] != -1 && t[*(u16 *)(e + 0x3D4)] == *(u16 *)(e + 0x364)) {
            tt_se_request(0x501);
            j = t[*(u16 *)(boss + 0x3D4) + 4];
            boss[j + 0x310] = 1;
            boss[j * 0x14 + 0x40] = 0x80;
            *(s16 *)(boss + j * 2 + 0x248) = t[*(u16 *)(boss + 0x3D4) + 1];
            *(s16 *)(boss + j * 2 + 0x278) = t[*(u16 *)(boss + 0x3D4) + 2];
            *(s16 *)(boss + j * 2 + 0x2A8) = t[*(u16 *)(boss + 0x3D4) + 3];
            *(u16 *)(boss + 0x3D4) += 5;
        }
        for (i = 0x10; i < 0x12; i++) {
            tt_boss_part_animation_step(tt_boss_death_part_script, i);
        }
        if (*(u16 *)(boss + 0x364) == 0x4B) {
            boss[0x310] = 0;
            tt_boss1_side_parts_init();
            boss[0x52] = 1;
            p = tt_sprites + 0x1900;
            for (i = 0; i != 0x10; i++, p += 0x64) {
                p[0] = 1;
                p[1] = 0;
                *(s16 *)(p + 0x44) = 0;
                p[0x3B] = 0x80;
                *(u8 **)(p + 0x48) = transform;
                p[0x4D] = 0;
                p[0x4E] = 1;
                p[0x50] = 0;
                *(s16 *)(p + 0x14) = *(s16 *)(boss + 0x14) - 8;
                *(s16 *)(p + 0x16) = *(s16 *)(boss + 0x16) - 8;
                p[0x47] = (i & 1) * 0x10 + 0x40;
                p[0x46] = 0xF0;
                p[0x4F] = 0;
                *(s16 *)(p + 0x3C) = 0x10;
                *(s16 *)(p + 0x3E) = 0x10;
                *(s16 *)(p + 0x40) = 0x12;
                *(s16 *)(p + 0x42) = 0x3CD2;
                p[0x4C] = 0;
                *(s16 *)(p + 0x10) = 8;
            }
            *(u16 *)(boss + 0x3D4) = 0;
        }
        if (*(u16 *)(boss + 0x364) >= 0x4C) {
            p = tt_sprites + 0x1900;
            for (i = 0; i < 0x10; i++, p += 0x64) {
                *(s16 *)(p + 0x14) = *(s16 *)(boss + 0x14) + ((u32)(func_800A0140(i * 0x100) * *(u16 *)(boss + 0x3D4)) >> 12) - 8;
                *(s16 *)(p + 0x16) = *(s16 *)(boss + 0x16) + ((u32)(rsin(i * 0x100) * *(u16 *)(boss + 0x3D4)) >> 12) - 8;
                p[0x4C] = tt_boss1_ring_palette_cycle[(*(u16 *)(boss + 0x364) >> 2) & 3];
            }
            *(u16 *)(boss + 0x3D4) += 4;
        }
        if (*(u16 *)(boss + 0x364) >= 0x88) {
            p = tt_sprites + 0x1900;
            for (i = 0; i < 0x10; i++, p += 0x64) {
                p[0] = 0;
            }
            *(s16 *)(boss + 4) = 0x52;
            *(s16 *)(boss + 6) = 0;
        }
        *(u16 *)(boss + 0x364) += 1;
        return;
    }
}

void tt_boss1_spin_step(void) {
    TtBossSpinView *work;
    u16 step;
    u16 phase;

    work = (TtBossSpinView *)tt_boss;
    phase = work->phase += work->step;
    if (phase >= 0x800U) {
        phase = work->phase = 0;
    }
    step = work->step;
    work->frame = (s8) ((phase >> 8) + 1);
    if (step < 0x100U) {
        work->step = (u16) (step + 1);
        return;
    }
    work->step = 0x100U;
}

/* Writing the case-0 short arrays as *(s16 *)(c + 0x2A8 + i * 2) makes c + i * 2 one range with the 0x40 loops (T-11520); TT's zero-store chain pair in the 0xFD block puts the shared c[0x364] tail value in a0. T-11860 */
void tt_boss1_death_sequence(void) {
    u8 *boss;
    u8 *e;
    s16 *t;
    s16 j;
    s32 i;
    u16 k;
    u16 m;

    k = *(u16 *)(tt_boss + 6);
    boss = tt_boss;
    switch (k) {
    case 0:
        tt_boss_hitboxes_clear();
        *(u16 *)(boss + 0x364) = 0;
        *(u16 *)(boss + 0x3D4) = 0;
        for (i = 0x10; i < 0x13; i++) {
            ((TtPartAnimationView *)(boss + 0x38))[i].sets = tt_sprite_set_table;
            ((TtPartAnimationView *)(boss + 0x38))[i].flip = 0;
            ((TtPartAnimationView *)(boss + 0x38))[i].set = 4;
            ((TtPartAnimationView *)(boss + 0x38))[i].unk_0A = 0;
            ((TtPartAnimationView *)(boss + 0x38))[i].transform_enable = 0;
            boss[0x310 + i] = 0;
            *(s16 *)(boss + 0x2A8 + i * 2) = 8;
            *(s16 *)(boss + 0x248 + i * 2) = 0;
            *(s16 *)(boss + 0x278 + i * 2) = 0;
        }
        for (i = 0x13; i != 0x18; i++) {
            boss[0x310 + i] = 0;
        }
        *(s16 *)(boss + 6) = 0x40;
        *(s16 *)(boss + 8) = 0;
        break;
    case 0x40:
        t = tt_boss1_death_timeline;
        e = tt_boss;
        if (t[*(u16 *)(e + 0x3D4)] != -1 && t[*(u16 *)(e + 0x3D4)] == *(u16 *)(e + 0x364)) {
            tt_se_request(0x501);
            j = t[*(u16 *)(boss + 0x3D4) + 4];
            boss[j + 0x310] = 1;
            boss[j * 0x14 + 0x40] = 0x80;
            *(s16 *)(boss + j * 2 + 0x248) = t[*(u16 *)(boss + 0x3D4) + 1];
            *(s16 *)(boss + j * 2 + 0x278) = t[*(u16 *)(boss + 0x3D4) + 2];
            *(s16 *)(boss + j * 2 + 0x2A8) = t[*(u16 *)(boss + 0x3D4) + 3];
            *(u16 *)(boss + 0x3D4) += 5;
        }
        m = *(u16 *)(boss + 8);
        switch (m) {
        case 0:
            if (*(u16 *)(boss + 0x364) == 0x78) {
                tt_se_request(0x505);
                for (i = 0; i < 5; i++) {
                    *(s32 *)(boss + 0x3E4 + i * 4) = *(s16 *)(boss + 0x27C + i * 2) << 16;
                }
                *(s32 *)(boss + 0x3F8) = 0xFFFE0000;
                *(s32 *)(boss + 0x3FC) = 0x1000;
                *(s16 *)(boss + 0x3D6) = 0;
                *(s16 *)(boss + 8) = 0x40;
            }
            break;
        case 0x40:
            for (i = 0; i < 5; i++) {
                *(s16 *)(boss + 0x27C + i * 2) = *(s32 *)(boss + 0x3E4 + i * 4) >> 16;
                *(s32 *)(boss + 0x3E4 + i * 4) += *(s32 *)(boss + 0x3F8);
            }
            *(s16 *)(boss + 0x24C) -= 1;
            *(s16 *)(boss + 0x252) += 1;
            if (*(u16 *)(boss + 0x3D6) == 1) {
                *(u16 *)(boss + 0x3D6) = 0;
                *(s16 *)(boss + 0x24E) -= 1;
                *(s16 *)(boss + 0x250) += 1;
            } else {
                *(u16 *)(boss + 0x3D6) = 1;
            }
            *(s32 *)(boss + 0x3F8) += *(s32 *)(boss + 0x3FC);
            if (*(s16 *)(boss + 0x284) >= 0xB1) {
                *(u16 *)(boss + 8) = 0x50;
            }
            break;
        case 0x50:
            break;
        }
        for (i = 0x10; i < 0x13; i++) {
            tt_boss_part_animation_step(tt_boss_death_part_script, i);
        }
        if (*(u16 *)(boss + 0x364) == 0xDC) {
            boss[0x311] = 0;
        }
        if (*(u16 *)(boss + 0x364) == 0xFD) {
            for (i = 0; i != 0x18; i++) {
                boss[0x310 + i] = 0;
            }
            /* FAKE: TT's zero-store chain pair; the reloads move the shared boss[0x364] value from v0 to a0. T-11860 */
            *(u16 *)(boss + 6) = *(u16 *)(boss + 8) = *(u16 *)(boss + 0xA) = 0;
            boss[0xD] = boss[0xE] = boss[0xF] = 0;
            *(s16 *)(boss + 4) = 0xA0;
        }
        break;
    }
    *(u16 *)(boss + 0x364) += 1;
}

void tt_boss1_ropes_follow(void) {
    tt_boss1_rope_update();
}

void tt_boss1_fire_arm_shots(void) {
    u8 *boss = tt_boss;
    u8 *rope;

    if ((u32)*(u16 *)(boss + 0x366) >= 0x18) {
        rope = tt_boss_rope_buf;
        tt_enemy_shot_fire_aimed(*(s16 *)(rope + 0x3A), *(s16 *)(rope + 0xBA), 0x18, 0xA8);
        rope += 0xEFC;
        tt_enemy_shot_fire_aimed(*(s16 *)(rope + 0x3A), *(s16 *)(rope + 0xBA), 0x18, 0xA8);
        *(u16 *)(boss + 0x366) = 0;
        return;
    }
    *(u16 *)(boss + 0x366) += 1;
}

/* Non-prototype declaration for taking the address: the ANSI prototype of 80133970.c (s16/u16 parameters) would change the K&R calls in this file. T-11510 */
extern void tt_enemy_shot_fire_fan9_aimed();

void tt_boss1_phase2_attack(void) {
    void (*fn)();
    u8 *boss;
    s32 i;

    boss = tt_boss;
    if (*(u16 *)(boss + 0x30E) == 1) {
        fn = tt_enemy_shot_fire_3way_aimed;
    } else {
        fn = tt_enemy_shot_fire_fan9_aimed;
    }
    if (*(u16 *)(tt_boss + 0x366) >= 0x30) {
        tt_se_request(0x50B);
        switch (*(u16 *)(boss + 0x3A4)) {
        case 0:
            if (boss[0x52] == 1) {
                fn(*(s16 *)(boss + 0x14), *(s16 *)(boss + 0x16), 0x100, 0x20, 0xAA);
                for (i = 0; i != 5; i++) {
                    tt_enemy_shot_fire_angle(*(s16 *)(boss + 0x14) + tt_boss1_fan_x_0[i], *(s16 *)(boss + 0x16) + tt_boss1_fan_y_0[i], 0x20, tt_boss1_fan_angle_0[i], 0xA9);
                    ((TtPartAnimationView *)(boss + 0x38))[0x10 + i].flags = 0x80;
                    boss[0x320 + i] = 1;
                    ((s16 *)(boss + 0x268))[i] = tt_boss1_fan_x_0[i];
                    ((s16 *)(boss + 0x298))[i] = tt_boss1_fan_y_0[i];
                }
                *(u16 *)(boss + 0x3A4) = 1;
                *(u16 *)(boss + 0x366) = 0;
            }
            break;
        case 1:
            if (boss[0x52] == 3) {
                fn(*(s16 *)(boss + 0x14), *(s16 *)(boss + 0x16), 0x100, 0x20, 0xAA);
                for (i = 0; i != 5; i++) {
                    tt_enemy_shot_fire_angle(*(s16 *)(boss + 0x14) + tt_boss1_fan_x_1[i], *(s16 *)(boss + 0x16) + tt_boss1_fan_y_1[i], 0x20, tt_boss1_fan_angle_1[i], 0xA9);
                    ((TtPartAnimationView *)(boss + 0x38))[0x10 + i].flags = 0x80;
                    boss[0x320 + i] = 1;
                    ((s16 *)(boss + 0x268))[i] = tt_boss1_fan_x_1[i];
                    ((s16 *)(boss + 0x298))[i] = tt_boss1_fan_y_1[i];
                }
                *(u16 *)(boss + 0x3A4) = 2;
                *(u16 *)(boss + 0x366) = 0;
            }
            break;
        case 2:
            if (boss[0x52] == 5) {
                fn(*(s16 *)(boss + 0x14), *(s16 *)(boss + 0x16), 0x100, 0x20, 0xAA);
                for (i = 0; i != 4; i++) {
                    tt_enemy_shot_fire_angle(*(s16 *)(boss + 0x14) + tt_boss1_fan_x_2[i], *(s16 *)(boss + 0x16) + tt_boss1_fan_y_2[i], 0x20, tt_boss1_fan_angle_2[i], 0xA9);
                    ((TtPartAnimationView *)(boss + 0x38))[0x10 + i].flags = 0x80;
                    boss[0x320 + i] = 1;
                    ((s16 *)(boss + 0x268))[i] = tt_boss1_fan_x_2[i];
                    ((s16 *)(boss + 0x298))[i] = tt_boss1_fan_y_2[i];
                }
                *(u16 *)(boss + 0x3A4) = 3;
                *(u16 *)(boss + 0x366) = 0;
            }
            break;
        case 3:
            if (boss[0x52] == 7) {
                fn(*(s16 *)(boss + 0x14), *(s16 *)(boss + 0x16), 0x100, 0x20, 0xAA);
                for (i = 0; i != 5; i++) {
                    tt_enemy_shot_fire_angle(*(s16 *)(boss + 0x14) + tt_boss1_fan_x_3[i], *(s16 *)(boss + 0x16) + tt_boss1_fan_y_3[i], 0x20, tt_boss1_fan_angle_3[i], 0xA9);
                    ((TtPartAnimationView *)(boss + 0x38))[0x10 + i].flags = 0x80;
                    boss[0x320 + i] = 1;
                    ((s16 *)(boss + 0x268))[i] = tt_boss1_fan_x_3[i];
                    ((s16 *)(boss + 0x298))[i] = tt_boss1_fan_y_3[i];
                }
                *(u16 *)(boss + 0x3A4) = 0;
                *(u16 *)(boss + 0x366) = 0;
            }
            break;
        }
    } else {
        *(u16 *)(boss + 0x366) += 1;
    }
    for (i = 0x10; i != 0x15; i++) {
        tt_boss_part_animation_step(tt_boss1_phase2_script, i);
    }
}

/* T-11520 near miss: structure and frame match; 28 differing lines. Spill homes of the promoted a/b pointers are at sp+0x34/0x38 here against 0x38/0x3C in the original (one extra home between them; unused pad locals do not move them), and the hoisted constant 1 takes v0 instead of a2 (case 0x40, 0x52 and 0xA0 stores). The case-0x54 u16 store of 0x80 and the u8 store in case 0xA0 are needed to match the shared li a0,0x80 and sb. */
/* The constant 1 is one hoisted value (a2) over cases 0x40, 0x52, 0x80 and 0xA0 only when every store of it is unsigned (u16/u8), and the case-0x54 u16 store of 0x80 shares the dispatch's li a0,0x80. a and b are declared first for their spill homes at sp+0x3C/0x38. T-11830 */
void tt_boss1_update(void) {
    u8 *sys;
    u8 *core;
    u8 *boss;
    u8 *player;
    s32 i;
    s16 v;
    u16 k;

    boss = tt_boss;
    k = *(u16 *)(boss + 4);
    sys = tt_sys;
    core = tt_core;
    player = tt_player;
    switch (k) {
    case 0:
        tt_field_enemies_destroy(sys, player);
        if (core[0x22] != 0 && sys[0x52] == 0xFF) {
            audio_command(0x71);
        } else if (*(u16 *)(core + 4) == 0) {
            audio_command(0xB1);
            func_800451E0(0x8019A000);
            audio_command(0x12);
            audio_command(0x23);
            audio_command(0x2E);
            audio_command(0x21);
            audio_command(4);
            audio_command(0x200);
        }
        tt_boss1_init();
        *(s16 *)(boss + 4) = 0x20;
        break;
    case 0x20:
        tt_boss1_enter();
        tt_boss1_ropes_follow();
        break;
    case 0x30:
        tt_boss1_intro_animation();
        tt_boss1_ropes_follow();
        break;
    case 0x40:
        v = *(s16 *)(tt_boss + 0x30C);
        if (v < 0x20) {
            *(u16 *)(boss + 6) = 2;
        } else if (v < 0x40) {
            *(u16 *)(boss + 6) = 1;
        } else {
            *(u16 *)(boss + 6) = 0;
        }
        tt_boss1_track_player_x();
        tt_boss1_ropes_follow();
        if (*(u16 *)(boss + 8) == 0) {
            tt_boss1_fire_arm_shots();
        }
        v = *(s16 *)(boss + 0x30C);
        if (v < 0) {
            *(s16 *)(boss + 4) = 0x50;
            *(s16 *)(boss + 10) = 0;
            *(u16 *)(boss + 8) = 0;
            *(s16 *)(boss + 6) = 0;
            boss[15] = 0;
            boss[14] = 0;
            boss[13] = 0;
        } else if (v < 0x20) {
            boss[0x3E] = 0x3A;
        } else if (v < 0x40) {
            boss[0x3E] = 0x36;
        }
        break;
    case 0x50:
        *(s32 *)(boss + 0x218) = 0;
        tt_boss1_rope_break();
        break;
    case 0x51:
        tt_boss1_expand_ring();
        break;
    case 0x52:
        *(s16 *)(boss + 4) = 0x53;
        *(u16 *)(boss + 0x30E) = 1;
        *(s16 *)(boss + 0x3A4) = 0;
        *(s16 *)(boss + 0x3AA) = 0;
        *(u16 *)(boss + 0x3AC) = 1;
        *(s16 *)(boss + 0x366) = 0;
        for (i = 0; i < 4; i++) {
            *(s16 *)(boss + 0x2D8 + i * 2) = 0x18;
            boss[0x353 + i] = 1;
        }
        for (i = 0x10; i != 0x15; i++) {
            ((TtPartAnimationView *)(boss + 0x38))[i].sets = tt_sprite_set_table;
            ((TtPartAnimationView *)(boss + 0x38))[i].flip = 0;
            ((TtPartAnimationView *)(boss + 0x38))[i].set = 6;
            ((TtPartAnimationView *)(boss + 0x38))[i].unk_0A = 0;
            ((TtPartAnimationView *)(boss + 0x38))[i].transform_enable = 0;
            boss[0x310 + i] = 0;
            *(s16 *)(boss + 0x2A8 + i * 2) = 8;
            *(s16 *)(boss + 0x248 + i * 2) = 0;
            *(s16 *)(boss + 0x278 + i * 2) = 0;
            ((TtPartAnimationView *)(boss + 0x38))[i].flags = 0x60;
        }
        break;
    case 0x53:
        tt_boss1_track_player_x();
        tt_boss1_spin_step();
        tt_boss1_core_animation();
        tt_boss1_side_part_animation();
        tt_boss1_phase2_attack();
        if (boss[0x353] == 0 && boss[0x354] == 0 && boss[0x355] == 0 && boss[0x356] == 0) {
            *(s16 *)(boss + 4) = 0x54;
            *(s16 *)(boss + 0x30E) = 2;
            *(s16 *)(boss + 0x30C) = 0x48;
        }
        break;
    case 0x54:
        tt_boss1_track_player_x();
        tt_boss1_spin_step();
        tt_boss1_core_animation();
        tt_boss1_side_part_animation();
        tt_boss1_phase2_attack();
        if (*(s16 *)(boss + 0x30C) < 0) {
            tt_se_request(0x504);
            *(u16 *)(boss + 4) = 0x80;
            *(s16 *)(boss + 0x30E) = 3;
            boss[0xB8] = 0x80;
        }
        break;
    case 0x80:
        audio_command(0xC1);
        player[0x52] = 1;
        tt_boss1_death_sequence();
        tt_boss1_core_animation();
        tt_boss1_side_part_animation();
        break;
    case 0xA0:
        audio_command(0xB1);
        boss[0] = 0;
        core[0x12] = 0;
        *(u16 *)((u8 *)tt_stage_script + 0x1E) = 1;
        *(s16 *)(core + 2) = 0xA0;
        break;
    }
    tt_boss1_collision();
}
