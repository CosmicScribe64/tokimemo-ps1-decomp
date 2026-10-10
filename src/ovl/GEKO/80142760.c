#include "common.h"
#include "ovl/GEKO.h"

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

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80142760", func_801429D4);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80142760", func_80142A5C);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80142760", func_80142B20);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80142760", func_80142BB8);

void func_80142C38(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_80142C60(void) {
    func_80046318(0x10, 0x80197000, 0xAF23);
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
        *(s16 *)(D_800E6280 + D_800E71DF * 0x38 + 0x1C6) += 0xF;
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

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80142760", func_80143CBC);

void func_80143EA4(void) {
    if (D_80122CDC != 0) {
        if (D_800E71DF == 2 || D_800E71DF == 7 || D_800E71DF == 9 || D_800E71DF == 0xA) {
            func_80083418();
            return;
        }
        func_800833F0();
        return;
    }
    *(s16 *)(D_800E6280 + D_800E71DF * 0x38 + 0x1C2) += 1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80142760", func_80143F48);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80142760", func_80144198);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80142760", func_80144270);
