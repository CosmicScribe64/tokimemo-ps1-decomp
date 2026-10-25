/* player_commands_and_club_moves.c: src/ovl/RPG_BAT/8013CC90.c:7: player command handling, move dispatch, common move parts; body 10: rpg_command_menu_update(); 11: if (rpg_player_ready_delay(0) < rpg_player_ready_timer) {. Original object 0x8013CC90-0x8013D780 (config/objects/RPG_BAT.txt). */
#define RPG_BAT_OVERRIDE_rpg_player_action /* unsigned view: the request byte is read with srl 24 and shared across the flag tests (RPG_BAT.h: s32), T-17117 */
#include "common.h"
#include "ovl/rpg_battle.h"

extern u32 rpg_player_action;

void rpg_player_update(void) {
    s32 sel;

    rpg_command_menu_update();
    if (rpg_player_ready_delay(0) < rpg_player_ready_timer) {
        rpg_player_action |= RPG_PLAYER_READY;
        rpg_player_ready_timer = 0;
    }
    if (!(rpg_player_action & 0xFF) && !(rpg_enemy_action & 0xFF) && !(rpg_over & 0xFF) && !(rpg_girl_action & 0xFF) && ((rpg_player_action >> 0x18) != 0)) {
        rpg_player_pose_idle();
        /* FAKE: u32 view through the address; a plain read takes $v0 for the lui constant where the original has $v1; source unknown. T-17117 */
        switch (*(u32 *)&rpg_player_action & 0xFF000000) {
        case 0x1000000:
            rpg_player_action = RPG_PLAYER_ATTACK;
            if (rpg_encounter == 0x80000000) {
                rpg_girl_channel.unk_00 += 0x258;
            }
            rpg_state.special_gauge += rpg_rand(0xF) + 0x1E;
            break;
        case 0x2000000:
            rpg_player_action = RPG_PLAYER_GUARD;
            rpg_state.special_gauge += rpg_rand(0xF) + 0x14;
            break;
        case 0x4000000:
            rpg_player_action = RPG_PLAYER_SPECIAL;
            rpg_special_ready = 0;
            rpg_state.special_gauge -= 0x64;
            break;
        }
        if (rpg_state.special_gauge >= 0xAB) {
            rpg_state.special_gauge = 0xAA;
        }
        if ((rpg_state.special_gauge >= 0x64) && (rpg_club_move_available == 1)) {
            rpg_special_ready = 1;
        }
    }
    switch (sel = rpg_player_action & 0x1FF) {
    case 0x100:
        rpg_player_guard();
        sel = rpg_player_action & 0x1FF;
        break;
    case 0x104:
        rpg_player_guard();
        sel = rpg_player_action & 0x1FF;
        break;
    case 0x1:
        rpg_player_attack();
        sel = rpg_player_action & 0x1FF;
        break;
    case 0x2:
        rpg_club_move_dispatch();
        sel = rpg_player_action & 0x1FF;
        break;
    }
    if ((sel == 0) && !(rpg_enemy_action & 0xFF) && !(rpg_girl_action & 0xFF) && !(rpg_over & 0xFF) && (!(game_state.pad_held & PAD_L1) || !(game_state.pad_held & PAD_R1) || !(rpg_encounter & 0xF0000))) {
        rpg_player_pose_idle();
        sel = rpg_player_action & 0x1FF;
    }
    if (sel == 0) {
        if (!(rpg_enemy_action & 0xFF) && !(rpg_over & 0xFF)) {
            if (rpg_encounter & 0xF0000) {
                if ((game_state.pad_held & PAD_L1) && (game_state.pad_held & PAD_R1)) {
                    if (((Actor *)actor_pool)[144].sequence_slot != 2) {
                        rpg_actor_set_animation(0x30, 2, 1, 1);
                    }
                } else {
                    rpg_player_pose_idle();
                }
            }
        }
    }
}

void rpg_player_pose_idle(void) {
    if (rpg_player_hit_points == 0) {
        rpg_actor_set_animation(0x30, 7, 1, 1);
        return;
    }
    /* FAKE: absolute address instead of rpg_player_action; the original's lui and lw use different registers. Real source unknown. T-4010 */
    if (*(s32 *)0x8015ED98 & 0x100) {
        rpg_actor_set_animation(0x30, 3, 1, 1);
        return;
    }
    rpg_actor_set_animation(0x30, 0, 1, 1);
    if (rpg_player_hit_points < 0xA) {
        rpg_actor_set_animation(0x30, 4, 1, 1);
    }
}

void rpg_club_move_dispatch(void) {
    switch (rpg_player_club) {
    case 0:
        rpg_club_move_literature();
        return;
    case 1:
        rpg_club_move_drama();
        return;
    case 2:
        rpg_club_move_science();
        return;
    case 3:
        rpg_club_move_computer();
        return;
    case 4:
        rpg_club_move_art();
        return;
    case 5:
        rpg_club_move_brass_band();
        return;
    case 6:
        rpg_club_move_baseball();
        return;
    case 7:
        rpg_club_move_soccer();
        return;
    case 8:
        rpg_club_move_tennis();
        return;
    case 9:
        rpg_club_move_swimming();
        return;
    case 10:
        rpg_club_move_basketball();
        return;
    case 11:
        rpg_club_move_no_club();
        return;
    }
}

