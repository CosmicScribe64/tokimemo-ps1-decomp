/* girl_script_and_choice_flow.c: src/ovl/KANGEI/80137B90.c:20,244-289,432; per-girl script pointer rows, choice-dependent dialogue groups/rewards, birthday reply resources and saved scene flow. Original object 0x80137B90-0x80139600 (config/objects/KANGEI.txt). */
#define MAIN_API_OVERRIDE_get_h_tokimeki /* u8 return narrows the metric byte on return (T-24122). */
#define MAIN_API_OVERRIDE_get_h_yuukou   /* u8 return narrows the metric byte on return (T-24122). */
#include "common.h"
#include "ovl/KANGEI.h"

u8 get_h_tokimeki(s32 arg0);
u8 get_h_yuukou(s32 arg0);

typedef struct {
    u8 s[0xCC3];
} DateTxt; /* size 0xCC3 */
extern DateTxt D_8013A570;

typedef struct {
    void (*f[40])();
} FnTbl40; /* size 0xA0 */
extern FnTbl40 D_8013A4D0;

void camp_script_pointer_table_bind(void) {
    D_8013A170[0][0][0] = 0x801B00DC;
    D_8013A170[0][0][1] = 0x801B02AC;
    D_8013A170[0][0][2] = 0x801B03C8;
    D_8013A170[0][0][3] = 0x801B04C4;
    D_8013A170[0][0][4] = 0x801B0668;
    D_8013A170[0][0][5] = 0x801B0808;
    D_8013A170[0][0][6] = 0x801B09DC;
    D_8013A170[0][0][7] = 0x801B0BE8;
    D_8013A170[0][0][8] = 0x801B0D8C;
    D_8013A170[0][0][9] = 0x801B0F5C;
    D_8013A170[0][0][10] = 0x801B1164;
    D_8013A170[0][0][11] = 0x801B12D8;
    D_8013A170[0][0][12] = 0x801B1454;
    D_8013A170[0][1][0] = 0x801B00F4;
    D_8013A170[0][1][1] = 0x801B02C8;
    D_8013A170[0][1][2] = 0x801B03D0;
    D_8013A170[0][1][3] = 0x801B04DC;
    D_8013A170[0][1][4] = 0x801B0680;
    D_8013A170[0][1][5] = 0x801B0820;
    D_8013A170[0][1][6] = 0x801B09F8;
    D_8013A170[0][1][7] = 0x801B0C00;
    D_8013A170[0][1][8] = 0x801B0DA4;
    D_8013A170[0][1][9] = 0x801B0F78;
    D_8013A170[0][1][10] = 0x801B1180;
    D_8013A170[0][1][11] = 0x801B12E0;
    D_8013A170[0][1][12] = 0x801B1470;
    D_8013A170[0][2][0] = 0x801B01A8;
    D_8013A170[0][2][1] = 0x801B0398;
    D_8013A170[0][2][2] = 0x801B03F0;
    D_8013A170[0][2][3] = 0x801B0590;
    D_8013A170[0][2][4] = 0x801B0734;
    D_8013A170[0][2][5] = 0x801B08D4;
    D_8013A170[0][2][6] = 0x801B0AE4;
    D_8013A170[0][2][7] = 0x801B0CB4;
    D_8013A170[0][2][8] = 0x801B0E58;
    D_8013A170[0][2][9] = 0x801B1064;
    D_8013A170[0][2][10] = 0x801B126C;
    D_8013A170[0][2][11] = 0x801B1338;
    D_8013A170[0][2][12] = 0x801B155C;
}

