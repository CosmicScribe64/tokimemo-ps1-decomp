#include "common.h"
#include "game.h"

void week_day_main(void);
void week_day_exit(void);

void func_800737A0(void) {
    func_80072B5C(0);
    parameter_show();
    message_window_show();
    hizuke_show();
    func_80065B0C(0);
    func_80067870();
    func_80066C08(1);
    func_800578F4(get_last_gamen_mode());
}

void week_day(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        week_day_init();
        break;
    case 1:
        week_day_main();
        break;
    case 2:
        week_day_exit();
        break;
    case 10:
        vacation_day_init();
        break;
    default:
        func_80046500();
        break;
    }
    parameter_show();
    message_window_show();
    hizuke_show();
    func_80065B0C(1);
    func_800578F4(0);
    func_80066C08(1);
    if (D_8011F3FF[0] & 0x80) {
        func_80067870();
    }
}

void week_day_exit(void) {
    D_800E6280.unk_1104.w += 1;
    if (D_800E6280.unk_110A == 0) {
        week_day_exit0();
    }
}

u8 week_day_init(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        return func_80073AD8();
    case 1:
        return func_800741B8();
    case 2:
        return func_8007437C();
    }
}

void week_day_main(void) {
    D_800E6280.unk_1104.w += 1;
    if (D_800E6280.unk_110A == 0) {
        week_day_main0();
    }
}

s32 get_weekly_bg_sector(void) {
    u8 x;
    u16 h;

    x = D_800E6280.unk_71C;
    if (D_800E6280.unk_69C[x].unk_01 != 4) {
        return D_800B6760[D_800E6280.unk_69C[x].unk_01];
    }
    h = D_800E6280.unk_0F4.h;
    return D_800B6780[h >> 0xC];
}

s32 func_80073A40(void) {
    s32 ret;

    if ((D_800E6280.unk_0F4.b[1] & 0xF) == 3 && !(*(u16 *)&D_800E6280.unk_0F6.b[0] & 1) && (D_800E6280.unk_F68.w & 0xF) == 7) {
        ret = dec_bg_cd_read(D_800B5950[func_80066A2C()], 0);
    } else {
        ret = dec_bg_cd_read(get_weekly_bg_sector(), 0);
    }
    return ret;
}

u8 func_80073AD8(void) {
    s32 temp_v0;

    func_8007259C(0);
    if (D_800E6280.unk_03D != 0) {
        func_80041584();
        func_80042908(0xA);
        return 0;
    } else {
        switch (D_800E6280.unk_110D) {                       /* irregular */
        case 0:
            func_80048E78();
            set_dec_bri(0x80U);
            back_clear_switch(1);
            tpage_buf_clear();
            func_80041584();
            hizuke_init();
            func_8006509C();
            icon_disp_switch(1);
            func_8006CE84();
            parameter_show_init();
            func_8006BC28(0);
            parameter_disp_switch(1);
            message_window_init();
            func_8005AD1C();
            if (D_800E6280.unk_0F6.h & 1) {
                D_801217D0->unk_00 = 0x80000000;
            } else {
                func_8005AD70();
            }
            func_8004E58C();
            func_80065F34(0);
            k_disp_start(0);
            temp_v0 = func_80073A40();
            if (temp_v0 != -1) {
                if ((temp_v0 == 0) || (temp_v0 == 1)) {
                    D_800E6280.unk_110D = 2;
                    dec_bg_show_set(0, temp_v0);
                }
            } else {
                D_800E6280.unk_110D = 1;
            }
            break;
        case 1:
            if (cd_read_sync_status() & 1) {
                dec_bg_show_set(0, func_8005751C(0));
                if (D_800B594C == -1) {
                    dec_bg_reset();
                    func_80073A40();
                } else {
                    D_800E6280.unk_110D = 2;
                }
            }
            break;
        default:
        case 2:
            func_8004284C();
            break;
        }
    }
}

