/* club_move_no_club.c: src/ovl/RPG_BAT/801498C0.c:7: club 11 special move; body 8: switch (rpg_player_step) {; 11: rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x5A); club-index dispatch src/ovl/RPG_BAT/8013CC90.c:96. Original object 0x801498C0-0x8014A660 (config/objects/RPG_BAT.txt). */
#define RPG_BAT_OVERRIDE_rpg_se_play_latched_6xx /* implicit int view: the call's $v0 keeps the compare-chain values in rpg_no_club_step_5 off $v0 (T-20600) */
#include "common.h"
#include "ovl/RPG_BAT.h"

s32 rpg_se_play_latched_6xx();

void rpg_club_move_no_club(void) {
    switch (rpg_player_step) {
    case 0:
        strcpy(rpg_text_player_0, "　　　　　　　気分最高　　　　　　　");
        rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x5A);
        rpg_sparks_park();
        rpg_pick_target();
        rpg_set_target_position_alt(rpg_enemy_slot);
        rpg_player_damage_roll();
        if (rpg_enemy_slot == 1) {
            D_801215FD = 4;
            D_80121685 = 4;
        }
        switch (rpg_escalation_counter) {
        default:
        case 0:
            rpg_scratch = 5;
            break;
        case 1:
            rpg_scratch = 6;
            break;
        case 2:
            rpg_scratch = 7;
            break;
        case 3:
            rpg_scratch = 8;
            break;
        case 4:
            rpg_scratch = 9;
            break;
        case 5:
            rpg_scratch = 0xA;
            break;
        }
        rpg_player_step_next();
        rpg_player_step_next();
        return;
    case 2:
        rpg_move_approach();
        return;
    case 3:
        rpg_no_club_step_3();
        return;
    case 4:
        rpg_no_club_step_4();
        return;
    case 5:
        rpg_no_club_step_5();
        return;
    case 6:
        rpg_no_club_step_6();
        return;
    case 7:
        rpg_apply_damage_single();
        rpg_player_step_next();
        return;
    case 8:
        rpg_move_return();
        return;
    case 9:
        rpg_player_action_end();
        rpg_escalation_counter = 0;
        /* fallthrough */
    default:
        return;
    }
}

void rpg_no_club_step_6(void) {
    switch (rpg_player_channel.unk_00) {
    case 0:
        rpg_actor_set_animation(0x28, 0xFF, 1, 0);
        rpg_actor_set_animation_alt(0x29, 0xE, 5, 1);
        rpg_player_substep_next();
        break;
    case 1:
        if (D_8012114C + 3 == rpg_scratch) {
            rpg_fade_set_level(0xFF);
            switch (rpg_escalation_counter) {
            case 0:
            default:
                rpg_se_play_latched_6xx(0x505, 1, 0);
                break;
            case 1:
                rpg_se_play_latched_6xx(0x505, 1, 0);
                break;
            case 2:
                rpg_se_play_latched_6xx(0x506, 1, 0);
                break;
            case 3:
                rpg_se_play_latched_6xx(0x507, 1, 0);
                break;
            case 4:
                rpg_se_play_latched_6xx(0x508, 1, 0);
                break;
            case 5:
                rpg_se_play_latched_6xx(0x509, 1, 0);
                break;
            }
            rpg_player_substep_next();
        }
        break;
    case 2:
        D_8012114C = rpg_scratch - 3;
        rpg_player_wait_substep(0xA);
        break;
    case 3:
        rpg_fade_set_level(0x80);
        rpg_actor_set_animation_alt(0x29, 0xFF, 1, 0);
        rpg_player_step_next();
        break;
    }
}

