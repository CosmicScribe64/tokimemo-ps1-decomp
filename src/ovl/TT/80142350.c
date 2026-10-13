#include "common.h"
#include "ovl/TT.h"

/* NON_MATCHING: T-11000, 11 lines (registers only): constant 1 is in a0 and the TtButtons06 pointer in v0, the original has a1 and a2; v = base[0x52] after the store gives the andi */
#ifdef NON_MATCHING
void func_80142350(void) {
    u8 *base;
    u8 *s;
    TtButtons06 *buttons;
    u8 v;

    base = D_80158A60;
    s = D_80158A64;
    buttons = (TtButtons06 *)(base + 0x10);
    *(u16 *)s = 1;
    s[9] = 0;
    s[10] = 0;
    s[11] = 0;
    *(s16 *)(s + 2) = 0;
    *(s16 *)(s + 4) = 0;
    *(s16 *)(s + 6) = 0;
    s[14] = 1;
    s[17] = 0;
    if (base[0x53] == 0) {
        v = base[0x52];
        if (v == 0xFF) {
            base[0x52] = 0;
            v = base[0x52];
        }
        if (v != 0) {
            switch (v) {
            case 1:
                func_8014304C(0x80016000, 0xC812, 0x2420);
                func_8014304C(0x8002E800, 0xC7C1, 0x69E4);
                func_8014304C(0x8019A000, 0xC7C5, 0x27D60);
                break;
            case 2:
                func_8014304C(0x80016000, 0xC869, 0x2420);
                func_8014304C(0x8002E800, 0xC817, 0x69E4);
                func_8014304C(0x8019A000, 0xC81A, 0x27D60);
                break;
            case 3:
                func_8014304C(0x80016000, 0xC8BD, 0x2420);
                func_8014304C(0x8002E800, 0xC86D, 0x69E4);
                func_8014304C(0x8019A000, 0xC871, 0x27D60);
                break;
            case 4:
                func_8014304C(0x80016000, 0xC916, 0x2420);
                func_8014304C(0x8002E800, 0xC8C1, 0x69E4);
                func_8014304C(0x8019A000, 0xC8C6, 0x27D60);
                break;
            }
            func_8014304C(0x8001A000, 0xBFD1, 0xE20);
            func_8014304C(0x801C2000, 0xBF9C, 0x1A680);
            *(u16 *)s = 2;
        } else {
            *(u16 *)s = 1;
        }
    } else if (buttons->unk_04 & 0xF0) {
        *(u16 *)s = 1;
    }
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142350", func_80142350);
#endif
