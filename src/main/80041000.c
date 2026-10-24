#include "common.h"
#include "game.h"
#include "libapi.h"

/* The original clears GameState +0x56C through +0x66B as one byte block. */
typedef struct GameStateClearBlock {
    /* 0x00 */ u8 bytes[0x100];
} GameStateClearBlock; /* size 0x100 */

void func_80041000(void) {
    func_80097D90(0x140, 0xF0, 0, 0, 0);
    func_80098380();
    func_80098490(0, 0, 0, 0xF0);
    D_800E8C70 = 8;
    D_800E8C74 = D_800E8CA0;
    D_800E8C84 = 8;
    D_800E8C88 = D_800E90A0;
    func_80098530();
    InitGeom();
    func_800985A0();
    D_8011ECA0 = func_80098370();
    func_800410AC();
}

/* Stack copies of the PsyQ DRAWENV (0x60 bytes here) and DISPENV (0x14 bytes); libgpu.h has only RECT. */
typedef struct DrawEnvCopy {
    /* 0x00 */ RECT clip;
    /* 0x08 */ u8 rest[0x58];
} DrawEnvCopy; /* size 0x60 */

typedef struct DispEnvCopy {
    /* 0x00 */ RECT disp;
    /* 0x08 */ RECT screen;
    /* 0x10 */ u8 rest[4];
} DispEnvCopy; /* size 0x14 */

void func_800410AC(void) {
    DrawEnvCopy draw;
    DispEnvCopy disp;

    func_8009CCBC(&draw);
    draw.clip.x = 0;
    draw.clip.y = 0;
    draw.clip.w = 320;
    draw.clip.h = 240;
    func_8009CC04(&draw);
    func_8009D10C(&disp);
    disp.disp.x = 1;
    disp.disp.y = 1;
    disp.disp.w = 320;
    disp.disp.h = 240;
    func_8009CCF4(&disp);
}

void func_8004111C(void) {
    RECT rect;

    rect.w = 640; rect.h = 480; /* FAKE: both on one source line, as1 then stores h before w */
    rect.x = 0;
    rect.y = 0;
    func_8009C7F8(&rect, 0, 0, 0);
    func_8009C674(0);
}

void func_80041168(s32 arg0) {
    s32 i;
    s32 j;
    s32 k;
    RECT rect;

    if (arg0 == 0xFF) {
        rect.x = 0;
        rect.y = 0;
        rect.w = 0x40;
        rect.h = 0x80;
        func_8009C7F8(&rect, 0, 0, 0);
        i = 1;
        do {
            func_80041168(i);
            i++;
        } while (i != 0x40);
    } else {
        j = 0;
        if (arg0 == 0xFE) {
            do {
                func_80041168(0xF - j);
                func_80041168(0x2F - j);
                func_80041168(j + 0x15);
                func_80041168(j + 0x35);
                j++;
            } while (j != 5);
        } else if (arg0 == 0) {
            k = 0;
            do {
                if (((k & 0xF) >= 5) && ((k & 0xF) < 0xF) && (k < 0x16)) {
                    func_80041168(k);
                    func_80041168(k + 0x20);
                }
                k++;
            } while (k != 0x20);
        } else {
            rect.x = (arg0 & 0xF) << 6;
            rect.y = ((arg0 & 0x10) << 4) + ((arg0 & 0x20) << 2);
            rect.w = 0x40;
            rect.h = 0x80;
        }
    }
    func_8009C7F8(&rect, 0, 0, 0);
    func_8009C674(0);
}

