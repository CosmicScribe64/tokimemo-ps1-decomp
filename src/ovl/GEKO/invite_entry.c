/* invite_entry.c: src/ovl/GEKO/801426D0.c:4; invitation mode-stage0 entry with shared per-frame text/HUD updates. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x801426D0-0x80142760 (config/objects/GEKO.txt). */
#include "common.h"
#include "ovl/GEKO.h"

void encounter_invite_mode_handler(void) {
    portrait_wink_update();
    if (game_state.mode_stage == 0) {
        encounter_invite_dispatch();
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
