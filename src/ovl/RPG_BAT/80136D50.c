#include "common.h"
#include "ovl/RPG_BAT.h"

s32 func_80136D50(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E904, "虹野「がんばってっ！");
        func_8014B738(D_8015E9CC, 1, 0x82);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x06000032, 0x08000034, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F220();
        func_8014F230();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80136D50", func_80136E2C);

void func_80137148(s32 arg0, s32 arg1) {
    if (arg1 == 7) {
        if (!(D_8015EC74 & 1)) {
            func_8013E97C(arg0, func_8013E9A8(0x10) + 0x108, 0x40);
        } else {
            func_8013E97C(arg0, func_8013E9A8(0x10) + 0x108, 0x58);
        }
        D_8015EC74 += 1;
    } else {
        func_8013E97C(arg0, func_8013E9A8(0x10) + 0x108, 0x40);
    }
    func_8013E810(arg0, arg1, 5, 1);
}

void func_80137204(void) {
    s32 var_s1;
    u8 *var_s0;

    var_s0 = D_8011ECD0;
    var_s1 = 0;
    do {
        *(s16 *)(var_s0 + 0x1ABA) = (s16) (*(s16 *)(var_s0 + 0x1ABA) + 1);
        if (!(*(u8 *)(var_s0 + 0x1A92) & 1)) {
            func_8013E7C0(var_s1 + 4, 0xFF, 1, 0);
        }
        var_s1 += 1;
        var_s0 += 0x44;
    } while (var_s1 != 0xC);
}
