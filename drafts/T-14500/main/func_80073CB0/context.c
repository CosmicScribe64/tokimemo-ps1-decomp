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

/* bulk_m2c declarations (T-11190) */
extern s32 D_800B6890;
extern s32 D_800B6892;
extern s32 D_800B6894;
extern s32 D_800B6896;
extern s32 D_800B6898;
extern s32 D_800B689A;
extern s32 D_800B689C;
extern s32 D_800B689E;
extern s32 D_800B68A0;
extern s32 D_800B68A2;
extern s32 D_800B68A4;
extern s32 D_800B68A8;
extern s32 D_800B68AA;
extern s32 D_800B68AC;
extern s32 D_800B68AE;
extern s32 D_800B68B0;
extern s32 D_800B68B2;
extern s32 D_800B68B4;
extern s32 D_800B68B6;
extern s32 D_800B68B8;
extern s32 D_800B68BA;
extern s32 D_800B68BC;
extern s32 D_800B68C0;
extern s32 D_800B68C2;
extern s32 D_800B68C4;
extern s32 D_800B68C6;
extern s32 D_800B68C9;
extern s32 D_800B68CB;
extern s32 D_800B68D0;
extern s32 D_800B68D2;
extern s32 D_800B68D4;
extern s32 D_800B68D6;
extern s32 D_800B68D9;
extern s32 D_800B68DB;
extern u16 D_800E6374;
extern u8 D_800B68CD;
extern u8 D_800B68DD;
extern u8 D_800E699C;
/* end bulk_m2c declarations */
void func_80073CB0(void) {
    s32 sp28;
    s32 var_v1;
    u32 temp_t6;
    u32 temp_t9;
    u8 temp_a0;
    u8 temp_t3;
    u8 temp_t5;
    u8 var_t0;

    D_801230D0 = 0xFF;
    temp_a0 = D_800E6280.unk_69C[D_800E699C].unk_03;
    if (temp_a0 != 0) {
        var_v1 = 1;
    } else {
        var_v1 = 0;
    }
    if ((D_800E6280.unk_69C[D_800E699C].unk_01 == 3) && (temp_a0 == 5)) {
        var_v1 = 2;
    }
    sp28 = var_v1;
    if (func_80066A84() == 1) {
        temp_t3 = D_800E6280.unk_69C[D_800E699C].unk_01;
        switch (temp_t3) {
        case 0:
            D_801230D0 = (s32) *(&D_800B68C0 + var_v1);
            break;
        case 1:
            D_801230D0 = (s32) *(&D_800B68C2 + var_v1);
            break;
        case 2:
            D_801230D0 = (s32) *(&D_800B68C4 + var_v1);
            break;
        case 3:
            D_801230D0 = (s32) *(&D_800B68C6 + var_v1);
            break;
        case 4:
            D_801230D0 = 0xFF;
            break;
        case 5:
            D_801230D0 = (s32) *(&D_800B68C9 + var_v1);
            break;
        case 6:
            D_801230D0 = (s32) *(&D_800B68CB + var_v1);
            break;
        case 7:
            D_801230D0 = (s32) D_800B68CD;
            break;
        }
    } else {
        temp_t5 = D_800E6280.unk_69C[D_800E699C].unk_01;
        switch (temp_t5) {
        case 0:
            D_801230D0 = (s32) *(&D_800B68D0 + var_v1);
            break;
        case 1:
            D_801230D0 = (s32) *(&D_800B68D2 + var_v1);
            break;
        case 2:
            D_801230D0 = (s32) *(&D_800B68D4 + var_v1);
            break;
        case 3:
            D_801230D0 = (s32) *(&D_800B68D6 + var_v1);
            break;
        case 4:
            D_801230D0 = 0xFF;
            break;
        case 5:
            D_801230D0 = (s32) *(&D_800B68D9 + var_v1);
            break;
        case 6:
            D_801230D0 = (s32) *(&D_800B68DB + var_v1);
            break;
        case 7:
            D_801230D0 = (s32) D_800B68DD;
            break;
        }
    }
    if (D_801230D0 == 0xFF) {
        sp28 = var_v1;
        if (func_80066A84() == 1) {
            temp_t6 = (u16) D_800E6374 >> 0xC;
            switch (temp_t6) {
            case 0:
                D_801230D0 = (s32) *(&D_800B6890 + var_v1);
                break;
            case 1:
                D_801230D0 = (s32) *(&D_800B6892 + var_v1);
                break;
            case 2:
                D_801230D0 = (s32) *(&D_800B6894 + var_v1);
                break;
            case 3:
                var_t0 = *(&D_800B6896 + var_v1);
                goto block_55;
            case 4:
                D_801230D0 = (s32) *(&D_800B6898 + var_v1);
                break;
            case 5:
                D_801230D0 = (s32) *(&D_800B689A + var_v1);
                break;
            case 6:
                D_801230D0 = (s32) *(&D_800B689C + var_v1);
                break;
            case 7:
                D_801230D0 = (s32) *(&D_800B689E + var_v1);
                break;
            case 8:
                D_801230D0 = (s32) *(&D_800B68A0 + var_v1);
                break;
            case 9:
                D_801230D0 = (s32) *(&D_800B68A2 + var_v1);
                break;
            case 10:
                D_801230D0 = (s32) *(&D_800B68A4 + var_v1);
                break;
            }
        } else {
            temp_t9 = (u16) D_800E6374 >> 0xC;
            switch (temp_t9) {
            case 0:
                var_t0 = *(&D_800B68A8 + var_v1);
block_55:
                D_801230D0 = (s32) var_t0;
                break;
            case 1:
                D_801230D0 = (s32) *(&D_800B68AA + var_v1);
                break;
            case 2:
                D_801230D0 = (s32) *(&D_800B68AC + var_v1);
                break;
            case 3:
                D_801230D0 = (s32) *(&D_800B68AE + var_v1);
                break;
            case 4:
                D_801230D0 = (s32) *(&D_800B68B0 + var_v1);
                break;
            case 5:
                D_801230D0 = (s32) *(&D_800B68B2 + var_v1);
                break;
            case 6:
                D_801230D0 = (s32) *(&D_800B68B4 + var_v1);
                break;
            case 7:
                D_801230D0 = (s32) *(&D_800B68B6 + var_v1);
                break;
            case 8:
                D_801230D0 = (s32) *(&D_800B68B8 + var_v1);
                break;
            case 9:
                D_801230D0 = (s32) *(&D_800B68BA + var_v1);
                break;
            case 10:
                var_t0 = *(&D_800B68BC + var_v1);
                goto block_55;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/800737A0", func_800741B8);

INCLUDE_ASM("asm/nonmatchings/main/800737A0", func_8007437C);

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

INCLUDE_ASM("asm/nonmatchings/main/800737A0", parameter_up_down);

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

INCLUDE_ASM("asm/nonmatchings/main/800737A0", func_8007505C);
