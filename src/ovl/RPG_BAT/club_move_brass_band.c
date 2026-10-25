/* club_move_brass_band.c: src/ovl/RPG_BAT/80134D30.c:7: club 5 special move; body 8: switch (rpg_player_step) {; 11: rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x5A); club-index dispatch src/ovl/RPG_BAT/player_commands_and_club_moves.c:96. Original object 0x80134D30-0x80135AE0 (config/objects/RPG_BAT.txt). */
#define RPG_BAT_OVERRIDE_rpg_actor_set_position /* implicit int view: the call's $v0 moves the D_8015EC68 - (h - D_8015EC6C) * 2 range of rpg_brass_band_step_6 to $v1 (T-17116) */
#include "common.h"
#include "ovl/RPG_BAT.h"

s32 rpg_actor_set_position();

void rpg_club_move_brass_band(void) {
    switch (rpg_player_step) {
    case 0:
        strcpy(rpg_text_player_0, "　　　　ハーメルンのホラ吹き　　　　");
        rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x5A);
        rpg_pick_target();
        rpg_set_target_position(rpg_enemy_slot);
        rpg_player_damage_roll();
        rpg_player_step_next();
        rpg_player_step_next();
        return;
    case 2:
        rpg_move_approach();
        return;
    case 3:
        rpg_brass_band_step_3();
        return;
    case 4:
        rpg_blink_move_hero_and_girl_60();
        return;
    case 5:
        rpg_brass_band_step_5();
        return;
    case 6:
        rpg_brass_band_step_6();
        return;
    case 7:
        rpg_apply_damage_to_targets();
        rpg_actor_set_animation(0x34, 2, 1, 0);
        rpg_player_step_next();
        return;
    case 8:
        rpg_blink_move_hero_and_girl_61();
        return;
    case 9:
        rpg_move_return();
        return;
    case 10:
        rpg_player_action_end();
        /* fallthrough */
    default:
        return;
    }
}

/* s32 without a return: rpg_club_move_brass_band calls this function earlier as an implicit int, a void definition does not build. */
/* FAKE: *(s32 *)&rpg_scratch stops IDO copypropagate from replacing the first read of rpg_scratch with 0x300 so uopt keeps rpg_scratch in $a1;
 * if (0) {} splits the basic block after r2 = r + 0x10 so uopt does not fold r + 0x10 into the +0x310 / -0x170 offsets; the words at
 * rpg_enemy_slot + 4 * (i + 60, 70, 80) are the same three 10-word rows as in rpg_brass_band_step_6. T-24124 */
