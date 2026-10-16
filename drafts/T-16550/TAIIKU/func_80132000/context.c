#include "common.h"
#include "ovl/TAIIKU.h"

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7374;
extern s32 D_80122E90;
extern s32 D_80148EC4;
extern s32 sp;
extern s8 D_800E62BA;
extern u8 D_800E71DF;
extern u8 D_800E738A;
extern u8 D_80148EC0;
/* end bulk_m2c declarations */
void func_80132000(void) {
    s32 sp75;
    s32 sp76;
    typedef struct { s32 w[10]; } M2cBlock28; /* m2c memcpy of 40 bytes */
    s32 sp88;
    s32 sp84;
    u8 sp83;
    u8 sp82;
    u8 sp81;
    s32 sp74[1];
    s32 sp70;
    s32 sp3C[10];
    GameState *var_v0;
    s32 temp_v1;
    s32 var_a3_2;
    s8 var_a3;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 temp_v0_3;
    u8 *temp_a0_2;
    u8 *temp_v1_2;
    u8 *var_v1_3;
    u8 temp_a0;
    u8 temp_a0_3;
    u8 temp_a1;
    u8 temp_t3;
    u8 var_a0;
    u8 var_a1;
    u8 var_v1;
    u8 var_v1_2;

    *(M2cBlock28 *)(&sp3C) = *(M2cBlock28 *)(&D_80148EC4);
    *(s32 *)((unsigned char *)(&sp3C + 0x24) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80148EC4 + 0x24) + (4));
    func_80083808();
    switch (D_800E738A) {
    case 0:
        func_80044890(0, 0xBF98, 0xBF79, 0xD712, 0xD6C3, 0xD6B7);
        func_80085E30(1, 0);
        dec_bg_reset();
        set_dec_bri(0);
        func_8008585C();
        func_80048E78();
        k_reset(0);
        func_8006612C((s32) "フォークダンス");
        k_disp_start(1);
        func_80065F34(0);
        parameter_disp_switch(0);
        icon_disp_switch(0);
        message_window_init();
        func_800AE120(D_800E7374);
        D_80122E90 = 0;
        func_8004284C();
        break;
    case 1:
        var_v1 = 0;
        if (func_80044E8C() == 1) {
            var_v0 = &D_800E6280;
            var_a3 = 0;
            do {
                if (((u32) (var_v0->unk_1BC[0].unk_0C.w << 0x1E) >> 0x1F) == 1) {
                    *(&sp74 + var_v1) = var_a3;
                    var_v1 = (var_v1 + 1) & 0xFF;
                }
                var_a3 += 1;
                var_v0 += 0x38;
            } while (var_a3 != 0xB);
            if (var_v1 < 3U) {
                D_80122ECC = 2;
            } else {
                if (D_80122E90 == 0) {
                    sp83 = var_v1;
                    dec_bg_cd_read(0x4133, 1);
                }
                D_800E62BA = 0x80;
                sp83 = var_v1;
                temp_a0 = *(&sp74 + (func_800AE0D0() % var_v1));
                sp82 = temp_a0;
                var_v1_2 = var_v1;
                var_a0 = temp_a0;
                var_a1 = *(&sp74 + (func_800AE0D0() % var_v1_2));
                if (D_80122E90 != 0) {
                    var_a0 = sp75;
                    var_a1 = sp76;
                } else {
                    if (var_a0 == 0) {
                        do {
                            sp83 = var_v1_2;
                            sp81 = var_a1;
                            var_a0 = *(&sp74 + (func_800AE0D0() % var_v1_2));
                        } while (var_a0 == 0);
                    }
                    if ((var_a0 == var_a1) || (var_a1 == 0)) {
                        do {
loop_18:
                            sp83 = var_v1_2;
                            sp82 = var_a0;
                            var_a1 = *(&sp74 + (func_800AE0D0() % var_v1_2));
                            if (var_a0 == var_a1) {
                                goto loop_18;
                            }
                        } while (var_a1 == 0);
                    }
                    D_80122E90 += 1;
                }
                D_80148EC0 = 0;
                *(u8 *)((unsigned char *)(&D_80148EC0) + (1)) = var_a0;
                *(u8 *)((unsigned char *)(&D_80148EC0) + (2)) = var_a1;
                temp_v1 = func_800AE0D0() & 0x1F;
                sp84 = temp_v1;
                var_a3_2 = 0;
                if (temp_v1 > 0) {
                    do {
                        sp88 = var_a3_2;
                        sp70 = func_800AE0D0() % 3;
                        temp_v1_2 = &(&D_80148EC0)[sp70];
                        temp_a1 = *temp_v1_2;
                        var_a3_2 += 1;
                        temp_a0_2 = &(&D_80148EC0)[func_800AE0D0() % 3];
                        *temp_v1_2 = *temp_a0_2;
                        *temp_a0_2 = temp_a1;
                    } while (var_a3_2 < sp84);
                }
                func_8004284C();
            }
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80132970();
            func_8004500C(0, 0x200);
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(*(s32 *)((unsigned char *)(sp + (D_80148EC0 * 4)) + (60)));
            func_80065F34(1);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E71DF = D_80148EC0;
            func_80083474();
        }
        break;
    case 5:
        cd_read_sectors(2U, 0x801E8000, 0xB6DC);
        func_8004284C();
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            func_80084E4C();
            func_800847B8(D_80148EC0);
            func_80133BF0();
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = D_801491B0;
            D_800CA164 = D_801491B4;
            D_800CA168 = D_801491B8;
            func_8004284C();
        }
        break;
    case 7:
    case 8:
    case 9:
    case 12:
    case 15:
    case 17:
    case 22:
    case 25:
    case 27:
    case 32:
    case 35:
    case 40:
        normal_date_speak();
        break;
    case 10:
        temp_v0 = func_8008667C((s32) D_80148EC0);
        switch (temp_v0) {
        case 0:
        case 1:
            D_800CA148 = (D_80148EC0 * 3) + 5;
            break;
        case 2:
        case 3:
            D_800CA148 = (D_80148EC0 * 3) + 6;
            break;
        case 4:
            D_800CA148 = (D_80148EC0 * 3) + 7;
            break;
        }
        func_8004284C();
        break;
    case 11:
    case 20:
    case 30:
        normal_date_girl_in();
        break;
    case 13:
    case 23:
    case 33:
    case 38:
        normal_date_girl_out();
        break;
    case 14:
    case 24:
        D_800CA148 = 1;
        D_800CA14C = 0;
        func_8004284C();
        break;
    case 16:
        func_800847B8(*(u8 *)((unsigned char *)(&D_80148EC0) + (1)));
        func_8004284C();
        break;
    case 18:
        func_80062CD0(*(s32 *)((unsigned char *)(sp + (*(u8 *)((unsigned char *)(&D_80148EC0) + (1)) * 4)) + (60)));
        func_8004284C();
        break;
    case 19:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E71DF = *(u8 *)((unsigned char *)(&D_80148EC0) + (1));
            func_80083474();
        }
        break;
    case 21:
        temp_v0_2 = func_8008667C((s32) *(u8 *)((unsigned char *)(&D_80148EC0) + (1)));
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = (*(u8 *)((unsigned char *)(&D_80148EC0) + (1)) * 3) + 5;
            break;
        case 2:
        case 3:
            D_800CA148 = (*(u8 *)((unsigned char *)(&D_80148EC0) + (1)) * 3) + 6;
            break;
        case 4:
            D_800CA148 = (*(u8 *)((unsigned char *)(&D_80148EC0) + (1)) * 3) + 7;
            break;
        }
        func_8004284C();
        break;
    case 26:
        func_800847B8(*(u8 *)((unsigned char *)(&D_80148EC0) + (2)));
        func_8004284C();
        break;
    case 28:
        func_80062CD0(*(s32 *)((unsigned char *)(sp + (*(u8 *)((unsigned char *)(&D_80148EC0) + (2)) * 4)) + (60)));
        func_8004284C();
        break;
    case 29:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E71DF = *(u8 *)((unsigned char *)(&D_80148EC0) + (2));
            func_80083474();
        }
        break;
    case 31:
        temp_a0_3 = *(u8 *)((unsigned char *)(&D_80148EC0) + (2));
        if ((D_800E6280.unk_1BC[*(u8 *)((unsigned char *)(&D_80148EC0) + (2))].unk_0A >= 0x4C) || (*(u8 *)((unsigned char *)(&D_80148EC0) + (2)) = (u8) *(u8 *)((unsigned char *)(&D_80148EC0) + (2)), (func_8008667C((s32) temp_a0_3) == 4))) {
            func_80044750(0xBF);
            func_80083440(4U);
            func_80042940(0x26);
        } else {
            temp_v0_3 = func_8008667C((s32) *(u8 *)((unsigned char *)(&D_80148EC0) + (2)));
            switch (temp_v0_3) {
            case 0:
            case 1:
                D_800CA148 = (*(u8 *)((unsigned char *)(&D_80148EC0) + (2)) * 3) + 5;
                break;
            case 2:
            case 3:
                D_800CA148 = (*(u8 *)((unsigned char *)(&D_80148EC0) + (2)) * 3) + 6;
                break;
            case 4:
                D_800CA148 = (*(u8 *)((unsigned char *)(&D_80148EC0) + (2)) * 3) + 7;
                break;
            }
            func_8004284C();
        }
        break;
    case 34:
        D_800CA148 = 3;
        D_800CA14C = 0;
        func_8004284C();
        break;
    case 36:
    case 41:
        normal_date_bg_fadeout();
        break;
    case 37:
    case 42:
        var_v1_3 = &D_80148EC0;
        do {
            temp_t3 = *var_v1_3;
            var_v1_3 += 1;
            D_800E6280.unk_1BC[temp_t3].unk_06 += 1;
        } while (var_v1_3 != (&D_80148EC0 + 3));
        D_80122ECC = 2;
        break;
    case 39:
        D_800CA148 = 4;
        D_800CA14C = 0;
        func_8004284C();
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80132000", func_80132970);
