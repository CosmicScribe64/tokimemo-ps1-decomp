/* club_move_soccer.c: src/ovl/RPG_BAT/801484E0.c:4: club 7 special move; body 5: switch (rpg_player_step) {; 8: rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x5A); club-index dispatch src/ovl/RPG_BAT/8013CC90.c:96. Original object 0x801484E0-0x80148F70 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/rpg_battle.h"

void rpg_club_move_soccer(void) {
    switch (rpg_player_step) {
    case 0:
        strcpy(rpg_text_player_0, "　　　　　レッドカード乱舞　　　　　");
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
        rpg_soccer_step_3();
        return;
    case 4:
        rpg_actor_set_animation(0x34, 3, 1, 1);
        rpg_player_step_next();
        return;
    case 5:
        rpg_player_wait_step(0x20);
        return;
    case 6:
        rpg_actor_set_animation(0x34, 5, 5, 1);
        rpg_player_step_next();
        return;
    case 7:
        rpg_player_wait_animation_step(0x34);
        return;
    case 8:
        rpg_soccer_step_8();
        return;
    case 9:
        rpg_soccer_step_9();
        return;
    case 10:
        rpg_player_wait_step(0x14);
        return;
    case 11:
        rpg_move_return();
        return;
    case 12:
        rpg_player_action_end();
        /* fallthrough */
    default:
        return;
    }
}

void rpg_soccer_step_3(void) {
    switch (rpg_player_channel.unk_00) {
    case 0:
        rpg_actor_set_animation(0x34, 4, 5, 1);
        rpg_se_play_latched_6xx(0x506, 1, 0);
        rpg_player_substep_next();
        break;
    case 1:
        if (!(D_80121422 & 1)) {
            rpg_actor_set_animation(0x34, 4, 5, 1);
            rpg_player_channel.unk_08 += 1;
        }
        if (rpg_player_channel.unk_08 >= 0xE) {
            rpg_player_step_next();
        }
        break;
    }
}

/* FAKE: the rpg_enemy_slot and rpg_move_target_actor tables are reached through their first symbol (aggregates not typed yet); m views the rpg_motion words as RpgMoveMotion, as rpg_soccer_step_9 does. T-17113, T-22013 */
void rpg_soccer_step_8(void) {
    s32 i;
    s32 start;
    s32 *r;
    s32 x;
    s32 y;
    RpgMoveMotion *m = (RpgMoveMotion *)&rpg_motion;

    switch (rpg_player_channel.unk_00) {
    case 0:
        /* FAKE: T-22013, rpg_target_x/2C/50/54 are words 31, 32, 41, 42 of the table at rpg_enemy_slot; as1 keeps the loads behind the first store only when all four are one symbol (aggregate not typed yet) */
        (&rpg_enemy_slot)[41] = (&rpg_enemy_slot)[31] - 0xB0;
        (&rpg_enemy_slot)[42] = (&rpg_enemy_slot)[32] - 0x58;
        if (rpg_enemy_slot == 1) {
            D_801215FD = 4;
            D_80121685 = 4;
        }
        if (rpg_enemy_slot == 0) {
            D_80121685 = 4;
        }
        rpg_se_play_latched_6xx(0x507, 1, 0);
        rpg_player_substep_next();
        return;
    case 1:
        start = 0;
        if (rpg_player_channel.unk_04 >= 0x3E8) {
            start = 1;
        } else {
            /* FAKE: redundant else; the original has the branch over an empty else (T-17113) */
            start = 0;
        }
        i = start;
        if (start < 6) {
            r = &(&rpg_enemy_slot)[start * 2];
            do {
                m->rect.param_4 = r[50] * m->dx / 1000;
                m->rect.param_5 = r[50] * m->dy / 1000;
                rpg_actor_set_position(i + 0x32, m->rect.param_4, m->rect.param_5);
                rpg_actor_set_animation(i + 0x32, 6, 1, 1);
                i++;
                r += 2;
            } while (i != 6);
        }
        if (rpg_player_channel.unk_04 < 0x3E8) {
            if (rpg_enemy_slot == 0) {
                rpg_player_channel.unk_04 += 0x3C;
            } else {
                rpg_player_channel.unk_04 += 0x28;
            }
            rpg_scratch = rpg_player_channel.unk_04;
        }
        for (i = 0; i < 0x31; i++) {
            (&rpg_voice_playing_seen)[-i] = (&rpg_ripple_width_2)[-i];
        }
        if (rpg_move_limit == rpg_scratch) {
            for (i = 1; i < 6; i++) {
                rpg_actor_set_animation(i + 0x32, 6, 1, 0);
            }
        }
        if (rpg_player_channel.unk_04 >= 0x3E8 && rpg_player_channel.unk_04 < 0x7D0) {
            rpg_actor_set_position(0x32, rpg_state.target_x, rpg_state.target_y);
            rpg_actor_set_animation_alt(0x32, 7, 5, 1);
            rpg_player_channel.unk_04 = 0x7D1;
        } else if (!(D_8012139A & 1)) {
            rpg_player_channel.unk_04 = 0x3E8;
            rpg_player_channel.unk_08 += 1;
        }
        if (rpg_player_channel.unk_04 >= 0x3E8) {
            rpg_se_play_latched_6xx(0x508, 0xA, 1);
            for (i = 0; i < 4; i++) {
                if (rpg_rand(0x14) == 0) {
                    x = rpg_rand(0x20);
                    y = rpg_rand(0x20);
                    rpg_actor_set_position(i + 0x28, x + rpg_state.target_x - 0x10, y + rpg_state.target_y - 0x10);
                    rpg_actor_set_animation_alt(i + 0x28, 8, 1, 1);
                }
            }
        }
        if (rpg_player_channel.unk_08 >= 8) {
            for (i = 0; i < 4; i++) {
                rpg_actor_set_animation_alt(i + 0x28, 0xFF, 1, 0);
            }
            rpg_player_step_next();
        }
        return;
    }
}