void func_800412E0(s32 arg0) {
    func_8009C210(0);
    if (arg0 != 0) {
        func_80041168(0xFF);
        bzero(&D_800E6280, 0x1A8C);
    }
    D_800E6280.video_width = 0x140;
    D_800E6280.video_height = 0xF0;
    D_800E6280.video_interlace = 0;
    D_800E6280.clear_color_r = 0;
    D_800E6280.clear_color_g = 0;
    D_800E6280.clear_color_b = 0;
    D_800E6280.screen_center_x[0] = 0xA0;
    D_800E6280.screen_center_x[1] = 0xA0;
    D_800E6280.screen_center_y[0] = 0x168;
    D_800E6280.screen_center_y[1] = 0x78;
    D_800E6280.frame_count = 0;
    D_800E6280.mode_timer = 0;
    D_800E6280.stage_timer = 0;
    D_800E6280.step_timer.w = 0;
    D_800E6280.mode = 0x10;
    D_800E6280.stage = 0;
    D_800E6280.step = 0;
    D_800E6280.pad_pressed = 0;
    D_800E6280.pad_prev = 0;
    D_800E6280.pad_held = 0;
    D_800E6280.paused = 0;
    D_800E6280.restart_request = 0;
    D_800E6280.frame_hcount_limit = 0x106;
    /* Word initialization overlaps the byte selectors read by the menus. */
    *(s32 *)&D_800E6280.menu_hit[3] = 0x9F;
    func_8004902C();
    D_800E6280.back_clear = 0;
    D_800E6280.draw_3d_enabled = 0;
    D_800E6280.draw_2d_enabled = 0;
    D_800E6280.sound_active = 1;
    tpage_buf_clear();
    search_tpage(0x7FFF, 0, 0);
    D_800E6280.quads_enabled = 1;
    func_80041584();
    func_80048EB8(0);
    dec_bg_reset();
    func_8004482C();
    menu_bar_color(0x10, 0, 0, 8);
    func_80041840();
    strncpy(D_800E6280.player_full_name, "ときめき", 0x10);
    strncpy(D_800E6280.player_family_name, "アルバ", 8);
    strncpy(D_800E6280.player_given_name, "ム作成", 8);
    strncpy(D_800E6280.player_nickname, "後未セーブ", 0x10);
    /* Birthday fields share the existing word view. T-14200. */
    ((BirthdayBits *)&D_800E6280.player_birthday)->month = 1;
    ((BirthdayBits *)&D_800E6280.player_birthday)->day = 1;
    func_8006BC28(0);
    func_8006BD6C(0);
    func_8006C848(1);
    func_8006C848(0);
    func_80041878();
    func_80054C4C();
    bzero(D_80122DD0, 0x100);
    bzero(&D_80122CD0, 0x100);
    bzero(&D_801230D0, 0x40);
    func_80042058();
}

void func_80041584(void) {
    func_800415B4(0, 0x40);
    func_8004164C(0, 2);
}

/* One 36-byte sprite entry of the table at D_801217D0 (main_api.h declares only its first word).
 * Indexing the table as an array of this struct keeps IDO from unrolling the loop below, as in the
 * original (T-5020, wiki/matching-notes.md "Loop unrolling"). */
typedef struct SpriteQuadView {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ u8 unk_0E;
    /* 0x0F */ u8 unk_0F;
    /* 0x10 */ s16 unk_10;
    /* 0x12 */ s16 unk_12;
    /* 0x14 */ u8 unk_14;
    /* 0x15 */ u8 unk_15;
    /* 0x16 */ u8 unk_16;
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ s16 unk_1A;
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ s16 unk_1E;
    /* 0x20 */ s32 unk_20;
} SpriteQuadView; /* size 0x24 */

