/* swing_ring_boss.c: src/ovl/TT/80135360.c:227: boss collision, ring layout, swinging ring movement, attack and death sequence; body 234: c = tt_boss; 235: switch (*(u16 *)(c + 8)) {. Original object 0x80135360-0x801367C0 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/tt.h"

/* T-11510 near miss (about 45 differing instructions; frame, spill slots 0x60/0x9C, pt and every branch target match): the first clear loop pointer is s1 in the original (v0 here), temp numbering follows from it, and the two angle sums (idx * 0x733 + 0xC000 - 0x3998 and + 0x4000 - 0xACC8) fold their constants here while the original keeps the hoisted 0xC000/0x4000 and adds the constant separately. The unused s32 locals and pad[4] only place the frame (FAKE, original source unknown). */
void tt_boss2_collision(void) {
    s32 pq1;
    s32 pq2;
    u8 *player;
    u8 *boss;
    u8 *core;
    u8 *shot;
    s32 i;
    s32 j;
    s32 px1;
    s32 px2;
    s32 px3;
    s32 px4;
    s16 pt[2];
    s32 pad[3]; /* FAKE: pq1..px4 and pad are unused; they place the frame, spill slots and pt as in the original. T-11510, T-11830 */

    boss = tt_boss;
    player = tt_player;
    core = tt_core;
    if (boss[0] != 0) {
        for (i = 0; i < 0x18; i++) {
            boss[i * 0x14 + 0x42] = 0;
        }
        if (boss[0x342] == 0 && tt_boss_check_hits() != 0) {
            tt_se_request(TT_SE_BOSS_HIT_B);
        }
        if (boss[0x344] == 0) {
            for (i = 0; i != 8; i++) {
                pt[0] = *(s16 *)(boss + 0x14) + *(s16 *)(boss + 0x258 + i * 2);
                pt[1] = *(s16 *)(boss + 0x16) + *(s16 *)(boss + 0x288 + i * 2);
                if (player[0x52] == 0 && player[0] != 0 && tt_box_overlap(player + 0x14, tt_hitboxes, (u8 *)pt, &tt_boss2_hitbox_core[*(boss + 0xDE + i * 0x14) * 6] + 0x42) != 0) {
                    tt_player_hit();
                    *(s16 *)(player + 4) = 0x60;
                    *(s16 *)(player + 10) = 0;
                    *(s16 *)(player + 8) = 0;
                    *(s16 *)(player + 6) = 0;
                    player[15] = 0;
                    player[14] = 0;
                    player[13] = 0;
                }
                shot = tt_shots;
                for (j = 0; j != 12; j++, shot += 0x54) {
                    if (shot[0] != 0 && *(u16 *)(shot + 4) == 0x40) {
                    if (tt_box_overlap(shot + 0x14, &tt_hitbox_shot, (u8 *)pt, &tt_boss2_hitbox_core[*(boss + 0xDE + i * 0x14) * 6] + 0x42) != 0) {
                        core[0x25] = core[0x25] + 1;
                        if (core[0x24] != 0 && !(core[0x25] & 3)) {
                            tt_enemy_shot_fire_aimed(*(s16 *)(shot + 0x14), *(s16 *)(shot + 0x16), 0x20, 0xA0);
                        }
                        *(u16 *)(shot + 4) = 0x80;
                        *(s16 *)(shot + 10) = 0;
                        *(s16 *)(shot + 8) = 0;
                        *(s16 *)(shot + 6) = 0;
                        shot[15] = 0;
                        shot[14] = 0;
                        shot[13] = 0;
                        if (boss[0x343] != 0) {
                            if (*(u16 *)(boss + 6) == 1) {
                                *(u16 *)(boss + 0x3B8) += 1;
                                if ((*(u16 *)(boss + 0x3B8) & 7) == 7) {
                                    tt_enemy_shot_fire_angle(pt[0], pt[1], 0x18, 0xC00 - (*(boss + 0xDE + i * 0x14) << 7), 0xA0);
                                }
                            }
                            if (*(boss + 0xDE + i * 0x14) != 0 && *(boss + 0xDE + i * 0x14) < 0x10) {
                                *(u16 *)(boss + 0x3AA) = 0xC000;
                                if (*(boss + 0xDE + i * 0x14) >= 9) {
                                    *(u16 *)(boss + 0x3AA) += *(boss + 0xDE + i * 0x14) * 0x733 - 0x3998;
                                }
                            } else if (*(boss + 0xDE + i * 0x14) >= 0x11) {
                                *(u16 *)(boss + 0x3AA) = 0x4000;
                                if (*(boss + 0xDE + i * 0x14) < 0x18) {
                                    *(u16 *)(boss + 0x3AA) += *(boss + 0xDE + i * 0x14) * 0x733 - 0xACC8;
                                }
                            }
                        }
                    }
                    }
                }
            }
        }
    }
}