void func_80073CB0(void) {
    /* FAKE: T-25049, this saved selection local preserves the original
     * call-spill home; removing it changes four instructions. */
    s32 sp28;
    s32 var_v1;
    u32 temp_t6;
    u32 temp_t9;
    u8 temp_a0;

    D_801230D0 = 0xFF;
    temp_a0 = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_03;
    if (temp_a0 != 0) {
        var_v1 = 1;
    } else {
        var_v1 = 0;
    }
    if ((D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 == 3) && (temp_a0 == 5)) {
        var_v1 = 2;
    }
    /* FAKE: T-25049, retain the entry value through this test so the
     * record address takes v0 and its byte value takes a0. No emitted test. */
    if (temp_a0) {}
    sp28 = var_v1;
    if (func_80066A84() == 1) {
        switch (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01) {
        case 0:
            D_801230D0 = (s32) D_800B68C0[0 + var_v1];
            break;
        case 1:
            D_801230D0 = (s32) D_800B68C0[2 + var_v1];
            break;
        case 2:
            D_801230D0 = (s32) D_800B68C0[4 + var_v1];
            break;
        case 3:
            D_801230D0 = (s32) D_800B68C0[6 + var_v1];
            break;
        case 4:
            D_801230D0 = 0xFF;
            break;
        case 5:
            D_801230D0 = (s32) D_800B68C0[9 + var_v1];
            break;
        case 6:
            D_801230D0 = (s32) D_800B68C0[11 + var_v1];
            break;
        case 7:
            D_801230D0 = (s32) D_800B68C0[13];
            break;
        }
    } else {
        switch (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01) {
        case 0:
            D_801230D0 = (s32) D_800B68D0[0 + var_v1];
            break;
        case 1:
            D_801230D0 = (s32) D_800B68D0[2 + var_v1];
            break;
        case 2:
            D_801230D0 = (s32) D_800B68D0[4 + var_v1];
            break;
        case 3:
            D_801230D0 = (s32) D_800B68D0[6 + var_v1];
            break;
        case 4:
            D_801230D0 = 0xFF;
            break;
        case 5:
            D_801230D0 = (s32) D_800B68D0[9 + var_v1];
            break;
        case 6:
            D_801230D0 = (s32) D_800B68D0[11 + var_v1];
            break;
        case 7:
            D_801230D0 = (s32) D_800B68D0[13];
            break;
        }
    }
    if (D_801230D0 == 0xFF) {
        sp28 = var_v1;
        if (func_80066A84() == 1) {
            temp_t6 = (u16) D_800E6280.unk_0F4.h >> 0xC;
            switch (temp_t6) {
            case 0:
                D_801230D0 = (s32) (*D_800B6890)[var_v1];
                break;
            case 1:
                D_801230D0 = (s32) D_800B6890[1][var_v1];
                break;
            case 2:
                D_801230D0 = (s32) D_800B6890[2][var_v1];
                break;
            case 3:
                D_801230D0 = D_800B6890[3][var_v1];
                break;
            case 4:
                D_801230D0 = (s32) D_800B6890[4][var_v1];
                break;
            case 5:
                D_801230D0 = (s32) D_800B6890[5][var_v1];
                break;
            case 6:
                D_801230D0 = (s32) D_800B6890[6][var_v1];
                break;
            case 7:
                D_801230D0 = (s32) D_800B6890[7][var_v1];
                break;
            case 8:
                D_801230D0 = (s32) D_800B6890[8][var_v1];
                break;
            case 9:
                D_801230D0 = (s32) D_800B6890[9][var_v1];
                break;
            case 10:
                D_801230D0 = (s32) D_800B6890[0xA][var_v1];
                break;
            }
        } else {
            temp_t9 = (u16) D_800E6280.unk_0F4.h >> 0xC;
            switch (temp_t9) {
            case 0:
                D_801230D0 = D_800B68A8[0][var_v1];
                break;
            case 1:
                D_801230D0 = (s32) D_800B68A8[1][var_v1];
                break;
            case 2:
                D_801230D0 = (s32) D_800B68A8[2][var_v1];
                break;
            case 3:
                D_801230D0 = (s32) D_800B68A8[3][var_v1];
                break;
            case 4:
                D_801230D0 = (s32) D_800B68A8[4][var_v1];
                break;
            case 5:
                D_801230D0 = (s32) D_800B68A8[5][var_v1];
                break;
            case 6:
                D_801230D0 = (s32) D_800B68A8[6][var_v1];
                break;
            case 7:
                D_801230D0 = (s32) D_800B68A8[7][var_v1];
                break;
            case 8:
                D_801230D0 = (s32) D_800B68A8[8][var_v1];
                break;
            case 9:
                D_801230D0 = (s32) D_800B68A8[9][var_v1];
                break;
            case 10:
                D_801230D0 = D_800B68A8[10][var_v1];
                break;
            }
        }
    }
}

