/* club_tournament_scene.c: ; src/ovl/MASTER/80133680.c:174-182 and 755-770 use tournament-specific actor/resource setup. Original object 0x80133680-0x80138490 (config/objects/MASTER.txt). */
#define MAIN_API_OVERRIDE_normal_date_bg_fadeout /* T-24517: s32 callee view keeps v0 live and matches fade-byte allocation. */
#include "ovl/MASTER.h"

extern s32 normal_date_bg_fadeout(void);

void func_80133680(void) {
    D_8013C4A0 = 0x801E8660;
    D_8013C4A4 = 0x801E8740;
    D_8013C4A8 = 0x801E8AF8;
}

void func_801336B4(void) {
    D_8013C4A0 = 0x801EAA54;
    D_8013C4A4 = 0x801EAE50;
    D_8013C4A8 = 0x801ED1C4;
}

void func_801336E8(void) {
    D_8013C4A0 = 0x801E8A10;
    D_8013C4A4 = 0x801E8B7C;
    D_8013C4A8 = 0x801E93D4;
}

void func_80133724(void) {
    D_8013C4B0 = 0x801EAFC4;
    D_8013C4B4 = 0x801EB144;
    D_8013C4B8 = 0x801EAFF0;
    D_8013C4BC = 0x801EB14C;
    D_8013C4C0 = 0x801EB030;
    D_8013C4C4 = 0x801EB174;
    D_8013C4C8 = *(s16 *)0x801EB17C;
    D_8013C4CC = *(s16 *)0x801EB180;
    D_8013C4D0 = 0x801B4400;
}

void func_801337B8(void) {
    D_8013C4D4 = 0x801B00DC;
    D_8013C4D8 = 0x801B02AC;
    D_8013C4DC = 0x801B03C8;
    D_8013C4E0 = 0x801B04C4;
    D_8013C4E4 = 0x801B0668;
    D_8013C4E8 = 0x801B0808;
    D_8013C4EC = 0x801B09DC;
    D_8013C4F0 = 0x801B0BE8;
    D_8013C4F4 = 0x801B0D8C;
    D_8013C4F8 = 0x801B0F5C;
    D_8013C4FC = 0x801B1164;
    D_8013C500 = 0x801B12D8;
    D_8013C504 = 0x801B1454;
    D_8013C508 = 0x801B00F4;
    D_8013C50C = 0x801B02C8;
    D_8013C510 = 0x801B03D0;
    D_8013C514 = 0x801B04DC;
    D_8013C518 = 0x801B0680;
    D_8013C51C = 0x801B0820;
    D_8013C520 = 0x801B09F8;
    D_8013C524 = 0x801B0C00;
    D_8013C528 = 0x801B0DA4;
    D_8013C52C = 0x801B0F78;
    D_8013C530 = 0x801B1180;
    D_8013C534 = 0x801B12E0;
    D_8013C538 = 0x801B1470;
    D_8013C53C = 0x801B01A8;
    D_8013C540 = 0x801B0398;
    D_8013C544 = 0x801B03F0;
    D_8013C548 = 0x801B0590;
    D_8013C54C = 0x801B0734;
    D_8013C550 = 0x801B08D4;
    D_8013C554 = 0x801B0AE4;
    D_8013C558 = 0x801B0CB4;
    D_8013C55C = 0x801B0E58;
    D_8013C560 = 0x801B1064;
    D_8013C564 = 0x801B126C;
    D_8013C568 = 0x801B1338;
    D_8013C56C = 0x801B155C;
}

void func_80133A2C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl30 tbl;

    tbl = D_8013C5C8;
    idx = game_state.mode_stage;
    tbl.f[idx]();
}

/* FAKE: scalar byte view of dec_bg_brightness preserves its target live range
 * across the case-1 calls; ordinary access selects v0 instead (T-10630). */
void func_80133AA0(void) {
    GameState *g = &game_state;
    s32 i;

    switch (g->mode_step) {
    case 0:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC043, 0xC01D, 0xC982, 0xC935, 0xC91B);
        scene_screen_reset(1, 0);
        talk_clear();
        dec_bg_reset();
        set_dec_bri(0);
        hizuke_init();
        message_window_init();
        parameter_disp_switch(0);
        icon_disp_switch(0);
        k_reset(0);
        message_window_set_title((s32) "甲子園");
        message_window_title_show(0);
        srand(g->timer_ticks);
        for (i = 0; i < 11; i++) {
            D_800CA2B0[i] = g->girls[i].flags.f.met;
        }
        mode_step_next();
        break;
    case 1:
        if (audio_bank_load_poll() == 1) {
            dec_bg_cd_read(0x41A8, 1);
            (*(u8 *)&dec_bg_brightness) = 0;
            mode_step_next();
        }
        if ((u8) (*(u8 *)&dec_bg_brightness) >= 4U) {
            (*(u8 *)&dec_bg_brightness) -= 4;
        } else {
            (*(u8 *)&dec_bg_brightness) = 0;
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            audio_command(0x200);
            message_window_title_show(1);
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(2U, 0x801E8000, 0xA9BA);
            mode_step_next();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_80133680();
            g->current_girl = GIRL_SHIORI;
            talk_names_set(0U);
            text_marker_hide();
            talk_group = 0;
            talk_line = 0;
            talk_text_table = (s32) D_8013C4A0;
            talk_command_table = (s32) D_8013C4A4;
            talk_voice_table = (s32) D_8013C4A8;
            mode_step_next();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        dec_bg_cd_read(0x417A, 1);
        mode_step_next();
        break;
    case 7:
        if (cd_read_sync_status() & 1) {
            dec_bg_decode_to_vram(1);
            mode_step_next();
        }
        break;
    case 8:
        cd_read_sectors(0x1BU, 0x80197000, 0xB03E);
        mode_step_next();
        break;
    case 9:
        normal_date_move_place();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            func_80138F10();
            i = 0;
            do {
                load_palette(D_8013C730, (i + 0x10) & 0xFF, 1, 1, i);
                i += 1;
            } while (i != 5);
            actor_clear(0x60);
            ((ActorFixed *)actor_pool_96)->unk_01 = 3;
            ((ActorFixed *)actor_pool_96)->quad_flags = 0x01000000;
            ((ActorFixed *)actor_pool_96)->unk_07 = 0x80;
            ((ActorFixed *)actor_pool_96)->unk_05 = 0x80;
            ((ActorFixed *)actor_pool_96)->unk_1A = 0x1000;
            ((ActorFixed *)actor_pool_96)->unk_1C = 0x1000;
            ((ActorFixed *)actor_pool_96)->unk_06 = 1;
            ((ActorFixed *)actor_pool_96)->x.h[1] = -0x60;
            ((ActorFixed *)actor_pool_96)->y.h[1] = 0x20;
            ((ActorFixed *)actor_pool_96)->frame_table = (u8 *) D_8013C724;
            ((ActorFixed *)actor_pool_96)->sequence_table = (u8 *) D_8013C728;
            ((ActorFixed *)actor_pool_96)->tpage_table = (u8 *) D_8013C720;
            ((ActorFixed *)actor_pool_96)->unk_14 = D_8013C72C;
            mode_step_next();
        }
        break;
    case 11:
        D_8013C570 = 0;
        ((ClubFlagBits *)&g->clubs[6])->unk_25 += 1;
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}


/* FAKE: the scalar byte view prevents promotion of the probability byte
 * into a conflicting live range. value is reused after the threshold tests
 * as the second shuffle index, preserving the frame layout (T-10630). */
