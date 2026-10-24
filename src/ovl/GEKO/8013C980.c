#define MAIN_API_OVERRIDE_talk_work /* matched as u32 (main_api.h: s32), switch selector in $v1 (T-6010) */
#include "common.h"
#include "ovl/GEKO.h"

extern u32 talk_work;

typedef struct {
    void (*f[38])();
} FnTbl38; /* size 0x98 */
extern FnTbl38 D_80146ACC;

typedef struct {
    void (*f[22])();
} FnTbl22; /* size 0x58 */
extern FnTbl22 D_80146B64;

void func_8013C980(void) {
    D_80146AA0 = 0x801CE218;
    D_80146AA4 = 0x801CE21C;
    D_80146AA8 = 0x801CE25C;
    D_80146AAC = *(s16 *)0x801CE27C;
    D_80146AB0 = 0x801B0000;
    D_80146AB4 = 0x801B2000;
    D_80146AB8 = 0x801B6000;
    D_80146ABC = 0x801BA000;
    D_80146AC0 = 0x801BE000;
    D_80146AC4 = 0x801C2000;
    D_80146AC8 = 0x801C6000;
}

void func_8013CA30(void) {
    switch (talk_work) {
    case 1:
        func_8013CA94();
        return;
    case 8:
        func_8013CC60();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void func_8013CA94(void) {
    if (game_state.mode_stage == 0) {
        func_8013CAD0();
        return;
    }
    game_mode_reset_to_title();
}

void func_8013CAD0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl38 tbl;

    tbl = D_80146ACC;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void func_8013CB58(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8440);
    func_8013C980();
    mode_step_next();
}

void func_8013CB90(void) {
    D_800B5A60 = 1;
    game_state.current_girl = GIRL_SOTOI;
    strcpy(name_code_girl_short, "男子１");
    D_80120666 = 4;
    mode_step_next();
}

void func_8013CBE4(void) {
    strcpy(name_code_girl_short, "男子２");
    D_80120666 = 5;
    mode_step_next();
}

/* FAKE: scalar byte store followed by member read preserves argument/constant scheduling. T-9550 */
void func_8013CC20(void) {
    D_800B5A60 = 0;
    *(u8 *)&game_state.current_girl = 7;
    talk_names_set(game_state.current_girl);
    D_80120666 = 2;
    mode_step_next();
}

void func_8013CC60(void) {
    if (game_state.mode_stage == 0) {
        func_8013CC9C();
        return;
    }
    game_mode_reset_to_title();
}

void func_8013CC9C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl22 tbl;

    tbl = D_80146B64;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void func_8013CD18(void) {
    s16 i;

    game_state.current_girl = GIRL_MIRA;
    draw2d3d(1, 0);
    actor_draw_enable(1);
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
    for (i = 0; i < 5; i++) {
        game_state.tpage_buf[i + 11] = 1;
        game_state.tpage_buf[i + 43] = 1;
        game_state.tpage_buf[i + 21] = 1;
        game_state.tpage_buf[i + 53] = 1;
    }
    addr_init_bustup(i, &game_state);
    func_80137990();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    dec_bg_brightness = 0;
    talk_names_set(game_state.current_girl);
    talk_clear();
    func_80137990();
    D_80146274 = D_80145F38;
    D_80146278 = D_80146074;
    D_8014627C = D_801461B0;
    strcpy(D_800CA19C, "廊下");
    game_state.girls[7].affection += 1;
    check_para_limit();
    D_800CA368 = 0x2E;
    mode_step_next();
}

void func_8013CE98(void) {
    func_8013C980();
    load_palette(D_80146AB0, 0x11, 1, 2, 0);
    scene_still_show(D_80146AB4, D_80146AB8, D_80146ABC, D_80146AC0, D_80146AC4, D_80146AC8);
    scene_anim_show(D_80146AA4, D_80146AA8, D_80146AA0, D_80146AAC);
    mode_step_next();
}

void func_8013CF40(void) {
    game_state.current_girl = GIRL_MIRA;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    message_window_set_title("");
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
    D_800CA150 = 8;
    D_800CA154 = 0;
    addr_init_bustup();
    func_80137990();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    dec_bg_brightness = 0;
    talk_names_set(game_state.current_girl);
    talk_clear();
    func_80137990();
    D_80146274 = D_80145F50;
    D_80146278 = D_8014608C;
    D_8014627C = D_801461C8;
    strcpy(D_800CA19C, "廊下");
    D_800CA368 = 0x35;
    mode_step_next();
}

void func_8013D06C(void) {
    bg_read_sub2(0x403B);
    mode_step_next();
}

void func_8013D094(void) {
    bg_read_sub2(0x403B);
    mode_step_next();
}

void func_8013D0BC(void) {
    bg_read_sub2(0x403B);
    mode_step_next();
}
