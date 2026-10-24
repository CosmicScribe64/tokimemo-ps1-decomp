#define SHOUGATU_OVERRIDE_func_80137AB4 /* matched with implicit int return view (SHOUGATU.h: void) */
#include "common.h"
#include "ovl/SHOUGATU.h"

s32 func_80137AB4(void);

void func_801390C0(void) {
    D_80145270 = (u8 *)0x801E8088;
    D_80145274 = (u8 *)0x801E808C;
    D_80145278 = (u8 *)0x801E80A0;
    D_8014527C = *(s16 *)0x801E80B4;
    D_80145280 = (u8 *)0x801B0000;
    D_80145284 = (u8 *)0x801B2000;
    D_80145288 = (u8 *)0x801B4000;
    D_8014528C = (u8 *)0x801B8000;
    D_80145290 = (u8 *)0x801BC000;
    D_80145294 = (u8 *)0x801C0000;
    D_80145298 = (u8 *)0x801C4000;
    D_8014529C = (u8 *)0x801C8000;
    D_801452A0 = (u8 *)0x801D0000;
    D_801452A4 = (u8 *)0x801D4000;
    D_801452A8 = (u8 *)0x801D8000;
    D_801452AC = (u8 *)0x801DC000;
    D_801452B0 = (u8 *)0x801E0000;
    D_801452B4 = (u8 *)0x801E4000;
}

void func_801391D4(void) {
    func_801391F4();
}

void func_801391F4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl62 tbl;

    tbl = D_801452B8;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
    if (D_80144E08 == 0) {
        if (D_80144E0C == 3) {
            D_80122CF4 = 0;
        }
    }
}

void func_801392A4(void) {
    k_reset(1);
    mode_step_next();
}

void func_801392CC(void) {
    cd_read_sectors(0x71, 0x801B0000, 0x85B6);
    func_801390C0();
    mode_step_next();
}

void func_80139304(void) {
    audio_command(0x24);
    audio_command(0x501);
    mode_step_next();
}

s32 func_80139334(void) {
    s32 x;

    if (D_80144E0C == 2) {
        game_state.current_girl = GIRL_MIRA;
    }
    /* FAKE: capturing implicit-int return of func_80137AB4 and referencing x after the branches reserves $v0 and colors D_80144E0C to $a0 and 2 to $v1. T-24130 */
    x = func_80137AB4();
    if (D_80144E0C == 2) {
        D_8011ED5B &= 0xFF7F;
    }
    if (x) {}
    if (D_80144E0C == 3) {
        D_80122D20 = 1;
        D_8011ED5B &= 0xFF7F;
    } else {
        D_80122D20 = 0;
    }
    if (x) {}
    if (D_80144E0C == 2 && check_end_k() != 0 &&
        (*(u8 *)&game_state.voice_mode == 0 || (*(u8 *)&game_state.voice_mode != 0 && func_80046094() == 9))) {
        k_reset(1);
        D_80122CE4 = 0;
        D_80122CF0 = 0;
        D_8011ED5B &= 0xFF7F;
        D_800CA2AC = 0;
        game_state.mode_step_timer.w = 0;
    } else if (game_state.mode_step_timer.u++ >= 0x1AU) {
        if (D_80144E0C == 3 && check_end_k() != 0 &&
            (*(u8 *)&game_state.voice_mode == 0 || (*(u8 *)&game_state.voice_mode != 0 && func_80046094() == 9))) {
            D_80122D20 = 1;
            D_80144E08 += 1;
            D_80122CE4 = 0;
            D_80122CF0 = 0;
            D_8011ED5B &= 0xFF7F;
            D_800CA2AC = 0;
            D_80144E0C = 0;
            mode_step_next();
        }
    }
}

void func_80139514(void) {
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
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
    *(u8 *)&game_state.date_girl = *(u8 *)&game_state.current_girl = 7;
    talk_names_set(*(u8 *)&game_state.current_girl);
    func_80136F90();
    D_80144DFC = D_80144D9C;
    D_80144E00 = D_80144DC0;
    D_80144E04 = D_80144DE4;
    mode_step_next();
}

void func_8013960C(void) {
    func_801390C0();
    if (((u8)game_state.month >= 6U) && ((u8)game_state.month < 0xAU)) {
        load_palette((s32)D_80145280, 0x11, 1, 2, 0);
        scene_still_show((s32)D_80145288, (s32)D_8014528C, (s32)D_80145290, (s32)D_80145294, (s32)D_80145298, (s32)D_8014529C);
    } else {
        load_palette((s32)D_80145284, 0x11, 1, 2, 0);
        scene_still_show((s32)D_801452A0, (s32)D_801452A4, (s32)D_801452A8, (s32)D_801452AC, (s32)D_801452B0, (s32)D_801452B4);
    }
    scene_anim_show((s32)D_80145274, (s32)D_80145278, (s32)D_80145270, (s32)D_8014527C);
    D_800CA360 = 1;
    mode_step_next();
}

void func_80139734(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    strcpy(D_800CA19C, "廊下");
    mode_step_next();
}

void func_80139778(void) {
    bg_read_sub2(0x4045);
    mode_step_next();
}

void func_801397A0(void) {
    bg_read_sub2(0x46F0);
    mode_step_next();
}
