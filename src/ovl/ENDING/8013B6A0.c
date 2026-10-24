#include "common.h"
#include "ovl/ENDING.h"
#include "game.h"

void func_8013B6A0(void) {
    D_8013CBB0.unk_00 = 0x801C22CC;
    D_8013CBB0.unk_04 = 0x801C22D4;
    D_8013CBB0.unk_08 = 0x801C22F0;
    D_8013CBB0.unk_0C = *(s16 *)0x801C2300;
    D_8013CBB0.unk_10[0] = 0x801A0000;
    D_8013CBB0.unk_10[1] = 0x801C2000;
    D_8013CBB0.unk_10[2] = 0x801AA000;
    D_8013CBB0.unk_10[3] = 0x801AE000;
    D_8013CBB0.unk_10[4] = 0x801B2000;
    D_8013CBB0.unk_10[5] = 0x801B6000;
    D_8013CBB0.unk_10[6] = 0x801BA000;
    D_8013CBB0.unk_10[7] = 0x801BE000;
}

void func_8013B760(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    EdFnTbl25 tbl;

    tbl = D_8013CBE4;
    if (game_state.mode_stage == 7 && game_state.mode_step < 0x10) {
        portrait_wink_update();
    }
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
    if (game_state.mode_stage == 7) {
        if (game_state.mode_step < 0x10) {
            check_k_scroll();
            k_disp_inc2();
            hud_screen_frame_draw(2);
            message_window_show();
            message_window_title_update();
            hizuke_show();
            func_80083A10();
        }
    }
}

void func_8013B868(void) {
    mode_stage_set(6);
}

void func_8013B888(void) {
    mode_stage_set(8);
}

void func_8013B8A8(void) {
    if (game_state.scene_flags[82] != 0) {
        mode_step_next();
        return;
    }
    normal_date_girl_out();
}

void func_8013B8E4(void) {
    cd_read_sectors(0x45, 0x801A0000, 0x727A);
    func_8013B6A0();
    mode_step_next();
}

s32 func_8013B91C(void) {
    if (game_state.scene_flags[82] != 0) {
        if ((u32) game_state.mode_step_timer.w++ < 0x78U) {
            return 0;
        }
    }
    game_state.current_girl = GIRL_YOSHIO;
    talk_group = 0;
    talk_line = 0;
    talk_names_set(game_state.scene_flags[82]);
    func_80132000();
    talk_text_table = D_8013C280[0][0];
    talk_command_table = D_8013C280[1][0];
    talk_voice_table = D_8013C280[2][0];
    if (game_state.scene_flags[82] != 0) {
        talk_group = 2;
        game_state.mode_step += 0xA;
    }
    mode_step_next();
}

void func_8013B9F0(void) {
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    cursor_show(0);
    back_clear_switch(1);
    tpage_buf_clear_all();
    game_state.tpage_buf[28] = 1;
    game_state.tpage_buf[29] = 1;
    game_state.tpage_buf[30] = 1;
    text_system_reset();
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    talk_clear();
    game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0;
    D_800B5940 = 0;
    func_8013B6A0();
    load_palette(D_8013CBB0.unk_10[1], 0x11, 1, 2, 0);
    load_palette(D_8013CBB0.unk_10[0], 0x12, 1, 2, 0);
    scene_still_show(D_8013CBB0.unk_10[2], D_8013CBB0.unk_10[3], D_8013CBB0.unk_10[4], D_8013CBB0.unk_10[5], D_8013CBB0.unk_10[6], D_8013CBB0.unk_10[7]);
    scene_anim_show(D_8013CBB0.unk_04, D_8013CBB0.unk_08, D_8013CBB0.unk_00, D_8013CBB0.unk_0C);
    sprite_quads[61].h = sprite_quads[62].h = sprite_quads[63].h = 0x70;
    actor_clear(0x61);
    D_80120666 = 0;
    D_80120654 = 8;
    D_80120693 = 0x12;
    D_80120652 = 0;
    D_80120653 = 4;
    mode_step_next();
}

void func_8013BB9C(void) {
    s32 r;
    EdWords4 v;

    v = D_8013CD90;
    audio_command(0xBF);
    if (game_state.scene_flags[82] != 0) {
        cd_audio_request_set(v.w[2], v.w[3], 0xF);
    } else {
        cd_audio_request_set(v.w[0], v.w[1], 0xF);
    }
    audio_command(0x300);
    r = set_kanji_string(-0x50, 0x50, 0, "『女々しい野郎どもの詩』", 0);
    k_disp_start(r);
    D_8013CBE0 = 0;
    mode_step_next();
}

void func_8013BC58(void) {
    EdShorts13 tbl;
    s16 i;
    s16 sel;

    tbl = D_8013CDA0;
    sel = 0xFF;
    for (i = 0; i < 13; i++) {
        if (D_8013CBE0 >= tbl.h[i]) {
            sel = i;
        }
    }
    if (D_8013CBE0 == 0x3E8) {
        k_reset(1, sel, &tbl);
    }
    if (sel != 0xFF) {
        if (D_8013CBE0 == tbl.h[sel]) {
            k_reset(1, sel, &tbl);
            func_8013BDD8(sel);
        }
    }
    if (D_8013CBE0 == 0x1536) {
        k_reset(1);
    }
    if (cd_audio_state() & 4) {
        D_8013CBE0 += 1;
    }
    if (func_80046094() == 9) {
        mode_step_next();
    }
}

void func_8013BDD8(s16 arg0) {
    u8 *p;

    p = D_8013CC48 + arg0 * 0x19;
    k_disp_start(set_kanji_string((s16) (-strlen(p) * 7 / 2), 0x50, 0, p, 0));
}

void func_8013BE64(void) {
    if (game_state.mode_step_timer.w == 0) {
        D_80120652 = 5;
    }
    if ((D_80120668 == 2) && (D_80120658 == 4)) {
        D_80120652 = 1;
        D_80120668 = 0;
        D_80120658 = 0;
        D_80120666 = 1;
    }
    if ((u32)++game_state.mode_step_timer.w >= 0xB5U) {
        mode_step_next();
    }
}
