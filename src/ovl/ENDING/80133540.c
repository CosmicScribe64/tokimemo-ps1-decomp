#include "common.h"
#include "ovl/ENDING.h"
#include "game.h"

void func_80133540(void) {
    s32 idx; /* FAKE: never read; declared first so tbl lands at the original frame offset (T-3330) */
    EdFnTbl43 tbl;

    tbl = D_8013C3E4;
    portrait_wink_update();
    tbl.f[game_state.mode_step](0x80);
    check_k_scroll();
    k_disp_inc2();
    hud_screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    hizuke_show();
    func_80083A10();
}

void func_801335F4(void) {
    if (D_8013C3E0 == 0xC) {
        portrait_layout_set(4);
    } else {
        portrait_layout_set(0);
    }
    mode_step_next();
}

void func_8013363C(void) {
    memcpy((void *)(0x801A0000 + game_state.mode_step_timer.w * 0x2800), 0x80180000 + game_state.mode_step_timer.w * 0x2800, 0x2800);
    game_state.mode_step_timer.w += 1;
    if (game_state.mode_step_timer.w == 9) {
        mode_step_next();
    }
}

void func_801336A8(void) {
    s32 pad; /* FAKE: unused local reproduces the extra 4-byte frame slot above rect; real source unknown. T-4030 */
    RECT rect;
    rect.x = 0x140;
    rect.y = 0x80;
    rect.w = 0x180;
    rect.h = 0x80;
    func_8009C884(&rect, (void *)0x80180000);
    game_state.tpage_buf[37] = 1;
    game_state.tpage_buf[38] = 1;
    game_state.tpage_buf[39] = 1;
    game_state.tpage_buf[40] = 1;
    game_state.tpage_buf[41] = 1;
    game_state.tpage_buf[42] = 1;
    mode_step_next();
}

void func_80133738(void) {
    memcpy((void *)(0x80180000 + game_state.mode_step_timer.w * 0x2800), 0x801A0000 + game_state.mode_step_timer.w * 0x2800, 0x2800);
    game_state.mode_step_timer.w += 1;
    if (game_state.mode_step_timer.w == 9) {
        mode_step_next();
    }
}

void func_801337A4(void) {
    RECT rect;
    rect.x = 0x140;
    rect.y = 0x80;
    rect.w = 0x180;
    rect.h = 0x80;
    func_8009C8E0(&rect, (void *)0x80180000);
    mode_step_next();
}

void func_801337F0(void) {
    if (talk_line == 4) {
        portrait_layout_set(4);
        load_palette(D_800CA130, 0x10, 1, 2, 1);
    }
    normal_date_speak();
    if (talk_line == 7) {
        D_80122D20 = 1;
        return;
    }
    D_80122D20 = 0;
}

void func_80133870(void) {
    if (game_state.date_girl == GIRL_SOTOI) {
        portrait_read_start(0x6B24);
    } else {
        portrait_read_start(0x5828);
    }
    mode_step_next();
}

void func_801338B8(void) {
    load_palette(D_800CA130, 0x10, 1, 2, 1);
    mode_step_next();
}

s32 func_801338F8(void) {
    if ((u32)game_state.girls[14].flags.w << 0x14 >> 0x1D < 3U) {
        mode_stage_next();
        return 0;
    }
    D_8013C3E0 = game_state.current_girl;
    game_state.current_girl = GIRL_REI;
    game_state.date_girl = GIRL_REI;
    talk_group = 0;
    talk_line = 0;
    strcpy(name_code_girl_short, "外井");
    func_80132000();
    talk_text_table = D_8013C280[0][15];
    talk_command_table = D_8013C280[1][15];
    talk_voice_table = D_8013C280[2][15];
    mode_step_next();
}

INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/80133540.rodata", D_8013BFB8);

void func_801339C0(void) {
    game_state.date_girl = GIRL_REI;
    strcpy(name_code_girl_short, D_8013BFB8);
    mode_step_next();
}

INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/80133540.rodata", D_8013BFC0);

void func_80133A00(void) {
    game_state.date_girl = GIRL_SOTOI;
    strcpy(name_code_girl_short, D_8013BFC0);
    mode_step_next();
}

void func_80133A40(void) {
    game_state.current_girl = D_8013C3E0;
    mode_step_next();
}

void func_80133A6C(void) {
    if (game_state.current_girl >= GIRL_YOSHIO) {
        mode_stage_set(7);
        D_800E7D34 |= 8;
        return;
    }
    mode_step_next();
    D_800E7D34 |= 4;
}

void func_80133AD0(void) {
    s32 idx; /* FAKE: never read; declared first so tbl lands at the original frame offset (T-3330) */
    EdFnTbl9 tbl;

    tbl = D_8013C490;
    portrait_wink_update();
    tbl.f[game_state.mode_step](0x80);
    check_k_scroll();
    k_disp_inc2();
    hud_screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    hizuke_show();
    func_80083A10();
}

void func_80133B80(void) {
    talk_group = 0;
    talk_line = 0;
    func_80132000();
    talk_text_table = D_8013C280[0][13];
    talk_command_table = D_8013C280[1][13];
    talk_voice_table = D_8013C280[2][13];
    mode_step_next();
}

void func_80133BE0(void) {
    bg_read_sub2(0x4055);
    mode_step_next();
}