void func_800415B4(s32 start, s32 end) {
    s32 i;

    for (i = start; i < end; i++) {
        ((SpriteQuadView *)&D_801217D0)[i].unk_00 = 0x80000000;
        ((SpriteQuadView *)&D_801217D0)[i].unk_04 = 0;
        ((SpriteQuadView *)&D_801217D0)[i].unk_06 = 0;
        ((SpriteQuadView *)&D_801217D0)[i].unk_08 = 0;
        ((SpriteQuadView *)&D_801217D0)[i].unk_0A = 0;
        ((SpriteQuadView *)&D_801217D0)[i].unk_0C = 0;
        ((SpriteQuadView *)&D_801217D0)[i].unk_0E = 0;
        ((SpriteQuadView *)&D_801217D0)[i].unk_0F = 0;
        ((SpriteQuadView *)&D_801217D0)[i].unk_10 = 0;
        ((SpriteQuadView *)&D_801217D0)[i].unk_12 = 0;
        ((SpriteQuadView *)&D_801217D0)[i].unk_14 = 0;
        ((SpriteQuadView *)&D_801217D0)[i].unk_15 = 0;
        ((SpriteQuadView *)&D_801217D0)[i].unk_16 = 0;
        ((SpriteQuadView *)&D_801217D0)[i].unk_18 = 0;
        ((SpriteQuadView *)&D_801217D0)[i].unk_1A = 0;
        ((SpriteQuadView *)&D_801217D0)[i].unk_1C = 0;
        ((SpriteQuadView *)&D_801217D0)[i].unk_1E = 0;
        ((SpriteQuadView *)&D_801217D0)[i].unk_20 = 0;
        D_800E6280.quad_layer[i] = 0;
    }
}

void func_8004164C(s32 arg0, s32 arg1) {
    s32 i;
    GameState *g;

    g = &D_800E6280;
    for (i = arg0; i < arg1; i++) {
        ((BgLayer *)&D_801220D0)[i].unk_00 = 0x80000000;
        ((BgLayer *)&D_801220D0)[i].unk_04 = 0;
        ((BgLayer *)&D_801220D0)[i].unk_06 = 0;
        ((BgLayer *)&D_801220D0)[i].unk_08 = 0;
        ((BgLayer *)&D_801220D0)[i].unk_0A = 0;
        ((BgLayer *)&D_801220D0)[i].unk_0C = 0;
        ((BgLayer *)&D_801220D0)[i].unk_0E = 0;
        ((BgLayer *)&D_801220D0)[i].unk_10 = 0;
        ((BgLayer *)&D_801220D0)[i].unk_11 = 0;
        ((BgLayer *)&D_801220D0)[i].unk_12 = 0;
        ((BgLayer *)&D_801220D0)[i].unk_14 = 0;
        ((BgLayer *)&D_801220D0)[i].unk_18 = 0;
        ((BgLayer *)&D_801220D0)[i].unk_1A = 0;
        ((BgLayer *)&D_801220D0)[i].unk_1C = 0;
        ((BgLayer *)&D_801220D0)[i].unk_1E = 0;
        ((BgLayer *)&D_801220D0)[i].unk_20 = 0;
        g->bg_layer_order[i] = 0x80;
    }
}

void func_80041840(void) {
    D_800E6280.input_device = INPUT_PAD;
    D_800E6280.cursor_speed = 3;
    D_800E6280.menu_mode = MENU_MODE_A;
    D_80123110 = 0x80132000;
}

void func_80041878(void) {
    func_800418B0();
    func_800419FC();
    func_80041C2C();
    func_80041F48();
}

void func_800418B0(void) {
    D_800E6280.vacation = VACATION_NONE;
    D_800E6280.year = 0x5F;
    D_800E6280.month = 4;
    D_800E6280.day = 4;
    D_800E6280.weekday = WEEKDAY_TUESDAY;
    D_800E6280.day_count = 0;
    bzero(D_800E6280.month_calendar[0].day_kind, 0x80);
    bzero(D_800E6280.clubs, 0x30);
    bzero(D_800E6280.quarter_flags, 4);
    D_800E6280.month_calendar[0].day_kind[0] = 0x1E;
    D_800E6280.month_calendar[0].marker[0] = 5;
    D_800E6280.month_calendar[0].day_kind[2] = 1;
    D_800E6280.month_calendar[0].day_kind[9] = 1;
    D_800E6280.month_calendar[0].day_kind[16] = 1;
    D_800E6280.month_calendar[0].day_kind[23] = 1;
    D_800E6280.month_calendar[0].day_kind[29] = 2;
    D_800E6280.month_calendar[0].day_kind[30] = 1;
    D_800E6280.month_calendar[0].marker[4] = 1;
    D_800E6280.month_calendar[1].day_kind[0] = 0x1F;
    D_800E6280.month_calendar[1].marker[0] = 0;
    D_800E6280.month_calendar[1].day_kind[7] = 1;
    D_800E6280.month_calendar[1].day_kind[14] = 1;
    D_800E6280.month_calendar[1].day_kind[21] = 1;
    D_800E6280.month_calendar[1].day_kind[28] = 1;
    D_800E6280.month_calendar[1].day_kind[3] = 2;
    D_800E6280.month_calendar[1].day_kind[4] = 2;
    D_800E6280.month_calendar[1].day_kind[5] = 2;
}

