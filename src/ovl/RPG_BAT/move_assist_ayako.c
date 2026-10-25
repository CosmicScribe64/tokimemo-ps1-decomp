/* move_assist_ayako.c: src/ovl/RPG_BAT/8013C5D0.c:4: girl 3 assist; body 8: switch (rpg_player_step) {; 10: rpg_girl_run_in(); girl-index assist dispatch src/ovl/RPG_BAT/drama_move_and_partner_assists.c:166. Original object 0x8013C5D0-0x8013C890 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/RPG_BAT.h"

void rpg_move_assist_ayako(void) {
    s32 temp_v1;
    s32 temp_v1_2;

    switch (rpg_player_step) {
    case 6:
        rpg_girl_run_in();
    default:
        break;
    case 7:
        strcpy(rpg_text_player_0, "片桐「はっ、");
        strcpy(rpg_text_player_1, "いいかげんにしなさいっ！");
        rpg_message_show((s32) &rpg_text_player_pointers, 2, 0xA);
        rpg_voice_play_once(0x42, 0x0A000043, 0);
        rpg_message_mode = 3;
        rpg_actor_set_animation_alt(0x1D, 7, 1, 1);
        rpg_player_step_next();
        break;
    case 8:
        temp_v1 = rpg_player_timer + 1;
        rpg_player_timer = temp_v1;
        if (temp_v1 >= 0x12D) {
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
        rpg_voice_play_once(0x44, 0x0B000049, 1);
        rpg_message_mode = 3;
        rpg_player_step_next();
        break;
    case 10:
        rpg_player_wait_step(0x32);
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
