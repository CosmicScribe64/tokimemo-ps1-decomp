/* move_assist_mira.c: src/ovl/RPG_BAT/8013A480.c:4: girl 7 assist; body 7: switch (rpg_player_step) {; 9: rpg_girl_run_in(); girl-index assist dispatch src/ovl/RPG_BAT/drama_move_and_partner_assists.c:166. Original object 0x8013A480-0x8013A650 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/RPG_BAT.h"

void rpg_move_assist_mira(void) {
    s32 temp_v1;

    switch (rpg_player_step) {
    case 6:
        rpg_girl_run_in();
    default:
        break;
    case 7:
        strcpy(rpg_text_player_0, "鏡「つまらないわ…よっ！");
        rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x1F4);
        rpg_actor_set_animation_alt(0x1D, 0xA, 1, 1);
        rpg_voice_play_once(0x71, 0x11000078, 0);
        rpg_player_step_next();
        break;
    case 8:
        rpg_player_wait_step(0x5A);
        break;
    case 9:
        rpg_girl_effect_a();
        temp_v1 = rpg_player_timer + 1;
        rpg_player_timer = temp_v1;
        if (temp_v1 >= 0x12D) {
            rpg_message_frame = rpg_message_line_frames + 1;
            rpg_actor_set_animation_alt(0x1D, 0, 1, 1);
            rpg_player_step_next();
        }
        if (cd_audio_state() == 4) {
            rpg_voice_playing_seen = 1;
        }
        if (rpg_voice_playing_seen == 1) {
            if (cd_audio_state() == 2) {
                rpg_message_frame = rpg_message_line_frames + 1;
                rpg_actor_set_animation_alt(0x1D, 0, 1, 1);
                rpg_voice_playing_seen = 0;
                rpg_player_step_next();
            }
        }
        break;
    case 10:
        rpg_girl_effect_a();
        rpg_player_wait_step(0x1E);
        break;
    case 11:
        rpg_girl_run_out();
        break;
    case 12:
        rpg_player_step = 0x64;
        break;
    }
}
