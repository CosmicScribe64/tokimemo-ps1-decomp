#include "common.h"
#include "ovl/TT.h"

/* T-11550: matched by writing the restart/loop/reset gotos as two nested for (;;) loops (break to the reset call, goto tail out): same code, but the CFG lets as1 fill the first test's delay slot with the store's lui $at (the goto form left it in the second test's slot). The 0 is still stored through another symbol (FAKE, see the line) so uopt keeps 'lbu; or; sb' for the first |=. */
void func_8013E850(void) {
    u8 *a;
    u8 *b;
    u8 *t;
    u8 *e;

    a = D_80158A60;
    b = D_80158A64;
    t = D_80158A6C;
    func_8013E454();
    func_8013E498();
    func_8013E290();
    func_8013E6A0();
    func_8013E824();
    func_801430D4();
    func_80133970();
    for (;;) {
        b[0xE] = 0;
        b[0x10] = 0;
        func_80044750(0x21);
        func_80044750(0x22);
        func_80044750(0x2F);
        func_80044750(0x21);
        func_80044750(0xF1);
        func_800452C4();
        for (;;) {
            func_8013EF38();
            if (func_8013EE88() != 0) {
                if (a[0x53] != 0) {
                    break;
                }
                *(u16 *)b = 8;
                *(s16 *)(b + 6) = 0;
                *(s16 *)(b + 4) = 0;
                *(s16 *)(b + 2) = 0;
                b[0xB] = 0;
                b[0xA] = 0;
                b[9] = 0;
            }
            if (b[0x11] != 0) {
                func_8013ED70();
            }
            if (b[0x10] == 0) {
                D_801564B0[*(u16 *)b]();
                *(u16 *)(a + 0xA) += 1;
            }
            if (a[0x55] != 0) {
                goto tail;
            }
            func_80148584(*(u16 *)(t + 8), 0x27);
            *(u16 *)(a + 8) += 1;
            func_80148460();
            func_8014D168();
            func_80144854();
            func_80148154();
            func_8014ACEC();
            func_80147150();
            func_801491F0();
            func_8014C84C();
            func_8013C1FC();
            func_80142C70();
            func_80147644();
            func_801336F8();
            func_80135148();
            func_8013E1E8();
            func_8009C674(0);
            func_800AD950(0);
            func_80142110();
            func_8013EFAC();
            func_80079B34();
            func_800455C4();
            func_80133B3C();
        }
        func_8013E320();
    }
tail:
    D_800E7D3C = *(u16 *)(a + 0x30);
    D_800E7D42 = a[0x32];
    D_800E7D43 = a[0x33];
    D_800E7D44 = a[0x34];
    D_800E7D3E = *(u16 *)(a + 0x36);
    D_800E7D45 = a[0x38];
    D_800E7D46 = a[0x39];
    D_800E7D47 = a[0x3A];
    D_800E7D40 = *(u16 *)(a + 0x3C);
    D_800E7D48 = a[0x3E];
    D_800E7D49 = a[0x3F];
    D_800E7D4A = a[0x40];
    D_800E7D4B = a[0x44];
    *(u8 *)&D_800E7D4C = a[0x45];
    e = D_80158A68;
    if (a[0x53] != 0 || a[0x52] == 0) {
        (&D_800E7D4E)[-1] = 0; /* FAKE: same byte as ((u8 *)&D_800E7D4C)[1] through another symbol, so uopt does not forward the 0 into the first |= (the original keeps lbu; or; sb). T-11520 */
        ((u8 *)&D_800E7D4C)[1] |= (a[0x43] != 0) ? 1 : 0;
        ((u8 *)&D_800E7D4C)[1] |= (a[0x49] != 0) ? 2 : 0;
    }
    D_800E7D4E = t[0xB];
    func_800AEFE0();
    func_800AF1A0(*(s32 *)e);
    func_800AE0B0("open event %x ", *(s32 *)e);
    func_800AF000();
    func_8009C210(3);
    func_8009C5E0(0);
    func_8009C674(0);
    func_800AE0B0("return to main!!\n");
}

void func_8013EC1C(void) {
    u8 *q = D_80158A64;
    u8 *p = D_80158A6C;

    if (q[0x12] != 0) {
        if (*(u16 *)(p + 8) < 0x8CA0U) {
            *(u16 *)(p + 8) += 1;
        }
    }
}

void func_8013EC64(void) {
    s32 pad; /* FAKE: moves the spill slot of p to sp+0x28. T-11810 */
    u8 *p;

    p = D_80158A64;
    if ((*(u16 *)(D_80158A60 + 0x14) & D_80155DD0[p[0x21]]) == D_80155DD0[p[0x21]]) {
        p[0x20] = 0x10;
        if (++p[0x21] == 10) {
            if (*(u16 *)p == 2 && *(u16 *)(p + 2) >= 0x50) {
                func_80133AD0(0x80A);
            } else {
                func_80133AD0(0x70A);
            }
            p[0x21] = 0;
            func_80144CF0();
        }
    } else if (*(u16 *)(*(s32 *)&D_80158A60 + 0x14) != 0) { /* FAKE: the s32 view stops uopt reusing the first read. T-11810 */
        p[0x21] = 0;
    }
    if (--p[0x20] == 0) {
        p[0x21] = 0;
    }
}

void func_8013ED70(void) {
    u8 *a;
    u8 *b;
    u8 *c;

    a = D_80158A60;
    b = D_80158A64;
    c = D_80158AA8 + 0x9EC;
    if (b[0x10] != 0) {
        if (*(u16 *)(a + 0x14) & 0x800) {
            func_80044750(0xF1);
            b[0x10] = 0;
            c[0] = 0;
        }
        func_8013EC64();
        if ((*(u16 *)(a + 8) >> 4) & 1) {
            *(u16 *)(c + 6) = 1;
        } else {
            *(u16 *)(c + 6) = 0;
        }
    } else if (*(u16 *)(D_80158A60 + 0x14) & 0x800) {
        func_8008FD68(0);
        func_80044750(0xE1);
        func_80133AD0(0x600);
        b[0x10] = 1;
        *(s16 *)(c + 0xA) = -0x14;
        *(s16 *)(c + 0xC) = 0x74;
        *(s16 *)(c + 0xE) = 8;
        c[2] = 6;
        c[0] = 1;
        *(s16 *)(c + 0x10) = 0;
    }
}

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

void func_8013EFAC(void) {
    s32 pad; /* FAKE: unused slot above p, the original frame has it (T-3330 idiom, real source unknown). T-6010 */
    u8 *p;

    p = D_80158AB4 + *(s32 *)(D_80158A68 + 8) * 0x80B8;
    func_8009C210(1);
    func_8009CC04(p + 0x8048);
    func_8009CCF4(p + 0x80A4);
    func_8009CB9C(p + 0x8044);
}
