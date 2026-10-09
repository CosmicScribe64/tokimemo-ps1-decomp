#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", func_8004AD60);

void func_8004ADAC(u8 arg0) {
    D_800B3D80 = arg0;
    if ((u32)arg0 >= 9) {
        D_800B3D80 = 8;
    }
}

u8 func_8004ADD4(void) {
    return D_800B3D80;
}

void func_8004ADE4(void) {
    if (D_800B3220 != 0) {
        LoadSquare(0x2C0, 0x1E0, 0x40, 0x20, (void *)0x801F0020);
    }
}

/* FAKE: func_8004AE54 takes four arguments and the original leaves a0-a2 untouched; passing the
 * (unused) incoming a0-a2 through reproduces that, real source unknown. T-0800 */
void func_8004AE28(s32 a0, s32 a1, s32 a2) {
    if (D_800B3220 != 0) {
        func_8004AE54(a0, a1, a2, 0xF);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", func_8004AE54);

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", func_8004B19C);

void func_8004B338(s32 arg0, s32 arg1, s32 arg2) {
    func_8004B358(arg0, arg1, arg2, 0xF);
}

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", func_8004B358);

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", func_8004B590);

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", make_color_bar);

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", make_color_bar16);

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", col2sepia);
