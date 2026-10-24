/* sightseeing_and_companion_selection.c: src/ovl/SHUGAKU/80138A60.c:9-49,144-177,187-221; sightseeing callbacks/site selection, same-club/girl choice gates, eligible-companion ranking and Ayako detour routing. Original object 0x80138A60-0x80139D00 (config/objects/SHUGAKU.txt). */
#define MAIN_API_OVERRIDE_get_h_tokimeki /* u8 return narrows the metric byte on return (T-24122). */
#define MAIN_API_OVERRIDE_get_h_yuukou   /* u8 return narrows the metric byte on return (T-24122). */
#include "common.h"
#include "ovl/SHUGAKU.h"

u8 get_h_tokimeki(s32 arg0);
u8 get_h_yuukou(s32 arg0);

void func_80138A60(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    ShFnTbl16 tbl;

    tbl = D_8013C930;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void func_80138ADC(void) {
    k_disp_start(set_kanji_string(-0x80, 0x30, 2, "【団体行動中】", 0));
    mode_step_next();
}

void func_80138B20(void) {
    /* FAKE: the wrapper reserves the observed extra frame extent. T-10020. */
    ShugakuCallbackLocal12C local;
    u8 state;

    local.callbacks = D_8013C984;
    /* FAKE: the dead branch preserves the later func_80138DD8 comparison
     * operand order in this compilation unit; it emits no code. T-10020. */
    if (1) {}
    if (D_800CA2CC == 1) {
        state = game_state.mode_step;
        if (state >= 0x1FU && state < 0x25U) {
            sprite_light_effect_draw();
        }
    }
    state = game_state.mode_step;
    local.callbacks.callback[state](0x80);
}

void func_80138BD8(void) {
    func_801386C4();
    if (game_state.current_girl == GIRL_SAKI && ((u32)game_state.girls[4].flags.b[2] >> 4) == ((u32)game_state.player_flags.h >> 0xC)) {
        *(u8 *)(*(s32 *)(D_800CA2D4 + D_800CA2DC * 4) + 3) = 0xFF;
    }
}

void func_80138C48(void) {
    if (game_state.substep == 0) {
        if (D_8013C97C == 4 && ((u32)game_state.girls[4].flags.b[2] >> 4) == ((u32)game_state.player_flags.h >> 0xC)) {
            D_800CA2DC = 0x2E;
        }
        game_state.substep = 1;
    }
    func_801386C4();
}

void func_80138CB8(void) {
    u8 **p;

    p = (u8 **)D_800CA2D4;
    if (p[D_800CA2DC][D_800CA2E0] == 9) {
        mode_step_next();
        return;
    }
    normal_date_girl_out();
}

void func_80138D24(void) {
    if (D_8013C974 == 1) {
        mode_step_next();
        return;
    }
    normal_date_girl_out();
}

void func_80138D64(void) {
    u8 **p;

    p = (u8 **)D_800CA2D4;
    if (p[D_800CA2DC][D_800CA2E0] == 9) {
        mode_step_next();
        D_8013C974 = 1;
        return;
    }
    normal_date_girl_in();
}

void func_80138DD8(void) {
    u8 value;

    func_801386C4();
    if (game_state.current_girl == GIRL_MIO && D_800CA2E0 == 3) {
        value = D_800CA2CC;
        if (value == 1) {
            D_80122CF4 = 0;
            return;
        }
    }
    D_80122CF4 = 1;
}

s32 func_80138E40(void) {
    /* FAKE: `pad` reserves the top stack word; `(game_state.substep & 0) +` places an entry load of unk_110D before the first branch so cvt_pass strips its widenings (T-24114). */
    s32 pad;
    s32 saved_phase;
    s32 step;

    if (((game_state.substep & 0) + game_state.current_girl != 4) || (D_8013C980 != 0x36)) {
        normal_date_bg_fadeout();
        return;
    }
    saved_phase = (s32) game_state.mode_step;
    if ((game_state.mode_step_timer.w == 0) && (game_state.substep == 0)) {
        D_800CA2DC = 0x10;
    }
    if (game_state.substep == 0) {
        game_state.mode_step_timer.w += 1;
    }
    step = (s32) game_state.substep;
    if (game_state.substep == 0) {
        func_801386C4();
    } else if (game_state.substep == 1) {
        normal_date_girl_in();
    } else if (game_state.substep == 2) {
        func_801386C4();
    } else if (game_state.substep == 3) {
        normal_date_girl_out();
    } else {
        normal_date_bg_fadeout();
    }
    if (saved_phase != game_state.mode_step) {
        if (step < 4) {
            game_state.substep = step + 1;
            game_state.mode_step = saved_phase;
            return;
        }
        D_800CA2DC = D_8013C970;
        game_state.substep = step + 1;
    }
}

void func_80138FAC(void) {
    if (game_state.current_girl != GIRL_AYAKO || D_800CA2CC != 1) {
        normal_date_girl_out();
        return;
    }
    game_state.saved_mode_stage = game_state.mode_stage + 1;
    game_state.saved_mode_step = 0;
    mode_stage_set(9);
}

INCLUDE_RODATA("asm/ovl/SHUGAKU/data/SHUGAKU/sightseeing_and_companion_selection.rodata", D_8013AEF0);

void func_80139018(void) {
    bg_read_sub2(0x4225);
    strcpy(D_800CA19C, &D_8013AEF0);
    mode_step_next();
}

void func_80139054(void) {
    u8 n;

    n = D_800CA2CC;
    switch (n) {
    case 0:
        bg_read_sub2(0x425E);
        strcpy(D_800CA19C, "社寺巡り");
        break;
    case 1:
        bg_read_sub2(0x424B);
        strcpy(D_800CA19C, "守礼門");
        break;
    default:
        bg_read_sub2(0x4271);
        strcpy(D_800CA19C, "時計台前");
        break;
    }
    if (D_800CA2EC == 0) {
        n = D_800CA2CC; /* reload into the switch variable: its reuse gives the original's a1 selector (T-18000) */
        event_log_add(3, n);
    }
    mode_step_next();
}

void func_80139118(void) {
    s16 i;
    u8 best_i;
    u8 best_s;
    u8 s;

    D_8013C978 = 0;
    best_i = 0xFF;
    best_s = 4;
    D_8013C980 = 0;
    for (i = 0; i < 10; i++) {
        /* FAKE: dead branch adjusts the loop basic-block count so uopt assigns best_s to $s6 (T-24122). */
        if (1) {}
        if (D_800CA2B0[i] != 0) {
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
        D_8013C980 = best_i + 0x32;
        /* FAKE: `(best_i & 0) +` keeps cfe's CVT J L narrowing so uopt emits `andi $a0, $s7, 0xff` (T-24122). */
        game_state.current_girl = (best_i & 0) + best_i;
        talk_names_set(game_state.current_girl);
        func_801399F4();
    } else {
        D_800CA2DC += 4;
        game_state.mode_step += 0x1E;
        game_state.current_girl = 0xFF;
    }
    mode_step_next();
}

s32 func_801392B4(void) {
    /* FAKE: `six` reserves the top stack word and `(six & 0) + six` promotes the constant 6 to $v0;
     * `*(s16 *)&D_800CA2DC` keeps the first store out of the second block's scalar web (T-24104). */
    s32 six;
    s32 previous_phase;

    if (D_80122CDC == 0) {
        if (game_state.current_girl == GIRL_YUKO) {
            previous_phase = game_state.mode_step;
            six = 6;
            *(s16 *)&D_800CA2DC = (six & 0) + six;
            func_801386C4();
            if (previous_phase == game_state.mode_step) {
                return 0;
            }
            game_state.mode_step--;
        }
        D_8013C970 = D_800CA2DC = D_800CA2CC + 0x1A;
        mode_step_next();
        mode_step_next();
        mode_step_next();
    } else {
        game_state.girls[game_state.current_girl].tokimeki -= 1;
        game_state.girls[game_state.current_girl].affection -= 2;
        check_para_limit();
    }
    mode_step_next();
}

void func_801393C4(void) {
    D_800CA2DC = (D_800CA2DC - D_800CA2CC) + 2;
    mode_step_next();
}

void func_80139400(void) {
    /* FAKE: the local selector preserves v0 instead of v1. T-10020. */
    s32 selection;
    D_8013C974 = 0;
    selection = D_800CA2CC;
    switch (selection) {
    case 0:
        strcpy(D_800CA23C, "京都");
        break;
    case 1:
        strcpy(D_800CA23C, "那覇");
        break;
    case 2:
        strcpy(D_800CA23C, "札幌");
        break;
    }
    mode_step_next();
}

void func_80139498(void) {
    if (D_80122CDC != 0) {
        if (game_state.current_girl == GIRL_YUINA || game_state.current_girl == GIRL_MIRA || game_state.current_girl == GIRL_MEGUMI) {
            func_80083418();
            return;
        }
        func_800833F0();
        return;
    }
    mode_step_next();
}

void func_80139508(void) {
    if (D_80122CDC != 0) {
        game_state.mode_step += 0x11;
    }
    mode_step_next();
}

void func_80139548(void) {
    if (D_80122CDC != 0) {
        D_800CA2DC = 0;
        D_800CA2D0 = D_8013C450;
        D_800CA2D4 = D_8013C484;
        D_800CA2D8 = D_8013C4B8;
        D_800CA2EC = 1;
        game_state.current_girl = GIRL_YOSHIO;
        talk_names_set(0xDU);
        game_state.mode_step = 0x33;
        return;
    }
    mode_step_next();
}

void func_801395DC(void) {
    D_800CA2DC = 0xE;
    mode_step_next();
}

void func_80139604(void) {
    s32 var_v0;
    u8 x;
    u32 t;

    if (D_800CA2EC == 0) {
        x = get_g_zyotai_s(game_state.current_girl);
        t = x & 0x7F;
        if (t < 2U) {
            var_v0 = 0;
        } else if (t == 2) {
            var_v0 = 3;
        } else if (t == 3) {
            var_v0 = 6;
        } else {
            var_v0 = 9;
        }
        D_800CA2DC += var_v0;
    }
    mode_step_next();
}

void func_80139698(void) {
    D_800CA2DC = D_800CA2CC + 0x1A;
    if (D_800CA2EC != 0) {
        D_800CA2DC = D_800CA2CC + 3;
    }
    game_state.mode_step -= 0x2E;
    mode_step_next();
}

void func_801396F4(void) {
    if (D_800CA2EC != 0) {
        portrait_read_start(0x6C32);
    } else {
        mode_step_next();
        mode_step_next();
    }
    mode_step_next();
}

s32 func_80139740(void) {
    s32 pad[2]; /* FAKE: unused locals, frame slots only (T-10310) */
    s32 state;
    u8 t; /* FAKE: narrow type shifts the temporary numbering to the original's (permuter, T-10310) */

    t = get_g_zyotai_s(game_state.current_girl);
    D_8013C97C = 0xFF;
    if ((t & 0x7F) == 4 && D_800CA2EC == 0) {
        D_8013C97C = game_state.current_girl;
        if (game_state.current_girl == GIRL_MIO) {
            state = game_state.mode_step;
            normal_date_girl_out();
            if (state == game_state.mode_step) {
                return 0;
            }
        }
        D_800CA2EC = 1;
        game_state.current_girl = GIRL_YOSHIO;
        talk_names_set(0xD);
        get_g_name(girl_name_buf, D_8013C97C);
        mode_step_next();
    } else {
        mode_step_next();
        mode_step_next();
        mode_step_next();
        mode_step_next();
        if (D_800CA2EC != 0) {
            mode_step_next();
            mode_step_next();
            mode_step_next();
            mode_step_next();
        }
    }
}

void func_80139870(void) {
    D_800CA2DC = 0;
    D_800CA2D0 = D_8013C450;
    D_800CA2D4 = D_8013C484;
    D_800CA2D8 = D_8013C4B8;
    game_state.mode_step = 0x33;
}

void func_801398B8(void) {
    get_g_zyotai_s(game_state.current_girl);
    mode_step_next();
}

s32 func_801398E8(void) {
    s32 selection;
    u8 value;

    selection = D_80122CDC;
    value = game_state.current_girl;
    if (value == selection) {
        value = game_state.mode_step;
        func_801386C4();
        if (value != game_state.mode_step) {
            D_800CA2DC -= 2;
            game_state.mode_step -= 3;
        }
        return 0;
    }
    value = selection;
    /* FAKE: second read of unk_F5F across the entry branch causes uopt to promote unk_F5F to $a1 while folding the unsigned >= 0 guard (T-24114). */
    if (game_state.current_girl >= 0) {
        if (selection == 0xB) {
            D_800CA2DC = 0;
            D_800CA2D0 = D_8013C450;
            D_800CA2D4 = D_8013C484;
            D_800CA2D8 = D_8013C4B8;
            D_800CA2EC = 1;
            value = 0xD;
            game_state.current_girl = value;
            talk_names_set(value);
            game_state.mode_step = 0x33;
            return 0;
        }
    }
    game_state.current_girl = value;
    talk_names_set(value);
    func_801399F4();
    mode_step_next();
}

void func_801399F4(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        D_800CA2D0 = D_8013C424;
        D_800CA2D4 = D_8013C458;
        D_800CA2D8 = D_8013C48C;
        break;
    case GIRL_MIO:
        D_800CA2D0 = D_8013C434;
        D_800CA2D4 = D_8013C468;
        D_800CA2D8 = D_8013C49C;
        break;
    case GIRL_YUINA:
        D_800CA2D0 = D_8013C428;
        D_800CA2D4 = D_8013C45C;
        D_800CA2D8 = D_8013C490;
        break;
    case GIRL_AYAKO:
        D_800CA2D0 = D_8013C42C;
        D_800CA2D4 = D_8013C460;
        D_800CA2D8 = D_8013C494;
        break;
    case GIRL_SAKI:
        D_800CA2D0 = D_8013C448;
        D_800CA2D4 = D_8013C47C;
        D_800CA2D8 = D_8013C4B0;
        break;
    case GIRL_YUKARI:
        D_800CA2D0 = D_8013C438;
        D_800CA2D4 = D_8013C46C;
        D_800CA2D8 = D_8013C4A0;
        break;
    case GIRL_NOZOMI:
        D_800CA2D0 = D_8013C440;
        D_800CA2D4 = D_8013C474;
        D_800CA2D8 = D_8013C4A8;
        if ((u8)(get_g_zyotai_s(GIRL_NOZOMI) & 0x7F) < 2) {
            D_800CA2D0 = D_8013C43C;
            D_800CA2D4 = D_8013C470;
            D_800CA2D8 = D_8013C4A4;
        }
        break;
    case GIRL_MIRA:
        D_800CA2D0 = D_8013C430;
        D_800CA2D4 = D_8013C464;
        D_800CA2D8 = D_8013C498;
        break;
    case GIRL_YUKO:
        D_800CA2D0 = D_8013C420;
        D_800CA2D4 = D_8013C454;
        D_800CA2D8 = D_8013C488;
        break;
    case GIRL_MEGUMI:
        D_800CA2D0 = D_8013C444;
        D_800CA2D4 = D_8013C478;
        D_800CA2D8 = D_8013C4AC;
        break;
    }
}

void func_80139C80(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    ShFnTbl18 tbl;

    tbl = D_8013CAA0;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}
