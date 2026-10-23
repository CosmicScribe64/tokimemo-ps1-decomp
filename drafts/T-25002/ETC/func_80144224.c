/* T-25002 draft, ETC func_80144224 (src/ovl/ETC/80143240.c); 106/97 differing rows, 185 instructions against 194.
 * Done: frame 0x38 (declare rem, quot, v in that order: quot at 0x30, v at 0x28), the divide-by-zero check of
 * the shared 701, func_8014407C(quot), the (&D_8015014C)[i] final read, s32 return (implicit-int caller).
 * Left: the original promotes D_800E7384 over the whole function in a3 (reloaded after every call) and
 * D_80150148 in v1 in the rem == 1 arm with write-backs before each call (lui/sw x3); the draft does neither.
 * Writing the write-backs as self-assignments (variant below the body, FAKE) gives 93/87 rows but frame 0x40.
 * A u32 declaration of D_800E7384 changes nothing. autotweak refuses (instruction shape differs). */
s32 func_80144224(void) {
    u32 rem;
    u32 quot;
    s32 v;

    D_801217D0[0].unk_00 = 0x80000000;
    D_801217D0[1].unk_00 = 0x80000000;
    func_80143968();
    rem = D_800E7384 % 701U;
    quot = D_800E7384 / 701U;
    if (rem == 1) {
        if (D_80150148 == 0) {
            func_801436E4(D_800E71DF);
        } else if (D_80150148 < 0xF) {
            if (D_800E71DF == D_80150150[D_80150148 - 1]) {
                D_80150148++;
            }
            func_801436E4(D_80150150[D_80150148 - 1]);
        } else {
            func_801436E4(D_800E71DF);
        }
        D_80150148 += 1;
    } else if ((s32)quot < 0xF) {
        if (rem == 2) {
            func_8014407C(quot);
        } else if (rem < 0xA0) {
            v = rem * 3;
            func_801441DC(2, v / 4);
            func_801441DC(3, (v - 0x3C) / 4);
            func_801441DC(4, (v - 0x78) / 4);
        }
    }
    if (D_80150148 < 0x10) {
        if (D_800E7384 % 701U >= 0x21E) {
            v = (701 - D_800E7384 % 701U) * 3;
            func_801441DC(2, (v - 0x78) / 4);
            func_801441DC(3, (v - 0x3C) / 4);
            func_801441DC(4, v / 4);
        }
    }
    if ((u32)D_800E7384 >= 0x259 && (func_800460EC() & 2)) {
        func_80042940(2);
    }
    func_8004B19C(D_800E7384, 0, -0xA);
    func_8004B19C(quot, 0, 0);
    func_8004B19C(D_80150148, 0, 0xA);
    func_8004B19C((&D_8015014C)[D_80150148], 0, 0x14);
}

/* n1 variant (93/87 rows, frame 0x40): */
#if 0
s32 func_80144224(void) {
    u32 rem;
    u32 quot;
    s32 v;
    s32 a;
    s32 *p;

    D_801217D0[0].unk_00 = 0x80000000;
    D_801217D0[1].unk_00 = 0x80000000;
    func_80143968();
    rem = D_800E7384 % 701U;
    quot = D_800E7384 / 701U;
    if (rem == 1) {
        if (D_80150148 == 0) {
            func_801436E4(D_800E71DF);
        } else if (D_80150148 < 0xF) {
            p = &D_80150150[D_80150148];
            a = p[-1];
            D_80150148 = D_80150148;
            if (D_800E71DF == a) {
                D_80150148++;
                a = *p++;
            }
            func_801436E4(a);
        } else {
            D_80150148 = D_80150148;
            func_801436E4(D_800E71DF);
        }
        D_80150148 += 1;
    } else if ((s32)quot < 0xF) {
        if (rem == 2) {
            func_8014407C(quot);
        } else if (rem < 0xA0) {
            v = rem * 3;
            func_801441DC(2, v / 4);
            func_801441DC(3, (v - 0x3C) / 4);
            func_801441DC(4, (v - 0x78) / 4);
        }
    }
    if (D_80150148 < 0x10) {
        if (D_800E7384 % 701U >= 0x21E) {
            v = (701 - D_800E7384 % 701U) * 3;
            func_801441DC(2, (v - 0x78) / 4);
            func_801441DC(3, (v - 0x3C) / 4);
            func_801441DC(4, v / 4);
        }
    }
    if ((u32)D_800E7384 >= 0x259 && (func_800460EC() & 2)) {
        func_80042940(2);
    }
    func_8004B19C(D_800E7384, 0, -0xA);
    func_8004B19C(quot, 0, 0);
    func_8004B19C(D_80150148, 0, 0xA);
    func_8004B19C((&D_8015014C)[D_80150148], 0, 0x14);
}
#endif
