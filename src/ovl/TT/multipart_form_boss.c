/* multipart_form_boss.c: src/ovl/TT/8013A3E0.c:318: independent part collision/pose handlers, alternate movement forms, attacks and part-death animation; body 323: switch (*(u16 *)(p + 6)) {; 328: if (*(s32 *)(p + 0x3E4) >= 0x2000) {. Original object 0x8013A3E0-0x8013BD10 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/TT.h"

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x1C];
} TtXY; /* FAKE: pad4 and fake_pad below reproduce the original's frame (locals 0x30..0x54); real locals unknown. T-8060 */

void tt_boss3_part_hit(void *arg0, u8 *arg1, s32 arg2) {
    u8 *player;
    u8 *core;
    u8 *boss;
    u8 *shot;
    s32 i;
    s32 j;

    player = tt_player;
    core = tt_core;
    boss = tt_boss;
    if (player[0x52] == 0 && player[0] != 0 && tt_box_overlap(player + 0x14, tt_hitboxes, arg0, arg1) != 0) {
        tt_player_hit();
        *(u16 *)(player + 4) = 0x60;
        *(s16 *)(player + 10) = 0;
        *(s16 *)(player + 8) = 0;
        *(s16 *)(player + 6) = 0;
        player[15] = 0;
        player[14] = 0;
        player[13] = 0;
    }
    shot = tt_shots;
    for (i = 0; i != 12; i++, shot += 0x54) {
        if (shot[0] == 0) {
            continue;
        }
        if (*(u16 *)(shot + 4) == 0x40 && tt_box_overlap(shot + 0x14, &tt_hitbox_shot, arg0, arg1) != 0) {
            core[0x25] = core[0x25] + 1;
            if (core[0x24] != 0 && !(core[0x25] & 3)) {
                tt_enemy_shot_fire_aimed(*(s16 *)(shot + 0x14), *(s16 *)(shot + 0x16), 0x20, 0xA0);
            }
            tt_se_request(0x508);
            *(u16 *)(shot + 4) = 0x80;
            *(s16 *)(shot + 10) = 0;
            *(s16 *)(shot + 8) = 0;
            *(s16 *)(shot + 6) = 0;
            shot[15] = 0;
            shot[14] = 0;
            shot[13] = 0;
            if (arg2 >= 0) {
                if (i >= 6) {
                    *(s16 *)(boss + 0x2D8 + arg2 * 2) -= 1;
                } else if (*(u16 *)(shot + 2) == 9) {
                    *(s16 *)(boss + 0x2D8 + arg2 * 2) -= 4;
                } else {
                    *(s16 *)(boss + 0x2D8 + arg2 * 2) -= 2;
                }
                if (arg2 >= 2) {
                    j = arg2 * 2 + 2;
                } else {
                    j = arg2 * 2;
                }
                boss[j * 0x14 + 0x42] = 1;
                if (*(s16 *)(boss + 0x2D8 + arg2 * 2) < 0) {
                    tt_se_request(0x50A);
                    boss[arg2 + 0x343] = 1;
                }
            }
        }
    }
}

void tt_boss3_part0_collision(void) {
    TtBossDrawView *boss;
    u8 unused; /* FAKE: preserve the pointer and position homes. T-10010. */
    TtXY pos;
    s32 fake_pad; /* FAKE: preserve the frame-index home at +0x30. T-10010. */
    u32 frame;
    u32 kind;
    boss = (TtBossDrawView *)tt_boss;
    kind = boss->unk_3E;
    frame = boss->unk_3A4[0] % 3U;
    if (boss->unk_3C == 2 && boss->unk_3E != 3) {
        kind += 3;
    }
    pos.x = boss->x + boss->xs[0];
    pos.y = boss->y + boss->ys[0];
    tt_boss3_part_hit(&pos, kind * 6 + tt_boss3_hitboxes, 0);
    pos.x = boss->x + boss->xs[1];
    pos.y = boss->y + boss->ys[1];
    tt_boss3_part_hit(&pos, frame * 6 + 0x54 + tt_boss3_hitboxes, -1);
}