u8 func_800741B8(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80073CB0();
        if (((D_800E6280.unk_0F4.b[1] & 0xF) == 3) && !(D_800E6280.unk_0F6.h & 1) && ((D_800E6280.unk_F68.w & 0xF) == 7)) {
            cd_read_sectors(9, 0x801E0000, 0x7D8A);
            D_800E6280.unk_110D = 1;
        } else if (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 != 4) {
            cd_read_sectors(0x29, 0x801E0000, D_800B67AC[D_801230D0]);
            D_800E6280.unk_110D = 1;
        } else {
            cd_read_sectors(0x29, 0x801E0000, D_800B6810[D_801230D0]);
            D_800E6280.unk_110D = 1;
        }
    } else if (D_800E6280.unk_110D == 1) {
        if (cd_read_sync_status() & 1) {
            if (((D_800E6280.unk_0F4.b[1] & 0xF) == 3) && !(D_800E6280.unk_0F6.h & 1) && ((D_800E6280.unk_F68.w & 0xF) == 7)) {
                if (file_checksum_ok(9, 0x801E0000) != 0) {
                    func_8004284C();
                } else {
                    D_800E6280.unk_110D = 0;
                }
            } else {
                if (file_checksum_ok(0x29, 0x801E0000) != 0) {
                    func_8004284C();
                } else {
                    D_800E6280.unk_110D = 0;
                }
            }
        }
    }
}

u8 func_8007437C(void) {
    GsRec69C *rec;
    s32 temp;
    u8 *tbl;

    func_80048F64(0x60);
    func_80048F64(0x61);
    rec = &D_800E6280.unk_69C[D_800E6280.unk_71C];
    temp = rec->unk_02;
    if (((D_800E6280.unk_0F4.b[1] & 0xF) == 3) && !(D_800E6280.unk_0F6.h & 1) && ((D_800E6280.unk_F68.w & 0xF) == 7)) {
        addr_init_else_sd(2);
        temp = 0;
    } else if (rec->unk_01 == 4) {
        addr_init_club_sd(D_801230D0);
    } else {
        addr_init_weekly_sd(D_801230D0);
        if ((D_801230D0 == 0xF) || (D_801230D0 == 0x10)) {
            temp = 0;
            D_800E6280.unk_69C[D_800E6280.unk_71C].unk_02 = 0;
        }
    }
    load_palette(D_80125CA4, 0x10, 1, 1, 0);
    ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 1;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 5;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0x01000000;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0x84;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_0C = (u8 *)D_80125CA8;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_10 = (u8 *)D_80125CAC;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_34 = (u8 *)D_80125CB0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 2;
    /* FAKE: u8 view of the s8 flag unk_F6E (both reads below). The original reloads the flag for the second
     * record; with the field read as s8 the first load is kept in a register and every temporary shifts.
     * Matches; real source unknown. T-17016 */
    if (*(u8 *)&D_800E6280.unk_F6E != 0) {
        ((DisplayActor44 *)D_8011ECD0)[96].unk_06 = 0x82;
    }
    ((DisplayActor44 *)D_8011ECD0)[96].unk_43 = 0x10;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = temp;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0x50;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x40;
    /* A pointer to the day table keeps its base apart from the record base (the original addresses it with
     * its own lui); D_800E6280.unk_044[0].unk_00[i] reuses the record base register. T-17016 */
    tbl = D_800E6280.unk_044[0].unk_00;
    if (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 != 7) {
        if ((tbl[D_800E6280.unk_040] != 1) && (tbl[D_800E6280.unk_040] != 2) && (tbl[D_800E6280.unk_040] != 3) && (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_03 != 0xF)) {
            ((DisplayActor44 *)D_8011ECD0)[97].unk_01 = 1;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_02 = 5;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_38 = 0x01000000;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_03 = 0x84;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (u8 *)D_80125CA8;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (u8 *)D_80125CAC;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (u8 *)D_80125CB0;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_05 = 2;
            if (*(u8 *)&D_800E6280.unk_F6E != 0) {
                ((DisplayActor44 *)D_8011ECD0)[97].unk_06 = 0x82;
            }
            ((DisplayActor44 *)D_8011ECD0)[97].unk_43 = 0x10;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_16 = (s16)(temp + 2);
            ((DisplayActor44 *)D_8011ECD0)[97].unk_26 = -0x18;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_2A = -0x40;
        }
    }
    func_80042808();
}

