/* sick_visit_scene_resource_addresses.c: ; src/ovl/OMIMAI/80132AD0.c:3-34 initializes secondary scene resource address tables in the sick-visit overlay. Original object 0x80132AD0-0x801331B0 (config/objects/OMIMAI.txt). */
#include "ovl/sick_visit.h"

void sick_visit_mode_stage1_girl_talk_pointer_setup(void) {
    D_80134B60[0] = 0x801B00DC;
    D_80134B60[1] = 0x801B02AC;
    D_80134B60[2] = 0x801B03C8;
    D_80134B60[3] = 0x801B04C4;
    D_80134B60[4] = 0x801B0668;
    D_80134B60[5] = 0x801B0808;
    D_80134B60[6] = 0x801B09DC;
    D_80134B60[7] = 0x801B0BE8;
    D_80134B60[8] = 0x801B0D8C;
    D_80134B60[9] = 0x801B0F5C;
    D_80134B60[10] = 0x801B1164;
    D_80134B60[11] = 0x801B12D8;
    D_80134B60[12] = 0x801B1454;
    D_80134B94[0] = 0x801B00F4;
    D_80134B94[1] = 0x801B02C8;
    D_80134B94[2] = 0x801B03D0;
    D_80134B94[3] = 0x801B04DC;
    D_80134B94[4] = 0x801B0680;
    D_80134B94[5] = 0x801B0820;
    D_80134B94[6] = 0x801B09F8;
    D_80134B94[7] = 0x801B0C00;
    D_80134B94[8] = 0x801B0DA4;
    D_80134B94[9] = 0x801B0F78;
    D_80134B94[10] = 0x801B1180;
    D_80134B94[11] = 0x801B12E0;
    D_80134B94[12] = 0x801B1470;
    D_80134BC8[0] = 0x801B01A8;
    D_80134BC8[1] = 0x801B0398;
    D_80134BC8[2] = 0x801B03F0;
    D_80134BC8[3] = 0x801B0590;
    D_80134BC8[4] = 0x801B0734;
    D_80134BC8[5] = 0x801B08D4;
    D_80134BC8[6] = 0x801B0AE4;
    D_80134BC8[7] = 0x801B0CB4;
    D_80134BC8[8] = 0x801B0E58;
    D_80134BC8[9] = 0x801B1064;
    D_80134BC8[10] = 0x801B126C;
    D_80134BC8[11] = 0x801B1338;
    D_80134BC8[12] = 0x801B155C;
}

typedef struct {
    void (*f[12])();
} FnTbl12; /* size 0x30 */
extern FnTbl12 D_80134BFC;

void sick_visit_mode_stage1_mode_step_callback_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl12 tbl;

    tbl = D_80134BFC;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
    hud_screen_frame_draw(2);
}

void sick_visit_mode_stage1_sector_load(void) {
    cd_read_sectors(3, 0x801B0000, 0xAF43);
    sick_visit_mode_stage1_girl_talk_pointer_setup();
    mode_step_next();
}

void sick_visit_mode_stage1_day_event_mark(void) {
    event_log_add(0xD, 0x25);
    game_state.day_event_done_bits |= 4;
    talk_text_table = D_80134A8C;
    talk_command_table = D_80134ABC;
    talk_voice_table = D_80134AEC;
    talk_group = 2;
    talk_line = 0;
    mode_step_next();
}

void sick_visit_mode_stage1_girl_dialogue_setup(void) {
    sick_visit_mode_stage1_talk_table_select();
    talk_group = 1;
    talk_line = 0;
    mode_step_next();
}

void sick_visit_mode_stage1_talk_table_select(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        talk_text_table = D_80134B60[1];
        talk_command_table = D_80134B94[1];
        talk_voice_table = D_80134BC8[1];
        return;
    case GIRL_MIO:
        talk_text_table = D_80134B60[6];
        talk_command_table = D_80134B94[6];
        talk_voice_table = D_80134BC8[6];
        return;
    case GIRL_YUINA:
        talk_text_table = D_80134B60[3];
        talk_command_table = D_80134B94[3];
        talk_voice_table = D_80134BC8[3];
        return;
    case GIRL_AYAKO:
        talk_text_table = D_80134B60[4];
        talk_command_table = D_80134B94[4];
        talk_voice_table = D_80134BC8[4];
        return;
    case GIRL_SAKI:
        talk_text_table = D_80134B60[10];
        talk_command_table = D_80134B94[10];
        talk_voice_table = D_80134BC8[10];
        return;
    case GIRL_YUKARI:
        talk_text_table = D_80134B60[7];
        talk_command_table = D_80134B94[7];
        talk_voice_table = D_80134BC8[7];
        return;
    case GIRL_NOZOMI:
        talk_text_table = D_80134B60[8];
        talk_command_table = D_80134B94[8];
        talk_voice_table = D_80134BC8[8];
        return;
    case GIRL_MIRA:
        talk_text_table = D_80134B60[5];
        talk_command_table = D_80134B94[5];
        talk_voice_table = D_80134BC8[5];
        return;
    case GIRL_YUKO:
        talk_text_table = D_80134B60[0];
        talk_command_table = D_80134B94[0];
        talk_voice_table = D_80134BC8[0];
        return;
    case GIRL_MEGUMI:
        talk_text_table = D_80134B60[9];
        talk_command_table = D_80134B94[9];
        talk_voice_table = D_80134BC8[9];
        return;
    case GIRL_YUMI:
        talk_text_table = D_80134B60[12];
        talk_command_table = D_80134B94[12];
        talk_voice_table = D_80134BC8[12];
    }
}

void sick_visit_common_mode_dispatch(void) {
    portrait_wink_update();
    if (game_state.mode_stage == 0) {
        sick_visit_common_mode_step_callback_dispatch();
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
