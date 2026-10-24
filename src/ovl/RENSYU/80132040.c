#include "ovl/RENSYU.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_801342C0);
u8 D_801342C4 = 0;
u8 D_801342C8[5] = { 0 };
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_801342D0);
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_801342D4);
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_801342F4);
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_80134328);
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_8013435C);
RensyuTbl11 D_80134378 = { { 0x1E, 0x3C, 0x5A, 0x78, 0x96, 0xB4, 0xD2, 0xF0, 0x10E, 0x12C, 0x14A } };
RensyuTbl11 D_80134390 = { { 0x1E, 0x3C, 0x5A, 0x78, 0x96, 0xB4, 0xD2, 0xF0, 0x10E, 0x12C, 0x14A } };
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_801343A8);
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_801343D0);
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_801343D4);
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_801343D8);
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_801343DC);
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_801343E0);

typedef struct {
    void (*f[8])();
} FnTbl8; /* size 0x20 */
extern FnTbl8 D_801342D4;

void func_80132040(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl8 tbl;

    tbl = D_801342D4;
    idx = game_state.mode_stage;
    tbl.f[idx]();
}

void func_801320E0(void) {
    u32 temp_t4;

    switch (game_state.mode_step) {
    case 0:
        audio_command(0xBF);
        audio_bank_load_start(0, 0xBF98, 0xBF79, 0xC982, 0xC935, 0xC91B);
        scene_screen_reset(1, 0);
        talk_clear();
        message_window_init();
        icon_disp_switch(0);
        func_8006764C(0);
        parameter_disp_switch(0);
        hizuke_init();
        k_reset(0);
        message_window_set_title((s32) "練習試合");
        message_window_title_show(0);
        dec_bg_reset();
        set_dec_bri(0);
        srand(game_state.timer_ticks);
        mode_step_next();
        break;
    case 1:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x206);
            mode_step_next();
        }
        break;
    case 2:
        cd_read_sectors(7U, 0x801E8000, 0xB037);
        mode_step_next();
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            talk_names_set(0U);
            text_marker_hide();
            func_80132000();
            talk_text_table = D_801342B0;
            talk_command_table = D_801342B4;
            temp_t4 = (u16) game_state.player_flags.h >> 0xC;
            talk_voice_table = D_801342B8;
            switch (temp_t4) {
            case 6:
                talk_group = 0;
                break;
            case 7:
                talk_group = 1;
                break;
            case 8:
                talk_group = 2;
                break;
            case 9:
                talk_group = 3;
                break;
            case 10:
                talk_group = 4;
                break;
            default:
                game_mode_reset_to_title();
                break;
            }
            talk_line = 0;
            mode_step_next();
        }
        break;
    case 4:
    case 6:
        normal_date_speak();
        break;
    case 5:
        /* The target reads the adjacent record bytes +2/+3 as one halfword. */
        talk_group = (*(u16 *)&game_state.clubs[(u16) game_state.player_flags.h >> 0xC].level & 0xF) + 5;
        talk_line = 0;
        mode_step_next();
        break;
    case 7:
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

/* FAKE: T-24138. Reading game_state.month into unused `pad2` at entry puts it in
 * cvt_pass's entry set so its comparisons lose CVT J<-L; `unk_03F + (unk_03F * 0)` in
 * the 3rd `&&` clause keeps CVT J<-L on both loads so uopt CSEs `cvt(unk_03F)` into $v0
 * there; `((unk_03F < kU) * 0)` in the two `else if` conditions strips CVT J<-L via
 * COMPARE_OPS and extends unk_03F's live range so uopt emits RLOD into $v0 at .L801324CC
 * while ugen discards `* 0` with zero instructions. */