s32 week_day_main0(void) {
    s16 *state;
    s32 sp28;

    if (!(D_80120652 & 1)) {
        if (D_80120658 == 0) {
            state = &D_80120666;
            switch ((u32)(*state & 1)) {                      /* irregular */
            case 0:
                func_80044750(0x501);
                break;
            case 1:
                func_80044750(0x502);
                break;
            }
            sp28 = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01;
            parameter_up_down(sp28, 0, (s32) D_800E6280.unk_69C[D_800E6280.unk_71C].unk_02);
            parameter_show_init();
            if (sp28 == 4) {
                D_800E6280.unk_66C[(D_800E6280.unk_0F4.h >> 12) % 11].unk_00 += 1;
            }
        }
        if (D_80120658++ >= 0xD) {
            func_80042808();
        }
    }
    if ((u32) D_800E6280.unk_1104.u >= 0x259U) {
        func_800452C4();
        func_80042808();
    }
}

void week_day_exit0(void) {
    D_800E6280.unk_71C++;
    func_80048F64(0x60);
    func_80048F64(0x61);
    main_state_set(0x31);
}

s32 parameter_up_down(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 div_a;
    s32 div_b;
    s8 *row;
    s8 *p;
    u32 h;

    div_a = 1;
    div_b = 1;
    /* FAKE: T-25044, the extra raw-byte use keeps the byte in v1 while
     * the nibble is assigned directly to v0, avoiding an extra copy. */
    if (D_800E6280.unk_0F4.b[1]) {}
    h = D_800E6280.unk_0F4.b[1] & 0xF;
    if (h == 3) {
        div_a = 2;
    } else if (((h == 1) || (h == 2)) && (arg0 != 5) && (arg0 != 7) && (arg0 != 6)) {
        div_b = 2;
    }
    if (arg1 != 0) {
        arg1 = 4;
    } else {
        arg1 = 1;
    }
    if (arg2 != 0) {
        arg2 = 2;
    } else {
        arg2 = 1;
    }
    if (arg0 == 4) {
        h = D_800E6280.unk_0F4.h;
        row = D_800B6928[h >> 12];
        p = row;
        for (i = 0; i != 8; i++) {
            if (*p > 0) {
                parameter_change(i, *p * (u32)(arg1 * 0x1000 / arg2 / div_a / div_b));
            } else {
                parameter_change(i, *p << 12);
            }
            p++;
        }
        if (row[8] < 0) {
            parameter_change(8, row[8] * (u32)(arg1 * 0x1000 / arg2 / div_a / div_b) + (arg2 << 16) + 0xFFFF0000);
        } else {
            parameter_change(8, (row[8] << 12) + (arg2 << 16) + 0xFFFF0000);
        }
    } else {
        row = D_800B68E0[arg0];
        p = row;
        for (i = 0; i != 8; i++) {
            if (*p > 0) {
                parameter_change(i, *p * (u32)(arg1 * 0x1000 / arg2 / div_a / div_b));
            } else {
                parameter_change(i, *p << 12);
            }
            p++;
        }
        if (row[8] < 0) {
            parameter_change(8, row[8] * (u32)(arg1 * 0x1000 / arg2 / div_a / div_b) + (arg2 << 16) + 0xFFFF0000);
        } else {
            parameter_change(8, (row[8] << 12) + (arg2 << 16) + 0xFFFF0000);
        }
    }
}

u8 vacation_day_init(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        return func_80074DE0();
    case 1:
        return func_80074F24();
    case 2:
        return func_8007505C();
    }
}

s32 func_80074D28(void) {
    s32 ret;

    if ((D_800E6280.unk_69C[0].unk_00 == 0x25) || (D_800E6280.unk_69C[0].unk_00 == 0x42)) {
        ret = dec_bg_cd_read(func_80075FA0(), 0);
    } else if (((D_800E6280.unk_0F4.b[1] & 0xF) == 3) && ((D_800E6280.unk_F68.w & 0xF) == 7)) {
        ret = dec_bg_cd_read(D_800B5960[func_80066A2C()], 0);
    } else {
        ret = dec_bg_cd_read(func_80075FA0(), 0);
    }
    return ret;
}

