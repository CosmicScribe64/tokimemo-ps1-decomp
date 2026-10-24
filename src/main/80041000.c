#include "common.h"
#include "game.h"
#include "libapi.h"

/* The original clears GameState +0x56C through +0x66B as one byte block. */
typedef struct GameStateClearBlock {
    /* 0x00 */ u8 bytes[0x100];
} GameStateClearBlock; /* size 0x100 */

void gpu_init(void) {
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
    draw_buffer = func_80098370();
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

void game_state_init(s32 arg0) {
    func_8009C210(0);
    if (arg0 != 0) {
        func_80041168(0xFF);
        bzero(&game_state, 0x1A8C);
    }
    game_state.video_width = 0x140;
    game_state.video_height = 0xF0;
    game_state.video_interlace = 0;
    game_state.clear_color_r = 0;
    game_state.clear_color_g = 0;
    game_state.clear_color_b = 0;
    game_state.screen_center_x[0] = 0xA0;
    game_state.screen_center_x[1] = 0xA0;
    game_state.screen_center_y[0] = 0x168;
    game_state.screen_center_y[1] = 0x78;
    game_state.frame_count = 0;
    game_state.game_mode_timer = 0;
    game_state.mode_stage_timer = 0;
    game_state.mode_step_timer.w = 0;
    game_state.game_mode = 0x10;
    game_state.mode_stage = 0;
    game_state.mode_step = 0;
    game_state.pad_pressed = 0;
    game_state.pad_prev = 0;
    game_state.pad_held = 0;
    game_state.paused = 0;
    game_state.restart_request = 0;
    game_state.frame_scanline_limit = 0x106;
    /* Word initialization overlaps the byte selectors read by the menus. */
    *(s32 *)&game_state.menu_hit[3] = 0x9F;
    func_8004902C();
    game_state.back_clear = 0;
    game_state.draw_3d_enabled = 0;
    game_state.draw_2d_enabled = 0;
    game_state.sound_active = 1;
    tpage_buf_clear();
    search_tpage(0x7FFF, 0, 0);
    game_state.quads_enabled = 1;
    sprite_tables_clear();
    actor_clear_range(0);
    dec_bg_reset();
    audio_bank_state_reset();
    menu_bar_color(0x10, 0, 0, 8);
    func_80041840();
    strncpy(game_state.player_full_name, "ときめき", 0x10);
    strncpy(game_state.player_family_name, "アルバ", 8);
    strncpy(game_state.player_given_name, "ム作成", 8);
    strncpy(game_state.player_nickname, "後未セーブ", 0x10);
    /* Birthday fields share the existing word view. T-14200. */
    ((BirthdayBits *)&game_state.player_birthday)->month = 1;
    ((BirthdayBits *)&game_state.player_birthday)->day = 1;
    cursor_actor_init(0);
    cursor_show(0);
    text_marker_set(1);
    text_marker_set(0);
    func_80041878();
    func_80054C4C();
    bzero(D_80122DD0, 0x100);
    bzero(&talk_work, 0x100);
    bzero(&D_801230D0, 0x40);
    func_80042058();
}

void sprite_tables_clear(void) {
    sprite_quads_clear(0, 0x40);
    bg_layers_clear(0, 2);
}

/* One 36-byte sprite entry of the table at sprite_quads (main_api.h declares only its first word).
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

void sprite_quads_clear(s32 start, s32 end) {
    s32 i;

    for (i = start; i < end; i++) {
        ((SpriteQuadView *)&sprite_quads)[i].unk_00 = 0x80000000;
        ((SpriteQuadView *)&sprite_quads)[i].unk_04 = 0;
        ((SpriteQuadView *)&sprite_quads)[i].unk_06 = 0;
        ((SpriteQuadView *)&sprite_quads)[i].unk_08 = 0;
        ((SpriteQuadView *)&sprite_quads)[i].unk_0A = 0;
        ((SpriteQuadView *)&sprite_quads)[i].unk_0C = 0;
        ((SpriteQuadView *)&sprite_quads)[i].unk_0E = 0;
        ((SpriteQuadView *)&sprite_quads)[i].unk_0F = 0;
        ((SpriteQuadView *)&sprite_quads)[i].unk_10 = 0;
        ((SpriteQuadView *)&sprite_quads)[i].unk_12 = 0;
        ((SpriteQuadView *)&sprite_quads)[i].unk_14 = 0;
        ((SpriteQuadView *)&sprite_quads)[i].unk_15 = 0;
        ((SpriteQuadView *)&sprite_quads)[i].unk_16 = 0;
        ((SpriteQuadView *)&sprite_quads)[i].unk_18 = 0;
        ((SpriteQuadView *)&sprite_quads)[i].unk_1A = 0;
        ((SpriteQuadView *)&sprite_quads)[i].unk_1C = 0;
        ((SpriteQuadView *)&sprite_quads)[i].unk_1E = 0;
        ((SpriteQuadView *)&sprite_quads)[i].unk_20 = 0;
        game_state.quad_layer[i] = 0;
    }
}

void bg_layers_clear(s32 arg0, s32 arg1) {
    s32 i;
    GameState *g;

    g = &game_state;
    for (i = arg0; i < arg1; i++) {
        ((BgLayer *)&bg_layers)[i].unk_00 = 0x80000000;
        ((BgLayer *)&bg_layers)[i].unk_04 = 0;
        ((BgLayer *)&bg_layers)[i].unk_06 = 0;
        ((BgLayer *)&bg_layers)[i].unk_08 = 0;
        ((BgLayer *)&bg_layers)[i].unk_0A = 0;
        ((BgLayer *)&bg_layers)[i].unk_0C = 0;
        ((BgLayer *)&bg_layers)[i].unk_0E = 0;
        ((BgLayer *)&bg_layers)[i].unk_10 = 0;
        ((BgLayer *)&bg_layers)[i].unk_11 = 0;
        ((BgLayer *)&bg_layers)[i].unk_12 = 0;
        ((BgLayer *)&bg_layers)[i].unk_14 = 0;
        ((BgLayer *)&bg_layers)[i].unk_18 = 0;
        ((BgLayer *)&bg_layers)[i].unk_1A = 0;
        ((BgLayer *)&bg_layers)[i].unk_1C = 0;
        ((BgLayer *)&bg_layers)[i].unk_1E = 0;
        ((BgLayer *)&bg_layers)[i].unk_20 = 0;
        g->bg_layer_order[i] = 0x80;
    }
}

void func_80041840(void) {
    game_state.input_device = INPUT_PAD;
    game_state.cursor_speed = 3;
    game_state.menu_mode = MENU_MODE_A;
    ovl_entry = 0x80132000;
}

void func_80041878(void) {
    func_800418B0();
    func_800419FC();
    func_80041C2C();
    func_80041F48();
}

void func_800418B0(void) {
    game_state.vacation = VACATION_NONE;
    game_state.year = 0x5F;
    game_state.month = 4;
    game_state.day = 4;
    game_state.weekday = WEEKDAY_TUESDAY;
    game_state.day_count = 0;
    bzero(game_state.month_calendar[0].day_kind, 0x80);
    bzero(game_state.clubs, 0x30);
    bzero(game_state.quarter_flags, 4);
    game_state.month_calendar[0].day_kind[0] = 0x1E;
    game_state.month_calendar[0].marker[0] = 5;
    game_state.month_calendar[0].day_kind[2] = 1;
    game_state.month_calendar[0].day_kind[9] = 1;
    game_state.month_calendar[0].day_kind[16] = 1;
    game_state.month_calendar[0].day_kind[23] = 1;
    game_state.month_calendar[0].day_kind[29] = 2;
    game_state.month_calendar[0].day_kind[30] = 1;
    game_state.month_calendar[0].marker[4] = 1;
    game_state.month_calendar[1].day_kind[0] = 0x1F;
    game_state.month_calendar[1].marker[0] = 0;
    game_state.month_calendar[1].day_kind[7] = 1;
    game_state.month_calendar[1].day_kind[14] = 1;
    game_state.month_calendar[1].day_kind[21] = 1;
    game_state.month_calendar[1].day_kind[28] = 1;
    game_state.month_calendar[1].day_kind[3] = 2;
    game_state.month_calendar[1].day_kind[4] = 2;
    game_state.month_calendar[1].day_kind[5] = 2;
}

/* Three flag bits at GameState +0xF6, cleared at init (lbu/andi/sb per bit). */
typedef struct PlayerFlagsUpperBits {
    /* 0x00, bit 0 */ u8 b0 : 1;
    /* 0x00, bit 1 */ u8 b1 : 1;
    /* 0x00, bit 2 */ u8 b2 : 1;
    /* 0x00, bits 3-7 */ u8 unk_03 : 5;
} PlayerFlagsUpperBits; /* size 0x01 */

void func_800419FC(void) {
    s32 i;

    bzero(game_state.player_full_name, 0x7C);
    game_state.player_flags.b[0] = 0;
    ((NibbleView *)&game_state.player_flags.b[1])->low = 0;
    ((NibbleView *)&game_state.player_flags.b[1])->high = 0xB;
    ((PlayerFlagsUpperBits *)&game_state.player_flags_upper)->b0 = 0;
    ((PlayerFlagsUpperBits *)&game_state.player_flags_upper)->b1 = 0;
    ((PlayerFlagsUpperBits *)&game_state.player_flags_upper)->b2 = 0;
    ((BirthdayBits *)&game_state.player_birthday)->blood_type = 1;
    ((BirthdayBits *)&game_state.player_birthday)->month = 1;
    ((BirthdayBits *)&game_state.player_birthday)->day = 1;
    /* Body on the line of the for, array start folded into the index: a braced body puts the
       wrong store in the delay slot, and for (i = 0x15; ...) starts the pointer at +0x16. T-17021 */
    for (i = 0; i < 9; i++) game_state.progress_state[0x15 + i] = 0;
    game_state.param_condition.value = 0x64;
    game_state.param_liberal_arts.value = 0x28;
    game_state.param_science.value = 0x28;
    game_state.param_art.value = 0x28;
    game_state.param_athletics.value = 0x28;
    game_state.param_trivia.value = 0x20;
    game_state.param_looks.value = 0x3C;
    game_state.param_guts.value = 5;
    game_state.param_stress.value = 0;
    game_state.progress_state[0] = 1;
    game_state.progress_state[1] = 0;
    game_state.progress_state[2] = 1;
    game_state.progress_state[3] = 0;
    game_state.progress_state[4] = 1;
    game_state.progress_state[5] = 0;
    game_state.progress_state[6] = 0;
    game_state.progress_state[7] = 0;
    game_state.progress_state[8] = 1;
    game_state.progress_state[9] = 1;
    game_state.progress_state[10] = 0;
    game_state.progress_state[11] = 0;
    game_state.progress_state[12] = 1;
    game_state.progress_state[13] = 0;
    game_state.progress_state[14] = 0;
    game_state.progress_state[15] = 1;
    game_state.progress_state[16] = 0;
    game_state.progress_state[17] = 0;
    game_state.progress_state[18] = 0;
    game_state.progress_state[19] = 0;
    game_state.progress_state[20] = 0;
    memcpy(&game_state.progress_state[0x20], game_state.player_full_name, 0x7C);
}

void func_80041C2C(void) {
    GameState *g = &game_state;
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

    bzero(game_state.unk_54C, 0x20);
    bzero(game_state.clubs, 0x30);
    for (i = 0; i < 32; i++) {
        game_state.day_event_index = 0;
        ((WordView *)&game_state.day_events[i])->w = 0;
    }
    for (i = 0; i < 256; i++) {
        ((GameStateClearBlock *)game_state.scene_flags)->bytes[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        ((WordView *)&game_state.appointment_place_0)->b[i] = 0;
    }
    game_state.date_girl = GIRL_SHIORI;
    for (i = 0; i < 4; i++) {
        game_state.date_place_history[i] = 0;
    }
    i = 0;
    while (i < 16) {
        game_state.girl_last_date_spot[i] = 0;
        i++;
    }
    game_state.appointment_flag = 0;
    game_state.event_log_count = 0;
    bzero(game_state.event_log, 0x800);
}

void func_80042058(void) {
    game_state.current_girl = GIRL_SHIORI;
    game_state.text_lines_total = 0;
    game_state.text_first_line = 0;
    game_state.text_scroll_state = 0;
    game_state.unk_F63 = 0;
    game_state.icon_mode = ICON_MODE_TYPE_A;
    game_state.voice_mode = VOICE_CANCELABLE;
    game_state.click_mode = 0;
    game_state.unk_F71 = 0;
    game_state.double_click_interval = 0x20;
    game_state.confirm_button = 0;
    game_state.icon_count = 0xE;
    game_state.sd_anim_fast = 0;
}
