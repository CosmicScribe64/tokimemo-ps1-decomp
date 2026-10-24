/* birthday_scene.c: src/ovl/BUNKASAI/80146FA0.c:4: birthday table setup, phase 9 scene, choice name. Original object 0x80146FA0-0x80148040 (config/objects/BUNKASAI.txt). */
#include "common.h"
#include "ovl/BUNKASAI.h"

void func_80146FA0(void) {
    D_80160640 = 0x801B00DC;
    D_80160644 = 0x801B02AC;
    D_80160648 = 0x801B03C8;
    D_8016064C = 0x801B04C4;
    D_80160650 = 0x801B0668;
    D_80160654 = 0x801B0808;
    D_80160658 = 0x801B09DC;
    D_8016065C = 0x801B0BE8;
    D_80160660 = 0x801B0D8C;
    D_80160664 = 0x801B0F5C;
    D_80160668 = 0x801B1164;
    D_8016066C = 0x801B12D8;
    D_80160670 = 0x801B1454;
    D_80160674 = 0x801B00F4;
    D_80160678 = 0x801B02C8;
    D_8016067C = 0x801B03D0;
    D_80160680 = 0x801B04DC;
    D_80160684 = 0x801B0680;
    D_80160688 = 0x801B0820;
    D_8016068C = 0x801B09F8;
    D_80160690 = 0x801B0C00;
    D_80160694 = 0x801B0DA4;
    D_80160698 = 0x801B0F78;
    D_8016069C = 0x801B1180;
    D_801606A0 = 0x801B12E0;
    D_801606A4 = 0x801B1470;
    D_801606A8 = 0x801B01A8;
    D_801606AC = 0x801B0398;
    D_801606B0 = 0x801B03F0;
    D_801606B4 = 0x801B0590;
    D_801606B8 = 0x801B0734;
    D_801606BC = 0x801B08D4;
    D_801606C0 = 0x801B0AE4;
    D_801606C4 = 0x801B0CB4;
    D_801606C8 = 0x801B0E58;
    D_801606CC = 0x801B1064;
    D_801606D0 = 0x801B126C;
    D_801606D4 = 0x801B1338;
    D_801606D8 = 0x801B155C;
}

