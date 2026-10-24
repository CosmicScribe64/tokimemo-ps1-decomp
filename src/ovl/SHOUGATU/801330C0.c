#define MAIN_API_OVERRIDE_talk_names_set /* Word argument at this call (func_80133760); T-17500. */
#include "common.h"
#include "ovl/SHOUGATU.h"
void talk_names_set();

void func_801330C0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl60 tbl;

    tbl = D_80143BC4;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void func_80133134(void) {
    audio_command(0x603);
    mode_step_next();
}

s32 func_8013315C(void) {
    func_801323D4();
    if (game_state.mode_step_timer.w++ == 0) {
        if (((u8 **)D_80143AF8)[D_80143B00][D_80143B04 - 1] == 7) {
            if (strcmp(((s32 **)D_80143AF4)[D_80143B00][D_80143B04 - 1], "ガチャ") == 0) {
                audio_command(0x60C);
            }
        }
    }
}

s32 func_80133208(void) {
    s32 sound_id;

    if ((game_state.pad_pressed & 0x400040) &&
        D_80143B04 > 0 && D_80143B04 < 5 &&
        (u32)game_state.mode_step_timer.w >= 2U) {
        audio_command(0x1508);
        D_80122D00 = 0;
        k_reset(1);
        D_800CA2AC = 0;
        D_80143B04 = 4;
    } else {
        if ((check_end_k() != 0) && (D_80143B04 < 5)) {
            if (D_80143B04 == 0) {
                if (D_80143BC0 < 0x5A) {
                    D_80143BC0 += 1;
                    if (D_80143BC0 == 1) {
                        audio_command(0x60A);
                    }
                    if (D_80143BC0 == 0x1E) {
                        audio_command(0x604);
                    }
                    return 0;
                }
                if (!(game_state.substep & 7)) {
                    sound_id = (((girl_phone_digits[game_state.current_girl][game_state.substep >> 3] + 0x505) & 0xFF) | 0x600) & 0xFFFF;
                    /* FAKE: retain the original unchanged-byte store before the call.
                     * Direct self-assignment is removed; original spelling unknown. T-24052. */
                    game_state.substep |= 0;
                    audio_command(sound_id);
                }
            }
            if (game_state.substep++ < 0x28U) {
                return 0;
            }
            game_state.substep = 0;
            k_reset(1);
        }
    }
    func_801323D4();
    if ((D_80143B04 < 5) && (game_state.mode_step_timer.w == 1)) {
        audio_command(0x608);
    }
    game_state.mode_step_timer.w += 1;
}

s32 func_801333F0(void) {
    s32 sound_id;

    if ((game_state.pad_pressed & 0x400040) &&
        D_80143B04 > 0 && D_80143B04 < 3 &&
        (u32)game_state.mode_step_timer.w >= 2U) {
        audio_command(0x1508);
        D_80122D00 = 0;
        k_reset(1);
        D_800CA2AC = 0;
        D_80143B04 = 2;
    } else {
        if ((check_end_k() != 0) && (D_80143B04 < 3)) {
            if (D_80143B04 == 0) {
                if (D_80143BC0 < 0x5A) {
                    D_80143BC0 += 1;
                    if (D_80143BC0 == 1) {
                        audio_command(0x60A);
                    }
                    if (D_80143BC0 == 0x1E) {
                        audio_command(0x604);
                    }
                    return 0;
                }
                if (!(game_state.substep & 7)) {
                    sound_id = (((girl_phone_digits[game_state.current_girl][game_state.substep >> 3] + 0x505) & 0xFF) | 0x600) & 0xFFFF;
                    /* FAKE: retain the original unchanged-byte store before the call.
                     * Direct self-assignment is removed; original spelling unknown. T-24052. */
                    game_state.substep |= 0;
                    audio_command(sound_id);
                }
            }
            if (game_state.substep++ < 0x28U) {
                return 0;
            }
            game_state.substep = 0;
            k_reset(1);
        }
    }
    func_801323D4();
    if ((D_80143B04 < 3) && (game_state.mode_step_timer.w == 1)) {
        /* FAKE: preserve the original unchanged halfword store. T-24052. */
        D_80143B04 = D_80143B04;
        audio_command(0x608);
    } else {
        /* FAKE: preserve the original unchanged halfword store. T-24052. */
        D_80143B04 = D_80143B04;
        if ((D_80143B04 == 3) && (game_state.mode_step_timer.w == 1)) {
            audio_command(0x60B);
        }
    }
    game_state.mode_step_timer.w += 1;
}

void func_80133610(void) {
    strcpy(D_800CA1DC, "正月");
    D_80143BC0 = 0;
    D_80143B20 = 0;
    D_80143B18 = 1;
    D_80143B00 = 6;
    D_80143AF4 = D_80143A48;
    D_80143AF8 = D_80143A84;
    D_80143AFC = D_80143AC0;
    mode_step_next();
}