void tt_boss3_part1_collision(void) {
    TtBossDrawView *boss;
    u8 unused; /* FAKE: preserve the pointer and position homes. T-10010. */
    TtXY pos;
    s32 fake_pad; /* FAKE: preserve the frame-index home at +0x30. T-10010. */
    u32 frame;
    u32 kind;
    boss = (TtBossDrawView *)tt_boss;
    kind = boss->unk_66;
    frame = boss->unk_3A4[1] % 3U;
    if (boss->unk_64 == 2 && boss->unk_66 != 3) {
        kind += 3;
    }
    pos.x = boss->x + boss->xs[2];
    pos.y = boss->y + boss->ys[2];
    tt_boss3_part_hit(&pos, kind * 6 + tt_boss3_hitboxes, 1);
    pos.x = boss->x + boss->xs[3];
    pos.y = boss->y + boss->ys[3];
    tt_boss3_part_hit(&pos, frame * 6 + 0x42 + tt_boss3_hitboxes, -1);
}

void tt_boss3_part2_collision(void) {
    u8 *boss;
    u16 idx;
    TtXY pos;
    s32 fake_pad[2];

    boss = tt_boss;
    idx = *(u16 *)(boss + 0x3A8);
    pos.x = *(s16 *)(boss + 0x14) + *(s16 *)(boss + 0x252);
    pos.y = *(s16 *)(boss + 0x16) + *(s16 *)(boss + 0x282);
    tt_boss3_part_hit(&pos, idx * 6 + 0x78 + tt_boss3_hitboxes, -1);
    pos.x = *(s16 *)(boss + 0x14) + *(s16 *)(boss + 0x254);
    pos.y = *(s16 *)(boss + 0x16) + *(s16 *)(boss + 0x284);
    tt_boss3_part_hit(&pos, &tt_boss3_tip_hitboxes, 2);
}

void tt_boss3_part3_collision(void) {
    u8 *boss;
    u16 idx;
    TtXY pos;
    s32 fake_pad[2];

    boss = tt_boss;
    idx = *(u16 *)(boss + 0x3AA);
    pos.x = *(s16 *)(boss + 0x14) + *(s16 *)(boss + 0x256);
    pos.y = *(s16 *)(boss + 0x16) + *(s16 *)(boss + 0x286);
    tt_boss3_part_hit(&pos, idx * 6 + 0x66 + tt_boss3_hitboxes, -1);
    pos.x = *(s16 *)(boss + 0x14) + *(s16 *)(boss + 0x258);
    pos.y = *(s16 *)(boss + 0x16) + *(s16 *)(boss + 0x288);
    tt_boss3_part_hit(&pos, &tt_boss3_tip_hitboxes, 3);
}

void tt_boss3_collision(void) {
    u8 *boss;
    s32 i;

    boss = tt_boss;
    if (boss[0] != 0) {
        for (i = 0; i < 0x18; i++) {
            boss[0x42 + i * 0x14] = 0;
        }
        if (*(s16 *)(boss + 0x2D8) >= 0) {
            tt_boss3_part0_collision();
        }
        if (*(s16 *)(boss + 0x2DA) >= 0) {
            tt_boss3_part1_collision();
        }
        if (*(s16 *)(boss + 0x2DC) >= 0) {
            tt_boss3_part2_collision();
        }
        if (*(s16 *)(boss + 0x2DE) >= 0) {
            tt_boss3_part3_collision();
        }
        if (tt_boss_check_hits() != 0) {
            tt_se_request(0x508);
        }
    }
}

void tt_boss3_part0_pose(void) {
    u8 *boss = tt_boss;
    s32 i = *(u16 *)(boss + 0x3A4);
    s32 r = i % 3;

    boss[0x3C] = tt_boss3_part0_sprite[i];
    boss[0x3E] = tt_boss3_part0_frame[i];
    *(s16 *)(boss + 0x248) = tt_boss3_part0_x[r];
    *(s16 *)(boss + 0x278) = tt_boss3_part0_y[r];
    boss[0x50] = 2;
    boss[0x52] = tt_boss3_part0_palette[r];
    *(s16 *)(boss + 0x24A) = 0x15;
    *(s16 *)(boss + 0x27A) = 9;
}

