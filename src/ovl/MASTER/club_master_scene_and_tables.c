/* club_master_scene_and_tables.c: ; src/ovl/MASTER/80139000.c:4-13 defines the scene resource table; spec documents the club-master scene flow. Original object 0x80139000-0x8013B9B0 (config/objects/MASTER.txt). */
#include "ovl/MASTER.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/club_master_scene_and_tables.data", D_8013C760);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/club_master_scene_and_tables.data", D_8013C764);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/club_master_scene_and_tables.data", D_8013C768);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/club_master_scene_and_tables.data", D_8013C76C);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/club_master_scene_and_tables.data", D_8013C770);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/club_master_scene_and_tables.data", D_8013C780);
u8 D_8013C784 = 0;
u8 D_8013C788[5] = { 0 };
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/club_master_scene_and_tables.data", D_8013C790);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/club_master_scene_and_tables.data", D_8013C794);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/club_master_scene_and_tables.data", D_8013C798);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/club_master_scene_and_tables.data", D_8013C7B4);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/club_master_scene_and_tables.data", D_8013C7E8);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/club_master_scene_and_tables.data", D_8013C804);
INCLUDE_RODATA("asm/ovl/MASTER/data/MASTER/club_master_scene_and_tables.data", D_8013C820);

void func_80139000(void) {
    D_8013C760 = 0x801A42D4;
    D_8013C764 = 0x801A42E0;
    D_8013C768 = 0x801A4330;
    D_8013C76C = *(s16 *)0x801A4338;
    D_8013C770 = 0x80197000;
}

void func_80139054(void) {
    D_8013C760 = 0x801A3E3C;
    D_8013C764 = 0x801A3E48;
    D_8013C768 = 0x801A3E90;
    D_8013C76C = *(s16 *)0x801A3E98;
    D_8013C770 = 0x80197000;
}

void func_801390A8(void) {
    D_8013C760 = 0x801A42E4;
    D_8013C764 = 0x801A42F0;
    D_8013C768 = 0x801A4368;
    D_8013C76C = *(s16 *)0x801A4370;
    D_8013C770 = 0x80197000;
}

void func_801390FC(void) {
    D_8013C760 = 0x801A432C;
    D_8013C764 = 0x801A4338;
    D_8013C768 = 0x801A4394;
    D_8013C76C = *(s16 *)0x801A43A4;
    D_8013C770 = 0x80197000;
}

void func_80139150(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl7 tbl;

    tbl = D_8013C798;
    idx = game_state.mode_stage;
    tbl.f[idx]();
}

