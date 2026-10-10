#include "common.h"
#include "ovl/TAIIKU.h"
extern u8 D_8011ECD0[]; /* 3 records of 0x44 bytes; s16 at +0x19EA is read */

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80132BD0", func_80132BD0);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80132BD0", func_80132E20);

void func_801330D4(void) {
    func_801330FC();
    func_801331D0();
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80132BD0", func_801330FC);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80132BD0", func_801331D0);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80132BD0", func_80133258);

void func_80133438(void) {
    func_80132E20(0, 1);
    Scroll(0, 0xF, 0, 0x70, 0x280);
    Scroll(0, 0xF, 0x70, 0x80, 0x80);
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80132BD0", func_80133494);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80132BD0", func_80133644);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80132BD0", func_8013384C);

void func_80133928(s32 arg0) {
    u8 *p;

    func_801339C4();
    p = D_8011ECD0 + arg0 * 0x44;
    if (*(s16 *)(p + 0x26) >= 0x90) {
        *(s16 *)(p + 0x26) = 0x90;
    }
    if (*(s16 *)(p + 0x26) < -0x9F) {
        *(s16 *)(p + 0x26) = -0xA0;
    }
    if (*(s16 *)(p + 0x2A) >= 0x68) {
        *(s16 *)(p + 0x2A) = 0x68;
    }
    if (*(s16 *)(p + 0x2A) < -0x67) {
        *(s16 *)(p + 0x2A) = -0x68;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80132BD0", func_801339C4);
