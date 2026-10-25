/* girl_action_yuina.c: src/ovl/RPG_BAT/801480F0.c:26: girl 2 action; body 27: switch (rpg_girl_step) {; 29: rpg_girl_slide_in(9, 0); partner-id dispatch src/ovl/RPG_BAT/801439E0.c:24. Original object 0x801480F0-0x801484E0 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/rpg_battle.h"

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 rpg_dialog_yuina_take_this(void) {
    switch (rpg_dialog_step[0]) {
    case 0:
        strcpy(rpg_text_girl_0, "紐緒「これでもくらいなさい！");
        rpg_message_show(rpg_text_girl_pointers, 1, 0xAA);
        rpg_dialog_step[0] += 1;
        return;
    case 1:
        rpg_voice_play_once(0x12, 0x04000019, 0);
        rpg_dialog_step[0] += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_voice_reset();
        rpg_girl_step_next();
        return;
    }
}

void rpg_girl_action_yuina(void) {
    switch (rpg_girl_step) {
    case 0:
        rpg_girl_slide_in(9, 0);
        return;
    case 1:
        rpg_actor_set_animation(0x1D, 3, 1, 1);
        rpg_actor_set_position(0x1E, 0xD8, 0x88);
        rpg_actor_set_animation_alt(0x1E, 6, 5, 1);
        rpg_pick_target();
        rpg_set_target_position_alt(rpg_enemy_slot);
        rpg_girl_damage_roll();
        rpg_se_play_latched_5xx(0x50E, 1, 0);
        rpg_girl_step_next();
        return;
    case 2:
        rpg_girl_wait_animation_step(0x1E);
        return;
    case 3:
        rpg_actor_set_animation_alt(0x1E, 7, 1, 1);
        rpg_girl_step_next();
        return;
    case 4:
        rpg_dialog_yuina_take_this();
        return;
    case 5:
        rpg_actor_set_animation(0x1D, 4, 5, 1);
        rpg_girl_step_next();
        return;
    case 6:
        rpg_girl_wait_animation_step(0x1D);
        return;
    case 7:
        rpg_actor_set_animation(0x1D, 5, 1, 1);
        rpg_se_play_latched_5xx(0x50F, 1, 0);
        rpg_girl_step_next();
        return;
    case 8:
        if (rpg_enemy_slot == 0) {
            rpg_girl_move_linear(0x1E, 0xD8, 0x88, rpg_target_x, rpg_target_y, 0x3C);
            return;
        }
        rpg_girl_move_linear(0x1E, 0xD8, 0x88, rpg_target_x, rpg_target_y, 0x28);
        return;
    case 9:
        rpg_girl_wait_step(0x1E);
        return;
    case 10:
        if (rpg_enemy_action & 0x100000) {
            rpg_actor_set_position(0x1E, rpg_target_x, rpg_target_y - 0x30);
        } else {
            rpg_actor_set_position(0x1E, rpg_target_x - 4, 0x5E);
        }
        rpg_actor_set_animation_alt(0x1E, 8, 5, 1);
        rpg_se_play_latched_5xx(0x510, 1, 0);
        rpg_girl_step_next();
        return;
    case 11:
        rpg_girl_wait_animation_step(0x1E);
        return;
    case 12:
        rpg_actor_set_animation(0x1E, 0xFF, 1, 0);
        rpg_apply_damage(0, rpg_enemy_slot, rpg_damage, 1);
        rpg_girl_step_next();
        return;
    case 13:
        rpg_girl_wait_step(0x3C);
        return;
    case 14:
        rpg_girl_slide_out(1, 0);
        return;
    case 15:
        rpg_girl_wait_step(0x14);
        return;
    case 16:
        rpg_girl_action_end();
        /* fallthrough */
    default:
        return;
    }
}
