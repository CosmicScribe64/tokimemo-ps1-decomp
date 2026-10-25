/* partner_scheduler_and_motion.c: src/ovl/RPG_BAT/801439E0.c:4: girl action scheduler and shared girl helpers; body 5: if ((rpg_girl_action_delay() < rpg_girl_timer) && !(rpg_player_action & 0xFF) && !(rpg_enemy_action & 0xFF) && !(rpg_over & 0xFF) &&; 7: rpg_player_pose_idle();. Original object 0x801439E0-0x80144010 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/rpg_battle.h"

void rpg_girl_update(void) {
    if ((rpg_girl_action_delay() < rpg_girl_timer) && !(rpg_player_action & 0xFF) && !(rpg_enemy_action & 0xFF) && !(rpg_over & 0xFF) &&
        !(rpg_girl_action & 0xFF)) {
        rpg_player_pose_idle();
        rpg_girl_timer = 0;
        rpg_girl_action = rpg_girl_pick_action();
        if (!(rpg_girl_action & 0xFF)) {
            rpg_girl_timer = rpg_rand2(0x64, 0x32);
        }
    }
    if (rpg_girl_action == 0x10001000) {
        rpg_girl_survival_timer();
    }
    if (rpg_girl_action == 0x10001001) {
        if ((s32)rpg_girl_step < 0xF) {
            rpg_player_pose_idle();
        }
        rpg_girl_rescue_sequence();
    }
    if (!(rpg_over & 0xFF) && ((rpg_girl_action & 0xFF) || (rpg_girl_action & 0x80000000))) {
        switch (rpg_girl_id) {
        case 0:
            rpg_girl_action_shiori();
        default:
            break;
        case 1:
            rpg_girl_action_mio();
            break;
        case 2:
            if (rpg_encounter != 0x80000000) {
                rpg_girl_action_yuina();
            }
            break;
        case 3:
            rpg_girl_action_ayako();
            break;
        case 4:
            rpg_girl_action_saki();
            break;
        case 5:
            rpg_girl_action_yukari();
            break;
        case 6:
            rpg_girl_action_nozomi();
            break;
        case 7:
            rpg_girl_action_mira();
            break;
        case 8:
            rpg_girl_action_yuko();
            break;
        case 9:
            rpg_girl_action_megumi();
            break;
        case 10:
            rpg_girl_action_yumi();
            break;
        case 13:
            rpg_girl_action_yoshio();
            break;
        case 14:
            break;
        }
    }
}

void rpg_girl_slide_in(s32 arg0, s32 arg1) {
    s32 temp_v0;

    switch (rpg_girl_channel.unk_08) {
    case 0:
        rpg_actor_set_animation(0x1D, arg0, 1, 1);
        rpg_girl_substep_next();
        return;
    case 1:
        rpg_actor_set_position(0x1D, -rpg_girl_timer_b, 0);
        temp_v0 = rpg_girl_timer_b + 2;
        rpg_girl_timer_b = temp_v0;
        if (temp_v0 >= 0x21) {
            rpg_actor_set_animation(0x1D, arg1, 1, 1);
            rpg_actor_set_position(0x1D, -0x20, 0);
            rpg_girl_step_next();
        }
        return;
    }
}

void rpg_girl_slide_out(s32 arg0, s32 arg1) {
    s32 temp_v0;

    switch (rpg_girl_channel.unk_08) {
    case 0:
        rpg_actor_set_animation(0x1D, arg0, 1, 1);
        rpg_girl_substep_next();
        return;
    case 1:
        rpg_actor_set_position(0x1D, rpg_girl_timer_b - 0x20, 0);
        temp_v0 = rpg_girl_timer_b + 2;
        rpg_girl_timer_b = temp_v0;
        if (temp_v0 >= 0x21) {
            rpg_actor_set_animation(0x1D, arg1, 1, 1);
            rpg_actor_set_position(0x1D, 0, 0);
            rpg_girl_step_next();
        }
        return;
    }
}

void rpg_girl_move_linear(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    rpg_motion = arg3 - arg1;
    rpg_motion_dy = arg4 - arg2;
    rpg_actor_set_position(arg0, (rpg_motion * rpg_girl_timer_b) / 1000 + arg1, (rpg_motion_dy * rpg_girl_timer_b) / 1000 + arg2);
    rpg_girl_timer_b += arg5;
    if (rpg_girl_timer_b >= 0x3E9) {
        rpg_actor_set_position(arg0, arg3, arg4);
        rpg_girl_step_next();
    }
}

void rpg_blink_player_and_girl_60(void) {
    if (!(rpg_girl_channel.unk_0C & 1)) {
        ((Actor *)actor_pool)[144].draw_flags |= 0x80;
        ((Actor *)actor_pool)[125].draw_flags |= 0x80;
    } else {
        ((Actor *)actor_pool)[144].draw_flags &= 0xFF7F;
        ((Actor *)actor_pool)[125].draw_flags &= 0xFF7F;
    }
    rpg_girl_channel.unk_0C += 1;
    if (rpg_girl_channel.unk_0C >= 0x3C) {
        rpg_girl_step_next();
    }
}

void rpg_blink_player_and_girl_61(void) {
    if (!(rpg_girl_channel.unk_0C & 1)) {
        ((Actor *)actor_pool)[144].draw_flags |= 0x80;
        ((Actor *)actor_pool)[125].draw_flags |= 0x80;
    } else {
        ((Actor *)actor_pool)[144].draw_flags &= 0xFF7F;
        ((Actor *)actor_pool)[125].draw_flags &= 0xFF7F;
    }
    rpg_girl_channel.unk_0C += 1;
    if (rpg_girl_channel.unk_0C >= 0x3D) {
        rpg_girl_step_next();
    }
}
