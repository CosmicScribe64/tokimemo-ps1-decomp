#define MAIN_API_OVERRIDE_dialogue_work /* matched as u32 (main_api.h: s32), switch selector in $v1 (T-6010) */
#include "common.h"
#include "ovl/DATE.h"

extern u32 dialogue_work;

void func_8015A190(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8E3F);
    func_80159560();
    main_step_next();
}

void func_8015A1C8(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl44 tbl;

    tbl = D_801609F8;
    idx = game_state.step;
    tbl.f[idx](0x80);
}

void func_8015A250(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8E7C);
    func_80159610();
    main_step_next();
}

void func_8015A288(void) {
    s32 idx;
    DateDispatch8015A288 tbl;
    tbl = D_80160AA8;
    if ((game_state.step >= 0x1D) && (game_state.step < 0x26)) {
        light_effect_draw();
    }
    idx = game_state.step;
    tbl.f[idx](0x3C);
}

void func_8015A328(void) {
    snd_command(0x501);
    main_step_next();
}

void func_8015A350(void) {
    snd_command(0x500);
    main_step_next();
}

void func_8015A378(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x8EB9);
    func_801596C0();
    main_step_next();
}

void func_8015A3B0(void) {
    switch (dialogue_work) {
    case 3:
        func_8015A4A8();
        return;
    case 5:
        func_8015A58C();
        return;
    default:
        game_reset_to_title();
        return;
    }
}

void func_8015A414(void) {
    message_window_title_show(1);
    game_state.current_girl = GIRL_MIHARU;
    strcpy(name_buf_girl_short, "謎の女");
    event_log_add(0xB, D_80122D08);
    D_8015E208 = D_8015DF80;
    D_8015E20C = D_8015E0BC;
    D_8015E210 = D_8015E1F8;
    D_800CA150 = 0;
    D_800CA154 = 0;
    main_step_next();
}

void func_8015A4A8(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80160B80;
    idx = game_state.step;
    tbl.f[idx](0x80);
}

void func_8015A530(void) {
    game_state.current_girl = game_state.date_girl;
    dialogue_names_set(game_state.date_girl);
    main_step_next();
}

void func_8015A564(void) {
    bustup_read_start(0x6BAB);
    main_step_next();
}

void func_8015A58C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80160BD0;
    light_effect_draw();
    idx = game_state.step;
    tbl.f[idx](0x80);
}

void func_8015A610(void) {
    bustup_read_start(0x6BD8);
    main_step_next();
}
