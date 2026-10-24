/* birthday_detours_and_talk.c: src/ovl/SHUGAKU/80132000.c:16-65,299-355; binds per-girl birthday tables, dispatches birthday callbacks, handles saved-stage returns and choice effects. Shared birthday routing/ranking is in func_8013243C; no location-table selection claim. Original object 0x80132000-0x80133430 (config/objects/SHUGAKU.txt). */
#define MAIN_API_OVERRIDE_portrait_layout_set /* matched with void(s32) (main_api.h: void(u8)) */
#define MAIN_API_OVERRIDE_get_h_tokimeki /* u8 return narrows the metric byte on return (T-24122). */
#define MAIN_API_OVERRIDE_get_h_yuukou   /* u8 return narrows the metric byte on return (T-24122). */
#include "common.h"
#include "ovl/SHUGAKU.h"

void portrait_layout_set(s32 arg0);
u8 get_h_tokimeki(s32 arg0);
u8 get_h_yuukou(s32 arg0);

typedef struct {
    u8 s[0xCC3];
} DateTxt; /* size 0xCC3 */
extern DateTxt D_8013B1AC;

void func_80132000(void) {
    D_8013AFD0 = 0x801B0478;
    D_8013AFD4 = 0x801B13A4;
    D_8013AFD8 = 0x801B2998;
    D_8013AFDC = 0x801B38C8;
    D_8013AFE0 = 0x801B4258;
    D_8013AFE4 = 0x801B4B98;
    D_8013AFE8 = 0x801B5574;
    D_8013AFEC = 0x801B6560;
    D_8013AFF0 = 0x801B7B74;
    D_8013AFF4 = 0x801B8B18;
    D_8013AFF8 = 0x801B93D0;
    D_8013AFFC = 0x801B9DA0;
    D_8013B000 = 0x801BA780;
    D_8013B004 = 0x801B0510;
    D_8013B008 = 0x801B14E0;
    D_8013B00C = 0x801B2AD0;
    D_8013B010 = 0x801B3960;
    D_8013B014 = 0x801B42F0;
    D_8013B018 = 0x801B4C38;
    D_8013B01C = 0x801B560C;
    D_8013B020 = 0x801B66A0;
    D_8013B024 = 0x801B7CB4;
    D_8013B028 = 0x801B8BB0;
    D_8013B02C = 0x801B9468;
    D_8013B030 = 0x801B9EA8;
    D_8013B034 = 0x801BA818;
    D_8013B038 = 0x801B0888;
    D_8013B03C = 0x801B1E14;
    D_8013B040 = 0x801B33B0;
    D_8013B044 = 0x801B3CF4;
    D_8013B048 = 0x801B4684;
    D_8013B04C = 0x801B4FEC;
    D_8013B050 = 0x801B59F4;
    D_8013B054 = 0x801B6FB8;
    D_8013B058 = 0x801B85CC;
    D_8013B05C = 0x801B8F28;
    D_8013B060 = 0x801B97E0;
    D_8013B064 = 0x801BA2C8;
    D_8013B068 = 0x801BAB90;
}

