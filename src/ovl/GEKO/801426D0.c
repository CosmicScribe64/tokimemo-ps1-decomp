#include "common.h"
#include "ovl/GEKO.h"

void func_801426D0(void) {
    bustup_wink_update();
    if (game_state.stage == 0) {
        func_801429D4();
    } else {
        game_reset_to_title();
    }
    check_k_scroll();
    k_disp_inc2();
    screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    hizuke_show();
    func_80083A10();
}
