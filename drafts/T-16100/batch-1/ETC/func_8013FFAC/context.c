#include "common.h"
#include "ovl/ETC.h"

void func_8013FF80(void) {
    if (D_800E6280.unk_110A == 0) {
        func_8013FFAC();
    }
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7208;
extern s32 D_8014DB90;
extern s32 D_8014E070;
extern s32 D_801500C0;
extern s32 D_801500D8;
extern s32 D_801500DC;
extern s8 D_800B3D00;
extern s8 D_800E62B6;
extern s8 D_800E62B7;
extern s8 D_800E62B8;
extern u32 D_800E7384;
extern u8 D_800E62BA;
extern u8 D_800E71DF;
extern u8 D_800E738D;
/* end bulk_m2c declarations */
void func_8013FFAC(void) {
    s32 sp54[16];
    s32 sp58;
    s32 *temp_v1;
    s32 *var_v1;
    s32 temp_v0;
    s32 var_v0;
    u8 temp_t3;
    u8 temp_v0_2;
    u8 *temp_v1_2;

    switch (D_800E738D) {
    case 0:
        draw2d3d(1, 0);
        D_800E62B7 = 0;
        D_800E62B6 = 0;
        D_800E62B8 = 0;
        func_80048DAC(1);
        func_80048EB8(0);
        func_8006BD6C(0);
        func_80041584();
        func_8004E58C();
        back_clear_switch(1);
        tpage_buf_clear();
        D_801500C0 = 0;
        func_801408F0();
        D_801500C4 = *(&D_8014E070 + (((u8) D_800E71DF % 13U) * 4));
        func_80044890(0, 0xBF98, 0xBF98, 0xCFA7, 0xCF8B, 0xCF83);
        D_800E738D += 1;
    default:
        break;
    case 1:
        if (func_80044E8C() == 1) {
            func_80044750(0x201);
            func_80044890(1, 0xC5B7, 0xC594, 0xD6B2, 0xD664, 0xD643);
            D_800E738D += 1;
        }
        break;
    case 2:
        if (func_80044E8C() == 1) {
            func_800462C8(1U, 0x801FA800, 0xBE09);
            cd_read_sectors(0x69U, 0x80180000, 0xB389);
            D_800E738D += 1;
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            D_800E738D += 1;
        }
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            D_800E738D += 1;
        }
        break;
    case 5:
        draw2d3d(1, 1);
        func_8013F980();
        func_80061710();
        func_80061790();
        func_800618B0();
        load_palette(0x80180000, 0x1F, 1, 3, 0);
        load_palette(0x80180200, 0x1E, 1, 3, 0);
        load_palette(0x80180400, 0x1D, 1, 3, 0);
        load_tpage_buf_lock(0xF, 0x801B0600, 1);
        var_v0 = 0x80198600;
        var_v1 = &sp54[0];
        do {
            *var_v1 = var_v0;
            var_v0 += 0x4000;
            var_v1 += 4;
        } while (var_v0 < 0x801B0600);
        func_8004A1A0((s32) &sp54[0], 1, 6, 3, 2, 0x1FE, -0xA0, -0x7D, 0xC, 0x15);
        D_801220F4 = 0x01000000;
        D_80122106 = 0;
        D_80122105 = 0;
        D_80122104 = 0;
        D_800E62BA = 0;
        temp_v0 = 2 << 0xE;
        temp_v1 = &(&sp54[0])[2];
        sp54[0] = 0x80180600;
        sp58 = 0x80184600;
        (*(s32 *)temp_v1) = temp_v0 + 0x80180600;
        *(s32 *)((unsigned char *)temp_v1 + (4)) = (s32) (temp_v0 + 0x80184600);
        *(s32 *)((unsigned char *)temp_v1 + (12)) = (s32) (temp_v0 + 0x8018C600);
        *(s32 *)((unsigned char *)temp_v1 + (8)) = (s32) (temp_v0 + 0x80188600);
        func_8004A1A0((s32) &sp54[0], 0, 6, 3, 2, 0x1FF, -0xA0, -0x7D, 0xD, 0x18);
        D_801220D0 = 0x80000000;
        D_801220E1 = 0x80;
        D_801220E0 = 0x80;
        D_800E62BA = 0;
        D_801220E2 = 0x80;
        func_800AE0A0((void *)0x801E2000, 0x80180600, 0x18000);
        temp_v1_2 = (D_800E71DF * 8) + &D_8014DB90;
        func_80046290((*(s32 *)temp_v1_2), *(s32 *)((unsigned char *)temp_v1_2 + (4)), (s32) D_800E71DF);
        func_80044750(0x300);
        D_800E738D += 1;
        break;
    case 6:
        if (func_800460EC() & 4) {
            D_800E738D += 1;
        }
        break;
    case 7:
        func_80140AE4();
        if ((u8) D_800E62BA < 0x81U) {
            D_80122106 = D_800E62BA;
            D_80122105 = D_800E62BA;
            D_80122104 = D_800E62BA;
            D_800E62BA += 1;
        }
        if (func_800460EC() & 2) {
            D_801500DC += 1;
            D_800B3CF8 = 0;
            D_800B3CFC = 0;
            D_800B3D00 = 0;
            D_800E7384 = 0;
            D_800E738D += 1;
        }
        break;
    case 8:
        if ((u8) D_800E62BA < 0x81U) {
            temp_v0_2 = D_800E62BA & 0xFF;
            D_80122106 = temp_v0_2;
            D_80122105 = temp_v0_2;
            D_80122104 = temp_v0_2;
            D_800E62BA += 1;
        }
        if (D_800E71DF == 0) {
            if ((u32) D_800E7384 >= 0x1EU) {
                func_8013F320(0);
                if (D_8015006C >= 3) {
                    D_8015006C = 0;
                    D_800E738D += 1;
                }
            }
        } else if (D_800E71DF == 9) {
            if ((u32) D_800E7384 >= 0x1EU) {
                func_8013F320(0);
                if (D_8015006C >= 3) {
                    D_8015006C = 0;
                    D_800E738D += 1;
                }
            }
        } else if (D_800E71DF == 0xC) {
            if ((u32) D_800E7384 >= 0x1EU) {
                func_8013F320(0);
                if (D_8015006C >= 3) {
                    D_8015006C = 0;
                    D_800E738D += 1;
                }
            }
        } else {
            D_800E738D += 1;
        }
        D_800E7384 += 1;
        break;
    case 9:
        D_80122106 = D_800E62BA;
        D_80122105 = D_800E62BA;
        D_80122104 = D_800E62BA;
        temp_t3 = (D_800E62BA + 1) & 0xFF;
        D_800E62BA = temp_t3;
        if (temp_t3 >= 0x80U) {
            D_800E738D += 1;
        }
        break;
    case 10:
        func_8013FC08();
        D_800E738D += 1;
        break;
    case 11:
        D_801220F4 = 0x01000000;
        D_801220D0 = 0x01000000;
        D_800E738D += 1;
        break;
    case 12:
        D_801500D4 = 0x800;
        func_8013FA7C();
        D_801220F4 = 0x80000000;
        D_800E738D += 1;
        break;
    case 13:
        func_80140AE4();
        if (D_800E7208 & 0x20) {
            if ((D_801500D8 == 0) || (D_801500D8 == 2)) {
                func_8013EEFC();
            }
            D_800E738D += 1;
        }
        break;
    case 14:
        if (D_80150070 >= 3) {
            if (D_801500D4 >= 0xC00) {
                D_80150070 = 0;
                func_80042808();
            } else {
                goto block_50;
            }
        } else {
block_50:
            if ((D_80150070 == -1) && (D_801500D4 >= 0xC00)) {
                func_80042808();
            } else {
                func_8013E89C(1);
                D_801500D4 += 8;
                func_8013FA7C();
            }
        }
        break;
    }
}

