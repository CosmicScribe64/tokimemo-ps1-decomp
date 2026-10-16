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

INCLUDE_ASM("asm/ovl/RENSYU/nonmatchings/RENSYU/80132040", func_8013325C);

INCLUDE_ASM("asm/ovl/RENSYU/nonmatchings/RENSYU/80132040", func_80133628);

/* bulk_m2c declarations (T-11190) */
extern s32 D_801343A8;
extern u16 D_800E6374;
extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E652A;
extern u8 D_800E659A;
extern u8 D_800E738A;
extern u8 D_801342C4;
/* end bulk_m2c declarations */
void func_80133990(void) {
    s16 sp30[13];
    s16 temp_v1;
    s16 temp_v1_2;
    s16 var_v1;
    s16 var_v1_2;
    s16 var_v1_3;
    u32 temp_v0;

    *(s32 *)sp30 = (s32) D_801343A8;
    *(s32 *)((unsigned char *)(&sp30[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_801343A8) + (8));
    *(s32 *)((unsigned char *)(&sp30[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801343A8) + (4));
    *(s32 *)((unsigned char *)(&sp30[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_801343A8) + (12));
    *(s32 *)((unsigned char *)(&sp30[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_801343A8) + (16));
    *(u16 *)((unsigned char *)(&sp30[0]) + (24)) = (u16) *(u16 *)((unsigned char *)(&D_801343A8) + (24));
    *(s32 *)((unsigned char *)(&sp30[0]) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_801343A8) + (20));
    func_80083808();
    switch (D_800E738A) {
    case 0:
        func_8004500C(1, 0);
        func_80083474();
        dec_bg_show_switch(1);
        break;
    case 1:
        normal_date_bggirl_fadein();
        break;
    case 2:
        if ((u8) D_801342C4 >= 3U) {
            temp_v1 = (&sp30[0])[D_801342D0] + 2;
            var_v1 = temp_v1;
            if (D_801342D0 == 4) {
                temp_v0 = (u8) D_800E652A >> 4;
                if (((u16) D_800E6374 >> 0xC) != temp_v0) {
                    if (temp_v0 == 7) {
                        var_v1 = temp_v1 + 1;
                    }
                    if (temp_v0 == 6) {
                        var_v1 += 2;
                    }
                }
            }
            D_800CA148 = var_v1;
            if (D_801342D0 == 6) {
                D_800CA148 = var_v1;
                if (((u16) D_800E6374 >> 0xC) != ((u8) D_800E659A >> 4)) {
                    D_800CA148 = var_v1 + 1;
                }
            }
        } else {
            temp_v1_2 = (&sp30[0])[D_801342D0] + 3;
            var_v1_2 = temp_v1_2;
            if (D_801342D0 == 4) {
                var_v1_2 = temp_v1_2 + 2;
            }
            D_800CA148 = var_v1_2;
            if (D_801342D0 == 6) {
                D_800CA148 = var_v1_2 + 1;
            }
        }
        func_8004284C();
        break;
    case 3:
    case 6:
        normal_date_speak();
        break;
    case 4:
        if ((u8) D_801342C4 >= 3U) {
            var_v1_3 = 0x25;
            if (D_801342D0 == 0xC) {
                var_v1_3 = 0x26;
            }
        } else if ((D_800E62BE == 0x61) && (var_v1_3 = 0x28, (D_800E62BF == 0xC))) {

        } else {
            var_v1_3 = 0x27;
        }
        D_800CA148 = var_v1_3;
        func_8004284C();
        break;
    case 5:
        if (D_801342D0 == 0xC) {
            normal_date_girl_out(D_801342D0);
        } else {
            func_8004284C();
        }
        break;
    case 7:
        if (D_801342D0 == 0xC) {
            normal_date_bg_fadeout(D_801342D0);
        } else {
            normal_date_bggirl_fadeout(D_801342D0);
        }
        break;
    case 8:
        func_80044750(0xC1);
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}

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