/* Three flag bits at GameState +0xF6, cleared at init (lbu/andi/sb per bit). */
typedef struct PlayerFlagsHiBits {
    /* 0x00, bit 0 */ u8 b0 : 1;
    /* 0x00, bit 1 */ u8 b1 : 1;
    /* 0x00, bit 2 */ u8 b2 : 1;
    /* 0x00, bits 3-7 */ u8 unk_03 : 5;
} PlayerFlagsHiBits; /* size 0x01 */

void func_800419FC(void) {
    s32 i;

    bzero(D_800E6280.player_full_name, 0x7C);
    D_800E6280.player_flags.b[0] = 0;
    ((NibbleView *)&D_800E6280.player_flags.b[1])->low = 0;
    ((NibbleView *)&D_800E6280.player_flags.b[1])->high = 0xB;
    ((PlayerFlagsHiBits *)&D_800E6280.player_flags_hi)->b0 = 0;
    ((PlayerFlagsHiBits *)&D_800E6280.player_flags_hi)->b1 = 0;
    ((PlayerFlagsHiBits *)&D_800E6280.player_flags_hi)->b2 = 0;
    ((BirthdayBits *)&D_800E6280.player_birthday)->blood_type = 1;
    ((BirthdayBits *)&D_800E6280.player_birthday)->month = 1;
    ((BirthdayBits *)&D_800E6280.player_birthday)->day = 1;
    /* Body on the line of the for, array start folded into the index: a braced body puts the
       wrong store in the delay slot, and for (i = 0x15; ...) starts the pointer at +0x16. T-17021 */
    for (i = 0; i < 9; i++) D_800E6280.progress_state[0x15 + i] = 0;
    D_800E6280.param_condition.value = 0x64;
    D_800E6280.param_liberal_arts.value = 0x28;
    D_800E6280.param_science.value = 0x28;
    D_800E6280.param_art.value = 0x28;
    D_800E6280.param_athletics.value = 0x28;
    D_800E6280.param_trivia.value = 0x20;
    D_800E6280.param_looks.value = 0x3C;
    D_800E6280.param_guts.value = 5;
    D_800E6280.param_stress.value = 0;
    D_800E6280.progress_state[0] = 1;
    D_800E6280.progress_state[1] = 0;
    D_800E6280.progress_state[2] = 1;
    D_800E6280.progress_state[3] = 0;
    D_800E6280.progress_state[4] = 1;
    D_800E6280.progress_state[5] = 0;
    D_800E6280.progress_state[6] = 0;
    D_800E6280.progress_state[7] = 0;
    D_800E6280.progress_state[8] = 1;
    D_800E6280.progress_state[9] = 1;
    D_800E6280.progress_state[10] = 0;
    D_800E6280.progress_state[11] = 0;
    D_800E6280.progress_state[12] = 1;
    D_800E6280.progress_state[13] = 0;
    D_800E6280.progress_state[14] = 0;
    D_800E6280.progress_state[15] = 1;
    D_800E6280.progress_state[16] = 0;
    D_800E6280.progress_state[17] = 0;
    D_800E6280.progress_state[18] = 0;
    D_800E6280.progress_state[19] = 0;
    D_800E6280.progress_state[20] = 0;
    memcpy(&D_800E6280.progress_state[0x20], D_800E6280.player_full_name, 0x7C);
}

