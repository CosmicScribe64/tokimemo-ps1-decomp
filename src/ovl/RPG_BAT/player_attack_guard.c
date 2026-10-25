/* player_attack_guard.c: src/ovl/RPG_BAT/8013AD50.c:4: basic attack and guard; body 5: if (!(rpg_girl_action & 0xFF)) {; 8: if (*(s32 *)0x8015ED98 & 0x100) {. Original object 0x8013AD50-0x8013B5B0 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/RPG_BAT.h"

void rpg_player_guard(void) {
    if (!(rpg_girl_action & 0xFF)) {
        /* FAKE: absolute address instead of rpg_player_action: the original's lui and lw use different registers
         * (as in rpg_player_pose_idle, T-4010). T-16210 */
        if (*(s32 *)0x8015ED98 & 0x100) {
            rpg_actor_set_animation(0x30, 3, 1, 1);
        } else {
            rpg_player_pose_idle();
        }
    }
    /* FAKE: rpg_player_timer reached through rpg_player_action_words (one base symbol keeps the switch load after the store,
     * T-4070); the real aggregate over rpg_player_action_words..rpg_player_timer_2 is not declared yet. T-16210 */
    rpg_player_action_words[3] += 1;
    switch (rpg_player_action_words[1]) {
    case 0:
        if (!(rpg_enemy_action & 0xFF) && !(rpg_girl_action & 0xFF)) {
            if (rpg_player_timer >= 0x97) {
                rpg_player_action = 0x104;
                /* FAKE: rpg_player_hit_points_maximum read through rpg_player_action so that as1 keeps the load after the store; the
                 * original probably had one aggregate over these words. T-16210 */
                rpg_player_guard_heal = (&rpg_player_action)[2] / 20;
                if (rpg_player_guard_heal == 0) {
                    rpg_player_guard_heal = 1;
                }
                strcpy(rpg_text_player_0, game_state.player_family_name);
                strcat(rpg_text_player_0, "の体力が５％回復した");
                rpg_message_show(&rpg_text_player_pointers, 1, 0x3C);
                rpg_player_step_next();
            }
        }
        break;
    case 1:
        rpg_player_wait_msg_step(1);
        break;
    case 2:
        rpg_apply_damage(0, 0x21, rpg_player_guard_heal, 0);
        rpg_se_play_latched_5xx(0x502, 1, 0);
        rpg_player_step_next();
        break;
    case 3:
        rpg_player_wait_step(0x14);
        break;
    case 4:
        rpg_player_action_end();
        rpg_player_ready_timer = rpg_player_ready_delay(1);
        break;
    }
}