s32 rpg_brass_band_step_5(void) {
    s32 r;
    s32 r2;
    s32 i;

    rpg_move_timer = -0x1C0;
    rpg_move_value_0 = 0x280;
    rpg_move_value_1 = -0x140;
    ((Actor *)actor_pool)[146].unk_01 = 2;
    ((Actor *)actor_pool)[147].unk_01 = 2;
    ((Actor *)actor_pool)[150].unk_01 = 8;
    ((Actor *)actor_pool)[151].unk_01 = 8;
    rpg_scratch = 0x300;
    rpg_actor_set_position(0x32, *(s32 *)&rpg_scratch, 0x88);
    rpg_actor_set_position(0x33, rpg_scratch, 0x88);
    rpg_actor_set_position(0x36, rpg_move_value_0, 0x50);
    rpg_actor_set_position(0x37, rpg_move_value_0, 0x50);
    rpg_actor_set_animation_alt(0x32, 6, 1, 1);
    rpg_actor_set_animation_alt(0x33, 5, 1, 1);
    rpg_actor_set_animation_alt(0x36, 6, 1, 1);
    rpg_actor_set_animation_alt(0x37, 5, 1, 1);
    r = rpg_rand(0x20);
    (&rpg_enemy_slot)[60] = 0x2F0;
    (&rpg_enemy_slot)[70] = -0x190;
    (&rpg_enemy_slot)[80] = 0x7D;
    r2 = r + 0x10;
    if (0) {}
    (&rpg_enemy_slot)[61] = r2 + 0x310;
    (&rpg_enemy_slot)[71] = r2 - 0x170;
    (&rpg_enemy_slot)[81] = 0x7A;
    r = rpg_rand(0x10) + 0x20;
    r2 = rpg_rand(0x10);
    (&rpg_enemy_slot)[62] = 0x2B0;
    (&rpg_enemy_slot)[72] = -0x190;
    (&rpg_enemy_slot)[82] = 0x6C;
    (&rpg_enemy_slot)[63] = r + 0x2C0;
    (&rpg_enemy_slot)[73] = r - 0x180;
    (&rpg_enemy_slot)[83] = 0x69;
    r2 += r;
    (&rpg_enemy_slot)[64] = r2 + 0x2F0;
    (&rpg_enemy_slot)[74] = r2 - 0x150;
    (&rpg_enemy_slot)[84] = 0x6F;
    r = rpg_rand(0x10) + 0x10;
    r2 = rpg_rand(0x10);
    (&rpg_enemy_slot)[65] = r + 0x2A0;
    (&rpg_enemy_slot)[75] = r - 0x160;
    (&rpg_enemy_slot)[85] = 0x61;
    r2 += r;
    (&rpg_enemy_slot)[66] = r2 + 0x2D0;
    (&rpg_enemy_slot)[76] = r2 - 0x130;
    (&rpg_enemy_slot)[86] = 0x5E;
    switch (rpg_enemy_slot) {
    case 0:
        rpg_move_x_start = 0x2F8;
        rpg_move_x_end = -0x148;
        rpg_move_extent = 0x64;
        D_8015EC68 = 0x77;
        D_8015EC6C = 0x3E;
        break;
    case 1:
        rpg_move_x_start = 0x2D0;
        rpg_move_x_end = -0x130;
        rpg_move_extent = 0x5B;
        D_8015EC68 = 0x3F;
        D_8015EC6C = 0x2E;
        break;
    case 2:
        rpg_move_x_start = 0x310;
        rpg_move_x_end = -0x170;
        rpg_move_extent = 0x85;
        D_8015EC68 = 0x37;
        D_8015EC6C = 0x5E;
        break;
    case 3:
        rpg_move_x_start = 0x310;
        rpg_move_x_end = -0x170;
        rpg_move_extent = 0x85;
        D_8015EC68 = 0x4C;
        D_8015EC6C = 0xE;
        break;
    case 4:
        rpg_move_x_start = 0x310;
        rpg_move_x_end = -0x170;
        rpg_move_extent = 0x85;
        D_8015EC68 = 0x54;
        D_8015EC6C = 0x2A;
        break;
    }
    for (i = 0; i != 8; i++) {
        rpg_actor_set_animation_alt(i + 0x28, 5, 1, 1);
        rpg_actor_set_position(i + 0x28, (&rpg_enemy_slot)[i + 60], (&rpg_enemy_slot)[i + 80]);
        ((Actor *)actor_pool)[i + 136].unk_18 = rpg_rand(4);
        ((Actor *)actor_pool)[i + 136].unk_08 = rpg_rand(7);
        if ((&rpg_enemy_slot)[i + 80] >= 0x85) {
            ((Actor *)actor_pool)[i + 136].unk_01 = 3;
        } else if ((&rpg_enemy_slot)[i + 80] >= 0x66) {
            ((Actor *)actor_pool)[i + 136].unk_01 = 4;
        } else if ((&rpg_enemy_slot)[i + 80] >= 0x64) {
            ((Actor *)actor_pool)[i + 136].unk_01 = 5;
        } else if ((&rpg_enemy_slot)[i + 80] >= 0x5D) {
            ((Actor *)actor_pool)[i + 136].unk_01 = 6;
        } else {
            ((Actor *)actor_pool)[i + 136].unk_01 = 7;
        }
    }
    if (rpg_enemy_action & 0x100000) {
        ((Actor *)actor_pool)[155].unk_01 = 5;
        ((Actor *)actor_pool)[156].unk_01 = 7;
        ((Actor *)actor_pool)[157].unk_01 = 3;
    } else {
        ((Actor *)actor_pool)[155].unk_01 = 3;
    }
    rpg_player_step_next();
}

/* s32 without a return: rpg_club_move_brass_band calls this function earlier as an implicit int, a void definition does not build. */
/* FAKE: h + -h is the unfolded (h - h) of the original (subu s1,s1: uopt folds a plain h - h); the words at rpg_enemy_slot + 4 * (i + 60,
 * 70, 80) are three 10-word rows (from x, to x, y) reached through the first symbol, the real aggregate is unknown. Frame size and the
 * spill slot at sp+0x34 match only with these three declared locals. T-17116 */
