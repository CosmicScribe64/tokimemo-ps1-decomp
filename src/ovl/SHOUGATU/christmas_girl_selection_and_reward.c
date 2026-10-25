/* christmas_girl_selection_and_reward.c: ; src/ovl/SHOUGATU/8013E330.c:64-106 resets current girl to Shiori, awards tokimeki by looks threshold, then updates affection and event log. Original object 0x8013E330-0x8013F190 (config/objects/SHOUGATU.txt). */
#define MAIN_API_OVERRIDE_printf /* FAKE: integer result keeps the retry flag in v1. T-25038 */
#define MAIN_API_OVERRIDE_talk_names_set /* FAKE: integer return view keeps E50C's loop counter in v1. T-25033 */
#define MAIN_API_OVERRIDE_get_g_zyotai_s /* matched with s32() (main_api.h: s32(s32)) */
#include "common.h"
#include "ovl/SHOUGATU.h"

s32 get_g_zyotai_s();

s32 talk_names_set();
s32 printf(void *arg0, ...);

/* Existing exception: EVENT owns a table at this overlapping address; moving
 * this scalar to main_api.h needs its override first. T-11590, T-25038 */
extern s32 D_80122D48;

/* T-9740: integer definition without an explicit return matches the original. */
s32 christmas_girl_dialogue_step_dispatch(void) {
    s32 mode_step_index;
    ShoCalendarFn28 christmas_girl_mode_step_callbacks;

    christmas_girl_mode_step_callbacks = D_8014605C;
    mode_step_index = game_state.mode_step;
    christmas_girl_mode_step_callbacks.f[mode_step_index](0x80);
    ((Actor *)actor_pool)[96].brightness = bg_brightness;
    if (((Actor *)actor_pool)[96].unk_18 == 0 && ((Actor *)actor_pool)[96].unk_08 == 0) {
        ((Actor *)actor_pool)[96].sequence_slot += 1;
        if (((Actor *)actor_pool)[96].sequence_slot >= 3) {
            ((Actor *)actor_pool)[96].sequence_slot = 0;
        }
    }
}

void christmas_shiori_status_gate(void) {
    u8 kind;

    kind = get_g_zyotai_s(GIRL_SHIORI) & 0x7F;
    if ((D_80146050 != 0) || (kind == 4)) {
        new_year_current_talk_start();
        return;
    }
    mode_step_next();
}

void christmas_current_girl_status_gate(void) {
    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    if (((u8)get_g_zyotai_s(game_state.current_girl) & 0x7F) != 4) {
        mode_step_next();
        return;
    }
    new_year_current_talk_start();
}

void christmas_reward_mode_step_update(void) {
    if (D_80146050 != 0) {
        if (D_80145F2C == 1) {
            D_80145F2C += 1;
        } else {
            D_80145F2C += 2;
        }
    }
    mode_step_next();
}

void christmas_shiori_stat_reward_apply(void) {
    s16 looks_reward_tier_index;
    s32 current_girl_index;
    ShoShorts7 looks_reward_thresholds;
    ShoShorts7 tokimeki_reward_values;

    looks_reward_thresholds = D_801460CC;
    tokimeki_reward_values = D_801460DC;
    /* FAKE: multiply by zero keeps this byte promoted across the conditional;
     * literal zero loses the entry a2 carrier and its later reload. T-25033 */
    current_girl_index = game_state.current_girl * 0;
    game_state.current_girl = current_girl_index;
    D_80146050 = 0;
    if (game_state.girls[9].flags.f.met == 0 &&
        game_state.girls[9].tokimeki >= 20 && (game_state.frame_count & 1)) {
        D_80146050 = 1;
    }
    /* FAKE: narrowing spends the target's expression temporaries; the byte
     * argument view keeps the later index-reload base in a2. T-25033 */
    D_80145F2C = (((u8)get_g_zyotai_s(*(u8 *)&game_state.current_girl) & 0x7F) == 4) * 4;
    D_80145F20 = D_80145AD4;
    D_80145F24 = D_80145B08;
    D_80145F28 = D_80145B3C;
    talk_names_set(game_state.current_girl);
    game_state.girls[game_state.current_girl].affection += 4;
    for (looks_reward_tier_index = 0; looks_reward_tier_index < 7; looks_reward_tier_index++) {
        if (game_state.param_looks.value < looks_reward_thresholds.v[looks_reward_tier_index]) {
            break;
        }
    }
    game_state.girls[game_state.current_girl].tokimeki += tokimeki_reward_values.v[looks_reward_tier_index];
    check_para_limit();
    event_log_add(4, 0);
    mode_step_next();
}

