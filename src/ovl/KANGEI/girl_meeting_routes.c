/* girl_meeting_routes.c: src/ovl/KANGEI/80134120.c:24-48; scene callback tables and met/unmet-girl/year route choice, followed by girl-specific background/dialogue setup. Original object 0x80134120-0x801355F0 (config/objects/KANGEI.txt). */
#include "common.h"
#include "ovl/KANGEI.h"

typedef struct {
    void (*f[30])();
} FnTbl30; /* size 0x78 */

typedef struct {
    void (*f[37])();
} FnTbl37; /* size 0x94 */
typedef struct {
    void (*f[33])();
} FnTbl33; /* size 0x84 */
extern FnTbl37 D_80139BEC;
extern FnTbl33 D_80139C80;

/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
/* Scene table, indexed by game_state.mode_step in camp_scene_callback_dispatch. Defining it here as the
 * original did matters for code: as1 treats every function named in this file's data as
 * an address-taken label, so the jump-table dispatch of camp_scene_text_index_select gets an edge back to
 * that function's entry. The resulting loop changes as1's block weights and its choice for
 * the switch's range-check delay slot (T-25150). */
FnTbl30 D_80139B30 = { {
    camp_candidate_scene_initialize, camp_girl_meeting_audio_bank_load_start, camp_girl_meeting_audio_bank_poll_retry, camp_dialogue_data_sector_load, scene_wait_cd_read,
    camp_scene_data_sector_load, scene_wait_cd_read, camp_scene_text_index_select, camp_club_location_label_select, scene_wait_cd_read,
    dec_init, change_dec_bg, scene_bg_wipe_in, place_init, camp_candidate_portrait_read,
    scene_wait_cd_read, portrait_show_checked, normal_date_girl_in, camp_opening_talk_run, normal_date_girl_out,
    normal_date_bg_out, camp_audio_bank_start, camp_audio_bank_retry, camp_audio_load_poll_retry_alt, camp_dialogue_data_sector_load,
    scene_wait_cd_read, camp_scene_data_sector_load, scene_wait_cd_read, camp_current_girl_restore, game_mode_reset_to_title,
} };
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/girl_meeting_routes.data", D_80139BA8);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/girl_meeting_routes.data", D_80139BD4);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/girl_meeting_routes.data", D_80139BDC);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/girl_meeting_routes.data", D_80139BE4);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/girl_meeting_routes.data", D_80139BEC);

void camp_current_girl_scene_gate(void) {
    if ((game_state.current_girl != 0xFF) && game_state.girls[game_state.current_girl].flags.f.met) {
        camp_scene_callback_dispatch();
        return;
    }
    if ((game_state.current_girl != 0xFF) && !game_state.girls[game_state.current_girl].flags.f.met && ((u8)game_state.year < 0x61U) && (game_state.current_girl != GIRL_YUMI)) {
        camp_unmet_girl_callback_dispatch();
        return;
    }
    mode_stage_next();
}

void camp_scene_callback_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl30 tbl;

    tbl = D_80139B30;
    idx = game_state.mode_step;
    tbl.f[idx](0x20);
}

void camp_girl_meeting_audio_bank_load_start(void) {
    audio_bank_load_start(1, 0xBF98, 0xBF79, (&D_800B3688)[game_state.current_girl], (&D_800B36C8)[game_state.current_girl], (&D_800B3708)[game_state.current_girl]);
    if (audio_bank_load_poll() == 1) {
        audio_command(0x200);
        mode_step_next();
    }
    mode_step_next();
}

