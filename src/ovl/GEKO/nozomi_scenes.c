/* nozomi_scenes.c: src/ovl/GEKO/8013DB80.c:18,39,54; Nozomi selector1, script/talk callbacks and score initialization. Current GEKO folder retained; no narrative meaning is inferred from callback ordinals. Original object 0x8013DB80-0x8013DF70 (config/objects/GEKO.txt). */
#include "common.h"
#include "ovl/GEKO.h"

void func_8013DB80(void) {
    D_80146DA0 = 0x801CE124;
    D_80146DA4 = 0x801CE128;
    D_80146DA8 = 0x801CE148;
    D_80146DAC = *(s16 *)0x801CE15C;
    D_80146DB0 = 0x801B0000;
    D_80146DB4 = 0x801B2000;
    D_80146DB8 = 0x801B6000;
    D_80146DBC = 0x801BA000;
    D_80146DC0 = 0x801BE000;
    D_80146DC4 = 0x801C2000;
    D_80146DC8 = 0x801C6000;
}

void func_8013DC30(void) {
    if (talk_work == 1) {
        func_8013DC70();
        return;
    }
    game_mode_reset_to_title();
}

void func_8013DC70(void) {
    if (game_state.mode_stage == 0) {
        func_8013DCAC();
        return;
    }
    game_mode_reset_to_title();
}

typedef struct {
    void (*f[33])();
} FnTbl33; /* size 0x84 */
extern FnTbl33 D_80146DCC;

void func_8013DCAC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl33 tbl;

    tbl = D_80146DCC;
    idx = game_state.mode_step;
    tbl.f[idx]();
}

void func_8013DD20(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8947);
    func_8013DB80();
    mode_step_next();
}

void func_8013DD58(void) {
    game_state.current_girl = GIRL_NOZOMI;
    draw2d3d(1, 0);
    actor_draw_enable(1);
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
    talk_names_set(game_state.current_girl);
    talk_clear();
    func_80137990();
    D_80146274 = D_80145F84;
    D_80146278 = D_801460C0;
    D_8014627C = D_801461FC;
    strcpy(D_800CA19C, "廊下");
    game_state.girls[6].tokimeki += 1;
    game_state.girls[6].affection += 2;
    check_para_limit();
    D_800CA368 = 0x28;
    mode_step_next();
}

void func_8013DE90(void) {
    func_8013DB80();
    load_palette(D_80146DB0, 0x11, 1, 2, 0);
    scene_still_show(D_80146DB4, D_80146DB8, D_80146DBC, D_80146DC0, D_80146DC4, D_80146DC8);
    scene_anim_show(D_80146DA4, D_80146DA8, D_80146DA0, D_80146DAC);
    D_800CA360 = 1;
    mode_step_next();
}

void func_8013DF40(void) {
    bg_read_sub2(0x403B);
    mode_step_next();
}