/* -(ang + 0x400) keeps -0x400 out of the hoisted constants, so 85 takes $fp and divides with break checks. T-11830 */
void tt_boss2_ring_layout(x, y, ang)
    s16 x;
    s16 y;
    s32 ang;
{
    u8 *boss;
    s32 i;
    s32 dir;
    s32 s;

    boss = tt_boss;
    ang &= 0xFFF;
    for (i = 8; i < 16; i++) {
        dir = ((ang + 0x40) & 0xFFF) / 128;
        boss[i * 0x14 + 0x3E] = dir;
        s = -(ang + 0x400);
        *(s16 *)(boss + 0x248 + i * 2) = func_800A0140(s) / 85 + x;
        *(s16 *)(boss + 0x278 + i * 2) = y - 0x18;
        if (dir >= 8 && dir < 25) {
            *(s16 *)(boss + 0x278 + i * 2) += rsin(s) / 128;
            *(s16 *)(boss + 0x2A8 + i * 2) = 7;
        } else {
            *(s16 *)(boss + 0x278 + i * 2) += rsin(s) / 171;
            *(s16 *)(boss + 0x2A8 + i * 2) = 9;
        }
        ang += 0x200;
    }
}

void tt_boss2_set_face_direction(s32 angle) {
    tt_boss[0x3E] = ((angle + 0x40) & 0xFFF) / 128;
}

void tt_boss2_init(void) {
    u8 *boss;
    s32 i;

    boss = tt_boss;
    tt_boss_parts_hide_all();
    tt_boss_hitboxes_clear();
    boss[0] = 1;
    boss[0x342] = 0;
    *(s32 *)(boss + 0x20) = 0;
    *(s32 *)(boss + 0x24) = 0xFFD00000;
    *(s16 *)(boss + 0x14) = 0;
    *(s16 *)(boss + 0x16) = -0x30;
    *(s32 *)(boss + 0x28) = 0;
    *(s32 *)(boss + 0x2C) = 0x20000;
    for (i = 0; i < 6; i++) {
        ((TtPartAnimation *)(boss + 0x38))[i].sets = tt_sprite_set_table;
        ((TtPartAnimation *)(boss + 0x38))[i].flip = 0;
        ((TtPartAnimation *)(boss + 0x38))[i].set = tt_boss2_part_set[i];
        ((TtPartAnimation *)(boss + 0x38))[i].frame = tt_boss2_part_frame[i];
        ((TtPartAnimation *)(boss + 0x38))[i].flags = 0x60;
        ((TtPartAnimation *)(boss + 0x38))[i].clut = 0xF;
        ((TtPartAnimation *)(boss + 0x38))[i].transform_enable = 0;
        boss[i + 0x310] = 1;
        ((s16 *)(boss + 0x2A8))[i] = 8;
        ((s16 *)(boss + 0x248))[i] = 0;
        ((s16 *)(boss + 0x278))[i] = 0;
    }
    for (i = 8; i < 0x10; i++) {
        ((TtPartAnimation *)(boss + 0x38))[i].sets = tt_sprite_set_table;
        ((TtPartAnimation *)(boss + 0x38))[i].flip = 0;
        ((TtPartAnimation *)(boss + 0x38))[i].set = 4;
        ((TtPartAnimation *)(boss + 0x38))[i].frame = 0;
        ((TtPartAnimation *)(boss + 0x38))[i].flags = 0x60;
        ((TtPartAnimation *)(boss + 0x38))[i].clut = 0xF;
        boss[i + 0x310] = 1;
        ((s16 *)(boss + 0x2A8))[i] = 7;
        ((s16 *)(boss + 0x248))[i] = 0;
        ((s16 *)(boss + 0x278))[i] = 0;
    }
    tt_boss2_ring_layout(0, 0, 0);
    *(u8 **)(boss + 0x228) = tt_boss2_hitbox_slot_228;
    *(u8 **)(boss + 0x22C) = tt_boss2_hitbox_slot_22c;
    *(u8 **)(boss + 0x238) = tt_boss2_hitbox_slot_238;
    *(u8 **)(boss + 0x230) = tt_boss2_hitbox_slot_230;
    *(s16 *)(boss + 0x30C) = 0x60;
    *(s16 *)(boss + 0x30E) = 0;
    boss[0x91] = 0;
    boss[0xA5] = 0x10;
    *(s16 *)(boss + 0x3A4) = 0;
    *(s16 *)(boss + 0x3A6) = 0;
    *(s16 *)(boss + 0x3A8) = 0;
    *(u16 *)(boss + 0x3AA) = 0xF800;
    *(s16 *)(boss + 0x3AC) = 0;
    *(s16 *)(boss + 0x3AE) = 0x400;
    *(s16 *)(boss + 0x3B0) = 0;
    *(s16 *)(boss + 0x3B2) = 0;
    *(s16 *)(boss + 0x3B4) = 0;
    *(s16 *)(boss + 0x3B6) = 0;
    *(s16 *)(boss + 0x3B8) = 0;
    *(s16 *)(boss + 0x364) = 0;
    boss[0x343] = 0;
    boss[0x344] = 0;
}