void camp_girl_meeting_audio_bank_poll_retry(void) {
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

void camp_candidate_scene_initialize(void) {
    s16 i;
    u32 v;

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
    game_state.current_girl = 0xFF;
    v = game_state.player_flags.h >> 12;
    for (i = 0; i < 0xB; i++) {
        if (v == (game_state.girls[i].flags.b[2] >> 4) && game_state.girls[i].flags.f.met) {
            game_state.current_girl = i;
            break;
        }
    }
    if (v == 0xA && game_state.girls[0].tokimeki + game_state.girls[0].affection < game_state.girls[10].tokimeki + game_state.girls[10].affection && game_state.year >= 0x60) {
        game_state.current_girl = GIRL_YUMI;
    }
    talk_names_set(game_state.current_girl);
    camp_day_phase_dialogue_pointer_table_setup();
    D_80139AD0 = (KObj *)D_80139A5C[0][6];
    D_80139AD4 = D_80139A5C[1][6];
    D_80139AD8 = D_80139A5C[2][6];
    D_80139AC0 = 0;
    mode_step_next();
}

void camp_candidate_portrait_read(void) {
    u8 girl;
    KangeiWords11 all;
    KangeiWords2 first;
    KangeiWords2 second;
    KangeiWords2 third;

    all = D_80139BA8;
    first = D_80139BD4;
    second = D_80139BDC;
    third = D_80139BE4;
    girl = game_state.current_girl;
    switch (girl) {
    case 0:
        portrait_read_start(all.unk_00[game_state.player_flags.h >> 12]);
        break;
    case 1:
        portrait_read_start(first.unk_00[game_state.player_flags.h >> 12]);
        break;
    case 2:
        portrait_read_start(second.unk_00[(game_state.player_flags.h >> 12) - 2]);
        break;
    case 3:
        portrait_read_start(third.unk_00[(game_state.player_flags.h >> 12) - 4]);
        break;
    case 4:
        portrait_read_start(0x66EC);
        break;
    case 5:
        portrait_read_start(0x5FE4);
        break;
    case 6:
        portrait_read_start(0x625A);
        break;
    default:
        portrait_read_start(0x6962);
        break;
    }
    mode_step_next();
}

/* The switch's range-check delay slot holds the dispatch `sll` only because D_80139B30 (above)
 * names this function: the dispatch `jr` then has an edge back to the entry, the loop gives the
 * dispatch a lighter as1 weight than the default arm, and as1 fills the slot from the dispatch
 * instead of hoisting the default's `sll` (T-25150; history T-24201, T-24902, T-25042). */
void camp_scene_text_index_select(void) {
    u8 s;
    s16 v;
    s16 c2;

    s = get_g_zyotai_s(game_state.current_girl) & 0x7F;
    if (s < 2U) {
        s = 0;
    } else if (s == 2) {
        s = 1;
    } else if (s == 3) {
        s = 2;
    } else {
        s = 3;
    }
    v = game_state.clubs[(u16)game_state.player_flags.h >> 12].practice_points / 50;
    if (game_state.clubs[(u16)game_state.player_flags.h >> 12].practice_points >= 0x64U) {
        v = 2;
    }
    if (game_state.clubs[(u16)game_state.player_flags.h >> 12].practice_points >= 0xC8U) {
        v = 3;
    }
    if (game_state.clubs[(u16)game_state.player_flags.h >> 12].practice_points >= 0x164U) {
        v = 4;
    }
    D_80139AE0 = 0;
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        c2 = (u16)game_state.player_flags.h >> 12;
        if (((u16)game_state.player_flags.h >> 12) == 9) {
            c2 = 8;
            if (game_state.clubs[(u16)game_state.player_flags.h >> 12].practice_points >= 0x164U) {
                v = 5;
            }
        }
        if (((u16)game_state.player_flags.h >> 12) == 10) {
            c2 = 8;
            if (game_state.clubs[(u16)game_state.player_flags.h >> 12].practice_points >= 0x164U) {
                v = 6;
            }
        }
        D_80139ADC = c2 * 20 + v * 4 + s + 3;
        if (game_state.year == 0x61 && ((u16)game_state.player_flags.h >> 12) == 6) {
            D_80139ADC = s + 0xBF;
        }
        break;
    case GIRL_MIO:
        D_80139ADC = ((u16)game_state.player_flags.h >> 12) * 20 + v * 4 + s + 8;
        break;
    case GIRL_YUINA:
        D_80139ADC = ((u16)game_state.player_flags.h >> 12) * 20 + v * 4 + s - 0x20;
        break;
    case GIRL_AYAKO:
        D_80139ADC = ((u16)game_state.player_flags.h >> 12) * 20 + v * 4 + s - 0x48;
        break;
    case GIRL_SAKI:
        D_80139ADC = ((u16)game_state.player_flags.h >> 12) * 24 + v * 4 + s - 0x89;
        if (game_state.year == 0x61) {
            if (((KangeiStateView *)&game_state)->club == CLUB_BASEBALL) {
                D_80139ADC = s + 0x1B;
            }
        }
        break;
    case GIRL_YUKARI:
        D_80139ADC = v * 4 + s + 7;
        break;
    case GIRL_NOZOMI:
        D_80139ADC = v * 4 + s + 7;
        break;
    default:
        D_80139ADC = v * 4 + s + 3;
        break;
    }
    camp_current_girl_scene_pointer_bind();
    mode_step_next();
}