void camp_script_pointer_table_bind_alt(void) {
    D_8013A170[1][0][0] = 0x801B0478;
    D_8013A170[1][0][1] = 0x801B13A4;
    D_8013A170[1][0][2] = 0x801B2998;
    D_8013A170[1][0][3] = 0x801B38C8;
    D_8013A170[1][0][4] = 0x801B4258;
    D_8013A170[1][0][5] = 0x801B4B98;
    D_8013A170[1][0][6] = 0x801B5574;
    D_8013A170[1][0][7] = 0x801B6560;
    D_8013A170[1][0][8] = 0x801B7B74;
    D_8013A170[1][0][9] = 0x801B8B18;
    D_8013A170[1][0][10] = 0x801B93D0;
    D_8013A170[1][0][11] = 0x801B9DA0;
    D_8013A170[1][0][12] = 0x801BA780;
    D_8013A170[1][1][0] = 0x801B0510;
    D_8013A170[1][1][1] = 0x801B14E0;
    D_8013A170[1][1][2] = 0x801B2AD0;
    D_8013A170[1][1][3] = 0x801B3960;
    D_8013A170[1][1][4] = 0x801B42F0;
    D_8013A170[1][1][5] = 0x801B4C38;
    D_8013A170[1][1][6] = 0x801B560C;
    D_8013A170[1][1][7] = 0x801B66A0;
    D_8013A170[1][1][8] = 0x801B7CB4;
    D_8013A170[1][1][9] = 0x801B8BB0;
    D_8013A170[1][1][10] = 0x801B9468;
    D_8013A170[1][1][11] = 0x801B9EA8;
    D_8013A170[1][1][12] = 0x801BA818;
    D_8013A170[1][2][0] = 0x801B0888;
    D_8013A170[1][2][1] = 0x801B1E14;
    D_8013A170[1][2][2] = 0x801B33B0;
    D_8013A170[1][2][3] = 0x801B3CF4;
    D_8013A170[1][2][4] = 0x801B4684;
    D_8013A170[1][2][5] = 0x801B4FEC;
    D_8013A170[1][2][6] = 0x801B59F4;
    D_8013A170[1][2][7] = 0x801B6FB8;
    D_8013A170[1][2][8] = 0x801B85CC;
    D_8013A170[1][2][9] = 0x801B8F28;
    D_8013A170[1][2][10] = 0x801B97E0;
    D_8013A170[1][2][11] = 0x801BA2C8;
    D_8013A170[1][2][12] = 0x801BAB90;
}

void camp_birthday_scene_gate(void) {
    if ((((u32) (game_state.girls[0].birthday.w << 0x17) >> 0x1B) == game_state.day) && ((game_state.girls[0].birthday.w & 0xF) == game_state.month) && ((u8) D_80139AC4 == 0)) {
        camp_birthday_callback_dispatch();
        return;
    }
    if ((((u32) (game_state.player_birthday << 0x17) >> 0x1B) == game_state.day) && ((game_state.player_birthday & 0xF) == game_state.month)) {
        camp_player_birthday_callback_dispatch();
        return;
    }
    mode_stage_next();
    D_80139AC0 = 0;
}

void camp_birthday_callback_state_set(void) {
    D_80139AC4 = 1;
    mode_step_set(0);
}

typedef struct {
    void (*f[67])();
} FnTbl67; /* size 0x10C */
extern FnTbl67 D_8013A2B4;

typedef struct {
    void (*f[68])();
} FnTbl68; /* size 0x110 */
extern FnTbl68 D_8013A3C0;

void camp_birthday_callback_dispatch(void) {
    s32 idx; /* declared before the tables: its stack slot sits above them (T-3330) */
    FnTbl67 a;
    FnTbl68 b;

    a = D_8013A2B4;
    b = D_8013A3C0;
    if (((u32)game_state.player_flags.h >> 0xC) == ((u32)game_state.girls[0].flags.b[2] >> 4)) {
        idx = game_state.mode_step;
        a.f[idx](0x80);
        return;
    }
    idx = game_state.mode_step;
    b.f[idx](0x80);
}

void camp_birthday_step_flag_set(void) {
    D_8013A2A8 = 1;
    mode_step_next();
}

void camp_birthday_scene_setup(void) {
    game_state.day_event_done_bits |= 4;
    portrait_layout_set(get_g_zyotai_h(game_state.current_girl));
    D_80139AD0 = (KObj *) D_8013A170[0][0][1];
    D_80139AD4 = D_8013A170[0][1][1];
    D_80139AD8 = D_8013A170[0][2][1];
    D_80139ADC = 1;
    D_80139AE0 = 0;
    camp_current_girl_script_pointer_bind();
    mode_step_next();
}

