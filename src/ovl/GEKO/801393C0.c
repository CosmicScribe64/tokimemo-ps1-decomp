#include "common.h"
#include "ovl/GEKO.h"

void func_801393C0(void) {
    D_80146280 = (u8 *)0x801CE0FC;
    D_80146284 = (u8 *)0x801CE100;
    D_80146288 = (u8 *)0x801CE120;
    D_8014628C = *(s16 *)0x801CE13C;
    D_80146290 = (u8 *)0x801B0000;
    D_80146294 = (u8 *)0x801B2000;
    D_80146298 = (u8 *)0x801B6000;
    D_8014629C = (u8 *)0x801BA000;
    D_801462A0 = (u8 *)0x801BE000;
    D_801462A4 = (u8 *)0x801C2000;
    D_801462A8 = (u8 *)0x801C6000;
}

void func_80139470(void) {
    if (dialogue_work == 1) {
        func_801394B0();
        return;
    }
    game_reset_to_title();
}

void func_801394B0(void) {
    if (game_state.stage == 0) {
        func_801394EC();
        return;
    }
    game_reset_to_title();
}

typedef struct {
    void (*f[53])();
} FnTbl53; /* size 0xD4 */
extern FnTbl53 D_801462AC;

void func_801394EC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl53 tbl;

    tbl = D_801462AC;
    idx = game_state.step;
    tbl.f[idx](0x80);
    D_80120652 = 1;
}

void func_8013957C(void) {
    normal_date_move_place();
    if (game_state.substep++ == 0) {
        snd_command(0x501);
    }
}

void func_801395C0(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x7D96);
    func_801393C0();
    main_step_next();
}

void func_801395F8(void) {
    func_800634FC(get_g_zyotai_h(GIRL_YUKO));
    snd_command(0x501);
    dialogue_names_set(game_state.current_girl);
    main_step_next();
}

void func_80139640(void) {
    game_state.current_girl = GIRL_YUKO;
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
    strcpy(name_buf_girl_short, "先生");
    dialogue_clear();
    func_80137990();
    D_80146274 = D_80145EC0;
    D_80146278 = D_80145FFC;
    D_8014627C = D_80146138;
    strcpy(D_800CA19C, "廊下");
    strcpy(D_800CA1DC, "職員室");
    game_state.girls[8].affection += 1;
    check_para_limit();
    D_800CA368 = 0x36;
    main_step_next();
}

void func_801397A8(void) {
    func_801393C0();
    load_palette(D_80146290, 0x11, 1, 2, 0);
    event_still_show((s32) D_80146294, (s32) D_80146298, (s32) D_8014629C, (s32) D_801462A0, (s32) D_801462A4, (s32) D_801462A8);
    event_anim_show((s32) D_80146284, (s32) D_80146288, (s32) D_80146280, (s32) D_8014628C);
    D_80120666 = 4;
    D_80122D10 = 1;
    main_step_next();
}

void func_80139864(void) {
    bg_read_sub2(0x4045);
    main_step_next();
}

void func_8013988C(void) {
    bg_read_sub2(0x408D);
    main_step_next();
}

void func_801398B4(void) {
    bg_read_sub2(0x46D6);
    main_step_next();
}

void func_801398DC(void) {
    bg_read_sub2(0x4045);
    main_step_next();
}
