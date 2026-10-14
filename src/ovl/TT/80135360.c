#include "common.h"
#include "ovl/TT.h"

/* T-11510 near miss (about 45 differing instructions; frame, spill slots 0x60/0x9C, pt and every branch target match): the first clear loop pointer is s1 in the original (v0 here), temp numbering follows from it, and the two angle sums (idx * 0x733 + 0xC000 - 0x3998 and + 0x4000 - 0xACC8) fold their constants here while the original keeps the hoisted 0xC000/0x4000 and adds the constant separately. The unused s32 locals and pad[4] only place the frame (FAKE, original source unknown). */
#ifdef NON_MATCHING
void func_80135360(void) {
    s32 pq1;
    s32 pq2;
    u8 *a;
    u8 *c;
    u8 *b;
    u8 *p;
    u8 *e;
    s32 i;
    s32 j;
    s32 px1;
    s32 px2;
    s32 px3;
    s16 pt[2];
    s32 pad[4]; /* FAKE: unused, places the frame, spill slots and pt as in the original. T-11510 */

    c = D_80158A8C;
    a = D_80158A74;
    b = D_80158A64;
    if (c[0] != 0) {
        for (i = 0, e = c; i < 0x18; i++, e += 0x14) {
            e[0x42] = 0;
        }
        if (c[0x342] == 0 && func_801427A8() != 0) {
            func_80133AD0(0x503);
        }
        if (c[0x344] == 0) {
            for (i = 0; i != 8; i++) {
                pt[0] = *(s16 *)(c + 0x14) + *(s16 *)(c + 0x258 + i * 2);
                pt[1] = *(s16 *)(c + 0x16) + *(s16 *)(c + 0x288 + i * 2);
                if (a[0x52] == 0 && a[0] != 0 && func_80145508(a + 0x14, D_80155B30, (u8 *)pt, &D_80155B54[c[0xDE + i * 0x14] * 6] + 0x42) != 0) {
                    func_80144094();
                    *(s16 *)(a + 4) = 0x60;
                    *(s16 *)(a + 10) = 0;
                    *(s16 *)(a + 8) = 0;
                    *(s16 *)(a + 6) = 0;
                    a[15] = 0;
                    a[14] = 0;
                    a[13] = 0;
                }
                p = D_80158A80;
                for (j = 0; j != 12; j++, p += 0x54) {
                    if (p[0] != 0 && *(u16 *)(p + 4) == 0x40) {
                    e = i * 0x14 + c;
                    if (func_80145508(p + 0x14, &D_80155B36, (u8 *)pt, &D_80155B54[e[0xDE] * 6] + 0x42) != 0) {
                        b[0x25] = b[0x25] + 1;
                        if (b[0x24] != 0 && !(b[0x25] & 3)) {
                            func_80133D14(*(s16 *)(p + 0x14), *(s16 *)(p + 0x16), 0x20, 0xA0);
                        }
                        *(u16 *)(p + 4) = 0x80;
                        *(s16 *)(p + 10) = 0;
                        *(s16 *)(p + 8) = 0;
                        *(s16 *)(p + 6) = 0;
                        p[15] = 0;
                        p[14] = 0;
                        p[13] = 0;
                        if (c[0x343] != 0) {
                            if (*(u16 *)(c + 6) == 1) {
                                *(u16 *)(c + 0x3B8) += 1;
                                if ((*(u16 *)(c + 0x3B8) & 7) == 7) {
                                    func_80133E30(pt[0], pt[1], 0x18, 0xC00 - (e[0xDE] << 7), 0xA0);
                                }
                            }
                            if (e[0xDE] != 0 && e[0xDE] < 0x10) {
                                *(u16 *)(c + 0x3AA) = 0xC000;
                                if (e[0xDE] >= 9) {
                                    *(u16 *)(c + 0x3AA) = e[0xDE] * 0x733 + 0xC000 - 0x3998;
                                }
                            } else if (e[0xDE] >= 0x11) {
                                *(u16 *)(c + 0x3AA) = 0x4000;
                                if (e[0xDE] < 0x18) {
                                    *(u16 *)(c + 0x3AA) = e[0xDE] * 0x733 + 0x4000 - 0xACC8;
                                }
                            }
                        }
                    }
                    }
                }
            }
        }
    }
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80135360", func_80135360);
#endif

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80135360", func_801356C8);

void func_80135848(s32 arg0) {
    D_80158A8C[0x3E] = ((arg0 + 0x40) & 0xFFF) / 128;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80135360", func_80135870);

void func_80135A54(void) {
    u8 *p = D_80158A8C;

    *(s32 *)(p + 0x20) = 0;
    *(s32 *)(p + 0x24) += 0x10000;
    *(s16 *)(p + 0x14) = *(s16 *)(p + 0x22);
    *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
    if (*(s16 *)(p + 0x16) >= 0x50) {
        *(s32 *)(p + 0x24) = 0x500000;
        *(s16 *)(p + 0x16) = *(s16 *)(p + 0x26);
        *(s16 *)(p + 4) = 0x30;
        *(s16 *)(p + 0xA) = 0;
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 6) = 0;
        p[0xF] = 0;
        p[0xE] = 0;
        p[0xD] = 0;
    }
}

void func_80135AC4(void) {
    u8 *p = D_80158A8C;

    switch (*(u16 *)(p + 8)) {
    case 0:
        p[0x54] = 0x80;
        *(u16 *)(p + 8) = 0x40;
        break;
    case 0x40:
        func_80142AF0(D_80150978, 1);
        p[0x54] &= 1;
        if (p[0x54] != 0) {
            *(u8 **)(p + 0x218) = D_80155B54;
            p[0x343] = 1;
            *(u16 *)(p + 8) = 0;
            *(u16 *)(p + 4) = 0x40;
        }
        break;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80135360", func_80135B5C);

void func_80135E9C(void) {
    u8 *q;

    q = D_80158A8C;
    *(s32 *)(q + 0x20) = func_800A0140(*(u16 *)(D_80158A8C + 0x3AE)) * 0x600;
    *(s32 *)(q + 0x24) = (func_800A0070(*(u16 *)(q + 0x3AE) * 2) << 9) + 0x500000;
    *(s16 *)(q + 0x14) = *(s16 *)(q + 0x22);
    *(s16 *)(q + 0x16) = *(s16 *)(q + 0x26);
    if (*(u16 *)(q + 6) == 2) {
        if (*(u16 *)(q + 0x3AE) == 0x400 || *(u16 *)(q + 0x3AE) == 0x1400) {
            func_80135B5C(1);
            return;
        }
        if (*(u16 *)(q + 0x3AE) == 0xC00 || *(u16 *)(q + 0x3AE) == 0x1C00) {
            func_80135B5C(0);
            return;
        }
        *(u16 *)(q + 0x3AE) += 8;
        if (*(u16 *)(q + 0x3AE) >= 0x2001U) {
            *(u16 *)(q + 0x3AE) -= 0x2000;
        }
    } else {
        *(u16 *)(q + 0x3AE) += 8;
        if (*(u16 *)(q + 0x3AE) >= 0x2001U) {
            *(u16 *)(q + 0x3AE) -= 0x2000;
        }
    }
}

void func_80135FA8(void) {
    u8 *c;
    s32 i;
    s16 *t;
    s16 j;
    u8 *e;
    u16 k;

    k = *(u16 *)(D_80158A8C + 8);
    c = D_80158A8C;
    switch (k) {
    case 0:
        func_80133AD0(0x501);
        c[0x344] = 1;
        func_80142774();
        *(u16 *)(c + 0x364) = 0;
        *(u16 *)(c + 0x3D4) = 0;
        for (i = 8; i < 0x10; i++) {
            ((TtSlot14 *)(c + 0x38))[i].unk_08 = 0x80;
        }
        for (i = 0x10; i != 0x13; i++) {
            ((TtSlot14 *)(c + 0x38))[i].unk_00 = D_80151A50;
            ((TtSlot14 *)(c + 0x38))[i].unk_04 = 0;
            ((TtSlot14 *)(c + 0x38))[i].unk_05 = 4;
            ((TtSlot14 *)(c + 0x38))[i].unk_0A = 0;
            ((TtSlot14 *)(c + 0x38))[i].unk_10 = 0;
            c[0x310 + i] = 0;
            ((s16 *)(c + 0x2A8))[i] = 7;
            ((s16 *)(c + 0x248))[i] = 0;
            ((s16 *)(c + 0x278))[i] = 0;
        }
        *(u16 *)(c + 8) = 0x40;
        break;
    case 0x40:
        t = D_801509C0;
        e = D_80158A8C;
        if (t[*(u16 *)(e + 0x3D4)] != -1 && t[*(u16 *)(e + 0x3D4)] == *(u16 *)(e + 0x364)) {
            func_80133AD0(0x501);
            j = t[*(u16 *)(c + 0x3D4) + 4];
            c[j + 0x310] = 1;
            c[j * 0x14 + 0x40] = 0x80;
            *(s16 *)(c + j * 2 + 0x248) = t[*(u16 *)(c + 0x3D4) + 1];
            *(s16 *)(c + j * 2 + 0x278) = t[*(u16 *)(c + 0x3D4) + 2];
            *(s16 *)(c + j * 2 + 0x2A8) = t[*(u16 *)(c + 0x3D4) + 3];
            *(u16 *)(c + 0x3D4) += 5;
        }
        for (i = 8; i < 0x10; i++) {
            func_80142AF0(D_8015099C, i);
        }
        for (i = 0x10; i < 0x13; i++) {
            func_80142AF0(D_80150950, i);
        }
        if (*(u16 *)(c + 0x364) == 0x85) {
            for (i = 0; i != 0x10; i++) {
                c[0x310 + i] = 0;
            }
        }
        if (*(u16 *)(c + 0x364) == 0x9C) {
            for (i = 0; i != 0x18; i++) {
                c[0x310 + i] = 0;
            }
            *(s16 *)(c + 8) = 0;
            *(s16 *)(c + 4) = 0xA0;
        }
        break;
    }
    *(u16 *)(c + 0x364) += 1;
}

void func_80136244(void) {
    s32 pad; /* FAKE: unused home declared first keeps the frame layout, real source unknown. T-11500 */
    u8 *p;
    u8 *e;
    u8 k;
    s16 v;
    u16 t;
    s16 b;

    k = D_80158A8C[0x3E];
    p = D_80158A8C;
    switch (k) {
    case 0:
        *(u8 **)(p + 0x230) = 0;
        break;
    case 1:
        *(u8 **)(p + 0x230) = D_80155B72;
        break;
    case 2:
        *(u8 **)(p + 0x230) = D_80155B78;
        break;
    case 3:
        *(u8 **)(p + 0x230) = D_80155B7E;
        break;
    case 0x1F:
        *(u8 **)(p + 0x230) = D_80155B84;
        break;
    case 0x1E:
        *(u8 **)(p + 0x230) = D_80155B8A;
        break;
    case 0x1D:
        *(u8 **)(p + 0x230) = D_80155B90;
        break;
    default:
        *(u8 **)(p + 0x230) = D_80155B6C;
        break;
    }
    func_80142AF0(D_80150A14, 4);
    func_80142AF0(D_80150A30, 5);
    switch (*(u16 *)(p + 6)) {
    case 0:
        D_801579D4 = 0x3C10;
        D_80158A5C = 0x3C10;
        break;
    case 1:
        D_801579D4 = 0x3C50;
        D_80158A5C = 0x3C50;
        break;
    case 2:
        D_801579D4 = 0x3C90;
        D_80158A5C = 0x3C90;
        break;
    }
    func_801356C8(*(s16 *)(p + 0x3B4), *(s16 *)(p + 0x3B6), *(u16 *)(p + 0x3A8));
    v = *(u16 *)(p + 0x3AA);
    *(u16 *)(p + 0x3A8) += v / 256;
    if (v < 0) {
        if (v < -0x800) {
            *(u16 *)(p + 0x3AA) = *(u16 *)(p + 0x3AA) + 0x80;
        } else {
            *(u16 *)(p + 0x3AA) = 0xF800;
        }
    } else if (v >= 0x801) {
        *(u16 *)(p + 0x3AA) = *(u16 *)(p + 0x3AA) - 0x80;
    } else {
        *(u16 *)(p + 0x3AA) = 0x800;
    }
    v = *(u16 *)(p + 0x3AA);
    if (((v < 0) ? -v : v) >= 0x801) {
        *(u16 *)(p + 0x3AC) -= v / 256 / 4;
        t = *(u16 *)(p + 0x3AC);
        b = t;
        if (((b < 0) ? -b : b) < 2) {
            *(u16 *)(p + 0x3AC) = 0;
        } else if (b < 0x800) {
            *(u16 *)(p + 0x3AC) = t - 2;
        } else {
            *(u16 *)(p + 0x3AC) = t + 2;
        }
        *(u16 *)(p + 0x3AC) = *(u16 *)(p + 0x3AC) & 0xFFF;
        func_80135848(*(u16 *)(p + 0x3AC));
    }
}

void func_801364EC(void) {
    u8 *q;

    q = D_80158A8C;
    switch (*(u16 *)(q + 6)) {
    case 0:
    case 1:
        if (*(u16 *)(D_80158A8C + 0x364) >= 0x60U) {
            func_80133D14(*(s16 *)(q + 0x14), *(s16 *)(q + 0x16), 0x18, 0xA0);
        }
        break;
    case 2:
        if (*(u16 *)(D_80158A8C + 0x364) >= 0x60U) {
            func_80133FB0(*(s16 *)(q + 0x14), *(s16 *)(q + 0x16), 0x100, 0x18, 0xA0);
        }
        break;
    }
    if (*(u16 *)(q + 0x364) >= 0x60U) {
        *(u16 *)(q + 0x364) = 0;
        return;
    }
    *(u16 *)(q + 0x364) += 1;
}

void func_801365C4(s32 arg0) {
    u8 *c;
    s16 v;
    u8 *t;
    u8 *s;

    s = D_80158A8C;
    c = D_80158A64;
    t = D_80158A74;
    switch (*(u16 *)(s + 4)) {
    case 0x0:
        func_80142570(arg0, t);
        if (*(u16 *)(c + 4) == 0) {
            func_80044750(0xB1);
            func_800451E0(0x8019A000);
            func_80044750(0x12);
            func_80044750(0x23);
            func_80044750(0x2E);
            func_80044750(0x21);
            func_80044750(4);
            func_80044750(0x200);
        }
        func_80135870();
        *(s16 *)(s + 4) = 0x20;
        break;
    case 0x20:
        func_80135A54();
        func_80136244();
        break;
    case 0x30:
        func_80135AC4();
        func_80136244();
        break;
    case 0x40:
        v = *(s16 *)(D_80158A8C + 0x30C);
        if (v < 0x20) {
            *(s16 *)(s + 6) = 2;
        } else if (v < 0x40) {
            *(s16 *)(s + 6) = 1;
        } else {
            *(s16 *)(s + 6) = 0;
        }
        func_80135E9C();
        func_80136244();
        if (*(u16 *)(s + 8) == 0) {
            func_801364EC();
        }
        if (*(s16 *)(s + 0x30C) < 0) {
            *(s16 *)(s + 4) = 0x80;
            *(s16 *)(s + 0xA) = 0;
            *(u16 *)(s + 8) = 0;
            *(s16 *)(s + 6) = 0;
            s[0xF] = 0;
            s[0xE] = 0;
            s[0xD] = 0;
        }
        break;
    case 0x80:
        func_80044750(0xC1);
        t[0x52] = 1;
        func_80135FA8();
        break;
    case 0xA0:
        s[0] = 0;
        c[0x12] = 0;
        *(s16 *)((u8 *)D_80158A70 + 0x1E) = 1;
        *(u16 *)(c + 2) = 0xA0;
        break;
    }
    func_80135360();
}
