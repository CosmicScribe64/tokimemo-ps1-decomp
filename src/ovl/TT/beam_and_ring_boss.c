/* beam_and_ring_boss.c: src/ovl/TT/8013F040.c:645: boss form changes, aimed fan and drone-ring phase, beam phase and thrown-body attack; body 656: mode = ((TtBossView *) tt_boss)->unk_08; 657: other = tt_player;. Original object 0x8013F040-0x801410A0 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/TT.h"

typedef struct TtBossPartFrameView {
    /* 0x00 */ u8 pad[0x12];
    /* 0x12 */ u8 frame;
    /* 0x13 */ u8 pad13;
} TtBossPartFrameView; /* size 0x14 */

typedef struct TtBossPartTimelineView {
    /* 0x000 */ u8 pad[0x40];
    /* 0x040 */ TtBossPartFrameView frames[10];
    /* 0x108 */ u8 pad108[0x25C];
    /* 0x364 */ u16 timer;
} TtBossPartTimelineView; /* accessed prefix size 0x366; T-10030 */

void tt_boss4_collision(void) {
    u8 *boss;
    u8 *player;
    s32 pad[9]; /* FAKE: unused, places pt, box and the spilled q at the original's frame offsets. T-11860 */
    s16 pt[2];
    TtHitbox box;
    s32 i;
    s32 j;
    u8 *shot;
    u8 *q;

    boss = tt_boss;
    player = tt_player;
    if (boss[0] == 0) {
        return;
    }
    for (i = 0; i < 0x18; i++) {
        boss[i * 0x14 + 0x42] = 0;
    }
    if (boss[0x342] == 0 && tt_boss_check_hits() != 0) {
        tt_se_request(0x503);
    }
    for (i = 0x10; i < 0x13; i++) {
        if (boss[i + 0x310] != 0) {
            boss[i * 0x14 + 0x42] = 0;
        }
    }
    if (boss[0x343] == 0) {
        for (i = 0x10, q = boss + 0x10; i != 0x13; i++, q++) {
            if (q[0x310] != 0) {
                pt[0] = *(s16 *)(boss + 0x14) + ((s16 *)(boss + 0x248))[i];
                pt[1] = *(s16 *)(boss + 0x16) + ((s16 *)(boss + 0x278))[i];
                if (player[0x52] == 0 && player[0] != 0 && tt_box_overlap(player + 0x14, tt_hitboxes, pt, &tt_boss4_hitbox_body) != 0) {
                    tt_player_hit();
                    *(s16 *)(player + 4) = 0x60;
                    *(s16 *)(player + 10) = 0;
                    *(s16 *)(player + 8) = 0;
                    *(s16 *)(player + 6) = 0;
                    player[15] = 0;
                    player[14] = 0;
                    player[13] = 0;
                }
                for (j = 0, shot = tt_shots; j != 12; j++, shot += 0x54) {
                    /* Separate tests number the hoisted constants 12, 0x40, &tt_hitbox_shot, 0x80 as in the original. */
                    if (shot[0] == 0) {
                        continue;
                    }
                    if (*(u16 *)(shot + 4) != 0x40) {
                        continue;
                    }
                    if (tt_box_overlap(shot + 0x14, &tt_hitbox_shot, pt, &tt_boss4_hitbox_body) == 0) {
                        continue;
                    }
                    *(u16 *)(shot + 4) = 0x80;
                    *(s16 *)(shot + 10) = 0;
                    *(s16 *)(shot + 8) = 0;
                    *(s16 *)(shot + 6) = 0;
                    shot[15] = 0;
                    shot[14] = 0;
                    shot[13] = 0;
                }
            }
        }
    }
    if (boss[0x344] == 0) {
        boss[0x20E] = 0;
        if (player[0] != 0) {
            pt[0] = *(s16 *)(boss + 0x14) + *(s16 *)(boss + 0x276);
            pt[1] = *(s16 *)(boss + 0x16) + *(s16 *)(boss + 0x2A6);
            box.x = 0;
            box.y = 0;
            box.half_w = *(s16 *)(boss + 0x210) * 0x30 / 4096;
            box.half_h = *(s16 *)(boss + 0x212) * 0x30 / 4096;
            if (player[0x52] == 0 && player[0] != 0 && tt_box_overlap(player + 0x14, tt_hitboxes, pt, &box) != 0) {
                tt_player_hit();
                *(s16 *)(player + 4) = 0x60;
                *(s16 *)(player + 10) = 0;
                *(s16 *)(player + 8) = 0;
                *(s16 *)(player + 6) = 0;
                player[15] = 0;
                player[14] = 0;
                player[13] = 0;
            }
        }
    }
}

