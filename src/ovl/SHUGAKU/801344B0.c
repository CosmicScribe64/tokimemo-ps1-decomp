#include "common.h"
#include "ovl/SHUGAKU.h"

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/801344B0", func_801344B0);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/801344B0", func_80134560);

void func_801345E8(void) {
    func_80044750(0x24);
    func_80044750(0x206);
    func_80044750(0x504);
    func_8004284C();
}

void func_80134620(void) {
    func_80046318(0x45, 0x801B0000, 0x8902);
    func_801344B0();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/801344B0", func_80134658);

void func_801347C8(void) {
    bg_read_sub2(0x4285);
    func_8004284C();
}

void func_801347F0(void) {
    bg_read_sub2(0x4774);
    func_80085B3C(2, 0x27);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/801344B0", func_80134824);
