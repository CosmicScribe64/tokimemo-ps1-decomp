#include "common.h"
#include "ovl/TT.h"

#ifdef NON_MATCHING
/* NON_MATCHING: T-11860, 4 lines: in the else arm the original loads a[0] into a ugen temporary (t3), the draft into c (a2) */
void func_80142110(void) {
    u8 *s;
    u8 *g;
    u16 *a;
    u16 *b;
    s32 x;
    u16 c;
    u16 t;

    g = D_80158A60;
    s = D_80158A64;
    a = (u16 *)(g + 0x10);
    b = (u16 *)(g + 0x20);
    func_80042A00();
    x = func_80043010();
    a[1] = a[0];
    if (g[0x54] != 0) {
        a[0] = ((u16 *)&x)[1];
    } else {
        a[0] = ((u16 *)&x)[0];
    }
    a[2] = a[0] & ~a[1];
    b[1] = b[0];
    *(u16 *)(g + 0x50) &= 0xFFF;
    if (s[0x22] != 0) {
        c = *(*(u16 *)(g + 0x50) + *(u8 **)(g + 0x4C));
        b[0] = (u16)((u16)(c & 0xF0) << 8) | (c & 3); /* FAKE: the two u16 conversions give the original's skipped ugen temporaries. T-11860 */
        *(u16 *)(g + 0x50) += 1;
        *(u16 *)(g + 0x50) &= 0xFFF;
    } else {
        c = a[0] & 0xFF00;
        b[0] = c;
        t = a[0] << 8;
        b[0] |= (t >> (g[0x44] + 8)) & 1;
        b[0] |= (t >> (g[0x45] + 7)) & 2;
        if (g[0x43] != 0) {
            if (b[0] & 1) {
                if (g[0x46] >= 6) {
                    g[0x46] = 0;
                    b[0] ^= 1;
                } else {
                    g[0x46]++;
                }
            } else {
                g[0x46] = 6;
            }
            if (b[0] & 2) {
                if (g[0x47] >= 6) {
                    g[0x47] = 0;
                    b[0] ^= 2;
                } else {
                    g[0x47]++;
                }
            } else {
                g[0x47] = 6;
            }
        }
    }
    b[2] = b[0] & ~b[1];
    a += 4;
    a[1] = a[0];
    a[0] = ((u16 *)&x)[1];
    a[2] = a[0] & ~a[1];
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142110", func_80142110);
#endif
