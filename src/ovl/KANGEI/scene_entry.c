/* scene_entry.c: src/ovl/KANGEI/80132000.c:4: mode 0x60 scene dispatcher and return-to-title branch. Original object 0x80132000-0x80132090 (config/objects/KANGEI.txt). */
#include "common.h"
#include "ovl/KANGEI.h"

void func_80132000(void) {
    portrait_wink_update();
    if (game_state.mode_stage == 0) {
        func_80132214();
    } else {
        game_mode_reset_to_title();
    }
    check_k_scroll();
    k_disp_inc2();
    hud_screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    hizuke_show();
    func_80083A10();
}