void func_801391E4(void) {
    s32 sum1;
    s32 sum2;
    s32 best;
    s32 i;

    portrait_wink_update();
    switch (game_state.mode_step) {
    case 0:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC043, 0xC01D, 0xC982, 0xC935, 0xC91B);
        scene_screen_reset(1, 0);
        dec_bg_reset();
        set_dec_bri(0);
        message_window_init();
        k_reset(0);
        message_window_set_title((s32) "　");
        message_window_title_show(0);
        hizuke_init();
        parameter_disp_switch(0);
        icon_disp_switch(0);
        talk_clear();
        srand(game_state.timer_ticks);
        mode_step_next();
        break;
    case 1:
        if (audio_bank_load_poll() == 1) {
            k_reset(0);
            switch ((u16) game_state.player_flags.h >> 0xC) {
            case 7:
                message_window_set_title((s32) "国立競技場");
                audio_command(0x207);
                dec_bg_cd_read(0x41B1, 1);
                break;
            case 8:
                message_window_set_title((s32) "インターハイ");
                audio_command(0x209);
                dec_bg_cd_read(0x41BA, 1);
                break;
            case 9:
                message_window_set_title((s32) "インターハイ");
                audio_command(0x208);
                dec_bg_cd_read(0x41BA, 1);
                break;
            case 10:
                message_window_set_title((s32) "インターハイ");
                audio_command(0x201);
                dec_bg_cd_read(0x41BA, 1);
                break;
            default:
                game_mode_reset_to_title();
                break;
            }
            message_window_title_show(0);
            mode_step_next();
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            best = func_8008647C();
            if ((u8) game_state.girls[0xC].flags.b[3] >= 0x41U) {
                D_8013C790 = 0xC;
            } else if (girl_state_rank(best) < 2) {
                D_8013C790 = (u8) best;
                for (i = 0; i < 0xB; i++) {
                    if ((((u16) game_state.player_flags.h >> 0xC) == ((u8) game_state.girls[i].flags.b[2] >> 4)) && (((u32) (game_state.girls[i].flags.w << 0x1E) >> 0x1F) == 1)) {
                        if (girl_state_rank(i) < 2) {
                            D_8013C790 = (u8) i;
                        }
                        if ((((u8) game_state.girls[0].flags.b[2] >> 4) == 0xA) && (((u16) game_state.player_flags.h >> 0xC) == 0xA)) {
                            if ((girl_state_rank(0) < 2) && (girl_state_rank(0xA) >= 2)) {
                                D_8013C790 = 0;
                            } else if ((girl_state_rank(0) >= 2) && (girl_state_rank(0xA) < 2)) {
                                D_8013C790 = 0xA;
                            } else if ((girl_state_rank(0) < 2) && (girl_state_rank(0xA) < 2)) {
                                sum1 = game_state.girls[0].affection + game_state.girls[0].tokimeki;
                                sum2 = game_state.girls[0xA].affection + game_state.girls[0xA].tokimeki;
                                if (sum1 >= sum2) {
                                    D_8013C790 = 0;
                                } else {
                                    D_8013C790 = 0xA;
                                }
                            }
                        }
                    }
                }
            } else if ((u8) game_state.girls[0xB].flags.b[3] >= 2U) {
                D_8013C790 = 0xB;
            } else {
                D_8013C790 = 0xFF;
            }
            if (D_8013C790 != 0xFF) {
                audio_bank_load_start(0, 0xBF98, 0xBF79, D_800B3688[D_8013C790], D_800B36C8[D_8013C790], D_800B3708[D_8013C790]);
            }
            mode_step_next();
        }
        break;
    case 3:
        if ((audio_bank_load_poll() == 1) || (D_8013C790 == 0xFF)) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0xBU, 0x801E8000, 0xA9C5);
            message_window_title_show(1);
            mode_step_next();
        }
        break;
    case 4:
        normal_date_bg_fadein();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            talk_names_set(0U);
            text_marker_hide();
            func_80138FC0();
            talk_text_table = (s32) D_8013C750;
            talk_command_table = (s32) D_8013C754;
            talk_voice_table = (s32) D_8013C758;
            switch ((u16) game_state.player_flags.h >> 0xC) {
            case 7:
                talk_group = 1;
                break;
            case 8:
                talk_group = 2;
                break;
            case 9:
                talk_group = 3;
                break;
            case 10:
                talk_group = 4;
                break;
            default:
                game_mode_reset_to_title();
                break;
            }
            talk_line = 0;
            mode_step_next();
        }
        break;
    case 6:
    case 8:
        normal_date_speak();
        break;
    case 7:
        talk_group = (D_8013C794 = rand() % 6) + 6;
        mode_step_next();
        break;
    case 9:
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

