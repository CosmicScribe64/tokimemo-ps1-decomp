#include "common.h"
#include "ovl/ENDING.h"

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133030", func_80133030);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133030", func_801330E4);

void func_80133400(void) {
    bg_read_sub2(0x4045);
    func_8004284C();
}

void func_80133428(void) {
    func_80046318(0x11, 0x801F0000, 0xAE07);
    func_80132000();
    func_8004284C();
}

void func_80133460(void) {
    if (((u8) D_800E71DF >= 0xDU) && ((u32) ((u32) (D_800E6758 << 0x14) >> 0x1D) < 3U)) {
        func_80042908(7);
        D_800E7D34 |= 8;
        return;
    }
    func_8004284C();
    D_800E7D34 |= 4;
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133030", func_801334E4);
