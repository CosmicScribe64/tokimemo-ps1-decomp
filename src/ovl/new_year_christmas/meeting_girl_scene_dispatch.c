/* meeting_girl_scene_dispatch.c: ; src/ovl/SHOUGATU/80136F90.c:34-92 dispatches meeting scenes by the queued girl selector. Original object 0x80136F90-0x80137BB0 (config/objects/SHOUGATU.txt). */
#include "common.h"
#include "ovl/new_year_christmas.h"

void new_year_meeting_dialogue_addresses_init(void) {
    D_80144D90 = 0x801976F8;
    D_80144D94 = 0x80198BF0;
    D_80144D98 = 0x8019A37C;
    D_80144D9C = 0x8019ADCC;
    D_80144DA0 = 0x8019BC80;
    D_80144DA4 = 0x8019C918;
    D_80144DA8 = 0x8019D460;
    D_80144DAC = 0x8019E168;
    D_80144DB0 = 0x8019EB4C;
    D_80144DB4 = 0x80197774;
    D_80144DB8 = 0x80198CF8;
    D_80144DBC = 0x8019A414;
    D_80144DC0 = 0x8019ADF8;
    D_80144DC4 = 0x8019BD34;
    D_80144DC8 = 0x8019C960;
    D_80144DCC = 0x8019D4C4;
    D_80144DD0 = 0x8019E210;
    D_80144DD4 = 0x8019EBA8;
    D_80144DD8 = 0x80197C94;
    D_80144DDC = 0x80199738;
    D_80144DE0 = 0x8019AB30;
    D_80144DE4 = 0x8019B000;
    D_80144DE8 = 0x8019C4B4;
    D_80144DEC = 0x8019CC70;
    D_80144DF0 = 0x8019D910;
    D_80144DF4 = 0x8019E7B4;
    D_80144DF8 = 0x8019EE04;
}

void new_year_meeting_mode_handler(void) {
    portrait_wink_update();
    switch (game_state.mode_stage) {
    case 0:
        new_year_meeting_scene_init();
        break;
    case 1:
        new_year_meeting_audio_bank_load_retry();
        break;
    case 2:
        new_year_meeting_resources_load();
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            mode_stage_next();
        } else {
            if (game_state.mode_stage_timer++ >= 0x401U) {
                mode_stage_set(2);
            }
        }
        break;
    case 4:
        switch (new_year_meeting_girl) {
        case 0:
            game_mode_reset_to_title();
            break;
        case 1:
            new_year_scene_callback_group_dispatch();
            break;
        case 2:
            new_year_yuina_scene_group_dispatch();
            break;
        case 3:
            new_year_ayako_scene_group_dispatch();
            break;
        case 4:
            new_year_saki_scene_callback_group_dispatch();
            break;
        case 5:
            new_year_yukari_scene_group_dispatch();
            break;
        case 6:
            new_year_nozomi_scene_group_dispatch();
            break;
        case 7:
            new_year_mira_scene_mode_step_dispatch_call();
            break;
        case 8:
            new_year_yuko_scene_callback_group_dispatch();
            break;
        case 9:
            game_mode_reset_to_title();
            break;
        case 10:
            new_year_yumi_scene_mode_step_dispatch_call();
            break;
        default:
            game_mode_reset_to_title();
            break;
        }
        break;
    case 5:
        new_year_meeting_audio_bank_load_start();
        break;
    case 6:
        new_year_meeting_audio_bank_load_retry();
        break;
    case 7:
        new_year_meeting_completion_state_apply();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    check_k_scroll();
    k_disp_inc2();
    hud_screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    hizuke_show();
    scene_frame_update();
    draw_shake_update();
}

