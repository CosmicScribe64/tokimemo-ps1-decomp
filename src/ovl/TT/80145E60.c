#include "common.h"
#include "ovl/TT.h"

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80145E60", func_80145E60);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80145E60", func_80146000);

void func_80146224(void) {
    u8 *p = D_80158A6C;
    s16 *q = (s16 *)D_80158AB0;
    s32 i;

    for (i = 0; i != 0x19; i++) {
        s16 v = func_800A0140(*(u16 *)(p + 0x1E));

        q += 8;
        q[-8] = v;
        q[-7] = 0x1000;
    }
    *(u16 *)(p + 0x1E) += 0x10;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80145E60", func_801462AC);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80145E60", func_80146348);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80145E60", func_8014687C);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80145E60", func_80146924);
