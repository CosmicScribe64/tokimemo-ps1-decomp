#include "ovl/OMIMAI.h"

void func_80132AD0(void) {
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

void func_80132D44(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl12 tbl;

    tbl = D_80134BFC;
    idx = game_state.step;
    tbl.f[idx](0x80);
    screen_frame_draw(2);
}

void func_80132DC0(void) {
    cd_read_sectors(3, 0x801B0000, 0xAF43);
    func_80132AD0();
    main_step_next();
}

void func_80132DF8(void) {
    event_log_add(0xD, 0x25);
    game_state.daily_event_bits |= 4;
    dialogue_text_table = D_80134A8C;
    dialogue_command_table = D_80134ABC;
    dialogue_voice_table = D_80134AEC;
    dialogue_group = 2;
    dialogue_line = 0;
    main_step_next();
}

void func_80132E78(void) {
    func_80132EB0();
    dialogue_group = 1;
    dialogue_line = 0;
    main_step_next();
}

void func_80132EB0(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        dialogue_text_table = D_80134B60[1];
        dialogue_command_table = D_80134B94[1];
        dialogue_voice_table = D_80134BC8[1];
        return;
    case GIRL_MIO:
        dialogue_text_table = D_80134B60[6];
        dialogue_command_table = D_80134B94[6];
        dialogue_voice_table = D_80134BC8[6];
        return;
    case GIRL_YUINA:
        dialogue_text_table = D_80134B60[3];
        dialogue_command_table = D_80134B94[3];
        dialogue_voice_table = D_80134BC8[3];
        return;
    case GIRL_AYAKO:
        dialogue_text_table = D_80134B60[4];
        dialogue_command_table = D_80134B94[4];
        dialogue_voice_table = D_80134BC8[4];
        return;
    case GIRL_SAKI:
        dialogue_text_table = D_80134B60[10];
        dialogue_command_table = D_80134B94[10];
        dialogue_voice_table = D_80134BC8[10];
        return;
    case GIRL_YUKARI:
        dialogue_text_table = D_80134B60[7];
        dialogue_command_table = D_80134B94[7];
        dialogue_voice_table = D_80134BC8[7];
        return;
    case GIRL_NOZOMI:
        dialogue_text_table = D_80134B60[8];
        dialogue_command_table = D_80134B94[8];
        dialogue_voice_table = D_80134BC8[8];
        return;
    case GIRL_MIRA:
        dialogue_text_table = D_80134B60[5];
        dialogue_command_table = D_80134B94[5];
        dialogue_voice_table = D_80134BC8[5];
        return;
    case GIRL_YUKO:
        dialogue_text_table = D_80134B60[0];
        dialogue_command_table = D_80134B94[0];
        dialogue_voice_table = D_80134BC8[0];
        return;
    case GIRL_MEGUMI:
        dialogue_text_table = D_80134B60[9];
        dialogue_command_table = D_80134B94[9];
        dialogue_voice_table = D_80134BC8[9];
        return;
    case GIRL_YUMI:
        dialogue_text_table = D_80134B60[12];
        dialogue_command_table = D_80134B94[12];
        dialogue_voice_table = D_80134BC8[12];
    }
}

void func_80133120(void) {
    bustup_wink_update();
    if (game_state.stage == 0) {
        func_80133424();
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