void func_80041C2C(void) {
    GameState *g = &D_800E6280;
    s32 i;

    bzero(g->girls, 0x380);
    for (i = 0; i < 16; i++) {
        g->girls[i].flags.f.met = 0;
        g->girls[i].flags.f.b3 = 0;
        g->girls[i].flags.f.b6 = 0;
        g->girls[i].tokimeki = 0;
        g->girls[i].affection = 0x32;
        g->girls[i].syoushin = 0;
        g->girls[i].flags.b[3] = 0;
        g->girls[i].flags.f.b7 = 0;
        g->girls[i].flags.f.story_stage = 0;
        g->girls[i].flags.f.club = CLUB_NONE;
    }
    g->girls[0].flags.f.met = 1;
    g->girls[12].flags.f.met = 1;
    g->girls[0].flags.f.b3 = 1;
    g->girls[0].flags.f.b6 = 1;
    g->girls[0].flags.f.b7 = 1;
    g->girls[12].flags.f.b3 = 1;
    g->girls[0].birthday.date.month = 2;
    g->girls[0].birthday.date.day = 14;
    g->girls[1].birthday.date.month = 2;
    g->girls[1].birthday.date.day = 3;
    g->girls[2].birthday.date.month = 7;
    g->girls[2].birthday.date.day = 7;
    g->girls[3].birthday.date.month = 9;
    g->girls[3].birthday.date.day = 30;
    g->girls[4].birthday.date.month = 1;
    g->girls[4].birthday.date.day = 13;
    g->girls[5].birthday.date.month = 6;
    g->girls[5].birthday.date.day = 13;
    g->girls[6].birthday.date.month = 12;
    g->girls[6].birthday.date.day = 3;
    g->girls[7].birthday.date.month = 11;
    g->girls[7].birthday.date.day = 15;
    g->girls[8].birthday.date.month = 10;
    g->girls[8].birthday.date.day = 17;
    g->girls[9].birthday.date.month = 9;
    g->girls[9].birthday.date.day = 5;
    g->girls[10].birthday.date.month = 5;
    g->girls[10].birthday.date.day = 16;
    g->girls[11].birthday.date.month = 3;
    g->girls[11].birthday.date.day = 3;
    g->girls[12].birthday.date.month = 8;
    g->girls[12].birthday.date.day = 23;
    g->girls[13].birthday.date.month = 4;
    g->girls[13].birthday.date.day = 4;
}

void func_80041F48(void) {
    s32 i;

    bzero(D_800E6280.unk_54C, 0x20);
    bzero(D_800E6280.clubs, 0x30);
    for (i = 0; i < 32; i++) {
        D_800E6280.day_event_index = 0;
        ((WordView *)&D_800E6280.day_events[i])->w = 0;
    }
    for (i = 0; i < 256; i++) {
        ((GameStateClearBlock *)D_800E6280.event_flags)->bytes[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        ((WordView *)&D_800E6280.appt_place_0)->b[i] = 0;
    }
    D_800E6280.date_girl = GIRL_SHIORI;
    for (i = 0; i < 4; i++) {
        D_800E6280.date_place_history[i] = 0;
    }
    i = 0;
    while (i < 16) {
        D_800E6280.girl_last_date_spot[i] = 0;
        i++;
    }
    D_800E6280.appointment_flag = 0;
    D_800E6280.event_log_count = 0;
    bzero(D_800E6280.event_log, 0x800);
}

void func_80042058(void) {
    D_800E6280.current_girl = GIRL_SHIORI;
    D_800E6280.text_lines_total = 0;
    D_800E6280.text_first_line = 0;
    D_800E6280.text_scroll_state = 0;
    D_800E6280.unk_F63 = 0;
    D_800E6280.icon_mode = ICON_MODE_TYPE_A;
    D_800E6280.voice_mode = VOICE_CANCELABLE;
    D_800E6280.click_mode = 0;
    D_800E6280.unk_F71 = 0;
    D_800E6280.double_click_interval = 0x20;
    D_800E6280.confirm_button = 0;
    D_800E6280.icon_count = 0xE;
    D_800E6280.sd_anim_fast = 0;
}
