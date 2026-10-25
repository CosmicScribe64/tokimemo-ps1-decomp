/* scene_callbacks.c: src/ovl/KANGEI/80132090.c:49: mode 0x60 callback handlers for the scene state table. Original object 0x80132090-0x80132E40 (config/objects/KANGEI.txt). */
#include "common.h"
#include "ovl/KANGEI.h"

typedef struct {
    void (*f[22])();
} FnTbl22; /* size 0x58 */
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_80139950);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_80139954);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_80139958);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_8013995C);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_80139960);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_80139964);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_80139968);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_8013996C);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_80139970);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_80139974);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_80139978);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_8013997C);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_80139980);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_80139984);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_80139988);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_8013998C);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_80139990);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_80139994);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_80139998);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_8013999C);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_801399A0);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_801399A4);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_801399A8);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_801399AC);
/* Scene table, indexed by game_state.mode_step in camp_mode_step_callback_dispatch. Defining it here as the
 * original did matters for code: as1 treats every function named in this file's data as an
 * address-taken label, so a jump-table dispatch in this file gets an edge back to the entry of
 * each named function (T-25150, T-25139). */
FnTbl22 D_801399B0 = { {
    camp_scene_ui_reset, camp_scene_callback_current_girl_audio_bank_load_start, camp_scene_callback_audio_bank_poll_retry, camp_dialogue_sector_load, scene_wait_cd_read,
    camp_event_log_and_talk_setup, scene_wait_cd_read, dec_init, change_dec_bg, camp_club_variant_current_girl_portrait_read,
    scene_wait_cd_read, portrait_show_checked, scene_bg_wipe_in, place_init, camp_audio_transition_command,
    camp_current_girl_portrait_layout, normal_date_girl_in, normal_date_speak, normal_date_girl_out, normal_date_bg_out,
    camp_final_audio_load_complete, game_mode_reset_to_title,
} };
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_80139A08);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_80139A34);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_80139A3C);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/scene_callbacks.data", D_80139A44);

void camp_scene_callback_dialogue_pointer_table_setup(void) {
    D_80139950[0][0] = 0x801973AC;
    D_80139950[0][1] = 0x80197910;
    D_80139950[0][2] = 0x80197C44;
    D_80139950[0][3] = 0x80197F34;
    D_80139950[0][4] = 0x80198130;
    D_80139950[0][5] = 0x801982DC;
    D_80139950[0][6] = 0x801984F4;
    D_80139950[0][7] = 0x8019865C;
    D_80139950[1][0] = 0x8019740C;
    D_80139950[1][1] = 0x80197930;
    D_80139950[1][2] = 0x80197C74;
    D_80139950[1][3] = 0x80197F4C;
    D_80139950[1][4] = 0x80198140;
    D_80139950[1][5] = 0x801982F4;
    D_80139950[1][6] = 0x8019850C;
    D_80139950[1][7] = 0x80198668;
    D_80139950[2][0] = 0x801977AC;
    D_80139950[2][1] = 0x80197A70;
    D_80139950[2][2] = 0x80197E44;
    D_80139950[2][3] = 0x80198034;
    D_80139950[2][4] = 0x801981F0;
    D_80139950[2][5] = 0x801983DC;
    D_80139950[2][6] = 0x801985F4;
    D_80139950[2][7] = 0x801986DC;
}

void camp_mode_step_callback_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl22 tbl;

    tbl = D_801399B0;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