s32 new_year_meeting_completion_state_apply(void) {
    GameState *g = &game_state;
    s32 pad;

    /* FAKE: touch narrow unsigned globals at entry for cvt_pass.py (T-20610); dead-store eliminated by uopt. T-24130 */
    pad = new_year_meeting_girl;
    pad = new_year_meeting_choice;
    pad = game_state.month;
    if (game_state.substep == 0) {
        audio_command(0xCF);
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
    game_state.current_girl = new_year_meeting_girl;
    if (new_year_meeting_girl == 2 && (new_year_meeting_choice == 2 || new_year_meeting_choice == 3) &&
        (game_state.girls[2].flags.b[2] >> 4) == 3) {
        event_log_add(8, new_year_meeting_girl * 10 + 6);
    } else if (new_year_meeting_girl == 4 && new_year_meeting_choice == 0 &&
               game_state.month >= 6 && game_state.month < 10) {
        event_log_add(8, new_year_meeting_girl * 10 + 3);
    } else {
        event_log_add(8, new_year_meeting_girl * 10 + new_year_meeting_choice);
    }
    if (new_year_meeting_girl == 10) {
        game_state.girls[game_state.day_events[game_state.day_event_index].girl].affection = 0x28;
    } else {
        game_state.girls[game_state.day_events[game_state.day_event_index].girl].affection = 0x32;
    }
    if (game_state.day_events[game_state.day_event_index].girl != 8) {
        game_state.girls[game_state.day_events[game_state.day_event_index].girl].flags.f.met = 1;
    } else {
        g->girls[8].flags.f.story_stage += 1;
        if (g->girls[8].flags.f.story_stage == 4) {
            game_state.girls[game_state.day_events[game_state.day_event_index].girl].flags.f.met = 1;
        }
    }
    audio_command(0x22);
    audio_command(0x21);
    audio_command(0x2F);
    day_event_next(1);
}

void new_year_meeting_scene_init(void) {
    GameState *g = &game_state;
    s32 a1;
    s32 a2;

    cd_read_checked = 0;
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    hizuke_init();
    message_window_init();
    talk_clear();
    /* FAKE: g-view store keeps ILOD CSE of unk_69C[unk_71C].unk_01. T-24137 */
    g->current_girl = g->day_events[g->day_event_index].girl;
    new_year_meeting_choice = talk_work;
    new_year_meeting_girl = g->current_girl;
    if (g->day_events[g->day_event_index].girl == 2 ||
        g->day_events[g->day_event_index].girl == 6) {
        a1 = 0xC063;
        a2 = 0xC045;
    } else {
        a1 = 0xC043;
        a2 = 0xC01D;
    }
    audio_bank_load_start(1, a1, a2,
                  D_800B3688[g->day_events[g->day_event_index].girl],
                  D_800B36C8[g->day_events[g->day_event_index].girl],
                  D_800B3708[g->day_events[g->day_event_index].girl]);
    if (audio_bank_load_poll() == 1) {
        mode_stage_next();
    }
    g->girls[g->day_events[g->day_event_index].girl].affection = 0x46;
    mode_stage_next();
}

void new_year_meeting_audio_bank_load_start(void) {
    audio_command(0xCF);
    hud_day_bgm_load();
    if (audio_bank_load_poll() == 1) {
        mode_stage_next();
    }
    mode_stage_next();
}

void new_year_meeting_audio_bank_load_retry(void) {
    if (audio_bank_load_poll() == 1) {
        mode_stage_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_stage_set((game_state.mode_stage - 1) & 0xFF);
    }
}

void new_year_meeting_audio_command_00cf(void) {
    audio_command(0xCF);
    mode_step_next();
}

void new_year_meeting_scene_audio_setup(void) {
    cd_read_checked = 0;
    audio_bank_load_start(0, 0xBF98, 0xBF79, 0xD989, 0xD949, 0xD941);
    audio_command(0x23);
    audio_command(0x24);
    audio_command(0x2E);
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    mode_step_next();
}

void new_year_meeting_scene_audio_wait(void) {
    if (audio_bank_load_poll() == 1) {
        audio_command(0x23);
        audio_command(0x24);
        audio_command(0x2E);
        mode_step_next();
    }
    if (game_state.mode_step_timer.u++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void new_year_meeting_audio_bank_start(void) {
    audio_bank_start(1, 0x200);
    mode_step_next();
}

void new_year_meeting_resources_load(void) {
    cd_read_sectors(0x10, 0x80197000, 0xAEFD);
    new_year_meeting_dialogue_addresses_init();
    new_year_meeting_girl = game_state.day_events[game_state.day_event_index].girl;
    mode_stage_next();
}

void new_year_meeting_resources_load_alt(void) {
    cd_read_sectors(0x10, 0x80197000, 0xAEFD);
    new_year_meeting_dialogue_addresses_init();
    new_year_meeting_girl = game_state.day_events[game_state.day_event_index].girl;
    mode_step_next();
}

void new_year_meeting_talk_run(void) {
    talk_tables = &new_year_meeting_dialogue_group;
    talk_active_line_ptr = &new_year_meeting_dialogue_line;
    talk_active_text_table = new_year_meeting_dialogue_text_table;
    talk_active_command_table = new_year_meeting_dialogue_command_table;
    talk_active_voice_table = new_year_meeting_dialogue_voice_table;
    talk_run(talk_choice, 1, 0);
}

void new_year_meeting_talk_run_with_flags(void) {
    talk_tables = (u8 *) &new_year_meeting_dialogue_group;
    talk_active_line_ptr = (u8 *) &new_year_meeting_dialogue_line;
    talk_active_text_table = new_year_meeting_dialogue_text_table;
    talk_active_command_table = new_year_meeting_dialogue_command_table;
    talk_active_voice_table = new_year_meeting_dialogue_voice_table;
    talk_run(talk_choice, 1, 1);
}