void camp_current_girl_scene_pointer_bind(void) {
    camp_day_phase_dialogue_pointer_table_setup();
    camp_background_pointer_setup();
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        D_80139AD0 = (KObj *) D_80139A50[0];
        D_80139AD4 = D_80139A50[1];
        D_80139AD8 = D_80139A50[2];
        return;
    case GIRL_MIO:
        D_80139AD0 = (KObj *) D_80139A5C[0][2];
        D_80139AD4 = D_80139A5C[1][2];
        D_80139AD8 = D_80139A5C[2][2];
        return;
    case GIRL_YUINA:
        D_80139AD0 = (KObj *) D_80139A5C[0][0];
        D_80139AD4 = D_80139A5C[1][0];
        D_80139AD8 = D_80139A5C[2][0];
        return;
    case GIRL_AYAKO:
        D_80139AD0 = (KObj *) D_80139A5C[0][1];
        D_80139AD4 = D_80139A5C[1][1];
        D_80139AD8 = D_80139A5C[2][1];
        return;
    case GIRL_SAKI:
        D_80139AD0 = (KObj *) D_80139A5C[0][5];
        D_80139AD4 = D_80139A5C[1][5];
        D_80139AD8 = D_80139A5C[2][5];
        return;
    case GIRL_YUKARI:
        D_80139AD0 = (KObj *) D_80139A5C[0][3];
        D_80139AD4 = D_80139A5C[1][3];
        D_80139AD8 = D_80139A5C[2][3];
        return;
    case GIRL_NOZOMI:
        D_80139AD0 = (KObj *) D_80139A5C[0][4];
        D_80139AD4 = D_80139A5C[1][4];
        D_80139AD8 = D_80139A5C[2][4];
        return;
    default:
        D_80139AD0 = (KObj *) D_80139A5C[0][7];
        D_80139AD4 = D_80139A5C[1][7];
        D_80139AD8 = D_80139A5C[2][7];
        return;
    }
}

void camp_club_location_label_select(void) {
    u32 selection;

    selection = (u16) game_state.player_flags.h >> 0xC;
    switch (selection) {
    case 0:
        strcpy(scene_place_title, "図書室");
        bg_read_sub2(0x40BC);
        break;
    case 1:
        strcpy(scene_place_title, "演劇部");
        bg_read_sub2(0x40B2);
        break;
    case 2:
        strcpy(scene_place_title, "実験室");
        bg_read_sub2(0x40C6);
        break;
    case 3:
        strcpy(scene_place_title, "電脳部室");
        bg_read_sub2(0x40D0);
        break;
    case 4:
        strcpy(scene_place_title, "美術室");
        bg_read_sub2(0x40D9);
        break;
    case 5:
        strcpy(scene_place_title, "音楽室");
        bg_read_sub2(0x40E2);
        break;
    case 6:
        strcpy(scene_place_title, "グランド");
        bg_read_sub2(0x4296);
        break;
    case 7:
        strcpy(scene_place_title, "グランド");
        bg_read_sub2(0x4296);
        break;
    case 8:
        strcpy(scene_place_title, "グランド");
        bg_read_sub2(0x4296);
        break;
    case 9:
        strcpy(scene_place_title, "プール");
        bg_read_sub2(0x40A0);
        break;
    default:
        strcpy(scene_place_title, "体育館");
        bg_read_sub2(0x40A9);
        break;
    }
    event_log_add(0xE, (s32) ((u16) game_state.player_flags.h >> 0xC));
    mode_step_next();
}

void camp_unmet_girl_callback_dispatch(void) {
    s32 pad; /* FAKE: unused slot above tblA, the original frame has 8 more bytes (T-3330 idiom, real source unknown). T-8010 */
    FnTbl37 tblA;
    FnTbl33 tblB;

    tblA = D_80139BEC;
    tblB = D_80139C80;
    if ((u8)game_state.date_girl < GIRL_SAKI) {
        tblA.f[game_state.mode_step](0x80);
        D_80139AC8 = 1;
        return;
    }
    tblB.f[game_state.mode_step](0x80);
}

void camp_temporary_sotoi_talk_run(void) {
    s32 pad; /* FAKE: unused local above the saved selector (the original frame has the copy at sp+0x28), T-6050 */
    u32 sp;

    game_state.current_girl = GIRL_SOTOI;
    sp = game_state.mode_step;
    camp_opening_talk_run();
    if (sp != game_state.mode_step) {
        game_state.current_girl = game_state.date_girl;
    }
}

void camp_date_girl_scene_initialize(void) {
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
    talk_names_set(game_state.date_girl);
    camp_day_phase_dialogue_pointer_table_setup();
    D_80139AD0 = (KObj *) D_80139A5C[0][6];
    D_80139AD4 = D_80139A5C[1][6];
    D_80139AD8 = D_80139A5C[2][6];
    D_80139AC0 = 0;
    mode_step_next();
}

/* FAKE: entry read of unk_F5F puts it in cvt_pass's entry set ($v1) and keeps rect at
 * 0x34($sp); empty `if (rect.x == 1U)` promotes LDC 1U to $v0; pointer dereference on
 * unk_1104 puts $v0 first in `bne $v0, $t7` via uoptinput's == swap (T-22016, T-24122, T-24131). */
