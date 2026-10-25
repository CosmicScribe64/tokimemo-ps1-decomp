/* move_assist_shiori.c: src/ovl/RPG_BAT/80134A70.c:4: girl 0 assist; body 8: switch (rpg_player_step) {; 10: rpg_girl_run_in(); girl-index assist dispatch src/ovl/rpg_battle/drama_move_and_partner_assists.c:166. Original object 0x80134A70-0x80134D30 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/rpg_battle.h"

void rpg_move_assist_shiori(void) {
    s32 temp_v1;
    s32 temp_v1_2;

    switch (rpg_player_step) {
    case 6:
        rpg_girl_run_in();
    default:
        break;
    case 7:
        strcpy(rpg_text_player_0, "藤崎「もう、");
        strcpy(rpg_text_player_1, "なにバカなこと言ってるのよ。");
        rpg_message_show((s32) &rpg_text_player_pointers, 2, 0xA);
        rpg_voice_play_once(6, 0x02000008, 0);
        rpg_message_mode = 3;
        rpg_actor_set_animation_alt(0x1D, 6, 1, 1);
        rpg_player_step_next();
        break;
    case 8:
        temp_v1 = rpg_player_timer + 1;
        rpg_player_timer = temp_v1;
        if (temp_v1 >= 0x79) {
            rpg_message_mode = 1;
            rpg_player_step_next();
        }
        if (cd_audio_state() == 4) {
            rpg_voice_playing_seen = 1;
        }
        if (rpg_voice_playing_seen == 1) {
            if (cd_audio_state() == 2) {
                rpg_message_mode = 1;
                rpg_voice_playing_seen = 0;
                rpg_player_step_next();
            }
        }
        break;
    case 9:
        rpg_voice_play_once(9, 0x03000011, 1);
        rpg_message_mode = 3;
        rpg_player_step_next();
        break;
    case 10:
        rpg_player_wait_step(5);
        break;
    case 11:
        rpg_girl_effect_a();
        temp_v1_2 = rpg_player_timer + 1;
        rpg_player_timer = temp_v1_2;
        if (temp_v1_2 >= 0x12D) {
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
    case 12:
        rpg_girl_effect_a();
        rpg_player_wait_step(0x1E);
        break;
    case 13:
        rpg_girl_run_out();
        break;
    case 14:
        rpg_player_step = 0x64;
        break;
    }
}
