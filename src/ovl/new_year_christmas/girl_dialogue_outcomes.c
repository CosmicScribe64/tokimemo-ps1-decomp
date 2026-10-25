/* girl_dialogue_outcomes.c: ; src/ovl/SHOUGATU/8013BA00.c:4-10,13-50,68-83 dispatches outcome steps, selects the current girl, applies relationship changes and advances the day event. Original object 0x8013BA00-0x8013C070 (config/objects/SHOUGATU.txt). */
#include "common.h"
#include "ovl/new_year_christmas.h"

void new_year_relationship_outcome_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl19 tbl;

    tbl = D_80145A80;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

s32 new_year_dialogue_relationship_outcome_apply(void) {
    s16 girl_record_index;

    if (game_state.substep == 0) {
        hud_day_bgm_load();
        game_state.substep += 1;
    }
    if (game_state.substep == 1) {
        if (game_state.mode_step_timer.u++ >= 0x400U) {
            cd_command_reset();
            audio_bank_state_reset();
            game_state.substep = 0;
        }
        if (audio_bank_load_poll() == 1) {
            game_state.substep += 1;
        } else {
            return 0;
        }
    }
    for (girl_record_index = 0; girl_record_index < 11; girl_record_index++) {
        if (((ShouByteFlags *)&game_state.girls[girl_record_index].flags.b[1])->b0 != 0) {
            ((ShouByteFlags *)&game_state.girls[girl_record_index].flags.b[1])->b0 = 0;
            if (girl_record_index != (s32)game_state.current_girl) {
                game_state.girls[girl_record_index].affection = game_state.girls[girl_record_index].affection + 1;
                /* FAKE: uopt CSE priority boost so &game_state + girl_record_index * 0x38 gets $v0 (girl_record_index in $v1)
                 * and &game_state gets $a1 (0x38 in $a2), matching pre-unification GameState layout. T-24125. */
                if (game_state.girls[girl_record_index].affection) {
                }
                if (game_state.girls[0].affection) {
                }
            }
        }
    }
    game_state.girls[game_state.current_girl].tokimeki += 8;
    game_state.girls[game_state.current_girl].affection += 8;
    game_state.girls[game_state.current_girl].syoushin -= 10;
    check_para_limit();
    day_event_next(1);
}

void new_year_relationship_snapshot_init(void) {
    s16 i;

    new_year_hallway_dialogue_text_table = D_8014598C;
    new_year_hallway_dialogue_command_table = D_801459C0;
    new_year_hallway_dialogue_voice_table = D_801459F4;
    new_year_hallway_dialogue_group = 0;
    new_year_hallway_dialogue_line = 0;
    for (i = 0; i < 0xB; i++) {
        select_girl_menu_enabled[i] = (game_state.girls[i].flags.b[1] & 1) >= 1;
    }
    mode_step_next();
}

void new_year_selected_girl_context_set(void) {
    game_state.current_girl = (u8) talk_choice;
    event_log_add(7, 0);
    talk_names_set(game_state.current_girl);
    mode_step_next();
}

void new_year_current_girl_status_selector_update(void) {
    u8 current_girl_status;

    current_girl_status = get_g_zyotai_s(game_state.current_girl) & 0x7F;
    if (current_girl_status >= 2U) {
        new_year_hallway_dialogue_group += 2;
    }
    if (current_girl_status >= 3U) {
        new_year_hallway_dialogue_group += 2;
    }
    if (current_girl_status == 4) {
        new_year_hallway_dialogue_group += 2;
    }
    if (game_state.current_girl == GIRL_MEGUMI && current_girl_status < 2U) {
        portrait_layout_set(0);
    }
    mode_step_next();
}

void new_year_current_girl_dialogue_resource_select(void) {

    new_year_hallway_dialogue_group = 0;
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        new_year_hallway_dialogue_text_table = D_80145964;
        new_year_hallway_dialogue_command_table = D_80145998;
        new_year_hallway_dialogue_voice_table = D_801459CC;
        break;
    case GIRL_MIO:
        new_year_hallway_dialogue_text_table = D_80145978;
        new_year_hallway_dialogue_command_table = D_801459AC;
        new_year_hallway_dialogue_voice_table = D_801459E0;
        break;
    case GIRL_YUINA:
        new_year_hallway_dialogue_text_table = D_80145968;
        new_year_hallway_dialogue_command_table = D_8014599C;
        new_year_hallway_dialogue_voice_table = D_801459D0;
        break;
    case GIRL_AYAKO:
        new_year_hallway_dialogue_text_table = D_80145970;
        new_year_hallway_dialogue_command_table = D_801459A4;
        new_year_hallway_dialogue_voice_table = D_801459D8;
        break;
    case GIRL_SAKI:
        new_year_hallway_dialogue_text_table = D_80145988;
        new_year_hallway_dialogue_command_table = D_801459BC;
        new_year_hallway_dialogue_voice_table = D_801459F0;
        break;
    case GIRL_YUKARI:
        new_year_hallway_dialogue_text_table = D_8014597C;
        new_year_hallway_dialogue_command_table = D_801459B0;
        new_year_hallway_dialogue_voice_table = D_801459E4;
        break;
    case GIRL_NOZOMI:
        new_year_hallway_dialogue_text_table = D_80145980;
        new_year_hallway_dialogue_command_table = D_801459B4;
        new_year_hallway_dialogue_voice_table = D_801459E8;
        break;
    case GIRL_MIRA:
        new_year_hallway_dialogue_text_table = D_80145974;
        new_year_hallway_dialogue_command_table = D_801459A8;
        new_year_hallway_dialogue_voice_table = D_801459DC;
        break;
    case GIRL_YUKO:
        new_year_hallway_dialogue_text_table = D_80145960;
        new_year_hallway_dialogue_command_table = D_80145994;
        new_year_hallway_dialogue_voice_table = D_801459C8;
        break;
    case GIRL_MEGUMI:
        new_year_hallway_dialogue_text_table = D_80145984;
        new_year_hallway_dialogue_command_table = D_801459B8;
        new_year_hallway_dialogue_voice_table = D_801459EC;
        break;
    case GIRL_YUMI:
        new_year_hallway_dialogue_text_table = D_80145990;
        new_year_hallway_dialogue_command_table = D_801459C4;
        new_year_hallway_dialogue_voice_table = D_801459F8;
        break;
    }
    mode_step_next();
}