void camp_shiori_status_branch(void) {
    u8 s;

    s = get_g_zyotai_s(GIRL_SHIORI) & 0x7F;
    if (s >= 2U) {
        game_state.mode_step -= 0xB;
        return;
    }
    mode_step_next();
}

void camp_player_birthday_step_branch(void) {
    if (((u32)(game_state.player_birthday << 0x17) >> 0x1B) == game_state.day && (game_state.player_birthday & 0xF) == game_state.month) {
        game_state.mode_step += 0xC;
        return;
    }
    mode_step_next();
}

void camp_player_birthday_callback_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl40 tbl;

    tbl = D_8013A4D0;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void camp_dialogue_state_clear_and_dispatch(void) {
    D_80139AC0 = 0;
    camp_current_girl_restore();
}

void camp_response_parameter_update(void) {
    if (D_80122CDC != 0) {
        game_state.girls[game_state.current_girl].affection -= 1;
        game_state.girls[game_state.current_girl].syoushin += 5;
        check_para_limit();
        D_80139AC0 = 1;
        game_state.mode_step += 0x38;
        return;
    }
    D_80139ADC += 1;
    mode_step_next();
}

void camp_year_script_select(void) {
    u8 year;

    if (game_state.month >= 4) {
        year = game_state.year - 0x5F;
    } else {
        year = game_state.year - 0x60;
    }
    D_80139AD0 = (KObj *) D_8013A170[1][0][11];
    D_80139AD4 = D_8013A170[1][1][11];
    D_80139AD8 = D_8013A170[1][2][11];
    /* FAKE: repeated field use and the halfword conversion recover the
     * promoted girl byte and temporary numbering. The empty test makes
     * uopt emit the store separately, keeping the sum before its address.
     * Original source unknown. T-24054, T-24122, T-24131, T-25041 */
    D_80139ADC = (u16)((game_state.current_girl << 2) - game_state.current_girl) + year;
    if (D_80139ADC) {}
    D_8013A2AC = year;
    mode_step_next();
}

void camp_script_row_one_bind(void) {
    D_80139AD0 = (KObj *)D_8013A170[1][0][1];
    D_80139AD4 = D_8013A170[1][1][1];
    D_80139AD8 = D_8013A170[1][2][1];
    D_80139ADC = 3;
    camp_script_text_copy_from_selector();
    mode_step_next();
}

void camp_shiori_scene_state_setup(void) {
    s32 unused; /* FAKE: the original frame has one more 4-byte local above sp2A (same extra scalar as the T-3330 table functions); its real use is unknown. T-4040 */
    s16 sp2A;

    sp2A = (s16) D_80122CDC;
    camp_candidate_scene_initialize();
    D_80122CDC = (s32) sp2A;
    D_80139AC0 = 0;
    game_state.current_girl = GIRL_SHIORI;
    talk_names_set(0U);
    D_80139AD0 = (KObj *) D_8013A170[1][0][1];
    D_80139AD4 = D_8013A170[1][1][1];
    D_80139AD8 = D_8013A170[1][2][1];
    D_80139ADC = 4;
}

void camp_shiori_portrait_variant_load(void) {
    u8 s;

    s = get_g_zyotai_s(GIRL_SHIORI) & 0x7F;
    if ((s < 2U) && (D_80122CDC < 2)) {
        portrait_read_start(0x5774);
    } else {
        portrait_read_start(0x55DF);
    }
    mode_step_next();
}

void camp_status_script_offset(void) {
    u8 s;

    s = get_g_zyotai_s(GIRL_SHIORI) & 0x7F;
    if (s == 2) {
        D_80139ADC += 2;
    } else if (s == 3) {
        D_80139ADC += 4;
    } else if (s == 4) {
        D_80139ADC += 6;
    }
    mode_step_next();
}

