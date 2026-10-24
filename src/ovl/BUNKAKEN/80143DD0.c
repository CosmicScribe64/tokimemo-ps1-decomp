#define MAIN_API_OVERRIDE_talk_names_set /* T-24005: matched caller passes a word; callee narrows it. */
#include "common.h"
#include "ovl/BUNKAKEN.h"

void talk_names_set();

typedef struct {
    s32 w[6];
} Tbl6_80144D38;
extern Tbl6_80144D38 D_80155F70;

void func_80143DD0(void) {
    s32 temp_v0_3;
    s32 temp_v0_6;
    s32 var_v0;
    s32 var_v0_2;
    u8 temp_t4;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_4;
    u32 temp_v0_5;

    switch (game_state.mode_step) {
    case 0:
        if (D_80122EBC != 0xFF) {
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            mode_step_next();
        } else {
            talk_group = 1;
            talk_line = 0;
            mode_step_set(2);
        }
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_801324E0();
            talk_group = 1;
            talk_line = 0;
            talk_text_table = (s32) D_80155E90;
            talk_command_table = (s32) D_80155E94;
            talk_voice_table = (s32) D_80155E98;
            mode_step_next();
        }
        break;
    case 2:
        normal_date_speak();
        break;
    case 3:
        normal_date_two_select();
        break;
    case 4:
        if (D_80122CDC == 1) {
            mode_step_set(6);
        } else {
            mode_step_set(7);
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        if (D_80122EBC != 0xFF) {
            D_80122EBC = 0xFF;
        }
        mode_stage_set(2);
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        select_girl();
        break;
    case 9:
        if (D_80122CDC == D_80122EA4) {
            talk_group = 4;
            talk_line = 0;
            mode_step_next();
        } else if (D_80122CDC == 0xB) {
            talk_group = 5;
            talk_line = 0;
            mode_step_set(5);
        } else {
            if ((D_80122CDC == 0) && ((u32) ((u8) game_state.girls[0].flags.b[2] >> 4) >= 6U)) {
                D_80122EBC = 0xB;
            } else {
                D_80122EBC = D_80122CDC;
            }
            talk_names_set(D_80122CDC);
            mode_step_set(0xC);
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        talk_group = 2;
        mode_step_set(7);
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        portrait_read_step(D_80155EA0[D_80122EBC]);
        break;
    case 14:
        addr_init_bustup();
        if (D_80122EBC == 0xB) {
            game_state.current_girl = GIRL_SHIORI;
        } else {
            game_state.current_girl = (u8) D_80122EBC;
        }
        portrait_show_checked();
        cd_read_sectors(D_80155F00[D_80122EBC], 0x801E8000, D_80155F0C[D_80122EBC]);
        break;
    case 15:
        normal_date_girl_in();
        break;
    case 16:
        if (cd_read_sync_status() & 1) {
            D_80155ED0[D_80122EBC]();
            func_8013BFA0();
            if (D_80122EBC == 0) {
                mode_step_set(0x14);
            } else if (D_80122EBC < 4) {
                mode_step_set(0x1F);
            } else {
                mode_step_set(0x2A);
            }
        }
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        D_80122EBC = 0xFF;
        mode_stage_set(2);
        break;
    case 20:
        temp_v0 = girl_state_rank(0);
        switch (temp_v0) {
        case 0:
        case 1:
            talk_group = 0x10;
            break;
        case 2:
            talk_group = 0x1B;
            break;
        case 3:
            talk_group = 0x24;
            break;
        case 4:
            talk_group = 0x2D;
            break;
        }
        talk_line = 0;
        mode_step_next();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        temp_v0_2 = girl_state_rank(0);
        switch (temp_v0_2) {
        case 0:
        case 1:
            talk_group = 0x11;
            break;
        case 2:
            talk_group = 0x1C;
            break;
        case 3:
            talk_group = 0x25;
            break;
        case 4:
            talk_group = 0x2E;
            break;
        }
        /* The halfword conversion preserves the original temporary order. T-24005. */

        talk_group += (s16) ((u8) game_state.girls[0].flags.b[2] >> 4);
        mode_step_next();
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        if (girl_state_rank(0) == 4) {
            talk_group = 0x34;
            mode_step_set(0x11);
        } else {
            if (game_state.girls[0].flags.f.b2) {
                talk_group = 0x17;
            } else {
                talk_group = 0x18;
            }
            mode_step_next();
        }
        game_state.girls[0].flags.f.b2 = 1;
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        temp_v0_3 = girl_state_rank(0);
        switch (temp_v0_3) {
        case 0:
        case 1:
            talk_group = 0x19;
            mode_step_next();
            break;
        case 2:
            talk_group = 0x22;
            mode_step_next();
            break;
        case 3:
            talk_group = 0x2B;
            mode_step_next();
            break;
        }
        /* fallthrough */
    case 27:
        normal_date_speak();
        break;
    case 28:
        normal_date_girl_out();
        break;
    case 29:
        normal_date_speak();
        break;
    case 30:
        D_80122EBC = 0xFF;
        mode_stage_set(2);
        break;
    case 31:
        temp_v0_4 = girl_state_rank(D_80122EBC);
        switch (temp_v0_4) {
        case 0:
        case 1:
            talk_group = 8;
            break;
        case 2:
            talk_group = 0xF;
            break;
        case 3:
            talk_group = 0x15;
            break;
        case 4:
            talk_group = 0x1B;
            break;
        }
        talk_line = 0;
        mode_step_next();
        break;
    case 32:
        normal_date_speak();
        break;
    case 33:
        temp_v0_5 = girl_state_rank(D_80122EBC);
        switch (temp_v0_5) {
        case 0:
        case 1:
            talk_group = 9;
            break;
        case 2:
            talk_group = 0x10;
            break;
        case 3:
            talk_group = 0x16;
            break;
        case 4:
            talk_group = 0x1C;
            break;
        }
        if (((D_80122EBC == 1) && (((u8) game_state.girls[1].flags.b[2] >> 4) == 1)) || ((D_80122EBC == 2) && (((u8) game_state.girls[2].flags.b[2] >> 4) == 3)) || ((D_80122EBC == 3) && (((u8) game_state.girls[3].flags.b[2] >> 4) == 5))) {
            talk_group += 1;
        }
        mode_step_next();
        break;
    case 34:
        normal_date_speak();
        break;
    case 35:
        if (girl_state_rank(D_80122EBC) == 4) {
            talk_group = 0xE;
            mode_step_set(0x11);
        } else {
            if ((D_80122EBC == 1) && (girl_state_rank(1) < 2)) {
                talk_group = 0x12;
            } else if (game_state.girls[D_80122EBC].flags.f.b2 == 1) {
                talk_group = 0xB;
            } else {
                talk_group = 0x14;
            }
            mode_step_next();
        }
        game_state.girls[D_80122EBC].flags.f.b2 = 1;
        break;
    case 36:
        normal_date_speak();
        break;
    case 37:
        temp_v0_6 = girl_state_rank(D_80122EBC);
        switch (temp_v0_6) {
        case 0:
        case 1:
            talk_group = 0xC;
            if (((D_80122EBC == 1) && (((u8) game_state.girls[1].flags.b[2] >> 4) == 1)) || ((D_80122EBC == 2) && (((u8) game_state.girls[2].flags.b[2] >> 4) == 3)) || ((D_80122EBC == 3) && (((u8) game_state.girls[3].flags.b[2] >> 4) == 5))) {
                talk_group = 0xD;
            }
            mode_step_next();
            break;
        case 2:
            talk_group = 0x13;
            mode_step_next();
            break;
        case 3:
            talk_group = 0x19;
            mode_step_next();
            break;
        }
        break;
    case 38:
        normal_date_speak();
        break;
    case 39:
        normal_date_girl_out();
        talk_group = 0xE;
        break;
    case 40:
        normal_date_speak();
        break;
    case 41:
        D_80122EBC = 0xFF;
        mode_stage_set(2);
        break;
    case 42:
        if (D_80122EBC == 0xB) {
            var_v0 = get_g_zyotai_s(GIRL_SHIORI);
        } else {
            /* FAKE: self-store preserves the original store in the call delay slot. T-24005. */
            D_80122EBC = D_80122EBC;
            var_v0 = get_g_zyotai_s(D_80122EBC);
        }
        temp_t4 = var_v0 & 0x7F;
        /* FAKE: empty test keeps the mask in t4 and the switch index in t5. T-24005. */
        if (temp_t4) {}
        switch (temp_t4) {
        case 0:
        case 1:
            talk_group = 3;
            break;
        case 2:
            talk_group = 4;
            break;
        case 3:
            talk_group = 5;
            break;
        case 4:
            if ((D_80122EBC == 4) && (((u16) game_state.player_flags.h >> 0xC) == ((u8) game_state.girls[4].flags.b[2] >> 4))) {
                talk_group = 8;
            } else {
                talk_group = 6;
            }
            break;
        }
        talk_line = 0;
        mode_step_next();
        break;
    case 43:
        normal_date_speak();
        break;
    case 44:
        if (D_80122EBC == 0xB) {
            var_v0_2 = get_g_zyotai_s(GIRL_SHIORI);
        } else {
            /* FAKE: self-store preserves the original store in the call delay slot. T-24005. */
            D_80122EBC = D_80122EBC;
            var_v0_2 = get_g_zyotai_s(D_80122EBC);
        }
        if (var_v0_2 == 4) {
            mode_step_set(0x11);
        } else if ((D_80122EBC == 8) && (girl_state_rank(8) >= 2)) {
            mode_step_next();
        } else {
            mode_stage_set(3);
        }
        break;
    case 45:
        switch (game_state.year) {
        case 0x5F:
            talk_group = 9;
            break;
        case 0x60:
            talk_group = 0xA;
            break;
        case 0x61:
            talk_group = 0xB;
            break;
        }
        if (girl_state_rank(8) == 3) {
            talk_group += 3;
        }
        mode_step_next();
        break;
    case 46:
        normal_date_speak();
        break;
    case 47:
        talk_group = 0xF;
        mode_step_next();
        break;
    case 48:
        normal_date_speak();
        break;
    case 49:
        mode_stage_set(3);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80144A70(void) {
    switch (game_state.mode_step) {
    case 0:
        portrait_read_step(0x6C5F);
        break;
    case 1:
        addr_init_bustup();
        game_state.current_girl = GIRL_YOSHIO;
        portrait_show_checked();
        cd_read_sectors(5U, 0x801E8000, 0x49BA);
        break;
    case 2:
        normal_date_girl_in();
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            talk_names_set(0xDU);
            func_801324E0();
            talk_group = 6;
            talk_line = 0;
            talk_text_table = (s32) D_80155E90;
            talk_command_table = (s32) D_80155E94;
            talk_voice_table = (s32) D_80155E98;
            mode_step_next();
        }
        break;
    case 4:
        normal_date_speak();
        break;
    case 5:
        switch (game_state.year) {
        case 0x5F:
            break;
        case 0x60:
            talk_group += 1;
            break;
        case 0x61:
            talk_group += 2;
            break;
        default:
            game_mode_reset_to_title();
            break;
        }
        mode_step_next();
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_girl_out();
        break;
    case 8:
        talk_group = 0xA;
        mode_step_next();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        mode_stage_set(3);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80144C5C(void) {
    switch (game_state.mode_step) {
    case 0:
        func_80144D38();
        mode_step_next();
        return;
    case 1:
        func_80144E9C();
        return;
    case 2:
        ((CultureFestivalDispatch *)&D_80122EB8)->unk_00 = D_80122CDC;
        if (((CultureFestivalDispatch *)&D_80122EB8)->unk_04 == 0xFF) {
            talk_group = D_80122CDC + 0xC;
            mode_step_next();
            return;
        }
        mode_stage_next();
        return;
    case 3:
        normal_date_speak();
        return;
    case 4:
        mode_stage_next();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void func_80144D38(void) {
    s32 i;
    s32 pad[2]; /* FAKE: unused local above the arrays reproduces the original frame 0xC0; real source unknown. T-9350 */
    s16 a[6];
    s16 b[6];
    s16 c[6];
    s16 d[6];
    Tbl6_80144D38 tbl;

    tbl = D_80155F70;
    k_reset(1);
    for (i = 0; i < 6; i++) {
        a[i] = (i & 1) * 0x78 - 0x80;
        b[i] = (i >> 1) * 0x10 + 0x32;
        set_kanji_string(a[i], b[i], 0, tbl.w[i], 0);
        c[i] = 0x78;
        d[i] = 0x10;
    }
    D_80122D30 = 1;
    k_disp_start(7);
    menu_set(0, 6, a, b, c, d);
    cursor_actor_init(0);
}

void func_80144E9C(void) {
    s32 i;

    menu_check(0, cursor_x, cursor_y);
    menu_cursor_update(0);
    menu_bar_show(0);
    cursor_show(0);
    cursor_update();
    i = 0;
    if (game_state.pad_pressed & PAD_CIRCLE) {
        for (i = 0; i < 6; i++) {
            if (game_state.menu_items[i * 2].hover != 0) {
                D_80122CDC = i;
                D_8011ECD3 &= ~0x80;
                k_reset(1);
                D_80122D30 = 0;
                mode_step_next();
            }
        }
    }
}

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80143DD0.rodata", D_80154920);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80143DD0.rodata", D_80154930);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80143DD0.rodata", D_80154940);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80143DD0.rodata", D_80154950);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80143DD0.rodata", D_80154960);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80143DD0.rodata", D_80154970);