s32 rpg_brass_band_step_6(void) {
    s32 cx;
    s32 h;
    s32 sp34;

    if (rpg_player_channel.unk_04 >= 0x68 && rpg_player_channel.unk_04 < 0x450) {
        if ((rpg_player_channel.unk_04 & 7) == 0) {
            rpg_effects_shift(0, 1);
        } else {
            rpg_effects_shift(0, -1);
        }
    }
    if (rpg_player_channel.unk_04 >= 0x130 && rpg_player_channel.unk_04 < 0x3B8) {
        if ((rpg_player_channel.unk_04 & 7) == 0) {
            rpg_effects_shift(0, 1);
        } else {
            rpg_effects_shift(0, -1);
        }
    }
    if (rpg_player_channel.unk_04 >= 0x1F8 && rpg_player_channel.unk_04 < 0x320) {
        if ((rpg_player_channel.unk_04 & 7) == 0) {
            rpg_effects_shift(0, 1);
        } else {
            rpg_effects_shift(0, -1);
        }
    }
    rpg_se_play_latched_6xx(0x50A, 0x11, 1);
    rpg_actor_set_position(0x32, (rpg_move_timer - rpg_scratch) * rpg_player_channel.unk_04 / 1000 + rpg_scratch, 0x88);
    rpg_actor_set_position(0x33, (rpg_move_timer - rpg_scratch) * rpg_player_channel.unk_04 / 1000 + rpg_scratch, 0x88);
    rpg_actor_set_position(0x36, (rpg_move_value_1 - rpg_move_value_0) * rpg_player_channel.unk_04 / 1000 + rpg_move_value_0, 0x50);
    rpg_actor_set_position(0x37, (rpg_move_value_1 - rpg_move_value_0) * rpg_player_channel.unk_04 / 1000 + rpg_move_value_0, 0x50);
    for (h = 0; h != 8; h++) {
        rpg_actor_set_position(h + 0x28, (&rpg_enemy_slot)[h + 60] + ((&rpg_enemy_slot)[h + 70] - (&rpg_enemy_slot)[h + 60]) * rpg_player_channel.unk_04 / 1000, (&rpg_enemy_slot)[h + 80]);
    }
    cx = (rpg_move_x_end - rpg_move_x_start) * rpg_player_channel.unk_04 / 1000 + rpg_move_x_start;
    h = rpg_move_extent;
    if (cx >= D_8015EC68 && (h - D_8015EC6C) + D_8015EC68 >= cx) {
        rpg_se_play_latched_6xx(0x50C, 0, 3);
        rpg_actor_set_animation_alt(0x2F, 4, 1, 1);
        if (rpg_enemy_action & 0x100000) {
            rpg_actor_set_position(0x2F, cx, (h + -h + D_8015EC6C + cx) - D_8015EC68 - (rsin((cx - D_8015EC68) * 0x800 / (h - D_8015EC6C)) >> 7));
        } else {
            rpg_actor_set_position(0x2F, cx, (h + -h + D_8015EC6C + cx) - D_8015EC68 - (rsin((cx - D_8015EC68) * 0x800 / (h - D_8015EC6C)) >> 6));
        }
    }
    if (D_8015EC68 - (h - D_8015EC6C) * 2 < cx && cx < D_8015EC68) {
        sp34 = h + -h;
        rpg_se_play_latched_6xx(0x50B, 0, 2);
        rpg_actor_set_animation_alt(0x2F, 4, 1, 1);
        if (rpg_enemy_action & 0x100000) {
            rpg_actor_set_position(0x2F, cx, (sp34 + D_8015EC6C + (D_8015EC68 - cx) / 2) - (rsin((D_8015EC68 - cx) * 0x800 / ((h - D_8015EC6C) * 2)) >> 7));
        } else {
            rpg_actor_set_position(0x2F, cx, (sp34 + D_8015EC6C + (D_8015EC68 - cx) / 2) - (rsin((D_8015EC68 - cx) * 0x800 / ((h - D_8015EC6C) * 2)) >> 6));
        }
    }
    if (cx == D_8015EC68 - (h - D_8015EC6C) * 2) {
        rpg_actor_set_animation_alt(0x2F, 5, 1, 1);
    }
    rpg_player_channel.unk_04 += 4;
    if (rpg_player_channel.unk_04 >= 0x4B1) {
        rpg_player_step_next();
    }
}

void rpg_brass_band_step_3(void) {
    switch (rpg_player_channel.unk_00) {
    case 0:
        rpg_actor_set_animation(0x34, 3, 5, 1);
        rpg_player_substep_next();
        break;
    case 1:
        rpg_player_wait_substep(0xF);
        break;
    case 2:
        rpg_player_substep_next();
        break;
    case 3:
        rpg_player_wait_animation_substep(0x34);
        break;
    case 4:
        rpg_actor_set_animation(0x34, 7, 1, 1);
        rpg_se_play_latched_6xx(0x50D, 1, 0);
        rpg_player_substep_next();
        break;
    case 5:
        rpg_player_wait_substep(0x4B);
        break;
    case 6:
        rpg_player_step_next();
        break;
    }
}