void christmas_girl_select_and_reward(void) {
    s16 girl_selection_scan_index;
    s16 met_girl_count;
    s32 selected_girl_status;
    ShoShorts7 looks_reward_thresholds;
    ShoShorts7 tokimeki_reward_values;

    looks_reward_thresholds = D_801460EC;
    tokimeki_reward_values = D_801460FC;
    met_girl_count = 0;
    for (girl_selection_scan_index = 0; girl_selection_scan_index < 11; girl_selection_scan_index++) {
        met_girl_count += game_state.girls[girl_selection_scan_index].flags.f.met >= 1;
    }
    girl_selection_scan_index = 0;
    do {
        /* FAKE: multiplication by one preserves the original sum operand order. T-25038 */
        game_state.current_girl = (game_state.frame_count * 1 + girl_selection_scan_index) % 10U + 1;
        printf("num%d girl%d\n", met_girl_count, game_state.current_girl);
        girl_selection_scan_index++;
        if (!game_state.girls[game_state.current_girl].flags.f.met && game_state.current_girl >= GIRL_MEGUMI) {
            continue;
        }
        if (game_state.current_girl == GIRL_YUKARI && (game_state.girls[0].flags.b[2] >> 4) == 8) {
            continue;
        }
        /* FAKE: a separate byte view preserves the second selector reload. T-25038 */
        if (*(u8 *)&game_state.current_girl == 6 && (game_state.girls[0].flags.b[2] >> 4) == 9) {
            continue;
        }
        if (!game_state.girls[game_state.current_girl].flags.f.met && met_girl_count >= 2) {
            continue;
        }
        break;
    } while (1);
    if (D_80146050 != 0) {
        game_state.current_girl = GIRL_MEGUMI;
    }
    D_80146054 = game_state.current_girl;
    D_80145F2C = 0;
    /* FAKE: narrowing spends the original temporary slots; the byte argument
     * view prevents selector promotion through the club filters. T-25038 */
    selected_girl_status = (u8)get_g_zyotai_s(*(u8 *)&game_state.current_girl);
    if (game_state.current_girl == GIRL_NOZOMI && (selected_girl_status & 0x7F) >= 2U) {
        D_80145F2C++;
    }
    if ((selected_girl_status & 0x7F) == 4) {
        D_80145F2C++;
    }
    christmas_girl_dialogue_resource_select();
    talk_names_set(game_state.current_girl);
    if (!game_state.girls[game_state.current_girl].flags.f.met || (D_80146050 != 0 && game_state.current_girl == GIRL_MEGUMI)) {
        game_state.girls[game_state.current_girl].flags.f.met = 1;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = 0x11;
        D_80122D48 = 3;
        if (D_80146050 != 0) {
            event_log_add(8, game_state.current_girl * 10);
        } else {
            event_log_add(4, 0);
        }
        mode_stage_set(5);
        return;
    }
    game_state.girls[game_state.current_girl].affection += 4;
    for (girl_selection_scan_index = 0; girl_selection_scan_index < 7; girl_selection_scan_index++) {
        if (game_state.param_looks.value < looks_reward_thresholds.v[girl_selection_scan_index]) {
            break;
        }
    }
    game_state.girls[game_state.current_girl].tokimeki += tokimeki_reward_values.v[girl_selection_scan_index];
    event_log_add(4, 0);
    check_para_limit();
    mode_step_next();
}

