/* karaoke_scene_and_return.c: src/ovl/DATE2/80137D50.c:4; karaoke mode entry amusement_karaoke_return_frame_update, room/title setup amusement_karaoke_return_dialogue_setup, song/audio/text/rating flow and DATE return amusement_return_to_date. Original object 0x80137D50-0x8013A020 (config/objects/DATE2.txt). */
#define MAIN_API_OVERRIDE_pad_code_entered /* T-11880: amusement_karaoke_return_menu_input matched with the word as a u8[4] (main_api.h: u8 scalar); the scalar view keeps a second register range */
#define MAIN_API_OVERRIDE_menu_set /* T-11880: amusement_karaoke_return_menu_build keeps the call result live in v0 (implicit int view; main_api.h: void) */
#define set_kanji_string func_8004E788_main_api /* T-24094: amusement_karaoke_result_text_page_display passes void * string pointers as arg 3 without A->J cvt */
#include "common.h"
#include "ovl/DATE2.h"
#undef set_kanji_string
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B540);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B544);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B548);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B54C);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B550);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B554);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B558);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B55C);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B560);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B564);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B568);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B56C);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B570);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B574);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B578);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B57C);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B580);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B584);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B588);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B58C);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B590);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B594);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B598);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B59C);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5A0);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5A4);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5A8);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5AC);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5B0);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5B4);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5B8);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5BC);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5C0);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5C4);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5C8);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5CC);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5D0);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5D4);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5D8);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5DC);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5E0);
s16 D_8013B5E4[4] = { 0, 0, 0, 0 };
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5EC);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5F0);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5F4);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5F8);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B5FC);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B600);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B634);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B688);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B6A8);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B6B0);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B6B8);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B6F4);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B75C);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/karaoke_scene_and_return.data", D_8013B7AC);

typedef struct {
    void (*f[13])();
} FnTbl13; /* size 0x34 */
extern FnTbl13 D_8013B600;
extern u8 pad_code_entered[4];
s32 menu_set();

void amusement_karaoke_return_pointer_setup(void) {
    D_8013B540 = 0x801B00A4;
    D_8013B544 = 0x801B0238;
    D_8013B548 = 0x801B03C8;
    D_8013B54C = 0x801B0558;
    D_8013B550 = 0x801B0704;
    D_8013B554 = 0x801B0878;
    D_8013B558 = 0x801B0A20;
    D_8013B55C = 0x801B0B9C;
    D_8013B560 = 0x801B0D88;
    D_8013B564 = 0x801B0F24;
    D_8013B568 = 0x801B106C;
    D_8013B56C = 0x801B00CC;
    D_8013B570 = 0x801B0260;
    D_8013B574 = 0x801B03F0;
    D_8013B578 = 0x801B0580;
    D_8013B57C = 0x801B072C;
    D_8013B580 = 0x801B08A0;
    D_8013B584 = 0x801B0A48;
    D_8013B588 = 0x801B0BC4;
    D_8013B58C = 0x801B0DB0;
    D_8013B590 = 0x801B0F4C;
    D_8013B594 = 0x801B1084;
    D_8013B598 = 0x801B016C;
    D_8013B59C = 0x801B0300;
    D_8013B5A0 = 0x801B0490;
    D_8013B5A4 = 0x801B0620;
    D_8013B5A8 = 0x801B07CC;
    D_8013B5AC = 0x801B0940;
    D_8013B5B0 = 0x801B0AE8;
    D_8013B5B4 = 0x801B0C64;
    D_8013B5B8 = 0x801B0E50;
    D_8013B5BC = 0x801B0FEC;
    D_8013B5C0 = 0x801B10E4;
}

void amusement_karaoke_return_frame_update(void) {
    portrait_wink_update();
    amusement_karaoke_mode_step_dispatch();
    check_k_scroll();
    k_disp_inc2();
    hud_screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    hizuke_show();
    scene_frame_update();
    draw_shake_update();
}

