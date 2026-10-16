#include "common.h"
#include "ovl/TT.h"

void func_80142570(void) {
    u8 *p;
    s32 i;

    func_80133AD0(0x60C);
    p = D_80158A98;
    i = 0;
    do {
        i++;
        if (*p != 0 && *(u16 *)(p + 4) == 0x40) {
            *(u16 *)(p + 4) = 0x70;
            /* FAKE: chained stores (u16 view); the reloads keep dead uopt ranges (no code) that move i, 0x70 and 0x40 to a1/a3/a2. T-11860 */
            *(u16 *)(p + 6) = *(u16 *)(p + 8) = *(u16 *)(p + 0xA) = 0;
            p[0xD] = p[0xE] = p[0xF] = 0;
        }
        p += 0x78;
    } while (i < 0x40);
    p = D_80158A9C;
    for (i = 0; i < 0x20; i++) {
        if (*p != 0 && *(u16 *)(p + 4) == 0x40) {
            *(u16 *)(p + 4) = 0x70;
            /* FAKE: chained stores, as in the first loop. T-11860 */
            *(u16 *)(p + 6) = *(u16 *)(p + 8) = *(u16 *)(p + 0xA) = 0;
            p[0xD] = p[0xE] = p[0xF] = 0;
        }
        p += 0x78;
    }
}

void func_801426F0(void) {
    s32 i;
    u8 *p = D_80158A8C;
    u8 *r = D_80158AA4 + 0xC80;

    p[0] = 0;
    r[0xC8] = 0;
    r[0x64] = 0;
    r[0] = 0;
    r += 0x12C;
    for (i = 3; i < 0x1F; i++) {
        *r = 0;
        r += 0x64;
    }
}

void func_80142740(void) {
    u8 *p = D_80158A8C;
    s32 i;

    for (i = 0; i < 0x18; i++) {
        p[0x310 + i] = 0;
    }
}

void func_80142774(void) {
    u8 *p = D_80158A8C;
    s32 i;

    for (i = 0; i < 12; i++) {
        *(s32 *)(p + 0x218 + i * 4) = 0;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80142570", func_801427A8);

void func_80142AF0(void *arg0, s32 arg1) {
    u8 *p = D_80158A8C;
    TtAnim14 *a = (TtAnim14 *)(p + 0x38) + arg1;
    s32 k;
    s16 *s;
    s16 v;

    /* FAKE: the two reads of the index tables go through an s32 view of D_80158A8C, so uopt reloads the pointer (lw t9) instead of reusing p. T-11860 */
    if (p[0] != 0 && ((u8 *)*(s32 *)&D_80158A8C)[arg1 + 0x310] != 0) {
        if (a->unk_08 & 0x80) {
            a->unk_08 &= ~0x80;
            a->unk_06 = ((s16 *)arg0)[0];
            a->unk_09 = ((s16 *)arg0)[1];
            p[arg1 + 0x328] = 0;
            return;
        }
        if (a->unk_08 & 0x20) {
            a->unk_08 &= ~0x20;
            p[arg1 + 0x328] = 0;
            return;
        }
        if (a->unk_09 == 0) {
            k = ((u8 *)*(s32 *)&D_80158A8C)[arg1 + 0x328] + 2; /* FAKE: s32 view, see above. T-11860 */
            s = (s16 *)((u8 *)arg0 + k * 2);
            v = *s;
            if (v == -1) {
                a->unk_08 |= 1;
                a->unk_08 |= 0x40;
                a->unk_06 = s[-2];
            } else if (v == -2) {
                a->unk_06 = ((s16 *)arg0)[0];
                a->unk_09 = ((s16 *)arg0)[1];
                k = 0;
            } else if (v == -3) {
                p[arg1 + 0x310] = 0;
                a->unk_08 |= 1;
            } else {
                a->unk_06 = v;
                a->unk_09 = s[1];
            }
            p[arg1 + 0x328] = k;
        } else {
            a->unk_09--;
        }
    }
}

void func_80142C70(void) {
    s32 i;
    u8 *base;

    base = D_80158A8C;
    if (base[0] != 0) {
        for (i = 0; i != 0x18; i++) {
            if (base[i + 0x310] != 0) {
                *(s16 *)(base + 0x14) = *(s16 *)(base + 0x22) + *(s16 *)(base + 0x248 + i * 2);
                *(s16 *)(base + 0x16) = *(s16 *)(base + 0x26) + *(s16 *)(base + 0x278 + i * 2);
                func_80147A18(base + 0x14, base + 0x38 + i * 0x14, *(u16 *)(base + 0x2A8 + i * 2));
            }
        }
        *(s16 *)(base + 0x14) = *(s16 *)(base + 0x22);
        *(s16 *)(base + 0x16) = *(s16 *)(base + 0x26);
    }
}
