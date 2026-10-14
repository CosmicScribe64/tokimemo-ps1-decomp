#include "common.h"
#include "ovl/TT.h"

/* T-11520 near miss: 2 instruction lines left. Storing the 0 through another symbol (FAKE, see the line) gives the original 'lbu; or; sb' for the first |= (every cast, signed, struct and local form of the same symbol let uopt forward the 0 into 'sb v0'). Left: the original puts the store's lui $at in the delay slot of the first test (bnez t9) and a nop after the second; here as1 fills the second test's delay slot with it. Tried: the && / nested if / goto forms of the condition, the e load in three places, permuter 240 s (no gain). */
#ifdef NON_MATCHING
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
restart:
    b[0xE] = 0;
    b[0x10] = 0;
    func_80044750(0x21);
    func_80044750(0x22);
    func_80044750(0x2F);
    func_80044750(0x21);
    func_80044750(0xF1);
    func_800452C4();
loop:
    func_8013EF38();
    if (func_8013EE88() != 0) {
        if (a[0x53] != 0) {
            goto reset;
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
    if (a[0x55] == 0) {
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
        goto loop;
    }
    goto tail;
reset:
    func_8013E320();
    goto restart;
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
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/8013E850", func_8013E850);
#endif

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
