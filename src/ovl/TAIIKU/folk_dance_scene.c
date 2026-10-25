/* folk_dance_scene.c: ; src/ovl/TAIIKU/80132000.c:9-18 begins the folk-dance scene and selects runner participants. Original object 0x80132000-0x80132B40 (config/objects/TAIIKU.txt). */
#include "common.h"
#include "ovl/TAIIKU.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
/* Defined in the unit (not an island piece) so that the stores to the runner list share one lui at; T-19031, sports_folk_dance_entry. */
u8 D_80148EC0[4] = { 0 };
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/folk_dance_scene.data", D_80148EC4);

void sports_folk_dance_entry(void) {
    s32 i;
    s32 j;
    s32 participant_swap_count;
    u8 met_girl_count;
    u8 first_girl;
    u8 second_girl;
    u8 t;
    u8 sp74[11];
    s32 k;
    s32 l;
    s32 m;
    TaiWords11 sp3C;

    sp3C = D_80148EC4;
    portrait_wink_update();
    switch (game_state.mode_step) {
    case 0:
        audio_bank_load_start(0, 0xBF98, 0xBF79, 0xD712, 0xD6C3, 0xD6B7);
        scene_screen_reset(1, 0);
        dec_bg_reset();
        set_dec_bri(0);
        talk_clear();
        actor_clear_upper();
        k_reset(0);
        message_window_set_title((s32) "フォークダンス");
        k_disp_start(1);
        message_window_title_show(0);
        parameter_disp_switch(0);
        icon_disp_switch(0);
        message_window_init();
        srand(game_state.timer_ticks);
        D_80122E90 = 0;
        mode_step_next();
        break;
    case 1:
        if (audio_bank_load_poll() == 1) {
            met_girl_count = 0;
            if (j) {} /* FAKE: empty test splits the live range of j. T-19031 */
            for (j = 0; j < 11; j++) {
                if (game_state.girls[j].flags.f.met == 1) {
                    sp74[met_girl_count] = j;
                    met_girl_count++;
                }
            }
            if (met_girl_count < 3) {
                D_80122ECC = 2;
            } else {
                if (D_80122E90 == 0) {
                    dec_bg_cd_read(0x4133, 1);
                }
                game_state.screen_brightness = 0x80;
                first_girl = sp74[rand() % met_girl_count];
                second_girl = sp74[rand() % met_girl_count];
                if (D_80122E90 != 0) {
                    first_girl = sp74[1];
                    second_girl = sp74[2];
                } else {
                    while (first_girl == 0) {
                        first_girl = sp74[rand() % met_girl_count];
                    }
                    while ((first_girl == second_girl) || (second_girl == 0)) {
                        second_girl = sp74[rand() % met_girl_count];
                    }
                    D_80122E90 += 1;
                }
                D_80148EC0[0] = 0;
                D_80148EC0[1] = first_girl;
                D_80148EC0[2] = second_girl;
                participant_swap_count = rand() & 0x1F;
                for (j = 0; j < participant_swap_count; j++) {
                    k = rand() % 3;
                    l = rand() % 3;
                    t = D_80148EC0[k];
                    m = D_80148EC0[l];
                    D_80148EC0[k] = m;
                    D_80148EC0[l] = t;
                }
                mode_step_next();
            }
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            sports_folk_dance_log_append();
            audio_bank_start(0, 0x200);
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            portrait_read_start(sp3C.value[D_80148EC0[0]]);
            message_window_title_show(1);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = D_80148EC0[0];
            portrait_show_checked();
        }
        break;
    case 5:
        cd_read_sectors(2, 0x801E8000, 0xB6DC);
        mode_step_next();
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            text_marker_hide();
            talk_names_set(D_80148EC0[0]);
            sports_folk_dance_talk_resource_addresses_init();
            talk_group = 0;
            talk_line = 0;
            talk_text_table = D_801491B0;
            talk_command_table = D_801491B4;
            talk_voice_table = D_801491B8;
            mode_step_next();
        }
        break;
    case 7:
    case 8:
    case 9:
    case 12:
    case 15:
    case 17:
    case 22:
    case 25:
    case 27:
    case 32:
    case 35:
    case 40:
        normal_date_speak();
        break;
    case 10:
        switch (girl_state_rank(D_80148EC0[0])) {
        case 0:
        case 1:
            talk_group = D_80148EC0[0] * 3 + 5;
            break;
        case 2:
        case 3:
            talk_group = D_80148EC0[0] * 3 + 6;
            break;
        case 4:
            talk_group = D_80148EC0[0] * 3 + 7;
            break;
        }
        mode_step_next();
        break;
    case 11:
    case 20:
    case 30:
        normal_date_girl_in();
        break;
    case 13:
    case 23:
    case 33:
    case 38:
        normal_date_girl_out();
        break;
    case 14:
    case 24:
        talk_group = 1;
        talk_line = 0;
        mode_step_next();
        break;
    case 16:
        talk_names_set(D_80148EC0[1]);
        mode_step_next();
        break;
    case 18:
        portrait_read_start(sp3C.value[D_80148EC0[1]]);
        mode_step_next();
        break;
    case 19:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = D_80148EC0[1];
            portrait_show_checked();
        }
        break;
    case 21:
        switch (girl_state_rank(D_80148EC0[1])) {
        case 0:
        case 1:
            talk_group = D_80148EC0[1] * 3 + 5;
            break;
        case 2:
        case 3:
            talk_group = D_80148EC0[1] * 3 + 6;
            break;
        case 4:
            talk_group = D_80148EC0[1] * 3 + 7;
            break;
        }
        mode_step_next();
        break;
    case 26:
        talk_names_set(D_80148EC0[2]);
        mode_step_next();
        break;
    case 28:
        portrait_read_start(sp3C.value[D_80148EC0[2]]);
        mode_step_next();
        break;
    case 29:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            game_state.current_girl = D_80148EC0[2];
            portrait_show_checked();
        }
        break;
    case 31:
        if ((game_state.girls[D_80148EC0[2]].syoushin >= 0x4C) || (girl_state_rank(D_80148EC0[2]) == 4)) {
            audio_command(0xBF);
            portrait_layout_set(4);
            mode_step_set(0x26);
        } else {
            switch (girl_state_rank(D_80148EC0[2])) {
            case 0:
            case 1:
                talk_group = D_80148EC0[2] * 3 + 5;
                break;
            case 2:
            case 3:
                talk_group = D_80148EC0[2] * 3 + 6;
                break;
            case 4:
                talk_group = D_80148EC0[2] * 3 + 7;
                break;
            }
            mode_step_next();
        }
        break;
    case 34:
        talk_group = 3;
        talk_line = 0;
        mode_step_next();
        break;
    case 36:
    case 41:
        normal_date_bg_fadeout();
        break;
    case 37:
    case 42:
        for (i = 0; i < 3; i++) {
            game_state.girls[D_80148EC0[i]].affection += 1;
        }
        D_80122ECC = 2;
        break;
    case 39:
        talk_group = 4;
        talk_line = 0;
        mode_step_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