void func_80139810(void) {
    s32 pad1; /* FAKE: two unused locals declared before tbl reproduce the original frame (0x68); T-8020 */
    s32 pad2;
    MasterWords13 tbl;

    tbl = D_8013C7B4;
    switch (game_state.mode_step) {
    case 0:
        if (D_8013C790 != 0xFF) {
            portrait_read_start(tbl.v[D_8013C790]);
            mode_stage_next();
        } else {
            D_8013C790 = 0xFF;
            mode_step_next();
        }
        break;
    case 1:
        normal_date_bg_fadeout();
        break;
    case 2:
        mode_stage_set(3);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

/* FAKE: the scalar byte view preserves the selector live range (T-13001).
 * The thirteen-halfword copy is demonstrated by the original accesses. */
void func_80139910(void) {
    u32 temp_t9;
    MasterHalfwords13 table;

    table = D_8013C7E8;
    portrait_wink_update();
    switch (game_state.mode_step) {
    case 0:
        if (cd_read_sync_status() & 1) {
            talk_names_set((*(u8 *)&D_8013C790));
            talk_group = table.v[(*(u8 *)&D_8013C790)];
            talk_line = 0;
            addr_init_bustup();
            game_state.current_girl = (*(u8 *)&D_8013C790);
            portrait_show_checked();
            if ((*(u8 *)&D_8013C790) == 0xC) {
                portrait_layout_set(0U);
            }
            if (((u16) game_state.player_flags.h >> 0xC) != 7) {
                dec_bg_cd_read(0x41C3, 1);
            }
        }
        break;
    case 1:
        if ((cd_read_sync_status() & 1) || (((u16) game_state.player_flags.h >> 0xC) == 7)) {
            if (((u16) game_state.player_flags.h >> 0xC) != 7) {
                k_reset(0);
                message_window_set_title("選手控え室");
                D_80125CA0 = 0xC0;
            }
            dec_bg_decode_to_vram(1);
            mode_step_next();
        }
        break;
    case 2:
        if (((u16) game_state.player_flags.h >> 0xC) == 7) {
            mode_step_next();
        } else {
            normal_date_move_place();
        }
        if (((*(u8 *)&D_8013C790) == 0xB) && (game_state.mode_step == 3)) {
            audio_command(0x500);
            func_80047550();
        }
        break;
    case 3:
        normal_date_speak();
        if (game_state.mode_step == 4) {
            audio_bank_start(0, 0);
        }
        break;
    case 5:
    case 7:
        normal_date_speak();
        break;
    case 4:
        normal_date_girl_in();
        break;
    case 6:
        if ((*(u8 *)&D_8013C790) == 0xB) {
            if (((u32) (game_state.girls[11].flags.w << 0x14) >> 0x1D) == 0) {
                talk_group = 0x1E;
            } else {
                talk_group = 0x1F;
            }
            normal_date_girl_out();
        } else if ((*(u8 *)&D_8013C790) == 0xC) {
            talk_group = 0x20;
            mode_step_next();
        } else {
            talk_group = 0x1D;
            mode_step_next();
        }
        break;
    case 8:
        if ((*(u8 *)&D_8013C790) == 0xB) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 9:
        temp_t9 = (u16) game_state.player_flags.h >> 0xC;
        switch (temp_t9) {
        case 7:
            audio_bank_start(1, 7);
            break;
        case 8:
            audio_bank_start(1, 9);
            break;
        case 9:
            audio_bank_start(1, 8);
            break;
        case 10:
            audio_bank_start(1, 1);
            break;
        }
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}


/* FAKE: scalar byte views on D_8013C780 and D_8013C784 preserve the target live ranges,
 * and value is reused as the second shuffle index to preserve the frame layout (T-10630, T-24102). */
void func_80139CC8(void) {
    u8 swap;
    u8 other;
    s32 value;
    s32 first;
    u8 *a;
    s32 i;
    s32 swap_count;

    switch (game_state.mode_step) {
    case 0:
        switch ((u16) game_state.player_flags.h >> 0xC) {
        case 7:
            dec_bg_cd_read(0x4183, 1);
            break;
        case 8:
            dec_bg_cd_read(0x419E, 1);
            break;
        case 9:
            dec_bg_cd_read(0x4195, 1);
            break;
        case 10:
            dec_bg_cd_read(0x418D, 1);
            break;
        default:
            game_mode_reset_to_title();
            break;
        }
        k_reset(0);
        message_window_set_title((s32) "決勝戦");
        D_80125CA0 = 0xC0;
        mode_step_next();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            switch ((u16) game_state.player_flags.h >> 0xC) {
            case 7:
                cd_read_sectors(0x1AU, 0x80197000, 0xAFE7);
                break;
            case 8:
                cd_read_sectors(0x1BU, 0x80197000, 0xB01C);
                break;
            case 9:
                cd_read_sectors(0x1BU, 0x80197000, 0xB001);
                break;
            case 10:
                cd_read_sectors(0x1BU, 0x80197000, 0xAFCC);
                break;
            default:
                game_mode_reset_to_title();
                break;
            }
            mode_step_next();
        }
        break;
    case 2:
        normal_date_bg_fadein();
        break;
    case 3:
        talk_group = 0xC;
        mode_step_next();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            switch ((u16) game_state.player_flags.h >> 0xC) {
            case 7:
                func_80139054();
                break;
            case 8:
                func_801390FC();
                break;
            case 9:
                func_801390A8();
                break;
            case 10:
                func_80139000();
                break;
            default:
                game_mode_reset_to_title();
                break;
            }
            for (i = 0; i < 5; i++) {
                load_palette(D_8013C770, (i + 0x10) & 0xFF, 1, 1, i);
            }
            mode_step_next();
        }
        break;
    case 5:
    case 11:
    case 13:
        normal_date_speak();
        break;
    case 6:
        value = (s16)(game_state.clubs[(u16)game_state.player_flags.h >> 0xC].practice_points - 350);
        if (value < -0x1E) {
            (*(u8 *)&D_8013C784) = 0;
        } else if (value < -0x14) {
            (*(u8 *)&D_8013C784) = 1;
        } else if (value < -0xA) {
            (*(u8 *)&D_8013C784) = 1;
        } else if (value < 0) {
            (*(u8 *)&D_8013C784) = 2;
        } else if (value >= 0x1E) {
            (*(u8 *)&D_8013C784) = 5;
        } else if (value >= 0x14) {
            (*(u8 *)&D_8013C784) = 4;
        } else if (value >= 0xA) {
            (*(u8 *)&D_8013C784) = 4;
        } else if (value >= 0) {
            (*(u8 *)&D_8013C784) = 3;
        }
        mode_step_next();
        break;
    case 7:
        for (a = D_8013C788; a < D_8013C788 + 5; a++) {
            *a = 0;
        }
        for (i = 0; i < (u8)(*(u8 *)&D_8013C784); i++) {
            D_8013C788[i] = 1;
        }
        i = 0;
        swap_count = rand() & 0xF;
        if ((u8)(*(u8 *)&D_8013C784) >= 3U) {
            swap = D_8013C788[0];
            other = D_8013C788[4];
            D_8013C788[0] = other;
            D_8013C788[4] = swap;
        }
        if (swap_count > 0) {
            do {
                first = rand() & 3;
                value = rand() & 3;
                swap = D_8013C788[first];
                i++;
                other = D_8013C788[value];
                D_8013C788[first] = other;
                D_8013C788[value] = swap;
            } while (i != swap_count);
        }
        if (((u16)game_state.player_flags.h >> 0xC) == 9) {
            if ((u8)(*(u8 *)&D_8013C784) >= 3U) {
                D_8013C788[0] = 1;
            } else {
                D_8013C788[0] = 0;
            }
        }
        mode_step_next();
        break;
    case 8:
        actor_clear(0x60);
        ((ActorFixed *)actor_pool_96)->unk_01 = 3;
        ((ActorFixed *)actor_pool_96)->quad_flags = 0x01000000;
        ((ActorFixed *)actor_pool_96)->unk_05 = 0x80;
        ((ActorFixed *)actor_pool_96)->unk_1A = 0x1000;
        ((ActorFixed *)actor_pool_96)->unk_1C = 0x1000;
        ((ActorFixed *)actor_pool_96)->unk_06 = 1;
        ((ActorFixed *)actor_pool_96)->unk_07 = 0x80;
        ((ActorFixed *)actor_pool_96)->unk_43 = (rand() % 5) + 0x10;
        ((ActorFixed *)actor_pool_96)->x.h[1] = -0x60;
        ((ActorFixed *)actor_pool_96)->y.h[1] = 0x20;
        ((ActorFixed *)actor_pool_96)->frame_table = (u8 *) D_8013C764;
        ((ActorFixed *)actor_pool_96)->sequence_table = (u8 *) D_8013C768;
        ((ActorFixed *)actor_pool_96)->tpage_table = (u8 *) D_8013C760;
        ((ActorFixed *)actor_pool_96)->unk_14 = D_8013C76C;
        D_8013C780 = 0;
        mode_step_next();
        break;
    case 9:
        if (((ActorFixed *)actor_pool_96)->flags == 0) {
            if (((*(u8 *)&D_8013C780) >= 5U) || ((*(u8 *)&D_8013C780) && (((u16) game_state.player_flags.h >> 0xC) == 9))) {
                mode_step_next();
            } else {
                ((ActorFixed *)actor_pool_96)->draw_flags = 0x84;
                ((ActorFixed *)actor_pool_96)->flags = 5;
                ((ActorFixed *)actor_pool_96)->frame = 0;
                ((ActorFixed *)actor_pool_96)->unk_08 = 0;
                if (D_8013C788[(*(u8 *)&D_8013C780)] == 1) {
                    ((ActorFixed *)actor_pool_96)->sequence_slot = 0;
                } else {
                    ((ActorFixed *)actor_pool_96)->sequence_slot = 1;
                }
                (*(u8 *)&D_8013C780) += 1;
            }
        }
        break;
    case 10:
        if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
            talk_group = 0xD;
            audio_command(0x204);
        } else {
            talk_group = D_8013C794 + 0x10;
            audio_command(0x205);
        }
        mode_step_next();
        break;
    case 12:
        switch ((u16) game_state.player_flags.h >> 0xC) {
        case 7:
            if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
                talk_group = 0x17;
            } else {
                talk_group = 0x66;
            }
            break;
        case 8:
            if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
                talk_group = 0x18;
            } else {
                talk_group = 0x67;
            }
            break;
        case 9:
            if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
                talk_group = 0x19;
            } else {
                talk_group = 0x68;
            }
            break;
        case 10:
            if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
                talk_group = 0x1A;
            } else {
                talk_group = 0x69;
            }
            break;
        default:
            game_mode_reset_to_title();
            break;
        }
        mode_step_next();
        break;
    case 14:
        if (((ActorFixed *)actor_pool_96)->unk_07 < 4U) {
            ((ActorFixed *)actor_pool_96)->unk_07 = 0;
        } else {
            ((ActorFixed *)actor_pool_96)->unk_07 -= 4;
        }
        normal_date_bg_fadeout();
        break;
    case 15:
        actor_clear(0x60);
        if (D_8013C790 == 0xFF) {
            if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
                game_state.girls[0].tokimeki += 0x19;
                for (i = 1; i < 13; i++) {
                    game_state.girls[i].tokimeki += 0x19;
                }
            } else {
                game_state.girls[0].tokimeki += 0x12;
                for (i = 1; i < 13; i++) {
                    game_state.girls[i].tokimeki += 0x12;
                }
            }
            mode_stage_set(6);
        } else {
            k_reset(0);
            if (((u16) game_state.player_flags.h >> 0xC) == 7) {
                message_window_set_title((s32) "国立競技場");
            } else {
                message_window_set_title((s32) "選手控え室");
            }
            D_80125CA0 = 0xC0;
            game_state.girls[D_8013C790].affection += 2;
            if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
                game_state.girls[0].tokimeki += 0x19;
                for (i = 1; i < 13; i++) {
                    game_state.girls[i].tokimeki += 0x19;
                }
                mode_stage_next();
            } else {
                game_state.girls[0].tokimeki += 0x12;
                for (i = 1; i < 13; i++) {
                    game_state.girls[i].tokimeki += 0x12;
                }
                mode_stage_set(5);
            }
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

