#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*f[32])();
} FnTbl32; /* size 0x80 */
extern FnTbl32 D_801476E4;

void func_80142760(void) {
    D_80147630 = 0x80197558;
    D_80147634 = 0x80197F60;
    D_80147638 = 0x80198830;
    D_8014763C = 0x801991A8;
    D_80147640 = 0x80199C00;
    D_80147644 = 0x8019A598;
    D_80147648 = 0x8019B0D4;
    D_8014764C = 0x8019BAEC;
    D_80147650 = 0x8019C4FC;
    D_80147654 = 0x8019CF64;
    D_80147658 = 0x8019D970;
    D_8014765C = 0x8019DF94;
    D_80147660 = 0x8019E5E4;
    D_80147664 = 0x80197624;
    D_80147668 = 0x80198024;
    D_8014766C = 0x801988FC;
    D_80147670 = 0x8019926C;
    D_80147674 = 0x80199CCC;
    D_80147678 = 0x8019A664;
    D_8014767C = 0x8019B1A0;
    D_80147680 = 0x8019BBB8;
    D_80147684 = 0x8019C5C8;
    D_80147688 = 0x8019D030;
    D_8014768C = 0x8019DA3C;
    D_80147690 = 0x8019DFBC;
    D_80147694 = 0x8019E6B0;
    D_80147698 = 0x801979D0;
    D_8014769C = 0x801983B0;
    D_801476A0 = 0x80198CA8;
    D_801476A4 = 0x801995F8;
    D_801476A8 = 0x8019A078;
    D_801476AC = 0x8019AA2C;
    D_801476B0 = 0x8019B54C;
    D_801476B4 = 0x8019BF64;
    D_801476B8 = 0x8019C974;
    D_801476BC = 0x8019D3DC;
    D_801476C0 = 0x8019DE04;
    D_801476C4 = 0x8019E05C;
    D_801476C8 = 0x8019EA5C;
}

void func_801429D4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl32 tbl;

    tbl = D_801476E4;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

s32 func_80142A5C(void) {
    if (D_800E6280.unk_110D == 0) {
        func_800674B0();
        D_800E6280.unk_110D += 1;
    }
    if (D_800E6280.unk_110D == 1) {
        if (D_800E6280.unk_1104.u++ >= 0x400U) {
            func_800452C4();
            func_8004482C();
            D_800E6280.unk_110D = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            return 0;
        }
    }
    return func_80072B5C(1);
}

