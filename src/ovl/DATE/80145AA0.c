#include "common.h"
#include "ovl/DATE.h"

void func_80145AA0(void) {
    D_8015CE80 = 0x801B00DC;
    D_8015CE84 = 0x801B02AC;
    D_8015CE88 = 0x801B03C8;
    D_8015CE8C = 0x801B04C4;
    D_8015CE90 = 0x801B0668;
    D_8015CE94 = 0x801B0808;
    D_8015CE98 = 0x801B09DC;
    D_8015CE9C = 0x801B0BE8;
    D_8015CEA0 = 0x801B0D8C;
    D_8015CEA4 = 0x801B0F5C;
    D_8015CEA8 = 0x801B1164;
    D_8015CEAC = 0x801B12D8;
    D_8015CEB0 = 0x801B1454;
    D_8015CEB4 = 0x801B00F4;
    D_8015CEB8 = 0x801B02C8;
    D_8015CEBC = 0x801B03D0;
    D_8015CEC0 = 0x801B04DC;
    D_8015CEC4 = 0x801B0680;
    D_8015CEC8 = 0x801B0820;
    D_8015CECC = 0x801B09F8;
    D_8015CED0 = 0x801B0C00;
    D_8015CED4 = 0x801B0DA4;
    D_8015CED8 = 0x801B0F78;
    D_8015CEDC = 0x801B1180;
    D_8015CEE0 = 0x801B12E0;
    D_8015CEE4 = 0x801B1470;
    D_8015CEE8 = 0x801B01A8;
    D_8015CEEC = 0x801B0398;
    D_8015CEF0 = 0x801B03F0;
    D_8015CEF4 = 0x801B0590;
    D_8015CEF8 = 0x801B0734;
    D_8015CEFC = 0x801B08D4;
    D_8015CF00 = 0x801B0AE4;
    D_8015CF04 = 0x801B0CB4;
    D_8015CF08 = 0x801B0E58;
    D_8015CF0C = 0x801B1064;
    D_8015CF10 = 0x801B126C;
    D_8015CF14 = 0x801B1338;
    D_8015CF18 = 0x801B155C;
}

typedef struct {
    void (*f[15])();
} FnTbl15; /* size 0x3C */
extern FnTbl15 D_8015CF3C;

void func_80145D14(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl15 tbl;

    tbl = D_8015CF3C;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    if ((D_80122D44 & 1) && (D_800B593C == 0x80)) {
        func_8006B900();
    }
}

void func_80145DB8(void) {
    func_80046318(3, 0x801B0000, 0xAF43);
    func_80145AA0();
    func_8004284C();
}

void func_80145DF0(void) {
    u8 x;

    D_800E6280.unk_71E |= 8;
    x = func_80051A68(D_800E6280.unk_F5F);
    if ((x & 0x7F) >= 2U) {
        func_80042940(9);
        return;
    }
    D_800E6280.unk_71E |= 4;
    func_80085B3C(0xD, D_80122D08);
    func_8004284C();
}

void func_80145E74(void) {
    D_8015CF34 = 1;
    D_8015CF38 = 0;
    func_800847B8(D_800E6280.unk_F5F);
    func_80145AA0();
    func_80145EC4();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80145AA0", func_80145EC4);

void func_80146134(void) {
    D_800CA134 = &D_8015CF34;
    D_800CA138 = &D_8015CF38;
    D_800CA13C = D_8015CF1C;
    D_800CA140 = D_8015CF20;
    D_800CA144 = D_8015CF24;
    func_80082764(D_80122CDC, 1, 0);
}