void camp_choice_script_offset_set(void) {
    portrait_layout_set(D_80122CDC + 1);
    D_80139ADC = (D_80122CDC * 2) + 0xD;
    mode_step_next();
}

void camp_choice_response_branch(void) {
    if (D_80122CDC != 0) {
        D_80139AC0 = 1;
        game_state.mode_step += 0x39;
        return;
    }
    D_80139ADC += 1;
    mode_step_next();
}

void camp_script_index_set_22(void) {
    D_80139ADC = 0x22;
    mode_step_next();
}

void camp_shiori_status_script_offset_set(void) {
    u8 s;

    s = get_g_zyotai_s(GIRL_SHIORI) & 0x7F;
    D_80139ADC = (s >= 2U) * 2 + (s >= 3U) * 2 + (s >= 4U) * 2 + 0x16;
    mode_step_next();
}

void camp_shiori_status_script_offset_set_alt(void) {
    u8 s;

    s = get_g_zyotai_s(GIRL_SHIORI) & 0x7F;
    D_80139ADC = (s >= 2U) * 2 + (s >= 3U) * 2 + (s >= 4U) * 2 + 6;
    mode_step_next();
}

void camp_player_girl_club_compare(void) {
    D_8013A2B0 = game_state.current_girl;
    if (((u32) game_state.player_flags.h >> 0xC) == ((u32) game_state.girls[game_state.current_girl].flags.b[2] >> 4)) {
        mode_step_next();
        mode_step_next();
    } else {
        game_state.current_girl = GIRL_SOTOI;
        D_80139AD0 = (KObj *) D_8013A170[0][0][2];
        D_80139AD4 = D_8013A170[0][1][2];
        D_80139AD8 = D_8013A170[0][2][2];
    }
    mode_step_next();
}

void camp_saved_girl_scene_restore(void) {
    game_state.current_girl = D_8013A2B0;
    camp_current_girl_script_pointer_bind();
    D_80139ADC = 0;
    D_80139AE0 = 0;
    mode_step_next();
}

void camp_current_girl_portrait_resource_load_alt(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        portrait_read_start(0x541D);
        break;
    case GIRL_MIO:
        portrait_read_start(0x5D14);
        break;
    case GIRL_YUINA:
        portrait_read_start(0x5201);
        break;
    case GIRL_AYAKO:
        portrait_read_start(0x5882);
        break;
    case GIRL_SAKI:
        portrait_read_start(0x6692);
        break;
    case GIRL_YUKARI:
        portrait_read_start(0x5F8A);
        break;
    case GIRL_NOZOMI:
        portrait_read_start(0x6200);
        break;
    case GIRL_MIRA:
        portrait_read_start(0x5B7F);
        break;
    case GIRL_YUKO:
        portrait_read_start(0x506C);
        break;
    case GIRL_MEGUMI:
        portrait_read_start(0x652A);
        break;
    default:
        portrait_read_start(0x6908);
        break;
    }
    mode_step_next();
}

void camp_low_status_girl_select(void) {
    cd_read_checked = 0;
    audio_bank_load_start(1, 0xBF98, 0xBF79, 0xCA95, 0xCA4F, 0xCA3E);
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    mode_step_next();
}