void camp_portrait_bustup_render_update(void) {
    u8 pad;
    RECT rect;

    pad = game_state.current_girl;
    camp_opening_talk_run();
    if (D_80139AE0 == 2) {
        if (rect.x == 1U) {}
        if (*(u32 *)&game_state.mode_step_timer == 1U) {
            if ((game_state.current_girl == GIRL_MIO) || (game_state.current_girl == GIRL_AYAKO)) {
                rect.x = 0x140;
                rect.y = 0x80;
                rect.w = 0x180;
                rect.h = 0x80;
                func_8009C8E0(&rect, (void *)0x80180000);
                func_8009C674(0);
                portrait_build(face_palette, face_images, face_frame_tables, face_sequences, (u8)get_g_zyotai_h(game_state.current_girl) & 0x7F);
                portrait_face_update();
            }
        }
    }
    game_state.mode_step_timer.w += 1;
}

void camp_portrait_rectangle_restore(void) {
    RECT rect;

    portrait_show_checked();
    rect.x = 0x140;
    rect.y = 0x80;
    rect.w = 0x180;
    rect.h = 0x80;
    func_8009C884(&rect, (void *)0x80180000);
}

void camp_current_girl_affection_set(void) {
    game_state.girls[game_state.current_girl].affection = 0x32;
    audio_command(0x24);
    camp_girl_met_flag_set();
}

void camp_nozomi_wait_gate(void) {
    if (game_state.current_girl == GIRL_NOZOMI) {
        don_wait();
        return;
    }
    mode_step_next();
}

void camp_scene_audio_transition(void) {
    audio_command(0x23);
    audio_command(0x24);
    audio_command(0x2E);
    if ((u8) game_state.current_girl < GIRL_SAKI) {
        audio_command(0x202);
    } else {
        audio_command(0x201);
    }
    mode_step_next();
}

void camp_scene_audio_transition_alt(void) {
    audio_command(0x24);
    audio_command(0x501);
    mode_step_next();
}

void camp_current_girl_affection_set_high(void) {
    game_state.girls[game_state.current_girl].affection = 0x46;
    D_80139ADC = 3;
    camp_current_girl_scene_pointer_bind();
    mode_step_next();
}

void camp_background_location_select(void) {
    if ((u8)game_state.current_girl < GIRL_SAKI) {
        bg_read_sub2(0x42A9);
        strcpy(scene_place_title, "ロビー");
    } else {
        bg_read_sub2(0x4296);
        strcpy(scene_place_title, "グランド");
    }
    mode_step_next();
}

INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/girl_meeting_routes.rodata", D_8013980C);

void camp_background_event_log_update(void) {
    bg_read_sub2(0x42C6);
    event_log_add(0xE, 0xB);
    strcpy(scene_place_title_2, &D_8013980C);
    mode_step_next();
}

void camp_background_event_log_update_alt(void) {
    bg_read_sub2(0x429F);
    event_log_add(0xE, 0xC);
    mode_step_next();
}

void camp_nozomi_talk_audio_update(void) {
    if ((D_80139AE0 == 2) && (game_state.current_girl == GIRL_NOZOMI)) {
        D_80139AE4 = 2;
    } else {
        D_80139AE4 = 0;
    }
    camp_opening_talk_run();
    if ((D_80139AE0 == 3) && (game_state.mode_step_timer.w++ == 0)) {
        if (game_state.current_girl == GIRL_NOZOMI) {
            audio_command(0x503);
            draw_shake_start();
        }
    }
}

/* Bits 1 and 2 of the current girl's flag word; the index is read again after the first store (T-7010). */
void camp_girl_met_flag_set(void) {
    game_state.girls[game_state.current_girl].flags.f.met = 1;
    game_state.girls[game_state.current_girl].flags.f.b2 = 1;
    mode_stage_next();
}

void camp_current_girl_fixed_portrait_read(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        portrait_read_start(0x55DF);
        break;
    case GIRL_MIO:
        portrait_read_start(0x5F03);
        break;
    case GIRL_YUINA:
        portrait_read_start(0x53C3);
        break;
    case GIRL_AYAKO:
        portrait_read_start(0x5A71);
        break;
    case GIRL_SAKI:
        portrait_read_start(0x68AE);
        break;
    case GIRL_YUKARI:
        portrait_read_start(0x61A6);
        break;
    case GIRL_NOZOMI:
        portrait_read_start(0x641C);
        break;
    default:
        portrait_read_start(0x69BC);
        break;
    }
    mode_step_next();
}

void camp_special_portrait_read(void) {
    switch (game_state.current_girl) {
    case GIRL_MIO:
        portrait_read_start(0x5F30);
        break;
    case GIRL_AYAKO:
        portrait_read_start(0x5A9E);
        break;
    default:
        mode_step_next();
        mode_step_next();
        break;
    }
    mode_step_next();
}
