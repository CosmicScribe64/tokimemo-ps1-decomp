/* girl_action_mira.c: src/ovl/RPG_BAT/8014DF60.c:4: girl 7 action; body 5: switch (rpg_girl_step) {; 7: rpg_mira_pick_target(); partner-id dispatch src/ovl/RPG_BAT/801439E0.c:24. Original object 0x8014DF60-0x8014E780 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/rpg_battle.h"

void rpg_girl_action_mira(void) {
    switch (rpg_girl_step) {
    case 0:
        rpg_mira_pick_target();
        rpg_girl_step_next();
        return;
    case 1:
        rpg_girl_slide_in(1, 0);
        return;
    case 2:
        D_80120E49 = 4;
        rpg_actor_set_animation(0x1D, 5, 5, 1);
        rpg_girl_step_next();
        return;
    case 3:
        if (D_80120E1C == 5) {
            rpg_se_play_latched_5xx(0x50E, 0, 4);
        }
        rpg_girl_wait_animation_step(0x1D);
        return;
    case 4:
        rpg_actor_set_position(0x1E, 0xE0, 0x80);
        rpg_actor_set_animation_alt(0x1E, 7, 5, 1);
        rpg_girl_step_next();
        return;
    case 5:
        rpg_girl_wait_animation_step(0x1E);
        return;
    case 6:
        rpg_actor_set_animation_alt(0x1E, 8, 1, 1);
        rpg_girl_step_next();
        return;
    case 7:
        rpg_mira_move();
        return;
    case 8:
        rpg_actor_set_animation_alt(0x1E, 9, 5, 1);
        rpg_se_play_latched_5xx(0x510, 1, 2);
        rpg_girl_step_next();
        return;
    case 9:
        rpg_girl_wait_animation_step(0x1E);
        return;
    case 10:
        rpg_actor_set_animation_alt(0x1E, 0xFF, 1, 0);
        if (rpg_girl_action == 1) {
            rpg_mira_charm_apply();
            return;
        }
        rpg_mira_reject_line();
        return;
    case 11:
        if (cd_audio_state() == 2) {
            rpg_actor_set_animation(0x1D, 0, 1, 1);
        }
        rpg_girl_wait_msg_step(1);
        return;
    case 12:
        rpg_girl_wait_step(0x14);
        return;
    case 13:
        rpg_girl_slide_out(3, 0);
        return;
    case 14:
        rpg_girl_wait_step(0x14);
        return;
    case 15:
        rpg_girl_action_end();
        /* fallthrough */
    default:
        return;
    }
}

void rpg_mira_charm_apply(void) {
    strcpy(rpg_text_girl_0, "鏡「また私のとりこが一人。");
    rpg_message_show(rpg_text_girl_pointers, 1, 0x8C);
    rpg_voice_play_once(0x0300001D, 0x04000025, 0);
    switch (rpg_enemy_slot) {
    case 0:
        rpg_delinquent_charmed_0[0] = 1;
        break;
    case 1:
        rpg_delinquent_charmed_1 = 1;
        break;
    case 2:
        rpg_delinquent_charmed_2 = 1;
        break;
    }
    ((Actor *)actor_pool)[rpg_enemy_slot + 155].draw_flags |= 1;
    rpg_actor_set_animation(0x1D, 6, 1, 1);
    rpg_girl_step_next();
}

void rpg_mira_reject_line(void) {
    strcpy(rpg_text_girl_0, "鏡「でなおしてらっしゃい！");
    rpg_message_show(rpg_text_girl_pointers, 1, 0x64);
    rpg_voice_play_once(0x1F000069, 0x1300006D, 0);
    rpg_actor_set_animation(0x1D, 6, 1, 1);
    rpg_girl_step_next();
}

void rpg_mira_move(void) {
    RpgActorMotion *p = (RpgActorMotion *)&rpg_target_x;
    rpg_mira_target_position(rpg_enemy_slot);
    p->dx = p->x - 0xE0;
    p->dy = p->y - 0x8E;
    rpg_se_play_latched_5xx(0x50F, 0, 1);
    if (rpg_girl_timer_a >= 0x140) {
        rpg_se_play_latched_5xx(0x514, 0x10, 3);
    }
    if (rpg_enemy_slot == 0) {
        rpg_girl_timer_a += 7;
        rpg_actor_set_position(0x1E, (p->dx * rpg_girl_timer_a) / 1000 + 0xE0, (rsin((rpg_girl_timer_a << 0xD) / 1000) >> 8) + (p->dy * rpg_girl_timer_a) / 1000 + 0x80);
    } else {
        rpg_girl_timer_a += 5;
        rpg_actor_set_position(0x1E, (p->dx * rpg_girl_timer_a) / 1000 + 0xE0, (rsin((rpg_girl_timer_a * 0x3000) / 1000) >> 8) + (p->dy * rpg_girl_timer_a) / 1000 + 0x80);
    }
    if (rpg_girl_timer_a >= 0x3E9) {
        rpg_actor_set_position(0x1E, p->x, p->y - 0xE);
        rpg_girl_step_next();
    }
}


void rpg_mira_target_position(s32 target_slot) {
    switch (target_slot) {
    case 0:
        rpg_target_x = 0x78;
        rpg_target_y = 0x68;
        return;
    case 1:
        rpg_target_x = 0x40;
        rpg_target_y = 0x58;
        return;
    case 2:
        rpg_target_x = 0x38;
        rpg_target_y = 0x88;
        return;
    case 3:
        rpg_target_x = 0x48;
        rpg_target_y = 0x60;
        return;
    }
}

s32 rpg_mira_pick_target() {
    s32 target_roll;

    if (rpg_enemy_action & 0x100000) {
        target_roll = rpg_rand(3);
        switch (target_roll) {
        case 0:
            if (*rpg_delinquent_charmed_0 == 0) {
                rpg_enemy_slot = 0;
                return;
            }
            if (rpg_delinquent_charmed_1 == 0) {
                rpg_enemy_slot = 1;
                return;
            }
            rpg_enemy_slot = 2;
            return;
        case 1:
            if (rpg_delinquent_charmed_1 == 0) {
                rpg_enemy_slot = 1;
                return;
            }
            if (rpg_delinquent_charmed_2 == 0) {
                rpg_enemy_slot = 2;
                return;
            }
            rpg_enemy_slot = 0;
            return;
        case 2:
            if (rpg_delinquent_charmed_2 == 0) {
                rpg_enemy_slot = 2;
                return;
            }
            if (*rpg_delinquent_charmed_0 == 0) {
                rpg_enemy_slot = 0;
                return;
            }
            rpg_enemy_slot = 1;
            return;
        }
    } else {
        rpg_enemy_slot = 3;
        rpg_girl_action = 2;
    }
}

