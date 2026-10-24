#include "common.h"
#include "ovl/KANGEI.h"

typedef struct {
    void (*f[15])();
} FnTbl15; /* size 0x3C */
extern FnTbl15 D_80139AE8;

void func_80133C10(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl15 tbl;

    tbl = D_80139AE8;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void func_80133C84(void) {
    game_state.current_girl = game_state.date_girl;
    mode_stage_next();
}

void func_80133CB0(void) {
    cd_read_sectors(0x3B, 0x80197000, 0xAE60);
    func_80132E74();
    mode_step_next();
}

void func_80133CEC(void) {
    cd_read_sectors(0x2D, 0x801B4400, 0xAE9B);
    func_80132E40();
    mode_step_next();
}

void func_80133D28(void) {
    s16 i;
    u32 v;

    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    back_clear_switch(1);
    actor_clear_range(0);
    message_window_init();
    hizuke_init();
    tpage_buf_clear();
    text_system_reset();
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    dec_bg_brightness = 0;
    talk_clear();
    parameter_disp_switch(0);
    icon_disp_switch(0);
    D_80139ADC = 0;
    D_80139AE0 = 0;
    addr_init_bustup();
    text_marker_hide();
    game_state.current_girl = 0xFF;
    v = game_state.player_flags.h >> 12;
    for (i = 0; i < 0xB; i++) {
        if (v == (game_state.girls[i].flags.b[2] >> 4)) {
            game_state.current_girl = i;
            break;
        }
    }
    if (v == 0xA && game_state.girls[0].tokimeki + game_state.girls[0].affection < game_state.girls[10].tokimeki + game_state.girls[10].affection && game_state.year >= 0x60) {
        game_state.current_girl = GIRL_YUMI;
    }
    game_state.date_girl = game_state.current_girl;
    talk_names_set(game_state.current_girl);
    func_80132E74();
    D_80139AD0 = (KObj *)D_80139A5C[0][6];
    D_80139AD4 = D_80139A5C[1][6];
    D_80139AD8 = D_80139A5C[2][6];
    D_80139AE4 = 0;
    D_80139AC0 = 0;
    mode_step_next();
}

void func_80133EF8(void) {
    talk_tables = (u8 *)&D_80139ADC;
    D_800CA138 = (u8 *)&D_80139AE0;
    D_800CA13C = (s32)D_80139AD0;
    D_800CA140 = D_80139AD4;
    D_800CA144 = D_80139AD8;
    talk_run(0xFF, 1, *(u8 *)&D_80139AE4);
}

void func_80133F74(void) {
    talk_tables = (u8 *)&D_80139ADC;
    D_800CA138 = (u8 *)&D_80139AE0;
    D_800CA13C = (s32)D_80139AD0;
    D_800CA140 = D_80139AD4;
    D_800CA144 = D_80139AD8;
    talk_run(D_80122CDC, 1, *(u8 *)&D_80139AE4);
}

void func_80133FF4(void) {
    talk_tables = (u8 *)&D_80139ADC;
    D_800CA138 = (u8 *)&D_80139AE0;
    D_800CA13C = (s32)D_80139AD0;
    D_800CA140 = D_80139AD4;
    D_800CA144 = D_80139AD8;
    if (talk_run(0xFF, 1, *(u8 *)&D_80139AE4) == 1) {
        mode_step_next();
    }
}

void func_80134084(void) {
    RECT rect;

    dec_bg_reset();
    rect.x = 0x140;
    rect.y = 0x100;
    rect.w = 0x140;
    rect.h = 0xF8;
    func_8009C7F8(&rect, 8, 0, 0);
    rect.x = 0x2C0;
    rect.y = 0;
    func_8009C7F8(&rect, 8, 0, 0);
    bg_read_sub2(0x428D);
    mode_step_next();
    strcpy(D_800CA19C, "合宿所");
}
