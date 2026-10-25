/* girl_action_yoshio.c: src/ovl/RPG_BAT/801340F0.c:4: partner 13 actions; body 5: switch (rpg_girl_action) {; 7: rpg_yoshio_punch(); partner-id dispatch src/ovl/RPG_BAT/partner_scheduler_and_motion.c:24. Original object 0x801340F0-0x80134A70 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/RPG_BAT.h"

void rpg_girl_action_yoshio(void) {
    switch (rpg_girl_action) {
    case 1:
        rpg_yoshio_punch();
        return;
    case 2:
        rpg_yoshio_kick();
        return;
    case 3:
        rpg_yoshio_potion();
        return;
    }
}

void rpg_yoshio_punch(void) {

    switch (rpg_girl_step) {
    case 0:
        strcpy(rpg_text_girl_0, "好雄「いくぞっ！");
        strcpy(rpg_text_girl_1, "スペシャル好雄パーンチ！");
        rpg_message_show((s32) rpg_text_girl_pointers, 2, 0x3C);
        rpg_actor_set_animation(0x1D, 5, 5, 1);
        rpg_voice_play_once(0x1F0000A1, 0x1D0000A4, 0);
        rpg_girl_step_next();
        return;
    case 1:
        rpg_girl_wait_msg_step(1);
        return;
    case 2:
        rpg_voice_play_once(0x0700001F, 0x0600002B, 1);
        rpg_message_mode = 3;
        rpg_girl_wait_step(0x1E);
        return;
    case 3:
        rpg_actor_set_animation(0x1D, 7, 1, 1);
        rpg_girl_step_next();
        return;
    case 4:
        rpg_actor_set_position(0x1D, (s32) (rpg_girl_timer_a * -0xA8) / 1000, (s32) (rpg_girl_timer_a * -0x30) / 1000);
        rpg_girl_timer_a += 5;
        if (rpg_girl_timer_a >= 0x3E9) {
            rpg_actor_set_position(0x1D, -0xA8, -0x30);
            rpg_actor_set_position(0x1E, -0xA8, -0x30);
            rpg_actor_set_animation(0x1D, 8, 1, 1);
            rpg_actor_set_animation(0x1E, 0xC, 1, 1);
            rpg_popup_number_show(0, 0x48, 0x50, 0, 1);
            rpg_se_play_latched_5xx(0x503, 1, 0);
            rpg_message_mode = 1;
            rpg_girl_step_next();
            return;
        }
    default:
        return;
    case 5:
        rpg_girl_wait_step(0x3C);
        return;
    case 6:
        rpg_popup_number_show(1, 0x64, 0x60, rpg_girl_hit_points - 7, 1);
        rpg_girl_hit_points = 7;
        rpg_girl_step_next();
        return;
    case 7:
        rpg_girl_wait_step(0x3C);
        return;
    case 8:
        rpg_actor_set_animation(0x1D, 3, 1, 1);
        rpg_actor_set_animation(0x1E, 0xFF, 1, 0);
        rpg_girl_step_next();
        return;
    case 9:
        rpg_actor_set_position(0x1D, ((s32) (rpg_girl_timer_a * 0xA8) / 100) - 0xA8, ((s32) (rpg_girl_timer_a * 0x30) / 100) - 0x30);
        rpg_girl_timer_a += 2;
        if (rpg_girl_timer_a >= 0x65) {
            rpg_actor_set_position(0x1D, 0, 0);
            rpg_actor_set_animation(0x1D, 0, 1, 1);
            rpg_girl_step_next();
            return;
        }
        break;
    case 10:
        rpg_girl_wait_msg_step(0);
        return;
    case 11:
        rpg_girl_wait_step(0x14);
        return;
    case 12:
        rpg_girl_action_end();
        break;
    }
}


void rpg_yoshio_kick(void) {

    switch (rpg_girl_step) {
    case 0:
        strcpy(rpg_text_girl_0, "好雄「食らえ！");
        strcpy(rpg_text_girl_1, "グレート好雄キーック！");
        rpg_message_show((s32) rpg_text_girl_pointers, 2, 0x3C);
        rpg_actor_set_animation(0x1D, 5, 5, 1);
        rpg_voice_play_once(0x1F0000A5, 0x1E0000A8, 0);
        rpg_girl_step_next();
        return;
    case 1:
        rpg_girl_wait_msg_step(1);
        return;
    case 2:
        rpg_voice_play_once(0x1C0000A8, 0x0F0000B7, 1);
        rpg_message_mode = 3;
        rpg_girl_wait_step(0x1E);
        return;
    case 3:
        rpg_actor_set_animation(0x1D, 0xA, 1, 1);
        rpg_girl_step_next();
        return;
    case 4:
        rpg_actor_set_position(0x1D, (s32) (rpg_girl_timer_a * -0xA8) / 1000, (s32) (rpg_girl_timer_a * -0x30) / 1000);
        rpg_girl_timer_a += 5;
        if (rpg_girl_timer_a >= 0x3E9) {
            rpg_actor_set_position(0x1D, -0xA8, -0x30);
            rpg_actor_set_position(0x1E, -0xA8, -0x30);
            rpg_actor_set_animation(0x1D, 0xB, 1, 1);
            rpg_actor_set_animation(0x1E, 0xD, 1, 1);
            rpg_popup_number_show(0, 0x48, 0x5C, 0, 1);
            rpg_se_play_latched_5xx(0x504, 1, 0);
            rpg_message_mode = 1;
            rpg_girl_step_next();
            return;
        }
    default:
        return;
    case 5:
        rpg_girl_wait_step(0x3C);
        return;
    case 6:
        rpg_popup_number_show(1, 0x60, 0x58, rpg_girl_hit_points - 7, 1);
        rpg_girl_hit_points = 7;
        rpg_girl_step_next();
        return;
    case 7:
        rpg_girl_wait_step(0x3C);
        return;
    case 8:
        rpg_actor_set_animation(0x1D, 3, 1, 1);
        rpg_actor_set_animation(0x1E, 0xFF, 1, 0);
        rpg_girl_step_next();
        return;
    case 9:
        rpg_actor_set_position(0x1D, ((s32) (rpg_girl_timer_a * 0xA8) / 100) - 0xA8, ((s32) (rpg_girl_timer_a * 0x30) / 100) - 0x30);
        rpg_girl_timer_a += 2;
        if (rpg_girl_timer_a >= 0x65) {
            rpg_actor_set_position(0x1D, 0, 0);
            rpg_actor_set_animation(0x1D, 0, 1, 1);
            rpg_girl_step_next();
            return;
        }
        break;
    case 10:
        rpg_girl_wait_msg_step(0);
        return;
    case 11:
        rpg_girl_wait_step(0x14);
        return;
    case 12:
        rpg_girl_action_end();
        break;
    }
}


void rpg_yoshio_potion(void) {
    switch (rpg_girl_step) {
    case 0:
        strcpy(rpg_text_girl_0, "　　　　　　好雄の秘薬　　　　　　");
        rpg_message_show((s32) rpg_text_girl_pointers, 1, 0x3C);
        rpg_girl_step_next();
        return;
    case 1:
        rpg_girl_wait_msg_step(1);
        return;
    case 2:
        rpg_apply_damage(2, 0x23, 0x270F, 0);
        rpg_girl_hit_points = rpg_girl_hit_points_maximum;
        rpg_se_play_latched_5xx(0x502, 1, 0);
        rpg_girl_step_next();
        return;
    case 3:
        rpg_girl_wait_step(0x1E);
        return;
    case 4:
        rpg_girl_action_end();
        /* fallthrough */
    default:
        return;
    }
}