void func_801407F0(void) {
    D_800E6280.unk_1100 += 1;
    switch (D_800E6280.unk_1109) {
    case 0:
        func_8013FF80();
        break;
    case 1:
        func_8013BF34();
        break;
    case 2:
        func_8013CF2C();
        break;
    case 0xFF:
        func_80140FD8();
        break;
    }
    func_80066334();
    if (D_800E6280.unk_1109 != 0xFF) {
        if (D_801500E0 == 0 && (D_800E6280.unk_F88 & 0x800)) {
            func_8013FC08();
            D_801500D4 = 0x800;
            func_8013FA7C();
            D_801500E0 = 1;
            func_80042908(2);
        }
    }
}
void func_801408F0(void) {
    D_800E6280.unk_1228[10] = 0;
    D_800E6280.unk_1228[11] = 0;
    D_800E6280.unk_1228[12] = 0;
    D_800E6280.unk_1228[13] = 0;
    D_800E6280.unk_1228[14] = 0;
    D_800E6280.unk_1228[42] = 0;
    D_800E6280.unk_1228[43] = 0;
    D_800E6280.unk_1228[44] = 0;
    D_800E6280.unk_1228[45] = 0;
    D_800E6280.unk_1228[46] = 0;
    D_800E6280.unk_1228[0] = 0;
    D_800E6280.unk_1228[1] = 0;
    D_800E6280.unk_1228[2] = 0;
    D_800E6280.unk_1228[3] = 0;
    D_800E6280.unk_1228[4] = 0;
    D_800E6280.unk_1228[32] = 0;
    D_800E6280.unk_1228[33] = 0;
    D_800E6280.unk_1228[34] = 0;
    D_800E6280.unk_1228[35] = 0;
    D_800E6280.unk_1228[36] = 0;
    D_800E6280.unk_1228[16] = 0;
    D_800E6280.unk_1228[17] = 0;
    D_800E6280.unk_1228[18] = 0;
    D_800E6280.unk_1228[19] = 0;
    D_800E6280.unk_1228[20] = 0;
    D_800E6280.unk_1228[48] = 0;
    D_800E6280.unk_1228[49] = 0;
    D_800E6280.unk_1228[50] = 0;
    D_800E6280.unk_1228[51] = 0;
    D_800E6280.unk_1228[52] = 0;
    D_800E6280.unk_1228[5] = -1;
    D_800E6280.unk_1228[6] = -1;
    D_800E6280.unk_1228[7] = -1;
    D_800E6280.unk_1228[8] = -1;
    D_800E6280.unk_1228[9] = -1;
    D_800E6280.unk_1228[37] = -1;
    D_800E6280.unk_1228[38] = -1;
    D_800E6280.unk_1228[39] = -1;
    D_800E6280.unk_1228[40] = -1;
    D_800E6280.unk_1228[41] = -1;
    D_800E6280.unk_1228[21] = -1;
    D_800E6280.unk_1228[22] = -1;
    D_800E6280.unk_1228[23] = -1;
    D_800E6280.unk_1228[24] = -1;
    D_800E6280.unk_1228[25] = -1;
    D_800E6280.unk_1228[53] = -1;
    D_800E6280.unk_1228[54] = -1;
    D_800E6280.unk_1228[55] = -1;
    D_800E6280.unk_1228[56] = -1;
    D_800E6280.unk_1228[57] = -1;
    if ((func_8013F3F8() == 1) && (D_80150060 == 0xFF)) {
        D_800E6280.unk_1228[7] = 0;
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013FF80", func_80140AE4);

s32 func_80140BC4(void) {
    s32 r;
    r = 0;
    if (D_800E6280.unk_1124 == 0) {
        if (D_800E6280.unk_53C[0] >= 2U && D_800E6280.unk_53C[1] >= 2U && D_800E6280.unk_53C[2] >= 2U && D_800E6280.unk_53C[3] >= 2U && D_800E6280.unk_53C[4] >= 2U && D_800E6280.unk_53C[5] >= 2U && D_800E6280.unk_53C[6] >= 2U && D_800E6280.unk_53C[7] >= 2U && D_800E6280.unk_53C[8] >= 2U && D_800E6280.unk_53C[9] >= 2U && D_800E6280.unk_53C[10] >= 2U && D_800E6280.unk_53C[11] >= 2U && D_800E6280.unk_53C[12] >= 2U) {
            return 2;
        }
        if (D_800E6280.unk_53C[0] != 0 && D_800E6280.unk_53C[1] != 0 && D_800E6280.unk_53C[2] != 0 && D_800E6280.unk_53C[3] != 0 && D_800E6280.unk_53C[4] != 0 && D_800E6280.unk_53C[5] != 0 && D_800E6280.unk_53C[6] != 0 && D_800E6280.unk_53C[7] != 0 && D_800E6280.unk_53C[8] != 0 && D_800E6280.unk_53C[9] != 0 && D_800E6280.unk_53C[10] != 0 && D_800E6280.unk_53C[11] != 0 && D_800E6280.unk_53C[12] != 0) {
            return 1;
        }
        return 0;
    }
    return r;
}

s32 func_80140DC8(void) {
    s32 i;
    s32 n;

    n = 0;
    for (i = 0; i < 83; i++) {
        if (D_800E6280.unk_56C[i] != 0) {
            n++;
        }
    }
    if (n >= 30) {
        return 1;
    }
    return 0;
}

s32 func_80140E80(void) {
    s32 i;
    s32 n;

    n = 0;
    for (i = 0; i < 13; i++) {
        if (D_800E6280.unk_1BC[i].unk_0C.f.b1) {
            n++;
        }
    }
    if (D_800E6280.unk_1BC[11].unk_0C.f.f9 != 0) {
        n++;
    }
    if (n == 13) {
        return 1;
    }
    return 0;
}