void tt_boss4_init(void) {
    u8 *boss;
    s32 i;

    boss = tt_boss;
    tt_boss_parts_hide_all();
    tt_boss_hitboxes_clear();
    boss[0] = 1;
    boss[0x342] = 0;
    *(s32 *)(boss + 0x20) = 0;
    *(s32 *)(boss + 0x24) = 0xFFC00000;
    *(s16 *)(boss + 0x14) = 0;
    *(s16 *)(boss + 0x16) = -0x40;
    *(s32 *)(boss + 0x28) = 0;
    *(s32 *)(boss + 0x2C) = 0;
    *(s16 *)(boss + 0x30C) = 0xE0;
    *(s16 *)(boss + 0x30E) = 0;
    for (i = 0; i < 11; i++) {
        ((TtPartAnimation *)(boss + 0x38))[i].sets = tt_sprite_set_table;
        ((TtPartAnimation *)(boss + 0x38))[i].flip = tt_boss4_part_flip[i];
        ((TtPartAnimation *)(boss + 0x38))[i].set = 0xA;
        ((TtPartAnimation *)(boss + 0x38))[i].frame = tt_boss4_part_frame[i];
        ((TtPartAnimation *)(boss + 0x38))[i].flags = 0x60;
        ((TtPartAnimation *)(boss + 0x38))[i].clut = 0xF;
        ((TtPartAnimation *)(boss + 0x38))[i].transform_enable = 0;
        boss[i + 0x310] = 1;
        ((s16 *)(boss + 0x2A8))[i] = 9;
        ((s16 *)(boss + 0x248))[i] = 0;
        ((s16 *)(boss + 0x278))[i] = 0;
    }
    *(s32 *)(boss + 0x3E4) = *(s32 *)(boss + 0x20);
    *(s16 *)(boss + 0x2AA) = 8;
    *(s16 *)(boss + 0x2AC) = 8;
    *(s16 *)(boss + 0x3A4) = 0;
    *(s16 *)(boss + 0x3A6) = 0;
    *(s32 *)(boss + 0x3E8) = 0;
    *(s32 *)(boss + 0x3EC) = 0;
    *(s32 *)(boss + 0x3F0) = 0;
    *(s32 *)(boss + 0x3F4) = 0;
    *(s32 *)(boss + 0x3F8) = 0;
    *(s16 *)(boss + 0x364) = 0;
    *(s16 *)(boss + 0x366) = 0;
    *(s16 *)(boss + 0x368) = 0;
    boss[0x343] = 1;
    boss[0x344] = 1;
    *(u8 **)(boss + 0x228) = tt_boss4_hitbox_slot_228;
    *(u8 **)(boss + 0x22C) = tt_boss4_hitbox_slot_22c;
    *(u8 **)(boss + 0x230) = tt_boss4_hitbox_slot_230;
}