void rpg_blink_move_hero_and_girl_60(void) {
    if (!(rpg_player_channel.unk_04 & 1)) {
        D_80121423 |= 0x80;
        if (rpg_girl_id != 0xE) {
            D_80120E07 |= 0x80;
        }
    } else {
        D_80121423 &= ~0x80;
        if (rpg_girl_id != 0xE) {
            D_80120E07 &= ~0x80;
        }
    }
    rpg_player_channel.unk_04++;
    if (rpg_player_channel.unk_04 >= 0x3C) {
        rpg_player_step_next();
    }
}

void rpg_blink_move_hero_and_girl_61(void) {
    if (!(rpg_player_channel.unk_04 & 1)) {
        D_80121423 |= 0x80;
        if (rpg_girl_id != 0xE) {
            D_80120E07 |= 0x80;
        }
    } else {
        D_80121423 &= ~0x80;
        if (rpg_girl_id != 0xE) {
            D_80120E07 &= ~0x80;
        }
    }
    rpg_player_channel.unk_04++;
    if (rpg_player_channel.unk_04 >= 0x3D) {
        rpg_player_step_next();
    }
}

void rpg_apply_damage_to_targets(void) {
    if (rpg_enemy_action & 0x100000) {
        rpg_apply_damage(0, 0, rpg_damage, 1);
        rpg_apply_damage(1, 1, rpg_damage_slot1, 1);
        rpg_apply_damage(2, 2, rpg_damage_slot2, 1);
        return;
    }
    rpg_apply_damage(0, rpg_enemy_slot, rpg_damage, 1);
}

void rpg_apply_damage_single(void) {
    rpg_apply_damage(0, rpg_enemy_slot, rpg_damage, 1);
}

void rpg_move_approach(void) {
    switch (rpg_player_channel.unk_00) {
    case 0:
        rpg_actor_set_position(0x30, 0, 0);
        rpg_actor_set_animation(0x30, 1, 1, 1);
        rpg_player_substep_next();
        return;
    case 1:
        rpg_actor_set_position(0x30, -rpg_player_channel.unk_08, 0);
        rpg_player_channel.unk_08 += 2;
        if (rpg_player_channel.unk_08 >= 0x21) {
            rpg_actor_set_animation(0x30, 0, 1, 0);
            rpg_actor_set_position(0x34, 0, 0);
            if (rpg_enemy_action & 0x600000) {
                D_80121426 = 0x82;
                rpg_actor_set_animation(0x34, 1, 5, 1);
            } else {
                D_80121426 = 0x82;
                rpg_actor_set_animation(0x34, 0, 5, 1);
            }
            rpg_player_substep_next();
            return;
        }
        return;
    case 2:
        rpg_player_channel.unk_08 += 1;
        if (rpg_player_channel.unk_08 == 1) {
            rpg_se_play_latched_5xx(0x503, 1, 0);
        }
        rpg_player_wait_animation_substep(0x34);
        return;
    case 3:
        D_80121426 = 1;
        rpg_player_step_next();
        break;
    }
}

void rpg_move_return(void) {
    switch (rpg_player_channel.unk_00) {
    case 0:
        if (rpg_enemy_action & 0x600000) {
            rpg_actor_set_animation(0x34, 1, 0x21, 1);
            D_80121426 = 0x82;
            D_80121438 = 0xF;
        } else {
            rpg_actor_set_animation(0x34, 0, 0x21, 1);
            D_80121426 = 0x82;
            D_80121438 = 0xF;
        }
        rpg_actor_set_position(0x34, 0, 0);
        rpg_player_substep_next();
        return;
    case 1:
        rpg_player_wait_animation_substep(0x34);
        return;
    case 2:
        D_80121426 = 1;
        rpg_actor_set_animation(0x34, 0, 1, 0);
        rpg_actor_set_animation(0x30, 0, 1, 1);
        rpg_player_substep_next();
        return;
    case 3:
        rpg_player_wait_substep(0x14);
        return;
    case 4:
        rpg_actor_set_animation(0x30, 2, 1, 1);
        rpg_player_substep_next();
        return;
    case 5:
        rpg_actor_set_position(0x30, rpg_player_channel.unk_04 - 0x20, 0);
        rpg_player_channel.unk_04 += 2;
        if (rpg_player_channel.unk_04 >= 0x21) {
            rpg_actor_set_position(0x30, 0, 0);
            rpg_actor_set_animation(0x30, 0, 1, 1);
            rpg_player_substep_next();
            return;
        }
    default:
        return;
    case 6:
        rpg_player_wait_substep(0x14);
        return;
    case 7:
        rpg_player_step_next();
        break;
    }
}
