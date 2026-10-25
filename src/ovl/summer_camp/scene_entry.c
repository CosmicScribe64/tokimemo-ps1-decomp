/* scene_entry.c: src/ovl/KANGEI/80132000.c:4: mode 0x60 scene dispatcher and return-to-title branch. Original object 0x80132000-0x80132090 (config/objects/KANGEI.txt). */
#include "common.h"
#include "ovl/summer_camp.h"

void camp_scene_entry_mode_gate(void) {
    portrait_wink_update();
    if (game_state.mode_stage == 0) {
        camp_mode_step_callback_dispatch();
    } else {
        game_mode_reset_to_title();
    }
    check_k_scroll();
    k_disp_inc2();
    hud_screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    hizuke_show();
    scene_frame_update();
}
