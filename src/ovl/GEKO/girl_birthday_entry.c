/* girl_birthday_entry.c: src/ovl/GEKO/80134C90.c:4; only mode-stage0 dispatches the girl-birthday callbacks, then common text/HUD updates run. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x80134C90-0x80134D10 (config/objects/GEKO.txt). */
#include "common.h"
#include "ovl/GEKO.h"

void func_80134C90(void) {
    portrait_wink_update();
    if (game_state.mode_stage == 0) {
        func_801351F8();
    } else {
        game_mode_reset_to_title();
    }
    check_k_scroll();
    k_disp_inc2();
    message_window_show();
    message_window_title_update();
    hizuke_show();
}