void christmas_alternate_girl_select_and_reward(void) {
    s16 girl_selection_scan_index;
    s16 met_girl_count;
    u32 girl_selection_scan_counter;
    ShoShorts7 looks_reward_thresholds;
    ShoShorts7 tokimeki_reward_values;

    looks_reward_thresholds = D_8014610C;
    tokimeki_reward_values = D_8014611C;
    met_girl_count = 0;
    /* FAKE (T-25501): the increment inside the subscript, not in the for
     * header, keeps the loop's block shape (header girl_selection_scan_index++ gives 8 rows). */
    for (girl_selection_scan_index = 0; girl_selection_scan_index < 11;) {
        met_girl_count += game_state.girls[girl_selection_scan_index++].flags.f.met >= 1;
    }
    girl_selection_scan_counter = 0;
    /* FAKE (T-25501): the "* 0" terms in this loop are references that uopt
     * counts and ugen folds away. The b1 re-read makes the >= 9 test an alias
     * hole of the selector; the selector and counter terms set the colouring
     * order of the loop ranges (selector piece in t4, counter in v0).
     * The three *(u8 *)& views of unk_F5F in the == 5, == 6 and D_80146054
     * compares are also FAKE: plain reads give 236, 227 and 224 rows.
     * The u32 counter with the (s16) truncation is FAKE too: girl_selection_scan_counter++ gives
     * 314 rows and an s16 counter 2; the counter is reused below. */
    do {
        game_state.current_girl = (game_state.frame_count + girl_selection_scan_counter) % 10U + 1;
        girl_selection_scan_counter = (s16)(girl_selection_scan_counter + 1);
        if (game_state.girls[game_state.current_girl].flags.f.met || game_state.current_girl < 9 + game_state.girls[game_state.current_girl].flags.f.met * 0) {
            if (*(u8 *)&game_state.current_girl != 5 || ((game_state.girls[0].flags.b[2] >> 4) + game_state.current_girl * 0 + girl_selection_scan_counter * 0) != 8) {
                if (game_state.current_girl != GIRL_NOZOMI || ((game_state.girls[0].flags.b[2] >> 4) + *(u8 *)&game_state.current_girl * 0 + girl_selection_scan_counter * 0) != 9) {
                    if (D_80146054 != *(u8 *)&game_state.current_girl) {
                        if (game_state.girls[game_state.current_girl].flags.f.met) {
                            break;
                        }
                        if (met_girl_count < 3) {
                            break;
                        }
                    }
                }
            }
        }
    } while (1);
    /* FAKE (T-25501): the counter holds the forced choice so the compares
     * keep their operand order; the record read is an alias hole, and the
     * forced store and the next two reads go through the D_800E71DF alias
     * of unk_F5F, which gives the v1 piece of the original. The byte-view
     * "* 0" term in the forced store keeps the selector range live through
     * the exit path (152 rows without it). */
    girl_selection_scan_counter = D_80145FE4;
    if ((girl_selection_scan_counter == 7 || girl_selection_scan_counter == 9) && D_80146054 != girl_selection_scan_counter + game_state.girls[girl_selection_scan_counter * 0].affection * 0) {
        D_800E71DF = girl_selection_scan_counter + *(u8 *)&game_state.current_girl * 0;
    }
    D_80146058 = D_800E71DF;
    D_80145F2C = 0;
    /* FAKE: narrowing as in christmas_girl_select_and_reward. T-25038 */
    girl_selection_scan_counter = (u8)get_g_zyotai_s(D_800E71DF);
    if (game_state.current_girl == GIRL_NOZOMI && (girl_selection_scan_counter & 0x7F) >= 2U) {
        D_80145F2C++;
    }
    if ((girl_selection_scan_counter & 0x7F) == 4) {
        D_80145F2C++;
    }
    christmas_girl_dialogue_resource_select();
    /* FAKE (T-25501): the byte view passes the selector itself, which pins
     * its range to a0 here and forces uopt to split the loop piece. */
    talk_names_set(*(u8 *)&game_state.current_girl);
    if (!game_state.girls[game_state.current_girl].flags.f.met) {
        game_state.girls[game_state.current_girl].flags.f.met = 1;
        game_state.saved_game_mode = game_state.game_mode;
        game_state.saved_mode_stage = game_state.mode_stage;
        game_state.saved_mode_step = 0x18;
        D_80122D48 = 3;
        event_log_add(4, 0);
        mode_stage_set(5);
    } else {
        game_state.girls[game_state.current_girl].affection += 4;
        for (girl_selection_scan_index = 0; girl_selection_scan_index < 7; girl_selection_scan_index++) {
            if (game_state.param_looks.value < looks_reward_thresholds.v[girl_selection_scan_index]) {
                break;
            }
        }
        game_state.girls[game_state.current_girl].tokimeki += tokimeki_reward_values.v[girl_selection_scan_index];
        event_log_add(4, 0);
        mode_step_next();
    }
    check_para_limit();
}

