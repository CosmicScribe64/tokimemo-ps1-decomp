#include "common.h"
#include "ovl/TT.h"

/* Rope update: turn the head angle (+0x99C, s16[32]) towards the target with wrap at 0x400/0xC00, clamp the head
 * length (+0x11C, s32[32]), shift the per-segment history rows (+0x9DC s16[32][16], +0x19C s32[32][16]) and
 * rebuild the segment positions (+0x1C x, +0x9C y). */
void func_801410A0(u8 *s) {
    s32 dx;
    s32 dy;
    s32 n;
    s16 cur;
    s16 tgt;
    s16 step;
    s32 i;
    u32 j;

    n = s[0x1A];
    dx = *(s16 *)(s + 0) - *(s16 *)(s + 4);
    dy = *(s16 *)(s + 2) - *(s16 *)(s + 6);
    *(s16 *)(s + 0x12) = func_8014D260(dy, dx);
    *(s16 *)(s + 0x12) &= 0xFFF;
    ((s16 *)(s + 0x99C))[n] &= 0xFFF;
    cur = ((s16 *)(s + 0x99C))[n];
    tgt = *(s16 *)(s + 0x12);
    if (((cur - tgt < 0) ? -(cur - tgt) : cur - tgt) <= *(s16 *)(s + 0x18)) {
        ((s16 *)(s + 0x99C))[n] = tgt;
    } else if (tgt - cur < 0) {
        step = -*(s16 *)(s + 0x18);
        if ((tgt > 0xC00 && cur < 0x400) || (tgt < 0x400 && cur > 0xC00)) {
            /* FAKE: the (s16) cast keeps negu + addu; cur - step gives subu. T-11860 */
            ((s16 *)(s + 0x99C))[n] = cur + (s16)-step;
            return;
        }
        ((s16 *)(s + 0x99C))[n] = cur + step;
    } else {
        step = *(s16 *)(s + 0x18);
        if ((tgt > 0xC00 && cur < 0x400) || (tgt < 0x400 && cur > 0xC00)) {
            /* FAKE: as above. T-11860 */
            ((s16 *)(s + 0x99C))[n] = cur + (s16)-step;
            return;
        }
        ((s16 *)(s + 0x99C))[n] = cur + step;
    }
    ((s16 *)(s + 0x99C))[n] &= 0xFFF;
    *(s32 *)(s + 0x1C) = *(s16 *)(s + 0) << 16;
    *(s32 *)(s + 0x9C) = *(s16 *)(s + 2) << 16;
    ((s32 *)(s + 0x11C))[n] = (SquareRoot0(dx * dx + dy * dy) << 16) / 4096;
    if (((s32 *)(s + 0x11C))[n] < *(s32 *)(s + 8)) {
        ((s32 *)(s + 0x11C))[n] = *(s32 *)(s + 8);
    }
    if (*(s32 *)(s + 0xC) < ((s32 *)(s + 0x11C))[n]) {
        ((s32 *)(s + 0x11C))[n] = *(s32 *)(s + 0xC);
    }
    for (i = 1; i <= n; i++) {
        ((s16 (*)[16])(s + 0x9DC))[i][s[0x1B]] = ((s16 *)(s + 0x99C))[i];
        ((s32 (*)[16])(s + 0x19C))[i][s[0x1B]] = ((s32 *)(s + 0x11C))[i];
        for (j = 0; j < s[0x1B]; j++) {
            ((s16 (*)[16])(s + 0x9DC))[i][j] = ((s16 (*)[16])(s + 0x9DC))[i][j + 1];
            ((s32 (*)[16])(s + 0x19C))[i][j] = ((s32 (*)[16])(s + 0x19C))[i][j + 1];
        }
        ((s16 *)(s + 0x99C))[i - 1] = ((s16 (*)[16])(s + 0x9DC))[i][0];
        ((s32 *)(s + 0x11C))[i - 1] = ((s32 (*)[16])(s + 0x19C))[i][0];
    }
    for (i = 1; i < n; i++) {
        ((s16 *)(s + 0xDDC))[i] = ((s16 *)(s + 0x99C))[i] - ((s16 *)(s + 0x99C))[i - 1];
        ((s32 *)(s + 0x1C))[i] = ((s32 *)(s + 0x1C))[i - 1] -
                                 func_800A0140(((s16 *)(s + 0x99C))[i]) * ((s32 *)(s + 0x11C))[i] / (n - 1);
        ((s32 *)(s + 0x9C))[i] = ((s32 *)(s + 0x9C))[i - 1] -
                                 func_800A0070(((s16 *)(s + 0x99C))[i]) * ((s32 *)(s + 0x11C))[i] / (n - 1);
    }
}