void tt_boss3_part1_pose(void) {
    u8 *boss = tt_boss;
    s32 i = *(u16 *)(boss + 0x3A6);
    s32 r = i % 3;

    boss[0x64] = tt_boss3_part1_sprite[i];
    boss[0x66] = tt_boss3_part1_frame[i];
    *(s16 *)(boss + 0x24C) = tt_boss3_part1_x[r];
    *(s16 *)(boss + 0x27C) = tt_boss3_part1_y[r];
    boss[0x78] = 0;
    boss[0x7A] = tt_boss3_part1_palette[r];
    *(s16 *)(boss + 0x24E) = -0x15;
    *(s16 *)(boss + 0x27E) = 9;
}

void tt_boss3_part2_pose(void) {
    u8 *boss = tt_boss;
    u16 i = *(u16 *)(boss + 0x3A8);

    boss[0xA0] = 2;
    boss[0xA2] = tt_boss3_part2_palette[i];
    *(s16 *)(boss + 0x252) = 0x1A;
    *(s16 *)(boss + 0x282) = 0x24;
    boss[0xB4] = 2;
    boss[0xB6] = 0xE;
    *(s16 *)(boss + 0x254) = 0x26;
    *(s16 *)(boss + 0x284) = tt_boss3_tip2_y[i];
}

void tt_boss3_part3_pose(void) {
    u8 *boss = tt_boss;
    u16 i = *(u16 *)(boss + 0x3AA);

    boss[0xC8] = 0;
    boss[0xCA] = tt_boss3_part3_palette[i];
    *(s16 *)(boss + 0x256) = -0x1A;
    *(s16 *)(boss + 0x286) = 0x24;
    boss[0xDC] = 0;
    boss[0xDE] = 0xE;
    *(s16 *)(boss + 0x258) = -0x26;
    *(s16 *)(boss + 0x288) = tt_boss3_tip3_y[i];
}

void tt_boss3_init(void) {
    u8 *boss;
    s32 i;

    boss = tt_boss;
    tt_boss_parts_hide_all();
    tt_boss_hitboxes_clear();
    boss[0] = 1;
    boss[0x342] = 0;
    *(s32 *)(boss + 0x20) = 0;
    *(s32 *)(boss + 0x24) = 0xFFB00000;
    *(s16 *)(boss + 0x14) = 0;
    *(s16 *)(boss + 0x16) = -0x50;
    for (i = 0; i < 10; i++) {
        *(u8 **)(boss + i * 0x14 + 0x38) = tt_sprite_set_table;
        boss[i * 0x14 + 0x3C] = 0;
        boss[i * 0x14 + 0x3D] = 9;
        boss[i * 0x14 + 0x40] = 0x60;
        boss[i * 0x14 + 0x43] = 0xF;
        boss[i * 0x14 + 0x48] = 0;
        boss[i + 0x310] = 1;
        *(u16 *)(boss + i * 2 + 0x2A8) = 9;
    }
    for (i = 2; i < 24; i++) {
        boss[0x310 + i] = 0;
    }
    boss[0x312] = 1;
    boss[0x313] = 1;
    boss[0x314] = 1;
    boss[0x315] = 1;
    boss[0x316] = 1;
    boss[0x317] = 1;
    boss[0x318] = 1;
    boss[0x8C] = 0;
    boss[0x8E] = 0;
    *(s16 *)(boss + 0x250) = 0;
    *(s16 *)(boss + 0x280) = 0;
    *(s16 *)(boss + 0x30C) = 0x80;
    *(s16 *)(boss + 0x30E) = 0;
    *(s16 *)(boss + 0x2D8) = 0x60;
    *(s16 *)(boss + 0x2DA) = 0x60;
    *(s16 *)(boss + 0x2DC) = 0x60;
    *(s16 *)(boss + 0x2DE) = 0x60;
    *(s16 *)(boss + 0x3A4) = 0xB;
    *(s16 *)(boss + 0x3A6) = 0xB;
    *(s16 *)(boss + 0x3A8) = 2;
    *(s16 *)(boss + 0x3AA) = 2;
    *(s16 *)(boss + 0x3AC) = 0;
    *(s32 *)(boss + 0x3E4) = 0;
    boss[0x343] = 0;
    boss[0x344] = 0;
    boss[0x345] = 0;
    boss[0x346] = 0;
    *(s16 *)(boss + 0x364) = 0;
    *(s16 *)(boss + 0x366) = 0;
    *(s16 *)(boss + 0x368) = 0;
    *(s16 *)(boss + 0x36A) = 0;
    tt_boss3_part0_pose();
    tt_boss3_part1_pose();
    tt_boss3_part2_pose();
    tt_boss3_part3_pose();
    tt_sprite_set_9_clut = 0x3E50;
    *(u8 **)(boss + 0x228) = tt_boss3_hitbox_slot_228;
    *(u8 **)(boss + 0x22C) = tt_boss3_hitbox_slot_22c;
}

