#include "common.h"
#include "ovl/GEKO.h"

void func_80134C90(void) {
    bustup_wink_update();
    if (game_state.stage == 0) {
        func_801351F8();
    } else {
        game_reset_to_title();
    }
    check_k_scroll();
    k_disp_inc2();
    message_window_show();
    message_window_title_update();
    hizuke_show();
}