void tt_boss2_enter(void) {
    u8 *boss = tt_boss;

    *(s32 *)(boss + 0x20) = 0;
    *(s32 *)(boss + 0x24) += 0x10000;
    *(s16 *)(boss + 0x14) = *(s16 *)(boss + 0x22);
    *(s16 *)(boss + 0x16) = *(s16 *)(boss + 0x26);
    if (*(s16 *)(boss + 0x16) >= 0x50) {
        *(s32 *)(boss + 0x24) = 0x500000;
        *(s16 *)(boss + 0x16) = *(s16 *)(boss + 0x26);
        *(s16 *)(boss + 4) = 0x30;
        *(s16 *)(boss + 0xA) = 0;
        *(s16 *)(boss + 8) = 0;
        *(s16 *)(boss + 6) = 0;
        boss[0xF] = 0;
        boss[0xE] = 0;
        boss[0xD] = 0;
    }
}

void tt_boss2_intro_animation(void) {
    u8 *boss = tt_boss;

    switch (*(u16 *)(boss + 8)) {
    case 0:
        boss[0x54] = 0x80;
        *(u16 *)(boss + 8) = 0x40;
        break;
    case 0x40:
        tt_boss_part_animation_step(tt_boss2_intro_script, 1);
        boss[0x54] &= 1;
        if (boss[0x54] != 0) {
            *(u8 **)(boss + 0x218) = tt_boss2_hitbox_core;
            boss[0x343] = 1;
            *(u16 *)(boss + 8) = 0;
            *(u16 *)(boss + 4) = 0x40;
        }
        break;
    }
}

