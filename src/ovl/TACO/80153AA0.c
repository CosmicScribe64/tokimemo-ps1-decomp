#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80153AA0", func_80153AA0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80153AA0", func_80153B38);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80153AA0", func_80153E78);

s32 func_80154014(void) {
    s32 t;
    s32 v;

    D_8015F4F0 = 0x80;
    t = D_80160464;
    if (t < 0x32) {
        func_8013A790(0, 2);
    } else if (t >= 0x32 && t < 0xDC) {
        func_8013A790(0, 3);
    }
    if (t >= 0x32 && t < 0x5A) {
        func_80153AA0(t * 3 - 0x96, 2, 0xA0A0A0);
    }
    if (t >= 0x5A && t < 0x96) {
        func_80153AA0(0x78, 2, 0xA0A0A0);
    }
    if (t >= 0x96 && t < 0xB4) {
        func_80153AA0(0x10E - t, 2, 0xA0A0A0);
    }
    if (t >= 0xB4 && t < 0xD2) {
        func_80153AA0(0x276 - t * 3, 2, 0xA0A0A0);
    }
    if (t < 0x32) {
        v = t * 2;
        func_80153E78(2, v, v, v);
    } else if (t >= 0x32 && t < 0xD2) {
        if ((t - 0x32) % 3 == 0) {
            func_80153E78(1, 0xA0, 0xA0, 0xC0);
        } else {
            func_80153E78(2, 0x64, 0x64, 0x64);
        }
    } else if (t >= 0xD2 && t < 0x104) {
        v = 0x208 - t * 2;
        func_80153E78(2, v, v, v);
    }
    if (t == 0x104) {
        D_80160464 = 0;
        D_80160460 = 0;
    } else {
        D_80160464 += 1;
    }
    return D_80160460;
}
