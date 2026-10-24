#include "common.h"
#include "ovl/GEKO.h"

void func_8013C4C0(void) {
    D_801469F0 = 0x801CE120;
    D_801469F4 = 0x801CE124;
    D_801469F8 = 0x801CE144;
    D_801469FC = (*(s16 *)0x801CE158);
    D_80146A00 = 0x801B0000;
    D_80146A04 = 0x801B2000;
    D_80146A08 = 0x801B6000;
    D_80146A0C = 0x801BA000;
    D_80146A10 = 0x801BE000;
    D_80146A14 = 0x801C2000;
    D_80146A18 = 0x801C6000;
}

void func_8013C570(void) {
    if (dialogue_work == 1) {
        func_8013C5B0();
        return;
    }
    game_reset_to_title();
}

void func_8013C5B0(void) {
    if (game_state.stage == 0) {
        func_8013C5EC();
        return;
    }
    game_reset_to_title();
}

typedef struct {
    void (*f[32])();
} FnTbl32; /* size 0x80 */
extern FnTbl32 D_80146A1C;

void func_8013C5EC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl32 tbl;

    tbl = D_80146A1C;
    idx = game_state.step;
    tbl.f[idx](0x80);
}

void func_8013C674(void) {
    D_80122D10 = 2;
    D_80120652 = 0;
    main_step_next();
}

void func_8013C6A4(void) {
    if ((u16) D_800CA154 == 0) {
        game_state.current_girl = GIRL_SOTOI;
    } else if ((u16) D_800CA154 == 1) {
        game_state.current_girl = GIRL_YOSHIO;
    } else {
        game_state.current_girl = GIRL_AYAKO;
    }
    func_80138AF8();
}

void func_8013C704(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x829A);
    func_8013C4C0();
    main_step_next();
}

void func_8013C73C(void) {
    game_state.current_girl = GIRL_AYAKO;
    draw2d3d(1, 0);
    actors_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    message_window_set_title("");
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    func_80137990();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    dec_bg_brightness = 0;
    dialogue_names_set(game_state.current_girl);
    dialogue_clear();
    func_80137990();
    D_80146274 = D_80145F20;
    D_80146278 = D_8014605C;
    D_8014627C = D_80146198;
    strcpy(D_800CA19C, "校舎裏");
    game_state.girls[3].affection += 2;
    check_para_limit();
    D_800CA368 = 0x15;
    main_step_next();
}

void func_8013C87C(void) {
    func_8013C4C0();
    load_palette(D_80146A00, 0x11, 1, 2, 0);
    event_still_show(D_80146A04, D_80146A08, D_80146A0C, D_80146A10, D_80146A14, D_80146A18);
    event_anim_show(D_801469F4, D_801469F8, D_801469F0, D_801469FC);
    D_800CA360 = 1;
    main_step_next();
}

void func_8013C92C(void) {
    bg_read_sub2(0x407A);
    main_step_next();
}

void func_8013C954(void) {
    bg_read_sub2(0x46BC);
    main_step_next();
}