/* Implicit int with no return value: the call in rpg_club_move_no_club comes before this definition. */
s32 rpg_no_club_step_5(void) {
    s32 x;
    s32 y;

    x = (rpg_target_x - 0xE0) * rpg_player_channel.unk_04 / 1000 + 0xE0;
    y = (rpg_target_y - 0x58) * rpg_player_channel.unk_04 / 1000 + 0x58;
    if (rpg_enemy_slot == 0) {
        if (rpg_player_channel.unk_04 * 0x1800 / 1000 > 0) {
            rpg_se_play_latched_6xx(0x504, 0, 2);
        }
        if (rpg_player_channel.unk_04 * 0x1800 / 1000 > 0x400) {
            rpg_se_play_latched_6xx(0x504, 0, 3);
        }
        if (rpg_player_channel.unk_04 * 0x1800 / 1000 > 0x800) {
            rpg_se_play_latched_6xx(0x504, 0, 4);
        }
        if (rpg_player_channel.unk_04 * 0x1800 / 1000 > 0xC00) {
            rpg_se_play_latched_6xx(0x504, 0, 5);
        }
        if (rpg_player_channel.unk_04 * 0x1800 / 1000 > 0x1000) {
            rpg_se_play_latched_6xx(0x504, 0, 6);
        }
        if (rpg_player_channel.unk_04 * 0x1800 / 1000 > 0x1400) {
            rpg_se_play_latched_6xx(0x504, 0, 7);
        }
    } else {
        if (rpg_player_channel.unk_04 * 0x2000 / 1000 > 0) {
            rpg_se_play_latched_6xx(0x504, 0, 2);
        }
        if (rpg_player_channel.unk_04 * 0x2000 / 1000 > 0x400) {
            rpg_se_play_latched_6xx(0x504, 0, 3);
        }
        if (rpg_player_channel.unk_04 * 0x2000 / 1000 > 0x800) {
            rpg_se_play_latched_6xx(0x504, 0, 4);
        }
        if (rpg_player_channel.unk_04 * 0x2000 / 1000 > 0xC00) {
            rpg_se_play_latched_6xx(0x504, 0, 5);
        }
        if (rpg_player_channel.unk_04 * 0x2000 / 1000 > 0x1000) {
            rpg_se_play_latched_6xx(0x504, 0, 6);
        }
        if (rpg_player_channel.unk_04 * 0x2000 / 1000 > 0x1400) {
            rpg_se_play_latched_6xx(0x504, 0, 7);
        }
        if (rpg_player_channel.unk_04 * 0x2000 / 1000 > 0x1800) {
            rpg_se_play_latched_6xx(0x504, 0, 8);
        }
        if (rpg_player_channel.unk_04 * 0x2000 / 1000 > 0x1C00) {
            rpg_se_play_latched_6xx(0x504, 0, 9);
        }
    }
    if (rpg_enemy_slot == 0) {
        rpg_actor_set_position(0x29, x, (rsin(rpg_player_channel.unk_04 * 0x1800 / 1000) >> 8) + y);
        rpg_actor_set_position(0x28, x, (rsin(rpg_player_channel.unk_04 * 0x1800 / 1000) >> 8) + y);
    } else {
        rpg_actor_set_position(0x29, x, (rsin(rpg_player_channel.unk_04 * 0x2000 / 1000) >> 8) + y);
        rpg_actor_set_position(0x28, x, (rsin(rpg_player_channel.unk_04 * 0x2000 / 1000) >> 8) + y);
    }
    if (rpg_enemy_slot == 0) {
        rpg_player_channel.unk_04 += 0xC;
    } else {
        rpg_player_channel.unk_04 += 8;
    }
    if (rpg_player_channel.unk_04 > 0x3E8) {
        rpg_player_step_next();
    }
}

