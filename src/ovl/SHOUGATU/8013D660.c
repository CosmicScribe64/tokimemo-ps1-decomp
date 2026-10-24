#include "common.h"
#include "ovl/SHOUGATU.h"

void func_8013D660(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl33 tbl;

    tbl = D_80145F40;
    idx = game_state.step;
    tbl.f[idx](0x80);
}

void func_8013D6D4(void) {
    snd_bank_load_start(1, 0xC621, 0xC613, 0xD93B, 0xD8F0, 0xD8D0);
    if (snd_bank_load_poll() == 1) {
        main_step_next();
    }
    main_step_next();
}

void func_8013D734(void) {
    if (snd_bank_load_poll() == 1) {
        snd_command(0x202);
        main_step_next();
    }
    if ((u32) game_state.step_timer.w++ >= 0x400U) {
        cd_cmd_reset();
        snd_bank_state_reset();
        main_step_set((game_state.step - 1) & 0xFF);
    }
}

void func_8013D7BC(void) {
    cd_read_sectors(0xE, 0x80197000, 0xAFBC);
    func_8013C070();
    main_step_next();
}

void func_8013D7F8(void) {
    D_80122CFC = 0x1E;
    main_step_next();
}

void func_8013D820(void) {
    s16 thresh = game_state.girls[12].flags.f.story_stage * 25 + 150;

    if (game_state.param_athletics.value >= thresh &&
        game_state.girls[14].flags.f.story_stage != 0) {
        main_step_next();
        main_step_next();
        main_step_next();
        main_step_next();
        return;
    }
    if (game_state.param_looks.value >= (s16)(game_state.girls[12].flags.f.story_stage * 30 - (game_state.girls[12].flags.f.story_stage >= 2) * 10 + 70)) {
        main_step_next();
        return;
    }
    main_step_next();
    main_step_next();
    main_step_next();
    main_step_next();
}

void func_8013D92C(void) {
    s16 v;
    /* FAKE: GameState *g splits the second unk_1BC[14].unk_0C access from the first so uopt keeps it in $t2 instead of promoting to $v0. T-24130 */
    GameState *g = &game_state;

    v = game_state.girls[12].flags.f.story_stage * 25 + 150;
    if (game_state.param_athletics.value >= v) {
        D_80145F2C = (game_state.girls[14].flags.f.story_stage >= 1) * 3 + 4;
        g->girls[14].flags.f.story_stage += 1;
        main_step_next();
        return;
    }
    D_80145F2C += 1;
    game_state.param_guts.value += 10;
    game_state.param_stress.value += 10;
    check_para_limit();
    main_step_next();
    main_step_next();
    main_step_next();
    main_step_next();
    main_step_next();
    main_step_next();
    main_step_next();
    main_step_next();
}

void func_8013DA5C(void) {
    if (((u32) (game_state.girls[14].flags.w << 0x14) >> 0x1D) == 1) {
        func_8013D2A8();
        return;
    }
    main_step_next();
    main_step_next();
}

s32 func_8013DAAC(void) {
    GameState *g;

    if (game_state.girls[0].birthday.date.month == 12 &&
        game_state.girls[0].birthday.date.day == 24 &&
        !(*(u8 *)&game_state.daily_event_bits & 0x20)) {
        *(u8 *)&game_state.daily_event_bits |= 0x20;
        game_state.day_events[game_state.day_event_index].girl = 0;
        main_state_set(MODE_GIRL_BIRTHDAY);
        return 0;
    }
    if (((WordView *)&game_state.player_birthday)->date.month == 12 &&
        ((WordView *)&game_state.player_birthday)->date.day == 24 &&
        !(*(u8 *)&game_state.daily_event_bits & 0x40)) {
        *(u8 *)&game_state.daily_event_bits |= 0x40;
        main_state_set(MODE_PLAYER_BIRTHDAY);
        return 0;
    }
    draw2d3d(1, 0);
    actors_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    dialogue_clear();
    strcpy(D_800CA19C, "伊集院邸");
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0;
    D_800B5940 = 0;
    D_80145F2C = 0;
    D_80145F30 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    g = &game_state;
    strcpy(name_buf_family, g->player_family_name);
    if (g->girls[12].flags.f.story_stage >= 3) {
        g->girls[12].flags.f.story_stage = 2;
    }
    if (g->girls[12].flags.f.story_stage == 0) {
        strcpy(name_buf_girl_short, "黒服");
    } else {
        strcpy(name_buf_girl_short, "外井");
    }
    game_state.current_girl = GIRL_REI;
    func_8013C070();
    D_80145F20 = D_80145AFC;
    D_80145F24 = D_80145B30;
    D_80145F28 = D_80145B64;
    main_step_next();
}

void func_8013DCFC(void) {
    bg_read_sub2(0x41CA);
    main_step_next();
}

void func_8013DD24(void) {
    bustup_read_start(0x6B24);
    main_step_next();
}

void func_8013DD4C(void) {
    main_stage_next();
}

void func_8013DD6C(void) {
    main_stage_set(4);
}
