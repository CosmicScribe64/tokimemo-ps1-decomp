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

/* bulk_m2c declarations (T-11190) */
extern s32 D_8013435C;
extern u16 D_800E6374;
extern u8 D_800E644A;
extern u8 D_800E652A;
extern u8 D_800E667A;
extern u8 D_800E71DF;
extern u8 D_800E738A;
extern s32 sp;
/* end bulk_m2c declarations */
void func_80132A28(void) {
    s32 sp30[7];
    u8 sp2F;
    u32 temp_t7;
    u8 var_a1;

    (*(s32 *)&sp30) = (s32) D_8013435C;
    *(s32 *)((unsigned char *)(&sp30) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8013435C) + (8));
    *(s32 *)((unsigned char *)(&sp30) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8013435C) + (4));
    *(s32 *)((unsigned char *)(&sp30) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8013435C) + (12));
    *(s32 *)((unsigned char *)(&sp30) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8013435C) + (16));
    *(u16 *)((unsigned char *)(&sp30) + (24)) = (u16) *(u16 *)((unsigned char *)(&D_8013435C) + (24));
    *(s32 *)((unsigned char *)(&sp30) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_8013435C) + (20));
    func_80083808();
    switch (D_800E738A) {
    case 0:
        if (func_80044E8C() == 1) {
            cd_read_sectors(7U, 0x801E8000, 0xB037);
            func_8004284C();
        }
        break;
    case 2:
    case 4:
    case 10:
        normal_date_speak();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_8004500C(1, 0);
            func_800847B8(D_801342D0);
            func_80132000();
            D_800CA160 = D_801342B0;
            D_800CA164 = D_801342B4;
            D_800CA168 = D_801342B8;
            D_800CA148 = *(s16 *)((unsigned char *)(sp + (D_801342D0 * 2)) + (48));
            D_800CA14C = 0;
            addr_init_bustup();
            D_800E71DF = D_801342D0;
            func_80083474();
            if (D_801342D0 == 0xC) {
                func_80083440(0U);
            }
            if (D_801342D0 == 0xB) {
                func_80044750(0x500);
                func_80047550();
            }
        }
        break;
    case 3:
        normal_date_girl_in();
        break;
    case 5:
        if (D_801342D0 == 0xB) {
            normal_date_girl_out();
            D_800CA148 = 0x5E;
        } else {
            D_800CA148 = 0x10;
            func_8004284C();
        }
        break;
    case 6:
        func_80044750(0xC1);
        Vblnk_Timer_Init();
        func_8004284C();
        break;
    case 7:
        if (D_801342D0 == 0xB) {
            normal_date_speak();
        } else {
            func_8004284C();
        }
        break;
    case 8:
        if (D_801342D0 == 0xB) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 9:
        func_8004500C(0, 6);
        if (D_801342D0 == 0xB) {
            func_8004284C();
            func_8004284C();
        } else {
            func_8004284C();
        }
        break;
    case 11:
        temp_t7 = (u16) D_800E6374 >> 0xC;
        switch (temp_t7) {
        case 6:
            if (((((u8) D_800E644A >> 4) == 6) && (D_801342D0 == 0)) || ((var_a1 = 0, (((u8) D_800E652A >> 4) == 6)) && (D_801342D0 == 4))) {
                var_a1 = 5;
            }
block_53:
            sp2F = var_a1;
            break;
        case 7:
            if (((((u8) D_800E644A >> 4) == 7) && (D_801342D0 == 0)) || ((var_a1 = 1, (((u8) D_800E652A >> 4) == 7)) && (D_801342D0 == 4))) {
                var_a1 = 6;
            }
            goto block_53;
        case 8:
            if (((((u8) D_800E644A >> 4) == 8) && (D_801342D0 == 0)) || (var_a1 = 2, (D_801342D0 == 5))) {
                var_a1 = 7;
            }
            goto block_53;
        case 9:
            if (((((u8) D_800E644A >> 4) == 9) && (D_801342D0 == 0)) || (var_a1 = 3, (D_801342D0 == 6))) {
                var_a1 = 8;
            }
            goto block_53;
        case 10:
            if (((((u8) D_800E644A >> 4) == 0xA) && (D_801342D0 == 0)) || ((var_a1 = 4, (((u8) D_800E667A >> 4) == 0xA)) && (D_801342D0 == 0xA))) {
                var_a1 = 9;
            }
            goto block_53;
        }
        func_80085B3C(0x12, (s32) sp2F);
        func_80065F34(0);
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}

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


