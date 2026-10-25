/* girl_action_mio.c: src/ovl/RPG_BAT/80155A70.c:6: girl 1 action; body 7: if (!(rpg_player_action & 0xFF) && !(rpg_enemy_action & 0xFF) && !(rpg_girl_action & 0xFF) && (rpg_girl_action & 0x80000000) &&; 9: rpg_player_pose_idle(); partner-id dispatch src/ovl/RPG_BAT/801439E0.c:24. Original object 0x80155A70-0x80155CC0 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/RPG_BAT.h"

/* s32 with no return (K&R implicit int): v0 stays live at the exit, so as1 leaves the bgez delay slot
 * empty instead of filling it with the next block's lui v0. T-16210 */
s32 rpg_girl_action_mio(void) {
    if (!(rpg_player_action & 0xFF) && !(rpg_enemy_action & 0xFF) && !(rpg_girl_action & 0xFF) && (rpg_girl_action & 0x80000000) &&
        rpg_player_hit_points_maximum / 10 < rpg_player_hit_points_maximum - rpg_player_hit_points) {
        rpg_player_pose_idle();
        rpg_girl_action = 1;
    }
    if (rpg_girl_action == 1) {
        switch (rpg_girl_step) {
        case 0:
            rpg_actor_set_animation(0x1D, 5, 1, 1);
            strcpy(rpg_text_girl_0, "如月「ああっ！　めまいが！");
            rpg_message_show((s32) rpg_text_girl_pointers, 1, 0x1F4);
            rpg_voice_play_once(0x03000008, 0x02000010, 0);
            rpg_girl_step_next();
            break;
        case 1:
            rpg_girl_wait_step(0x46);
            break;
        case 2:
            rpg_actor_set_animation(0x1D, 6, 1, 1);
            rpg_se_play_latched_5xx(0x511, 1, 0);
            rpg_girl_step_next();
            break;
        case 3:
            if (cd_audio_state() == 2) {
                rpg_message_frame = rpg_message_line_frames + 1;
                rpg_girl_step_next();
            }
            if (rpg_message_frame >= 0x1F5) {
                rpg_message_frame = rpg_message_line_frames + 1;
                rpg_girl_step_next();
            }
            break;
        case 4:
            rpg_girl_wait_step(0x14);
            break;
        case 5:
            rpg_actor_set_animation(0x1D, 7, 1, 1);
            rpg_se_play_latched_5xx(0x512, 1, 0);
            rpg_girl_step_next();
            break;
        case 6:
            rpg_girl_wait_step(0x1E);
            break;
        case 7:
            rpg_girl_action_end();
            rpg_girl_action = 0x1000;
            break;
        }
    }
}
