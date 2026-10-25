/* sick_visit_scene_dialogue.c: ; src/ovl/OMIMAI/801331B0.c:149-151,361-418 feeds talk tables and owns shared visit callbacks. Original object 0x801331B0-0x801348B0 (config/objects/OMIMAI.txt). */
#include "ovl/sick_visit.h"

typedef struct {
    void (*f[26])();
} FnTbl26; /* size 0x68 */
extern FnTbl26 D_80134CD0;

void sick_visit_common_mode_girl_talk_pointer_setup(void) {
    *D_80134C30 = 0x8019709C;
    D_80134C34 = 0x801977CC;
    D_80134C38 = 0x8019816C;
    D_80134C3C = 0x801984E0;
    D_80134C40 = 0x801987B8;
    D_80134C44 = 0x801989B4;
    D_80134C48 = 0x80198B84;
    D_80134C4C = 0x80198E70;
    D_80134C50 = 0x80199168;
    D_80134C54 = 0x80199420;
    D_80134C58 = 0x801996C4;
    D_80134C5C = 0x80199AE0;
    D_80134C60 = 0x80199DD8;
    *D_80134C64 = 0x801970AC;
    D_80134C68 = 0x801978AC;
    D_80134C6C = 0x8019819C;
    D_80134C70 = 0x80198520;
    D_80134C74 = 0x801987E8;
    D_80134C78 = 0x801989BC;
    D_80134C7C = 0x80198BB4;
    D_80134C80 = 0x80198E90;
    D_80134C84 = 0x801991A8;
    D_80134C88 = 0x80199438;
    D_80134C8C = 0x801996FC;
    D_80134C90 = 0x80199B38;
    D_80134C94 = 0x80199DF8;
    *D_80134C98 = 0x80197140;
    D_80134C9C = 0x80197F3C;
    D_80134CA0 = 0x80198304;
    D_80134CA4 = 0x80198620;
    D_80134CA8 = 0x80198950;
    D_80134CAC = 0x801989F8;
    D_80134CB0 = 0x80198D1C;
    D_80134CB4 = 0x80198F80;
    D_80134CB8 = 0x80199388;
    D_80134CBC = 0x801994B4;
    D_80134CC0 = 0x801998F0;
    D_80134CC4 = 0x80199CD0;
    D_80134CC8 = 0x80199EE8;
}

void sick_visit_common_mode_step_callback_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl26 tbl;

    tbl = D_80134CD0;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void sick_visit_saki_dialogue_special_case(void) {
    if ((game_state.current_girl == GIRL_SAKI) && (talk_group == 5)) {
        normal_date_speak_1line();
        return;
    }
    mode_step_next();
}

s32 sick_visit_common_day_event_complete(void) {
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
    return day_event_next(1);
}

void sick_visit_girl_audio_bank_load(void) {
    audio_bank_load_start(1, 0xBF98, 0xBF79, D_800B3688[game_state.current_girl], D_800B36C8[game_state.current_girl], D_800B3708[game_state.current_girl]);
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    mode_step_next();
}

