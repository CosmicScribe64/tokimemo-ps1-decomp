#include "common.h"
#include "ovl/TACO.h"

s32 func_80137590(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013760C();
        break;
    case 1:
        func_8013788C();
        break;
    }
    if (D_800E6280.unk_10F8 & 0x10) {
        func_801432F0("DEMO PLAY", -0x40, 0, 2);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80137590", func_8013760C);

void func_8013788C(void) {
    u32 t;

    func_80154960();
    if (D_800E6280.unk_F88 != 0) {
        func_80143AF4();
        func_801347A8();
        func_80059048();
        func_8015ABF0();
        func_800591D8(0);
        func_80042908(2);
    } else {
        D_800E6280.unk_F88 |= 0x20;
        t = D_800E6280.unk_1100 & 0x1FF;
        D_800E6280.unk_F80 |= 0x20;
        if (t >= 0x79 && t < 0x8C) {
            D_800E6280.unk_F80 |= 0x2000;
        }
        if (t >= 0xAB && t < 0xBE) {
            D_800E6280.unk_F80 |= 0x4000;
        }
        if (t >= 0xDD && t < 0xF0) {
            D_800E6280.unk_F80 |= 0x8000;
        }
        if (t >= 0x10F && t < 0x122) {
            D_800E6280.unk_F80 |= 0x1000;
        }
        if (t >= 0x191 && t < 0x1B8) {
            D_800E6280.unk_F80 |= 0x3000;
        }
        if (t >= 0x1CD && t < 0x1E0) {
            D_800E6280.unk_F80 |= 0xC000;
        }
        if (t >= 0x191 && t < 0x1B8) {
            D_800E6280.unk_F80 |= 0x6000;
        }
        if (t >= 0x1CD && t < 0x1E0) {
            D_800E6280.unk_F80 |= 0x9000;
        }
        func_8013A790(1, 1);
        func_8013F250();
        func_80132FE8();
    }
    if ((u32) D_800E6280.unk_1100 >= 0x709U) {
        func_80143AF4();
        func_801347A8();
        func_80059048();
        func_8015ABF0();
        func_800591D8(0);
        func_80042908(2);
    }
}