void func_80142B20(void) {
    u8 *p = &D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01;

    func_80044890(1, 0xBF98, 0xBF79, D_800B3688[*p], D_800B36C8[*p], D_800B3708[*p]);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80142BB8(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_80142C38(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_80142C60(void) {
    cd_read_sectors(0x10, 0x80197000, 0xAF23);
    func_80142760();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80142760", func_80142C9C);

void func_80143AE4(void) {
    bg_read_sub2(0x403B);
    func_8004284C();
}

void func_80143B0C(void) {
    D_800CA134 = &D_801476D8;
    D_800CA138 = &D_801476DC;
    D_800CA13C = D_801476CC;
    D_800CA140 = D_801476D0;
    D_800CA144 = D_801476D4;
    func_80082764(D_80122CDC, 1, 0);
}

void func_80143B88(void) {
    D_800CA134 = &D_801476D8;
    D_800CA138 = &D_801476DC;
    D_800CA13C = D_801476CC;
    D_800CA140 = D_801476D0;
    D_800CA144 = D_801476D4;
    func_80082764(0xFF, 1, 0);
}

void func_80143C00(void) {
    if (D_80122CDC != 0) {
        D_801476D8 = 0x17;
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0A += 0xF;
        func_80042940(0x18);
    } else {
        D_801476D8 += D_801476E0;
        func_8004284C();
    }
    check_para_limit();
}

void func_80143C94(void) {
    D_801476D8 = 0x14;
    func_8004284C();
}

void func_80143CBC(void) {
    s16 i;
    u8 a;
    u8 b;

    if (D_80122CDC != 0) {
        if (D_800E6280.unk_F5F == 2) {
            D_801476D8 = 0x18;
        } else {
            D_801476D8 = 0x17;
        }
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0A += 0x14;
        func_80042940(0x18);
    } else {
        D_801476D8 = 0x15;
        for (i = 0; i < 0xB; i++) {
            if (D_800E6280.unk_1BC[i].unk_0C.f.b1 && D_800E6280.unk_F5F != i) {
                a = D_800E6280.unk_1BC[i].unk_02 / 20;
                b = func_800AE0D0() % (u32)(a + 1);
                if ((u8)(func_80051A68(i) & 0x7F) < 2) {
                    D_800E6280.unk_1BC[i].unk_0A = D_800E6280.unk_1BC[i].unk_0A + b + 6;
                } else {
                    D_800E6280.unk_1BC[i].unk_0A = D_800E6280.unk_1BC[i].unk_0A + b + 4;
                }
            }
        }
        func_8004284C();
    }
    func_80084D3C();
}

void func_80143EA4(void) {
    if (D_80122CDC != 0) {
        if (D_800E6280.unk_F5F == 2 || D_800E6280.unk_F5F == 7 || D_800E6280.unk_F5F == 9 || D_800E6280.unk_F5F == 0xA) {
            func_80083418();
            return;
        }
        func_800833F0();
        return;
    }
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80142760", func_80143F48);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80142760", func_80144198);

/* bulk_m2c declarations (T-11190) */
extern s32 D_80147C6C;
extern s8 D_800E63A2;
extern s8 D_800E63A3;
extern s8 D_800E63A5;
extern s8 D_800E63A7;
extern s8 D_800E63A8;
extern s8 D_800E63AA;
extern s8 D_800E63AB;
extern s8 D_800E63AC;
extern s8 D_800E63AD;
extern s8 D_800E63AE;
extern s8 D_800E63AF;
extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E6616;
extern u8 D_800E6619;
extern u8 D_800E6622;
extern u8 D_800E6623;
extern u8 D_800E67F8;
extern u8 D_800E6804;
extern u8 D_800E680A;
extern u8 D_800E681B;
extern u8 D_800E6825;
extern u8 D_800E6833;
extern u8 D_800E71DF;
/* end bulk_m2c declarations */
void func_80144270(void) {
    s32 sp34;
    s32 sp38;
    s32 sp3A;
    s32 sp33;
    s32 sp2E;
    s32 sp39;
    s32 sp2F;
    s32 sp31;
    s32 sp36;
    s32 sp37;
    s32 sp3B;
    s32 sp2C[6];
    s32 var_at;

    (*(s32 *)&sp2C) = (s32) D_80147C6C;
    *(s32 *)((unsigned char *)(&sp2C) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80147C6C) + (8));
    *(s32 *)((unsigned char *)(&sp2C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80147C6C) + (4));
    *(s32 *)((unsigned char *)(&sp2C) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80147C6C) + (16));
    *(u8 *)((unsigned char *)(&sp2C) + (20)) = (u8) *(u8 *)((unsigned char *)(&D_80147C6C) + (20));
    *(s32 *)((unsigned char *)(&sp2C) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80147C6C) + (12));
    if ((D_800E71DF == 1) && (D_800E62BE == 0x60) && ((D_800E62BF == 0xC) || (D_800E62BF == 1) || ((D_800E62BF == 2) && ((u8) D_800E62C0 < 0x15U))) && (D_800E67F8 == 0)) {
        D_801476E0 = sp3A;
        D_800E63AE = 1;
    }
    if ((D_800E71DF == 3) && (D_800E62BE == 0x61) && ((D_800E62BF == 9) || (D_800E62BF == 0xA) || ((D_800E62BF == 0xB) && ((u8) D_800E62C0 < 0x15U))) && (D_800E6804 == 0)) {
        D_801476E0 = sp33;
        D_800E63A7 = 1;
    }
    if ((D_800E71DF == 4) && ((var_at = (u8) D_800E62BE < 0x60U, (D_800E62BF == 9)) || ((D_800E62BF == 0xA) && (var_at = (u8) D_800E62BE < 0x60U, (((u8) D_800E62C0 < 0x15U) != 0)))) && (var_at == 0) && (D_800E680A == 0)) {
        D_801476E0 = sp39;
        D_800E63AD = 1;
    }
    if ((D_800E71DF == 7) && ((D_800E62BF == 3) || (D_800E62BF == 4) || ((D_800E62BF == 5) && ((u8) D_800E62C0 < 0x15U))) && (D_800E681B == 0)) {
        D_801476E0 = sp2E;
        D_800E63A2 = 1;
    }
    if (D_800E71DF == 0xA) {
        if ((u8) D_800E62BE >= 0x60U) {
            if ((D_800E62BF == 0xA) && ((u8) D_800E62C0 < 0x15U) && (D_800E6833 == 0)) {
                D_801476E0 = sp39;
                D_800E63AD = 1;
            }
        }
    }
    if (D_800E71DF == 8) {
        if ((((D_800E62BE == 0x5F) && (D_800E62BF == 0xC)) || ((D_800E62BE == 0x60) && (D_800E62BF == 1)) || ((D_800E62BE == 0x60) && (D_800E62BF == 2) && ((u8) D_800E62C0 < 0x15U))) && (D_800E6616 == 0)) {
            D_801476E0 = sp2F;
            D_800E63A3 = 1;
        }
        if ((D_800E62BE == 0x60) && (D_800E6619 == 0)) {
            if (D_800E62BF != 6) {
                if (D_800E62BF == 7) {
                    goto block_52;
                }
            } else {
block_52:
                D_801476E0 = sp31;
                D_800E63A5 = 1;
            }
        }
        if ((D_800E62BE == 0x60) && (D_800E6622 == 0)) {
            if (D_800E62BF != 9) {
                if ((D_800E62BF == 0xA) || ((D_800E62BF == 0xB) && ((u8) D_800E62C0 < 0x15U))) {
                    goto block_60;
                }
            } else {
block_60:
                D_801476E0 = sp36;
                D_800E63AA = 1;
            }
        }
        if ((D_800E62BE == 0x61) && (D_800E6623 == 0)) {
            if (D_800E62BF != 3) {
                if ((D_800E62BF == 4) || ((D_800E62BF == 5) && ((u8) D_800E62C0 < 0x15U))) {
                    goto block_68;
                }
            } else {
block_68:
                D_801476E0 = sp37;
                D_800E63AB = 1;
            }
        }
        if ((D_800E62BE == 0x61) && (D_800E6825 == 0)) {
            if (D_800E62BF != 9) {
                if ((D_800E62BF == 0xA) || ((D_800E62BF == 0xB) && ((u8) D_800E62C0 < 0x15U))) {
                    goto block_76;
                }
            } else {
block_76:
                D_801476E0 = sp3B;
                D_800E63AF = 1;
            }
        }
        if ((D_800E62BE == 0x61) && ((D_800E62BF == 0xC) || (D_800E62BF == 1) || ((D_800E62BF == 2) && ((u8) D_800E62C0 < 0xBU)))) {
            D_801476E0 = sp38;
            D_800E63AC = 1;
        }
        if (((func_80144198(0) != 0x5F) || (func_80144198(1) != 0xC) || (func_80144198(2) != 0xA)) && ((func_80144198(0) != 0x60) || (func_80144198(1) != 0xC) || (func_80144198(2) != 8))) {
            if ((func_80144198(0) == 0x61) && (func_80144198(1) == 0xC) && (func_80144198(2) == 7)) {
                goto block_93;
            }
        } else {
block_93:
            D_801476E0 = sp34;
            D_800E63A8 = 1;
        }
    }
}
