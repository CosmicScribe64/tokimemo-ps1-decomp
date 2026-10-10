#include "common.h"
#include "ovl/TT.h"

void func_8013A040(void) {
    u8 *p = D_80158A60;
    u8 *q = D_80158A64;
    u8 *r = D_80158AA8 + 0x9D8;
    u16 *pad = (u16 *)(p + 0x10);

    if ((*(u16 *)(p + 8) >> 4) & 1) {
        *(u16 *)(r + 6) = 1;
    } else {
        *(u16 *)(r + 6) = 0;
    }
    if (q[0x22] == 0) {
        if ((pad[2] & 0x800) || (pad[2] & 0x20)) {
            q[9] = 1;
        }
    }
    if (*(u16 *)(q + 0x26) >= 0x1A4) {
        q[9] = 1;
        return;
    }
    *(u16 *)(q + 0x26) += 1;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013A040", func_8013A0DC);
