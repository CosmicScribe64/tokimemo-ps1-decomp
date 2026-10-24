#include "ovl/VALEN.h"

typedef struct {
    void (*f[31])();
} FnTbl31; /* size 0x7C */
extern FnTbl31 D_801344A0;

void func_801323D0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl31 tbl;

    tbl = D_801344A0;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void func_8013244C(void) {
    audio_bank_load_start(1, 0xBF98, 0xBF79, 0xCA95, 0xCA4F, 0xCA3E);
    if (audio_bank_load_poll() == 1) {
        audio_command(0x201);
        mode_step_next();
    }
    mode_step_next();
}

void func_801324B4(void) {
    if (audio_bank_load_poll() == 1) {
        audio_command(0x201);
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void func_8013253C(void) {
    cd_read_sectors(7, 0x80197000, 0xAFAC);
    func_80132000();
    mode_step_next();
}

void func_80132578(void) {
    game_state.day_event_done_bits |= 0x10;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    strcpy(D_800CA19C, "廊下");
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    talk_clear();
    game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    func_80132000();
    D_8013448C = D_80134400;
    D_80134490 = D_80134434;
    D_80134494 = D_80134468;
    D_80134498 = 0;
    D_8013449C = 0;
    game_state.current_girl = GIRL_REI;
    talk_names_set(0xC);
    game_state.day_event_done_bits |= 8;
    mode_step_next();
}

void func_801326B8(void) {
    bg_read_sub2(0x4045);
    event_log_add(6, 0);
    mode_step_next();
}

void func_801326EC(void) {
    bg_read_sub2(0x4122);
    mode_step_next();
}

void func_80132714(void) {
    D_80134498 = (D_80134498 + (game_state.year * 4)) - 0x180;
    mode_step_next();
}