s32 camp_final_audio_load_complete(void) {
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

void camp_current_girl_portrait_layout(void) {
    u32 v;
    s32 lo;

    v = get_g_zyotai_h(game_state.current_girl);
    lo = v & 0xFF;
    if (v >= 3U) {
        portrait_layout_set(3);
    } else {
        portrait_layout_set(lo);
    }
    mode_step_next();
}

void camp_scene_callback_current_girl_audio_bank_load_start(void) {
    audio_bank_load_start(1, 0xBF98, 0xBF79, (&D_800B3688)[game_state.current_girl], (&D_800B36C8)[game_state.current_girl], (&D_800B3708)[game_state.current_girl]);
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    mode_step_next();
}

void camp_scene_callback_audio_bank_poll_retry(void) {
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void camp_audio_transition_command(void) {
    audio_command(0x200);
    mode_step_next();
}

void camp_dialogue_sector_load(void) {
    cd_read_sectors(3, 0x80197000, 0xAF3C);
    camp_scene_callback_dialogue_pointer_table_setup();
    mode_step_next();
}

void camp_scene_ui_reset(void) {
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
    camp_date_girl_scene_pointer_setup();
    mode_step_next();
}

void camp_date_girl_scene_pointer_setup(void) {
    game_state.current_girl = game_state.date_girl;
    talk_names_set(game_state.date_girl);
    camp_scene_callback_dialogue_pointer_table_setup();
    camp_current_girl_dialogue_pointer_bind();
}

void camp_current_girl_dialogue_pointer_bind(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        talk_text_table = D_80139950[0][0];
        talk_command_table = D_80139950[1][0];
        talk_voice_table = D_80139950[2][0];
        return;
    case GIRL_MIO:
        talk_text_table = D_80139950[0][3];
        talk_command_table = D_80139950[1][3];
        talk_voice_table = D_80139950[2][3];
        return;
    case GIRL_YUINA:
        talk_text_table = D_80139950[0][1];
        talk_command_table = D_80139950[1][1];
        talk_voice_table = D_80139950[2][1];
        return;
    case GIRL_AYAKO:
        talk_text_table = D_80139950[0][2];
        talk_command_table = D_80139950[1][2];
        talk_voice_table = D_80139950[2][2];
        return;
    case GIRL_SAKI:
        talk_text_table = D_80139950[0][6];
        talk_command_table = D_80139950[1][6];
        talk_voice_table = D_80139950[2][6];
        return;
    case GIRL_YUKARI:
        talk_text_table = D_80139950[0][4];
        talk_command_table = D_80139950[1][4];
        talk_voice_table = D_80139950[2][4];
        return;
    case GIRL_NOZOMI:
        talk_text_table = D_80139950[0][5];
        talk_command_table = D_80139950[1][5];
        talk_voice_table = D_80139950[2][5];
        return;
    default:
        talk_text_table = D_80139950[0][7];
        talk_command_table = D_80139950[1][7];
        talk_voice_table = D_80139950[2][7];
        return;
    }
}

void camp_event_log_and_talk_setup(void) {
    u32 club;

    /* FAKE: the two empty tests of the club halfword are plain uses of the global (no widening
     * CVT); with them uopt keeps the halfword in a register range and every read of it is
     * `lui v1; lhu v1`, as in the original. Any two such tests anywhere in the function do it,
     * one is not enough. T-25139 */
    if (game_state.player_flags.h) {}
    event_log_add(0x10, game_state.player_flags.h >> 12);
    if (game_state.player_flags.h) {} /* FAKE: second plain use, see above. T-25139 */
    switch (game_state.player_flags.h >> 12) {
    case 0:
        strcpy(scene_place_title, "文芸部");
        bg_read_sub2(0x40BC);
        break;
    case 1:
        strcpy(scene_place_title, "演劇部");
        bg_read_sub2(0x40B2);
        break;
    case 2:
        strcpy(scene_place_title, "科学部");
        bg_read_sub2(0x40C6);
        break;
    case 3:
        strcpy(scene_place_title, "電脳部");
        bg_read_sub2(0x40D0);
        break;
    case 4:
        strcpy(scene_place_title, "美術部");
        bg_read_sub2(0x40D9);
        break;
    case 5:
        strcpy(scene_place_title, "吹奏楽部");
        bg_read_sub2(0x40E2);
        break;
    case 6:
        strcpy(scene_place_title, "野球部");
        bg_read_sub2(0x4097);
        break;
    case 7:
        strcpy(scene_place_title, "サッカー部");
        bg_read_sub2(0x4097);
        break;
    case 8:
        strcpy(scene_place_title, "テニス部");
        bg_read_sub2(0x4097);
        break;
    case 9:
        strcpy(scene_place_title, "水泳部");
        bg_read_sub2(0x40A0);
        break;
    default:
        strcpy(scene_place_title, "バスケ部");
        bg_read_sub2(0x40A9);
        break;
    }
    talk_line = 0;
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        club = game_state.player_flags.h >> 12;
        if (club < 6) {
            talk_group = 0;
        } else if (club == 6) {
            talk_group = 2;
        } else if (club == 7) {
            talk_group = 4;
        } else {
            talk_group = 6;
        }
        if (!game_state.girls[game_state.current_girl].flags.f.b2) {
            talk_group += 1;
        }
        break;
    case GIRL_MIO:
        talk_group = 0;
        if (!game_state.girls[game_state.current_girl].flags.f.b2) {
            talk_group = 1;
        }
        break;
    case GIRL_YUINA:
        talk_group = 0;
        if (!game_state.girls[game_state.current_girl].flags.f.b2) {
            talk_group = 1;
        }
        if (game_state.girls[game_state.current_girl].flags.f.b19) {
            talk_group = 2;
        }
        break;
    case GIRL_AYAKO:
        if ((game_state.player_flags.h >> 12) == 4) {
            talk_group = 0;
        } else {
            talk_group = 1;
        }
        if (!game_state.girls[game_state.current_girl].flags.f.b2) {
            talk_group += 2;
        }
        break;
    case GIRL_SAKI:
        if ((game_state.player_flags.h >> 12) == 6) {
            talk_group = 0;
        } else {
            talk_group = 1;
        }
        break;
    case GIRL_YUKARI:
        talk_group = 0;
        if (!game_state.girls[game_state.current_girl].flags.f.b2) {
            talk_group = 1;
        }
        break;
    case GIRL_NOZOMI:
        talk_group = 0;
        /* FAKE: the (u8) narrowing spends one ugen temporary that as1 deletes, which gives the
         * original's andi t1 and addiu t3. T-25139 */
        if (((u8) get_g_zyotai_s(GIRL_NOZOMI) & 0x7F) < 2) {
            talk_group += 1;
        }
        break;
    default:
        talk_group = 0;
        break;
    }
    mode_step_next();
}

void camp_club_variant_current_girl_portrait_read(void) {
    u8 girl;
    KangeiWords11 all;
    KangeiWords2 first;
    KangeiWords2 second;
    KangeiWords2 third;

    all = D_80139A08;
    first = D_80139A34;
    second = D_80139A3C;
    third = D_80139A44;
    if ((game_state.month < 6) || (game_state.month >= 10)) {
        all.unk_00[0] = 0x544A;
        all.unk_00[3] = 0x544A;
        all.unk_00[5] = 0x544A;
        first.unk_00[0] = 0x5D41;
        second.unk_00[1] = 0x522E;
        third.unk_00[1] = 0x58AF;
    }
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
    default:
        portrait_read_start(0x6962);
        break;
    }
    mode_step_next();
}