void func_80132390(void) {
    s32 sum0;
    s32 sum10;
    s32 temp_a0;
    s32 var_s0;
    s32 pad2;
    RensyuWords13 tbl1;
    RensyuWords13 tbl2;

    pad2 = game_state.month;
    tbl1 = D_801342F4;
    tbl2 = D_80134328;
    switch (game_state.mode_step) {
    case 0:
        temp_a0 = func_8008647C();
        if (((((u32)game_state.unk_54C[0].w << 2) >> 31) == 1) &&
            (game_state.year == (((u32)game_state.unk_54C[0].w << 20) >> 25)) &&
            ((game_state.month + (game_state.month * 0)) == (game_state.unk_54C[0].h[0] >> 12)) &&
            (game_state.day == (game_state.unk_54C[0].h[1] & 0x1F))) {
            D_801342D0 = 0xFF;
            mode_step_set(6);
        } else if ((((((u32)game_state.girls[11].flags.w << 20) >> 29) + ((game_state.month < 1U) * 0)) >= 3U) &&
                   ((game_state.girls[11].scene_counters[0xB] + ((game_state.month < 2U) * 0)) == 0)) {
            game_state.girls[11].scene_counters[0xB] = 1;
            D_801342D0 = 0xB;
            if ((game_state.month >= 6U) && (game_state.month < 10U)) {
                portrait_read_start(tbl1.v[11]);
            } else {
                portrait_read_start(tbl2.v[11]);
            }
            mode_step_next();
        } else if (((game_state.girls[12].flags.b[3] + ((game_state.month < 3U) * 0)) >= 0x41U) &&
                   ((game_state.girls[12].scene_counters[0xC] + ((game_state.month < 4U) * 0)) == 0)) {
            game_state.girls[12].scene_counters[0xC] = 1;
            D_801342D0 = 0xC;
            if ((game_state.month >= 6U) && (game_state.month < 10U)) {
                portrait_read_start(tbl1.v[12]);
            } else {
                portrait_read_start(tbl2.v[12]);
            }
            mode_step_next();
        } else {
            if (girl_state_rank(temp_a0) < 2) {
                D_801342D0 = temp_a0;
                for (var_s0 = 0; var_s0 < 11; var_s0++) {
                    if ((((u16)game_state.player_flags.h >> 12) == ((u8)game_state.girls[var_s0].flags.b[2] >> 4)) &&
                        ((((u32)game_state.girls[var_s0].flags.w << 30) >> 31) == 1)) {
                        if (girl_state_rank(var_s0) < 2) {
                            D_801342D0 = var_s0;
                        }
                        if ((((u8)game_state.girls[0].flags.b[2] >> 4) == 10) &&
                            (((u16)game_state.player_flags.h >> 12) == 10) &&
                            ((((u32)game_state.girls[10].flags.w << 30) >> 31) == 1)) {
                            if ((girl_state_rank(0) < 2) && (girl_state_rank(10) >= 2)) {
                                D_801342D0 = 0;
                            } else if ((girl_state_rank(0) >= 2) && (girl_state_rank(10) < 2)) {
                                D_801342D0 = 10;
                            } else if ((girl_state_rank(0) < 2) && (girl_state_rank(10) < 2)) {
                                sum0 = game_state.girls[0].affection + game_state.girls[0].tokimeki;
                                sum10 = game_state.girls[10].affection + game_state.girls[10].tokimeki;
                                if (sum0 >= sum10) {
                                    D_801342D0 = 0;
                                } else {
                                    D_801342D0 = 10;
                                }
                            }
                        }
                    }
                }
                if (((u16)game_state.player_flags.h >> 12) == ((u8)game_state.girls[D_801342D0].flags.b[2] >> 4)) {
                    func_80133E5C();
                } else if ((game_state.month >= 6U) && (game_state.month < 10U)) {
                    portrait_read_start(tbl1.v[D_801342D0]);
                } else {
                    portrait_read_start(tbl2.v[D_801342D0]);
                }
                mode_step_next();
            } else {
                D_801342D0 = 0xFF;
                mode_step_set(6);
            }
        }
        break;
    case 1:
        mode_step_next();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            switch ((u16)game_state.player_flags.h >> 12) {
            case 6:
            case 7:
            case 8:
                dec_bg_cd_read(0x4097, 1);
                break;
            case 9:
                dec_bg_cd_read(0x40A0, 1);
                break;
            case 10:
                dec_bg_cd_read(0x40A9, 1);
                break;
            default:
                game_mode_reset_to_title();
                break;
            }
            mode_step_next();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            message_window_title_show(1);
            D_80125CA0 = 0xC0;
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            audio_command(0xC1);
            audio_bank_load_start(1, 0xC043, 0xC01D, D_800B3688[D_801342D0], D_800B36C8[D_801342D0], D_800B3708[D_801342D0]);
            mode_step_next();
        }
        break;
    case 4:
        normal_date_bg_fadein();
        break;
    case 5:
        audio_command(0x71);
        mode_stage_next();
        break;
    case 6:
        mode_step_next();
        break;
    case 7:
        talk_group = 0x10;
        talk_line = 0;
        mode_step_next();
    case 8:
        normal_date_speak();
        break;
    case 9:
        mode_stage_set(3);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

/* FAKE: T-24138. Unused `pad` and `a` place `tbl` at 0x30(sp) and `sp2F` at 0x2F(sp);
 * reading D_801342D0 before portrait_wink_update() puts it in cvt_pass's entry set; adding
 * `(((pad + 1) + (pad + 1)) * 0)` in case 6 places a CSE into $v0 inside `* 0` (discarded
 * by ugen with zero instructions) so cvt_pass's `_constant_first` does not swap $v0 first
 * in `D_801342D0 == 6` and `D_801342D0 == 10` in case 11. */
void func_80132A28(void) {
    s32 pad;
    s16 a;
    RensyuTbl13 tbl;
    u8 sp2F;

    tbl = D_8013435C;
    a = D_801342D0;
    portrait_wink_update();
    switch (game_state.mode_step) {
    case 0:
        if (audio_bank_load_poll() == 1) {
            cd_read_sectors(7U, (void *)0x801E8000, 0xB037);
            mode_step_next();
        }
        break;
    case 2:
    case 4:
    case 10:
        normal_date_speak();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            audio_bank_start(1, 0);
            talk_names_set(D_801342D0);
            func_80132000();
            talk_text_table = D_801342B0;
            talk_command_table = D_801342B4;
            talk_voice_table = D_801342B8;
            talk_group = tbl.v[D_801342D0];
            talk_line = 0;
            addr_init_bustup();
            game_state.current_girl = D_801342D0;
            portrait_show_checked();
            if (D_801342D0 == 0xC) {
                portrait_layout_set(0);
            }
            if (D_801342D0 == 0xB) {
                audio_command(0x500);
                func_80047550();
            }
        }
        break;
    case 3:
        normal_date_girl_in();
        break;
    case 5:
        if (D_801342D0 == 0xB) {
            normal_date_girl_out();
            talk_group = 0x5E;
        } else {
            talk_group = 0x10;
            mode_step_next();
        }
        break;
    case 6:
        audio_command(0xC1 + (((pad + 1) + (pad + 1)) * 0));
        Vblnk_Timer_Init();
        mode_step_next();
        break;
    case 7:
        if (D_801342D0 == 0xB) {
            normal_date_speak();
        } else {
            mode_step_next();
        }
        break;
    case 8:
        if (D_801342D0 == 0xB) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 9:
        audio_bank_start(0, 6);
        if (D_801342D0 == 0xB) {
            mode_step_next();
            mode_step_next();
        } else {
            mode_step_next();
        }
        break;
    case 11:
        switch ((u16) game_state.player_flags.h >> 0xC) {
        case 6:
            if (((((u8) game_state.girls[0].flags.b[2] >> 4) == 6) && (D_801342D0 == 0)) ||
                ((((u8) game_state.girls[4].flags.b[2] >> 4) == 6) && (D_801342D0 == 4))) {
                sp2F = 5;
            } else {
                sp2F = 0;
            }
            break;
        case 7:
            if (((((u8) game_state.girls[0].flags.b[2] >> 4) == 7) && (D_801342D0 == 0)) ||
                ((((u8) game_state.girls[4].flags.b[2] >> 4) == 7) && (D_801342D0 == 4))) {
                sp2F = 6;
            } else {
                sp2F = 1;
            }
            break;
        case 8:
            if (((((u8) game_state.girls[0].flags.b[2] >> 4) == 8) && (D_801342D0 == 0)) ||
                (D_801342D0 == 5)) {
                sp2F = 7;
            } else {
                sp2F = 2;
            }
            break;
        case 9:
            if (((((u8) game_state.girls[0].flags.b[2] >> 4) == 9) && (D_801342D0 == 0)) ||
                (D_801342D0 == 6)) {
                sp2F = 8;
            } else {
                sp2F = 3;
            }
            break;
        case 10:
            if (((((u8) game_state.girls[0].flags.b[2] >> 4) == 10) && (D_801342D0 == 0)) ||
                ((((u8) game_state.girls[10].flags.b[2] >> 4) == 10) && (D_801342D0 == 10))) {
                sp2F = 9;
            } else {
                sp2F = 4;
            }
            break;
        }
        event_log_add(0x12, sp2F);
        message_window_title_show(0);
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

void func_80132F10(void) {
    s32 var_s0;

    switch (game_state.mode_step) {
    case 0:
        switch ((u16) game_state.player_flags.h >> 0xC) {
        case 6:
            dec_bg_cd_read(0x417A, 1);
            break;
        case 7:
            dec_bg_cd_read(0x4183, 1);
            break;
        case 8:
            dec_bg_cd_read(0x419E, 1);
            break;
        case 9:
            dec_bg_cd_read(0x4195, 1);
            break;
        case 10:
            dec_bg_cd_read(0x418D, 1);
            break;
        default:
            game_mode_reset_to_title();
            break;
        }
        mode_step_next();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            switch ((u16) game_state.player_flags.h >> 0xC) {
            case 6:
                cd_read_sectors(0x1BU, 0x80197000, 0xB03E);
                break;
            case 7:
                cd_read_sectors(0x1AU, 0x80197000, 0xAFE7);
                break;
            case 8:
                cd_read_sectors(0x1BU, 0x80197000, 0xB01C);
                break;
            case 9:
                cd_read_sectors(0x1BU, 0x80197000, 0xB001);
                break;
            case 10:
                cd_read_sectors(0x1BU, 0x80197000, 0xAFCC);
                break;
            default:
                game_mode_reset_to_title();
                break;
            }
            message_window_title_show(1);
            mode_step_next();
        }
        break;
    case 2:
        normal_date_bg_fadein();
        break;
    case 3:
        k_reset(1);
        set_kanji_string(-0x80, 0x32, 0, (s32) "試合中", 0);
        k_disp_start(1);
        mode_step_next();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            switch ((u16) game_state.player_flags.h >> 0xC) {
            case 6:
                func_801340B0();
                break;
            case 7:
                func_80133FB4();
                break;
            case 8:
                func_8013405C();
                break;
            case 9:
                func_80134008();
                break;
            case 10:
                func_80133F60();
                break;
            default:
                game_mode_reset_to_title();
                break;
            }
            var_s0 = 0;
            do {
                load_palette(D_801343E0, (var_s0 + 0x10) & 0xFF, 1, 1, var_s0);
                var_s0 += 1;
            } while (var_s0 != 5);
            mode_stage_next();
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

/* FAKE: scalar byte views on unk_110A and D_801342C4 preserve the target live ranges and
 * swap_count store placement; unused local `a` and reusing `value` as the second shuffle
 * index place `tbl` at 0x40(sp) and `swap_count` at 0x64(sp) (T-19020, T-24102). */
void func_8013325C(void) {
    u8 swap;
    u8 other;
    s32 i;
    s32 swap_count;
    u8 *a;
    s32 first;
    s32 value;
    RensyuTbl11 tbl;

    tbl = D_80134378;
    switch (*(u8 *)&game_state.mode_step) {
    case 0:
        value = (s16)(game_state.clubs[(u16) game_state.player_flags.h >> 0xC].practice_points - tbl.v[((RensyuClubCount *)&game_state.clubs[(u16) game_state.player_flags.h >> 0xC])->count]);
        if (value < -0x1E) {
            (*(u8 *)&D_801342C4) = 0;
        } else if (value < -0x14) {
            (*(u8 *)&D_801342C4) = 1;
        } else if (value < -0xA) {
            (*(u8 *)&D_801342C4) = 1;
        } else if (value < 0) {
            (*(u8 *)&D_801342C4) = 2;
        } else if (value >= 0x1E) {
            (*(u8 *)&D_801342C4) = 5;
        } else if (value >= 0x14) {
            (*(u8 *)&D_801342C4) = 4;
        } else if (value >= 0xA) {
            (*(u8 *)&D_801342C4) = 4;
        } else if (value >= 0) {
            (*(u8 *)&D_801342C4) = 3;
        }
        mode_step_next();
        break;
    case 1:
        for (i = 0; i < 5; i++) {
            D_801342C8[i] = 0;
        }
        for (i = 0; i < (u8)(*(u8 *)&D_801342C4); i++) {
            D_801342C8[i] = 1;
        }
        i = 0;
        swap_count = rand() & 0xF;
        if ((u8)(*(u8 *)&D_801342C4) >= 3U) {
            swap = D_801342C8[0];
            other = D_801342C8[4];
            D_801342C8[4] = swap;
            D_801342C8[0] = other;
        }
        if (swap_count > 0) {
            do {
                first = rand() & 3;
                value = rand() & 3;
                swap = D_801342C8[first];
                i++;
                other = D_801342C8[value];
                D_801342C8[first] = other;
                D_801342C8[value] = swap;
            } while (i != swap_count);
        }
        if (((u16) game_state.player_flags.h >> 0xC) == 9) {
            if ((u8)(*(u8 *)&D_801342C4) >= 3U) {
                D_801342C8[0] = 1;
            } else {
                D_801342C8[0] = 0;
            }
        }
        mode_step_next();
        break;
    case 2:
        actor_clear(0x60);
        ((Actor *)actor_pool)[96].unk_01 = 3;
        ((Actor *)actor_pool)[96].quad_flags = 0x01000000;
        ((Actor *)actor_pool)[96].brightness = 0x80;
        ((Actor *)actor_pool)[96].unk_05 = 0x80;
        ((Actor *)actor_pool)[96].scale_x = 0x1000;
        ((Actor *)actor_pool)[96].scale_y = 0x1000;
        ((Actor *)actor_pool)[96].blink_period = 1;
        ((Actor *)actor_pool)[96].unk_43 = ((RensyuClubCount *)&game_state.clubs[(u16) game_state.player_flags.h >> 0xC])->count % 5U + 0x10;
        ((Actor *)actor_pool)[96].x = -0x60;
        ((Actor *)actor_pool)[96].y = 0x20;
        ((Actor *)actor_pool)[96].frame_table = (u8 *) D_801343D4;
        ((Actor *)actor_pool)[96].sequence_table = (u8 *) D_801343D8;
        ((Actor *)actor_pool)[96].tpage_table = (u8 *) D_801343D0;
        ((Actor *)actor_pool)[96].unk_14 = D_801343DC;
        D_801342C0 = 0;
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

/* FAKE: T-24138. Adding `(s32)&tbl * 0` in `case 0` keeps `&tbl` ($a0) live across
 * the jump-table entry into `case 0` so uopt does not emit `UNAL(0,$4,$gp)` (`.alias $4,$gp`)
 * immediately before the `case 0` label; ugen discards `(s32)&tbl * 0` with zero instructions
 * emitted and as1 schedules `lui $v0` into the `lbu $t1` load-delay slot. */
void func_80133628(void) {
    s32 girl;
    s16 diff;
    RensyuTbl11 tbl;

    tbl = D_80134390;
    switch (game_state.mode_step) {
    case 0:
        if ((((Actor *)actor_pool)[96].flags + ((s32)&tbl * 0)) == 0) {
            if ((D_801342C0 >= 5U) || (D_801342C0 && (((u16) game_state.player_flags.h >> 0xC) == 9))) {
                mode_step_next();
            } else {
                ((Actor *)actor_pool)[96].draw_flags = 0x84;
                ((Actor *)actor_pool)[96].flags = 5;
                ((Actor *)actor_pool)[96].unk_18 = 0;
                ((Actor *)actor_pool)[96].unk_08 = 0;
                if (D_801342C8[D_801342C0] == 1) {
                    ((Actor *)actor_pool)[96].sequence_slot = 0;
                } else {
                    ((Actor *)actor_pool)[96].sequence_slot = 1;
                }
                ++D_801342C0;
            }
        }
        break;
    case 1:
        if (D_801342C4 >= 3U) {
            audio_command(0x204);
            talk_group = 0x11;
        } else {
            audio_command(0x205);
            talk_group = ((RensyuClubCount *)&game_state.clubs[(u16) game_state.player_flags.h >> 0xC])->count + 0x12;
        }
        k_reset(1);
        mode_step_next();
        break;
    case 2:
        normal_date_speak();
        break;
    case 3:
        girl = (u16) game_state.player_flags.h >> 0xC;
        diff = game_state.clubs[girl].practice_points - tbl.v[((RensyuClubCount *)&game_state.clubs[girl])->count];
        if (diff < -0x1E) {
            talk_group = 0x1D;
        } else if (diff < -0x14) {
            talk_group = 0x1E;
        } else if (diff < -0xA) {
            talk_group = 0x1F;
        } else if (diff < 0) {
            talk_group = 0x20;
        } else if (diff >= 0x1E) {
            talk_group = 0x24;
        } else if (diff >= 0x14) {
            talk_group = 0x23;
        } else if (diff >= 0xA) {
            talk_group = 0x22;
        } else if (diff >= 0) {
            talk_group = 0x21;
        }
        mode_step_next();
        break;
    case 4:
        normal_date_speak();
        break;
    case 5:
        ((Actor *)actor_pool)[96].brightness -= 4;
        if (((Actor *)actor_pool)[96].brightness < 3U) {
            ((Actor *)actor_pool)[96].brightness = 0;
        }
        normal_date_bg_fadeout();
        break;
    case 6:
        actor_clear(0x60);
        if ((D_801342D0 == 0xB) || (D_801342D0 == 0xFF)) {
            mode_stage_set(7);
        } else {
            mode_stage_next();
        }
        break;
    }
    Default_Disp();
}

/* FAKE: T-24122. Unused `pad` and `a` place `tbl` at 0x30(sp); reading D_801342D0 before
 * portrait_wink_update() puts it in cvt_pass's entry set so uopt promotes it to $a0 across the
 * switch; the empty raw-byte test on unk_1BC[4].unk_0C.b[2] retains separate live ranges
 * for the raw byte ($a1) and its high nibble ($v0), matching SHOUGATU:func_801341E4. */
void func_80133990(void) {
    s32 pad;
    s16 a;
    RensyuTbl13 tbl;

    tbl = D_801343A8;
    a = D_801342D0;
    portrait_wink_update();
    switch (game_state.mode_step) {
    case 0:
        audio_bank_start(1, 0);
        portrait_show_checked();
        dec_bg_show_switch(1);
        break;
    case 1:
        normal_date_bggirl_fadein();
        break;
    case 2:
        if (D_801342C4 >= 3U) {
            talk_group = tbl.v[D_801342D0] + 2;
            if (D_801342D0 == 4) {
                if (game_state.girls[4].flags.b[2]) {
                }
                if (((u16)game_state.player_flags.h >> 12) != ((u8)game_state.girls[4].flags.b[2] >> 4)) {
                    if (((u8)game_state.girls[4].flags.b[2] >> 4) == 7) {
                        talk_group += 1;
                    }
                    if (((u8)game_state.girls[4].flags.b[2] >> 4) == 6) {
                        talk_group += 2;
                    }
                }
            }
            if (D_801342D0 == 6) {
                if (((u8)game_state.girls[6].flags.b[2] >> 4) != ((u16)game_state.player_flags.h >> 12)) {
                    talk_group += 1;
                }
            }
        } else {
            talk_group = tbl.v[D_801342D0] + 3;
            if (D_801342D0 == 4) {
                talk_group += 2;
            }
            if (D_801342D0 == 6) {
                talk_group += 1;
            }
        }
        mode_step_next();
        break;
    case 3:
    case 6:
        normal_date_speak();
        break;
    case 4:
        if (D_801342C4 >= 3U) {
            if (D_801342D0 == 0xC) {
                talk_group = 0x26;
            } else {
                talk_group = 0x25;
            }
        } else {
            if (game_state.year == 0x61 && game_state.month == 0xC) {
                talk_group = 0x28;
            } else {
                talk_group = 0x27;
            }
        }
        mode_step_next();
        break;
    case 5:
        if (D_801342D0 == 0xC) {
            normal_date_girl_out();
        } else {
            mode_step_next();
        }
        break;
    case 7:
        if (D_801342D0 == 0xC) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 8:
        audio_command(0xC1);
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

void func_80133CB8(void) {
    s32 i;
    u32 temp_t8;

    switch (game_state.mode_step) {
    case 0:
        audio_command(0xBF);
        func_800674B0();
        mode_step_next();
        break;
    case 1:
        if (audio_bank_load_poll() == 1) {
            if (D_801342C4 >= 3U) {
                temp_t8 = (u16) game_state.player_flags.h >> 0xC;
                ((RensyuClubCount *)&game_state.clubs[temp_t8])->count++;
                /* FAKE: T-19020. The empty test puts a basic block boundary between the counter update and the
                 * loop setup; without it the loop pointer constant is defined in the counter's block and takes
                 * v0 from the counter address (original: both in v0). The loop sits on the line of the
                 * statement before it because as1 schedules by source line (the lui of the loop pointer then
                 * comes before the lui of the store). The real source is unknown. */
                i = 1;
                if (i) {
                }
                game_state.girls[0].tokimeki += 2; for (i = 1; i < 13; i++) {
                    game_state.girls[i].tokimeki += 2;
                }
            } else {
                game_state.param_guts.value += 0xA;
                game_state.param_stress.value += 0xA;
            }
            if (D_801342D0 != 0xFF) {
                game_state.girls[D_801342D0].affection += 2;
            }
            talk_clear();
            set_dec_bri(0);
            day_event_next(1);
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

void func_80133E5C(void) {
    switch (D_801342D0) {
    case 0:
        switch ((u32)game_state.girls[0].flags.b[2] >> 4) {
        case 6:
        case 7:
            portrait_read_start(0x54FE);
            break;
        case 8:
            portrait_read_start(0x552B);
            break;
        case 9:
            portrait_read_start(0x5558);
            break;
        case 10:
            portrait_read_start(0x5585);
            break;
        }
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
    case 10:
        portrait_read_start(0x6962);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80133F60(void) {
    D_801343D0 = 0x801A42D4;
    D_801343D4 = 0x801A42E0;
    D_801343D8 = 0x801A4330;
    D_801343DC = *(s16 *)0x801A4338;
    D_801343E0 = 0x80197000;
}

void func_80133FB4(void) {
    D_801343D0 = 0x801A3E3C;
    D_801343D4 = 0x801A3E48;
    D_801343D8 = 0x801A3E90;
    D_801343DC = *(s16 *)0x801A3E98;
    D_801343E0 = 0x80197000;
}

void func_80134008(void) {
    D_801343D0 = 0x801A42E4;
    D_801343D4 = 0x801A42F0;
    D_801343D8 = 0x801A4368;
    D_801343DC = *(s16 *)0x801A4370;
    D_801343E0 = 0x80197000;
}

void func_8013405C(void) {
    D_801343D0 = 0x801A432C;
    D_801343D4 = 0x801A4338;
    D_801343D8 = 0x801A4394;
    D_801343DC = *(s16 *)0x801A43A4;
    D_801343E0 = 0x80197000;
}

void func_801340B0(void) {
    D_801343D0 = 0x801A403C;
    D_801343D4 = 0x801A4048;
    D_801343D8 = 0x801A4098;
    D_801343DC = *(s16 *)0x801A40B0;
    D_801343E0 = 0x80197000;
}


