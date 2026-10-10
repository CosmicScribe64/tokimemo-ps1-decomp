#include "common.h"
#include "ovl/TT.h"

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80132000", func_80132000);

void func_801320F0(u8 *arg0, u8 *arg1) {
    s32 dx = *(s32 *)(arg1 + 0xC) - *(s32 *)(arg0 + 0xC);
    s32 dy = *(s32 *)(arg1 + 0x10) - *(s32 *)(arg0 + 0x10);

    func_8014D260(dy >> 16, dx >> 16);
}
