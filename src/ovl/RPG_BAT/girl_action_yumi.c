/* girl_action_yumi.c: src/ovl/RPG_BAT/80136070.c:80: girl 10 action; body 81: switch (rpg_girl_step) {; 83: rpg_girl_timer_a += 1; partner-id dispatch src/ovl/RPG_BAT/partner_scheduler_and_motion.c:24. Original object 0x80136070-0x80136530 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/RPG_BAT.h"

void rpg_dialog_yumi_fake_cry(void) {
    switch (*rpg_dialog_step) {
    case 0:
        strcpy(rpg_text_girl_0, "早乙女「えーん");
        strcpy(rpg_text_girl_1, "えーん（うそなき）");
        rpg_message_show((s32) rpg_text_girl_pointers, 2, 0xB4);
        *rpg_dialog_step += 1;
        return;
    case 1:
        rpg_voice_play_once(0x0700002C, 0x07000034, 0);
        *rpg_dialog_step += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        rpg_actor_set_animation(0x1E, 7, 5, 1);
        rpg_voice_play_once(0x04000029, 0x0600002E, 1);
        *rpg_dialog_step += 1;
        return;
    case 4:
        rpg_voice_wait(0);
        return;
    case 5:
        rpg_voice_reset();
        rpg_girl_step_next();
        /* fallthrough */
    default:
        return;
    }
}

void rpg_dialog_enemy_apologizes(void) {
    switch (*rpg_dialog_step) {
    case 0:
        if (rpg_enemy_action & 0x100000) {
            strcpy(rpg_text_girl_0, "不良達「これはいかん！");
            strcpy(rpg_text_girl_1, "女子供を泣かせては、男がすたる。");
        } else {
            strcpy(rpg_text_girl_0, "番長「いっか〜ん！");
            strcpy(rpg_text_girl_1, "女子供を泣かせては、番長の名折れ！");
        }
        rpg_message_show((s32) rpg_text_girl_pointers, 2, 0xC8);
        *rpg_dialog_step += 1;
        return;
    case 1:
        if (rpg_enemy_action & 0x100000) {
            rpg_voice_play_once(0x1D000014, 0x05000017, 0);
        } else {
            rpg_voice_play_once(0x1E00008C, 0x1B000093, 0);
        }
        *rpg_dialog_step += 1;
        return;
    case 2:
        rpg_voice_wait(1);
        return;
    case 3:
        if (rpg_enemy_action & 0x100000) {
            rpg_voice_play_once(0x0E00004E, 0x0C000057, 1);
        } else {
            rpg_voice_play_once(0x04000012, 0x04000020, 1);
        }
        *rpg_dialog_step += 1;
        return;
    case 4:
        rpg_voice_wait(0);
        return;
    case 5:
        rpg_voice_reset();
        rpg_girl_step_next();
        /* fallthrough */
    default:
        return;
    }
}

void rpg_girl_action_yumi(void) {
    switch (rpg_girl_step) {
    case 0:
        rpg_girl_timer_a += 1;
        if (rpg_girl_timer_a % 20 < 0xA) {
            rpg_actor_set_animation(0x1D, 5, 1, 1);
        } else {
            rpg_actor_set_animation(0x1D, 0, 1, 1);
        }
        if (rpg_girl_timer_a >= 0x3B) {
            rpg_girl_step_next();
        }
        break;
    case 1:
        rpg_actor_set_animation(0x1D, 6, 1, 1);
        rpg_girl_step_next();
        break;
    case 2:
        rpg_dialog_yumi_fake_cry();
        break;
    case 3:
        rpg_girl_wait_animation_step(0x1E);
        break;
    case 4:
        rpg_dialog_enemy_apologizes();
        break;
    case 5:
        rpg_actor_set_animation(0x1E, 7, 0x21, 1);
        D_80120E60 = 7;
        D_80120E4E = 0x83;
        rpg_actor_set_animation(0x1D, 0, 1, 1);
        rpg_girl_step_next();
        break;
    case 6:
        if (rpg_actor_is_idle(0x1E) != 0) {
            rpg_actor_set_animation(0x1E, 0xFF, 1, 0);
            D_80120E4E = 1;
            rpg_girl_step_next();
        }
        break;
    case 7:
        rpg_girl_wait_step(0x14);
        break;
    case 8:
        rpg_girl_action_end();
        break;
    }
}