/* The GameState pointer makes the record stores indirect, so the runner list
 * byte is reloaded for each use as in the original (T-16554). */
void sports_folk_dance_log_append(void) {
    GameState *game_state_ptr;
    s32 participant_index;

    game_state_ptr = &game_state;
    for (participant_index = 0; participant_index < 3; participant_index++) {
        game_state_ptr->event_log[game_state_ptr->event_log_count].girl = D_80148EC0[participant_index];
        game_state_ptr->event_log[game_state_ptr->event_log_count].scene = 0x13;
        game_state_ptr->event_log[game_state_ptr->event_log_count].flag = 0;
        game_state_ptr->event_log[game_state_ptr->event_log_count].rank = get_g_zyotai_h(D_80148EC0[participant_index]);
        if ((participant_index == 2) && (game_state_ptr->girls[D_80148EC0[participant_index]].syoushin >= 0x4C)) {
            game_state_ptr->event_log[game_state_ptr->event_log_count].rank = 4;
        }
        game_state_ptr->event_log[game_state_ptr->event_log_count].year = game_state_ptr->year;
        game_state_ptr->event_log[game_state_ptr->event_log_count].month = game_state_ptr->month;
        game_state_ptr->event_log[game_state_ptr->event_log_count].day = game_state_ptr->day;
        game_state_ptr->event_log[game_state_ptr->event_log_count].weekday = game_state_ptr->weekday;
        game_state_ptr->event_log_count++;
        if (game_state_ptr->event_log_count >= 0xFF) {
            game_state_ptr->event_log_count = 0xFE;
        }
    }
}