void amusement_karaoke_return_talk_run(void) {
    talk_tables = &D_8013B5F8;
    D_800CA138 = &D_8013B5FC;
    D_800CA13C = D_8013B5EC;
    D_800CA140 = D_8013B5F0;
    D_800CA144 = D_8013B5F4;
    talk_run(D_80122CDC, 1, 0);
}

void amusement_karaoke_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl13 tbl;

    tbl = D_8013B600;
    idx = game_state.mode_step;
    tbl.f[idx](0x40);
}

void amusement_karaoke_return_sector_load(void) {
    cd_read_sectors(2, 0x801E8000, 0xBD02);
    amusement_karaoke_return_asset_pointer_setup();
    mode_step_next();
}

void amusement_karaoke_return_asset_pointer_setup(void) {
    D_8013B5D4 = 0x801E80D0;
    D_8013B5D8 = 0x801E82C0;
    D_8013B5DC = 0x801E8444;
    D_8013B5E0 = 0x801E8700;
}

void amusement_karaoke_return_audio_setup(void) {
    audio_command(0x202);
    D_80122D20 = 0;
    mode_step_next();
}

void amusement_karaoke_return_portrait_layout_update(void) {
    if (D_80122CDC != 0) {
        portrait_layout_set(2);
    }
    mode_step_next();
}

void amusement_return_to_date(void) {
    game_mode_set(GAME_MODE_DATE);
    mode_stage_set(1);
    mode_step_set(0x11);
}

void amusement_karaoke_return_dialogue_sector_load(void) {
    cd_read_sectors(3, 0x801B0000, 0xAF39);
    amusement_karaoke_return_pointer_setup();
    mode_step_next();
}

void amusement_karaoke_return_dialogue_setup(void) {

    *D_800CA234 = 0;
    D_800CA236 = 0;
    D_800CA234[2] = 0;
    *D_800CA224 = 1;
    D_800CA224[1] = 1;
    D_800CA224[2] = 1;
    k_reset(0);
    message_window_set_title((s32) "ＢＯＸ内");
    D_80122CDC = 0;
    D_8013B5CC = 0;
    D_8013B5D0 = 0;
    amusement_karaoke_return_pointer_setup();
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        D_8013B5EC = D_8013B544;
        D_8013B5F0 = D_8013B570;
        D_8013B5F4 = D_8013B59C;
        break;
    case GIRL_MIO:
        D_8013B5EC = D_8013B554;
        D_8013B5F0 = D_8013B580;
        D_8013B5F4 = D_8013B5AC;
        break;
    case GIRL_YUINA:
        D_8013B5EC = D_8013B548;
        D_8013B5F0 = D_8013B574;
        D_8013B5F4 = D_8013B5A0;
        break;
    case GIRL_AYAKO:
        D_8013B5EC = D_8013B54C;
        D_8013B5F0 = D_8013B578;
        D_8013B5F4 = D_8013B5A4;
        break;
    case GIRL_SAKI:
        D_8013B5EC = D_8013B564;
        D_8013B5F0 = D_8013B590;
        D_8013B5F4 = D_8013B5BC;
        break;
    case GIRL_YUKARI:
        D_8013B5EC = D_8013B558;
        D_8013B5F0 = D_8013B584;
        D_8013B5F4 = D_8013B5B0;
        break;
    case GIRL_NOZOMI:
        D_8013B5EC = D_8013B55C;
        D_8013B5F0 = D_8013B588;
        D_8013B5F4 = D_8013B5B4;
        break;
    case GIRL_MIRA:
        D_8013B5EC = D_8013B550;
        D_8013B5F0 = D_8013B57C;
        D_8013B5F4 = D_8013B5A8;
        break;
    case GIRL_YUKO:
        D_8013B5EC = D_8013B540;
        D_8013B5F0 = D_8013B56C;
        D_8013B5F4 = D_8013B598;
        break;
    case GIRL_MEGUMI:
        D_8013B5EC = D_8013B560;
        D_8013B5F0 = D_8013B58C;
        D_8013B5F4 = D_8013B5B8;
        break;
    case GIRL_YUMI:
        D_8013B5EC = D_8013B568;
        D_8013B5F0 = D_8013B594;
        D_8013B5F4 = D_8013B5C0;
        break;
    }
    D_8013B5F8 = 0;
    D_8013B5FC = 0;
    mode_step_next();
}