u8 func_80074DE0(void) {
    s32 temp_v0;
    GameState *g;

    g = &D_800E6280;

    switch (g->unk_110D) {                           /* irregular */
    case 0:
        func_80048E78();
        set_dec_bri(0x80U);
        back_clear_switch(1);
        tpage_buf_clear();
        hizuke_init();
        func_8006509C();
        icon_disp_switch(1);
        func_8006CE84();
        parameter_show_init();
        parameter_disp_switch(1);
        message_window_init();
        D_801217D0->unk_00 = 0x80000000;
        func_8004E58C();
        func_80065F34(0);
        k_disp_start(0);
        temp_v0 = func_80074D28();
        if (temp_v0 != -1) {
            if ((temp_v0 == 0) || (temp_v0 == 1)) {
                g->unk_110D = 2;
                dec_bg_show_set(0, temp_v0);
            }
        } else {
            g->unk_110D = 1;
        }
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(0, func_8005751C(0));
            g->unk_110D = 2;
        }
        break;
    default:
    case 2:
        func_8004284C();
        break;
    }
}

u8 func_80074F24(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80076FDC();
        if ((D_800E6280.unk_0F4.b[1] & 0xF) == 3 && !(*(u16 *)&D_800E6280.unk_0F6.b[0] & 1) && (D_800E6280.unk_F68.w & 0xF) == 7) {
            cd_read_sectors(9, 0x801E0000, 0x7D8A);
            D_800E6280.unk_110D = 1;
        } else if (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 != 4) {
            cd_read_sectors(0x29, 0x801E0000, D_800B69E0[D_801230D0]);
            D_800E6280.unk_110D = 1;
        } else {
            cd_read_sectors(0x29, 0x801E0000, D_800B6810[D_801230D0]);
            D_800E6280.unk_110D = 1;
        }
    } else if ((D_800E6280.unk_110D == 1) && (cd_read_sync_status() & 1)) {
        func_8004284C();
    }
}

u8 func_8007505C(void) {
    GsRec69C *rec;
    s32 temp;

    func_80048F64(0x60);
    rec = &D_800E6280.unk_69C[D_800E6280.unk_71C];
    temp = rec->unk_02;
    if (((D_800E6280.unk_0F4.b[1] & 0xF) == 3) && !(D_800E6280.unk_0F6.h & 1) && ((D_800E6280.unk_F68.w & 0xF) == 7)) {
        addr_init_else_sd(2);
        temp = 0;
    } else if (rec->unk_01 == 4) {
        addr_init_club_sd(D_801230D0);
        if (func_8006F524(4) != 0xF) {
            ((DisplayActor44 *)D_8011ECD0)[97].unk_01 = 1;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_02 = 5;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_38 = 0x01000000;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_03 = 0x84;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (u8 *)D_80125CA8;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (u8 *)D_80125CAC;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (u8 *)D_80125CB0;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_05 = 2;
            if ((u8)D_800E6280.unk_F6E != 0) {
                ((DisplayActor44 *)D_8011ECD0)[97].unk_06 = 0x82;
            }
            ((DisplayActor44 *)D_8011ECD0)[97].unk_43 = 0x10;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_16 = (s16)(temp + 2);
            ((DisplayActor44 *)D_8011ECD0)[97].unk_26 = -0x18;
            ((DisplayActor44 *)D_8011ECD0)[97].unk_2A = -0x40;
        }
    } else {
        addr_init_holiday_sd(D_801230D0);
        if ((D_801230D0 == 6) || (D_801230D0 == 7)) {
            temp = 0;
            D_800E6280.unk_69C[D_800E6280.unk_71C].unk_02 = 0;
        }
    }
    load_palette(D_80125CA4, 0x10, 1, 1, 0);
    ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 1;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 5;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0x01000000;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0x84;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_0C = (u8 *)D_80125CA8;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_10 = (u8 *)D_80125CAC;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_34 = (u8 *)D_80125CB0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 2;
    if ((u8)D_800E6280.unk_F6E != 0) {
        ((DisplayActor44 *)D_8011ECD0)[96].unk_06 = 0x82;
    }
    ((DisplayActor44 *)D_8011ECD0)[96].unk_43 = 0x10;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = temp;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0x50;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x40;
    if (D_800E6280.unk_69C[0].unk_00 == 0x42) {
        func_80042808();
    } else {
        func_80042908(1);
    }
}
