#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8015A1F0", func_8015A1F0);

void func_8015A6D4(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        func_8013E810(i + 0x28, 7, 1, 1);
        func_8013E97C(i + 0x28, -0x19, 0x109);
    }
    func_8014EBA8();
}

#ifdef NON_MATCHING
/* NON_MATCHING: T-10320, two dead register copies ($a1 = v, $a0 = x) and the order of the D_8015EC54 store differ, 17 lines */
void func_8015A744(void) {
    u8 *p;
    s32 i;
    s32 x;
    s32 r;
    s32 v;
    s32 n;
    s32 y;

    p = D_8011ECD0;
    i = 0;
    do {
        x = *(s16 *)(p + 0x2446);
        if (x < -0xB4 || *(s16 *)(p + 0x244A) >= 0x8D) {
            if (func_8013E9A8(5) == 0) {
                func_8013F0F4(0x501, 2, 2);
                func_8013E97C(i + 0x28, 0xF0, 0x28);
                r = func_8013E9A8(8) + 5;
                D_8015EC50 = r;
                v = func_8013E9A8(r);
                p[0x2423] = 0x94;
                n = -D_8015EC50;
                *(s16 *)(p + 0x244E) = n;
                *(s16 *)(p + 0x2452) = v;
                D_8015EC50 = n;
                D_8015EC54 = v;
                x = *(s16 *)(p + 0x2446);
            } else {
                func_8013E810(i + 0x28, 7, 1, 0);
                x = *(s16 *)(p + 0x2446);
            }
        }
        y = x;
        r = ((0x70 - x) << 12) / 240;
        i++;
        p += 0x44;
        *(s16 *)(p + 0x23F6) = r;
        *(s16 *)(p + 0x23F8) = r;
        D_8015EC68 = y;
    } while (i != 8);
}
#else
INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8015A1F0", func_8015A744);
#endif

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8015A1F0", func_8015A8B4);