void christmas_girl_dialogue_resource_select(void) {
    switch (game_state.current_girl) {
    case GIRL_MIO:
        D_80145F20 = D_80145AE4;
        D_80145F24 = D_80145B18;
        D_80145F28 = D_80145B4C;
        return;
    case GIRL_YUINA:
        D_80145F20 = D_80145AD8;
        D_80145F24 = D_80145B0C;
        D_80145F28 = D_80145B40;
        return;
    case GIRL_AYAKO:
        D_80145F20 = D_80145ADC;
        D_80145F24 = D_80145B10;
        D_80145F28 = D_80145B44;
        return;
    case GIRL_SAKI:
        D_80145F20 = D_80145AF4;
        D_80145F24 = D_80145B28;
        D_80145F28 = D_80145B5C;
        return;
    case GIRL_YUKARI:
        D_80145F20 = D_80145AE8;
        D_80145F24 = D_80145B1C;
        D_80145F28 = D_80145B50;
        return;
    case GIRL_NOZOMI:
        D_80145F20 = D_80145AEC;
        D_80145F24 = D_80145B20;
        D_80145F28 = D_80145B54;
        return;
    case GIRL_MIRA:
        D_80145F20 = D_80145AE0;
        D_80145F24 = D_80145B14;
        D_80145F28 = D_80145B48;
        return;
    case GIRL_YUKO:
        D_80145F20 = D_80145AD0;
        D_80145F24 = D_80145B04;
        D_80145F28 = D_80145B38;
        return;
    case GIRL_MEGUMI:
        D_80145F20 = D_80145AF0;
        D_80145F24 = D_80145B24;
        D_80145F28 = D_80145B58;
        return;
    case GIRL_YUMI:
        D_80145F20 = D_80145B00;
        D_80145F24 = D_80145B34;
        D_80145F28 = D_80145B68;
        /* fallthrough */
    default:
        return;
    }
}

void new_year_christmas_portrait_resource_select(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        portrait_read_start(0x56C0);
        break;
    case GIRL_MIO:
        portrait_read_start(0x5E7C);
        break;
    case GIRL_YUINA:
        portrait_read_start(0x5369);
        break;
    case GIRL_AYAKO:
        portrait_read_start(0x59EA);
        break;
    case GIRL_SAKI:
        portrait_read_start(0x6827);
        break;
    case GIRL_YUKARI:
        portrait_read_start(0x611F);
        break;
    case GIRL_NOZOMI:
        portrait_read_start(0x6395);
        break;
    case GIRL_MIRA:
        portrait_read_start(0x5C60);
        break;
    case GIRL_YUKO:
        portrait_read_start(0x51A7);
        break;
    case GIRL_MEGUMI:
        if (D_80146050 != 0) {
            portrait_read_start(0x6638);
        } else {
            portrait_read_start(0x65DE);
        }
        break;
    case GIRL_YUMI:
        portrait_read_start(0x6A9D);
        break;
    }
    mode_step_next();
}
