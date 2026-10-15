#include "common.h"
#include "ovl/BUNKA_SD.h"

s32 func_801369D0(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80044890(1, 0xBEFB, 0xBEE2, 0xD19D, 0xD170, 0xD164);
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
    } else if (D_800E6280.unk_110D == 1) {
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
        if (func_80044F94(1) == 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
    } else if (D_800E6280.unk_110D == 3) {
        cd_read_sectors(0x6E, 0x80180000, 0x4EAF);
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
    } else if (D_800E6280.unk_110D == 4) {
        if (cd_read_sync_status() & 1) {
            D_8013CEC0 = D_8013C750;
            D_8013CEC4 = D_8013C7B4;
            D_800E6280.unk_F5F = 0xE;
            func_8004284C();
        }
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801369D0", func_80136B3C);

void func_80136BFC(void) {
    func_80066C08(2);
    func_80064F48();
    func_80066334();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80134260();
        func_80044750(0x7F);
        break;
    case 1:
        func_801369D0();
        break;
    case 2:
        func_80044750(0x200);
        func_80136EDC();
        break;
    case 3:
        func_80134418();
        break;
    case 4:
        func_80138898(0x78U);
        break;
    case 5:
        func_801344A8();
        break;
    case 6:
        func_801371BC();
        break;
    case 7:
        func_80134418();
        break;
    case 8:
    case 9:
    case 10:
    case 11:
        func_801373D0();
        break;
    case 12:
        func_80044750(0xC1);
        func_8004284C();
        break;
    case 13:
        func_801344A8();
        break;
    case 14:
        func_8013731C();
        break;
    case 15:
        func_8013785C(0x3C);
        break;
    case 16:
        func_8013722C();
        break;
    case 17:
        func_8013785C(0x3C);
        break;
    case 18:
        func_80044750(0x201);
        func_8004284C();
        break;
    case 19:
        func_80134418();
        break;
    case 20:
        func_8013785C(0x78);
        break;
    case 21:
        func_801344A8();
        break;
    case 22:
        func_8013785C(0x1E);
        break;
    case 23:
        func_8013731C();
        break;
    case 24:
        func_8013785C(0xF);
        break;
    case 25:
        func_8013731C();
        break;
    case 26:
        func_801372BC();
        break;
    case 27:
        func_80134418();
        break;
    case 28:
    case 29:
    case 30:
        func_801373D0();
        break;
    case 31:
        func_80044750(0x202);
        func_8004284C();
        break;
    case 32:
        func_8013785C(0x78);
        break;
    case 33:
    case 34:
        func_801373D0();
        break;
    case 35:
        func_80044750(0xC1);
        func_80044750(0x500);
        func_8004E9F4(1);
        func_8004284C();
        break;
    case 36:
        if ((u32) D_800E6280.unk_1104.u >= 0x1FU) {
            func_8004284C();
        }
        break;
    case 37:
        func_801344A8();
        break;
    case 38:
        func_80134314();
        break;
    default:
        func_80046500();
        break;
    }
    D_800E6280.unk_1104.u += 1;
}

void func_80136EDC(void) {
    func_80136F3C();
    k_speed_set(5);
    D_8013CEBC = 0;
    D_8013CEC8 = 1;
    D_8013CECC = 0;
    D_8013CED0 = 0;
    D_8013CED4 = 0;
    D_8013CED8 = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801369D0", func_80136F3C);

void func_801371BC(void) {
    func_801377D4(0, 0x44, 1);
    func_801377D4(1, 0x18, 1);
    func_801377D4(2, 0, 1);
    func_801377D4(3, 1, 1);
    func_801377D4(0x1F, 0x41, 1);
    func_8004284C();
}

void func_8013722C(void) {
    func_801377D4(1, 0xFF, 0);
    func_801377D4(2, 0xFF, 0);
    func_801377D4(3, 0xFF, 0);
    func_801377D4(4, 0xFF, 0);
    func_801377D4(5, 0xFF, 0);
    func_801377D4(0, 0x44, 1);
    func_801377D4(0x1F, 0x42, 1);
    func_8004284C();
}

void func_801372BC(void) {
    func_801377D4(0, 0x44, 1);
    func_801377D4(1, 0x26, 1);
    func_801377D4(2, 0x27, 1);
    func_801377D4(0x1F, 0x43, 1);
    func_8004284C();
}

void func_8013731C(void) {
    if (D_800E6280.unk_1104.u == 1) {
        func_80137824(D_8013C814[D_8013CECC]);
        D_800E6280.unk_1100 = 1;
        func_80137620();
        return;
    }
    func_80137620();
    if (D_800E6280.unk_1100 == 0) {
        D_8013CECC += 1;
        D_8013CEC8 = 1;
        D_8013CED0 = 0;
        D_8013CED4 = 0;
        k_reset(1);
        func_8004284C();
    }
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7380;
extern s32 D_800E7384;
extern s32 D_8013CE8C;
/* end bulk_m2c declarations */
void func_801373D0(void) {
    s32 temp_a1;
    s32 temp_fp;
    s32 temp_t5;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_s0;
    s32 var_t2;
    s32 var_v0;
    u8 *temp_s1;

    if ((D_800E7380 == 0) && (D_8013CED8 = 0, (D_8013CED4 != 0))) {
        switch (D_8013CED4) {
        case 1:
            D_800E7384 = 0;
            goto block_7;
        case 2:
            D_8013CECC += 1;
            D_8013CED0 = 0;
            D_8013CED4 = 0;
            D_8013CEC8 = 1;
            k_reset(1);
            func_8004284C();
            break;
        }
    } else {
block_7:
        var_v0 = D_8013CED4;
        temp_fp = *(&D_8013CE8C + (D_8013CECC * 4));
        if (var_v0 == 0) {
            var_v0 = 1;
        }
        D_8013CED4 = var_v0;
        if (var_v0 == 1) {
            var_s0 = D_8013CEC8;
            if (var_s0 < 0xFFFF) {
                var_t2 = var_s0 * 8;
                do {
                    temp_s1 = temp_fp + var_t2;
                    temp_v1 = var_s0;
                    if (D_8013CED0 == (*(s32 *)temp_s1)) {
                        temp_v0 = *(s32 *)((unsigned char *)temp_s1 + (4));
                        if (temp_v0 == 0x0FFFFFFF) {
                            D_8013CED4 = 2;
                            var_s0 = 0xFFFF;
                        } else {
                            temp_t5 = (s32) (temp_v0 & 0xFF0000) >> 0x10;
                            temp_a1 = temp_v0 & 0xFF;
                            if (temp_a1 == 0xFE) {
                                func_801377D4(temp_t5, 0x50, 0);
                            } else {
                                func_801377D4(temp_t5, temp_a1, 1);
                            }
                            if (*(s32 *)((unsigned char *)temp_s1 + (4)) & 0xF0000000) {
                                func_80137824(D_8013C814[D_8013CECC]);
                                D_800E7380 = 1;
                                D_8013CED8 = 1;
                            }
                        }
                    } else {
                        var_s0 = 0xFFFF;
                        D_8013CEC8 = temp_v1;
                    }
                    var_s0 += 1;
                    var_t2 = var_s0 * 8;
                } while (var_s0 < 0xFFFF);
            }
        }
        D_8013CED0 += 1;
        if (D_8013CED8 == 1) {
            func_80137620();
        }
    }
}

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AAD0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AAE8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AB00);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AB18);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AB30);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AB48);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AB5C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AB70);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AB88);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013ABA0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013ABBC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013ABCC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013ABDC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013ABF4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AC10);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AC2C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AC48);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AC60);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AC78);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AC94);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013ACB0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013ACBC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013ACD0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013ACD8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013ACF4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AD10);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AD18);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AD20);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AD30);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AD40);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AD48);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AD58);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AD68);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AD70);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AD74);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AD78);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/801369D0.rodata", D_8013AD88);