void rpg_soccer_step_9(void) {
    RpgMoveMotion *motion = (RpgMoveMotion *)&rpg_motion;

    switch (rpg_player_channel.unk_00) {
    case 0:
        switch (rpg_enemy_slot) {
        case 0:
            motion->rect.param_4 = 0x98;
            motion->rect.param_5 = 0x68;
            break;
        case 1:
            motion->rect.param_4 = 0x60;
            motion->rect.param_5 = 0x58;
            break;
        case 2:
            motion->rect.param_4 = 0x58;
            motion->rect.param_5 = 0x88;
            break;
        case 3:
        case 4:
            motion->rect.param_4 = 0x80;
            motion->rect.param_5 = 0x80;
            break;
        }
        rpg_actor_set_position(0x32, motion->rect.param_4, motion->rect.param_5);
        rpg_actor_set_animation_alt(0x32, 9, 1, 1);
        rpg_player_substep_next();
    default:
        break;
    case 1:
        rpg_player_wait_substep(0xA);
        break;
    case 2:
        if (rpg_enemy_action & 0x100000) {
            rpg_actor_set_animation_alt(0x32, 0xB, 5, 1);
        } else {
            rpg_actor_set_animation_alt(0x32, 0xA, 5, 1);
        }
        rpg_se_play_latched_6xx(0x507, 1, 0);
        rpg_player_substep_next();
        break;
    case 3:
        if (rpg_enemy_action & 0x100000) {
            if (D_801213B0 == 9) {
                rpg_se_play_latched_6xx(0x509, 0, 5);
                rpg_actor_set_position(0x28, motion->rect.param_4 - 0x23, motion->rect.param_5 - 0x14);
                rpg_actor_set_animation_alt(0x28, 8, 5, 1);
            }
        } else if (D_801213B0 == 0xE) {
            rpg_se_play_latched_6xx(0x509, 0, 5);
            if (rpg_enemy_action & 0x08000000) {
                rpg_actor_set_position(0x28, 0x48, 0x38);
            } else {
                rpg_actor_set_position(0x28, 0x4D, 0x24);
            }
            rpg_actor_set_animation_alt(0x28, 8, 5, 1);
        }
        if (!(D_801210F2 & 1)) {
            rpg_actor_set_animation_alt(0x28, 8, 1, 0);
        }
        if (!(D_8012139A & 1)) {
            rpg_apply_damage_single();
            rpg_player_substep_next();
        }
        break;
    case 4:
        motion->dx = 0x105 - motion->rect.param_4;
        motion->dy = 0x56 - motion->rect.param_5;
        rpg_actor_set_position(0x32, ((s32) (motion->dx * rpg_player_timer_2) / 1000) + motion->rect.param_4, (motion->rect.param_5 + ((s32) (motion->dy * rpg_player_timer_2) / 1000)) - (rsin((s32) (rpg_player_timer_2 << 0xB) / 1000) >> 7));
        rpg_player_timer_2 += 0x14;
        rpg_actor_set_animation_alt(0x32, 0xC, 1, 1);
        if (rpg_player_timer_2 >= 0x3E9) {
            rpg_se_play_latched_6xx(0x502, 1, 0);
            rpg_player_substep_next();
        }
        break;
    case 5:
        rpg_actor_set_animation_alt(0x32, 0xFF, 1, 0);
        rpg_actor_set_position(0x34, 0, 0);
        rpg_actor_set_animation(0x34, 2, 1, 1);
        rpg_player_step_next();
        break;
    }
}
