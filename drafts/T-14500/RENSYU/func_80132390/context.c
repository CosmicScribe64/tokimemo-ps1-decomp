#include "ovl/RENSYU.h"

typedef struct {
    void (*f[8])();
} FnTbl8; /* size 0x20 */
extern FnTbl8 D_801342D4;

void func_80132040(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl8 tbl;

    tbl = D_801342D4;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/RENSYU/nonmatchings/RENSYU/80132040", func_801320E0);

/* bulk_m2c declarations (T-11190) */
extern s16 D_800E643E;
extern s16 D_800E6442;
extern s16 D_800E666E;
extern s16 D_800E6672;
extern s32 D_800E6678;
extern s32 D_800E66B0;
extern s32 D_800E67CC;
extern s32 D_801342F4;
extern s32 D_80134328;
extern u16 D_800E6374;
extern u16 D_800E67CE;
extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E644A;
extern u8 D_800E66C3;
extern u8 D_800E66EB;
extern u8 D_800E66FC;
extern u8 D_800E738A;
void func_80133E5C(void);
extern s32 sp;
/* end bulk_m2c declarations */
void func_80132390(void) {
    s32 sp74;
    s32 spA8;
    s32 spA4;
    s32 sp70;
    typedef struct { s32 w[12]; } M2cBlock30; /* m2c memcpy of 48 bytes */
    typedef struct { s32 w[13]; } M2cBlock34; /* m2c memcpy of 52 bytes */
    s32 spB4;
    s32 sp78[13];
    s32 sp44[13];
    GameState *var_s1;
    s32 temp_a0;
    s32 var_s0;
    u32 temp_t6;
    u32 temp_t8;
    u32 var_v0;

    *(M2cBlock30 *)(&sp78) = *(M2cBlock30 *)(&D_801342F4);
    *(s32 *)((unsigned char *)(&sp78) + (48)) = (s32) *(s32 *)((unsigned char *)(&D_801342F4) + (48));
    *(M2cBlock34 *)(&sp44) = *(M2cBlock34 *)(&D_80134328);
    switch (D_800E738A) {
    case 0:
        temp_a0 = func_8008647C();
        if ((((u32) (D_800E67CC * 4) >> 0x1F) == 1) && (D_800E62BE == ((u32) (D_800E67CC << 0x14) >> 0x19)) && (D_800E62BF == ((u16) D_800E67CC >> 0xC)) && (D_800E62C0 == (D_800E67CE & 0x1F))) {
            D_801342D0 = 0xFF;
            func_80042940(6);
        } else if (((u32) ((u32) (D_800E66B0 << 0x14) >> 0x1D) >= 3U) && (D_800E66C3 == 0)) {
            D_800E66C3 = 1;
            D_801342D0 = 0xB;
            if ((u8) D_800E62BF >= 6U) {
                if ((u8) D_800E62BF < 0xAU) {
                    func_80062CD0(spA4);
                } else {
                    goto block_13;
                }
            } else {
block_13:
                func_80062CD0(sp70);
            }
            func_8004284C();
        } else if (((u8) D_800E66EB >= 0x41U) && (D_800E66FC == 0)) {
            D_800E66FC = 1;
            D_801342D0 = 0xC;
            if ((u8) D_800E62BF >= 6U) {
                if ((u8) D_800E62BF < 0xAU) {
                    func_80062CD0(spA8);
                } else {
                    goto block_21;
                }
            } else {
block_21:
                func_80062CD0(sp74);
            }
            func_8004284C();
        } else {
            spB4 = temp_a0;
            if (func_8008667C(temp_a0) < 2) {
                D_801342D0 = (u8) temp_a0;
                var_v0 = (u16) D_800E6374 >> 0xC;
                var_s1 = &D_800E6280;
                var_s0 = 0;
                do {
                    if ((var_v0 == ((u8) var_s1->unk_1BC[0].unk_0C.b[2] >> 4)) && (((u32) (var_s1->unk_1BC[0].unk_0C.w << 0x1E) >> 0x1F) == 1)) {
                        if (func_8008667C(var_s0) < 2) {
                            D_801342D0 = (u8) var_s0;
                        }
                        if ((((u8) D_800E644A >> 4) == 0xA) && (((u16) D_800E6374 >> 0xC) == 0xA) && (((u32) (D_800E6678 << 0x1E) >> 0x1F) == 1)) {
                            if ((func_8008667C(0) < 2) && (func_8008667C(0xA) >= 2)) {
                                D_801342D0 = 0;
                            } else if ((func_8008667C(0) >= 2) && (func_8008667C(0xA) < 2)) {
                                D_801342D0 = 0xA;
                            } else if ((func_8008667C(0) < 2) && (func_8008667C(0xA) < 2)) {
                                if ((D_800E643E + D_800E6442) >= (D_800E666E + D_800E6672)) {
                                    D_801342D0 = 0;
                                } else {
                                    D_801342D0 = 0xA;
                                }
                            }
                        }
                    }
                    var_s0 += 1;
                    temp_t8 = (u16) D_800E6374 >> 0xC;
                    var_v0 = temp_t8;
                    var_s1 += 0x38;
                } while (var_s0 != 0xB);
                if (temp_t8 == ((u8) *(&D_800E644A + (D_801342D0 * 0x38)) >> 4)) {
                    func_80133E5C();
                } else if (((u8) D_800E62BF >= 6U) && ((u8) D_800E62BF < 0xAU)) {
                    func_80062CD0(*(s32 *)((unsigned char *)(sp + (D_801342D0 * 4)) + (120)));
                } else {
                    func_80062CD0(*(s32 *)((unsigned char *)(sp + (D_801342D0 * 4)) + (68)));
                }
                func_8004284C();
            } else {
                D_801342D0 = 0xFF;
                func_80042940(6);
            }
        }
        break;
    case 1:
        func_8004284C();
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            temp_t6 = (u16) D_800E6374 >> 0xC;
            switch (temp_t6) {
            case 6:
            case 7:
            case 8:
                dec_bg_cd_read(0x4097, 1);
                break;
            case 9:
                dec_bg_cd_read(0x40A0, 1);
                break;
            case 10:
                dec_bg_cd_read(0x40A9, 1);
                break;
            default:
                func_80046500();
                break;
            }
            func_8004284C();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_80065F34(1);
            D_80125CA0 = 0xC0;
            dec_bg_show_set(1, func_8005751C(1));
            func_80044750(0xC1);
            func_80044890(1, 0xC043, 0xC01D, D_800B3688[D_801342D0], D_800B36C8[D_801342D0], D_800B3708[D_801342D0]);
            func_8004284C();
        }
        break;
    case 4:
        normal_date_bg_fadein();
        break;
    case 5:
        func_80044750(0x71);
        func_80042808();
        break;
    case 6:
        func_8004284C();
        break;
    case 7:
        D_800CA148 = 0x10;
        D_800CA14C = 0;
        func_8004284C();
        /* fallthrough */
    case 8:
        normal_date_speak();
        break;
    case 9:
        func_80042908(3);
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}

INCLUDE_ASM("asm/ovl/RENSYU/nonmatchings/RENSYU/80132040", func_80132A28);

void func_80132F10(void) {
    s32 var_s0;

    switch (D_800E6280.unk_110A) {
    case 0:
        switch ((u16) D_800E6280.unk_0F4.h >> 0xC) {
        case 6:
            func_8005742C(0x417A, 1);
            break;
        case 7:
            func_8005742C(0x4183, 1);
            break;
        case 8:
            func_8005742C(0x419E, 1);
            break;
        case 9:
            func_8005742C(0x4195, 1);
            break;
        case 10:
            func_8005742C(0x418D, 1);
            break;
        default:
            func_80046500();
            break;
        }
        func_8004284C();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            switch ((u16) D_800E6280.unk_0F4.h >> 0xC) {
            case 6:
                cd_read_sectors(0x1BU, 0x80197000, 0xB03E);
                break;
            case 7:
                cd_read_sectors(0x1AU, 0x80197000, 0xAFE7);
                break;
            case 8:
                cd_read_sectors(0x1BU, 0x80197000, 0xB01C);
                break;
            case 9:
                cd_read_sectors(0x1BU, 0x80197000, 0xB001);
                break;
            case 10:
                cd_read_sectors(0x1BU, 0x80197000, 0xAFCC);
                break;
            default:
                func_80046500();
                break;
            }
            func_80065F34(1);
            func_8004284C();
        }
        break;
    case 2:
        func_8007E81C();
        break;
    case 3:
        func_8004E9F4(1);
        func_8004E788(-0x80, 0x32, 0, (s32) "試合中", 0);
        func_8004E884(1);
        func_8004284C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            switch ((u16) D_800E6280.unk_0F4.h >> 0xC) {
            case 6:
                func_801340B0();
                break;
            case 7:
                func_80133FB4();
                break;
            case 8:
                func_8013405C();
                break;
            case 9:
                func_80134008();
                break;
            case 10:
                func_80133F60();
                break;
            default:
                func_80046500();
                break;
            }
            var_s0 = 0;
            do {
                func_80043914(D_801343E0, (var_s0 + 0x10) & 0xFF, 1, 1, var_s0);
                var_s0 += 1;
            } while (var_s0 != 5);
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}

INCLUDE_ASM("asm/ovl/RENSYU/nonmatchings/RENSYU/80132040", func_8013325C);

INCLUDE_ASM("asm/ovl/RENSYU/nonmatchings/RENSYU/80132040", func_80133628);

INCLUDE_ASM("asm/ovl/RENSYU/nonmatchings/RENSYU/80132040", func_80133990);

INCLUDE_ASM("asm/ovl/RENSYU/nonmatchings/RENSYU/80132040", func_80133CB8);

void func_80133E5C(void) {
    switch (D_801342D0) {
    case 0:
        switch ((u32)D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) {
        case 6:
        case 7:
            func_80062CD0(0x54FE);
            break;
        case 8:
            func_80062CD0(0x552B);
            break;
        case 9:
            func_80062CD0(0x5558);
            break;
        case 10:
            func_80062CD0(0x5585);
            break;
        }
        break;
    case 4:
        func_80062CD0(0x66EC);
        break;
    case 5:
        func_80062CD0(0x5FE4);
        break;
    case 6:
        func_80062CD0(0x625A);
        break;
    case 10:
        func_80062CD0(0x6962);
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80133F60(void) {
    D_801343D0 = 0x801A42D4;
    D_801343D4 = 0x801A42E0;
    D_801343D8 = 0x801A4330;
    D_801343DC = *(s16 *)0x801A4338;
    D_801343E0 = 0x80197000;
}

void func_80133FB4(void) {
    D_801343D0 = 0x801A3E3C;
    D_801343D4 = 0x801A3E48;
    D_801343D8 = 0x801A3E90;
    D_801343DC = *(s16 *)0x801A3E98;
    D_801343E0 = 0x80197000;
}

void func_80134008(void) {
    D_801343D0 = 0x801A42E4;
    D_801343D4 = 0x801A42F0;
    D_801343D8 = 0x801A4368;
    D_801343DC = *(s16 *)0x801A4370;
    D_801343E0 = 0x80197000;
}

void func_8013405C(void) {
    D_801343D0 = 0x801A432C;
    D_801343D4 = 0x801A4338;
    D_801343D8 = 0x801A4394;
    D_801343DC = *(s16 *)0x801A43A4;
    D_801343E0 = 0x80197000;
}

void func_801340B0(void) {
    D_801343D0 = 0x801A403C;
    D_801343D4 = 0x801A4048;
    D_801343D8 = 0x801A4098;
    D_801343DC = *(s16 *)0x801A40B0;
    D_801343E0 = 0x80197000;
}