void sick_visit_common_audio_bank_retry(void) {
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void sick_visit_common_audio_transition(void) {
    audio_command(0x200);
    mode_step_next();
}

void sick_visit_common_scene_sector_load(void) {
    cd_read_sectors(6, 0x80197000, 0xAF33);
    sick_visit_common_mode_girl_talk_pointer_setup();
    mode_step_next();
}

void sick_visit_common_scene_initialize(void) {
    draw2d3d(1, 0);
    actor_draw_enable(1);
    sprite_tables_clear();
    actor_clear_range(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_system_reset();
    k_reset(1);
    game_state.tile_layer_enabled = 0;
    game_state.quads_enabled = 1;
    talk_clear();
    game_state.screen_brightness = 0x80;
    bg_brightness = 0;
    bg_fade_amount = 0;
    hizuke_init();
    message_window_init();
    addr_init_bustup();
    text_marker_hide();
    sick_visit_day_event_girl_setup();
    mode_step_next();
}

void sick_visit_day_event_girl_setup(void) {
    talk_names_set(game_state.current_girl = game_state.day_events[game_state.day_event_index].girl);
    sick_visit_common_mode_girl_talk_pointer_setup();
    if (game_state.current_girl == GIRL_REI) {
        talk_group = 8;
    } else {
        talk_group = 0;
    }
    talk_line = 0;
    talk_text_table = D_80134C5C;
    talk_command_table = D_80134C90;
    talk_voice_table = D_80134CC4;
}

/* FAKE: uopt priority and cvt_pass NO_WIDEN helpers so unk_F68.w and unk_0F4.h
 * promote to $v1 per case, unk_03E promotes to $a0 in case 4, and talk_group
 * colours into $a0; T-24140 */
#define F68_SEL_388C ((((game_state.last_command.w ^ 1) * 0) + (game_state.last_command.w & 0xF)))
#define F0F4_SHR_388C (game_state.player_flags.h >> (((pad = 0) * 0) + 0xC))
#define F0F4_DIV_388C (game_state.player_flags.h / (((pad = 0) * 0) + 0x1000U))
#define F0F4_BOOST_388C ((game_state.player_flags.h - (((pad = 0) * 0) + 1)) * 0)

void sick_visit_talk_choice_select(void) {
    s32 pad; /* FAKE: 4-byte frame pad and dead-store target for SWP NO_WIDEN, T-24140 */
    u8 girl_rank;

    pad = (game_state.year == 0);
    girl_rank = get_g_zyotai_s(game_state.current_girl) & 0x7F;
    srand(game_state.frame_count);
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        if (F68_SEL_388C < 2U) {
            sick_visit_dialogue_variant = 0;
            talk_group = 0;
        } else if (F68_SEL_388C == 2) {
            sick_visit_dialogue_variant = 0;
            talk_group = 2;
        } else if (F68_SEL_388C == 3) {
            sick_visit_dialogue_variant = 0;
            talk_group = 4;
        } else if (F68_SEL_388C == 4) {
            sick_visit_dialogue_variant = 0;
            talk_group = (F0F4_SHR_388C * 2) + 6 + F0F4_BOOST_388C;
            if ((F0F4_DIV_388C + (F68_SEL_388C * 0)) == 0xA) {
                talk_group = talk_group - 4;
            }
        } else {
            sick_visit_dialogue_variant = 3;
            talk_group = 0x1A;
        }
        if (rand() % 2) {
            talk_group = talk_group + 1;
            if (talk_group == 5) {
                sick_visit_dialogue_variant = 2;
            }
        }
        break;
    case GIRL_MIO:
        if (F68_SEL_388C < 2U) {
            sick_visit_dialogue_variant = 0;
            talk_group = 0;
        } else {
            sick_visit_dialogue_variant = 0;
            talk_group = (F0F4_SHR_388C * 2) + 2 + F0F4_BOOST_388C;
        }
        if (rand() % 2) {
            talk_group += 1;
        }
        break;
    case GIRL_YUINA:
        if (F68_SEL_388C < 2U) {
            sick_visit_dialogue_variant = 0;
            talk_group = 0;
        } else {
            sick_visit_dialogue_variant = 4;
            talk_group = (F0F4_SHR_388C * 2) - 2 + F0F4_BOOST_388C;
        }
        if (rand() % 2) {
            talk_group += 1;
        }
        break;
    case GIRL_AYAKO:
        if (F68_SEL_388C < 3U) {
            sick_visit_dialogue_variant = 0;
            talk_group = 0;
        } else {
            sick_visit_dialogue_variant = 0;
            talk_group = (F0F4_SHR_388C * 2) - 6 + F0F4_BOOST_388C;
        }
        if (rand() % 2) {
            talk_group += 1;
        }
        break;
    case GIRL_SAKI:
        if (F68_SEL_388C < 4U) {
            sick_visit_dialogue_variant = 0;
            talk_group = 0;
            if (rand() % 2) {
                talk_group += 1;
            }
        } else if ((rand() % 2) || ((F0F4_SHR_388C == 6) && ((game_state.year == 0x62) || ((game_state.year == 0x61) && (game_state.month >= 9U)))) || (((((game_state.year == 0) * 0) + F0F4_SHR_388C) == 7) && (game_state.year == 0x62))) {
            if (girl_rank < 2U) {
                sick_visit_dialogue_variant = 6;
                talk_group = 4;
            } else {
                sick_visit_dialogue_variant = 5;
                talk_group = 5;
            }
        } else {
            sick_visit_dialogue_variant = 0;
            talk_group = F0F4_DIV_388C - 4 + F0F4_BOOST_388C;
        }
        break;
    case GIRL_YUKARI:
        if (F68_SEL_388C < 4U) {
            sick_visit_dialogue_variant = 0;
            talk_group = 0;
        } else {
            sick_visit_dialogue_variant = 0;
            talk_group = 2;
        }
        if (rand() % 2) {
            talk_group += 1;
        }
        break;
    case GIRL_NOZOMI:
        if (F68_SEL_388C < 4U) {
            sick_visit_dialogue_variant = 0;
            talk_group = 0;
            if (rand() % 2) {
                talk_group += 1;
                sick_visit_dialogue_variant = 2;
            }
        } else {
            sick_visit_dialogue_variant = 0;
            talk_group = 4;
            if (rand() % 2) {
                talk_group += 1;
            }
        }
        if (girl_rank < 2U) {
            talk_group += 2;
        }
        break;
    case GIRL_MIRA:
        sick_visit_dialogue_variant = 3;
        talk_group = 0;
        break;
    case GIRL_YUKO:
        sick_visit_dialogue_variant = 7;
        talk_group = 0;
        if (rand() % 2) {
            talk_group += 1;
        }
        break;
    case GIRL_MEGUMI:
        sick_visit_dialogue_variant = (u8) (rand() % 2);
        talk_group = 0;
        if (rand() % 2) {
            talk_group += 1;
        }
        break;
    case GIRL_YUMI:
        if (F68_SEL_388C < 4U) {
            talk_group = 0;
            if (rand() % 2) {
                talk_group += 1;
            }
            if (talk_group == 1) {
                sick_visit_dialogue_variant = 2;
            } else {
                sick_visit_dialogue_variant = 0;
            }
        } else {
            sick_visit_dialogue_variant = 0;
            talk_group = 2;
            if (rand() % 2) {
                talk_group += 1;
            }
        }
        break;
    default:
        talk_group = (s16) (rand() % 8);
        sick_visit_dialogue_variant = (rand() % 2) + 8;
        if (F68_SEL_388C == 3) {
            sick_visit_dialogue_variant = 9;
        }
        break;
    }
    talk_line = 0;
    sick_visit_common_girl_talk_pointer_select();
    mode_step_next();
}

#undef F68_SEL_388C
#undef F0F4_SHR_388C
#undef F0F4_DIV_388C
#undef F0F4_BOOST_388C

s32 sick_visit_talk_route_select(void) {
    if (game_state.current_girl == GIRL_MEGUMI) {
        if (talk_group == 2) {
            mode_step_next();
            return 0;
        }
    }
    if (game_state.current_girl == GIRL_YUKO) {
        mode_step_next();
        mode_step_next();
        return 0;
    }
    talk_text_table = D_80134C5C;
    talk_command_table = D_80134C90;
    talk_voice_table = D_80134CC4;
    talk_group = sick_visit_dialogue_variant + 1;
    mode_step_next();
}

void sick_visit_common_girl_talk_pointer_select(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        talk_text_table = D_80134C30[1];
        talk_command_table = D_80134C64[1];
        talk_voice_table = D_80134C98[1];
        return;
    case GIRL_MIO:
        talk_text_table = D_80134C30[6];
        talk_command_table = D_80134C64[6];
        talk_voice_table = D_80134C98[6];
        return;
    case GIRL_YUINA:
        talk_text_table = D_80134C30[2];
        talk_command_table = D_80134C64[2];
        talk_voice_table = D_80134C98[2];
        return;
    case GIRL_AYAKO:
        talk_text_table = D_80134C30[4];
        talk_command_table = D_80134C64[4];
        talk_voice_table = D_80134C98[4];
        return;
    case GIRL_SAKI:
        talk_text_table = D_80134C30[10];
        talk_command_table = D_80134C64[10];
        talk_voice_table = D_80134C98[10];
        return;
    case GIRL_YUKARI:
        talk_text_table = D_80134C30[7];
        talk_command_table = D_80134C64[7];
        talk_voice_table = D_80134C98[7];
        return;
    case GIRL_NOZOMI:
        talk_text_table = D_80134C30[8];
        talk_command_table = D_80134C64[8];
        talk_voice_table = D_80134C98[8];
        return;
    case GIRL_MIRA:
        talk_text_table = D_80134C30[5];
        talk_command_table = D_80134C64[5];
        talk_voice_table = D_80134C98[5];
        return;
    case GIRL_YUKO:
        talk_text_table = D_80134C30[0];
        talk_command_table = D_80134C64[0];
        talk_voice_table = D_80134C98[0];
        return;
    case GIRL_MEGUMI:
        talk_text_table = D_80134C30[9];
        talk_command_table = D_80134C64[9];
        talk_voice_table = D_80134C98[9];
        return;
    case GIRL_YUMI:
        talk_text_table = D_80134C30[12];
        talk_command_table = D_80134C64[12];
        talk_voice_table = D_80134C98[12];
        return;
    default:
        talk_text_table = D_80134C30[3];
        talk_command_table = D_80134C64[3];
        talk_voice_table = D_80134C98[3];
        return;
    }
}