void tt_boss2_ring_swing(flag)
    u8 flag;
{
    u8 *boss;
    s32 i;
    s32 v;

    boss = tt_boss;
    switch (*(u16 *)(boss + 8)) {
    case 0:
        boss[0x342] = 1;
        boss[0x343] = 0;
        *(u16 *)(boss + 0x3B0) = 0;
        for (i = 0; i < 6; i++) {
            boss[i * 0x14 + 0x48] = 1;
            *(s16 *)(boss + i * 0x14 + 0x44) = 0x1000;
            *(s16 *)(boss + i * 0x14 + 0x46) = 0x1000;
        }
        *(u16 *)(boss + 8) = 1;
        break;
    case 1:
        v = 0x1000 - rsin(*(u16 *)(boss + 0x3B0)) / 2;
        for (i = 0; i < 6; i++) {
            *(s16 *)(boss + i * 0x14 + 0x46) = v;
            *(s16 *)(boss + i * 0x14 + 0x44) = v;
        }
        *(u16 *)(boss + 0x3B0) += 0x10;
        if (*(u16 *)(boss + 0x3B0) >= 0x400) {
            tt_se_request(0x511);
            *(u16 *)(boss + 0x3B0) = 0x400;
            if (flag != 0) {
                *(u16 *)(boss + 0x3B2) = 0x1C00;
            } else {
                *(u16 *)(boss + 0x3B2) = 0xC00;
            }
            for (i = 0; i < 6; i++) {
                *(s16 *)(boss + 0x2A8 + i * 2) = 10;
            }
            *(u16 *)(boss + 8) = 2;
        }
        break;
    case 2:
        *(s16 *)(boss + 0x3B4) = func_800A0140(*(u16 *)(boss + 0x3B2)) / 40;
        *(s16 *)(boss + 0x3B6) = rsin(*(u16 *)(boss + 0x3B2)) / 51 + 0x50;
        if (flag != 0) {
            *(u16 *)(boss + 0x3B2) -= 0x20;
            if (*(u16 *)(boss + 0x3B2) <= 0xC00) {
                *(u16 *)(boss + 0x3B2) = 0xC00;
                *(s16 *)(boss + 0x3B4) = 0;
                *(s16 *)(boss + 0x3B6) = 0;
                *(u16 *)(boss + 8) = 3;
            }
        } else {
            *(u16 *)(boss + 0x3B2) += 0x20;
            if (*(u16 *)(boss + 0x3B2) >= 0x1C00) {
                *(u16 *)(boss + 0x3B2) = 0x1C00;
                *(s16 *)(boss + 0x3B4) = 0;
                *(s16 *)(boss + 0x3B6) = 0;
                *(u16 *)(boss + 8) = 3;
            }
        }
        break;
    case 3:
        v = 0x1000 - rsin(*(u16 *)(boss + 0x3B0)) / 2;
        for (i = 0; i < 6; i++) {
            *(s16 *)(boss + i * 0x14 + 0x46) = v;
            *(s16 *)(boss + i * 0x14 + 0x44) = v;
        }
        *(u16 *)(boss + 0x3B0) += 0x20;
        if (*(u16 *)(boss + 0x3B0) >= 0x800) {
            *(u16 *)(boss + 0x3B0) = 0x800;
            *(u16 *)(boss + 0x3B2) = 0;
            for (i = 0; i < 6; i++) {
                *(s16 *)(boss + 0x2A8 + i * 2) = 8;
            }
            *(u16 *)(boss + 8) = 4;
        }
        break;
    case 4:
        for (i = 0; i < 6; i++) {
            boss[i * 0x14 + 0x48] = 0;
        }
        boss[0x342] = 0;
        boss[0x343] = 1;
        *(u16 *)(boss + 0x3AE) += 8;
        *(u16 *)(boss + 0x3B0) = 0;
        *(s16 *)(boss + 0x364) = 0;
        *(u16 *)(boss + 8) = 0;
        break;
    }
}

void tt_boss2_move(void) {
    u8 *boss;

    boss = tt_boss;
    *(s32 *)(boss + 0x20) = func_800A0140(*(u16 *)(tt_boss + 0x3AE)) * 0x600;
    *(s32 *)(boss + 0x24) = (rsin(*(u16 *)(boss + 0x3AE) * 2) << 9) + 0x500000;
    *(s16 *)(boss + 0x14) = *(s16 *)(boss + 0x22);
    *(s16 *)(boss + 0x16) = *(s16 *)(boss + 0x26);
    if (*(u16 *)(boss + 6) == 2) {
        if (*(u16 *)(boss + 0x3AE) == 0x400 || *(u16 *)(boss + 0x3AE) == 0x1400) {
            tt_boss2_ring_swing(1);
            return;
        }
        if (*(u16 *)(boss + 0x3AE) == 0xC00 || *(u16 *)(boss + 0x3AE) == 0x1C00) {
            tt_boss2_ring_swing(0);
            return;
        }
        *(u16 *)(boss + 0x3AE) += 8;
        if (*(u16 *)(boss + 0x3AE) >= 0x2001U) {
            *(u16 *)(boss + 0x3AE) -= 0x2000;
        }
    } else {
        *(u16 *)(boss + 0x3AE) += 8;
        if (*(u16 *)(boss + 0x3AE) >= 0x2001U) {
            *(u16 *)(boss + 0x3AE) -= 0x2000;
        }
    }
}