void rpg_player_attack(void) {
    switch (rpg_player_step) {
    case 0:
        rpg_pick_target();
        rpg_set_target_position_alt(rpg_enemy_slot);
        rpg_player_damage_roll();
        rpg_actor_set_animation(0x30, 1, 1, 1);
        rpg_player_step_next();
        return;
    case 1:
        rpg_actor_set_position(0x30, -rpg_player_timer, 0);
        rpg_player_timer += 3;
        if (rpg_player_timer >= 0x21) {
            rpg_actor_set_position(0x30, -0x20, 0);
            rpg_actor_set_animation(0x30, 0, 1, 1);
            rpg_player_step_next();
            return;
        }
    default:
        return;
    case 2:
        strcpy(rpg_text_player_0, game_state.player_family_name);
        strcat(rpg_text_player_0, "の攻撃");
        if (rpg_damage == 0) {
            strcpy(rpg_text_player_1, rpg_text_player_0);
            strcat(rpg_text_player_1, "は失敗した");
        } else {
            rpg_message_format_damage(rpg_text_player_1, game_state.player_family_name, "は", rpg_damage, "のダメージを与えた");
        }
        rpg_message_show(&rpg_text_player_pointers, 2, 0x3C);
        rpg_message_mode = 3;
        rpg_actor_set_position(0x31, rpg_target_x, rpg_target_y);
        rpg_player_step_next();
        return;
    case 3:
        rpg_player_wait_step(0x19);
        return;
    case 4:
        if (rpg_damage < 0xC8) {
            rpg_scratch = 1;
        } else if (rpg_damage < 0x1F4) {
            rpg_scratch = 2;
        } else if (rpg_damage < 0x320) {
            rpg_scratch = 3;
        } else if (rpg_damage < 0x44C) {
            rpg_scratch = 4;
        } else if (rpg_damage < 0x514) {
            rpg_scratch = 5;
        } else if (rpg_damage == 0x270F) {
            rpg_scratch = 0xA;
        } else {
            rpg_scratch = 6;
        }
        D_80121316 = 0x82;
        D_8012135A = 0x82;
        rpg_player_step_next();
        return;
    case 5:
        if (rpg_damage == 0) {
            rpg_player_attack_miss_swing();
            return;
        }
        rpg_player_attack_hit_flurry();
        return;
    case 6:
        rpg_message_mode = 1;
        D_80121316 = 1;
        D_8012135A = 1;
        rpg_player_wait_msg_step(1);
        return;
    case 7:
        rpg_apply_damage(0, rpg_enemy_slot, rpg_damage, 1);
        rpg_actor_set_animation(0x30, 2, 1, 1);
        rpg_player_step_next();
        return;
    case 8:
        rpg_actor_set_position(0x30, rpg_player_timer - 0x20, 0);
        rpg_player_timer += 3;
        if (rpg_player_timer >= 0x21) {
            rpg_actor_set_position(0x30, 0, 0);
            rpg_actor_set_animation(0x30, 0, 1, 1);
            rpg_player_step_next();
            return;
        }
        break;
    case 9:
        rpg_player_wait_msg_step(0);
        return;
    case 10:
        rpg_player_wait_step(0x14);
        return;
    case 11:
        rpg_player_action_end();
        break;
    }
}

s32 rpg_player_attack_hit_flurry(void) {

    if (!(rpg_scratch & 1)) {
        switch (rpg_player_channel.unk_00) {
        case 0:
            rpg_actor_set_animation(0x30, 6, 5, 1);
            rpg_player_substep_next();
            return;
        case 1:
            /* FAKE: s32 view and dead test keep counter in v1; source unknown. T-9730. */
            if (!rpg_scratch) {
            }
            rpg_player_wait_substep(4);
            return;
        case 2:
            rpg_actor_set_animation_alt(0x31, 0xB, 5, 1);
            rpg_se_play_latched_5xx(0x501, 1, 0);
            rpg_player_substep_next();
            return;
        case 3:
            if (rpg_actor_is_idle(0x31) != 0) {
                rpg_actor_set_animation_alt(0x31, 0xFF, 1, 0);
            }
            if (rpg_actor_is_idle(0x30) != 0) {
                rpg_actor_set_animation_alt(0x31, 0xFF, 1, 0);
                if (--rpg_scratch == 0) {
                    rpg_player_step_next();
                    return;
                }
                rpg_player_channel.unk_00 = 0;
            }
            break;
        }
    } else {
        switch (rpg_player_channel.unk_00) {
        case 0:
            rpg_actor_set_animation(0x30, 6, 5, 1);
            rpg_player_substep_next();
            return;
        case 1:
            rpg_player_wait_substep(4);
            return;
        case 2:
            rpg_actor_set_animation_alt(0x31, 0xC, 5, 1);
            rpg_se_play_latched_5xx(0x501, 1, 0);
            rpg_player_substep_next();
            return;
        case 3:
            if (rpg_actor_is_idle(0x31) != 0) {
                rpg_actor_set_animation_alt(0x31, 0xFF, 1, 0);
            }
            if (rpg_actor_is_idle(0x30) != 0) {
                rpg_actor_set_animation_alt(0x31, 0xFF, 1, 0);
                if (--rpg_scratch == 0) {
                    rpg_player_step_next();
                    return;
                }
                rpg_player_channel.unk_00 = 0;
            } else {
                return;
            }
            break;
        }
    }
}

void rpg_player_attack_miss_swing(void) {
    switch (rpg_player_channel.unk_00) {
    case 0:
        rpg_actor_set_animation(0x30, 6, 5, 1);
        rpg_player_substep_next();
        break;
    case 1:
        if (rpg_actor_is_idle(0x30) != 0) {
            rpg_player_step_next();
        }
        break;
    }
}