void func_80147214(void) {
    u32 temp_t1;
    u32 temp_v0;
    u32 temp_v0_2;

    switch (game_state.mode_step) {
    case 0x0:
        k_reset(0);
        message_window_set_title((s32) "　");
        message_window_title_show(0);
        set_dec_bri(0);
        game_state.day_event_done_bits |= 1;
        audio_command(0xCF);
        Vblnk_Timer_Init();
        cd_read_sectors(3U, 0x801E8000, 0x49B7);
        mode_step_next();
        break;
    case 0x1:
        if (Vblnk_Timer() >= 0x15) {
            if (cd_read_sync_status() & 1) {
                audio_bank_load_start(1, 0xC478, 0xC44B, 0xCA95, 0xCA4F, 0xCA3E);
                game_state.current_girl = GIRL_SHIORI;
                talk_names_set(0U);
                func_80132444();
                talk_group = 0;
                talk_line = 0;
                talk_text_table = (s32) D_801604F0;
                talk_command_table = (s32) D_801604F4;
                talk_voice_table = (s32) D_801604F8;
                mode_step_next();
            }
        }
        break;
    case 0x2:
        if (((game_state.player_birthday & 0xF) == game_state.month) && (((u32) (game_state.player_birthday << 0x17) >> 0x1B) == game_state.day) && (girl_state_rank(0) < 2) && (D_80122EBC == 0)) {
            mode_step_set(0x24);
        } else {
            dec_bg_reset();
            set_dec_bri(0);
            mode_step_next();
        }
        break;
    case 0x3:
        normal_date_speak();
        break;
    case 0x4:
        normal_date_two_select();
        break;
    case 0x5:
        if (D_80122CDC != 0) {
            normal_date_speak();
        } else {
            mode_step_set(7);
        }
        break;
    case 0x6:
        game_state.girls[0].affection -= 1;
        game_state.girls[0].syoushin += 5;
        func_80142BC8();
        break;
    case 0x7:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            switch ((u8) game_state.girls[0].flags.b[2] >> 4) {
            case 0:
                if (game_state.year == 0x61) {
                    event_log_add(0x14, 0x1C);
                    dec_bg_cd_read(0x40F4, 1);
                } else {
                    event_log_add(0x14, 0x1B);
                    dec_bg_cd_read(0x40EA, 1);
                }
                break;
            case 1:
                event_log_add(0x14, 0x1D);
                dec_bg_cd_read(0x40F4, 1);
                break;
            case 5:
                event_log_add(0x14, 0x22);
                dec_bg_cd_read(0x40F4, 1);
                break;
            case 2:
                event_log_add(0x14, 0x1E);
                dec_bg_cd_read(0x40FC, 1);
                break;
            case 3:
                event_log_add(0x14, 0x1F);
                dec_bg_cd_read(0x4105, 1);
                break;
            case 4:
                if (game_state.year == 0x60) {
                    event_log_add(0x14, 0x21);
                    dec_bg_cd_read(0x4118, 1);
                } else {
                    event_log_add(0x14, 0x20);
                    dec_bg_cd_read(0x410F, 1);
                }
                break;
            default:
                dec_bg_cd_read(0x403B, 1);
                break;
            }
            talk_group = 2;
            mode_step_next();
        }
        break;
    case 0x8:
        normal_date_speak();
        break;
    case 0x9:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            temp_t1 = (u8) game_state.girls[0].flags.b[2] >> 4;
            switch (temp_t1) {
            case 1:
                portrait_read_start(0x5477);
                break;
            case 2:
                portrait_read_start(0x54D1);
                break;
            case 4:
                portrait_read_start(0x54A4);
                break;
            default:
                portrait_read_start(0x544A);
                break;
            }
            mode_step_next();
        }
        break;
    case 0xA:
        talk_group = game_state.year - 0x5C;
        mode_step_next();
        break;
    case 0xB:
        normal_date_speak();
        break;
    case 0xC:
        normal_date_three_select();
        break;
    case 0xD:
        func_80147F8C();
        talk_group = 6;
        mode_step_next();
        break;
    case 0xE:
        normal_date_speak();
        break;
    case 0xF:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            portrait_show_checked();
        }
        break;
    case 0x10:
        if (((u16) game_state.player_flags.h >> 0xC) == ((u8) game_state.girls[0].flags.b[2] >> 4)) {
            talk_group = 0xA;
        } else {
            talk_group = 0xB;
        }
        mode_step_next();
        break;
    case 0x11:
        normal_date_speak();
        break;
    case 0x12:
        normal_date_bg_fadein();
        break;
    case 0x13:
        if (((u16) game_state.player_flags.h >> 0xC) == ((u8) game_state.girls[0].flags.b[2] >> 4)) {
            talk_group = 0xE;
        } else {
            talk_group = 0xD;
        }
        mode_step_next();
        break;
    case 0x14:
        normal_date_speak();
        break;
    case 0x15:
        normal_date_girl_in();
        break;
    case 0x16:
        temp_v0 = girl_state_rank(0);
        switch (temp_v0) {
        case 0:
        case 1:
            talk_group = 0x12;
            break;
        case 2:
            talk_group = 0x13;
            break;
        case 3:
            talk_group = 0x14;
            break;
        case 4:
            talk_group = 0x15;
            break;
        }
        mode_step_next();
        break;
    case 0x17:
        normal_date_speak();
        break;
    case 0x18:
        if (((u16) game_state.player_flags.h >> 0xC) == ((u8) game_state.girls[0].flags.b[2] >> 4)) {
            talk_group = 0x16;
            mode_step_next();
        } else {
            mode_step_set(0x1A);
        }
        break;
    case 0x19:
        normal_date_speak();
        break;
    case 0x1A:
        temp_v0_2 = girl_state_rank(0);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0x17;
            break;
        case 2:
            talk_group = 0x18;
            break;
        case 3:
            talk_group = 0x19;
            break;
        case 4:
            talk_group = 0x1A;
            break;
        }
        mode_step_next();
        break;
    case 0x1B:
        normal_date_speak();
        break;
    case 0x1C:
        switch (*(u32 *)&D_80122CDC) {
        case 0:
            talk_group = 0x1B;
            portrait_layout_set(1U);
            game_state.girls[0].tokimeki += 3;
            game_state.girls[0].affection += 2;
            game_state.girls[0].syoushin -= 0xF;
            break;
        case 1:
            talk_group = 0x1D;
            portrait_layout_set(2U);
            game_state.girls[0].tokimeki += 2;
            game_state.girls[0].affection += 1;
            game_state.girls[0].syoushin -= 0xA;
            break;
        case 2:
            talk_group = 0x1F;
            portrait_layout_set(3U);
            game_state.girls[0].affection += 1;
            game_state.girls[0].syoushin += 0xA;
            break;
        }
        func_80063668();
        mode_step_next();
        break;
    case 0x1D:
        normal_date_speak();
        break;
    case 0x1E:
        normal_date_girl_out();
        break;
    case 0x1F:
        normal_date_speak();
        break;
    case 0x20:
        normal_date_bg_fadeout();
        break;
    case 0x21:
        talk_group = 0x24;
        mode_step_next();
        break;
    case 0x22:
        normal_date_speak();
        break;
    case 0x23:
        func_80142BC8();
        break;
    case 0x24:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x200);
            game_state.day_event_done_bits |= 4;
            mode_step_next();
        }
        break;
    case 0x25:
        normal_date_bg_fadein();
        break;
    case 0x26:
        if (((u16) game_state.player_flags.h >> 0xC) != ((u8) game_state.girls[0].flags.b[2] >> 4)) {
            talk_group = 0x22;
            mode_step_set(0x3E);
        } else {
            talk_group = 0x2A;
            mode_step_next();
        }
        break;
    case 0x27:
        normal_date_girl_in();
        break;
    case 0x28:
        normal_date_speak();
        break;
    case 0x29:
        cd_read_sectors(3U, 0x801B0000, 0xAF43);
        mode_step_next();
        break;
    case 0x2A:
        if (cd_read_sync_status() & 1) {
            func_80146FA0();
            talk_group = 1;
            talk_line = 0;
            talk_text_table = D_80160644;
            talk_command_table = D_80160678;
            talk_voice_table = D_801606AC;
            mode_step_next();
        }
        break;
    case 0x2B:
        normal_date_speak();
        break;
    case 0x2C:
        cd_read_sectors(3U, 0x801E8000, 0x49B7);
        mode_step_next();
        break;
    case 0x2D:
        if (cd_read_sync_status() & 1) {
            func_80132444();
            talk_group = 0x26;
            talk_line = 0;
            talk_text_table = (s32) D_801604F0;
            talk_command_table = (s32) D_801604F4;
            talk_voice_table = (s32) D_801604F8;
            mode_step_next();
        }
        break;
    case 0x2E:
        normal_date_speak();
        break;
    case 0x2F:
        talk_group = 0x29;
        mode_step_next();
        break;
    case 0x30:
        normal_date_speak();
        break;
    case 0x31:
        talk_group = game_state.year - 0x5C;
        mode_step_next();
        break;
    case 0x32:
        normal_date_speak();
        break;
    case 0x33:
        normal_date_three_select();
        break;
    case 0x34:
        talk_group = 0x23;
        mode_step_next();
        break;
    case 0x35:
        normal_date_speak();
        break;
    case 0x36:
        switch (*(u32 *)&D_80122CDC) {
        case 0:
            talk_group = 0x1B;
            portrait_layout_set(1U);
            game_state.girls[0].tokimeki += 3;
            game_state.girls[0].affection += 2;
            game_state.girls[0].syoushin -= 0xF;
            break;
        case 1:
            talk_group = 0x1D;
            portrait_layout_set(2U);
            game_state.girls[0].tokimeki += 2;
            game_state.girls[0].affection += 1;
            game_state.girls[0].syoushin -= 0xA;
            break;
        case 2:
            talk_group = 0x1F;
            portrait_layout_set(3U);
            game_state.girls[0].affection += 1;
            game_state.girls[0].syoushin += 0xA;
            break;
        }
        func_80063668();
        mode_step_next();
        break;
    case 0x37:
        normal_date_speak();
        break;
    case 0x38:
        normal_date_girl_out();
        break;
    case 0x39:
        normal_date_speak();
        break;
    case 0x3A:
        normal_date_bg_fadeout();
        break;
    case 0x3B:
        talk_group = 0x24;
        mode_step_next();
        break;
    case 0x3C:
        normal_date_speak();
        break;
    case 0x3D:
        func_80142BC8();
        break;
    case 0x3E:
        normal_date_girl_in();
        break;
    case 0x3F:
        normal_date_speak();
        break;
    case 0x40:
        talk_group = 0x26;
        mode_step_set(0x2E);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80147F8C(void) {
    u8 *name;
    BunkasaiNameTable tbl;

    tbl = D_801606DC;
    name = tbl.s[game_state.year - 95][D_80122CDC];
    strcpy(D_800CA25C, name);
}