void tt_boss4_enter(void) {
    u8 *boss = tt_boss;

    *(s32 *)(boss + 0x20) = 0;
    *(s32 *)(boss + 0x24) += 0x10000;
    *(s16 *)(boss + 0x14) = *(s16 *)(boss + 0x22);
    *(s16 *)(boss + 0x16) = *(s16 *)(boss + 0x26);
    if (*(s16 *)(boss + 0x16) >= 0x40) {
        *(s32 *)(boss + 0x24) = 0x400000;
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

void tt_boss4_intro_animation(void) {
    u8 *boss = tt_boss;

    tt_boss_part_animation_step(tt_boss4_intro_script, 0);
    if (boss[0x40] & 1) {
        *(u8 **)(boss + 0x218) = tt_boss4_hitbox_slot_218;
        *(s16 *)(boss + 4) = 0x40;
    }
}

void tt_boss4_follow_player_x(void) {
    u8 *boss;
    u8 *player;
    s32 d;
    s32 a;

    boss = tt_boss;
    player = tt_player;
    d = *(s32 *)(player + 0x20) - *(s32 *)(boss + 0x3E4);
    a = d < 0 ? -d : d;
    if (a < 0x4000) {
        *(s32 *)(boss + 0x3E4) = *(s32 *)(player + 0x20);
    } else if (d < 0) {
        *(s32 *)(boss + 0x3E4) -= 0x4000;
    } else {
        *(s32 *)(boss + 0x3E4) += 0x4000;
    }
    if (*(s32 *)(boss + 0x3E4) < -0x400000) {
        *(s32 *)(boss + 0x3E4) = -0x400000;
    } else if (*(s32 *)(boss + 0x3E4) > 0x400000) {
        *(s32 *)(boss + 0x3E4) = 0x400000;
    }
    *(s32 *)(boss + 0x20) = *(s32 *)(boss + 0x3E4) + (rsin(*(s32 *)(boss + 0x3E8)) << 9);
    *(s32 *)(boss + 0x3E8) += 8;
    if (*(s32 *)(boss + 0x3E8) > 0x1000) {
        *(s32 *)(boss + 0x3E8) = 0;
    }
}

void tt_boss4_sweep_x(void) {
    u8 *boss = tt_boss;
    s32 v = *(s32 *)(boss + 0x20);

    if (v < (s32)0xFFE00000) {
        *(s32 *)(boss + 0x20) += 0x10000;
        return;
    }
    if (v >= 0x200001) {
        *(s32 *)(boss + 0x20) += 0xFFFF0000;
        return;
    }
    *(s16 *)(boss + 6) = 2;
}

void tt_boss4_move(void) {
    switch (*(u16 *)(tt_boss + 0x30E)) {
    case 0:
        switch (*(u16 *)(tt_boss + 6)) {
        case 0:
            tt_boss4_follow_player_x();
            return;
        case 1:
            tt_boss4_sweep_x();
            return;
        }
        break;
    case 1:
        switch (*(u16 *)(tt_boss + 6)) {
        case 0:
            tt_boss4_follow_player_x();
            return;
        case 1:
            tt_boss4_sweep_x();
            return;
        }
        break;
    }
}

void tt_boss4_part_animation(void) {
    TtBossPartTimelineView *scene;

    scene = (TtBossPartTimelineView *)tt_boss;
    scene->frames[0].frame = ((scene->timer >> 2) & 1) + 14;
    scene->frames[1].frame = ((scene->timer >> 2) & 1) + 14;
    if ((scene->timer & 7) == 7) {
        scene->frames[2].frame++;
        if (scene->frames[2].frame >= 52U) {
            scene->frames[2].frame = 46;
        }
        scene->frames[3].frame = scene->frames[2].frame;
        scene->frames[4].frame++;
        if (scene->frames[4].frame >= 46U) {
            scene->frames[4].frame = 40;
        }
        scene->frames[5].frame = scene->frames[4].frame;
        scene->frames[6].frame++;
        if (scene->frames[6].frame >= 40U) {
            scene->frames[6].frame = 34;
        }
        scene->frames[7].frame = scene->frames[6].frame;
    }
    if ((scene->timer & 15) == 15) {
        if (scene->frames[8].frame & 1) {
            scene->frames[8].frame--;
        } else {
            scene->frames[8].frame++;
        }
    }
    scene->frames[9].frame = (scene->timer >> 4) & 1;
    scene->timer++;
}

/* FAKE: the loop counter and pointer are declared as parameters so that they get $a0/$a1 like the original; nothing reads the incoming values. T-4050 */
void tt_boss4_part_animation_cycle(s32 i, u8 *p) {
    i = 0x140;
    p = tt_boss + 0x140;
    if ((*(u16 *)(tt_boss + 0x366) & 7) == 7) {
        do {
            i += 0x14;
            p[0x3E] += 1;
            if (p[0x3E] >= 0x16U) {
                p[0x3E] = 0x10;
            }
            p += 0x14;
        } while (i != 0x17C);
    }
}

void tt_boss4_form_change(void) {
    u8 *boss;
    s32 i;
    s16 v;

    boss = tt_boss;
    switch (*(u16 *)(boss + 6)) {
    case 0:
        if (*(s32 *)(tt_boss + 0x24) >= 0x600000) {
            *(s32 *)(boss + 0x24) = 0x600000;
            *(s16 *)(boss + 6) = 0x20;
        } else {
            *(s32 *)(boss + 0x24) += 0x8000;
        }
        break;
    case 0x20:
        for (i = 0xB; i < 0xD; i++) {
            ((TtPartAnimation *)(boss + 0x38))[i].sets = tt_sprite_set_table;
            ((TtPartAnimation *)(boss + 0x38))[i].flip = 0;
            ((TtPartAnimation *)(boss + 0x38))[i].set = 0xA;
            ((TtPartAnimation *)(boss + 0x38))[i].flags = 0x60;
            ((TtPartAnimation *)(boss + 0x38))[i].clut = 0xF;
            ((TtPartAnimation *)(boss + 0x38))[i].transform_enable = 0;
            boss[i + 0x310] = 1;
            ((s16 *)(boss + 0x2A8))[i] = 8;
            ((s16 *)(boss + 0x248))[i] = 0;
            ((s16 *)(boss + 0x278))[i] = -0x78;
        }
        boss[0x11A] = 0x18;
        boss[0x12E] = 0xC;
        *(s16 *)(boss + 6) = 0x40;
        break;
    case 0x40:
        v = *(s16 *)(tt_boss + 0x28E);
        if (v >= -0x30) {
            tt_se_request(0x502);
            *(s16 *)(boss + 0x28E) = -0x30;
            *(s16 *)(boss + 0x290) = -0x30;
            *(s16 *)(boss + 6) = 0x50;
            break;
        }
        if (v >= -0x40) {
            if (*(u8 *)(tt_boss + 0xF2) == 0x16) {
                boss[0xF2] = 0x3C;
            } else if (*(u8 *)(tt_boss + 0xF2) == 0x17) {
                boss[0xF2] = 0x3D;
            }
        }
        *(s16 *)(boss + 0x28E) += 1;
        *(s16 *)(boss + 0x290) = *(s16 *)(boss + 0x28E);
        break;
    case 0x50:
        tt_boss_part_animation_step(tt_boss4_part_0b_script, 0xB);
        if (boss[0x11C] & 1) {
            *(s16 *)(boss + 4) = 0x80;
            *(s16 *)(boss + 6) = 0;
        }
        break;
    }
}

void tt_boss4_death_sequence(void) {
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
        tt_se_request(0x501);
        *(u16 *)(boss + 0x364) = 0;
        *(u16 *)(boss + 0x3D4) = 0;
        for (i = 3; i < 9; i++) {
            ((TtPartAnimationView *)(boss + 0x38))[i].sets = tt_sprite_set_table;
            ((TtPartAnimationView *)(boss + 0x38))[i].flip = 0;
            ((TtPartAnimationView *)(boss + 0x38))[i].set = 4;
            ((TtPartAnimationView *)(boss + 0x38))[i].flags = 0x80;
            ((TtPartAnimationView *)(boss + 0x38))[i].unk_0A = 0;
            ((TtPartAnimationView *)(boss + 0x38))[i].transform_enable = 0;
            ((s16 *)(boss + 0x2A8))[i] = 6;
        }
        ((s16 *)(boss + 0x248))[3] = -0x26;
        ((s16 *)(boss + 0x278))[3] = 0xF;
        ((s16 *)(boss + 0x248))[4] = 0x26;
        ((s16 *)(boss + 0x278))[4] = 0xF;
        ((s16 *)(boss + 0x248))[5] = -0x2C;
        ((s16 *)(boss + 0x278))[5] = 0xA;
        ((s16 *)(boss + 0x248))[6] = 0x2C;
        ((s16 *)(boss + 0x278))[6] = 0xA;
        ((s16 *)(boss + 0x248))[7] = -0x30;
        ((s16 *)(boss + 0x278))[7] = 0;
        ((s16 *)(boss + 0x248))[8] = 0x30;
        ((s16 *)(boss + 0x278))[8] = 0;
        for (i = 3; i < 9; i++) {
            tt_boss_part_animation_step(tt_boss4_part_script, i);
        }
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
        t = tt_boss4_death_timeline;
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
        for (i = 3; i < 9; i++) {
            tt_boss_part_animation_step(tt_boss4_part_script, i);
        }
        for (i = 0x10; i < 0x13; i++) {
            tt_boss_part_animation_step(tt_boss_death_part_script, i);
        }
        if (*(u16 *)(boss + 0x364) == 0x8F) {
            for (i = 0; i != 0x10; i++) {
                boss[0x310 + i] = 0;
            }
        }
        if (*(u16 *)(boss + 0x364) == 0xA6) {
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

/* FAKE: the one/period locals and declaration order reproduce the original
 * constant allocation and stack layout; original source unknown. T-9520. */
void tt_boss4_form0_step0_aimed_fan(void) {
    u16 mode;
    u8 *player;
    s32 fake_one;
    s32 angle;
    u32 period;
    TtBossView *boss;
    TtPositionView point;

    fake_one = 1;
    period = 384;
    mode = ((TtBossView *) tt_boss)->attack_step;
    player = tt_player;
    boss = (TtBossView *) tt_boss;
    switch (mode) {
    case 0:
        boss->phase_timer = 0;
        boss->attack_step = 0x40;
        break;
    case 0x40:
        if ((u16) boss->phase_timer % period == 192U) {
            tt_enemy_shot_fire_fan9_aimed(boss->x, boss->y + 8, 0x80, 0x30, 0xAA);
        }
        if ((boss->phase_timer & 7) == 7) {
            point.x = boss->x << 16;
            point.y = (boss->y + 8) << 16;
            angle = tt_angle_between((s32) &point, (s32) (player + 0x14));
            tt_enemy_shot_fire_angle(boss->x, boss->y + 8, 0x18, boss->angle_sign * boss->angle_offset + angle, 0xA0);
            boss->angle_offset += 0x40;
            if ((u16) boss->angle_offset >= 0x301U) {
                boss->angle_offset = 0;
            }
            if ((u16) boss->angle_sign == fake_one) {
                boss->angle_sign = -fake_one;
            } else {
                boss->angle_sign = fake_one;
            }
        }
        if ((u16) boss->phase_timer >= period) {
            boss->attack_step = 0xA0;
        }
        break;
    case 0xA0:
        boss->phase = fake_one;
        boss->attack_step = 0;
        break;
    }
    boss->phase_timer++;
}

/* T-11840: matched once the 7 stored to +0x2A8[i] is u16 (it then shares v0 with the (timer & 7) == 7 test), the sums are written base + table[index] (uopt loads the table entry first), and two unused words size the frame. */
void tt_boss4_form0_step2_drone_rings(void) {
    u16 mode;
    u8 *other;
    u8 *boss;
    s32 angle;
    s32 pad[2]; /* FAKE: unused, sizes the frame (0x88, point at sp+0x5C). T-11840 */
    TtPositionView point;
    s32 i;

    boss = tt_boss;
    mode = *(u16 *)(boss + 8);
    other = tt_player;
    switch (mode) {
    case 0:
        tt_se_request(0x512);
        *(u16 *)(boss + 0x366) = 0;
        for (i = 0x10; i < 0x13; i++) {
            ((TtPartAnimation *)(boss + 0x38))[i].sets = tt_sprite_set_table;
            ((TtPartAnimation *)(boss + 0x38))[i].flip = 0;
            ((TtPartAnimation *)(boss + 0x38))[i].set = 0xA;
            ((TtPartAnimation *)(boss + 0x38))[i].frame = 0x10;
            ((TtPartAnimation *)(boss + 0x38))[i].flags = 0x60;
            ((TtPartAnimation *)(boss + 0x38))[i].clut = 0xF;
            ((TtPartAnimation *)(boss + 0x38))[i].transform_enable = 0;
            boss[i + 0x310] = 1;
            ((u16 *)(boss + 0x2A8))[i] = 7;
            ((s16 *)(boss + 0x248))[i] = 0;
            ((s16 *)(boss + 0x278))[i] = 8;
        }
        *(s32 *)(boss + 0x3EC) = 0x80000;
        *(s32 *)(boss + 0x3F0) = -0x10000;
        *(s32 *)(boss + 0x3F4) = 0x400;
        *(s32 *)(boss + 0x3F8) = 0;
        boss[0x343] = 0;
        *(u16 *)(boss + 8) = 0x40;
        break;
    case 0x40:
        if (*(s32 *)(tt_boss + 0x3EC) > 0x200000) {
            *(s32 *)(boss + 0x3F4) = -0x400;
        } else {
            *(s32 *)(boss + 0x3F4) = 0x400;
        }
        if (*(s32 *)(boss + 0x3EC) > 0x200000 && *(s32 *)(boss + 0x3F0) <= 0) {
            *(s32 *)(boss + 0x3F4) = 0;
            *(u16 *)(boss + 8) = 0x50;
        }
        *(s32 *)(boss + 0x3F8) += 0x4000;
        *(s32 *)(boss + 0x3EC) += *(s32 *)(boss + 0x3F0);
        *(s32 *)(boss + 0x3F0) += *(s32 *)(boss + 0x3F4);
        *(s16 *)(boss + 0x26A) = -*(s32 *)(boss + 0x3F8) / 0x10000;
        *(s16 *)(boss + 0x26C) = *(s32 *)(boss + 0x3F8) / 0x10000;
        for (i = 0x10; i < 0x13; i++) {
            ((s16 *)(boss + 0x278))[i] = *(s32 *)(boss + 0x3EC) / 0x10000;
        }
        break;
    case 0x50:
        *(u16 *)(boss + 0x366) = 0;
        *(u16 *)(boss + 8) = 0x51;
        break;
    case 0x51:
        if (*(u16 *)(tt_boss + 0x366) >= 0x20) {
            *(u16 *)(boss + 0x366) = 0;
            *(u16 *)(boss + 8) = 0x52;
        }
        break;
    case 0x52:
        *(u16 *)(boss + 0x366) = 0;
        *(u16 *)(boss + 0x3D4) = 0;
        *(u16 *)(boss + 0x3D6) = 1;
        *(u16 *)(boss + 0x3D8) = 2;
        *(u16 *)(boss + 8) = 0x53;
        break;
    case 0x53:
        if (*(u8 *)(tt_boss + 0x17E) == 0x12 && !(*(u16 *)(tt_boss + 0x366) & 7)) {
            tt_se_request(0x513);
        }
        if (boss[0x17E] == 0x12 && (*(u16 *)(boss + 0x366) & 7) == 7) {
            point.x = (*(s16 *)(boss + 0x14) + ((s16 *)(boss + 0x268))[*(u16 *)(boss + 0x3D4)]) << 16;
            point.y = (*(s16 *)(boss + 0x16) + ((s16 *)(boss + 0x298))[*(u16 *)(boss + 0x3D4)]) << 16;
            angle = tt_angle_between((s32) &point, (s32) (other + 0x14));
            for (i = 0; i != 0x1000; i += 0x400) {
                tt_enemy_shot_fire_angle(*(s16 *)(boss + 0x14) + ((s16 *)(boss + 0x268))[*(u16 *)(boss + 0x3D4)],
                              *(s16 *)(boss + 0x16) + ((s16 *)(boss + 0x298))[*(u16 *)(boss + 0x3D4)], 0x18, angle + i, 0xA0);
                tt_enemy_shot_fire_angle(*(s16 *)(boss + 0x14) + ((s16 *)(boss + 0x268))[*(u16 *)(boss + 0x3D6)],
                              *(s16 *)(boss + 0x16) + ((s16 *)(boss + 0x298))[*(u16 *)(boss + 0x3D6)], 0x18, i, 0xA0);
                tt_enemy_shot_fire_angle(*(s16 *)(boss + 0x14) + ((s16 *)(boss + 0x268))[*(u16 *)(boss + 0x3D8)],
                              *(s16 *)(boss + 0x16) + ((s16 *)(boss + 0x298))[*(u16 *)(boss + 0x3D8)], 0x18, i + 0x200, 0xA0);
            }
        }
        tt_boss4_part_animation_cycle();
        *(u16 *)(boss + 0x3D4) += 1;
        if (*(u16 *)(boss + 0x3D4) >= 3) {
            *(u16 *)(boss + 0x3D4) = 0;
        }
        *(u16 *)(boss + 0x3D6) += 1;
        if (*(u16 *)(boss + 0x3D6) >= 3) {
            *(u16 *)(boss + 0x3D6) = 0;
        }
        *(u16 *)(boss + 0x3D8) += 1;
        if (*(u16 *)(boss + 0x3D8) >= 3) {
            *(u16 *)(boss + 0x3D8) = 0;
        }
        if (*(u16 *)(boss + 0x366) >= 0x180) {
            *(u16 *)(boss + 8) = 0x80;
        }
        break;
    case 0x80:
        tt_se_request(0x50A);
        boss[0x343] = 1;
        for (i = 0x10; i < 0x13; i++) {
            ((TtPartAnimation *)(boss + 0x38))[i].set = 4;
            ((TtPartAnimation *)(boss + 0x38))[i].frame = 0x34;
            ((TtPartAnimation *)(boss + 0x38))[i].flip = 0;
            ((TtPartAnimation *)(boss + 0x38))[i].flags = 0x80;
        }
        *(u16 *)(boss + 0x366) = 0;
        *(u16 *)(boss + 8) = 0x81;
        break;
    case 0x81:
        for (i = 0x10; i < 0x13; i++) {
            tt_boss_part_animation_step(tt_boss4_part_script, i);
        }
        if (*(u16 *)(boss + 0x366) >= 0x40) {
            *(u16 *)(boss + 8) = 0xA0;
        }
        break;
    case 0xA0:
        for (i = 0x10; i < 0x13; i++) {
            boss[i + 0x310] = 0;
        }
        *(s32 *)(boss + 0x3E8) = 0;
        *(u16 *)(boss + 6) = 0;
        *(u16 *)(boss + 8) = 0;
        *(s32 *)(boss + 0x3E4) = *(s32 *)(boss + 0x20);
        break;
    }
    *(u16 *)(boss + 0x366) += 1;
}

/* T-11520/T-11550: matched once the case-0x40 tables are read as x[i]/y[i] in counted loops (i < 5, i < 4): uopt strength-reduces them to the same pointer walk; with i shared by those loops (likely a lower priority for i) the angle loop keeps 0x900 (n) in s1 and angle + i in s0 and spills i, as in the original. The stack layout needs n and i declared after the TtPositionView. */
void tt_boss4_form1_step0_beams(void) {
    u16 mode;
    u8 *player;
    TtBossView *boss;
    s32 *x;
    s32 *y;
    s32 angle;
    TtPositionView point;
    s32 n;
    s32 i;

    mode = ((TtBossView *) tt_boss)->attack_step;
    player = tt_player;
    boss = (TtBossView *) tt_boss;
    switch (mode) {
    case 0:
        boss->phase_timer = 0;
        boss->attack_step = 0x10;
        break;
    case 0x10:
        if (((TtBossView *) tt_boss)->phase_timer >= 0x3C) {
            boss->attack_step = 0x40;
        }
        break;
    case 0x40:
        if (boss->phase_timer < 0x80) {
            if (((TtBossView *) player)->y < 0x78) {
                boss->phase_timer = 0x80;
            }
            if ((boss->phase_timer & 0x1F) == 0x1F) {
                tt_se_request(0x500);
                if (((u8 *) boss)[0x106] == 0) {
                    x = tt_boss4_shot_x_a;
                    y = tt_boss4_shot_y_a;
                    for (i = 0; i < 5; i++) {
                        tt_enemy_shot_fire_beam(boss->x - x[i], y[i] + boss->y);
                        tt_enemy_shot_fire_beam(x[i] + boss->x, y[i] + boss->y);
                    }
                } else {
                    x = tt_boss4_shot_x_b;
                    y = tt_boss4_shot_y_b;
                    for (i = 0; i < 4; i++) {
                        tt_enemy_shot_fire_beam(boss->x - x[i], y[i] + boss->y);
                        tt_enemy_shot_fire_beam(x[i] + boss->x, y[i] + boss->y);
                    }
                }
            }
        } else if (boss->phase_timer < 0x100) {
            if ((boss->phase_timer & 1) == 1) {
                if ((boss->phase_timer & 3) == 3) {
                    tt_se_request(0x50D);
                }
                tt_enemy_shot_fire_angle(boss->x, boss->y + 8, 0x30, ((boss->phase_timer & 0x7F) << 6) - 0x400, 0xA1);
            }
        } else if ((boss->phase_timer & 0x3F) == 0x3F) {
            tt_se_request(0x50B);
            point.x = boss->x << 16;
            point.y = (boss->y + 8) << 16;
            angle = tt_angle_between((s32) &point, (s32) (player + 0x14));
            angle &= 0x7F;
            for (i = -0x100, n = 0x900; i != n; i += 0x80) {
                tt_enemy_shot_fire_angle(boss->x, boss->y + 8, 0x48, angle + i, 0xA0);
            }
        }
        if (boss->phase_timer >= 0x180) {
            boss->attack_step = 0xA0;
        }
        break;
    case 0xA0:
        boss->phase = 1;
        boss->attack_step = 0;
        break;
    }
    boss->phase_timer++;
}

/* T-11540/T-11550: matched once case 0x52 reads +0x3EC/+0x3F0 back from memory for the /0x10000 stores (the vx/vy copies took a register and spilled one more word) and pt is declared after vy, which puts it at sp+56. */
void tt_boss4_form1_step2_body_throw(void) {
    u8 *sys;
    u8 *boss;
    u8 *player;
    u16 mode;
    s32 t; /* FAKE: unused, needed for the frame size. T-11510 */
    s32 vx; /* FAKE: unused, needed for the frame layout (pt at sp+56). T-11550 */
    s32 vy; /* FAKE: unused, needed for the frame layout (pt at sp+56). T-11550 */
    TtPositionView pt;
    s16 x;
    s16 y;
    boss = tt_boss;
    mode = *(u16 *)(boss + 8);
    sys = tt_sys;
    player = tt_player;
    switch (mode) {
    case 0:
        tt_se_request(0x510);
        *(u8 **)(boss + 0x204) = tt_sprite_set_table;
        boss[0x208] = 0;
        boss[0x209] = 4;
        boss[0x20A] = 0x41;
        boss[0x20C] = 0x60;
        boss[0x20F] = 0xF;
        boss[0x214] = 1;
        boss[0x327] = 1;
        *(s16 *)(boss + 0x2D6) = 0xA;
        *(s16 *)(boss + 0x276) = 0;
        *(s16 *)(boss + 0x2A6) = 8;
        boss[0x20E] = 0;
        *(s16 *)(boss + 0x210) = 0;
        *(s16 *)(boss + 0x212) = 0;
        *(s32 *)(boss + 0x3EC) = 0x80000;
        boss[0x344] = 0;
        *(s16 *)(boss + 8) = 0x40;
        return;
    case 0x40:
        if (*(u16 *)(tt_sys + 8) & 3) {
            boss[0x327] = 1;
        } else {
            boss[0x327] = 0;
        }
        boss[0x20E] = 0;
        *(s16 *)(boss + 0x210) += 0x20;
        *(s16 *)(boss + 0x212) += 0x20;
        *(s32 *)(boss + 0x3EC) += 0x8000;
        *(s16 *)(boss + 0x2A6) = *(s32 *)(boss + 0x3EC) / 0x10000;
        if (*(s16 *)(boss + 0x212) >= 0x1000) {
            *(s16 *)(boss + 8) = 0x50;
            boss[0x214] = 0;
            *(u16 *)(boss + 0x368) = 0;
            return;
        }
        return;
    case 0x50:
        if (*(u16 *)(tt_sys + 8) & 3) {
            boss[0x327] = 1;
        } else {
            boss[0x327] = 0;
        }
        boss[0x20E] = 0;
        if (*(u16 *)(boss + 0x368) >= 0x3C) {
            *(s16 *)(boss + 8) = 0x51;
            return;
        }
        *(u16 *)(boss + 0x368) += 1;
        return;
    case 0x51:
        tt_se_request(0x60C);
        if (*(u16 *)(sys + 8) & 3) {
            boss[0x327] = 1;
        } else {
            boss[0x327] = 0;
        }
        boss[0x20E] = 0;
        pt.x = *(s16 *)(boss + 0x14) << 16;
        pt.y = (*(s16 *)(boss + 0x16) + 0x48) << 16;
        tt_boss4_aim_angle = tt_angle_between((s32) &pt, (s32) (player + 0x14));
        *(s32 *)(boss + 0x3EC) = *(s16 *)(boss + 0x276) << 16;
        *(s32 *)(boss + 0x3F0) = *(s16 *)(boss + 0x2A6) << 16;
        *(s16 *)(boss + 8) = 0x52;
        return;
    case 0x52:
        if (*(u16 *)(tt_sys + 8) & 3) {
            boss[0x327] = 1;
        } else {
            boss[0x327] = 0;
        }
        boss[0x20E] = 0;
        *(s32 *)(boss + 0x3EC) += func_800A0140(tt_boss4_aim_angle) * 0x1C;
        *(s32 *)(boss + 0x3F0) += rsin(tt_boss4_aim_angle) * 0x1C;
        *(s16 *)(boss + 0x276) = *(s32 *)(boss + 0x3EC) / 0x10000;
        *(s16 *)(boss + 0x2A6) = *(s32 *)(boss + 0x3F0) / 0x10000;
        x = *(s16 *)(boss + 0x14) + *(s16 *)(boss + 0x276);
        y = *(s16 *)(boss + 0x16) + *(s16 *)(boss + 0x2A6);
        if (y >= 0x139 || y < -0x48 || x < -0xC8 || x >= 0xC9) {
            boss[0x327] = 0;
            *(s16 *)(boss + 8) = 0xA0;
            return;
        }
        break;
    case 0xA0:
        boss[0x344] = 1;
        *(s32 *)(boss + 0x3E8) = 0;
        *(s16 *)(boss + 6) = 0;
        *(u16 *)(boss + 8) = 0;
        *(s32 *)(boss + 0x3E4) = *(s32 *)(boss + 0x20);
        break;
    }
}

void tt_boss4_attack(void) {
    switch (*(u16 *)(tt_boss + 0x30E)) {
    case 0:
        switch (*(u16 *)(tt_boss + 6)) {
        case 0:
            tt_boss4_form0_step0_aimed_fan();
            return;
        case 2:
            tt_boss4_form0_step2_drone_rings();
            return;
        }
        break;
    case 1:
        switch (*(u16 *)(tt_boss + 6)) {
        case 0:
            tt_boss4_form1_step0_beams();
            return;
        case 2:
            tt_boss4_form1_step2_body_throw();
            return;
        }
        break;
    }
}

void tt_boss4_update(void) {
    u8 *core;
    u8 *stage_script;
    u8 *boss;
    u8 *player;

    boss = tt_boss;
    core = tt_core;
    player = tt_player;
    switch (*(u16 *)(boss + 4)) {
    case 0:
        tt_field_enemies_destroy();
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
        *(s16 *)(core + 0x32) = 0;
        *(s16 *)(core + 0x34) = 0x1000;
        tt_boss4_init();
        *(s16 *)(boss + 4) = 0x20;
        break;
    case 0x20:
        if (*(s32 *)tt_run == 0) {
            core[0x1C] = 0x10;
        }
        tt_boss4_enter();
        tt_boss4_part_animation();
        break;
    case 0x30:
        if (*(s32 *)tt_run == 0) {
            core[0x1C] = 0x10;
        }
        tt_boss4_intro_animation();
        tt_boss4_part_animation();
        break;
    case 0x40:
        if (*(s32 *)tt_run == 0) {
            core[0x1C] = 0x10;
        }
        tt_boss4_move();
        tt_boss4_part_animation();
        tt_boss4_attack();
        if (*(s16 *)(boss + 0x30C) < 0) {
            switch (*(u16 *)(boss + 0x30E)) {
            case 0:
                if (*(u16 *)(boss + 6) == 2) {
                    if (*(u16 *)(boss + 8) < 0x80) {
                        *(u16 *)(boss + 8) = 0x80;
                    }
                } else {
                    *(u16 *)(boss + 0x30E) = 0x10;
                    *(u16 *)(boss + 0x366) = 0;
                    *(u16 *)(boss + 8) = 0;
                }
                break;
            case 1:
                boss[0x327] = 0;
                boss[0x344] = 1;
                tt_boss_hitboxes_clear();
                /* FAKE: zero-store chains (TT house idiom); their dead reloads keep the constant 1 in a0. T-11860 */
                *(u16 *)(boss + 6) = *(u16 *)(boss + 8) = *(u16 *)(boss + 0xA) = 0;
                boss[0xD] = boss[0xE] = boss[0xF] = 0;
                *(s16 *)(boss + 4) = 0x50;
                break;
            case 0x10:
                if (*(u16 *)(boss + 0x366) >= 0x3C) {
                    *(s16 *)(boss + 0x30C) = 0x100;
                    *(u16 *)(boss + 0x30E) = 1;
                } else {
                    *(u16 *)(boss + 0x366) += 1;
                }
                break;
            }
        }
        break;
    case 0x50:
        tt_boss4_form_change();
        tt_boss4_part_animation();
        break;
    case 0x80:
        audio_command(0xC1);
        player[0x52] = 1;
        core[0x1C] = 0x11;
        tt_boss4_death_sequence();
        break;
    case 0xA0:
        audio_command(0xB1);
        boss[0] = 0;
        core[0x12] = 0;
        stage_script = (u8 *)tt_stage_script;
        *(u16 *)(stage_script + 0x1E) = 1;
        *(s16 *)(core + 2) = 0xA0;
        break;
    }
    tt_boss4_collision();
}
