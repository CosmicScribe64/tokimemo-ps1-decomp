/* move_assist_yumi.c: src/ovl/RPG_BAT/801496F0.c:4: girl 10 assist; body 7: switch (rpg_player_step) {; 9: rpg_girl_run_in(); girl-index assist dispatch src/ovl/rpg_battle/drama_move_and_partner_assists.c:166. Original object 0x801496F0-0x801498C0 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/rpg_battle.h"

void rpg_move_assist_yumi(void) {
    s32 temp_v0;

    switch (rpg_player_step) {
    case 6:
        rpg_girl_run_in();
    default:
        break;
    case 7:
        strcpy(rpg_text_player_0, "早乙女「優美つまんなーい！");
        rpg_message_show((s32) &rpg_text_player_pointers, 1, 500);
        rpg_actor_set_animation_alt(0x1D, 9, 1, 1);
        rpg_voice_play_once(0xB0, 0x190000B7, 0);
        rpg_player_step_next();
        break;
    case 8:
        rpg_player_wait_step(0x28);
        break;
    case 9:
        temp_v0 = rpg_player_channel.unk_04 + 1;
        rpg_player_channel.unk_04 = temp_v0;
        if (temp_v0 >= 0x12D) {
            rpg_message.unk_08 = rpg_message_line_frames + 1;
            rpg_actor_set_animation_alt(0x1D, 0, 1, 1);
            rpg_player_step_next();
        }
        if (cd_audio_state() == 4) {
            rpg_voice_playing_seen = 1;
        }
        if (rpg_voice_playing_seen == 1) {
            if (cd_audio_state() == 2) {
                rpg_message.unk_08 = rpg_message_line_frames + 1;
                rpg_actor_set_animation_alt(0x1D, 0, 1, 1);
                rpg_voice_playing_seen = 0;
                rpg_player_step_next();
            }
        }
        break;
    case 10:
        rpg_girl_effect_a();
        rpg_player_wait_step(0x32);
        break;
    case 11:
        rpg_girl_run_out();
        break;
    case 12:
        rpg_player_step = 0x64;
        break;
    }
}