void tt_boss3_enter(void) {
    u8 *boss = tt_boss;

    *(s32 *)(boss + 0x20) = 0;
    *(s32 *)(boss + 0x24) += 0x10000;
    *(s16 *)(boss + 0x14) = *(s16 *)(boss + 0x22);
    *(s16 *)(boss + 0x16) = *(s16 *)(boss + 0x26);
    if (*(s16 *)(boss + 0x16) >= 0x40) {
        *(s32 *)(boss + 0x24) = 0x400000;
        *(s16 *)(boss + 0x16) = *(s16 *)(boss + 0x26);
        *(u8 **)(boss + 0x218) = tt_boss3_hitboxes;
        *(s16 *)(boss + 4) = 0x40;
        *(s16 *)(boss + 0xA) = 0;
        *(s16 *)(boss + 8) = 0;
        *(s16 *)(boss + 6) = 0;
        boss[0xF] = 0;
        boss[0xE] = 0;
        boss[0xD] = 0;
    }
}

void tt_boss3_move_form0(void) {
    u8 *boss = tt_boss;
    s32 i;
    s32 x;

    switch (*(u16 *)(boss + 6)) {
    case 0:
        *(s32 *)(boss + 0x20) = rsin(*(s32 *)(boss + 0x3E4)) * 0x500;
        *(s32 *)(boss + 0x24) = 0x400000;
        *(s32 *)(boss + 0x3E4) += 8;
        if (*(s32 *)(boss + 0x3E4) >= 0x2000) {
            *(s32 *)(boss + 0x3E4) = 0;
            *(s16 *)(boss + 6) = 1;
        }
        break;
    case 1:
        *(s32 *)(boss + 0x24) -= 0x8000;
        if (*(s32 *)(boss + 0x24) <= 0x300000) {
            *(s32 *)(boss + 0x24) = 0x300000;
            *(s16 *)(boss + 6) = 2;
        }
        break;
    case 2:
        *(s32 *)(boss + 0x20) = rsin(*(s32 *)(boss + 0x3E4)) * 0x500;
        *(s32 *)(boss + 0x24) = 0x300000;
        *(s32 *)(boss + 0x3E4) += 0x10;
        if (*(s32 *)(boss + 0x3E4) >= 0x2000) {
            *(s32 *)(boss + 0x3E4) = 0;
            *(s16 *)(boss + 6) = 3;
        }
        break;
    case 3:
        *(s32 *)(boss + 0x24) += 0x8000;
        if (*(s32 *)(boss + 0x24) >= 0x400000) {
            *(s32 *)(boss + 0x24) = 0x400000;
            *(s16 *)(boss + 6) = 0;
        }
        break;
    case 4:
        if (*(s32 *)(tt_boss + 0x24) < 0x400000) {
            *(s32 *)(boss + 0x24) += 0x8000;
        } else {
            *(s32 *)(boss + 0x24) = 0x400000;
            *(s16 *)(boss + 6) = 5;
        }
        break;
    case 5:
        x = *(s32 *)(tt_boss + 0x20);
        if ((x < 0 ? -x : x) < 0x10000) {
            *(s32 *)(boss + 0x20) = 0;
            *(s16 *)(boss + 6) = 6;
        } else if (x < 0) {
            *(s32 *)(boss + 0x20) += 0x10000;
        } else {
            *(s32 *)(boss + 0x20) -= 0x10000;
        }
        break;
    case 6:
        tt_se_request(0x50A, boss); /* FAKE: boss as an extra argument keeps it in a1, as in the original. T-11800 */
        *(s16 *)(boss + 0x30E) = 1;
        *(s16 *)(boss + 6) = 0;
        tt_sprite_set_9_clut = 0x3ED0;
        *(u8 **)(boss + 0x218) = tt_boss3_hitboxes;
        /* four slots, fully unrolled by IDO; the s8 flag store shares the constant 1 with the +0x30E store */
        for (i = 0; i < 4; i++) {
            if (boss[0x311 + i * 2] != 0) {
                *(s8 *)(boss + 0x343 + i) = 1;
                *(s16 *)(boss + 0x2D8 + i * 2) = -1;
            }
        }
        *(s32 *)(boss + 0x3E4) = 0;
        *(s16 *)(boss + 0x364) = 0x80;
        break;
    }
    if (*(s16 *)(boss + 0x30C) < 0) {
        *(u8 **)(boss + 0x218) = 0;
        *(s16 *)(boss + 0x30C) = 0xA0;
        *(s16 *)(boss + 6) = 4;
    }
}