void func_80133F80(void) {
    u8 swap;
    u8 other;
    s32 i;
    s32 swap_count;
    u8 *a;
    s32 first;
    s32 value;
    MasterHalfwords5 thresholds;

    thresholds = D_8013C640;
    switch (game_state.mode_step) {
    case 0:
        talk_group = (D_8013C570 * 3) + 1;
        mode_step_next();
        break;
    case 1:
        normal_date_speak();
        break;
    case 2:
        if ((rand() % 100) == 0) {
            mode_step_set(0xA);
        } else {
            mode_step_next();
        }
        break;
    case 3:
        value = (s16)((game_state.clubs[6].practice_points + ((s16) game_state.param_guts.value >> 4)) - thresholds.v[D_8013C570]);
        if (value < -0x1E) {
            (*(u8 *)&D_8013C578) = 0;
        } else if (value < -0x14) {
            (*(u8 *)&D_8013C578) = 1;
        } else if (value < -0xA) {
            (*(u8 *)&D_8013C578) = 1;
        } else if (value < 0) {
            (*(u8 *)&D_8013C578) = 2;
        } else if (value >= 0x1E) {
            (*(u8 *)&D_8013C578) = 5;
        } else if (value >= 0x14) {
            (*(u8 *)&D_8013C578) = 4;
        } else if (value >= 0xA) {
            (*(u8 *)&D_8013C578) = 4;
        } else if (value >= 0) {
            (*(u8 *)&D_8013C578) = 3;
        }
        for (a = D_8013C57C; a < D_8013C57C + 5; a++) {
            *a = 0;
        }
        for (i = 0; i < (u8)(*(u8 *)&D_8013C578); i++) {
            D_8013C57C[i] = 1;
        }
        i = 0;
        swap_count = rand() & 0xF;
        if ((u8) (*(u8 *)&D_8013C578) >= 3U) {
            swap = D_8013C57C[0];
            other = D_8013C57C[4];
            D_8013C57C[0] = other;
            D_8013C57C[4] = swap;
        }
        if (swap_count > 0) {
            do {

                first = rand() & 3;
                value = rand() & 3;
                swap = D_8013C57C[first];
                i++;
                other = D_8013C57C[value];
                D_8013C57C[first] = other;
                D_8013C57C[value] = swap;
            } while (i != swap_count);
        }
        ((ActorFixed *)actor_pool_96)->unk_43 = ((s32) (D_8013C570 + 2) % 5) + 0x10;
        D_8013C574 = 0;
        mode_step_next();
        break;
    case 4:
        if (((ActorFixed *)actor_pool_96)->flags == 0) {
            if (D_8013C574 >= 5) {
                mode_step_next();
            } else {
                ((ActorFixed *)actor_pool_96)->draw_flags = 0x84;
                ((ActorFixed *)actor_pool_96)->flags = 5;
                ((ActorFixed *)actor_pool_96)->frame = 0;
                ((ActorFixed *)actor_pool_96)->unk_08 = 0;
                if (D_8013C57C[D_8013C574] == 1) {
                    ((ActorFixed *)actor_pool_96)->sequence_slot = 0;
                } else {
                    ((ActorFixed *)actor_pool_96)->sequence_slot = 1;
                }
                D_8013C574 += 1;
            }
        }
        break;
    case 5:
        ((ActorFixed *)actor_pool_96)->draw_flags = 0x84;
        ((ActorFixed *)actor_pool_96)->flags = 1;
        ((ActorFixed *)actor_pool_96)->frame = 0;
        ((ActorFixed *)actor_pool_96)->unk_08 = 0;
        if ((u8) (*(u8 *)&D_8013C578) >= 3U) {
            ((ActorFixed *)actor_pool_96)->sequence_slot = 2;
        } else {
            ((ActorFixed *)actor_pool_96)->sequence_slot = 3;
            if (D_8013C570 == 3) {
                talk_group = 0x17;
            } else {
                talk_group = 0x19;
            }
        }
        if ((u8) (*(u8 *)&D_8013C578) >= 3U) {
            audio_command(0x204);
        } else {
            audio_command(0x205);
        }
        mode_step_next();
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        k_reset(1);
        mode_step_next();
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        ((ActorFixed *)actor_pool_96)->draw_flags = 0;
        ((ActorFixed *)actor_pool_96)->flags = 0;
        mode_stage_next();
        break;
    case 10:
        talk_group = 0x10;
        mode_step_next();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        k_reset(1);
        talk_group += D_8013C570;
        mode_step_next();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        audio_command(0x204);
        talk_group = 0x16;
        mode_step_next();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        (*(u8 *)&D_8013C578) = 3;
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}


void func_801344E0(void) {
    s32 i;
    s32 value;

    switch (game_state.mode_step) {
    case 0:
        if ((u8) D_8013C578 >= 3U) {
            game_state.day_event_done_bits = 0;
            value = D_8013C570 + 1;
            D_8013C570 = value;
            switch (value) {
            case 1:
            case 2:
                break;
            case 3:
                game_state.girls[0].tokimeki = game_state.girls[0].tokimeki + 0xF;
                for (i = 1; i < 13; i++) {
                    game_state.girls[i].tokimeki += 0xF;
                }
                break;
            case 4:
                game_state.girls[0].tokimeki = game_state.girls[0].tokimeki + 3;
                for (i = 1; i < 13; i++) {
                    game_state.girls[i].tokimeki += 3;
                }
                break;
            case 5:
                game_state.girls[0].tokimeki = game_state.girls[0].tokimeki + 4;
                for (i = 1; i < 13; i++) {
                    game_state.girls[i].tokimeki += 4;
                }
                break;
            }
            calendar_advance_day();
            game_state.day_event_index = game_state.day_event_index + 1;
            hizuke_init();
            audio_command(0x200);
            mode_stage_next();
        } else {
            cd_read_sectors(0x6EU, 0x801B4400, 0xBBA5);
            tpage_buf_clear();
            D_800B5BD4 = 0xA;
            D_8013C588 = 0;
            mode_step_next();
        }
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        k_reset(1);
        set_kanji_string(-0x80, 0x32, 0, (s32) game_state.player_family_name, 0);
        k_disp_start(2);
        set_kanji_string(-0x56, 0x32, 0, (s32) "（大会に敗北したくやしさが", 0);
        set_kanji_string(-0x56, 0x42, 0, (s32) "　俺の、眠れる力を", 0);
        set_kanji_string(-0x56, 0x52, 0, (s32) "　呼びおこした！！）", 0);
        mode_step_next();
        break;
    case 3:
        D_8013C588++;
        if ((D_8013C588 >= 0x10U) && (check_end_k() != 0)) {
            ((ActorFixed *)actor_pool)[2].draw_flags = (u8) (((ActorFixed *)actor_pool)[2].draw_flags ^ 0x80);
            D_8013C588 = 0;
        }
        if (game_state.pad_pressed & 0xF0) {
            k_reset(1);
            ((ActorFixed *)actor_pool)[2].draw_flags = (u8) (((ActorFixed *)actor_pool)[2].draw_flags & 0x7F);
            mode_step_next();
        }
        break;
    case 4: {
        if (cd_read_sync_status() & 1) {
            func_80133724();
            load_palette(D_8013C4D0, 0x10, 1, 0, 0);
            load_palette(D_8013C4D0, 0x11, 1, 0, 1);
            actor_clear(0x60);
            ((ActorFixed *)actor_pool_96)->unk_01 = 0xB;
            ((ActorFixed *)actor_pool_96)->flags = 0;
            ((ActorFixed *)actor_pool_96)->quad_flags = 0x01000000;
            ((ActorFixed *)actor_pool_96)->draw_flags = 4;
            ((ActorFixed *)actor_pool_96)->frame_table = (u8 *) D_8013C4B8;
            ((ActorFixed *)actor_pool_96)->sequence_table = (u8 *) D_8013C4C0;
            ((ActorFixed *)actor_pool_96)->tpage_table = (u8 *) D_8013C4B0;
            ((ActorFixed *)actor_pool_96)->unk_14 = D_8013C4C8;
            ((ActorFixed *)actor_pool_96)->sequence_slot = 0;
            ((ActorFixed *)actor_pool_96)->frame = 0;
            ((ActorFixed *)actor_pool_96)->unk_06 = 1;
            ((ActorFixed *)actor_pool_96)->unk_43 = 0;
            ((ActorFixed *)actor_pool_96)->x.h[1] = -0xA0;
            ((ActorFixed *)actor_pool_96)->y.h[1] = -0x78;
            ((ActorFixed *)actor_pool_96)->unk_07 = 0;
            ((ActorFixed *)actor_pool_96)->unk_05 = 0x80;
            ((ActorFixed *)actor_pool_96)->unk_04 = 0;
            /* FAKE: T-25107 (cut down in T-26000): never-true arm that ugen drops.
             * Its indirect store puts both record copies below in block-copy
             * state F, as in the original (one scratch register before the end
             * pointer; see tools/ugen_pass.py). Still needed in T-26002 (state B without it). */
            if ((u32)D_8013C578 < 0) {
                *(u8 *)0 = 0;
            }
            ((ActorFixed *)actor_pool_96)[1] = ((ActorFixed *)actor_pool_96)[0];
            ((ActorFixed *)actor_pool_96)[1].flags = 0x40;
            ((ActorFixed *)actor_pool_96)[1].frame_table = (u8 *)D_8013C4BC;
            ((ActorFixed *)actor_pool_96)[1].sequence_table = (u8 *)D_8013C4C4;
            ((ActorFixed *)actor_pool_96)[1].tpage_table = (u8 *)D_8013C4B4;
            ((ActorFixed *)actor_pool_96)[1].unk_14 = (s16) D_8013C4CC;
            value = rand() % 320;
            ((ActorFixed *)actor_pool_96)[1].x.h[1] = value - 0xA0;
            ((ActorFixed *)actor_pool_96)[1].y.h[1] = 0x3C;
            for (i = 2; i < 20; i++) {
                ((ActorFixed *)actor_pool_96)[i] = ((ActorFixed *)actor_pool_96)[1];
                ((ActorFixed *)actor_pool_96)[i].frame = rand() % 10;
            }
            for (i = 0; i < 10; i++) {
                D_8013C5B4[i] = (rand() % 5) + 1;
                value = rand() % 320;
                D_8013C58C[i].x = value - 0xA0;
                D_8013C58C[i].y = 0x3C;
            }
            hizuke_disp_switch(0);
            message_window_title_show(0);
            D_8013C588 = 0xF;
            mode_step_next();
        }
        break;
    }
    case 5:
        set_kanji_string(-0x80, 0x32, 0, (s32) game_state.player_family_name, 0);
        k_disp_start(2);
        set_kanji_string(-0x56, 0x32, 0, (s32) "（おおおおおおっっっ！！！！）", 0);
        audio_command(0x203);
        mode_step_next();
        break;
    case 6:
        func_80137BF4();
        ((ActorFixed *)actor_pool_96)[0].draw_flags |= 0x80;
        ((ActorFixed *)actor_pool_96)[0].unk_07 += 8;
        for (i = 1; i < 20; i++) {
            ((ActorFixed *)actor_pool_96)[i].draw_flags |= 0x80;
            ((ActorFixed *)actor_pool_96)[i].unk_07 += 8;
        }
        ((ActorFixed *)actor_pool_96)->flags = 5;
        if ((u8) ((ActorFixed *)actor_pool_96)->unk_07 >= 0x80U) {
            ((ActorFixed *)actor_pool_96)->unk_07 = 0x80;
            ((ActorFixed *)actor_pool_96)[1].unk_07 = 0x80U;
            mode_step_next();
        }
        break;
    case 7:
        func_80137BF4();
        D_8013C588++;
        if ((D_8013C588 >= 0x10U) && (check_end_k() != 0)) {
            ((ActorFixed *)actor_pool)[2].draw_flags = (u8) (((ActorFixed *)actor_pool)[2].draw_flags ^ 0x80);
            D_8013C588 = 0;
        }
        if (game_state.pad_pressed & 0xF0) {
            k_reset(1);
            ((ActorFixed *)actor_pool)[2].draw_flags = (u8) (((ActorFixed *)actor_pool)[2].draw_flags & 0x7F);
            ((ActorFixed *)actor_pool_96)->sequence_slot = 2;
            ((ActorFixed *)actor_pool_96)->frame = 0;
            ((ActorFixed *)actor_pool_96)->unk_08 = 0;
            ((ActorFixed *)actor_pool_96)->flags = 5;
            mode_step_next();
        }
        break;
    case 8:
        func_80137BF4();
        k_reset(1);
        set_kanji_string(-0x80, 0x32, 2, (s32) "　　野球部奥義", 0);
        set_kanji_string(-0x80, 0x42, 1, (s32) "　　　「超球百裂弾」", 0);
        set_kanji_string(-0x80, 0x52, 0, (s32) "　　　　　　をマスターした。", 0);
        D_8013C588 = 0;
        mode_step_next();
        /* fallthrough */
    case 9:
        func_80137BF4();
        D_8013C588++;
        if ((D_8013C588 >= 0x10U) && (check_end_k() != 0)) {
            ((ActorFixed *)actor_pool)[2].draw_flags = (u8) (((ActorFixed *)actor_pool)[2].draw_flags ^ 0x80);
            D_8013C588 = 0;
        }
        if (game_state.pad_pressed & 0xF0) {
            k_reset(1);
            ((ActorFixed *)actor_pool)[2].draw_flags = (u8) (((ActorFixed *)actor_pool)[2].draw_flags & 0x7F);
            mode_step_next();
        }
        break;
    case 10:
        func_80137BF4();
        ((ActorFixed *)actor_pool_96)[0].unk_07 -= 8;
        for (i = 1; i < 20; i++) {
            ((ActorFixed *)actor_pool_96)[i].unk_07 -= 8;
        }
        if ((u8) ((ActorFixed *)actor_pool_96)->unk_07 < 9U) {
            ((ActorFixed *)actor_pool_96)[0].unk_07 = 0;
            ((ActorFixed *)actor_pool_96)[0].draw_flags &= ~0x80;
            for (i = 1; i < 20; i++) {
                ((ActorFixed *)actor_pool_96)[i].unk_07 = 0;
                ((ActorFixed *)actor_pool_96)[i].draw_flags &= ~0x80;
            }
            D_800B5BD4 = 0xF;
            sprite_tables_clear();
            mode_step_next();
        }
        break;
    case 11:
        game_state.day_event_done_bits = game_state.day_event_done_bits | 0x10;
        talk_clear();
        set_dec_bri(0);
        day_event_queue_resync_after_master();
        day_event_next(1);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}


void func_80135040(void) {
    if (((game_state.girls[0].birthday.w & 0xF) == game_state.month) && (((u32)(game_state.girls[0].birthday.w << 0x17) >> 0x1B) == game_state.day)) {
        func_80138490();
        return;
    }
    mode_stage_next();
    Default_Disp();
}

void func_801350A8(void) {
    s32 index;
    MasterWords11 text_ids;
    u8 flags;

    text_ids = D_8013C64C;
    portrait_wink_update();
    switch (game_state.mode_step) {
    case 0:
        if ((game_state.month == (game_state.player_birthday & 0xF)) && (game_state.day == ((u32) (game_state.player_birthday << 0x17) >> 0x1B)) && !((flags = game_state.day_event_done_bits) & 4)) {
            audio_command(0xC1);
            flags = game_state.day_event_done_bits;
            flags |= 4;
            game_state.day_event_done_bits = flags;
            mode_step_next();
        } else {
            mode_step_set(0xB);
        }
        break;
    case 1:
        game_state.day_event_index += 1;
        if (girl_state_rank(func_8008647C()) < 2) {
            game_state.current_girl = func_8008647C();
            event_log_add(0x15, 6);
            audio_command(0xBF);
            audio_bank_load_start(1, 0xC043, 0xC01D, D_800B3688[game_state.current_girl], D_800B36C8[game_state.current_girl], D_800B3708[game_state.current_girl]);
            mode_step_next();
        } else {
            mode_step_set(0x10);
        }
        break;
    case 2:
        if (audio_bank_load_poll() == 1) {
            index = game_state.current_girl;
            portrait_read_start(text_ids.v[index]);
            mode_step_next();
        }
        break;
    case 3:
        if ((flags = game_state.day_event_done_bits) & 1) {
            mode_step_next();
        } else {
            normal_date_move_place();
        }
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            portrait_show_checked();
            cd_read_sectors(3U, 0x801B0000, 0xAF43);
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_801337B8();
            talk_names_set(game_state.current_girl);
            talk_group = 0;
            talk_line = 0;
            switch (game_state.current_girl) {
            case GIRL_SHIORI:
                talk_text_table = D_8013C4D8;
                talk_command_table = D_8013C50C;
                talk_voice_table = D_8013C540;
                break;
            case GIRL_MIO:
                talk_text_table = D_8013C4EC;
                talk_command_table = D_8013C520;
                talk_voice_table = D_8013C554;
                break;
            case GIRL_YUINA:
                talk_text_table = D_8013C4E0;
                talk_command_table = D_8013C514;
                talk_voice_table = D_8013C548;
                break;
            case GIRL_AYAKO:
                talk_text_table = D_8013C4E4;
                talk_command_table = D_8013C518;
                talk_voice_table = D_8013C54C;
                break;
            case GIRL_SAKI:
                talk_text_table = D_8013C4FC;
                talk_command_table = D_8013C530;
                talk_voice_table = D_8013C564;
                break;
            case GIRL_YUKARI:
                talk_text_table = D_8013C4F0;
                talk_command_table = D_8013C524;
                talk_voice_table = D_8013C558;
                break;
            case GIRL_NOZOMI:
                talk_text_table = D_8013C4F4;
                talk_command_table = D_8013C528;
                talk_voice_table = D_8013C55C;
                break;
            case GIRL_MIRA:
                talk_text_table = D_8013C4E8;
                talk_command_table = D_8013C51C;
                talk_voice_table = D_8013C550;
                break;
            case GIRL_YUKO:
                talk_text_table = D_8013C4D4;
                talk_command_table = D_8013C508;
                talk_voice_table = D_8013C53C;
                break;
            case GIRL_MEGUMI:
                talk_text_table = D_8013C4F8;
                talk_command_table = D_8013C52C;
                talk_voice_table = D_8013C560;
                break;
            case GIRL_YUMI:
                talk_text_table = D_8013C504;
                talk_command_table = D_8013C538;
                talk_voice_table = D_8013C56C;
                break;
            }
            audio_command(0x200);
            mode_step_next();
        }
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_girl_in();
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        normal_date_girl_out();
        if (game_state.mode_step == 0xA) {
            audio_command(0xC1);
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        if (((flags = game_state.day_event_done_bits) & 1) || ((flags = game_state.day_event_done_bits) & 4)) {
            audio_command(0xBF);
            audio_bank_load_start(1, 0xC043, 0xC01D, 0xC982, 0xC935, 0xC91B);
            mode_step_next();
        } else {
            mode_stage_next();
        }
        break;
    case 12:
        if (audio_bank_load_poll() == 1) {
            cd_read_sectors(0x1BU, 0x80197000, 0xB03E);
            mode_step_next();
        }
        break;
    case 13:
        normal_date_move_place();
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            func_80138F10();
            load_palette(D_8013C730, 0x10, 1, 1, 0);
            cd_read_sectors(2U, 0x801E8000, 0xA9BA);
            mode_step_next();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            func_80133680();
            game_state.current_girl = GIRL_SHIORI;
            talk_names_set(0U);
            talk_group = 0;
            talk_line = 0;
            talk_text_table = (s32) D_8013C4A0;
            talk_command_table = (s32) D_8013C4A4;
            talk_voice_table = (s32) D_8013C4A8;
            mode_stage_next();
        }
        break;
    case 16:
        if ((flags = game_state.day_event_done_bits) & 1) {
            cd_read_sectors(2U, 0x801E8000, 0xA9BA);
            mode_step_next();
        } else {
            talk_group = 0x1B;
            mode_step_set(0x12);
        }
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            func_80133680();
            game_state.current_girl = GIRL_SHIORI;
            talk_names_set(0U);
            talk_group = 0x1B;
            talk_line = 0;
            talk_text_table = (s32) D_8013C4A0;
            talk_command_table = (s32) D_8013C4A4;
            talk_voice_table = (s32) D_8013C4A8;
            mode_step_next();
        }
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        mode_step_set(0xB);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

void func_801358C8(void) {
    switch (game_state.mode_step) {
    case 0:
        cd_read_sectors(0xBU, 0x801E8000, 0xA9C5);
        mode_step_next();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_801336B4();
            talk_text_table = (s32) D_8013C4A0;
            talk_command_table = (s32) D_8013C4A4;
            talk_voice_table = (s32) D_8013C4A8;
            talk_group = 0;
            talk_line = 0;
            mode_step_next();
        }
        break;
    case 2:
        normal_date_speak();
        break;
    case 3:
        talk_group = 5;
        mode_step_next();
        break;
    case 4:
        normal_date_speak();
        break;
    case 5:
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

void func_801359F0(void) {
    s32 pad[2]; /* FAKE: keep the original 0x68-byte frame without
                 * moving the table or call spill (T-10630). */
    s32 selection;
    MasterWords11 messages;

    messages = D_8013C678;
    switch (game_state.mode_step) {
    case 0:
        mode_step_next();
        selection = func_8008647C();
        if ((u8) game_state.girls[12].flags.b[3] >= 0x41U) {
            D_8013C584 = 0xC;
            portrait_read_start(0x57FB);
        } else {
            if (girl_state_rank(selection) < 2) {
                D_8013C584 = (u8) selection;
                if ((((u8) game_state.girls[0].flags.b[2] >> 4) == 6) && (girl_state_rank(0) < 2)) {
                    D_8013C584 = 0;
                }
                if ((((u8) game_state.girls[4].flags.b[2] >> 4) == 6) && (girl_state_rank(4) < 2) && (game_state.girls[4].flags.f.met == 1)) {
                    D_8013C584 = 4;
                }
                portrait_read_start(messages.v[D_8013C584]);
            } else if ((u8) game_state.girls[11].flags.b[3] >= 2U) {
                D_8013C584 = 0xB;
                portrait_read_start(0x6B51);
            } else {
                D_8013C584 = 0xFF;
                mode_step_set(4);
            }
        }
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            audio_bank_load_start(0, 0xC043, 0xC01D, D_800B3688[D_8013C584], D_800B36C8[D_8013C584], D_800B3708[D_8013C584]);
            mode_step_next();
        }
        break;
    case 2:
        if (audio_bank_load_poll() == 1) {
            cd_read_sectors(0xBU, 0x801E8000, 0xA9C5);
            mode_step_next();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_801336B4();
            talk_text_table = (s32) D_8013C4A0;
            talk_command_table = (s32) D_8013C4A4;
            talk_voice_table = (s32) D_8013C4A8;
            talk_group = 0;
            talk_line = 0;
            mode_stage_next();
        }
        break;
    case 4:
        mode_stage_next();
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

/* FAKE: the byte views of D_8013C584 keep each read in v1, and `idx` declared above the table puts
 * the table at sp+0x28, as in func_80137044 (T-18600). */
void func_80135CF4(void) {
    s32 idx; /* declared before the table: its stack slot sits above it */
    MasterHalfwords13 values;

    values = D_8013C6A4;
    portrait_wink_update();
    switch (game_state.mode_step) {
    case 0:
        if (cd_read_sync_status() & 1) {
            talk_names_set((*(u8 *)&D_8013C584));
            idx = (*(u8 *)&D_8013C584);
            talk_group = values.v[idx];
            if (((*(u8 *)&D_8013C584) == 4) && (((u16) game_state.player_flags.h >> 0xC) == 6)) {
                talk_group = 0x3C;
            }
            talk_line = 0;
            addr_init_bustup();
            game_state.current_girl = (*(u8 *)&D_8013C584);
            portrait_show_checked();
            if ((*(u8 *)&D_8013C584) == 0xC) {
                portrait_layout_set(0U);
            }
        }
        break;
    case 1:
        normal_date_move_place();
        break;
    case 2:
        if ((*(u8 *)&D_8013C584) == 0xB) {
            audio_command(0x500);
            func_80047550();
        }
        audio_bank_start(0, 0);
        mode_step_next();
        break;
    case 3:
        normal_date_speak();
        break;
    case 4:
        normal_date_girl_in();
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        if ((*(u8 *)&D_8013C584) == 0xB) {
            if (((u32) (game_state.girls[0xB].flags.w << 0x14) >> 0x1D) == 0) {
                talk_group = 0x1E;
            } else {
                talk_group = 0x1F;
            }
            normal_date_girl_out();
        } else if ((*(u8 *)&D_8013C584) == 0xC) {
            talk_group = 0x20;
            mode_step_next();
        } else {
            talk_group = 0x1D;
            mode_step_next();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if ((*(u8 *)&D_8013C584) == 0xB) {
            mode_step_next();
        } else {
            normal_date_girl_out();
        }
        break;
    case 9:
        normal_date_move_place();
        break;
    case 10:
        audio_bank_start(1, 0);
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

/* FAKE: scalar probability-byte views suppress a promoted live range,
 * retained from the matched twin func_80133F80 (T-10630). T-24514, T-24517 */

void func_80135FB8(void) {
    u8 swap;
    u8 other;
    s32 i;
    s32 swap_count;
    u8 *a;
    s32 first;
    s32 value;

    switch (game_state.mode_step) {
    case 0:
        value = (s16)(((s16) game_state.param_guts.value >> 4) - (0x140 - game_state.clubs[6].practice_points));
        if (value < -0x1E) {
            (*(u8 *)&D_8013C578) = 0;
        } else if (value < -0x14) {
            (*(u8 *)&D_8013C578) = 1;
        } else if (value < -0xA) {
            (*(u8 *)&D_8013C578) = 1;
        } else if (value < 0) {
            (*(u8 *)&D_8013C578) = 2;
        } else if (value >= 0x1E) {
            (*(u8 *)&D_8013C578) = 5;
        } else if (value >= 0x14) {
            (*(u8 *)&D_8013C578) = 4;
        } else if (value >= 0xA) {
            (*(u8 *)&D_8013C578) = 4;
        } else if (value >= 0) {
            (*(u8 *)&D_8013C578) = 3;
        }
        for (i = 0; i < 5; i++) {
            D_8013C57C[i] = 0;
        }
        for (i = 0; i < (u8)(*(u8 *)&D_8013C578); i++) {
            D_8013C57C[i] = 1;
        }
        i = 0;
        swap_count = rand() & 0xF;
        if ((u8) (*(u8 *)&D_8013C578) >= 3U) {
            swap = D_8013C57C[0];
            other = D_8013C57C[4];
            D_8013C57C[0] = other;
            D_8013C57C[4] = swap;
        }
        a = D_8013C57C;
        if (swap_count > 0) {
            do {
                first = rand() & 3;
                value = rand() & 3;
                swap = a[first];
                i++;
                other = a[value];
                a[first] = other;
                a[value] = swap;
            } while (i != swap_count);
        }
        ((ActorFixed *)actor_pool_96)->unk_43 = 0x12;
        D_8013C574 = 0;
        mode_step_next();
        break;
    case 1:
        if (((ActorFixed *)actor_pool_96)->flags == 0) {
            if (D_8013C574 >= 5) {
                mode_step_next();
            } else {
                ((ActorFixed *)actor_pool_96)->draw_flags = 0x84;
                ((ActorFixed *)actor_pool_96)->flags = 5;
                ((ActorFixed *)actor_pool_96)->frame = 0;
                ((ActorFixed *)actor_pool_96)->unk_08 = 0;
                if (D_8013C57C[D_8013C574] == 1) {
                    ((ActorFixed *)actor_pool_96)->sequence_slot = 0;
                } else {
                    ((ActorFixed *)actor_pool_96)->sequence_slot = 1;
                }
                D_8013C574 += 1;
            }
        }
        break;
    case 2:
        ((ActorFixed *)actor_pool_96)->draw_flags = 0x84;
        ((ActorFixed *)actor_pool_96)->flags = 1;
        ((ActorFixed *)actor_pool_96)->frame = 0;
        ((ActorFixed *)actor_pool_96)->unk_08 = 0;
        if ((u8) (*(u8 *)&D_8013C578) >= 3U) {
            audio_command(0x204);
            ((ActorFixed *)actor_pool_96)->sequence_slot = 2;
            talk_group = 0xE;
        } else {
            audio_command(0x205);
            ((ActorFixed *)actor_pool_96)->sequence_slot = 3;
            talk_group = 0xF;
        }
        mode_step_next();
        break;
    case 3:
        normal_date_speak();
        break;
    case 4:
        if ((u8) (*(u8 *)&D_8013C578) >= 3U) {
            talk_group = 0x16;
        } else {
            talk_group = 0x1B;
        }
        mode_step_next();
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        ((ActorFixed *)actor_pool_96)->unk_07 -= 4;
        if (((ActorFixed *)actor_pool_96)->unk_07 < 4U) {
            ((ActorFixed *)actor_pool_96)->unk_07 = 4;
        }
        break;
    case 7:
        ((ActorFixed *)actor_pool_96)->draw_flags = 0;
        ((ActorFixed *)actor_pool_96)->flags = 0;
        mode_stage_next();
        if (((ClubFlagBits *)&game_state.clubs[6] )->unk_25 < 3U) {
            mode_stage_next();
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}


void func_801363F4(void) {
    s32 i;
    s32 value;

    switch (game_state.mode_step) {
    case 0:
        audio_command(0xC1);
        cd_read_sectors(0x6EU, 0x801B4400, 0xBBA5);
        tpage_buf_clear();
        D_800B5BD4 = 0xA;
        D_8013C588 = 0;
        mode_step_next();
        break;
    case 1:
        k_reset(1);
        set_kanji_string(-0x80, 0x32, 0, (s32) game_state.player_family_name, 0);
        k_disp_start(2);
        if ((u8) D_8013C578 >= 3U) {
            set_kanji_string(-0x56, 0x32, 0, (s32) "（大会に勝利した興奮が", 0);
            set_kanji_string(-0x56, 0x42, 0, (s32) "　俺の体に", 0);
            set_kanji_string(-0x56, 0x52, 0, (s32) "　新たな力を与えた！！）", 0);
        } else {
            set_kanji_string(-0x56, 0x32, 0, (s32) "（大会に敗北したくやしさが", 0);
            set_kanji_string(-0x56, 0x42, 0, (s32) "　俺の、眠れる力を", 0);
            set_kanji_string(-0x56, 0x52, 0, (s32) "　呼びおこした！！）", 0);
        }
        mode_step_next();
        break;
    case 2:
        D_8013C588++;
        if ((D_8013C588 >= 0x10U) && (check_end_k() != 0)) {
            ((ActorFixed *)actor_pool)[2].draw_flags = (u8) (((ActorFixed *)actor_pool)[2].draw_flags ^ 0x80);
            D_8013C588 = 0;
        }
        if ((game_state.pad_pressed & 0xF0) && (check_end_k() != 0)) {
            k_reset(1);
            ((ActorFixed *)actor_pool)[2].draw_flags = (u8) (((ActorFixed *)actor_pool)[2].draw_flags & 0x7F);
            mode_step_next();
        }
        break;
    case 3:
        mode_step_next();
        break;
    case 4: {
        if (cd_read_sync_status() & 1) {
            func_80133724();
            load_palette(D_8013C4D0, 0x10, 1, 0, 0);
            load_palette(D_8013C4D0, 0x11, 1, 0, 1);
            actor_clear(0x60);
            ((ActorFixed *)actor_pool_96)->unk_01 = 0xB;
            ((ActorFixed *)actor_pool_96)->flags = 0;
            ((ActorFixed *)actor_pool_96)->quad_flags = 0x01000000;
            ((ActorFixed *)actor_pool_96)->draw_flags = 4;
            ((ActorFixed *)actor_pool_96)->frame_table = (u8 *) D_8013C4B8;
            ((ActorFixed *)actor_pool_96)->sequence_table = (u8 *) D_8013C4C0;
            ((ActorFixed *)actor_pool_96)->tpage_table = (u8 *) D_8013C4B0;
            ((ActorFixed *)actor_pool_96)->unk_14 = D_8013C4C8;
            ((ActorFixed *)actor_pool_96)->sequence_slot = 0;
            ((ActorFixed *)actor_pool_96)->frame = 0;
            ((ActorFixed *)actor_pool_96)->unk_06 = 1;
            ((ActorFixed *)actor_pool_96)->unk_43 = 0;
            ((ActorFixed *)actor_pool_96)->x.h[1] = -0xA0;
            ((ActorFixed *)actor_pool_96)->y.h[1] = -0x78;
            ((ActorFixed *)actor_pool_96)->unk_07 = 0;
            ((ActorFixed *)actor_pool_96)->unk_05 = 0x80;
            ((ActorFixed *)actor_pool_96)->unk_04 = 0;
            /* FAKE: T-25107 (cut down in T-26000): never-true arm that ugen drops.
             * Its indirect store puts both record copies below in block-copy
             * state F, as in the original (one scratch register before the end
             * pointer; see tools/ugen_pass.py). Still needed in T-26002 (state B without it). */
            if ((u32)D_8013C578 < 0) {
                *(u8 *)0 = 0;
            }
            ((ActorFixed *)actor_pool_96)[1] = ((ActorFixed *)actor_pool_96)[0];
            ((ActorFixed *)actor_pool_96)[1].flags = 0x40;
            ((ActorFixed *)actor_pool_96)[1].frame_table = (u8 *)D_8013C4BC;
            ((ActorFixed *)actor_pool_96)[1].sequence_table = (u8 *)D_8013C4C4;
            ((ActorFixed *)actor_pool_96)[1].tpage_table = (u8 *)D_8013C4B4;
            ((ActorFixed *)actor_pool_96)[1].unk_14 = (s16) D_8013C4CC;
            value = rand() % 320;
            ((ActorFixed *)actor_pool_96)[1].x.h[1] = value - 0xA0;
            ((ActorFixed *)actor_pool_96)[1].y.h[1] = 0x3C;
            for (i = 2; i < 20; i++) {
                ((ActorFixed *)actor_pool_96)[i] = ((ActorFixed *)actor_pool_96)[1];
                ((ActorFixed *)actor_pool_96)[i].frame = rand() % 10;
            }
            for (i = 0; i < 10; i++) {
                D_8013C5B4[i] = (rand() % 5) + 1;
                value = rand() % 320;
                D_8013C58C[i].x = value - 0xA0;
                D_8013C58C[i].y = 0x3C;
            }
            hizuke_disp_switch(0);
            message_window_title_show(0);
            D_8013C588 = 0xF;
            mode_step_next();
        }
        break;
    }
    case 5:
        set_kanji_string(-0x80, 0x32, 0, (s32) game_state.player_family_name, 0);
        k_disp_start(2);
        set_kanji_string(-0x56, 0x32, 0, (s32) "（おおおおおおっっっ！！！！）", 0);
        audio_command(0x203);
        mode_step_next();
        break;
    case 6:
        func_80137BF4();
        ((ActorFixed *)actor_pool_96)[0].draw_flags |= 0x80;
        ((ActorFixed *)actor_pool_96)[0].unk_07 += 8;
        for (i = 1; i < 20; i++) {
            ((ActorFixed *)actor_pool_96)[i].draw_flags |= 0x80;
            ((ActorFixed *)actor_pool_96)[i].unk_07 += 8;
        }
        ((ActorFixed *)actor_pool_96)->flags = 5;
        if ((u8) ((ActorFixed *)actor_pool_96)->unk_07 >= 0x80U) {
            ((ActorFixed *)actor_pool_96)->unk_07 = 0x80;
            ((ActorFixed *)actor_pool_96)[1].unk_07 = 0x80U;
            mode_step_next();
        }
        break;
    case 7:
        func_80137BF4();
        D_8013C588++;
        if ((D_8013C588 >= 0x10U) && (check_end_k() != 0)) {
            ((ActorFixed *)actor_pool)[2].draw_flags = (u8) (((ActorFixed *)actor_pool)[2].draw_flags ^ 0x80);
            D_8013C588 = 0;
        }
        if ((game_state.pad_pressed & 0xF0) && check_end_k() != 0) {
            k_reset(1);
            ((ActorFixed *)actor_pool)[2].draw_flags = (u8) (((ActorFixed *)actor_pool)[2].draw_flags & 0x7F);
            ((ActorFixed *)actor_pool_96)->sequence_slot = 2;
            ((ActorFixed *)actor_pool_96)->frame = 0;
            ((ActorFixed *)actor_pool_96)->unk_08 = 0;
            ((ActorFixed *)actor_pool_96)->flags = 5;
            mode_step_next();
        }
        break;
    case 8:
        func_80137BF4();
        k_reset(1);
        set_kanji_string(-0x80, 0x32, 2, (s32) "　　野球部奥義", 0);
        set_kanji_string(-0x80, 0x42, 1, (s32) "　　　「超球百裂弾」", 0);
        set_kanji_string(-0x80, 0x52, 0, (s32) "　　　　　　をマスターした。", 0);
        D_8013C588 = 0;
        mode_step_next();
        /* fallthrough */
    case 9:
        func_80137BF4();
        D_8013C588++;
        if ((D_8013C588 >= 0x10U) && (check_end_k() != 0)) {
            ((ActorFixed *)actor_pool)[2].draw_flags = (u8) (((ActorFixed *)actor_pool)[2].draw_flags ^ 0x80);
            D_8013C588 = 0;
        }
        if ((game_state.pad_pressed & 0xF0) && check_end_k() != 0) {
            k_reset(1);
            ((ActorFixed *)actor_pool)[2].draw_flags = (u8) (((ActorFixed *)actor_pool)[2].draw_flags & 0x7F);
            mode_step_next();
        }
        break;
    case 10:
        func_80137BF4();
        ((ActorFixed *)actor_pool_96)[0].unk_07 -= 8;
        for (i = 1; i < 20; i++) {
            ((ActorFixed *)actor_pool_96)[i].unk_07 -= 8;
        }
        if ((u8) ((ActorFixed *)actor_pool_96)->unk_07 < 9U) {
            ((ActorFixed *)actor_pool_96)[0].unk_07 = 0;
            ((ActorFixed *)actor_pool_96)[0].draw_flags &= ~0x80;
            for (i = 1; i < 20; i++) {
                ((ActorFixed *)actor_pool_96)[i].unk_07 = 0;
                ((ActorFixed *)actor_pool_96)[i].draw_flags &= ~0x80;
            }
            D_800B5BD4 = 0xF;
            sprite_tables_clear();
            audio_command(0xC1);
            mode_step_next();
        }
        break;
    case 11:
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

void func_80136E24(void) {
    s32 i;

    switch (game_state.mode_step) {
    case 0:
        if (((ClubFlagBits *)&game_state.clubs[6])->unk_25 < 3U) {
            mode_step_set(2);
        } else {
            cd_read_sectors(0xBU, 0x801E8000, 0xA9C5);
            mode_step_next();
        }
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_801336B4();
            talk_text_table = (s32) D_8013C4A0;
            talk_command_table = (s32) D_8013C4A4;
            talk_voice_table = (s32) D_8013C4A8;
            talk_group = 0;
            talk_line = 0;
            mode_step_next();
        }
        break;
    case 2:
        if ((u8) D_8013C578 >= 3U) {
            game_state.girls[0].tokimeki += 5;
            for (i = 1; i < 13; i++) {
                game_state.girls[i].tokimeki += 5;
            }
            ((ClubFlagBits *)&game_state.clubs[6])->b30 = 1;
        }
        if (D_8013C584 == 0xFF) {
            mode_stage_next();
            mode_stage_next();
            mode_stage_next();
        } else {
            game_state.girls[D_8013C584].affection += 2;
            audio_command(0x200);
            if ((u8) D_8013C578 >= 3U) {
                mode_stage_next();
            } else {
                mode_stage_next();
                mode_stage_next();
            }
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

/* FAKE: scalar byte views preserve the target live ranges, and
 * the cached girls pointer preserves the copied table offset (T-10630).
 * The sprite aggregate keeps flag loads after neighboring field stores. */
void func_80137044(void) {
    GirlRecord *girls;
    u8 index;
    MasterHalfwords13 messages;

    messages = D_8013C6C0;
    portrait_wink_update();
    switch (game_state.mode_step) {
    case 0:
        portrait_show_checked();
        switch ((*(u8 *)&D_8013C584)) {
        case 0:
            portrait_layout_set(0U);
            break;
        case 6:
            portrait_layout_set(1U);
            break;
        case 7:
            portrait_layout_set(1U);
            break;
        case 9:
            portrait_layout_set(5U);
            sprite_quads[0].unk_0C = 7;
            sprite_quads[0].unk_0E = 0x70;
            break;
        case 12:
            portrait_layout_set(0U);
            break;
        }
        cd_read_sectors(0xEU, 0x80197000, 0xA9D0);
        dec_bg_show_switch(1);
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            girls = game_state.girls;
            func_80137F94();
            index = (*(u8 *)&D_8013C584);
            if ((index == 4) && (((u16) game_state.player_flags.h >> 0xC) != ((u8) girls[4].flags.b[2] >> 4))) {
                talk_group = 0x40;
            } else if (index == 6) {
                talk_group = 0x4B;
            } else {
                talk_group = messages.v[index];
            }
            if (index == 9) {
                ((ActorFixed *)actor_pool_96)->draw_flags |= 0x80;
                ((ActorFixed *)actor_pool_96)->sequence_slot = 8;
            }
            mode_step_next();
        }
        break;
    case 2:
        if ((*(u8 *)&D_8013C584) == 0xB) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        if ((*(u8 *)&D_8013C584) == 9) {
            (*(u8 *)&((ActorFixed *)actor_pool_96)->unk_07) += 4;
            if ((*(u8 *)&((ActorFixed *)actor_pool_96)->unk_07) >= 0x7DU) {
                (*(u8 *)&((ActorFixed *)actor_pool_96)->unk_07) = 0x80;
            }
        }
        if (((*(u8 *)&D_8013C584) != 0xB) && (game_state.mode_step == 3)) {
            audio_bank_start(0, 0);
        }
        break;
    case 3:
        normal_date_speak();
        break;
    case 4:
        switch ((*(u8 *)&D_8013C584)) {
        case 0:
            if (talk_group == 0x2A) {
                portrait_layout_set(1U);
                func_80063668();
                mode_step_set(3);
            } else {
                talk_group = 0x21;
                mode_step_next();
            }
            break;
        case 1:
            talk_group = 0x21;
            mode_step_next();
            break;
        case 2:
            talk_group = 0x21;
            mode_step_next();
            break;
        case 3:
            talk_group = 0x21;
            mode_step_next();
            break;
        case 4:
            if (talk_group == 0x3F) {
                ((ActorFixed *)actor_pool_96)->sequence_slot = 4;
                ((ActorFixed *)actor_pool_96)->draw_flags |= 0x80;
                (*(u8 *)&((ActorFixed *)actor_pool_96)->unk_07) = 0x80;
                portrait_layout_set(5U);
                sprite_quads[0].unk_0C = 7;
                sprite_quads[0].unk_0E = 0x70;
                func_80063668();
                mode_step_set(3);
            } else {
                talk_group = 0x21;
                mode_step_next();
            }
            break;
        case 5:
            talk_group = 0x21;
            mode_step_next();
            break;
        case 6:
            if (talk_group == 0x4C) {
                portrait_layout_set(0U);
                func_80063668();
                mode_step_set(3);
            } else {
                talk_group = 0x21;
                mode_step_next();
            }
            break;
        case 7:
            if (talk_group == 0x52) {
                portrait_layout_set(0U);
                func_80063668();
                mode_step_set(3);
            } else {
                talk_group = 0x21;
                mode_step_next();
            }
            break;
        case 8:
            talk_group = 0x21;
            mode_step_next();
            break;
        case 9:
            talk_group = 0x21;
            mode_step_next();
            break;
        case 10:
            talk_group = 0x21;
            mode_step_next();
            break;
        case 11:
            mode_step_set(6);
            break;
        case 12:
            talk_group = 0x23;
            mode_step_next();
            break;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        if ((*(u8 *)&D_8013C584) == 0xB) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        if ((u8) (*(u8 *)&((ActorFixed *)actor_pool_96)->unk_07) < 4U) {
            (*(u8 *)&((ActorFixed *)actor_pool_96)->unk_07) = 0;
        } else {
            (*(u8 *)&((ActorFixed *)actor_pool_96)->unk_07) -= 4;
        }
        break;
    case 7:
        func_80138088();
        mode_stage_next();
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}


/* FAKE: scalar byte views preserve the target live ranges, and
 * the constant value local preserves the copied table offset (T-10630).
 * The sprite aggregate keeps flag loads after neighboring field stores. */
void func_801375E8(void) {
    s32 value;
    u8 index;
    MasterHalfwords13 messages;

    messages = D_8013C6DC;
    portrait_wink_update();
    switch (game_state.mode_step) {
    case 0:
        portrait_show_checked();
        portrait_layout_set(5U);
        if ((*(u8 *)&D_8013C584) == 0xC) {
            portrait_layout_set(0U);
        }
        dec_bg_show_switch(1);
        cd_read_sectors(0xEU, 0x80197000, 0xA9D0);
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_80137F94();
            index = (*(u8 *)&D_8013C584);
            talk_group = messages.v[index];
            talk_line = 0;
            if (index == 9) {
                ((ActorFixed *)actor_pool_96)->draw_flags |= 0x80;
                value = 8;
                ((ActorFixed *)actor_pool_96)->sequence_slot = value;
            }
            if (index == 0xA) {
                ((ActorFixed *)actor_pool_96)->draw_flags |= 0x80;
                ((ActorFixed *)actor_pool_96)->sequence_slot = 0xA;
            }
            mode_step_next();
        }
        break;
    case 2:
        if ((*(u8 *)&D_8013C584) == 0xB) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        if (((*(u8 *)&D_8013C584) == 0xA) || ((*(u8 *)&D_8013C584) == 9)) {
            if ((u8) (*(u8 *)&((ActorFixed *)actor_pool_96)->unk_07) >= 0x81U) {
                (*(u8 *)&((ActorFixed *)actor_pool_96)->unk_07) = 0x80;
            } else {
                (*(u8 *)&((ActorFixed *)actor_pool_96)->unk_07) += 4;
            }
        }
        if (((*(u8 *)&D_8013C584) != 0xB) && (game_state.mode_step == 3)) {
            audio_bank_start(0, 0);
        }
        break;
    case 3:
    case 5:
        normal_date_speak();
        break;
    case 4:
        switch ((*(u8 *)&D_8013C584)) {
        case 0:
            if (talk_group == 0x2C) {
                ((ActorFixed *)actor_pool_96)->sequence_slot = 0;
                ((ActorFixed *)actor_pool_96)->draw_flags |= 0x80;
                (*(u8 *)&((ActorFixed *)actor_pool_96)->unk_07) = 0x80;
                mode_step_set(3);
            } else {
                talk_group = 0x24;
                mode_step_next();
            }
            break;
        case 1:
            if (talk_group == 0x31) {
                ((ActorFixed *)actor_pool_96)->sequence_slot = 2;
                ((ActorFixed *)actor_pool_96)->draw_flags |= 0x80;
                (*(u8 *)&((ActorFixed *)actor_pool_96)->unk_07) = 0x80;
                mode_step_set(3);
            } else {
                talk_group = 0x24;
                mode_step_next();
            }
            break;
        case 2:
            talk_group = 0x24;
            mode_step_next();
            break;
        case 3:
            talk_group = 0x24;
            mode_step_next();
            break;
        case 4:
            if (talk_group == 0x42) {
                ((ActorFixed *)actor_pool_96)->sequence_slot = 4;
                ((ActorFixed *)actor_pool_96)->draw_flags |= 0x80;
                (*(u8 *)&((ActorFixed *)actor_pool_96)->unk_07) = 0x80;
                mode_step_set(3);
            } else {
                talk_group = 0x24;
                mode_step_next();
            }
            break;
        case 5:
            if (talk_group == 0x47) {
                portrait_layout_set(2U);
                func_80063668();
                mode_step_set(3);
            } else {
                talk_group = 0x24;
                mode_step_next();
            }
            break;
        case 6:
            if (talk_group == 0x4E) {
                ((ActorFixed *)actor_pool_96)->draw_flags |= 0x80;
                ((ActorFixed *)actor_pool_96)->sequence_slot = 6;
                (*(u8 *)&((ActorFixed *)actor_pool_96)->unk_07) = 0x80;
                mode_step_set(3);
            } else {
                talk_group = 0x24;
                mode_step_next();
            }
            break;
        case 7:
            talk_group = 0x24;
            mode_step_next();
            break;
        case 8:
            talk_group = 0x24;
            mode_step_next();
            break;
        case 9:
            talk_group = 0x24;
            mode_step_next();
            break;
        case 10:
            talk_group = 0x24;
            mode_step_next();
            break;
        case 11:
            mode_step_set(6);
            break;
        case 12:
            talk_group = 0x26;
            mode_step_next();
            break;
        }
        break;
    case 6:
        if ((*(u8 *)&D_8013C584) == 0xB) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        if ((u8) (*(u8 *)&((ActorFixed *)actor_pool_96)->unk_07) < 4U) {
            (*(u8 *)&((ActorFixed *)actor_pool_96)->unk_07) = 0;
        } else {
            (*(u8 *)&((ActorFixed *)actor_pool_96)->unk_07) -= 4;
        }
        break;
    case 7:
        func_80138374();
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}


void func_80137B5C(void) {
    if (game_state.mode_step == 0) {
        audio_command(0xBF);
        func_800674B0();
        mode_step_next();
    } else if (audio_bank_load_poll() == 1) {
        actor_clear(0x60);
        game_state.day_event_done_bits |= 0x10;
        talk_clear();
        set_dec_bri(0);
        day_event_queue_resync_after_master();
        day_event_next(1);
    }
    Default_Disp();
}

void func_80137BF4(void) {
    MasterPoint2 *temp_s0_2;
    s16 *temp_s1;
    /* FAKE: wide X local plus a narrow third argument preserves the
     * original sign-extension sequence. T-10630. */
    s32 temp_a0;
    s16 temp_a1;
    s16 temp_v0;
    s16 temp_v0_2;
    s16 var_s2;
    s32 temp_hi;
    ActorFixed *temp_s0;

    if ((((ActorFixed *)actor_pool_96)->sequence_slot == 0) && (((ActorFixed *)actor_pool_96)->frame == 7) && (((ActorFixed *)actor_pool_96)->unk_08 == 2)) {
        ((ActorFixed *)actor_pool_96)->sequence_slot = 1;
        ((ActorFixed *)actor_pool_96)->frame = 0;
        ((ActorFixed *)actor_pool_96)->unk_08 = 0;
        ((ActorFixed *)actor_pool_96)->flags = 1;
    }
    var_s2 = 0x61;
    if ((((ActorFixed *)actor_pool_96)->sequence_slot == 2) && (((ActorFixed *)actor_pool_96)->frame == 1) && (((ActorFixed *)actor_pool_96)->unk_08 == 4)) {
        ((ActorFixed *)actor_pool_96)->sequence_slot = 3;
        ((ActorFixed *)actor_pool_96)->frame = 0;
        ((ActorFixed *)actor_pool_96)->unk_08 = 0;
        ((ActorFixed *)actor_pool_96)->flags = 1;
    }
    do {
        temp_s0 = &((ActorFixed *)actor_pool)[var_s2];
        temp_v0 = temp_s0->frame;
        if ((temp_v0 < 2) || (temp_v0 >= 8)) {
            temp_s0->y.w = temp_s0->y.w - ((((ActorFixed *)actor_pool_96)->sequence_slot == 3) * 0x50000) - 0x50000;
        } else if ((temp_v0 == 4) || (temp_v0 == 7)) {
            temp_s0->y.w = temp_s0->y.w - ((((ActorFixed *)actor_pool_96)->sequence_slot == 3) * 0x30000) - 0x30000;
        } else {
            temp_s0->y.w = temp_s0->y.w - ((((ActorFixed *)actor_pool_96)->sequence_slot == 3) * 0x20000) - 0x20000;
        }
        if (temp_s0->y.h[1] < -0x9F) {
            temp_hi = rand() % 320;
            temp_s0->x.h[1] = (s16) (temp_hi - 0xA0);
            temp_s0->y.h[1] = 0x3C;
            temp_s0->frame = (s16) (rand() % 10);
        }
        var_s2 += 1;
    } while (var_s2 < 0x74);
    var_s2 = 0;
    if ((u8) ((ActorFixed *)actor_pool_96)->unk_07 >= 0x78U) {
        do {
            temp_s0_2 = &D_8013C58C[var_s2];
            temp_s1 = &D_8013C5B4[var_s2];
            temp_a0 = temp_s0_2->x;
            temp_v0_2 = *temp_s1;
            temp_s0_2->y -= temp_v0_2 * 4 * ((((ActorFixed *)actor_pool_96)->sequence_slot == 3) + 1);
            temp_a1 = temp_s0_2->y;
            prim_gradient_line((s32) temp_a0, (s32) temp_a1, (s16) temp_a0, (s32) (s16) ((temp_v0_2 * 0x14) + temp_a1), 0xFFFFFF, 0xFFFFFF, 0xB);
            if (temp_s0_2->y < -0xAA) {
                temp_s0_2->y = 0x3C;
                temp_s0_2->x = (rand() % 320) - 0xA0;
                *temp_s1 = (rand() % 5) + 1;
            }
            var_s2 += 1;
        } while (var_s2 < 0xA);
    }
}

void func_80137F94(void) {
    func_80138EC0();
    actor_clear(0x60);
    D_80120651 = 3;
    D_80120688 = 0x01000000;
    D_80120657 = 0;
    D_80120655 = 0x80;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
    D_80120693 = 0x10;
    D_80120676 = 0;
    D_8012067A = -0x20;
    D_8012065C = (u8 *)D_8013C714;
    D_80120660 = (u8 *)D_8013C718;
    D_80120684 = (u8 *)D_8013C710;
    D_80120664 = D_8013C71C;
    D_80120652 = 1;
    D_80120653 = 4;
    D_80120668 = 0;
}

void func_80138088(void) {
    game_state.event_log[game_state.event_log_count].girl = game_state.current_girl;
    game_state.event_log[game_state.event_log_count].scene = 0x15;
    game_state.event_log[game_state.event_log_count].flag = 6;
    game_state.event_log[game_state.event_log_count].year = game_state.year;
    game_state.event_log[game_state.event_log_count].month = game_state.month;
    game_state.event_log[game_state.event_log_count].day = game_state.day;
    game_state.event_log[game_state.event_log_count].weekday = game_state.weekday;
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        game_state.event_log[game_state.event_log_count].rank = 1;
        break;
    case GIRL_MIO:
        game_state.event_log[game_state.event_log_count].rank = 1;
        break;
    case GIRL_YUINA:
        game_state.event_log[game_state.event_log_count].rank = 1;
        break;
    case GIRL_AYAKO:
        game_state.event_log[game_state.event_log_count].rank = 1;
        break;
    case GIRL_SAKI:
        if ((game_state.girls[4].flags.b[2] >> 4) == 6) {
            game_state.event_log[game_state.event_log_count].rank = 5;
            game_state.event_log[game_state.event_log_count].weekday = 0xFF;
        } else {
            game_state.event_log[game_state.event_log_count].rank = 1;
        }
        break;
    case GIRL_YUKARI:
        game_state.event_log[game_state.event_log_count].rank = 1;
        break;
    case GIRL_NOZOMI:
        game_state.event_log[game_state.event_log_count].rank = 0;
        break;
    case GIRL_MIRA:
        game_state.event_log[game_state.event_log_count].rank = 0;
        break;
    case GIRL_YUKO:
        game_state.event_log[game_state.event_log_count].rank = 1;
        break;
    case GIRL_MEGUMI:
        game_state.event_log[game_state.event_log_count].rank = 5;
        game_state.event_log[game_state.event_log_count].weekday = 0xFF;
        break;
    case GIRL_YUMI:
        game_state.event_log[game_state.event_log_count].rank = 1;
        break;
    case GIRL_MIHARU:
        game_state.event_log[game_state.event_log_count].rank = 1;
        break;
    case GIRL_REI:
        game_state.event_log[game_state.event_log_count].rank = 1;
        break;
    }
    if ((game_state.event_log_count = game_state.event_log_count + 1) >= 0xFF) {
        game_state.event_log_count = 0xFE;
    }
}

void func_80138374(void) {
    game_state.event_log[game_state.event_log_count].girl = game_state.current_girl;
    game_state.event_log[game_state.event_log_count].scene = 0x15;
    game_state.event_log[game_state.event_log_count].flag = 6;
    game_state.event_log[game_state.event_log_count].rank = 5;
    game_state.event_log[game_state.event_log_count].year = game_state.year;
    game_state.event_log[game_state.event_log_count].month = game_state.month;
    game_state.event_log[game_state.event_log_count].day = game_state.day;
    game_state.event_log[game_state.event_log_count].weekday = game_state.weekday;
    game_state.event_log_count += 1;
    if (game_state.event_log_count >= 0xFFU) {
        game_state.event_log_count = 0xFE;
    }
}
