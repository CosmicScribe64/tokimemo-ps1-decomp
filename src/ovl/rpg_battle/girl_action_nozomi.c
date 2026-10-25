/* girl_action_nozomi.c: src/ovl/RPG_BAT/80156870.c:14: girl 6 action; body 15: switch (rpg_girl_step) {; 17: if (rpg_nozomi_is_calm() != 0) {; partner-id dispatch src/ovl/RPG_BAT/801439E0.c:24. Original object 0x80156870-0x80156CC0 (config/objects/RPG_BAT.txt). */
#define RPG_BAT_OVERRIDE_rpg_actor_set_animation /* implicit int view: the call's $v0 moves the counter of rpg_girl_action_nozomi to $v1 */
#include "common.h"
#include "ovl/rpg_battle.h"

s32 rpg_actor_set_animation();

s32 rpg_nozomi_is_calm(void) {
    if ((u32)(get_g_zyotai_s(GIRL_NOZOMI) & 0x7F) < 2) {
        return 1;
    }
    return 0;
}

void rpg_girl_action_nozomi(void) {
    switch (rpg_girl_step) {
    case 0:
        if (rpg_nozomi_is_calm() != 0) {
            strcpy(rpg_text_girl_0, "清川「フザケンナヨー！");
            strcpy(rpg_text_girl_1, "あっ！いや、なんでもない・・・");
        } else {
            strcpy(rpg_text_girl_0, "清川「フザケンナヨー！");
            strcpy(rpg_text_girl_1, "馬鹿野郎ーっ！");
        }
        rpg_girl_step_next();
    default:
        break;
    case 1:
        rpg_girl_slide_in(1, 0);
        break;
    case 2:
        rpg_actor_set_animation(0x1D, 5, 5, 1);
        rpg_se_play_latched_5xx(0x511, 1, 0);
        rpg_girl_step_next();
        break;
    case 3:
        if (D_80120E1C == 4) {
            rpg_message_show(rpg_text_girl_pointers, 2, 0x50);
            if (rpg_nozomi_is_calm() != 0) {
                rpg_voice_play_once(0x1F00005A, 0x1000005E, 0);
            } else {
                rpg_voice_play_once(0x1F00005F, 0x11000063, 0);
            }
            rpg_girl_step_next();
        }
        break;
    case 4:
        rpg_girl_wait_msg_step(1);
        break;
    case 5:
        rpg_girl_timer_a += 1;
        if (rpg_girl_timer_a >= 0x12D) {
            rpg_girl_step_next();
        }
        if (cd_audio_state() == 2) {
            rpg_girl_step_next();
        }
        break;
    case 6:
        if (rpg_nozomi_is_calm() != 0) {
            rpg_message_mode = 3;
            rpg_voice_play_once(0x03000011, 0x0300001C, 1);
            rpg_actor_set_animation(0x1D, 7, 5, 1);
        } else {
            rpg_voice_play_once(0x1F000064, 0x12000068, 1);
            D_80120E0A = 0x82;
            rpg_actor_set_animation(0x1D, 6, 5, 1);
        }
        rpg_girl_step_next();
        break;
    case 7:
        rpg_message_line_frames = 0x5A;
        if (rpg_actor_is_idle(0x1D) != 0) {
            if (rpg_nozomi_is_calm() != 0) {
                rpg_girl_step_next();
            } else {
                rpg_girl_step = 0x14;
            }
        }
        break;
    case 8:
        rpg_actor_set_animation(0x1D, 7, 5, 1);
        rpg_girl_step_next();
        break;
    case 9:
        if (game_state.pad_pressed & PAD_CROSS) {
            rpg_actor_set_animation(0x1D, 7, 5, 1);
            if (rpg_message_mode == 3) {
                rpg_message_mode = 1;
            }
            rpg_girl_step = 0x14;
        }
        if (rpg_actor_is_idle(0x1D) != 0) {
            rpg_actor_set_animation(0x1D, 7, 5, 1);
            rpg_girl_timer_a += 1;
            if (rpg_girl_timer_a >= 5) {
                if (rpg_message_mode == 3) {
                    rpg_message_mode = 1;
                }
                rpg_girl_step = 0x14;
            }
        }
        break;
    case 20:
        rpg_girl_wait_step(0x28);
        break;
    case 21:
        rpg_actor_set_animation(0x1D, 0, 1, 1);
        rpg_girl_step_next();
        break;
    case 22:
        D_80120E0A = 1;
        rpg_actor_set_animation(0x1D, 3, 1, 1);
        rpg_girl_step_next();
        break;
    case 23:
        rpg_girl_slide_out(3, 0);
        break;
    case 24:
        rpg_girl_wait_step(0x14);
        break;
    case 25:
        rpg_girl_action_end();
        break;
    }
}
