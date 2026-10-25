/* move_assist_yuina.c: src/ovl/RPG_BAT/80136530.c:4: girl 2 assist; body 7: switch (rpg_player_step) {; 9: rpg_girl_run_in(); girl-index assist dispatch src/ovl/RPG_BAT/drama_move_and_partner_assists.c:166. Original object 0x80136530-0x801368B0 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/RPG_BAT.h"

void rpg_move_assist_yuina(void) {
    s32 temp_v0;

    switch (rpg_player_step) {
    case 6:
        rpg_girl_run_in();
    default:
        break;
    case 7:
        strcpy(rpg_text_player_0, "紐緒「馬鹿な事言ってるんじゃないわ");
        rpg_message_show((s32) &rpg_text_player_pointers, 1, 0x1F4);
        rpg_voice_play_once(0x27, 0x0600002D, 0);
        rpg_actor_set_animation_alt(0x1D, 0xB, 1, 1);
        rpg_player_step_next();
        break;
    case 8:
        rpg_player_wait_step(0x1E);
        break;
    case 9:
        temp_v0 = rpg_player_timer + 1;
        rpg_player_timer = temp_v0;
        if (temp_v0 >= 0x12D) {
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
        rpg_move_assist_yuina_flash();
        break;
    case 11:
        rpg_move_assist_yuina_flash();
        break;
    case 12:
        rpg_move_assist_yuina_flash();
        break;
    case 13:
        rpg_move_assist_yuina_flash();
        break;
    case 14:
        rpg_move_assist_yuina_flash();
        break;
    case 15:
        rpg_player_wait_step(0x1E);
        break;
    case 16:
        rpg_girl_run_out();
        break;
    case 17:
        rpg_player_step = 0x64;
        break;
    }
}

void rpg_move_assist_yuina_flash(void) {
    D_8012139E = 0x82;
    switch (rpg_scratch) {
    case 0:
        rpg_actor_set_animation_alt(0x32, 6, 5, 1);
        rpg_actor_set_animation(0x33, 8, 1, 0);
        rpg_scratch += 1;
        break;
    case 1:
        rpg_move_timer += 1;
        if (rpg_move_timer >= 7) {
            rpg_scratch += 1;
        }
        break;
    case 2:
        rpg_actor_set_animation(0x34, 0xC, 1, 1);
        rpg_actor_set_position(0x33, 0, 0);
        rpg_actor_set_animation(0x33, 8, 1, 1);
        rpg_se_play_latched_6xx(0x513, 1, 5);
        rpg_scratch += 1;
        break;
    case 3:
        rpg_move_timer += 1;
        if (rpg_move_timer >= 0xA) {
            rpg_scratch += 1;
        }
        break;
    case 4:
        D_8012139E = 1;
        rpg_move_timer = 0;
        rpg_scratch = 0;
        rpg_player_step_next();
        break;
    }
}