void func_80132274(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    ShFnTbl70 tbl;

    tbl = D_8013B094;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void func_801322F0(void) {
    GameState *state;
    s32 previous;

    state = &game_state;
    previous = (s32) game_state.mode_step;
    func_80139018();
    if (((state->mode_step != (previous + 3)) || ((u8) D_800CA2F0 != 0)) && (state->mode_step == (previous + 4)) && ((u8) D_800CA2F0 == 0)) {
        state->mode_step -= 1;
        dec_bg_show_switch(1);
    }
}

void func_80132364(void) {
    s32 pad; /* FAKE: unused local reserves a stack word (frame home). T-11170 */
    s32 sp28;

    if (game_state.substep == 0) {
        game_state.substep += 1;
        func_80136380();
        D_8013B06C = D_8013C424;
        D_8013B070 = D_8013C458;
        D_8013B074 = D_8013C48C;
        D_8013B084 = 0xE;
        D_8013B088 = 0;
    }
    if (game_state.substep == 1) {
        sp28 = (s32) game_state.mode_step;
        func_80132F44();
        if (sp28 != game_state.mode_step) {
            func_8013282C();
            game_state.mode_step -= 1;
            D_8013B084 = 5;
        }
    }
}

s32 func_8013243C(void) {
    s16 i;
    u8 best_i;
    u8 best_s;
    u8 s;

    best_i = 0xFF;
    best_s = 4;
    if (game_state.month != ((WordView *)&game_state.player_birthday)->date.month ||
        game_state.day != ((WordView *)&game_state.player_birthday)->date.day) {
        mode_step_next();
        return 0;
    }
    for (i = 0; i < 10; i++) {
        if (D_800CA2B0[i] != 0) {
            s = get_g_zyotai_s(i) & 0x7F;
            /* FAKE: duplicate `s == best_s` shifts uopt's live-range priority between s and best_s (T-24122). */
            if (s < best_s ||
                (s == best_s && s == best_s &&
                 get_h_tokimeki(i) + get_h_yuukou(i) >
                     get_h_tokimeki(best_i) + get_h_yuukou(best_i))) {
                best_i = i;
                best_s = s;
            }
        }
    }
    s = get_g_zyotai_s(best_i) & 0x7F;
    if (s < 2U && best_i == 0) {
        mode_step_set(0x32);
        return 0;
    }
    mode_step_next();
}

void func_801325C0(void) {
    audio_bank_load_start(0, 0xBF98, 0xBF79, 0xCA95, 0xCA4F, 0xCA3E);
    if (audio_bank_load_poll() == 1) {
        if (game_state.mode_stage == 6) {
            audio_command(0x200);
        }
        mode_step_next();
    }
    mode_step_next();
}

void func_80132638(void) {
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

void func_801326C0(void) {
    cd_read_sectors(0x16, 0x801B0000, 0xAF0D);
    func_80132000();
    mode_step_next();
}

void func_801326F8(void) {
    func_80133430();
    D_8013B06C = D_8013BE74;
    D_8013B070 = D_8013BEA8;
    D_8013B074 = D_8013BEDC;
    D_8013B084 = 1;
    D_8013B088 = 0;
    mode_step_next();
}

void func_80132760(void) {
    if (game_state.saved_mode_stage != 6) {
        event_log_add(0xA, 0x29);
    } else {
        switch (D_800CA2CC) {                       /* irregular */
        case 0:
            event_log_add(0xA, 0x2A);
            break;
        case 1:
            event_log_add(0xA, 0x2B);
            break;
        default:
            event_log_add(0xA, 0x2C);
            break;
        }
    }
    func_80132000();
    func_8013282C();
    if ((dec_bg_brightness != 0) && (D_80122CF8 != 0)) {
        D_800CA2F0 = 1;
        return;
    }
    D_800CA2F0 = 0;
}

void func_8013282C(void) {
    D_8013B084 = 0;
    D_8013B088 = 0;
    talk_names_set(game_state.current_girl);
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        D_8013B06C = D_8013AFD4;
        D_8013B070 = D_8013B008;
        D_8013B074 = D_8013B03C;
        break;
    case GIRL_MIO:
        D_8013B06C = D_8013AFE4;
        D_8013B070 = D_8013B018;
        D_8013B074 = D_8013B04C;
        break;
    case GIRL_YUINA:
        D_8013B06C = D_8013AFD8;
        D_8013B070 = D_8013B00C;
        D_8013B074 = D_8013B040;
        break;
    case GIRL_AYAKO:
        D_8013B06C = D_8013AFDC;
        D_8013B070 = D_8013B010;
        D_8013B074 = D_8013B044;
        break;
    case GIRL_SAKI:
        D_8013B06C = D_8013AFF8;
        D_8013B070 = D_8013B02C;
        D_8013B074 = D_8013B060;
        break;
    case GIRL_YUKARI:
        D_8013B06C = D_8013AFE8;
        D_8013B070 = D_8013B01C;
        D_8013B074 = D_8013B050;
        break;
    case GIRL_NOZOMI:
        D_8013B06C = D_8013AFF0;
        D_8013B070 = D_8013B024;
        D_8013B074 = D_8013B058;
        if ((u8)(get_g_zyotai_s(GIRL_NOZOMI) & 0x7F) < 2) {
            D_8013B06C = D_8013AFEC;
            D_8013B070 = D_8013B020;
            D_8013B074 = D_8013B054;
        }
        break;
    case GIRL_MIRA:
        D_8013B06C = D_8013AFE0;
        D_8013B070 = D_8013B014;
        D_8013B074 = D_8013B048;
        break;
    case GIRL_YUKO:
        D_8013B06C = D_8013AFD0;
        D_8013B070 = D_8013B004;
        D_8013B074 = D_8013B038;
        break;
    case GIRL_MEGUMI:
        D_8013B06C = D_8013AFF4;
        D_8013B070 = D_8013B028;
        D_8013B074 = D_8013B05C;
        break;
    case GIRL_YUMI:
        D_8013B06C = D_8013B000;
        D_8013B070 = D_8013B034;
        D_8013B074 = D_8013B068;
        break;
    }
    if (game_state.month >= 4) {
        D_8013B090 = game_state.year - 0x5F;
    } else {
        D_8013B090 = game_state.year - 0x60;
    }
    mode_step_next();
}

void func_80132B50(void) {
    if ((u8) D_800CA2F0 == 1) {
        normal_date_move_place();
        return;
    }
    if ((u8) D_800CA2F0 == 2) {
        mode_step_next();
        return;
    }
    if (game_state.substep++ == 0) {
        dec_bg_show_switch(1);
    }
    func_8007C8A4();
}

void func_80132BCC(void) {
    if (game_state.saved_mode_stage != 6) {
        /* FAKE: the comma expression preserves the original argument register.
         * Original spelling unknown; found by permute.py. T-9530. */
        mode_stage_set((0, (s32) game_state.saved_mode_stage));
        mode_step_set((s32) game_state.saved_mode_step);
        return;
    }
    func_8013ABAC(game_state.saved_mode_stage);
}

void func_80132C1C(void) {
    if (game_state.saved_mode_stage == 6) {
        game_state.mode_step += 6;
        return;
    }
    mode_step_next();
}

void func_80132C64(void) {
    if (D_80122CDC != 0) {
        game_state.girls[game_state.current_girl].affection -= 1;
        game_state.girls[game_state.current_girl].syoushin += 5;
        check_para_limit();
        game_state.mode_step += 0x1B;
        return;
    }
    D_8013B084 += 1;
    mode_step_next();
}

void func_80132D14(void) {
    u32 t;

    t = get_g_zyotai_s(game_state.current_girl);
    t = t & 0x7F;
    t = t & 0x7F; /* FAKE: the repeated mask reproduces the original's extra move; real source unknown. T-4070 */
    if (t >= 2U) {
        if (t == 2) {
            D_8013B084 += 2;
        } else if (t == 3) {
            D_8013B084 += 4;
        } else {
            D_8013B084 += 6;
        }
    }
    mode_step_next();
}

void func_80132DAC(void) {
    game_state.current_girl = game_state.date_girl;
    talk_names_set(game_state.date_girl);
    mode_step_next();
}

void func_80132DE0(void) {
    if (game_state.saved_mode_stage != 6) {
        func_801386C4();
        return;
    }
    mode_step_next();
}

void func_80132E20(void) {
    D_8013B084 = D_8013B08C;
    D_8013B06C = D_8013B078;
    D_8013B070 = D_8013B07C;
    D_8013B074 = D_8013B080;
    func_80132E84();
    mode_step_next();
}

void func_80132E84(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    DateTxt tbl;

    tbl = D_8013B1AC;
    strcpy(D_800CA25C, &tbl.s[game_state.current_girl * 0x129 + D_8013B090 * 0x63 + D_80122CDC * 0x21]);
}

void func_80132F44(void) {
    talk_tables = &D_8013B084;
    D_800CA138 = &D_8013B088;
    D_800CA13C = D_8013B06C;
    D_800CA140 = D_8013B070;
    D_800CA144 = D_8013B074;
    talk_run(D_80122CDC, 1, 0);
}

void func_80132FC0(void) {
    talk_tables = &D_8013B084;
    D_800CA138 = &D_8013B088;
    D_800CA13C = D_8013B06C;
    D_800CA140 = D_8013B070;
    D_800CA144 = D_8013B074;
    talk_run(0xFF, 1, 0);
}

void func_80133038(void) {
    D_8013B08C = D_8013B084;
    D_8013B078 = D_8013B06C;
    D_8013B07C = D_8013B070;
    D_8013B080 = D_8013B074;
    D_8013B084 = game_state.current_girl;
    /* (s16) leaves the ugen temporary between sll and subu unused, as in the original; the product sits before the last three copies so their constant loads take t5/t6/t7. T-24017 */
    D_8013B084 = (s16)(D_8013B084 * 3) + D_8013B090;
    D_8013B06C = D_8013AFFC;
    D_8013B070 = D_8013B030;
    D_8013B074 = D_8013B064;
    mode_step_next();
}


void func_801330E8(void) {
    portrait_layout_set(D_80122CDC + 1);
    if (D_80122CDC == 0) {
        game_state.girls[game_state.current_girl].tokimeki += 3;
        game_state.girls[game_state.current_girl].affection += 2;
        game_state.girls[game_state.current_girl].syoushin -= 0xF;
    } else if (D_80122CDC == 1) {
        game_state.girls[game_state.current_girl].tokimeki += 2;
        game_state.girls[game_state.current_girl].affection += 1;
        game_state.girls[game_state.current_girl].syoushin -= 0xA;
    } else {
        game_state.girls[game_state.current_girl].affection += 1;
        game_state.girls[game_state.current_girl].syoushin += 0xA;
    }
    check_para_limit();
    mode_step_next();
}

void func_80133298(void) {
    D_8013B084 = (D_80122CDC * 2) + 0xD;
    D_800CA2F0 = 2;
    mode_step_next();
}

void func_801332D8(void) {
    get_g_zyotai_s(game_state.date_girl);
    if (game_state.month == (game_state.player_birthday & 0xF) && game_state.day == ((u32)(game_state.player_birthday << 0x17) >> 0x1B)) {
        mode_stage_set(7);
        return;
    }
    mode_step_next();
}

void func_80133348(void) {
    u16 v;
    u8 c; /* FAKE: copy of D_800CA2CC, gives the original's bne operand order; real source unknown. T-11440 */

    v = get_g_zyotai_s(game_state.date_girl);
    if (game_state.saved_mode_stage == 5 && game_state.date_girl == GIRL_YUINA && (c = D_800CA2CC) == 2) {
        dec_bg_brightness = 0;
        mode_stage_set(8);
        return;
    }
    /* FAKE: the byte view of unk_75D below gives the original's bne operand order; real source unknown. T-11440 */
    if (game_state.saved_mode_stage == 3 && *(u8 *) &game_state.date_girl == 5 && (v & 0x7F) < 2) {
        mode_stage_set(0xB);
        return;
    }
    if (game_state.saved_mode_stage == 4 && *(u8 *) &game_state.date_girl == 4 && (v & 0x7F) < 2) {
        mode_stage_set(0xA);
        return;
    }
    mode_step_next();
}
