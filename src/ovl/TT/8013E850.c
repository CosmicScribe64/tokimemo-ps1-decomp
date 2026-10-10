#include "common.h"
#include "ovl/TT.h"

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013E850", func_8013E850);

void func_8013EC1C(void) {
    u8 *q = D_80158A64;
    u8 *p = D_80158A6C;

    if (q[0x12] != 0) {
        if (*(u16 *)(p + 8) < 0x8CA0U) {
            *(u16 *)(p + 8) += 1;
        }
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013E850", func_8013EC64);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013E850", func_8013ED70);

s32 func_8013EE88(void) {
    u8 *q = D_80158A64;
    u16 *pad = (u16 *)(D_80158A60 + 0x10);

    if (q[0xE] != 0) {
        if ((pad[0] & 0x90C) == 0x90C && (pad[1] & 0x90C) != 0x90C) {
            q[0xF] = 1;
        }
        if (q[0xF] != 0 && (pad[0] & 0x90C) == 0x90C) {
            *(u16 *)(q + 0xC) += 1;
        } else {
            *(u16 *)(q + 0xC) = 0;
        }
    }
    if ((u32)*(u16 *)(q + 0xC) >= 0x1F) {
        return 1;
    }
    return 0;
}

void func_8013EF38(void) {
    u8 *q = D_80158A68;
    u8 *p;

    *(s32 *)(q + 8) += 1;
    *(s32 *)(q + 8) &= 1;
    p = D_80158AB4 + *(s32 *)(q + 8) * 0x80B8;
    *(s32 *)(p + 4) = *(s32 *)p;
    func_8009CAAC(p + 0x8008, 0x10);
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013E850", func_8013EFAC);
