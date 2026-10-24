#include "common.h"
#include "ovl/ENDING.h"
#include "game.h"

void func_80133030(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    EdFnTbl16 tbl;

    tbl = D_8013C370;
    bustup_wink_update();
    idx = game_state.step;
    tbl.f[idx](0x80);
    check_k_scroll();
    k_disp_inc2();
    screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    hizuke_show();
    func_80083A10();
}

void func_801330E4(void) {
    s32 pad[4]; /* FAKE: T-25145, four unused word slots above rank: frame 0xB0, chance at 0x30 */
    s16 best;
    s16 *p;
    u8 rank[32];
    s16 points[16];
    s16 i; /* declared after points: level gets the home 0x54 it is saved to around rand() */
    s16 level;
    EdShorts18 chance;

    chance = D_8013C3B0;
    game_state.year = 0x62;
    game_state.month = 3;
    game_state.day = 1;
    game_state.weekday = WEEKDAY_SUNDAY;
    func_80053418(rank, points);
    game_state.current_girl = GIRL_YOSHIO;
    if (points[0] < 0x1F4 && game_state.girls[0xC].flags.b[3] >= 0x46 && game_state.event_flags[0x4F] == 2) {
        game_state.current_girl = GIRL_REI;
    } else {
        best = 0x1F3;
        for (i = 0; i < 0xB; i++) {
            p = &points[i];
            if (game_state.girls[i].flags.f.met && best < *p) {
                game_state.current_girl = i;
                best = *p;
            }
        }
    }
    if (game_state.current_girl == GIRL_YOSHIO) {
        /* FAKE: T-25145, a second read of the flag word: IDO 5.3 then keeps the word in a register
         * (a0), as the original compiler did for its one read (wiki/original-compiler.md rule 3),
         * so ugen shifts it into v0 and copies the f9 temporary back after the sum */
        if (game_state.girls[0xB].flags.u) {
        }
        level = game_state.girls[0xB].flags.f.story_stage;
        level += (game_state.event_flags[0x4C] > 0) * 2;
        level += (game_state.event_flags[0x4E] > 0) * 2;
        if (level >= 0x12) {
            level = 0x11;
        }
        if (game_state.girls[0xB].flags.f.story_stage != 0 && game_state.event_flags[0x4D] == 0) {
            if (rand() % 256 < chance.v[level]) {
                game_state.current_girl = GIRL_MIHARU;
            }
        }
    }
    draw2d3d(1, 0);
    actors_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    strcpy(D_800CA19C, "廊下");
    strcpy(D_800CA1DC, "教室");
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    dialogue_clear();
    game_state.screen_brightness = 0x80;
    dec_bg_brightness = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    dialogue_group = 0;
    dialogue_line = 0;
    dialogue_names_set(game_state.current_girl);
    func_80132000();
    dialogue_text_table = D_8013C280[0][3];
    dialogue_command_table = D_8013C280[1][3];
    dialogue_voice_table = D_8013C280[2][3];
    main_step_next();
}

void func_80133400(void) {
    bg_read_sub2(0x4045);
    main_step_next();
}

void func_80133428(void) {
    cd_read_sectors(0x11, 0x801F0000, 0xAE07);
    func_80132000();
    main_step_next();
}

void func_80133460(void) {
    if (((u8) game_state.current_girl >= GIRL_YOSHIO) && ((u32) ((u32) (game_state.girls[14].flags.w << 0x14) >> 0x1D) < 3U)) {
        main_stage_set(7);
        D_800E7D34 |= 8;
        return;
    }
    main_step_next();
    D_800E7D34 |= 4;
}

void func_801334E4(void) {
    if ((((u32)(game_state.girls[1].flags.w << 0x1E) >> 0x1F)) && (get_h_tokimeki(GIRL_MIO) >= 0x46U)) {
        normal_date_speak();
        return;
    }
    main_step_next();
}
