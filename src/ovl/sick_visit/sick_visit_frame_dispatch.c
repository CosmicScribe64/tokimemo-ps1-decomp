/* sick_visit_frame_dispatch.c: ; src/ovl/OMIMAI/80132000.c:3-21 dispatches sick-visit stages and updates the shared display. Original object 0x80132000-0x801320A0 (config/objects/OMIMAI.txt). */
#include "ovl/sick_visit.h"

void sick_visit_mode_stage_dispatch(void) {
    portrait_wink_update();
    switch (game_state.mode_stage) {
    case 0:
        sick_visit_mode_stage0_mode_step_callback_dispatch();
        break;
    case 1:
        sick_visit_mode_stage1_mode_step_callback_dispatch();
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
    scene_frame_update();
}
