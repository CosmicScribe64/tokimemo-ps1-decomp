/* saki_scenes.c: src/ovl/GEKO/8013EE40.c:23,39,54,65; Saki selector1, script/resource callbacks and scene-counter/condition changes. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x8013EE40-0x8013F360 (config/objects/GEKO.txt). */
#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*f[35])();
} FnTbl35; /* size 0x8C */
extern FnTbl35 D_80146FBC;

void func_8013EE40(void) {
    D_80146F90 = 0x801CE124;
    D_80146F94 = 0x801CE128;
    D_80146F98 = 0x801CE148;
    D_80146F9C = (*(s16 *)0x801CE15C);
    D_80146FA0 = 0x801B0000;
    D_80146FA4 = 0x801B2000;
    D_80146FA8 = 0x801B6000;
    D_80146FAC = 0x801BA000;
    D_80146FB0 = 0x801BE000;
    D_80146FB4 = 0x801C2000;
    D_80146FB8 = 0x801C6000;
}

void func_8013EEF0(void) {
    if (talk_work == 1) {
        func_8013EF30();
        return;
    }
    game_mode_reset_to_title();
}

void func_8013EF30(void) {
    if (game_state.mode_stage == 0) {
        func_8013EF6C();
        return;
    }
    game_mode_reset_to_title();
}

void func_8013EF6C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl35 tbl;

    tbl = D_80146FBC;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void func_8013EFF4(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8C0E);
    func_8013EE40();
    mode_step_next();
}

void func_8013F02C(void) {
    if (game_state.scene_flags[28] == 1 && game_state.scene_flags[27] == 1) {
        D_800CA150 = (u16) D_800CA150 + 4;
        D_800CA154 = 0;
    } else if (game_state.scene_flags[27] >= 2U) {
        D_800CA150 = (u16) D_800CA150 + 2;
        D_800CA154 = 0;
    }
    mode_step_next();
}

void func_8013F0BC(void) {
    game_state.current_girl = GIRL_SAKI;
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
    addr_init_bustup();
    func_80137990();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    dec_bg_brightness = 0;
    talk_names_set(game_state.current_girl);
    talk_clear();
    func_80137990();
    D_80146274 = D_80145FB4;
    D_80146278 = D_801460F0;
    D_8014627C = D_8014622C;
    strcpy(D_800CA19C, "廊下");
    if (game_state.scene_flags[27] >= 3U) {
        game_state.scene_flags[27] = 2;
    }
    if (game_state.scene_flags[28] == 0 && game_state.scene_flags[27] == 1) {
        game_state.girls[4].tokimeki += 1;
    }
    game_state.girls[4].affection += 1;
    game_state.param_condition.value += 0xA;
    check_para_limit();
    D_800CA368 = 0x1B;
    mode_step_next();
}

void func_8013F25C(void) {
    func_8013EE40();
    load_palette(D_80146FA0, 0x11, 1, 2, 0);
    scene_still_show(D_80146FA4, D_80146FA8, D_80146FAC, D_80146FB0, D_80146FB4, D_80146FB8);
    scene_anim_show(D_80146F94, D_80146F98, D_80146F90, D_80146F9C);
    D_800CA360 = 1;
    mode_step_next();
}

void func_8013F30C(void) {
    bg_read_sub2(0x4045);
    mode_step_next();
}

void func_8013F334(void) {
    bg_read_sub2(0x46E0);
    mode_step_next();
}