/* FAKE: scalar byte views preserve the selector live ranges; the cached
 * girls pointer reproduces the copied-table stack offset (T-13001).
 * The sprite aggregate keeps flag loads after neighboring field stores. */
void func_8013A784(void) {
    GirlRecord *girls;
    u8 index;
    MasterHalfwords13 table;

    table = D_8013C804;
    portrait_wink_update();
    switch (game_state.mode_step) {
    case 0:
        portrait_show_checked();
        switch ((*(u8 *)&D_8013C790)) {
        case 0:
            portrait_layout_set(0U);
        default:
            break;
        case 6:
            if (((u16) game_state.player_flags.h >> 0xC) != 9) {
                portrait_layout_set(1U);
            }
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
            func_8013B354();
            index = (*(u8 *)&D_8013C790);
            if ((index == 4) && (((u16) game_state.player_flags.h >> 0xC) != ((u8) girls[4].flags.b[2] >> 4))) {
                talk_group = 0x40;
            } else if ((index == 6) && (((u16) game_state.player_flags.h >> 0xC) != 9)) {
                talk_group = 0x4B;
            } else {
                talk_group = table.v[index];
            }
            if (index == 9) {
                ((ActorFixed *)actor_pool_96)->draw_flags |= 0x80;
                ((ActorFixed *)actor_pool_96)->sequence_slot = 9;
            }
            mode_step_next();
        }
        break;
    case 2:
        if ((*(u8 *)&D_8013C790) == 0xB) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        if ((*(u8 *)&D_8013C790) == 9) {
            ((ActorFixed *)actor_pool_96)->unk_07 += 4;
            if (((ActorFixed *)actor_pool_96)->unk_07 >= 0x7DU) {
                ((ActorFixed *)actor_pool_96)->unk_07 = 0x80;
            }
        }
        if (((*(u8 *)&D_8013C790) != 0xB) && (game_state.mode_step == 3)) {
            audio_bank_start(0, 0);
        }
        break;
    case 3:
        normal_date_speak();
        break;
    case 4:
        switch ((*(u8 *)&D_8013C790)) {
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
                ((ActorFixed *)actor_pool_96)->sequence_slot = 5;
                ((ActorFixed *)actor_pool_96)->draw_flags |= 0x80;
                ((ActorFixed *)actor_pool_96)->unk_07 = 0x80;
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
        func_8013B448();
        mode_stage_set(6);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}


/* FAKE: scalar byte views preserve live ranges; the constant value local
 * reproduces the copied-table stack offset (T-13001).
 * The sprite aggregate keeps flag loads after neighboring field stores. */
void func_8013ACF0(void) {
    s32 value;
    u8 index;
    MasterHalfwords13 table;

    table = D_8013C820;
    portrait_wink_update();
    switch (game_state.mode_step) {
    case 0:
        portrait_show_checked();
        portrait_layout_set(5U);
        if ((*(u8 *)&D_8013C790) == 0xC) {
            portrait_layout_set(0U);
        }
        dec_bg_show_switch(1);
        cd_read_sectors(0xEU, 0x80197000, 0xA9D0);
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_8013B354();
            index = (*(u8 *)&D_8013C790);
            talk_group = table.v[index];
            talk_line = 0;
            if (index == 9) {
                ((ActorFixed *)actor_pool_96)->draw_flags |= 0x80;
                value = 9;
                ((ActorFixed *)actor_pool_96)->sequence_slot = value;
            }
            if (index == 0xA) {
                ((ActorFixed *)actor_pool_96)->draw_flags |= 0x80;
                ((ActorFixed *)actor_pool_96)->sequence_slot = 0xB;
            }
            mode_step_next();
        }
        break;
    case 2:
        if ((*(u8 *)&D_8013C790) == 0xB) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        if (((*(u8 *)&D_8013C790) == 0xA) || ((*(u8 *)&D_8013C790) == 9)) {
            if ((u8) (*(u8 *)&((ActorFixed *)actor_pool_96)->unk_07) >= 0x81U) {
                (*(u8 *)&((ActorFixed *)actor_pool_96)->unk_07) = 0x80;
            } else {
                (*(u8 *)&((ActorFixed *)actor_pool_96)->unk_07) += 4;
            }
        }
        if (((*(u8 *)&D_8013C790) != 0xB) && (game_state.mode_step == 3)) {
            audio_bank_start(0, 0);
        }
        break;
    case 3:
    case 5:
        normal_date_speak();
        break;
    case 4:
        switch ((*(u8 *)&D_8013C790)) {
        case 0:
            if (talk_group == 0x2C) {
                ((ActorFixed *)actor_pool_96)->sequence_slot = 1;
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
                ((ActorFixed *)actor_pool_96)->sequence_slot = 3;
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
                ((ActorFixed *)actor_pool_96)->sequence_slot = 5;
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
                ((ActorFixed *)actor_pool_96)->sequence_slot = 7;
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
        func_8013B804();
        mode_stage_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}


s32 func_8013B20C(void) {
    switch (game_state.mode_step) {
    case 0:
        if (D_80120657 < 4U) {
            D_80120657 = 0;
        } else {
            D_80120657 -= 4;
        }
        if (D_8013C790 == 0xB) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 1:
        audio_command(0xBF);
        func_800674B0();
        mode_step_next();
        break;
    case 2:
        if (audio_bank_load_poll() == 1) {
            game_state.day_event_done_bits |= 0x10;
            talk_clear();
            if (D_8013C784 >= 3U) {
                ((ClubFlagBits *)&game_state.clubs[(u32)game_state.player_flags.h >> 0xC])->b30 = 1;
            }
            set_dec_bri(0);
            day_event_next(1);
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
}

void func_8013B354(void) {
    func_80138F70();
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
    D_8012065C = (u8 *)D_8013C744;
    D_80120660 = (u8 *)D_8013C748;
    D_80120684 = (u8 *)D_8013C740;
    D_80120664 = D_8013C74C;
    D_80120652 = 1;
    D_80120653 = 4;
    D_80120668 = 0;
}

void func_8013B448(void) {
    GameState *g = &game_state;

    g->event_log[g->event_log_count].girl = g->current_girl;
    g->event_log[g->event_log_count].scene = 0x15;
    g->event_log[g->event_log_count].year = g->year;
    g->event_log[g->event_log_count].month = g->month;
    g->event_log[g->event_log_count].day = g->day;
    g->event_log[g->event_log_count].weekday = g->weekday;
    switch ((u16) g->player_flags.h >> 0xC) {
    case 7:
        g->event_log[g->event_log_count].flag = 7;
        break;
    case 8:
        g->event_log[g->event_log_count].flag = 8;
        break;
    case 9:
        g->event_log[g->event_log_count].flag = 9;
        break;
    case 10:
        g->event_log[g->event_log_count].flag = 0xA;
        break;
    }
    switch (g->current_girl) {
    case GIRL_SHIORI:
        g->event_log[g->event_log_count].rank = 1;
        break;
    case GIRL_MIO:
        g->event_log[g->event_log_count].rank = 1;
        break;
    case GIRL_YUINA:
        g->event_log[g->event_log_count].rank = 1;
        break;
    case GIRL_AYAKO:
        g->event_log[g->event_log_count].rank = 1;
        break;
    case GIRL_SAKI:
        if (((g->girls[4].flags.b[2] >> 4) == 7) && (((u16) g->player_flags.h >> 0xC) == 7)) {
            g->event_log[g->event_log_count].rank = 5;
            g->event_log[g->event_log_count].weekday = 0xFF;
        } else {
            g->event_log[g->event_log_count].rank = 1;
        }
        break;
    case GIRL_YUKARI:
        g->event_log[g->event_log_count].rank = 1;
        break;
    case GIRL_NOZOMI:
        if (((u16) g->player_flags.h >> 0xC) == 9) {
            g->event_log[g->event_log_count].rank = 1;
        } else {
            g->event_log[g->event_log_count].rank = 0;
        }
        break;
    case GIRL_MIRA:
        g->event_log[g->event_log_count].rank = 0;
        break;
    case GIRL_YUKO:
        g->event_log[g->event_log_count].rank = 1;
        break;
    case GIRL_MEGUMI:
        g->event_log[g->event_log_count].rank = 5;
        g->event_log[g->event_log_count].weekday = 0xFF;
        break;
    case GIRL_YUMI:
        g->event_log[g->event_log_count].rank = 1;
        break;
    case GIRL_MIHARU:
        g->event_log[g->event_log_count].rank = 1;
        break;
    case GIRL_REI:
        g->event_log[g->event_log_count].rank = 1;
        break;
    }
    g->event_log_count++;
    if (g->event_log_count >= 0xFFU) {
        g->event_log_count = 0xFE;
    }
}

void func_8013B804(void) {
    GameState *g = &game_state;

    switch ((u16) g->player_flags.h >> 0xC) {
    case 7:
        g->event_log[g->event_log_count].flag = 7;
        break;
    case 8:
        g->event_log[g->event_log_count].flag = 8;
        break;
    case 9:
        g->event_log[g->event_log_count].flag = 9;
        break;
    case 10:
        g->event_log[g->event_log_count].flag = 0xA;
        break;
    }
    g->event_log[g->event_log_count].girl = g->current_girl;
    g->event_log[g->event_log_count].scene = 0x15;
    g->event_log[g->event_log_count].rank = 5;
    g->event_log[g->event_log_count].year = g->year;
    g->event_log[g->event_log_count].month = g->month;
    g->event_log[g->event_log_count].day = g->day;
    g->event_log[g->event_log_count].weekday = g->weekday;
    g->event_log_count++;
    if (g->event_log_count >= 0xFFU) {
        g->event_log_count = 0xFE;
    }
}