void sick_visit_location_scene_select(void) {
    switch (game_state.last_command.w & 0xF) {
    case 0:
        strcpy(scene_place_title, "図書室");
        bg_read_sub2(0x40BC);
        break;
    case 1:
        strcpy(scene_place_title, "教室");
        if (game_state.month >= 6U && game_state.month < 10U) {
            bg_read_sub2(0x404D);
        } else {
            bg_read_sub2(0x4055);
        }
        break;
    case 2:
        strcpy(scene_place_title, "中庭");
        bg_read_sub2(0x4071);
        break;
    case 3:
        strcpy(scene_place_title, "自宅前");
        bg_read_sub2(0x414C);
        break;
    case 4:
        switch (game_state.player_flags.h >> 12) {
        case 0:
            strcpy(scene_place_title, "図書室");
            bg_read_sub2(0x40BC);
            break;
        case 1:
            strcpy(scene_place_title, "演劇部");
            bg_read_sub2(0x40B2);
            break;
        case 2:
            strcpy(scene_place_title, "実験室");
            bg_read_sub2(0x40C6);
            break;
        case 3:
            strcpy(scene_place_title, "電脳部室");
            bg_read_sub2(0x40D0);
            break;
        case 4:
            strcpy(scene_place_title, "美術室");
            bg_read_sub2(0x40D9);
            break;
        case 5:
            strcpy(scene_place_title, "音楽室");
            bg_read_sub2(0x40E2);
            break;
        case 6:
        case 7:
        case 8:
            strcpy(scene_place_title, "グランド");
            bg_read_sub2(0x4097);
            break;
        case 9:
            strcpy(scene_place_title, "プール");
            bg_read_sub2(0x40A0);
            break;
        default:
            strcpy(scene_place_title, "体育館");
            bg_read_sub2(0x40A9);
            break;
        }
        break;
    default:
        strcpy(scene_place_title, "廊下");
        bg_read_sub2(0x4045);
        break;
    }
    mode_step_next();
}

