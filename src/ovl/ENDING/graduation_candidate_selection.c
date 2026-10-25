/* graduation_candidate_selection.c: src/ovl/ENDING/80133030.c:6,23; mode-step callbacks and graduation candidate ranking in ending_graduation_candidate_select; rank/points, special girl branches and strict-score comparisons. See wiki/spec/o-master-ending.md for rule details. Original object 0x80133030-0x80133540 (config/objects/ENDING.txt). */
#include "common.h"
#include "ovl/ENDING.h"
#include "game.h"

void ending_candidate_selection_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    EdFnTbl16 tbl;

    tbl = D_8013C370;
    portrait_wink_update();
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
    check_k_scroll();
    k_disp_inc2();
    hud_screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    hizuke_show();
    scene_frame_update();
}

void ending_graduation_candidate_select(void) {
    s32 pad[4]; /* FAKE: T-25145, four unused word slots above rank: frame 0xB0, chance at 0x30 */
    s16 best_points;
    s16 *candidate_points;
    u8 rank[32];
    s16 points[16];
    s16 i; /* declared after points: miharu_chance_level gets the home 0x54 it is saved to around rand() */
    s16 miharu_chance_level;
    EdShorts18 chance;

    chance = ending_miharu_chance_by_level;
    game_state.year = 0x62;
    game_state.month = 3;
    game_state.day = 1;
    game_state.weekday = WEEKDAY_SUNDAY;
    girl_scores(rank, points);
    game_state.current_girl = GIRL_YOSHIO;
    if (points[0] < 0x1F4 && game_state.girls[GIRL_REI].flags.b[3] >= 0x46 && game_state.scene_flags[0x4F] == 2) {
        game_state.current_girl = GIRL_REI;
    } else {
        best_points = 0x1F3;
        for (i = 0; i < 0xB; i++) {
            candidate_points = &points[i];
            if (game_state.girls[i].flags.f.met && best_points < *candidate_points) {
                game_state.current_girl = i;
                best_points = *candidate_points;
            }
        }
    }
    if (game_state.current_girl == GIRL_YOSHIO) {
        /* FAKE: T-25145, a second read of the flag word: IDO 5.3 then keeps the word in a register
         * (a0), as the original compiler did for its one read (wiki/original-compiler.md rule 3),
         * so ugen shifts it into v0 and copies the f9 temporary back after the sum */
        if (game_state.girls[GIRL_MIHARU].flags.u) {
        }
        miharu_chance_level = game_state.girls[GIRL_MIHARU].flags.f.story_stage;
        miharu_chance_level += (game_state.scene_flags[0x4C] > 0) * 2;
        miharu_chance_level += (game_state.scene_flags[0x4E] > 0) * 2;
        if (miharu_chance_level >= 0x12) {
            miharu_chance_level = 0x11;
        }
        if (game_state.girls[GIRL_MIHARU].flags.f.story_stage != 0 && game_state.scene_flags[0x4D] == 0) {
            if (rand() % 256 < chance.v[miharu_chance_level]) {
                game_state.current_girl = GIRL_MIHARU;
            }
        }
    }
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    strcpy(scene_place_title, "廊下");
    strcpy(scene_place_title_2, "教室");
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    talk_clear();
    game_state.screen_brightness = 0x80;
    bg_brightness = 0;
    bg_fade_amount = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    text_marker_hide();
    talk_group = 0;
    talk_line = 0;
    talk_names_set(game_state.current_girl);
    ending_graduation_dialogue_resource_table_init();
    talk_text_table = ending_dialogue_resource_tables[0][3];
    talk_command_table = ending_dialogue_resource_tables[1][3];
    talk_voice_table = ending_dialogue_resource_tables[2][3];
    mode_step_next();
}

void ending_read_background_4045(void) {
    bg_read_sub2(0x4045);
    mode_step_next();
}

void ending_read_graduation_scene_data(void) {
    cd_read_sectors(0x11, 0x801F0000, 0xAE07);
    ending_graduation_dialogue_resource_table_init();
    mode_step_next();
}

void ending_candidate_flag_gate(void) {
    if (((u8) game_state.current_girl >= GIRL_YOSHIO) && ((u32) ((u32) (game_state.girls[14].flags.w << 0x14) >> 0x1D) < 3U)) {
        mode_stage_set(7);
        D_800E7D34 |= 8;
        return;
    }
    mode_step_next();
    D_800E7D34 |= 4;
}

void ending_mio_tokimeki_dialogue_gate(void) {
    if ((((u32)(game_state.girls[1].flags.w << 0x1E) >> 0x1F)) && (get_h_tokimeki(GIRL_MIO) >= 0x46U)) {
        normal_date_speak();
        return;
    }
    mode_step_next();
}
