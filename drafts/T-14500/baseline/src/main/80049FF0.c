#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/80049FF0", func_80049FF0);

void func_8004A14C(s32 arg0, RECT rect, s32 arg3) {
    func_80049A40(rect.x, rect.y, rect.w, rect.h, 1, arg3, 1);
}

INCLUDE_ASM("asm/nonmatchings/main/80049FF0", func_8004A1A0);

INCLUDE_ASM("asm/nonmatchings/main/80049FF0", func_8004A414);

INCLUDE_ASM("asm/nonmatchings/main/80049FF0", func_8004A814);

void func_8004AC18(s32 arg0) {
    s32 level;
    s32 color;

    level = arg0;
    if (level < 0x100) {
        color = level * 0x10101;
    } else {
        color = 0xFFFFFF;
    }
    func_80049A40(-0xA0, -0x78, 0xA0, 0xF0, 1, color, 0x81);
    func_80049A40(0, -0x78, 0xA0, 0xF0, 1, color, 0x81);
    dtd_on_tpage(0, 0, 1, 1, 1);
}

void func_8004ACC8(s32 arg0) {
    func_80049A40(-0x100, -0x78, 0x100, 0xF0, 1, arg0, 0x81);
    func_80049A40(0, -0x78, 0x100, 0xF0, 1, arg0, 0x81);
    dtd_on_tpage(0, 0, 1, 1, 1);
}
