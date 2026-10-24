#include "common.h"
#include "ovl/SHOUGATU.h"

void func_8013B5B0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl25 tbl;

    tbl = D_80145A14;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void func_8013B62C(void) {
    audio_bank_load_start(1, 0xBF98, 0xBF79, 0xCA95, 0xCA4F, 0xCA3E);
    if (audio_bank_load_poll() == 1) {
        audio_command(0x202);
        mode_step_next();
    }
    mode_step_next();
}

void func_8013B694(void) {
    if (audio_bank_load_poll() == 1) {
        audio_command(0x202);
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void func_8013B71C(void) {
    cd_read_sectors(9, 0x80197000, 0xAFB3);
    func_8013B210();
    mode_step_next();
}

void func_8013B758(void) {
    s16 i;

    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    strcpy(D_800CA19C, "廊下");
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
    /* FAKE: the byte-typed store as the argument keeps the constant in $a0 (T-9020 idiom); T-9320 */
    talk_names_set(*(u8 *)&game_state.current_girl = 0xC);
    func_8013B210();
    D_801459FC = D_8014596C;
    D_80145A00 = D_801459A0;
    D_80145A04 = D_801459D4;
    D_80145A08 = 0;
    D_80145A0C = 0;
    D_80145A10 = 0;
    for (i = 0; i < 0xB; i++) {
        if (game_state.girls[i].flags.b[1] & 1) {
            D_80145A10 = 1;
        }
    }
    mode_step_next();
}

void func_8013B8CC(void) {
    event_log_add(7, 0);
    bg_read_sub2(0x4045);
    mode_step_next();
}

void func_8013B900(void) {
    D_80145A08 += ((u8) game_state.year >= 0x61U) * 4;
    mode_step_next();
}

void func_8013B944(void) {
    if (D_80145A10 == 0) {
        D_80145A08 += 1;
    }
    mode_step_next();
}

void func_8013B984(void) {
    if (D_80145A10 != 0) {
        mode_step_next();
        return;
    }
    func_8013B528();
}

void func_8013B9C0(void) {
    if (D_80145A10 != 0) {
        mode_stage_next();
        return;
    }
    mode_step_next();
}