void func_80133698(void) {
    if (D_80143B14 == 0) {
        bg_read_sub2(0x4167);
        strcpy(D_800CA19C, "正月");
        k_reset(0);
        message_window_set_title(D_800CA19C);
    } else {
        game_state.mode_step += 3;
    }
    mode_step_next();
}

void func_80133710(void) {
    bg_read_sub2(0x4167);
    mode_step_next();
}

void func_80133738(void) {
    bg_read_sub2(0x4144);
    mode_step_next();
}

s32 func_80133760(void) {
    s32 girl;
    u8 previous;

    if (D_80122CDC == 0xB) {
        func_8013397C();
        return 0;
    }
    girl = D_80122CDC;
    previous = game_state.current_girl;
    if (previous == girl && D_80143B14 != 0) {
        mode_step_next();
        return 0;
    }
    if (game_state.girls[D_80122CDC].flags.f.b3 == 0) {
        D_80143B00 = 0;
        D_80143AF4 = D_80143A78;
        D_80143AF8 = D_80143AB4;
        D_80143AFC = D_80143AF0;
        talk_names_set(D_80122CDC);
        mode_step_next();
        return 0;
    }
    game_state.current_girl = (u8)girl;
    func_80132A34();
    talk_names_set(game_state.current_girl);
    D_80143B00 = 9;
    mode_step_next();
    mode_step_next();
    mode_step_next();
}

void func_80133874(void) {
    D_80143B00 = 7;
    D_80143AF4 = D_80143A48;
    D_80143AF8 = D_80143A84;
    D_80143AFC = D_80143AC0;
    mode_step_next();
    game_state.mode_step -= 5;
}

void func_801338DC(void) {
    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    if (((u8)get_g_zyotai_s(game_state.current_girl) & 0x7F) != 4) {
        D_80143B00 += 1;
        mode_step_next();
        mode_step_next();
        mode_step_next();
        return;
    }
    mode_step_next();
}

void func_80133948(void) {
    if (D_80122CDC == 0) {
        mode_step_next();
    }
    mode_step_next();
}

void func_8013397C(void) {
    D_80143B20 = 1;
    D_80143B00 = 0;
    D_80143AF4 = D_80143A40;
    D_80143AF8 = D_80143A7C;
    D_80143AFC = D_80143AB8;
    mode_stage_next();
}

void func_801339DC(void) {
    s32 temp_t6;

    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    temp_t6 = (u8)get_g_zyotai_s(game_state.current_girl) & 0x7F;
    switch (temp_t6) {
    case 0:
        break;
    case 1:
    case 2:
        D_80143B00 += 1;
        break;
    case 3:
        D_80143B00 += 2;
        break;
    }
    mode_step_next();
}

void func_80133A60(void) {
    D_80143B00 = 0x27;
    mode_step_next();
}

void func_80133A88(void) {
    if (D_80122CDC != 0) {
        D_80143B00 += 1;
        game_state.girls[0].tokimeki -= 1;
        game_state.girls[0].affection -= 1;
        game_state.girls[0].syoushin += 0xA;
        D_80143B24 = 0;
        mode_step_next();
        mode_step_next();
        mode_step_next();
    } else {
        game_state.girls[0].tokimeki += 1;
    }
    check_para_limit();
    mode_step_next();
}

void func_80133B44(void) {
    /* FAKE: two unused slots preserve the original local-buffer placement. T-24523 */
    s32 pad1;
    s32 pad2;
    s32 sp44;
    u8 sp40[3] = "　";

    /* FAKE: narrowing is redundant after the final s16 cast, but spends the two
     * temporary slots needed by the original register sequence. T-24523 */
    if (game_state.current_girl != GIRL_SHIORI) {
        if (game_state.substep == 0) {
            set_kanji_string((s16)(-0x68 - (s16)(strlen(name_code_family) * 4)), 0x30, 0, (s32)name_code_family, 0);
            set_kanji_string(-0x56, 0x30, 0, (s32)"『 ", 0);
            talk_render(-0x48, 0x30, 0, (s32)"それじゃ、神社に行くか。", 0, "", "", 0);
            D_80143B00 = 0;
            D_80143B04 = 0;
            ((u8 **)D_80143AF8)[0][0] = 7;
            ((u8 ***)D_80143AF4)[0][0][0] = sp40[0];
            ((u8 ***)D_80143AF4)[0][0][1] = sp40[1];
            ((u8 ***)D_80143AF4)[0][0][2] = 0;
            game_state.substep = 1;
        }
        sp44 = game_state.mode_step;
        func_801323D4();
        if (sp44 != game_state.mode_step) {
            mode_stage_next();
        }
    } else {
        mode_step_next();
    }
}

void func_80133CDC(void) {
    D_80143B00 = 0xF;
    mode_step_next();
}
