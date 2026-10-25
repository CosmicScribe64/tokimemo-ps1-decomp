/* move_assist_megumi.c: src/ovl/RPG_BAT/801454F0.c:7: girl 9 assist; body 8: if (rpg_girl_action == 0x40000000) {; 9: switch (rpg_player_step) {; girl-index assist dispatch src/ovl/rpg_battle/drama_move_and_partner_assists.c:166. Original object 0x801454F0-0x80145CC0 (config/objects/RPG_BAT.txt). */
#define RPG_BAT_OVERRIDE_rpg_actor_set_position /* implicit int view: the call's $v0 moves the rpg_player_timer_2 counters to $v1 */
#include "common.h"
#include "ovl/rpg_battle.h"

s32 rpg_actor_set_position();

void rpg_move_assist_megumi(void) {
    if (rpg_girl_action == 0x40000000) {
        switch (rpg_player_step) {
        case 6:
            rpg_girl_run_set_rect();
            rpg_actor_set_position(0x1D, rpg_move_params.param_2 + 0x60, rpg_move_params.param_3);
            rpg_actor_set_position(0x32, rpg_move_params.param_4 + rpg_move_params.param_2 + 0x60, rpg_move_params.param_5 + rpg_move_params.param_3);
            rpg_actor_set_animation_alt(0x1D, 0, 1, 1);
            rpg_actor_set_animation_alt(0x32, 7, 5, 1);
            D_801213B0 = 1;
            D_80120E05 = 4;
            D_80121399 = 3;
            rpg_player_step_next();
            break;
        case 7:
            rpg_actor_set_position(0x1D, (rpg_move_params.param_2 - rpg_player_timer_2) + 0x60, rpg_move_params.param_3);
            rpg_actor_set_position(0x32, ((rpg_move_params.param_4 + rpg_move_params.param_2) - rpg_player_timer_2) + 0x60, rpg_move_params.param_5 + rpg_move_params.param_3);
            rpg_player_timer_2 += 2;
            if (rpg_player_timer_2 >= 0x61) {
                rpg_player_step_next();
            }
            break;
        case 8:
            strcpy(rpg_text_player_0, "美樹原「いいかげんに…");
            strcpy(rpg_text_player_1, "い、いえ…やっぱりいいです…");
            rpg_message_show(&rpg_text_player_pointers, 2, 0xA);
            rpg_voice_play_once(0x86, 0x1300008B, 0);
            rpg_message_mode = 3;
            rpg_actor_set_animation_alt(0x1D, 8, 1, 1);
            rpg_player_step_next();
            break;
        case 9:
            rpg_player_wait_step(0x14);
            break;
        case 10:
            rpg_actor_set_animation_alt(0x32, 0xD, 1, 1);
            rpg_player_step_next();
            break;
        case 11:
            rpg_player_timer += 1;
            if (rpg_player_timer >= 0x12D) {
                rpg_actor_set_animation_alt(0x32, 0xD, 1, 0);
                rpg_message_mode = 1;
                rpg_player_step_next();
            }
            if (cd_audio_state() == 4) {
                rpg_voice_playing_seen = 1;
            }
            if (rpg_voice_playing_seen == 1) {
                if (cd_audio_state() == 2) {
                    rpg_actor_set_animation_alt(0x32, 0xD, 1, 0);
                    rpg_message_mode = 1;
                    rpg_voice_playing_seen = 0;
                    rpg_player_step_next();
                }
            }
            break;
        case 12:
            rpg_voice_play_once(0x8C, 0x14000099, 1);
            rpg_message_mode = 3;
            rpg_player_step_next();
            break;
        case 13:
            rpg_player_wait_step(5);
            break;
        case 14:
            rpg_player_timer += 1;
            if (rpg_player_timer >= 0x12D) {
                rpg_message_mode = 1;
                rpg_message_frame = rpg_message_line_frames + 1;
                rpg_actor_set_animation_alt(0x1D, 0, 1, 1);
                rpg_player_step_next();
            }
            if (cd_audio_state() == 4) {
                rpg_voice_playing_seen = 1;
            }
            if (rpg_voice_playing_seen == 1) {
                if (cd_audio_state() == 2) {
                    rpg_message_mode = 1;
                    rpg_message_frame = rpg_message_line_frames + 1;
                    rpg_actor_set_animation_alt(0x1D, 0, 1, 1);
                    rpg_voice_playing_seen = 0;
                    rpg_player_step_next();
                }
            }
            break;
        case 15:
            rpg_player_wait_step(0x1E);
            break;
        case 16:
            rpg_actor_set_position(0x1D, rpg_move_params.param_2 + rpg_player_timer_2, rpg_move_params.param_3);
            rpg_actor_set_position(0x32, rpg_move_params.param_4 + rpg_move_params.param_2 + rpg_player_timer_2, rpg_move_params.param_5 + rpg_move_params.param_3);
            rpg_player_timer_2 += 2;
            if (rpg_player_timer_2 >= 0x61) {
                rpg_player_step_next();
            }
            break;
        case 17:
            rpg_player_step = 0x64;
            break;
        }
    } else {
        switch (rpg_player_step) {
        case 6:
            rpg_girl_run_in();
        default:
            break;
        case 7:
            strcpy(rpg_text_player_0, "美樹原「いいかげんに…");
            strcpy(rpg_text_player_1, "い、いえ…やっぱりいいです…");
            rpg_message_show(&rpg_text_player_pointers, 2, 0xA);
            rpg_voice_play_once(0x86, 0x1300008B, 0);
            rpg_message_mode = 3;
            rpg_actor_set_animation_alt(0x1D, 8, 1, 1);
            rpg_player_step_next();
            break;
        case 8:
            rpg_player_wait_step(0x14);
            break;
        case 9:
            rpg_actor_set_animation_alt(0x32, 0xD, 1, 1);
            rpg_player_step_next();
            break;
        case 10:
            rpg_player_timer += 1;
            if (rpg_player_timer >= 0x12D) {
                rpg_actor_set_animation_alt(0x32, 0xD, 1, 0);
                rpg_message_mode = 1;
                rpg_player_step_next();
            }
            if (cd_audio_state() == 4) {
                rpg_voice_playing_seen = 1;
            }
            if (rpg_voice_playing_seen == 1) {
                if (cd_audio_state() == 2) {
                    rpg_actor_set_animation_alt(0x32, 0xD, 1, 0);
                    rpg_message_mode = 1;
                    rpg_voice_playing_seen = 0;
                    rpg_player_step_next();
                }
            }
            break;
        case 11:
            rpg_voice_play_once(0x8C, 0x14000099, 1);
            rpg_message_mode = 3;
            rpg_player_step_next();
            break;
        case 12:
            rpg_player_wait_step(5);
            break;
        case 13:
            rpg_player_timer += 1;
            if (rpg_player_timer >= 0x12D) {
                rpg_message_mode = 1;
                rpg_message_frame = rpg_message_line_frames + 1;
                rpg_actor_set_animation_alt(0x1D, 0, 1, 1);
                rpg_player_step_next();
            }
            if (cd_audio_state() == 4) {
                rpg_voice_playing_seen = 1;
            }
            if (rpg_voice_playing_seen == 1) {
                if (cd_audio_state() == 2) {
                    rpg_message_mode = 1;
                    rpg_message_frame = rpg_message_line_frames + 1;
                    rpg_actor_set_animation_alt(0x1D, 0, 1, 1);
                    rpg_voice_playing_seen = 0;
                    rpg_player_step_next();
                }
            }
            break;
        case 14:
            rpg_player_wait_step(0x1E);
            break;
        case 15:
            rpg_girl_run_out();
            break;
        case 16:
            rpg_player_step = 0x64;
            break;
        }
    }
}
