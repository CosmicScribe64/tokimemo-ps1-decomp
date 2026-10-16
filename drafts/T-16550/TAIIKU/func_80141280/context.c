#include "common.h"
#include "ovl/TAIIKU.h"

/* bulk_m2c declarations (T-11190) */
extern s32 D_80122ED0;
s32 func_801419F8();
void func_80141964();
void func_80141AE8();
void func_80141B28();
/* end bulk_m2c declarations */
void func_80141280(void) {
    s32 sp3C;
    s32 sp38;
    s32 *temp_v0;
    s32 *temp_v1;
    u8 *var_v1;

    sp38 = D_8014A088.v[0];
    *(s32 *)((unsigned char *)(&sp38) + (4)) = (s32) D_8014A088.v[1];
    *(s32 *)((unsigned char *)(&sp38) + (8)) = (s32) D_8014A088.v[2];
    *(s32 *)((unsigned char *)(&sp38) + (12)) = (s32) D_8014A088.v[3];
    *(s32 *)((unsigned char *)(&sp38) + (16)) = (s32) D_8014A088.v[4];
    *(s32 *)((unsigned char *)(&sp38) + (20)) = (s32) D_8014A088.v[5];
    switch (D_80122EC8) {
    case 0:
        func_80044750(0xBF);
        func_80044890(1, 0xC5B7, 0xC594, 0xD6B2, 0xD664, 0xD643);
        func_80085E30(1, 0);
        D_80122ECC = 0;
        func_8008585C();
        (*(s32 *)D_8014A070) = sp38;
        D_8014A070[1] = sp3C;
        temp_v1 = &(&sp38)[2];
        temp_v0 = &D_8014A070[2];
        *(s32 *)((unsigned char *)temp_v0 + (4)) = (s32) *(s32 *)((unsigned char *)temp_v1 + (4));
        *(s32 *)((unsigned char *)temp_v0 + (8)) = (s32) *(s32 *)((unsigned char *)temp_v1 + (8));
        *(s32 *)((unsigned char *)temp_v0 + (12)) = (s32) *(s32 *)((unsigned char *)temp_v1 + (12));
        (*(s32 *)temp_v0) = (*(s32 *)temp_v1);
        dec_bg_reset(2 * 4, 2);
        message_window_init();
        k_reset(0);
        func_8006612C((s32) "体育祭");
        func_80065F34(0);
        func_8006764C(0);
        hizuke_init();
        icon_disp_switch(0);
        parameter_disp_switch(0);
        Default_Disp();
        D_80122EC8 += 1;
        return;
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
            cd_read_sectors(1U, 0x801F0000, 0xBA16);
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
            func_800847B8(0U);
            func_80132B90();
            func_80141B70();
            D_800CA148 = 0;
            if (D_800E6280.unk_0F4.b[1] & 2) {
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
            switch (D_80122CDC) {
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
            switch (D_80122CDC) {
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
            return;
        }
        return;
    case 13:
        func_80133C80();
        if (D_80122ECC == 1) {
            D_80122EC8 = 0x10;
            return;
        }
        break;
    case 14:
        func_80142240();
        if (D_80122ECC == 1) {
            D_80122EC8 = 0x10;
            return;
        }
        break;
    case 15:
        func_80138A34();
        if (D_80122ECC == 1) {
            D_80122EC8 = 0x10;
            return;
        }
        break;
    case 16:
        func_80132000();
        if (D_80122ECC == 2) {
            D_80122EC8 += 1;
            return;
        }
        break;
    case 17:
        func_80044750(0xBF);
        func_800674B0();
        Default_Disp();
        D_80122EC8 += 1;
        return;
    case 18:
        if (func_80044E8C() == 1) {
            dec_bg_reset();
            set_dec_bri(0);
            var_v1 = D_80122DD0;
            do {
                var_v1 += 0x10;
                *(s32 *)((unsigned char *)var_v1 + (-12)) = 0;
                *(s32 *)((unsigned char *)var_v1 + (-8)) = 0;
                *(s32 *)((unsigned char *)var_v1 + (-4)) = 0;
                *(s32 *)((unsigned char *)var_v1 + (-16)) = 0;
            } while (var_v1 != &D_80122ED0);
            func_80072B5C(1);
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
            D_8011ECD0[3] &= 0xFF7F;
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