void rpg_no_club_step_4(void) {
    switch (rpg_player_channel.unk_00) {
    case 0:
        rpg_actor_set_animation(0x2A, 0xFF, 1, 0);
        rpg_actor_set_animation(0x2B, 0xFF, 1, 0);
        rpg_actor_set_animation(0x2C, 0xFF, 1, 0);
        rpg_actor_set_animation(0x2D, 0xFF, 1, 0);
        rpg_actor_set_animation(0x2E, 0xFF, 1, 0);
        rpg_actor_set_animation(0x2F, 0xFF, 1, 0);
        rpg_actor_set_animation(0x34, 5, 5, 1);
        rpg_actor_set_position(0x29, 0xFB, 0x3D);
        rpg_actor_set_position(0x28, 0xFB, 0x3D);
        rpg_player_substep_next();
        break;
    case 1:
        rpg_player_wait_substep(3);
        break;
    case 2:
        rpg_actor_set_position(0x29, 0x102, 0x45);
        rpg_actor_set_position(0x28, 0x102, 0x45);
        rpg_player_substep_next();
        break;
    case 3:
        rpg_player_wait_substep(3);
        break;
    case 4:
        rpg_actor_set_position(0x29, 0x107, 0x4C);
        rpg_actor_set_position(0x28, 0x107, 0x4C);
        rpg_player_substep_next();
        break;
    case 5:
        rpg_player_wait_substep(5);
        break;
    case 6:
        rpg_actor_set_position(0x29, 0x109, 0x53);
        rpg_actor_set_position(0x28, 0x109, 0x53);
        rpg_player_substep_next();
        break;
    case 7:
        rpg_player_wait_substep(5);
        break;
    case 8:
        rpg_actor_set_position(0x29, 0x108, 0x58);
        rpg_actor_set_position(0x28, 0x108, 0x58);
        rpg_player_substep_next();
        break;
    case 9:
        rpg_player_wait_substep(7);
        break;
    case 10:
        rpg_actor_set_position(0x29, 0x108 - rpg_player_timer_2, 0x58);
        rpg_actor_set_position(0x28, 0x108 - rpg_player_timer_2, 0x58);
        rpg_player_timer_2 += 2;
        if (rpg_player_timer_2 >= 0x28) {
            rpg_player_step_next();
        }
        break;
    }
}

/* int-returning (implicit int in the original): v0 is live out, so as1 keeps the lui of the promoted
 * rpg_player_guard_heal behind the case 4 branch; with a void return it moves it above (T-17115) */
s32 rpg_no_club_step_3(void) {
    if (rpg_player_channel.unk_00 < 5) {
        rpg_se_play_latched_6xx(0x503, 0x1E, 1);
    }
    switch (rpg_player_channel.unk_00) {
    case 0:
        rpg_actor_set_animation(0x34, 3, 1, 1);
        rpg_actor_set_animation(0x2F, 0xD, 5, 1);
        rpg_player_substep_next();
        return;
    case 1:
        rpg_player_wait_animation_substep(0x2F);
        return;
    case 2:
        rpg_actor_set_animation(0x2F, 7, 1, 1);
        rpg_actor_set_animation(0x2A, 8, 1, 1);
        rpg_actor_set_animation(0x2B, 9, 1, 1);
        rpg_actor_set_animation(0x2C, 0xA, 1, 1);
        rpg_actor_set_animation(0x2D, 0xB, 1, 1);
        rpg_actor_set_animation(0x2E, 0xC, 1, 1);
        rpg_player_substep_next();
        return;
    case 3:
        rpg_actor_set_position(0x29, 0xF0, 0x38);
        rpg_actor_set_position(0x28, 0xF0, 0x38);
        rpg_actor_set_animation_alt(0x29, 0xF, 1, 1);
        rpg_player_substep_next();
        return;
    case 4:
        if (rpg_player_timer_2 >= 0x1F) {
            rpg_player_timer_2 = 0;
            if (++rpg_player_guard_heal == rpg_scratch) {
                rpg_player_substep_next();
            } else {
                rpg_actor_set_animation_alt(0x29, rpg_player_guard_heal + 0xF, 1, 1);
                if ((rpg_player_guard_heal == 6) || (rpg_player_guard_heal == 7)) {
                    rpg_actor_set_animation_alt(0x28, 6, 1, 1);
                }
                if ((rpg_player_guard_heal == 8) || (rpg_player_guard_heal == 9)) {
                    rpg_actor_set_animation_alt(0x28, 0x19, 1, 1);
                }
            }
        }
        rpg_player_timer_2 += 1;
        return;
    case 5:
        rpg_player_wait_substep(0x1E);
        return;
    case 6:
        rpg_player_step_next();
        /* fallthrough */
    default:
        return;
    }
}
