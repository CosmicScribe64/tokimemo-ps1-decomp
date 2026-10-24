#include "ovl/OMIMAI.h"

void func_80132000(void) {
    bustup_wink_update();
    switch (game_state.stage) {
    case 0:
        func_801322E4();
        break;
    case 1:
        func_80132D44();
        break;
    default:
        game_reset_to_title();
        break;
    }
    check_k_scroll();
    k_disp_inc2();
    message_window_show();
    message_window_title_update();
    hizuke_show();
    func_80083A10();
}
