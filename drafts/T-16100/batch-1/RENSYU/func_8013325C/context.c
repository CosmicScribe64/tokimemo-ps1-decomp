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

INCLUDE_ASM("asm/ovl/RENSYU/nonmatchings/RENSYU/80132040", func_80132390);

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

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E68EE;
extern s32 D_801342CC;
extern s32 D_801342CD;
extern s32 D_80134378;
extern s8 D_801342C0;
extern u16 D_800E6374;
extern u8 D_800E738A;
extern u8 D_801342C4;
extern u8 D_801342C8;
/* end bulk_m2c declarations */
void func_8013325C(void) {
    s16 sp40[11];
    s32 sp64;
    s16 temp_v0;
    s32 temp_a0;
    s32 temp_s1;
    u32 temp_t1;
    u32 var_s0;
    u8 *temp_a0_2;
    u8 *temp_v1_2;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 temp_a1;
    u8 temp_a2;
    u8 temp_v1;
    u8 var_v1;

    sp40[0] = (s32) D_80134378;
    *(s32 *)((unsigned char *)(&sp40[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80134378) + (4));
    *(s32 *)((unsigned char *)(&sp40[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80134378) + (8));
    *(s32 *)((unsigned char *)(&sp40[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80134378) + (12));
    *(s32 *)((unsigned char *)(&sp40[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80134378) + (16));
    *(u16 *)((unsigned char *)(&sp40[0]) + (20)) = (u16) *(u16 *)((unsigned char *)(&D_80134378) + (20));
    switch (D_800E738A) {
    case 0:
        temp_t1 = (u16) D_800E6374 >> 0xC;
        temp_v0 = D_800E6280.unk_66C[temp_t1].unk_00 - (&sp40[0])[*(u16 *)((unsigned char *)(&D_800E6280.unk_66C[temp_t1]) + (2)) & 0xF];
        if (temp_v0 < -0x1E) {
            D_801342C4 = 0;
        } else {
            var_v1 = 1;
            if (temp_v0 < -0x14) {
                goto block_20;
            }
            var_v1 = 1;
            if (temp_v0 < -0xA) {
                goto block_20;
            }
            if (temp_v0 < 0) {
                var_v1 = 2;
                goto block_20;
            }
            var_v1 = 5;
            if (temp_v0 >= 0x1E) {
                goto block_20;
            }
            var_v1 = 4;
            if (temp_v0 >= 0x14) {
                goto block_20;
            }
            var_v1 = 4;
            if (temp_v0 >= 0xA) {
                goto block_20;
            }
            var_v1 = 3;
            if (temp_v0 >= 0) {
block_20:
                D_801342C4 = var_v1;
            }
        }
        func_8004284C();
        break;
    case 1:
        var_v0 = &D_801342C8;
        do {
            var_v0 += 1;
            *(s8 *)((unsigned char *)var_v0 + (-1)) = 0;
        } while ((u32) var_v0 < (u32) &D_801342CD);
        var_s0 = 0;
        if (D_801342C4 != 0) {
            var_v0_2 = &D_801342C8;
            do {
                var_s0 += 1;
                var_v0_2 += 1;
                *(s8 *)((unsigned char *)var_v0_2 + (-1)) = 1;
            } while (var_s0 < (u8) D_801342C4);
            var_s0 = 0;
        }
        temp_v1 = D_801342C4;
        temp_a0 = func_800AE0D0() & 0xF;
        sp64 = temp_a0;
        if (temp_v1 >= 3U) {
            temp_a2 = D_801342CC;
            D_801342CC = (u8) D_801342C8;
            *(u8 *)((unsigned char *)(&D_801342CC) + (-4)) = temp_a2;
        }
        if (temp_a0 > 0) {
            do {
                temp_s1 = func_800AE0D0() & 3;
                temp_v1_2 = &(&D_801342C8)[temp_s1];
                temp_a0_2 = &(&D_801342C8)[func_800AE0D0() & 3];
                temp_a1 = *temp_v1_2;
                var_s0 += 1;
                *temp_v1_2 = *temp_a0_2;
                *temp_a0_2 = temp_a1;
            } while (var_s0 != sp64);
        }
        if (((u16) D_800E6374 >> 0xC) == 9) {
            if (temp_v1 >= 3U) {
                D_801342C8 = 1;
            } else {
                D_801342C8 = 0;
            }
        }
        func_8004284C();
        break;
    case 2:
        func_80048F64(0x60);
        D_80120651 = 3;
        D_80120688 = 0x01000000;
        D_80120657 = 0x80;
        D_80120655 = 0x80;
        D_8012066A = 0x1000;
        D_8012066C = 0x1000;
        D_80120656 = 1;
        D_80120693 = ((u32) (*(&D_800E68EE + (((u16) D_800E6374 >> 0xC) * 4)) & 0xF) % 5U) + 0x10;
        D_80120676 = -0x60;
        D_8012067A = 0x20;
        D_8012065C = (u8 *) D_801343D4;
        D_80120660 = (u8 *) D_801343D8;
        D_80120684 = (u8 *) D_801343D0;
        D_80120664 = D_801343DC;
        D_801342C0 = 0;
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}

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


