#include "common.h"
#include "ovl/DATE2.h"

typedef struct {
    void (*f[31])();
} FnTbl31; /* size 0x7C */
extern FnTbl31 D_8013A440;

void func_801326F0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl31 tbl;

    tbl = D_8013A440;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
    if (game_state.mode_stage == 0 && (u8)game_state.mode_step < 0x13U) {
        hud_screen_frame_draw(1);
    }
}

void func_8013279C(void) {
    s32 t;

    D_800B3D60 = 0;
    t = dec_bg_cd_read(0x3FC6, 0);
    if (t == dec_bg_show) {
        mode_step_next();
        mode_step_next();
        mode_step_next();
    } else if (t == 1 - dec_bg_show) {
        mode_step_next();
        mode_step_next();
    }
    mode_step_next();
}

s32 func_8013281C(void) {
    if ((((u32 *)D_80125C04)[0] & 0xFFFF0000) != 0x38000000 || (((u32 *)D_80125C04)[1] & 0xFF010000) != 0x10000) {
        dec_bg_reset();
        mode_step_next();
        game_state.mode_step -= 3;
        return 0;
    }
    dec_bg_decode_to_vram(0);
    mode_step_next();
}

void func_801328A8(void) {
    dec_bg_show_switch(0);
    mode_step_next();
}

/* FAKE: s32 fall-through preserves the original implicit v0 result. T-10010. */
s32 func_801328D0(void) {
    if ((game_state.pad_pressed & 0x400040) && game_state.mode_step_timer.u >= 2U) {
        audio_command(0x1508);
        D_80122D00 = 0;
        k_reset(1);
        D_800CA2AC = 0;
        D_8013A428 = 1;
        D_8013A42C = 0;
        mode_step_next();
        return 0;
    }
    if (check_end_k()) {
        if (game_state.substep++ < 0x40U) {
            return 0;
        }
        game_state.substep = 0;
        k_reset(1);
    }
    func_801325F4();
    if (game_state.mode_step_timer.u++ == 0) {
        if (D_8013A42C < 3 && D_8013A428 == 0) {
            audio_command(0x609);
        }
    }
}

void func_801329F0(void) {
    func_80132670();
    if (game_state.mode_step_timer.w++ == 0) {
        if (D_8013A42C == 1) {
            audio_command(0x60A);
        }
    }
}
void func_80132A48(void) {
    func_80132670();
    if (game_state.mode_step_timer.w++ == 0) {
        if (D_8013A42C == 1) {
            audio_command(0x60B);
        }
    }
}
void func_80132AA0(void) {
    D_800B3D60 = 0;
    audio_bank_load_start(1, 0xC657, 0xC623, 0xDA9F, 0xDA4F, 0xDA3D);
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    mode_step_next();
}

void func_80132B08(void) {
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void func_80132B88(void) {
    audio_command(0x200);
    mode_step_next();
}

void func_80132BB0(void) {
    cd_read_sectors(0x11, 0x80197000, 0xAEEC);
    func_80132000();
    mode_step_next();
}

void func_80132BEC(void) {
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    talk_clear();
    message_window_init();
    hizuke_init();
    text_marker_hide();
    mode_step_next();
}

void func_80132C60(void) {
    game_state.day_event_done_bits |= 0x10;
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    text_system_reset();
    strcpy(D_800CA19C, "遊園地");
    strcpy(D_800CA1DC, "園内");
    k_reset(1);
    talk_clear();
    game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0x80;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    text_marker_hide();
    talk_names_set(0);
    func_80132000();
    D_8013A404 = D_8013A3A8;
    D_8013A408 = D_8013A3D4;
    D_8013A40C = D_8013A400;
    D_8013A428 = 0;
    D_8013A42C = 0;
    D_8013A430 = 0;
    D_8013A434 = 0;
    D_8013A438 = 0;
    D_8013A43C = 0;
    mode_step_next();
}

void func_80132D84(void) {
    bg_read_sub2(0x432B);
    mode_step_next();
}

void func_80132DAC(void) {
    bg_read_sub2(0x4335);
    mode_step_next();
}
