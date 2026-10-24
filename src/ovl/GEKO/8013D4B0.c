#define MAIN_API_OVERRIDE_dialogue_work /* matched as u32 (main_api.h: s32), switch selector in $v1 (T-6010) */
#include "common.h"
#include "ovl/GEKO.h"

extern u32 dialogue_work;

typedef struct {
    void (*f[32])();
} FnTbl32; /* size 0x80 */
extern FnTbl32 D_80146CBC;

void func_8013D4B0(void) {
    bg_read_sub2(0x46E7);
    main_step_next();
}

void func_8013D4D8(void) {
    bg_read_sub2(0x403B);
    main_step_next();
}

void func_8013D500(void) {
    D_80146C90 = 0x801CE0F4;
    D_80146C94 = 0x801CE0F8;
    D_80146C98 = 0x801CE118;
    D_80146C9C = (*(s16 *)0x801CE12C);
    D_80146CA0 = 0x801B0000;
    D_80146CA4 = 0x801B2000;
    D_80146CA8 = 0x801B6000;
    D_80146CAC = 0x801BA000;
    D_80146CB0 = 0x801BE000;
    D_80146CB4 = 0x801C2000;
    D_80146CB8 = 0x801C6000;
}

void func_8013D5B0(void) {
    switch (dialogue_work) {
    case 1:
        func_8013D614();
        return;
    case 2:
        func_8013D650();
        return;
    default:
        game_reset_to_title();
        return;
    }
}

void func_8013D614(void) {
    if (game_state.stage == 0) {
        func_8013D68C();
        return;
    }
    game_reset_to_title();
}

void func_8013D650(void) {
    if (game_state.stage == 0) {
        func_8013D74C();
        return;
    }
    game_reset_to_title();
}

void func_8013D68C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl32 tbl;

    tbl = D_80146CBC;
    idx = game_state.step;
    tbl.f[idx](0x80);
}

void func_8013D714(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x87D9);
    func_8013D500();
    main_step_next();
}

typedef struct {
    void (*f[23])();
} FnTbl23; /* size 0x5C */
extern FnTbl23 D_80146D3C;

void func_8013D74C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl23 tbl;

    tbl = D_80146D3C;
    idx = game_state.step;
    tbl.f[idx]();
}

void func_8013D7D4(void) {
    game_state.current_girl = GIRL_YUKARI;
    draw2d3d(1, 0);
    actors_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
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
    D_80146274 = D_80145F68;
    D_80146278 = D_801460A4;
    D_8014627C = D_801461E0;
    strcpy(D_800CA19C, "廊下");
    if (dialogue_work == 1) {
        game_state.girls[5].affection += 1;
        game_state.girls[5].syoushin -= 1;
        check_para_limit();
    }
    D_800CA368 = 0x21;
    main_step_next();
}

void func_8013D91C(void) {
    func_8013D500();
    load_palette(D_80146CA0, 0x11, 1, 2, 0);
    event_still_show(D_80146CA4, D_80146CA8, D_80146CAC, D_80146CB0, D_80146CB4, D_80146CB8);
    event_anim_show(D_80146C94, D_80146C98, D_80146C90, D_80146C9C);
    main_step_next();
}

void func_8013D9C4(void) {
    game_state.current_girl = GIRL_YUKARI;
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
    D_80146274 = D_80145F6C;
    D_80146278 = D_801460A8;
    D_8014627C = D_801461E4;
    strcpy(D_800CA19C, "廊下");
    game_state.girls[5].tokimeki += 1;
    check_para_limit();
    D_800CA368 = 0x22;
    main_step_next();
}

void func_8013DB04(void) {
    bg_read_sub2(0x4045);
    main_step_next();
}

void func_8013DB2C(void) {
    bg_read_sub2(0x46CD);
    main_step_next();
}

void func_8013DB54(void) {
    bg_read_sub2(0x4045);
    main_step_next();
}
