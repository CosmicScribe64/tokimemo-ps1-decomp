#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*f[39])();
} FnTbl39; /* size 0x9C */
extern FnTbl39 D_80146BEC;

void func_8013D0F0(void) {
    D_80146BC0 = 0x801CE0F4;
    D_80146BC4 = 0x801CE0F8;
    D_80146BC8 = 0x801CE118;
    D_80146BCC = *(s16 *)0x801CE12C;
    D_80146BD0 = 0x801B0000;
    D_80146BD4 = 0x801B2000;
    D_80146BD8 = 0x801B6000;
    D_80146BDC = 0x801BA000;
    D_80146BE0 = 0x801BE000;
    D_80146BE4 = 0x801C2000;
    D_80146BE8 = 0x801C6000;
}

void func_8013D1A0(void) {
    if (dialogue_work == 1) {
        func_8013D1E0();
        return;
    }
    game_reset_to_title();
}

void func_8013D1E0(void) {
    if (game_state.stage == 0) {
        func_8013D21C();
        return;
    }
    game_reset_to_title();
}

void func_8013D21C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl39 tbl;

    tbl = D_80146BEC;
    idx = game_state.step;
    tbl.f[idx](0x80);
}

void func_8013D290(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8627);
    func_8013D0F0();
    main_step_next();
}

void func_8013D2C8(void) {
    game_state.current_girl = GIRL_MIO;
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
    hizuke_init();
    message_window_init();
    text_marker_hide();
    dialogue_names_set(1);
    dialogue_clear();
    func_80137990();
    D_80146274 = D_80145F54;
    D_80146278 = D_80146090;
    D_8014627C = D_801461CC;
    strcpy(D_800CA19C, "廊下");
    game_state.girls[1].tokimeki += 1;
    game_state.girls[1].affection += 2;
    check_para_limit();
    D_800CA368 = 8;
    main_step_next();
}

void func_8013D408(void) {
    func_8013D0F0();
    load_palette(D_80146BD0, 0x11, 1, 2, 0);
    event_still_show(D_80146BD4, D_80146BD8, D_80146BDC, D_80146BE0, D_80146BE4, D_80146BE8);
    event_anim_show(D_80146BC4, D_80146BC8, D_80146BC0, (s32) D_80146BCC);
    main_step_next();
}