void func_80137620(void) {
    s32 index;
    s32 *coordinates;

    if (D_800E6280.unk_1100 == 1) {
        D_800E6280.unk_1100++;
        if (D_8013CEBC != 21 && D_8013CEBC != 22) {
            func_8004E788(-86, 64, 0, (s32)"「", 0);
        }
        index = D_8013CEBC;
        if (D_8013CEBC < 100) {
            do {
                func_8004E788(-72, (s16)((index - D_8013CEBC) * 16 + 64), 0, (s32)(((u8 **)D_8013CEC0)[index] + 1), 0);
                if (((u8 **)D_8013CEC0)[index][0] == 103) {
                    if (!(D_800E6280.unk_F80 & 32)) {
                        coordinates = (s32 *)D_8013CEC4;
                        coordinates += *(u8 *)&D_800E6280.unk_110C;
                        func_80046290(coordinates[0], coordinates[1], D_800E6280.unk_F5F);
                        func_80044750(0x300);
                    }
                    (*(u8 *)&D_800E6280.unk_110C) += 2;
                    D_8013CEBC = index + 1;
                    index = 100;
                }
                index++;
            } while (index < 100);
        }
    }
    if (func_800460EC() == 2 && func_8004ECB4() != 0) {
        D_800E6280.unk_1100 = 0;
    }
    func_8007EDF8();
    func_8004EC24();
}

void func_801377D4(s32 arg0, s32 arg1, s32 arg2) {
    u8 *p;

    if (arg2 == 1) {
        p = D_8011ECD0 + arg0 * 0x44;
        p[0x1983] = 0x80;
    } else {
        p = D_8011ECD0 + arg0 * 0x44;
        p[0x1983] = 0;
    }
    *(s16 *)(p + 0x1998) = arg1;
}

void func_80137824(s32 arg0) {
    k_disp_start(set_kanji_string(-0x80, 0x30, 0, arg0, 0));
}

void func_8013785C(u32 arg0) {
    if (arg0 < (u32) D_800E6280.unk_1104.w) {
        func_8004284C();
    }
}
