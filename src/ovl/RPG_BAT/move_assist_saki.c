/* move_assist_saki.c: src/ovl/RPG_BAT/80147F10.c:4: girl 4 assist; body 7: switch (rpg_player_step) {; 9: rpg_girl_run_in(); girl-index assist dispatch src/ovl/RPG_BAT/drama_move_and_partner_assists.c:166. Original object 0x80147F10-0x801480F0 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/RPG_BAT.h"

void rpg_move_assist_saki(void) {
    s32 temp_v0;

    switch (rpg_player_step) {
    case 6:
        rpg_girl_run_in();
    default:
        break;
    case 7:
        strcpy(rpg_text_player_0, "虹野「な、なんだかな…");
        rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x1F4);
        rpg_voice_play_once(0x9A, 0x150000A8, 0);
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
        temp_v0 = rpg_player_timer + 1;
        rpg_player_timer = temp_v0;
        if (temp_v0 >= 0x12D) {
            rpg_actor_set_animation_alt(0x1D, 0, 1, 1);
            rpg_message_frame = rpg_message_line_frames + 1;
            rpg_player_step_next();
        }
        if (cd_audio_state() == 4) {
            rpg_voice_playing_seen = 1;
        }
        if (rpg_voice_playing_seen == 1) {
            if (cd_audio_state() == 2) {
                rpg_actor_set_animation_alt(0x1D, 0, 1, 1);
                rpg_message_frame = rpg_message_line_frames + 1;
                rpg_voice_playing_seen = 0;
                rpg_player_step_next();
            }
        }
        break;
    case 11:
        rpg_player_wait_step(0x1E);
        break;
    case 12:
        rpg_girl_run_out();
        break;
    case 13:
        rpg_player_step = 0x64;
        break;
    }
}
