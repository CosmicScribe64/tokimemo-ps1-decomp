#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013A650", func_8013A650);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013A650", func_8013A98C);

void func_8013ACA4(void) {
    s32 temp_v0;

    func_8013F0F4(0x507, 0, 2);
    func_8013E7C0(0x2E, 0xA, 1, 1);
    func_8013E97C(0x2E, -D_8015EDB4 * 8, 0);
    temp_v0 = D_8015EDB4 + 1;
    D_8015EDB4 = temp_v0;
    if (temp_v0 >= 0x27) {
        D_80121685 = 5;
        func_8013E7C0(0x2E, 0xA, 1, 0);
        func_8014EBA8();
    }
}
