/* sick_visit_frame_dispatch.c: ; src/ovl/OMIMAI/80132000.c:3-21 dispatches sick-visit stages and updates the shared display. Original object 0x80132000-0x801320A0 (config/objects/OMIMAI.txt). */
#include "ovl/OMIMAI.h"

void func_80132000(void) {
    portrait_wink_update();
    switch (game_state.mode_stage) {
    case 0:
        func_801322E4();
        break;
    case 1:
        func_80132D44();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    check_k_scroll();
    k_disp_inc2();
    message_window_show();
    message_window_title_update();
    hizuke_show();
    func_80083A10();
}
