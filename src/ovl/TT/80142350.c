#include "common.h"
#include "ovl/TT.h"

/* T-11800: matched. T-11550 temp (FAKE) moves the constant 1 to a1; an empty if (buttons) {} in a default case (FAKE) keeps the TtButtons06 pointer live through the case dispatch, so it is coloured after the constant 1 and takes a2 as in the original. */
void func_80142350(void) {
    u8 *base;
    u8 *s;
    TtButtons06 *buttons;
    u8 v;
    u8 temp; /* FAKE: holds the reloaded base[0x52]; its range in a0 moves the constant 1 to a1. T-11550 */

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
            temp = base[0x52];
            v = temp;
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
            default:
                if (buttons) { /* FAKE: an empty test in a default case keeps buttons live through the case dispatch (before the calls), so it is coloured after the constant 1 and takes $a2. T-11800 */
                }
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