void camp_audio_bank_load_poll_retry(void) {
    if (audio_bank_load_poll() == 1) {
        audio_command(0x200);
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void camp_scene_ui_state_reset(void) {
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    back_clear_switch(1);
    actor_clear_range(0);
    message_window_init();
    hizuke_init();
    tpage_buf_clear();
    text_system_reset();
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    bg_brightness = 0;
    talk_clear();
    parameter_disp_switch(0);
    parameter_disp_switch(0);
    icon_disp_switch(0);
    addr_init_bustup();
    text_marker_hide();
    D_80139AC0 = 0;
    mode_step_next();
}

void camp_script_sector_load_group_one(void) {
    cd_read_sectors(0x16, 0x801B0000, 0xAF0D);
    camp_script_pointer_table_bind_alt();
    mode_step_next();
}

void camp_script_sector_load_group_zero(void) {
    cd_read_sectors(3, 0x801B0000, 0xAF43);
    camp_script_pointer_table_bind();
    mode_step_next();
}

void camp_shiori_scene_entry_setup(void) {
    s16 var_s0;

    D_8013A2A8 = 0;
    hud_cursor_actor_init(0);
    hud_cursor_show(0);
    hud_icon_bar_init();
    var_s0 = 0;
    do {
        icon_can_use_set(var_s0, 0);
        var_s0 += 1;
    } while (var_s0 < 0xF);
    hud_icon_bar_update(1);
    parameter_show_init();
    parameter_show_init();
    bg_brightness = 0x80;
    ((Actor *)actor_pool_96)->draw_flags &= 0x7F;
    ((Actor *)actor_pool_96)[1].draw_flags &= 0x7F;
    k_reset(1);
    game_state.current_girl = GIRL_SHIORI;
    D_80139ADC = 0;
    D_80139AE0 = 0;
    camp_script_pointer_table_bind_alt();
    camp_script_row_one_pointer_bind();
    talk_names_set(game_state.current_girl);
    D_80139AC0 = 1;
    mode_step_next();
}

void camp_scene_background_load(void) {
    bg_read_sub2(0x42A9);
    mode_step_next();
}

void camp_scene_background_event_log_select(void) {
    s32 pad; /* FAKE: unused local above s, puts s at sp+0x2B as in the original; real source unknown (T-3330 layout) */
    u8 s;

    s = get_g_zyotai_s(GIRL_SHIORI) & 0x7F;
    if (((u32)game_state.player_flags.h >> 0xC) == ((u32)game_state.girls[0].flags.b[2] >> 4)) {
        bg_read_sub2(0x42A9);
        if (s < 2U && D_80122CDC < 2) {
            event_log_add(0xA, 0x30);
        } else {
            event_log_add(0xA, 0x2D);
        }
    } else {
        bg_read_sub2(0x428D);
        if (s < 2U && D_80122CDC < 2) {
            event_log_add(0xA, 0x31);
        } else {
            event_log_add(0xA, 0x3E);
        }
    }
    mode_step_next();
}

void camp_script_row_one_pointer_bind(void) {
    D_80139AD0 = (KObj *)D_8013A170[1][0][1];
    D_80139AD4 = D_8013A170[1][1][1];
    D_80139AD8 = D_8013A170[1][2][1];
}

s32 camp_low_status_candidate_select(void) {
    s16 i;
    u8 best_i;
    u8 best_s;
    u8 s;

    best_i = 0xFF;
    best_s = 4;
    for (i = 0; i < 11; i++) {
        /* FAKE: dead branch adjusts the loop basic-block count so uopt assigns best_s to $s6 (T-24122). */
        if (1) {}
        if (game_state.girls[i].flags.f.met != 0) {
            s = get_g_zyotai_s(i) & 0x7F;
            if (s < best_s ||
                (s == best_s &&
                 get_h_tokimeki(i) + get_h_yuukou(i) >
                     get_h_tokimeki(best_i) + get_h_yuukou(best_i))) {
                best_i = i;
                best_s = s;
            }
        }
    }
    s = get_g_zyotai_s(best_i) & 0x7F;
    if (best_i != 0xFF && s < 2U) {
        game_state.current_girl = best_i;
        event_log_add(0xD, 0x2A);
    } else {
        talk_clear();
        D_80139ADC = 0;
        D_80139AE0 = 0;
        D_80139AD0 = (KObj *) D_8013A170[0][0][11];
        D_80139AD4 = D_8013A170[0][1][11];
        D_80139AD8 = D_8013A170[0][2][11];
        hud_icon_bar_init();
        for (i = 0; i < 15; i++) {
            icon_can_use_set(i, 0);
        }
        hud_icon_bar_update(1);
        parameter_show_init();
        parameter_show_init();
        talk_names_set(0);
        D_80139AC0 = 1;
        game_state.mode_step += 0x1E;
        return 0;
    }
    game_state.current_girl = best_i;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    back_clear_switch(1);
    actor_clear_range(0);
    message_window_init();
    hizuke_init();
    tpage_buf_clear();
    text_system_reset();
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    bg_brightness = 0;
    talk_clear();
    parameter_disp_switch(0);
    icon_disp_switch(0);
    D_80139ADC = 0;
    D_80139AE0 = 0;
    addr_init_bustup();
    text_marker_hide();
    talk_names_set(game_state.current_girl);
    camp_day_phase_dialogue_pointer_table_setup();
    D_80139AD0 = (KObj *) D_80139A5C[0][6];
    D_80139AD4 = D_80139A5C[1][6];
    D_80139AD8 = D_80139A5C[2][6];
    D_80139AC0 = 0;
    D_80139ADC = 0;
    D_80139AE0 = 0;
    talk_names_set(game_state.current_girl);
    camp_current_girl_script_pointer_bind();
    if ((u8)D_80139AC4 != 0 && (u8)D_8013A2A8 == 0) {
        game_state.mode_step += 2;
    }
    mode_step_next();
}

void camp_current_girl_script_pointer_bind(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        D_80139AD0 = (KObj *) D_8013A170[0][0][1];
        D_80139AD4 = D_8013A170[0][1][1];
        D_80139AD8 = D_8013A170[0][2][1];
        return;
    case GIRL_MIO:
        D_80139AD0 = (KObj *) D_8013A170[0][0][6];
        D_80139AD4 = D_8013A170[0][1][6];
        D_80139AD8 = D_8013A170[0][2][6];
        return;
    case GIRL_YUINA:
        D_80139AD0 = (KObj *) D_8013A170[0][0][3];
        D_80139AD4 = D_8013A170[0][1][3];
        D_80139AD8 = D_8013A170[0][2][3];
        return;
    case GIRL_AYAKO:
        D_80139AD0 = (KObj *) D_8013A170[0][0][4];
        D_80139AD4 = D_8013A170[0][1][4];
        D_80139AD8 = D_8013A170[0][2][4];
        return;
    case GIRL_SAKI:
        D_80139AD0 = (KObj *) D_8013A170[0][0][10];
        D_80139AD4 = D_8013A170[0][1][10];
        D_80139AD8 = D_8013A170[0][2][10];
        return;
    case GIRL_YUKARI:
        D_80139AD0 = (KObj *) D_8013A170[0][0][7];
        D_80139AD4 = D_8013A170[0][1][7];
        D_80139AD8 = D_8013A170[0][2][7];
        return;
    case GIRL_NOZOMI:
        D_80139AD0 = (KObj *) D_8013A170[0][0][8];
        D_80139AD4 = D_8013A170[0][1][8];
        D_80139AD8 = D_8013A170[0][2][8];
        return;
    case GIRL_MIRA:
        D_80139AD0 = (KObj *) D_8013A170[0][0][5];
        D_80139AD4 = D_8013A170[0][1][5];
        D_80139AD8 = D_8013A170[0][2][5];
        return;
    case GIRL_YUKO:
        D_80139AD0 = (KObj *) D_8013A170[0][0][0];
        D_80139AD4 = D_8013A170[0][1][0];
        D_80139AD8 = D_8013A170[0][2][0];
        return;
    case GIRL_MEGUMI:
        D_80139AD0 = (KObj *) D_8013A170[0][0][9];
        D_80139AD4 = D_8013A170[0][1][9];
        D_80139AD8 = D_8013A170[0][2][9];
        return;
    default:
        D_80139AD0 = (KObj *) D_8013A170[0][0][12];
        D_80139AD4 = D_8013A170[0][1][12];
        D_80139AD8 = D_8013A170[0][2][12];
        return;
    }
}

void camp_script_text_copy_from_selector(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    DateTxt tbl;

    tbl = D_8013A570;
    strcpy(name_code_a, &tbl.s[game_state.current_girl * 0x129 + D_8013A2AC * 0x63 + D_80122CDC * 0x21]);
}
