#include "common.h"
#include "game.h"

s32 func_80046500(void) {
    func_800789E0();
    func_800412E0(0);
    func_80042878(0x12);
    func_80041584();
    func_80048DD0(0);
    back_clear_switch(1);
    dec_bg_reset();
    func_8004482C();
    tpage_buf_clear();
    func_80044750(0x7F);
    func_8007BDE8();
    func_8007B844();
    func_800452C4();
    func_80041878();
    func_80042058();
    return -1;
}

s32 func_80046590(s16 arg0, u8 arg1) {
    s32 p;
    u32 i;

    p = 1;
    if ((u32)arg1 >= 6) {
        return 0;
    }
    i = 0;
    if (arg1 != 0) {
        do {
            i += 1;
            p *= 10;
        } while (i < (u32)arg1);
    }
    return (arg0 / (p / 10)) % 10;
}

s32 func_80046684(s32 arg0) {
    s32 result;
    s32 mul;
    s32 i;

    result = 0;
    mul = 1;
    for (i = 0; i < 8; i++) {
        result += (mul * (arg0 & (0xF << (i * 4)))) >> (i * 4);
        mul *= 10;
    }
    return result;
}

INCLUDE_ASM("asm/nonmatchings/main/80046500", func_80046754);

INCLUDE_ASM("asm/nonmatchings/main/80046500", func_80046AC8);