void sick_visit_portrait_select(void) {
    u8 girl;
    OmimaiWords11 all;
    OmimaiWords2 first;
    OmimaiWords2 second;
    OmimaiWords2 third;

    all = D_80134D38;
    first = D_80134D64;
    second = D_80134D6C;
    third = D_80134D74;
    if ((game_state.month < 6U) || (game_state.month >= 0xAU)) {
        all.unk_00[0] = 0x544A;
        all.unk_00[3] = 0x544A;
        all.unk_00[5] = 0x544A;
        first.unk_00[0] = 0x5D41;
        second.unk_00[1] = 0x522E;
        third.unk_00[1] = 0x58AF;
    }
    if ((game_state.last_command.w & 0xF) == 4) {
        girl = game_state.current_girl;
        switch (girl) {
        case 0:
            portrait_read_start(all.unk_00[game_state.player_flags.h >> 12]);
            break;
        case 1:
            portrait_read_start(first.unk_00[game_state.player_flags.h >> 12]);
            break;
        case 2:
            portrait_read_start(second.unk_00[(game_state.player_flags.h >> 12) - 2]);
            break;
        case 3:
            portrait_read_start(third.unk_00[(game_state.player_flags.h >> 12) - 4]);
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
        case 9:
            portrait_read_start(0x64D0);
            break;
        default:
            if ((game_state.month >= 6U) && (game_state.month < 0xAU)) {
                portrait_read_start(0x57FB);
            } else {
                portrait_read_start(0x5828);
            }
            break;
        }
        mode_step_next();
        return;
    }
    read_bustup_uniform();
}