void tt_boss2_death_sequence(void) {
    u8 *boss;
    s32 i;
    s16 *t;
    s16 j;
    u8 *e;
    u16 k;

    k = *(u16 *)(tt_boss + 8);
    boss = tt_boss;
    switch (k) {
    case 0:
        tt_se_request(TT_SE_BOSS_EXPLOSION);
        boss[0x344] = 1;
        tt_boss_hitboxes_clear();
        *(u16 *)(boss + 0x364) = 0;
        *(u16 *)(boss + 0x3D4) = 0;
        for (i = 8; i < 0x10; i++) {
            ((TtPartAnimationView *)(boss + 0x38))[i].flags = 0x80;
        }
        for (i = 0x10; i != 0x13; i++) {
            ((TtPartAnimationView *)(boss + 0x38))[i].sets = tt_sprite_set_table;
            ((TtPartAnimationView *)(boss + 0x38))[i].flip = 0;
            ((TtPartAnimationView *)(boss + 0x38))[i].set = 4;
            ((TtPartAnimationView *)(boss + 0x38))[i].unk_0A = 0;
            ((TtPartAnimationView *)(boss + 0x38))[i].transform_enable = 0;
            boss[0x310 + i] = 0;
            ((s16 *)(boss + 0x2A8))[i] = 7;
            ((s16 *)(boss + 0x248))[i] = 0;
            ((s16 *)(boss + 0x278))[i] = 0;
        }
        *(u16 *)(boss + 8) = 0x40;
        break;
    case 0x40:
        t = tt_boss2_death_timeline;
        e = tt_boss;
        if (t[*(u16 *)(e + 0x3D4)] != -1 && t[*(u16 *)(e + 0x3D4)] == *(u16 *)(e + 0x364)) {
            tt_se_request(TT_SE_BOSS_EXPLOSION);
            j = t[*(u16 *)(boss + 0x3D4) + 4];
            boss[j + 0x310] = 1;
            boss[j * 0x14 + 0x40] = 0x80;
            *(s16 *)(boss + j * 2 + 0x248) = t[*(u16 *)(boss + 0x3D4) + 1];
            *(s16 *)(boss + j * 2 + 0x278) = t[*(u16 *)(boss + 0x3D4) + 2];
            *(s16 *)(boss + j * 2 + 0x2A8) = t[*(u16 *)(boss + 0x3D4) + 3];
            *(u16 *)(boss + 0x3D4) += 5;
        }
        for (i = 8; i < 0x10; i++) {
            tt_boss_part_animation_step(tt_boss2_orbit_script, i);
        }
        for (i = 0x10; i < 0x13; i++) {
            tt_boss_part_animation_step(tt_boss_death_part_script, i);
        }
        if (*(u16 *)(boss + 0x364) == 0x85) {
            for (i = 0; i != 0x10; i++) {
                boss[0x310 + i] = 0;
            }
        }
        if (*(u16 *)(boss + 0x364) == 0x9C) {
            for (i = 0; i != 0x18; i++) {
                boss[0x310 + i] = 0;
            }
            *(s16 *)(boss + 8) = 0;
            *(s16 *)(boss + 4) = 0xA0;
        }
        break;
    }
    *(u16 *)(boss + 0x364) += 1;
}

void tt_boss2_ring_update(void) {
    s32 pad; /* FAKE: unused home declared first keeps the frame layout, real source unknown. T-11500 */
    u8 *boss;
    u8 *e;
    u8 k;
    s16 v;
    u16 t;
    s16 b;

    k = tt_boss[0x3E];
    boss = tt_boss;
    switch (k) {
    case 0:
        *(u8 **)(boss + 0x230) = 0;
        break;
    case 1:
        *(u8 **)(boss + 0x230) = tt_boss2_hitbox_face1;
        break;
    case 2:
        *(u8 **)(boss + 0x230) = tt_boss2_hitbox_face2;
        break;
    case 3:
        *(u8 **)(boss + 0x230) = tt_boss2_hitbox_face3;
        break;
    case 0x1F:
        *(u8 **)(boss + 0x230) = tt_boss2_hitbox_face31;
        break;
    case 0x1E:
        *(u8 **)(boss + 0x230) = tt_boss2_hitbox_face30;
        break;
    case 0x1D:
        *(u8 **)(boss + 0x230) = tt_boss2_hitbox_face29;
        break;
    default:
        *(u8 **)(boss + 0x230) = tt_boss2_hitbox_slot_230;
        break;
    }
    tt_boss_part_animation_step(tt_boss2_part4_script, 4);
    tt_boss_part_animation_step(tt_boss2_part5_script, 5);
    switch (*(u16 *)(boss + 6)) {
    case 0:
        tt_sprite_set_3_clut = 0x3C10;
        tt_sprite_set_4_clut = 0x3C10;
        break;
    case 1:
        tt_sprite_set_3_clut = 0x3C50;
        tt_sprite_set_4_clut = 0x3C50;
        break;
    case 2:
        tt_sprite_set_3_clut = 0x3C90;
        tt_sprite_set_4_clut = 0x3C90;
        break;
    }
    tt_boss2_ring_layout(*(s16 *)(boss + 0x3B4), *(s16 *)(boss + 0x3B6), *(u16 *)(boss + 0x3A8));
    v = *(u16 *)(boss + 0x3AA);
    *(u16 *)(boss + 0x3A8) += v / 256;
    if (v < 0) {
        if (v < -0x800) {
            *(u16 *)(boss + 0x3AA) = *(u16 *)(boss + 0x3AA) + 0x80;
        } else {
            *(u16 *)(boss + 0x3AA) = 0xF800;
        }
    } else if (v >= 0x801) {
        *(u16 *)(boss + 0x3AA) = *(u16 *)(boss + 0x3AA) - 0x80;
    } else {
        *(u16 *)(boss + 0x3AA) = 0x800;
    }
    v = *(u16 *)(boss + 0x3AA);
    if (((v < 0) ? -v : v) >= 0x801) {
        *(u16 *)(boss + 0x3AC) -= v / 256 / 4;
        t = *(u16 *)(boss + 0x3AC);
        b = t;
        if (((b < 0) ? -b : b) < 2) {
            *(u16 *)(boss + 0x3AC) = 0;
        } else if (b < 0x800) {
            *(u16 *)(boss + 0x3AC) = t - 2;
        } else {
            *(u16 *)(boss + 0x3AC) = t + 2;
        }
        *(u16 *)(boss + 0x3AC) = *(u16 *)(boss + 0x3AC) & 0xFFF;
        tt_boss2_set_face_direction(*(u16 *)(boss + 0x3AC));
    }
}

