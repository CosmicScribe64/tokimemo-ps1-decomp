/* seasonal_mode_stage_dispatch.c: ; src/ovl/SHOUGATU/80132000.c:52-86 dispatches seven mode stages and per-frame presentation. Original object 0x80132000-0x80132550 (config/objects/SHOUGATU.txt). */
#include "common.h"
#include "ovl/SHOUGATU.h"

void new_year_resource_addresses_init(void) {
    D_80143A40 = 0x80197424;
    D_80143A44 = 0x801988E8;
    D_80143A48 = 0x8019AA94;
    D_80143A4C = 0x8019C9A8;
    D_80143A50 = 0x8019E944;
    D_80143A54 = 0x801A098C;
    D_80143A58 = 0x801A2A60;
    D_80143A5C = 0x801A4BB4;
    D_80143A60 = 0x801A6C30;
    D_80143A64 = 0x801A8CE4;
    D_80143A68 = 0x801AAF98;
    D_80143A6C = 0x801AD2A0;
    D_80143A70 = 0x801AE3D0;
    D_80143A74 = 0x801AF56C;
    D_80143A78 = 0x801B0650;
    D_80143A7C = 0x801974DC;
    D_80143A80 = 0x80198A58;
    D_80143A84 = 0x8019AC44;
    D_80143A88 = 0x8019CB0C;
    D_80143A8C = 0x8019EAC0;
    D_80143A90 = 0x801A0B04;
    D_80143A94 = 0x801A2BD4;
    D_80143A98 = 0x801A4D38;
    D_80143A9C = 0x801A6DA4;
    D_80143AA0 = 0x801A8E5C;
    D_80143AA4 = 0x801AB124;
    D_80143AA8 = 0x801AD428;
    D_80143AAC = 0x801AE3D8;
    D_80143AB0 = 0x801AF6E8;
    D_80143AB4 = 0x801B0658;
    D_80143AB8 = 0x801978F0;
    D_80143ABC = 0x80199734;
    D_80143AC0 = 0x8019BAC8;
    D_80143AC4 = 0x8019D704;
    D_80143AC8 = 0x8019F860;
    D_80143ACC = 0x801A1850;
    D_80143AD0 = 0x801A39AC;
    D_80143AD4 = 0x801A5AB0;
    D_80143AD8 = 0x801A7BD0;
    D_80143ADC = 0x801A9CC0;
    D_80143AE0 = 0x801AC018;
    D_80143AE4 = 0x801AE318;
    D_80143AE8 = 0x801AE3F8;
    D_80143AEC = 0x801B0584;
    D_80143AF0 = 0x801B0678;
}

void new_year_mode_handler(void) {
    portrait_wink_update();
    switch (game_state.mode_stage) {
    case 0:
        new_year_scene_step_dispatch();
        break;
    case 1:
        new_year_result_step_dispatch();
        break;
    case 2:
        new_year_dialogue_mode_step_dispatch();
        break;
    case 3:
        new_year_shrine_dialogue_mode_step_dispatch();
        break;
    case 4:
        new_year_shrine_actor_mode_step_dispatch();
        break;
    case 5:
        new_year_omikuji_mode_step_dispatch();
        break;
    case 6:
        new_year_secondary_scene_step_dispatch();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    check_k_scroll();
    k_disp_inc2();
    hud_screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    hizuke_show();
    scene_frame_update();
}

s32 new_year_talk_run_current(void) {
    talk_tables = &D_80143B00;
    D_800CA138 = &D_80143B04;
    D_800CA13C = D_80143AF4;
    D_800CA140 = D_80143AF8;
    D_800CA144 = D_80143AFC;
    talk_run(D_80122CDC, 1, 0);
}

void new_year_talk_run_default(void) {
    talk_tables = &D_80143B00;
    D_800CA138 = &D_80143B04;
    D_800CA13C = D_80143AF4;
    D_800CA140 = D_80143AF8;
    D_800CA144 = D_80143AFC;
    talk_run(0xFF, 1, 0);
}

void new_year_talk_run_current_with_flags(void) {
    talk_tables = (u8 *) &D_80143B00;
    D_800CA138 = (u8 *) &D_80143B04;
    D_800CA13C = D_80143AF4;
    D_800CA140 = D_80143AF8;
    D_800CA144 = D_80143AFC;
    talk_run(D_80122CDC, 1, 2);
}
