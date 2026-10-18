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

void func_80144270(void) {
    s32 pad; /* FAKE: unused local that moves the table to sp+0x2C and the frame to 0x48 (T-3330 rule, T-23005) */
    GekoBytes21 tbl;

    tbl = D_80147C6C;
    if ((*(u8 *)&D_800E6280.unk_F5F == 1) && (*(u8 *)&D_800E6280.unk_03E == 0x60) && ((*(u8 *)&D_800E6280.unk_03F == 0xC) || (*(u8 *)&D_800E6280.unk_03F == 1) || ((*(u8 *)&D_800E6280.unk_03F == 2) && (*(u8 *)&D_800E6280.unk_040 < 0x15U))) && (D_800E6280.unk_56C[0xC] == 0)) {
        D_801476E0 = tbl.v[0xE];
        D_800E6280.unk_120[0xE] = 1;
    }
    if ((*(u8 *)&D_800E6280.unk_F5F == 3) && (*(u8 *)&D_800E6280.unk_03E == 0x61) && ((*(u8 *)&D_800E6280.unk_03F == 9) || (*(u8 *)&D_800E6280.unk_03F == 0xA) || ((*(u8 *)&D_800E6280.unk_03F == 0xB) && (*(u8 *)&D_800E6280.unk_040 < 0x15U))) && (D_800E6280.unk_56C[0x18] == 0)) {
        D_801476E0 = tbl.v[7];
        D_800E6280.unk_120[7] = 1;
    }
    if ((*(u8 *)&D_800E6280.unk_F5F == 4) && ((*(u8 *)&D_800E6280.unk_03F == 9) || ((*(u8 *)&D_800E6280.unk_03F == 0xA) && (*(u8 *)&D_800E6280.unk_040 < 0x15U))) && (*(u8 *)&D_800E6280.unk_03E >= 0x60U) && (D_800E6280.unk_56C[0x1E] == 0)) {
        D_801476E0 = tbl.v[0xD];
        D_800E6280.unk_120[0xD] = 1;
    }
    if ((*(u8 *)&D_800E6280.unk_F5F == 7) && ((*(u8 *)&D_800E6280.unk_03F == 3) || (*(u8 *)&D_800E6280.unk_03F == 4) || ((*(u8 *)&D_800E6280.unk_03F == 5) && (*(u8 *)&D_800E6280.unk_040 < 0x15U))) && (D_800E6280.unk_56C[0x2F] == 0)) {
        D_801476E0 = tbl.v[2];
        D_800E6280.unk_120[2] = 1;
    }
    if (*(u8 *)&D_800E6280.unk_F5F == 0xA) {
        if (*(u8 *)&D_800E6280.unk_03E >= 0x60U) {
            if ((*(u8 *)&D_800E6280.unk_03F == 0xA) && (*(u8 *)&D_800E6280.unk_040 < 0x15U) && (D_800E6280.unk_56C[0x47] == 0)) {
                D_801476E0 = tbl.v[0xD];
                D_800E6280.unk_120[0xD] = 1;
            }
        }
    }
    if (*(u8 *)&D_800E6280.unk_F5F == 8) {
        if ((((*(u8 *)&D_800E6280.unk_03E == 0x5F) && (*(u8 *)&D_800E6280.unk_03F == 0xC)) || ((*(u8 *)&D_800E6280.unk_03E == 0x60) && (*(u8 *)&D_800E6280.unk_03F == 1)) || ((*(u8 *)&D_800E6280.unk_03E == 0x60) && (*(u8 *)&D_800E6280.unk_03F == 2) && (*(u8 *)&D_800E6280.unk_040 < 0x15U))) && (D_800E6280.unk_1BC[8].unk_14[6] == 0)) {
            D_801476E0 = tbl.v[3];
            D_800E6280.unk_120[3] = 1;
        }
        if ((*(u8 *)&D_800E6280.unk_03E == 0x60) && (D_800E6280.unk_1BC[8].unk_14[9] == 0) && ((*(u8 *)&D_800E6280.unk_03F == 6) || (*(u8 *)&D_800E6280.unk_03F == 7))) {
            D_801476E0 = tbl.v[5];
            D_800E6280.unk_120[5] = 1;
        }
        if ((*(u8 *)&D_800E6280.unk_03E == 0x60) && (D_800E6280.unk_1BC[8].unk_14[0x12] == 0) && ((*(u8 *)&D_800E6280.unk_03F == 9) || (*(u8 *)&D_800E6280.unk_03F == 0xA) || ((*(u8 *)&D_800E6280.unk_03F == 0xB) && (*(u8 *)&D_800E6280.unk_040 < 0x15U)))) {
            D_801476E0 = tbl.v[0xA];
            D_800E6280.unk_120[0xA] = 1;
        }
        if ((*(u8 *)&D_800E6280.unk_03E == 0x61) && (D_800E6280.unk_1BC[8].unk_14[0x13] == 0) && ((*(u8 *)&D_800E6280.unk_03F == 3) || (*(u8 *)&D_800E6280.unk_03F == 4) || ((*(u8 *)&D_800E6280.unk_03F == 5) && (*(u8 *)&D_800E6280.unk_040 < 0x15U)))) {
            D_801476E0 = tbl.v[0xB];
            D_800E6280.unk_120[0xB] = 1;
        }
        if ((*(u8 *)&D_800E6280.unk_03E == 0x61) && (D_800E6280.unk_56C[0x39] == 0) && ((*(u8 *)&D_800E6280.unk_03F == 9) || (*(u8 *)&D_800E6280.unk_03F == 0xA) || ((*(u8 *)&D_800E6280.unk_03F == 0xB) && (*(u8 *)&D_800E6280.unk_040 < 0x15U)))) {
            D_801476E0 = tbl.v[0xF];
            D_800E6280.unk_120[0xF] = 1;
        }
        if ((*(u8 *)&D_800E6280.unk_03E == 0x61) && ((*(u8 *)&D_800E6280.unk_03F == 0xC) || (*(u8 *)&D_800E6280.unk_03F == 1) || ((*(u8 *)&D_800E6280.unk_03F == 2) && (*(u8 *)&D_800E6280.unk_040 < 0xBU)))) {
            D_801476E0 = tbl.v[0xC];
            D_800E6280.unk_120[0xC] = 1;
        }
        if (((func_80144198(0) == 0x5F) && (func_80144198(1) == 0xC) && (func_80144198(2) == 0xA)) || ((func_80144198(0) == 0x60) && (func_80144198(1) == 0xC) && (func_80144198(2) == 8)) || ((func_80144198(0) == 0x61) && (func_80144198(1) == 0xC) && (func_80144198(2) == 7))) {
            D_801476E0 = tbl.v[8];
            D_800E6280.unk_120[8] = 1;
        }
    }
}
