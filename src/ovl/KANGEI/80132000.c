#include "common.h"
#include "ovl/KANGEI.h"

void func_80132000(void) {
    bustup_wink_update();
    if (game_state.stage == 0) {
        func_80132214();
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