void tt_boss3_move_form1(void) {
    u8 *boss = tt_boss;

    *(s32 *)(boss + 0x20) = rsin(*(s32 *)(boss + 0x3E4)) * 0x500;
    *(s32 *)(boss + 0x24) = (func_800A0140(*(s32 *)(boss + 0x3E4) * 4) << 7) + 0x380000;
    *(s32 *)(boss + 0x3E4) += 0x10;
}

void tt_boss3_move(void) {
    u8 *boss = tt_boss;

    switch (*(u16 *)(boss + 0x30E)) {
    case 0:
        tt_boss3_move_form0();
        break;
    case 1:
        tt_boss3_move_form1();
        break;
    }
    *(s16 *)(boss + 0x14) = *(s16 *)(boss + 0x22);
    *(s16 *)(boss + 0x16) = *(s16 *)(boss + 0x26);
}

void tt_boss3_death_sequence(void) {
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
        tt_boss_hitboxes_clear();
        *(u16 *)(boss + 0x364) = 0;
        *(u16 *)(boss + 0x3D4) = 0;
        for (i = 0x10; i != 0x13; i++) {
            ((TtPartAnimationView *)(boss + 0x38))[i].sets = tt_sprite_set_table;
            ((TtPartAnimationView *)(boss + 0x38))[i].flip = 0;
            ((TtPartAnimationView *)(boss + 0x38))[i].set = 4;
            ((TtPartAnimationView *)(boss + 0x38))[i].unk_0A = 0;
            ((TtPartAnimationView *)(boss + 0x38))[i].transform_enable = 0;
            boss[0x310 + i] = 0;
            ((s16 *)(boss + 0x2A8))[i] = 8;
            ((s16 *)(boss + 0x248))[i] = 0;
            ((s16 *)(boss + 0x278))[i] = 0;
        }
        *(u16 *)(boss + 8) = 0x40;
        break;
    case 0x40:
        t = tt_boss3_death_timeline;
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

void tt_boss3_pose_by_height(void) {
    u8 *boss = tt_boss;
    s16 v = *(s16 *)(boss + 0x16);

    if (v >= 0x3F) {
        *(u16 *)(boss + 0x3A4) = 0xB;
        *(u16 *)(boss + 0x3A6) = 0xB;
        return;
    }
    if (v >= 0x3B) {
        *(u16 *)(boss + 0x3A4) = 0xA;
        *(u16 *)(boss + 0x3A6) = 0xA;
        return;
    }
    if (v >= 0x37) {
        *(u16 *)(boss + 0x3A4) = 9;
        *(u16 *)(boss + 0x3A6) = 9;
        return;
    }
    if (v >= 0x33) {
        *(u16 *)(boss + 0x3A4) = 8;
        *(u16 *)(boss + 0x3A6) = 8;
    }
}

/* T-11540/T-11800 near miss, 4 lines: T-11800 typed s16 indexing ((s16 *)(c + 0x248))[j] gives the original's addu c, j*2 operand order, and an empty if (j) {} (FAKE) raises j above the i * 2 CSE so j takes $v0 and i * 2 $v1 (was 20 rows). Left: in the hit block the c + i * 4 address (a1) is computed after the 0x2C8 store (the original computes it before and fills the bnez delay slot with the store), and the preheader splits lui/addiu of tt_sprite_set_table around r = c. Tried: an a2 local before/after the store, c + t * 2 and &c[t * 2] forms, order_search on the hit block and the three inits, a tp = tt_sprite_set_table local. */
void tt_boss3_part_death_animations(void) {
    u8 *boss;
    u8 *a;
    s32 i;
    s32 j;
    s32 t;

    boss = tt_boss;
    boss[0x8E] = *(u16 *)(boss + 0x3AC) >> 1;
    *(u16 *)(boss + 0x3AC) += 1;
    if (*(u16 *)(boss + 0x3AC) >= 6) {
        *(u16 *)(boss + 0x3AC) = 0;
    }
    for (i = 0; i < 4; i++) {
        t = i * 2;
        a = boss + t;
        if (boss[i + 0x343] != 0) {
            *(u8 **)(boss + i * 0x14 + 0x178) = tt_sprite_set_table;
            boss[i * 0x14 + 0x17C] = 0;
            boss[i * 0x14 + 0x17D] = 4;
            boss[i * 0x14 + 0x180] = 0x80;
            boss[i * 0x14 + 0x17E] = 0x34;
            boss[i * 0x14 + 0x182] = 0;
            boss[i * 0x14 + 0x188] = 0;
            boss[i + 0x320] = 1;
            /* FAKE: the empty test adds a block to the live range of i * 2 so that j takes $v0 before it
               (priority over more blocks); on the store's line it keeps the original's delay slot. T-11830 */
            *(s16 *)(a + 0x2C8) = 8; if (i) {}
            if (i >= 2) {
                j = t + 2;
            } else {
                j = t;
            }
            *(s16 *)(a + 0x268) = *(s16 *)(boss + 0x248 + j * 2);
            if (i >= 2) {
                j = t + 2;
            } else {
                j = t;
            }
            *(s16 *)(a + 0x298) = *(s16 *)(boss + 0x278 + j * 2);
            if (i >= 2) {
                *(s16 *)(a + 0x298) += 0xC;
            }
            *(u8 **)(boss + i * 0x14 + 0x1C8) = tt_sprite_set_table;
            boss[i * 0x14 + 0x1CC] = 0;
            boss[i * 0x14 + 0x1CD] = 4;
            boss[i * 0x14 + 0x1D0] = 0x80;
            boss[i * 0x14 + 0x1CE] = 0x34;
            boss[i * 0x14 + 0x1D2] = 0;
            boss[i * 0x14 + 0x1D8] = 0;
            boss[i + 0x324] = 1;
            *(s16 *)(a + 0x2D0) = 8;
            *(s16 *)(a + 0x270) = ((s16 *)(boss + 0x248))[t + 1];
            *(s16 *)(a + 0x2A0) = ((s16 *)(boss + 0x278))[t + 1];
            boss[i + 0x343] = 0;
        }
        tt_boss_part_animation_step(tt_boss3_part_death_script, i + 0x10);
        tt_boss_part_animation_step(tt_boss3_part_death_script, i + 0x14);
        if (boss[i * 0x14 + 0x17E] == 0x38) {
            if (i >= 2) {
                j = t + 2;
            } else {
                j = t;
            }
            boss[j + 0x310] = 0;
            boss[i * 2 + 0x311] = 0;
        }
    }
    if (*(u16 *)(boss + 4) != 0x20 && *(u16 *)(boss + 0x30E) == 0) {
        tt_boss3_pose_by_height();
    }
}

void tt_boss3_attack_form0(void) {
    u8 *boss;
    u16 k;

    k = *(u16 *)(tt_boss + 6);
    boss = tt_boss;
    switch (k) {
    case 0:
        switch (*(u16 *)(tt_boss + 0x364)) {
        case 0:
            *(u16 *)(boss + 0x3A8) = 2;
            *(u16 *)(boss + 0x3AA) = 2;
            break;
        case 0x3E:
            *(s16 *)(boss + 0x3A8) = 1;
            *(s16 *)(boss + 0x3AA) = 1;
            break;
        case 0x40:
            *(s16 *)(boss + 0x3A8) = 0;
            *(s16 *)(boss + 0x3AA) = 0;
            tt_enemy_shot_fire_aimed(*(s16 *)(boss + 0x14), *(s16 *)(boss + 0x16) + 0x38, 0x18, 0xA8);
            if (*(s16 *)(boss + 0x2DC) >= 0) {
                tt_enemy_shot_fire_3way_down(*(s16 *)(boss + 0x14) + 0x26, *(s16 *)(boss + 0x16) + 0x3E, 0x100, 0x18, 0xA8);
            }
            if (*(s16 *)(boss + 0x2DE) >= 0) {
                tt_enemy_shot_fire_3way_down(*(s16 *)(boss + 0x14) - 0x26, *(s16 *)(boss + 0x16) + 0x3E, 0x100, 0x18, 0xA8);
            }
            break;
        case 0x48:
            *(s16 *)(boss + 0x3A8) = 1;
            *(s16 *)(boss + 0x3AA) = 1;
            break;
        case 0x4E:
            *(u16 *)(boss + 0x3A8) = 2;
            *(u16 *)(boss + 0x3AA) = 2;
            break;
        }
        if (*(u16 *)(boss + 0x364) >= 0x80) {
            *(u16 *)(boss + 0x364) = 0;
        }
        break;
    case 2:
        if (*(u16 *)(tt_boss + 0x364) >= 0x40) {
            *(u16 *)(boss + 0x364) = 0;
            tt_enemy_shot_fire_3way_aimed(*(s16 *)(boss + 0x14), *(s16 *)(boss + 0x16) + 0x38, 0x100, 0x20, 0xA8);
        }
        if (*(u16 *)(boss + 0x368) >= 8) {
            if (*(s16 *)(boss + 0x2D8) >= 0 || *(s16 *)(boss + 0x2DA) >= 0) {
                tt_se_request(0x50C);
            }
            *(u16 *)(boss + 0x368) = 0;
            if (*(s16 *)(boss + 0x2D8) >= 0) {
                tt_enemy_shot_fire_animated(*(s16 *)(boss + 0x14) + 0x38, *(s16 *)(boss + 0x16) + 0x10, 0x20, 0x400);
            }
            if (*(s16 *)(boss + 0x2DA) >= 0) {
                tt_enemy_shot_fire_animated(*(s16 *)(boss + 0x14) - 0x38, *(s16 *)(boss + 0x16) + 0x10, 0x20, 0x400);
            }
            /* FAKE: redundant break; the skip path then jumps straight to the switch end (no join block),
               so the 0x368 load of the test stays in the $v1 range up to the increments. T-11510 */
            break;
        }
        break;
    }
    *(u16 *)(boss + 0x364) += 1;
    *(u16 *)(boss + 0x366) += 1;
    *(u16 *)(boss + 0x368) += 1;
}

void tt_boss3_attack_form1(void) {
    u8 *boss;
    u8 *player;
    s32 ang;
    s32 i;
    s32 v;

    boss = tt_boss;
    player = tt_player;
    if (*(u16 *)(boss + 0x364) == 0x80 || *(u16 *)(boss + 0x364) == 0x100) {
        tt_se_request(0x618);
        ang = func_8014D260((*(s32 *)(player + 0x24) - *(s32 *)(boss + 0x24)) >> 16,
                            (*(s32 *)(player + 0x20) - *(s32 *)(boss + 0x20)) >> 16);
        i = 0x180;
        v = ang + 0x180;
        do {
            tt_enemy_shot_fire_angle(*(s16 *)(boss + 0x14) - 0x12, *(s16 *)(boss + 0x16) + 0xA, 0x18, v, 0xAC);
            i += 0x100;
            v += 0x100;
        } while (i < 0x680);
        v = ang - 0x180;
        for (i = 0x180; i != 0x680; i += 0x100) {
            tt_enemy_shot_fire_angle(*(s16 *)(boss + 0x14) + 0x12, *(s16 *)(boss + 0x16) + 0xA, 0x18, v, 0xAC);
            v -= 0x100;
        }
    }
    if (*(u16 *)(boss + 0x364) == 0x178 || *(u16 *)(boss + 0x364) == 0x200) {
        tt_enemy_shot_fire_fan9_aimed(*(s16 *)(boss + 0x14) - 0x12, *(s16 *)(boss + 0x16) + 0xA, 0xC0, 0x28, 0xA0);
    }
    if (*(u16 *)(boss + 0x364) == 0x180 || *(u16 *)(boss + 0x364) == 0x1F8) {
        tt_enemy_shot_fire_fan9_aimed(*(s16 *)(boss + 0x14) + 0x12, *(s16 *)(boss + 0x16) + 0xA, 0xC0, 0x28, 0xA0);
    }
    if (*(u16 *)(boss + 0x364) >= 0x200) {
        *(u16 *)(boss + 0x364) = 0;
    } else {
        *(u16 *)(boss + 0x364) += 1;
    }
}

void tt_boss3_attack(void) {
    switch (*(u16 *)(tt_boss + 0x30E)) {
    case 0:
        tt_boss3_attack_form0();
        break;
    case 1:
        tt_boss3_attack_form1();
        break;
    }
}

void tt_boss3_update(s32 arg0) {
    u8 *state;
    u8 *view;
    u8 *pool;
    u8 *aux;

    state = tt_boss;
    view = tt_core;
    aux = tt_player;
    switch (*(u16 *)(state + 4)) {
    case 0x0:
        tt_field_enemies_destroy(arg0, aux);
        if (*(u16 *)(view + 4) == 0) {
            audio_command(0xB1);
            func_800451E0(0x8019A000);
            audio_command(0x12);
            audio_command(0x23);
            audio_command(0x2E);
            audio_command(0x21);
            audio_command(4);
            audio_command(0x200);
        }
        tt_boss3_init();
        *(s16 *)(state + 4) = 0x20;
        break;
    case 0x20:
        tt_boss3_part0_pose();
        tt_boss3_part1_pose();
        tt_boss3_part2_pose();
        tt_boss3_part3_pose();
        tt_boss3_enter();
        tt_boss3_part_death_animations();
        break;
    case 0x40:
        tt_boss3_move();
        tt_boss3_part_death_animations();
        tt_boss3_attack();
        tt_boss3_part0_pose();
        tt_boss3_part1_pose();
        tt_boss3_part2_pose();
        tt_boss3_part3_pose();
        if (*(u16 *)(state + 782) == 1 && *(s16 *)(state + 780) < 0) {
            *(s16 *)(state + 4) = 0x80;
            *(s16 *)(state + 10) = 0;
            *(s16 *)(state + 8) = 0;
            *(s16 *)(state + 6) = 0;
            state[15] = 0;
            state[14] = 0;
            state[13] = 0;
        }
        break;
    case 0x80:
        audio_command(0xC1);
        aux[82] = 1;
        tt_boss3_death_sequence();
        break;
    case 0xA0:
        audio_command(0xB1);
        *state = 0;
        view[18] = 0;
        pool = (u8 *)tt_stage_script;
        *(s16 *)(pool + 30) = 1;
        *(s16 *)(view + 2) = 0xA0;
        break;
    }
    tt_boss3_collision();
}
