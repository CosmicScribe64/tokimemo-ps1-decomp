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
        func_80044750(0xBF);
        func_80044890(1, 0xC5B7, 0xC594, 0xD6B2, 0xD664, 0xD643);
        func_80085E30(1, 0);
        D_80122ECC = 0;
        func_8008585C();
        for (i = 0; i < 6; i++) {
            D_8014A070[i] = tbl[i];
        }
        dec_bg_reset();
        message_window_init();
        k_reset(0);
        func_8006612C("\221\314\210\347\215\325");
        func_80065F34(0);
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
        if (func_80044E8C() == 1) {
            func_80044750(0x202);
            dec_bg_cd_read(0x4020, 1);
            D_80122EC8 += 1;
        }
        Default_Disp();
        return;
    case 3:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(1, 0x801F0000, 0xBA16);
            func_80065F34(1);
            D_80122EC8 += 1;
        }
        Default_Disp();
        return;
    case 4:
        normal_date_bg_fadein();
        if (D_800E6280.unk_110A != 0) {
            D_800E6280.unk_110A = 0;
            D_80122EC8 += 1;
        }
        Default_Disp();
        return;
    case 5:
        if (cd_read_sync_status() & 1) {
            D_80122EC8 += 1;
            func_80084E4C();
            func_800847B8(0);
            func_80132B90();
            func_80141B70();
            D_800CA148 = 0;
            /* FAKE: the (u8) narrowing adds the ugen temporary the original has for this test (T-19032) */
            if ((u8)(D_800E6280.unk_0F4.b[1] & 2)) {
                D_800CA148 = 6;
            }
            D_800CA14C = 0;
            D_800CA160 = D_80148F00;
            D_800CA164 = D_80148F04;
            D_800CA168 = D_80148F08;
        }
        Default_Disp();
        return;
    case 6:
        normal_date_speak();
        if (D_800E6280.unk_110A != 0) {
            D_800CA148 = 1;
            D_80122EC8 += 1;
            D_800E6280.unk_110A = 0;
        }
        Default_Disp();
        return;
    case 7:
        normal_date_speak_012();
        if (D_800E6280.unk_110A != 0) {
            func_80141964();
            D_800E6280.unk_110A = 0;
            D_80122EC8 = 8;
        }
        Default_Disp();
        return;
    case 8:
        func_801419F8();
        if (D_800E6280.unk_110A != 0) {
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
                func_80046500();
                break;
            }
            D_800E6280.unk_110A = 0;
        }
        Default_Disp();
        return;
    case 9:
        normal_date_speak_012();
        if (D_800E6280.unk_110A != 0) {
            D_80122EC8 = 0xA;
            D_800E6280.unk_110A = 0;
        }
        Default_Disp();
        return;
    case 10:
        func_801419F8();
        if (D_800E6280.unk_110A != 0) {
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
                func_80046500();
                break;
            }
            D_800E6280.unk_110A = 0;
        }
        Default_Disp();
        return;
    case 11:
        normal_date_speak_012();
        if (D_800E6280.unk_110A != 0) {
            D_80122EC8 = 8;
            D_800E6280.unk_110A = 0;
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
        func_80044750(0xBF);
        func_800674B0();
        Default_Disp();
        D_80122EC8 += 1;
        break;
    case 18:
        if (func_80044E8C() == 1) {
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
            func_80072B5C(j);
        }
        Default_Disp();
        return;
    default:
        func_80046500();
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
    func_8004F870(0, 3, x, y, w, h);
    func_8006BC28(0);
    func_8006BD6C(1);
}

s32 func_801419F8(void) {
    func_8004F984(0, D_8011ECF6, D_8011ECFA);
    func_8004FC10(0);
    func_80050B54(0);
    func_8006BD6C(1);
    func_8006BA40();
    if (D_800E6280.unk_F88 & 0x20) {
        if (func_8004ECB4() != 0) {
            if (D_800E6280.unk_F90[0].unk_06 != 0) {
                D_80122CDC = 0;
            } else if (D_800E6280.unk_F90[2].unk_06 != 0) {
                D_80122CDC = 1;
            } else if (D_800E6280.unk_F90[4].unk_06 != 0) {
                D_80122CDC = 2;
            } else {
                return 0;
            }
            ((DisplayActor44 *)D_8011ECD0)->unk_03 &= 0xFF7F;
            func_8004E9F4(1);
            func_8004284C();
        }
    }
}

void func_80141AE8(void) {
    D_80122EC8 = 9;
    D_800CA148 = 2;
    D_800CA14C = 0;
    k_reset(1);
}

void func_80141B28(void) {
    D_80122EC8 = 0xB;
    D_800CA148 = 1;
    D_800CA14C = 0;
    k_reset(1);
}