void amusement_karaoke_return_timer_dispatch(void) {
    switch (game_state.mode_step_timer.u) {
    case 0:
        amusement_karaoke_return_substep_dispatch();
        return;
    case 1:
        amusement_karaoke_audio_substep_dispatch();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void amusement_karaoke_return_substep_dispatch(void) {
    switch (game_state.substep) {
    case 0:
        amusement_karaoke_return_menu_build();
        return;
    case 1:
        amusement_karaoke_return_menu_input();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

s32 amusement_karaoke_return_menu_build(void) {
    s32 set_kanji_string(s16 x, s16 y, s32 c, s32 d, s32 e);
    GameState *g = &game_state;
    s16 i;
    s16 *p;
    Date2Labels4x21 lab;
    s16 x[4];
    s16 y[4];
    s16 h[4];

    lab = D_8013B634;
    /* FAKE: one x/y pair per line keeps the original li/sh order (as1 schedules by line). T-11880 */
    x[0] = -0x80; y[0] = 0x34;
    x[1] = 0; y[1] = 0x34;
    x[2] = -0x80; y[2] = 0x4C;
    x[3] = -0x20; y[3] = 0x4C;
    if (D_8013B5CC) {} /* FAKE: empty test adds a block; without it uopt promotes the h chain (frame 0xB8) and keeps D_8013B5CC out of v0 in the first label test. T-11880 */
    D_8013B5E4[1] = (strlen(lab.v[1]) / 2) * 0x10;
    h[0] = h[1] = h[2] = h[3] = 0x10; /* FAKE: chain store, one li for all four and h[3] stored last. T-11880 */
    D_8013B5E4[0] = 0;
    D_8013B5E4[2] = 0;
    D_8013B5E4[3] = 0;
    k_reset(1);
    if ((system_data.karaoke_unlock_bits[0] & 1) || (D_8013B5CC != 0)) {
        if (D_8013B5CC >= 2U) {
            strcpy(lab.v[0], "恋は…（歌付き）");
        }
        D_8013B5E4[0] = (strlen(lab.v[0]) / 2) * 0x10;
        k_disp_start(set_kanji_string(x[0], y[0], 0, (s32)lab.v[0], 0));
    }
    k_disp_start(set_kanji_string(x[1], y[1], 0, (s32)lab.v[1], 0));
    if ((system_data.karaoke_unlock_bits[0] & 4) || (D_8013B5CC != 0)) {
        D_8013B5E4[2] = (strlen(lab.v[2]) / 2) * 0x10;
        k_disp_start(set_kanji_string(x[2], y[2], 0, (s32)lab.v[2], 0));
    }
    if ((system_data.karaoke_unlock_bits[0] & 8) || (D_8013B5CC != 0)) {
        if (D_8013B5CC >= 2U) {
            strcpy(lab.v[3], "女々しい…（歌付き）");
        }
        D_8013B5E4[3] = (strlen(lab.v[3]) / 2) * 0x10;
        k_disp_start(set_kanji_string(x[3], y[3], 0, (s32)lab.v[3], 0));
    }
    p = D_8013B5E4;
    menu_set(0, 4, x, y, p, h);
    D_80122D30 = 1;
    for (i = 0; i < 4; i++) {
        if (p[i] != 0) {
            g->menu_cursor_0 = i;
            break;
        }
    }
    g->substep++;
}

s32 amusement_karaoke_return_menu_input(void) {
    s16 i;

    menu_check(0, cursor_x, cursor_y);
    menu_cursor_update(0);
    menu_bar_show(0);
    if (pad_code_entered[0] != 0 && pad_code_entered[0] != D_8013B5D0) {
        audio_command(0x601);
        if (++D_8013B5CC >= 3) {
            D_8013B5CC = 1;
        }
        game_state.substep = 0;
        pad_code_entered[0] = 0;
        pad_code_index = 0;
    }
    D_8013B5D0 = pad_code_entered[0];
    if (game_state.pad_pressed & 0x600060) {
        for (i = 0; i < 4; i++) {
            if (((MenuItem (*)[2])game_state.menu_items)[i][0].hover != 0 && D_8013B5E4[i] != 0) {
                D_80122CDC = i;
                D_8013B5C4 = i;
                D_80122D30 = 0;
                D_8011ECD3 &= ~0x80;
                k_reset(1);
                game_state.mode_step_timer.w++;
                game_state.substep = 0;
                return 0;
            }
        }
    }
}

s32 amusement_karaoke_audio_substep_dispatch(void) {
    switch (game_state.substep) {
    case 0:
        amusement_karaoke_cd_audio_request();
        break;
    case 1:
        amusement_karaoke_result_update();
        break;
    }
}

/* FAKE: leading byte fixes copy offsets; integer fallthrough retains allocation. T-9550 */
s32 amusement_karaoke_cd_audio_request(void) {
    u8 pad[1];
    Date2LoadPairs4 loads;
    Date2LoadPair first;
    Date2LoadPair second;
    loads = D_8013B688;
    first = D_8013B6A8;
    second = D_8013B6B0;
    D_80122D20 = 1;
    audio_command(0xBF);
    if (D_8013B5CC < 2U || D_80122CDC == 1 || D_80122CDC == 2) {
        cd_audio_request_set(loads.pair[D_80122CDC].unk_00, loads.pair[D_80122CDC].unk_04, 15);
    } else if (D_80122CDC == 0) {
        cd_audio_request_set(first.unk_00, first.unk_04, 15);
    } else {
        cd_audio_request_set(second.unk_00, second.unk_04, 15);
    }
    audio_command(0x300);
    D_8013B5C8 = 0;
    game_state.substep++;
}

void amusement_karaoke_result_update(void) {
    u8 t;

    if ((cd_last_command() == 9) && (D_8013B5C8 >= 0x79)) {
        mode_step_next();
        D_80122CDC = 0;
        game_state.girls[game_state.current_girl].tokimeki += 2;
        game_state.girls[game_state.current_girl].affection += 1;
        check_para_limit();
    } else if ((game_state.pad_pressed & 0x400040) && (D_8013B5C8 >= 0x79)) {
        t = get_g_zyotai_s(game_state.current_girl) & 0x7F;
        D_8013B5F8 = D_8013B5F8 + (t == 2) + ((t == 3) * 2) + 1;
        if (game_state.current_girl == GIRL_YUMI) {
            D_8013B5F8 = 2;
        }
        mode_step_next();
        k_reset(1);
        audio_command(0x74);
        D_80122CDC = 1;
        game_state.girls[game_state.current_girl].affection += 1;
        check_para_limit();
    }
    if (cd_audio_state() & 4) {
        D_8013B5C8 += 1;
    }
    switch (D_8013B5C4) {
    case 2:
        amusement_karaoke_result_table_display_30();
        break;
    case 0:
        amusement_karaoke_result_table_display_51();
        break;
    case 3:
        amusement_karaoke_result_table_display_39();
        break;
    default:
        amusement_karaoke_result_table_display_81();
        break;
    }
}

/* FAKE: padding fixes table offset; OR-zero preserves conversion; string pointers retain scalar views. T-9550 */
void amusement_karaoke_result_table_display_30(void) {
    s32 set_kanji_string(s16 x, s16 y, s32 c, s32 d, s32 e);
    s16 index;
    s16 selected;
    Date2Threshold30 thresholds;
    u8 pad[8];

    thresholds = D_8013B6B8;
    selected = 0xFF;
    index = 0;
    do {
        if (D_8013B5C8 >= thresholds.values[index]) selected = index;
        index++;
    } while (index < 30);
    if (D_8013B5C8 == 200) {
        set_kanji_string(-116, 54, 0, (s32)((u8 **)D_8013B5D4)[0], 0);
        k_disp_start(set_kanji_string((s16)(strlen(((u8 **)D_8013B5D4)[0]) * 7 - 116), 54, 0, (s32)((u8 **)D_8013B5D4)[1], 0));
    }
    if (selected != 0xFF && D_8013B5C8 == thresholds.values[selected]) {
        k_reset(1);
        amusement_karaoke_result_text_page_display((s16)(selected | 0));
    }
    if (D_8013B5C8 == 3540) k_reset(1);
}

/* FAKE: padding fixes table offset; OR-zero preserves conversion; string pointers retain scalar views. T-9550 */
void amusement_karaoke_result_table_display_51(void) {
    s32 set_kanji_string(s16 x, s16 y, s32 c, s32 d, s32 e);
    s16 index;
    s16 selected;
    Date2Threshold51 thresholds;
    u8 pad[4];
    thresholds = D_8013B6F4;
    selected = 0xFF;
    index = 0;
    do {
        if (D_8013B5C8 >= thresholds.values[index]) selected = index;
        index++;
    } while (index < 51);
    if (D_8013B5C8 == 1500) {
        set_kanji_string(-116, 54, 0, (s32)((u8 **)D_8013B5D8)[0], 0);
        set_kanji_string((s16)(strlen(((u8 **)D_8013B5D8)[0]) * 7 - 116), 54, 0, (s32)((u8 **)D_8013B5D8)[1], 0);
        k_disp_start(set_kanji_string((s16)(strlen(((u8 **)D_8013B5D8)[0]) * 7 + strlen(((u8 **)D_8013B5D8)[1]) * 7 - 116), 54, 0, (s32)((u8 **)D_8013B5D8)[2], 0));
    }
    if (selected != 0xFF && D_8013B5C8 == thresholds.values[selected]) {
        k_reset(1);
        amusement_karaoke_result_text_page_display((s16)(selected | 0));
    }
    if (D_8013B5C8 == 6364) k_reset(1);
}

/* FAKE: padding fixes table offset; OR-zero preserves conversion; string pointers retain scalar views. T-9550 */
void amusement_karaoke_result_table_display_39(void) {
    s32 set_kanji_string(s16 x, s16 y, s32 c, s32 d, s32 e);
    s16 index;
    s16 selected;
    Date2Threshold39 thresholds;
    u8 pad[4];
    thresholds = D_8013B75C;
    selected = 0xFF;
    index = 0;
    do {
        if (D_8013B5C8 >= thresholds.values[index]) selected = index;
        index++;
    } while (index < 39);
    if (D_8013B5C8 == 1200) {
        set_kanji_string(-116, 54, 0, (s32)((u8 **)D_8013B5DC)[0], 0);
        set_kanji_string((s16)(strlen(((u8 **)D_8013B5DC)[0]) * 7 - 116), 54, 0, (s32)((u8 **)D_8013B5DC)[1], 0);
        k_disp_start(set_kanji_string((s16)(strlen(((u8 **)D_8013B5DC)[0]) * 7 + strlen(((u8 **)D_8013B5DC)[1]) * 7 - 116), 54, 0, (s32)((u8 **)D_8013B5DC)[2], 0));
    }
    if (selected != 0xFF && D_8013B5C8 == thresholds.values[selected]) {
        k_reset(1);
        amusement_karaoke_result_text_page_display((s16)(selected | 0));
    }
    if (D_8013B5C8 == 5400) k_reset(1);
}

/* FAKE: padding fixes table offset; OR-zero preserves conversion; string pointers retain scalar views. T-9550 */
void amusement_karaoke_result_table_display_81(void) {
    s32 set_kanji_string(s16 x, s16 y, s32 c, s32 d, s32 e);
    s16 index;
    s16 selected;
    Date2Threshold81 thresholds;
    u8 pad[4];
    thresholds = D_8013B7AC;
    selected = 0xFF;
    index = 0;
    do {
        if (D_8013B5C8 >= thresholds.values[index]) selected = index;
        index++;
    } while (index < 81);
    if (D_8013B5C8 == 30) {
        set_kanji_string(-116, 54, 0, (s32)((u8 **)D_8013B5E0)[0], 0);
        set_kanji_string((s16)(strlen(((u8 **)D_8013B5E0)[0]) * 7 - 116), 54, 0, (s32)((u8 **)D_8013B5E0)[1], 0);
        k_disp_start(set_kanji_string((s16)(strlen(((u8 **)D_8013B5E0)[0]) * 7 + strlen(((u8 **)D_8013B5E0)[1]) * 7 - 116), 54, 0, (s32)((u8 **)D_8013B5E0)[2], 0));
    }
    if (selected != 0xFF && D_8013B5C8 == thresholds.values[selected]) {
        k_reset(1);
        amusement_karaoke_result_text_page_display((s16)(selected | 0));
    }
    if (D_8013B5C8 == 4620) k_reset(1);
}

/* FAKE: reuse p for the last call result to save one cfe vreg slot (-2532) so uopt places the two spill homes at sp+0x44/0x48. T-24094 */
void amusement_karaoke_result_text_page_display(arg0)
    s16 arg0;
{
    s32 set_kanji_string(s16 x, s16 y, s32 c, void *d, s32 e);
    u8 buf[81][31];
    s16 i;
    u8 (*p)[31];

    if (arg0) {} /* FAKE: lowers arg0's priority so it stays in its home as in the original (no s4). T-11880 */
    switch (D_8013B5C4) {
    case 2:
        for (i = 0; i < 30; i++) {
            strcpy(buf[i], ((u8 **)D_8013B5D4)[i]);
        }
        break;
    case 0:
        for (i = 0; i < 51; i++) {
            strcpy(buf[i], ((u8 **)D_8013B5D8)[i]);
        }
        break;
    case 3:
        for (i = 0; i < 39; i++) {
            strcpy(buf[i], ((u8 **)D_8013B5DC)[i]);
        }
        break;
    default:
        for (i = 0; i < 81; i++) {
            strcpy(buf[i], ((u8 **)D_8013B5E0)[i]);
        }
        break;
    }
    if (arg0 % 6 == 0) {
        p = &buf[arg0];
        set_kanji_string(-0x74, 0x36, 2, p[0], 0);
        set_kanji_string((s16)(strlen(p[0]) * 7 - 0x74), 0x36, 0, p[1], 0);
        set_kanji_string((s16)(strlen(p[0]) * 7 + strlen(p[1]) * 7 - 0x74), 0x36, 0, p[2], 0);
        set_kanji_string(-0x54, 0x4A, 0, p[3], 0);
        set_kanji_string((s16)(strlen(p[3]) * 7 - 0x54), 0x4A, 0, p[4], 0);
        p = (void *)set_kanji_string((s16)(strlen(p[3]) * 7 + strlen(p[4]) * 7 - 0x54), 0x4A, 0, p[5], 0);
        k_disp_start((s32)p);
    } else if (arg0 % 6 == 1) {
        p = &buf[arg0];
        set_kanji_string(-0x74, 0x36, 2, p[-1], 0);
        set_kanji_string((s16)(strlen(p[-1]) * 7 - 0x74), 0x36, 2, p[0], 0);
        set_kanji_string((s16)(strlen(p[-1]) * 7 + strlen(p[0]) * 7 - 0x74), 0x36, 0, p[1], 0);
        set_kanji_string(-0x54, 0x4A, 0, p[2], 0);
        set_kanji_string((s16)(strlen(p[2]) * 7 - 0x54), 0x4A, 0, p[3], 0);
        p = (void *)set_kanji_string((s16)(strlen(p[2]) * 7 + strlen(p[3]) * 7 - 0x54), 0x4A, 0, p[4], 0);
        k_disp_start((s32)p);
    } else if (arg0 % 6 == 2) {
        p = &buf[arg0];
        set_kanji_string(-0x74, 0x36, 2, p[-2], 0);
        set_kanji_string((s16)(strlen(p[-2]) * 7 - 0x74), 0x36, 2, p[-1], 0);
        set_kanji_string((s16)(strlen(p[-2]) * 7 + strlen(p[-1]) * 7 - 0x74), 0x36, 2, p[0], 0);
        set_kanji_string(-0x54, 0x4A, 0, p[1], 0);
        set_kanji_string((s16)(strlen(p[1]) * 7 - 0x54), 0x4A, 0, p[2], 0);
        p = (void *)set_kanji_string((s16)(strlen(p[1]) * 7 + strlen(p[2]) * 7 - 0x54), 0x4A, 0, p[3], 0);
        k_disp_start((s32)p);
    } else if (arg0 % 6 == 3) {
        p = &buf[arg0];
        set_kanji_string(-0x74, 0x36, 0, p[3], 0);
        set_kanji_string((s16)(strlen(p[3]) * 7 - 0x74), 0x36, 0, p[4], 0);
        set_kanji_string((s16)(strlen(p[3]) * 7 + strlen(p[4]) * 7 - 0x74), 0x36, 0, p[5], 0);
        set_kanji_string(-0x54, 0x4A, 2, p[0], 0);
        set_kanji_string((s16)(strlen(p[0]) * 7 - 0x54), 0x4A, 0, p[1], 0);
        p = (void *)set_kanji_string((s16)(strlen(p[0]) * 7 + strlen(p[1]) * 7 - 0x54), 0x4A, 0, p[2], 0);
        k_disp_start((s32)p);
    } else if (arg0 % 6 == 4) {
        p = &buf[arg0];
        set_kanji_string(-0x74, 0x36, 0, p[2], 0);
        set_kanji_string((s16)(strlen(p[2]) * 7 - 0x74), 0x36, 0, p[3], 0);
        set_kanji_string((s16)(strlen(p[2]) * 7 + strlen(p[3]) * 7 - 0x74), 0x36, 0, p[4], 0);
        set_kanji_string(-0x54, 0x4A, 2, p[-1], 0);
        set_kanji_string((s16)(strlen(p[-1]) * 7 - 0x54), 0x4A, 2, p[0], 0);
        p = (void *)set_kanji_string((s16)(strlen(p[-1]) * 7 + strlen(p[0]) * 7 - 0x54), 0x4A, 0, p[1], 0);
        k_disp_start((s32)p);
    } else {
        p = &buf[arg0];
        set_kanji_string(-0x74, 0x36, 0, p[1], 0);
        set_kanji_string((s16)(strlen(p[1]) * 7 - 0x74), 0x36, 0, p[2], 0);
        set_kanji_string((s16)(strlen(p[1]) * 7 + strlen(p[2]) * 7 - 0x74), 0x36, 0, p[3], 0);
        set_kanji_string(-0x54, 0x4A, 2, p[-2], 0);
        set_kanji_string((s16)(strlen(p[-2]) * 7 - 0x54), 0x4A, 2, p[-1], 0);
        p = (void *)set_kanji_string((s16)(strlen(p[-2]) * 7 + strlen(p[-1]) * 7 - 0x54), 0x4A, 2, p[0], 0);
        k_disp_start((s32)p);
    }
}