void tt_boss2_fire(void) {
    u8 *boss;

    boss = tt_boss;
    switch (*(u16 *)(boss + 6)) {
    case 0:
    case 1:
        if (*(u16 *)(tt_boss + 0x364) >= 0x60U) {
            tt_enemy_shot_fire_aimed(*(s16 *)(boss + 0x14), *(s16 *)(boss + 0x16), 0x18, 0xA0);
        }
        break;
    case 2:
        if (*(u16 *)(tt_boss + 0x364) >= 0x60U) {
            tt_enemy_shot_fire_3way_aimed(*(s16 *)(boss + 0x14), *(s16 *)(boss + 0x16), 0x100, 0x18, 0xA0);
        }
        break;
    }
    if (*(u16 *)(boss + 0x364) >= 0x60U) {
        *(u16 *)(boss + 0x364) = 0;
        return;
    }
    *(u16 *)(boss + 0x364) += 1;
}

void tt_boss2_update(s32 arg0) {
    u8 *core;
    s16 v;
    u8 *player;
    u8 *boss;

    boss = tt_boss;
    core = tt_core;
    player = tt_player;
    switch (*(u16 *)(boss + 4)) {
    case 0x0:
        tt_field_enemies_destroy(arg0, player);
        if (*(u16 *)(core + 4) == 0) {
            audio_command(0xB1);
            cd_load_destination_base_set(0x8019A000);
            audio_command(0x12);
            audio_command(0x23);
            audio_command(0x2E);
            audio_command(0x21);
            audio_command(4);
            audio_command(0x200);
        }
        tt_boss2_init();
        *(s16 *)(boss + 4) = 0x20;
        break;
    case 0x20:
        tt_boss2_enter();
        tt_boss2_ring_update();
        break;
    case 0x30:
        tt_boss2_intro_animation();
        tt_boss2_ring_update();
        break;
    case 0x40:
        v = *(s16 *)(tt_boss + 0x30C);
        if (v < 0x20) {
            *(s16 *)(boss + 6) = 2;
        } else if (v < 0x40) {
            *(s16 *)(boss + 6) = 1;
        } else {
            *(s16 *)(boss + 6) = 0;
        }
        tt_boss2_move();
        tt_boss2_ring_update();
        if (*(u16 *)(boss + 8) == 0) {
            tt_boss2_fire();
        }
        if (*(s16 *)(boss + 0x30C) < 0) {
            *(s16 *)(boss + 4) = 0x80;
            *(s16 *)(boss + 0xA) = 0;
            *(u16 *)(boss + 8) = 0;
            *(s16 *)(boss + 6) = 0;
            boss[0xF] = 0;
            boss[0xE] = 0;
            boss[0xD] = 0;
        }
        break;
    case 0x80:
        audio_command(0xC1);
        player[0x52] = 1;
        tt_boss2_death_sequence();
        break;
    case 0xA0:
        boss[0] = 0;
        core[0x12] = 0;
        *(s16 *)((u8 *)tt_stage_script + 0x1E) = 1;
        *(u16 *)(core + 2) = 0xA0;
        break;
    }
    tt_boss2_collision();
}
