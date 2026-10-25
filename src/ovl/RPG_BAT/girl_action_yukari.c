/* girl_action_yukari.c: src/ovl/RPG_BAT/80159D50.c:25: girl 5 action; body 26: switch (rpg_girl_step) {; 29: rpg_message_show(rpg_text_girl_pointers, 1, 0x5A); partner-id dispatch src/ovl/RPG_BAT/801439E0.c:24. Original object 0x80159D50-0x8015A1F0 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/RPG_BAT.h"

s32 rpg_dialog_yukari_father(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_girl_0, "古式「お父様ーーっ！！");
        rpg_message_show(rpg_text_girl_pointers, 1, 0xAA);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x05000000, 0x01000007, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_reset();
        rpg_girl_substep_next();
        return;
    }
}

void rpg_girl_action_yukari(void) {
    switch (rpg_girl_step) {
    case 0:
        strcpy(rpg_text_girl_0, "　　　　　　おやじの怒り　　　　　　");
        rpg_message_show(rpg_text_girl_pointers, 1, 0x5A);
        rpg_pick_target();
        rpg_set_target_position_alt(rpg_enemy_slot);
        rpg_girl_damage_roll();
        rpg_girl_step_next();
        return;
    case 1:
        rpg_girl_wait_msg_step(1);
        return;
    case 2:
        rpg_girl_slide_in(0, 0);
        return;
    case 3:
        rpg_yukari_call_father();
        return;
    case 4:
        rpg_blink_player_and_girl_60();
        return;
    case 5:
        rpg_actor_set_position(0x1D, 0x160, 0x68);
        rpg_actor_set_animation_alt(0x1D, 4, 1, 1);
        rpg_girl_step_next();
        return;
    case 6:
        if (rpg_enemy_action & 0x100000) {
            rpg_girl_move_linear(0x1D, 0x160, 0x68, 0xA8, 0x68, 0x96);
            return;
        }
        rpg_girl_move_linear(0x1D, 0x160, 0x68, 0x90, 0x68, 0x96);
        return;
    case 7:
        rpg_actor_set_position(0x1E, rpg_target_x, rpg_target_y);
        rpg_actor_set_animation_alt(0x1E, 5, 1, 1);
        rpg_girl_step_next();
        return;
    case 8:
        rpg_se_play_latched_5xx(0x512, 0xF, 1);
        rpg_girl_wait_step(0x37);
        return;
    case 9:
        rpg_actor_set_animation_alt(0x1E, 0xFF, 1, 0);
        rpg_apply_damage(0, rpg_enemy_slot, rpg_damage, 1);
        rpg_girl_step_next();
        return;
    case 10:
        rpg_girl_wait_step(0x1E);
        return;
    case 11:
        if (!(rpg_girl_timer_a & 1)) {
            rpg_actor_set_animation_alt(0x1D, 0xFF, 1, 1);
        } else {
            rpg_actor_set_animation_alt(0x1D, 0xFF, 1, 0);
        }
        rpg_girl_timer_a += 1;
        if (rpg_girl_timer_a >= 0x3C) {
            rpg_actor_set_position(0x1D, -0x20, 0);
            rpg_actor_set_animation(0x1D, 1, 1, 0);
            rpg_girl_step_next();
            return;
        }
        break;
    case 12:
        rpg_blink_player_and_girl_61();
        return;
    case 13:
        rpg_girl_slide_out(1, 0);
        return;
    case 14:
        rpg_girl_wait_step(0x14);
        return;
    case 15:
        rpg_girl_action_end();
        break;
    }
}

void rpg_yukari_call_father(void) {
    switch (rpg_girl_substep) {
    case 0:
        rpg_actor_set_animation(0x1D, 1, 1, 1);
        rpg_girl_substep_next();
        return;
    case 1:
        rpg_girl_wait_substep(0x14);
        return;
    case 2:
        rpg_actor_set_animation(0x1D, 3, 1, 1);
        rpg_girl_substep_next();
        return;
    case 3:
        rpg_dialog_yukari_father();
        return;
    case 4:
        rpg_actor_set_animation(0x1D, 1, 1, 1);
        rpg_girl_step_next();
        return;
    }
}
