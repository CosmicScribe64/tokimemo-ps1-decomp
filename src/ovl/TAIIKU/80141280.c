#define MAIN_API_OVERRIDE_set_dec_bri /* the original calls it as implicit int: the zero loop's pointer takes $v1, its end $v0 (T-19032) */
#include "common.h"
#include "ovl/TAIIKU.h"

s32 set_dec_bri();

/* Defined here, as in the original: the two stores in the copy loop share one lui $at (T-9010). The auto
 * array `tbl` below also puts its initialiser in this object's .data, right after this table. */
s32 D_8014A070[6] = { 0 };

void func_80141280(void) {
    s32 i;
    s32 j;
    s32 tbl[6] = { 0x80197000, 0x80193000, 0x8018F000, 0x8019B000, 0x8019F000, 0x801A3000 };

    switch (D_80122EC8) {
    case 0:
        audio_command(0xBF);
        audio_bank_load_start(1, 0xC5B7, 0xC594, 0xD6B2, 0xD664, 0xD643);
        scene_screen_reset(1, 0);
        D_80122ECC = 0;
        talk_clear();
        for (i = 0; i < 6; i++) {
            D_8014A070[i] = tbl[i];
        }
        dec_bg_reset();
        message_window_init();
        k_reset(0);
        message_window_set_title("\221\314\210\347\215\325");
        message_window_title_show(0);
        func_8006764C(0);
        hizuke_init();
        icon_disp_switch(0);
        parameter_disp_switch(0);
        Default_Disp();
        D_80122EC8 += 1;
        break;
    case 1:
        set_dec_bri(0);
        D_80122EC8 += 1;
        Default_Disp();
        return;
    case 2:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x202);
            dec_bg_cd_read(0x4020, 1);
            D_80122EC8 += 1;
        }
        Default_Disp();
        return;
    case 3:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(1, 0x801F0000, 0xBA16);
            message_window_title_show(1);
            D_80122EC8 += 1;
        }
        Default_Disp();
        return;
    case 4:
        normal_date_bg_fadein();
        if (game_state.mode_step != 0) {
            game_state.mode_step = 0;
            D_80122EC8 += 1;
        }
        Default_Disp();
        return;
    case 5:
        if (cd_read_sync_status() & 1) {
            D_80122EC8 += 1;
            text_marker_hide();
            talk_names_set(0);
            func_80132B90();
            func_80141B70();
            talk_group = 0;
            /* FAKE: the (u8) narrowing adds the ugen temporary the original has for this test (T-19032) */
            if ((u8)(game_state.player_flags.b[1] & 2)) {
                talk_group = 6;
            }
            talk_line = 0;
            talk_text_table = D_80148F00;
            talk_command_table = D_80148F04;
            talk_voice_table = D_80148F08;
        }
        Default_Disp();
        return;
    case 6:
        normal_date_speak();
        if (game_state.mode_step != 0) {
            talk_group = 1;
            D_80122EC8 += 1;
            game_state.mode_step = 0;
        }
        Default_Disp();
        return;
    case 7:
        normal_date_speak_012();
        if (game_state.mode_step != 0) {
            func_80141964();
            game_state.mode_step = 0;
            D_80122EC8 = 8;
        }
        Default_Disp();
        return;
    case 8:
        func_801419F8();
        if (game_state.mode_step != 0) {
            /* FAKE: u32 view through the address gives the selector $v1, as in the original (T-19032) */
            switch (*(u32 *)&D_80122CDC) {
            case 0:
                D_80122EC8 = 0xC;
                break;
            case 1:
                D_80122EC8 = 0xD;
                break;
            case 2:
                func_80141AE8();
                break;
            default:
                game_mode_reset_to_title();
                break;
            }
            game_state.mode_step = 0;
        }
        Default_Disp();
        return;
    case 9:
        normal_date_speak_012();
        if (game_state.mode_step != 0) {
            D_80122EC8 = 0xA;
            game_state.mode_step = 0;
        }
        Default_Disp();
        return;
    case 10:
        func_801419F8();
        if (game_state.mode_step != 0) {
            /* FAKE: u32 view through the address gives the selector $v1, as in the original (T-19032) */
            switch (*(u32 *)&D_80122CDC) {
            case 0:
                D_80122EC8 = 0xE;
                break;
            case 1:
                D_80122EC8 = 0xF;
                break;
            case 2:
                func_80141B28();
                break;
            default:
                game_mode_reset_to_title();
                break;
            }
            game_state.mode_step = 0;
        }
        Default_Disp();
        return;
    case 11:
        normal_date_speak_012();
        if (game_state.mode_step != 0) {
            D_80122EC8 = 8;
            game_state.mode_step = 0;
        }
        Default_Disp();
        return;
    case 12:
        func_801452D0();
        if (D_80122ECC == 1) {
            D_80122EC8 = 0x10;
        }
        break;
    case 13:
        func_80133C80();
        if (D_80122ECC == 1) {
            D_80122EC8 = 0x10;
        }
        break;
    case 14:
        func_80142240();
        if (D_80122ECC == 1) {
            D_80122EC8 = 0x10;
        }
        break;
    case 15:
        func_80138A34();
        if (D_80122ECC == 1) {
            D_80122EC8 = 0x10;
        }
        break;
    case 16:
        func_80132000();
        if (D_80122ECC == 2) {
            D_80122EC8 += 1;
        }
        break;
    case 17:
        audio_command(0xBF);
        func_800674B0();
        Default_Disp();
        D_80122EC8 += 1;
        break;
    case 18:
        if (audio_bank_load_poll() == 1) {
            dec_bg_reset();
            set_dec_bri(0);
            /* D_80122DD0 is a u8[0x100] in main_api.h (main bzeroes it); here it is 16 records of 4 words, and
             * the store order below is the original's: the last one lands in the bne delay slot (T-19032) */
            for (j = 0; j != 16; j++) {
                ((TaiRec16 *)D_80122DD0)[j].unk_04 = 0;
                ((TaiRec16 *)D_80122DD0)[j].unk_08 = 0;
                ((TaiRec16 *)D_80122DD0)[j].unk_0C = 0;
                ((TaiRec16 *)D_80122DD0)[j].unk_00 = 0;
            }
            /* the counter is assigned again: IDO then keeps the loop rolled, as the original is (T-5020) */
            j = 1;
            day_event_next(j);
        }
        Default_Disp();
        return;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80141964(void) {
    s32 pad; /* FAKE: unused word above the arrays, the original frame has it (T-9020) */
    s16 x[3];
    s16 y[3];
    s16 w[3];
    s16 h[3];

    x[0] = x[1] = x[2] = -0x80;
    y[0] = 0x30;
    y[1] = 0x40;
    y[2] = 0x50;
    w[0] = w[1] = w[2] = 0xFC;
    h[0] = h[1] = h[2] = 0x10;
    menu_set(0, 3, x, y, w, h);
    cursor_actor_init(0);
    cursor_show(1);
}

s32 func_801419F8(void) {
    menu_check(0, cursor_x, cursor_y);
    menu_cursor_update(0);
    menu_bar_show(0);
    cursor_show(1);
    cursor_update();
    if (game_state.pad_pressed & PAD_CIRCLE) {
        if (check_end_k() != 0) {
            if (game_state.menu_items[0].hover != 0) {
                D_80122CDC = 0;
            } else if (game_state.menu_items[2].hover != 0) {
                D_80122CDC = 1;
            } else if (game_state.menu_items[4].hover != 0) {
                D_80122CDC = 2;
            } else {
                return 0;
            }
            ((Actor *)actor_pool)->draw_flags &= 0xFF7F;
            k_reset(1);
            mode_step_next();
        }
    }
}

void func_80141AE8(void) {
    D_80122EC8 = 9;
    talk_group = 2;
    talk_line = 0;
    k_reset(1);
}

void func_80141B28(void) {
    D_80122EC8 = 0xB;
    talk_group = 1;
    talk_line = 0;
    k_reset(1);
}
