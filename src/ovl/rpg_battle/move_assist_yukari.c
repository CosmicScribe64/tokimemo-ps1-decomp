/* move_assist_yukari.c: src/ovl/RPG_BAT/80143830.c:4: girl 5 assist; body 7: switch (rpg_player_step) {; 10: rpg_voice_play_once(0x79, 0x12000085, 0); girl-index assist dispatch src/ovl/rpg_battle/drama_move_and_partner_assists.c:166. Original object 0x80143830-0x801439E0 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/rpg_battle.h"

void rpg_move_assist_yukari(void) {
    s32 temp_v0;

    switch (rpg_player_step) {
    case 6:
        strcpy(rpg_text_player_0, "古式「まぁ、おもしろいですねぇ。");
        rpg_voice_play_once(0x79, 0x12000085, 0);
        rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x1F4);
        rpg_actor_set_animation(0x1D, 6, 1, 1);
        rpg_player_step_next();
    default:
        break;
    case 7:
        temp_v0 = rpg_player_timer + 1;
        rpg_player_timer = temp_v0;
        if (temp_v0 >= 0x12D) {
            rpg_actor_set_animation(0x1D, 0, 1, 1);
            rpg_message_frame = rpg_message_line_frames + 1;
            rpg_player_step_next();
        }
        if (cd_audio_state() == 4) {
            rpg_voice_playing_seen = 1;
        }
        if (rpg_voice_playing_seen == 1) {
            if (cd_audio_state() == 2) {
                rpg_actor_set_animation(0x1D, 0, 1, 1);
                rpg_message_frame = rpg_message_line_frames + 1;
                rpg_voice_playing_seen = 0;
                rpg_player_step_next();
            }
        }
        break;
    case 8:
        rpg_actor_set_animation(0x1D, 0, 1, 1);
        rpg_player_step_next();
        break;
    case 9:
        rpg_player_wait_step(0x1E);
        break;
    case 10:
        rpg_player_step = 0x64;
        break;
    }
}
