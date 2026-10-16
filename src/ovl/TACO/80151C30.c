#include "common.h"
#include "ovl/TACO.h"

void func_80151C30(void) {
    s32 pad[4]; /* FAKE: unused slots above r, the original frame is 16 bytes larger (T-3330 idiom, real source unknown). T-6010 */
    s32 r;

    if (D_8015EDB4[16].unk84[2] == 0) {
        func_80151D1C(5);
    }
    func_8015266C(6);
    func_80152808(4);
    func_801532C4();
    func_801536BC();
    if (D_8015EDB4[16].unk84[1] == 0 && D_8015EDB4[17].unk84[1] == 0 && D_8015EDB4[18].unk84[1] == 0) {
        r = func_800AE0D0();
        func_80147C98(0, 0x1C, (s16)((r & 0x1FF) - 0x100), (s16)((func_800AE0D0() & 0x1FF) - 0x100), 0x3E8, 0x190, 0xC4);
    }
}

s32 func_80151D1C(u32 arg0) {
    s32 i;
    s32 r;

    if (arg0 < D_8015EDB4[16].unk3) {
        for (i = 0x16; i != 0x1C; i++) {
            func_801472D4(i, D_8015EDB4[16].unk2, D_80160290[i - 1].a, D_80160290[i - 1].b, D_801603A8[i - 1].a, D_801603A8[i - 1].b);
        }
        if (D_8015EDB4[16].unk2 % 10 == 0) {
            func_801335A0(0, 0x502);
        }
        D_8015EDB4[16].unk2 = D_8015EDB4[16].unk2 + 1;
        D_8015EDB4[16].unk3 = 0;
        if (D_8015EDB4[16].unk2 % 3 == 0) {
            r = func_800AE0D0();
            func_80147C98(0, 0x16, (s16)((r & 0x1FF) - 0x100), (s16)((func_800AE0D0() & 0x1FF) - 0x100), 0x3E8, 0xC8, 0xC4);
            r = func_800AE0D0();
            func_80147C98(0, 0x17, (s16)((r & 0x1FF) - 0x100), (s16)((func_800AE0D0() & 0x1FF) - 0x100), 0x3E8, 0xC8, 0xC4);
        }
    }
    if (D_8015EDB4[16].unk2 >= 0x1E) {
        D_8015EDB4[16].unk2 = 0;
        D_8015EDB4[16].unk3 = D_8015EDB4[16].unk2;
        for (i = 0x16; i != 0x1C; i++) {
            func_801472D4(i, 0, D_80160290[i - 1].a, D_80160290[i - 1].b, D_801603A8[i - 1].a, D_801603A8[i - 1].b);
        }
        if (D_8015EDB4[17].unk84[2] == 1) {
            D_8015EDB4[16].unk84[2] = 1;
        }
    } else {
        D_8015EDB4[16].unk3 = D_8015EDB4[16].unk3 + 1;
    }
}

void func_80151F94(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 pad[15]; /* FAKE: unused slots above idx, the original frame is 56 bytes larger (T-3330 idiom, real source unknown). T-9370 */
    s32 idx;

    if ((u32)arg4 < *arg0) {
        func_80152110(arg2, arg3, *arg1);
        *arg1 += 1;
        *arg0 = 0;
    }
    if (*arg1 >= 0x1E && *arg1 < 0x21) {
        if (arg2 == 1) {
            idx = 0x10;
        } else if (arg2 == 2) {
            idx = 0x11;
        } else if (arg2 == 3) {
            idx = 0x12;
        }
        if ((D_8015EDB4 + idx)->unk84[1] != 0) {
            func_80147C98(0, arg2, 0, 0x64, 0x3E8, 0xC8, 0x83);
            if (*arg0 == 0 && *arg1 == 0x1E) {
                func_801335A0(0, 0x501);
            }
        }
    }
    if (*arg1 >= 0x28) {
        *arg0 = *arg1 = 0;
        func_801534E0(arg2);
        func_801521EC(arg2, arg3);
    } else {
        *arg0 += 1;
    }
}

/* The table is indexed (D_801603F0[arg1]), not walked with a local pointer: uopt then knows the
 * loads do not alias the stack, and as1 leaves the stack-argument store for the jal delay slot
 * (T-9210). */
void func_80152110(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 j;

    if (arg2 < 0x14) {
        for (i = 0, j = arg0; i != 0x15; i += 3, j += 3, arg1++) {
            func_80147674(j, D_801603F0[arg1], 1, 0, 0);
        }
    } else if (arg2 >= 0x14 && arg2 < 0x28) {
        for (i = 0, j = arg0; i != 0x15; i += 3, j += 3, arg1++) {
            func_80147674(j, D_801603F0[arg1], 2, 0, 0);
        }
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_801521EC);

void func_801522B0(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3) {
    u8 var_v0;

    var_v0 = *arg1;
    if ((var_v0 < 0x1EU) || (var_v0 >= 0x23U)) {
        if (*arg0 >= 3U) {
            func_801523D0(arg2, arg3, (s32) var_v0);
            *arg1 += 1;
            *arg0 = 0;
            var_v0 = *arg1;
        }
    } else if (var_v0 < 0x23U) {
        if (var_v0 >= 0x1EU) {
            if (*arg0 != 0) {
                func_801523D0(arg2, arg3, (s32) var_v0);
                *arg1 += 1;
                *arg0 = 0;
                var_v0 = *arg1;
            }
        }
    }
    if (var_v0 >= 0x2D) {
        *arg1 = 0;
        *arg0 = 0;
        func_801534E0(arg2);
        func_801521EC(arg2, arg3);
        return;
    }
    /* FAKE: redundant byte mask reserves the original increment temporary;
     * real source unknown. T-9760 */
    *arg0 = (u8)((*arg0 & 0xFF) + 1);
}

void func_801523D0(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 pad[15]; /* FAKE: unused slots above k; the original frame is 0x40 larger and k sits at sp+0x4C (real source unknown). T-16563 */
    s32 k;
    s32 pad2; /* FAKE: one more unused slot below k, T-16563 */

    if (arg2 < 8) {
        for (i = 0; i < 7; i++) {
            func_80147674(arg0 + i * 3, D_801603F0[arg1], 3, 0, 0);
            arg1++;
        }
    } else if (arg2 >= 8 && arg2 < 0x10) {
        for (i = 0; i < 7; i++) {
            func_80147674(arg0 + i * 3, D_801603F0[arg1], 4, 0, 0);
            arg1++;
        }
    } else if (arg2 >= 0x10 && arg2 < 0x1A) {
        for (i = 0; i < 7; i++) {
            func_80147674(arg0 + i * 3, D_801603F0[arg1], 5, 0, 0);
            arg1++;
        }
    } else if (arg2 == 0x1D) {
        func_801335A0(0, 0x501);
    } else if (arg2 >= 0x1E && arg2 < 0x23) {
        for (i = 0; i < 7; i++) {
            func_80147674(arg0 + i * 3, D_801603F0[arg1], 6, 0, 0);
            arg1++;
        }
        if (arg0 == 1) {
            k = 0x10;
        } else if (arg0 == 2) {
            k = 0x11;
        } else if (arg0 == 3) {
            k = 0x12;
        }
        if ((k + D_8015EDB4)->unk84[1] != 0) {
            func_80147C98(0, arg0, 0, 0x64, 0x3E8, 0xC8, 0x83);
        }
    } else if (arg2 >= 0x23 && arg2 < 0x2D) {
        for (i = 0; i < 7; i++) {
            func_80147674(arg0 + i * 3, D_801603F0[arg1], 7, 0, 0);
            arg1++;
        }
    }
}


void func_8015266C(u32 arg0) {
    if (arg0 < D_8015EDB4[17].unk3) {
        func_80152764((s32) D_8015EDB4[17].unk2);
        D_8015EDB4[17].unk2 = D_8015EDB4[17].unk2 + 1;
        D_8015EDB4[17].unk3 = 0U;
    }
    if (D_8015EDB4[17].unk2 >= 0x14U) {
        D_8015EDB4[17].unk2 = 0U;
        D_8015EDB4[17].unk3 = D_8015EDB4[17].unk2;
        func_801472D4(0x23, 0, D_801603A0, D_801603A4, D_801603EC, D_801603ED);
        func_801478F8(0x23, (s32) D_80160444[0]);
        return;
    }
    D_8015EDB4[17].unk3 = D_8015EDB4[17].unk3 + 1;
}

void func_80152764(s32 arg0) {
    if (arg0 < 5) {
        func_80147674(0x23, D_80160444[0], 1, 0, 0);
        return;
    }
    if ((arg0 >= 5) && (arg0 < 0xF)) {
        func_80147674(0x23, D_80160444[0], 2, 0, 0);
        return;
    }
    if ((arg0 >= 0xF) && (arg0 < 0x14)) {
        func_80147674(0x23, D_80160444[0], 1, 0, 0);
    }
}

s32 func_80152808(u32 arg0) {
    s32 i;

    if (arg0 < D_8015EDB4[21].unk3) {
        func_80152944(0x1D, D_8015EDB4[21].unk2);
        D_8015EDB4[21].unk2 = D_8015EDB4[21].unk2 + 1;
        D_8015EDB4[21].unk3 = 0;
    }
    if (D_8015EDB4[21].unk2 >= 0x46) {
        D_8015EDB4[21].unk2 = 0;
        D_8015EDB4[21].unk3 = D_8015EDB4[21].unk2;
        i = 0x1D;
        for (; i != 0x21; i++) {
            func_801472D4(i, 0, D_80160290[i - 1].a, D_80160290[i - 1].b, D_801603A8[i - 1].a, D_801603A8[i - 1].b);
            /* FAKE: pointer table read from the base D_801603D4 (see TACO.h); D_80160444[i - 0x1C] gives another offset. T-16563 */
            func_801478F8(i, D_801603D4[i]);
        }
    } else {
        D_8015EDB4[21].unk3 = D_8015EDB4[21].unk3 + 1;
    }
}

void func_80152944(s32 arg0, s32 arg1) {
    s32 i;

    if (arg1 < 8) {
        for (i = 0; i < 4; i++) {
            func_80147674(arg0 + i, D_80160444[i + 1], 1, 0, 0);
        }
    } else if (arg1 >= 8 && arg1 < 0x10) {
        for (i = 0; i < 4; i++) {
            func_80147674(arg0 + i, D_80160444[i + 1], 2, 0, 0);
        }
    } else if (arg1 >= 0x10 && arg1 < 0x18) {
        for (i = 0; i < 4; i++) {
            func_80147674(arg0 + i, D_80160444[i + 1], 3, 0, 0);
        }
    } else if (arg1 >= 0x18 && arg1 < 0x20) {
        for (i = 0; i < 4; i++) {
            func_80147674(arg0 + i, D_80160444[i + 1], 4, 0, 0);
        }
    } else if (arg1 >= 0x20 && arg1 < 0x28) {
        for (i = 0; i < 4; i++) {
            func_80147674(arg0 + i, D_80160444[i + 1], 5, 0, 0);
        }
    } else if (arg1 >= 0x28 && arg1 < 0x2E) {
        for (i = 0; i < 4; i++) {
            func_80147674(arg0 + i, D_80160444[i + 1], 6, 0, 0);
        }
    } else if (arg1 >= 0x2E && arg1 < 0x36) {
        for (i = 0; i < 3; i++) {
            func_80147674(arg0 + i, D_80160444[i + 1], 7, 0, 0);
        }
    } else if (arg1 >= 0x36 && arg1 < 0x3E) {
        for (i = 0; i < 3; i++) {
            func_80147674(arg0 + i, D_80160444[i + 1], 8, 0, 0);
        }
        func_80147674(0x20, D_80160444[4], 7, 0, 0);
    } else if (arg1 >= 0x3E && arg1 < 0x46) {
        for (i = 0; i < 3; i++) {
            func_80147674(arg0 + i, D_80160444[i + 1], 9, 0, 0);
        }
        func_80147674(0x20, D_80160444[4], 8, 0, 0);
    }
}

void func_80152C60(s32 arg0) {
    if ((u32) arg0 < D_8015EDB4[18].unk3) {
        func_80152DB4(1, 0, D_8015EDB4[18].unk2);
        func_80152DB4(2, 7, D_8015EDB4[18].unk2);
        func_80152DB4(3, 0xE, D_8015EDB4[18].unk2);
        D_8015EDB4[18].unk2 = D_8015EDB4[18].unk2 + 1;
        D_8015EDB4[18].unk3 = 0U;
    }
    if (D_8015EDB4[18].unk2 == 0x50) {
        D_8015EDB4[16].unk84[2] = 0;
    }
    if (D_8015EDB4[18].unk2 >= 0x6EU) {
        D_8015EDB4[18].unk2 = 0U;
        D_8015EDB4[18].unk3 = D_8015EDB4[18].unk2;
        D_8015EDB4[17].unk84[2] = 0;
        func_801534E0(1);
        func_801534E0(2);
        func_801534E0(3);
        func_801521EC(1, 0);
        func_801521EC(2, 7);
        func_801521EC(3, 0xE);
        return;
    }
    D_8015EDB4[18].unk3 = D_8015EDB4[18].unk3 + 1;
}

/* arg1 walks the table (arg1++ after each call): it keeps the pointer walk and the 3-step counter apart, T-16563. */
void func_80152DB4(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 r;

    if (arg2 < 10) {
        for (i = 0; i < 7; i++) {
            func_80147674(arg0 + i * 3, D_801603F0[arg1], 8, 0, 0);
            arg1++;
        }
    } else if (arg2 >= 10 && arg2 < 20) {
        for (i = 0; i < 7; i++) {
            func_80147674(arg0 + i * 3, D_801603F0[arg1], 9, 0, 0);
            arg1++;
        }
    } else if (arg2 >= 20 && arg2 < 30) {
        for (i = 0; i < 7; i++) {
            func_80147674(arg0 + i * 3, D_801603F0[arg1], 10, 0, 0);
            arg1++;
        }
    } else if (arg2 >= 30 && arg2 < 80) {
        r = func_800AE0D0();
        func_80147C98(0, 0x1C, (s16)((r & 0x3FF) - 0x200), (s16)((func_800AE0D0() & 0x3FF) - 0x200), 0x3E8, 0x1F4, 0x86);
    } else if (arg2 >= 80 && arg2 < 90) {
        for (i = 0; i < 7; i++) {
            func_80147674(arg0 + i * 3, D_801603F0[arg1], 11, 0, 0);
            arg1++;
        }
    } else if (arg2 >= 90 && arg2 < 100) {
        for (i = 0; i < 7; i++) {
            func_80147674(arg0 + i * 3, D_801603F0[arg1], 12, 0, 0);
            arg1++;
        }
    } else if (arg2 >= 100 && arg2 < 110) {
        for (i = 0; i < 7; i++) {
            func_80147674(arg0 + i * 3, D_801603F0[arg1], 13, 0, 0);
            arg1++;
        }
    }
}


void func_80153068(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 pad[15]; /* FAKE: unused slots above idx, the original frame is 64 bytes larger (T-3330 idiom, real source unknown). T-9370 */
    s32 idx;
    s32 gap[1]; /* FAKE: unused slot between idx and r, same idiom. T-9370 */
    s32 r;

    if ((u32)arg4 < *arg0) {
        func_801531E8(arg2, arg3, *arg1);
        *arg1 += 1;
        *arg0 = 0;
        if (arg2 == 1) {
            idx = 0x10;
        } else if (arg2 == 2) {
            idx = 0x11;
        } else if (arg2 == 3) {
            idx = 0x12;
        }
        if ((D_8015EDB4 + idx)->unk84[1] != 0 && !(*arg1 & 1)) {
            r = func_800AE0D0();
            func_80147C98(0, arg2, (s16)((r & 0x1FF) - 0x100), (s16)((func_800AE0D0() & 0x1FF) + 0xF4), 0x3E8, 0x190, 0x82);
        }
    }
    if (*arg1 >= 0x14) {
        *arg0 = *arg1 = 0;
        func_801534E0(arg2);
        func_801521EC(arg2, arg3);
    } else {
        *arg0 += 1;
    }
}

void func_801531E8(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 j;

    if (arg2 < 0x0A) {
        for (i = 0, j = arg0; i != 0x15; i += 3, j += 3, arg1++) {
            func_80147674(j, D_801603F0[arg1], 14, 0, 0);
        }
    } else if (arg2 >= 0x0A && arg2 < 0x14) {
        for (i = 0, j = arg0; i != 0x15; i += 3, j += 3, arg1++) {
            func_80147674(j, D_801603F0[arg1], 15, 0, 0);
        }
    }
}

void func_801532C4(void) {
    u8 temp;

    temp = D_8015EDB4[18].unk84[2];
    if (temp == 1) {
        func_80151F94(&D_8015EDB4[18].unk3, &D_8015EDB4[18].unk2, 1, 0, 4);
    } else if (temp == 2) {
        func_80151F94(&D_8015EDB4[18].unk3, &D_8015EDB4[18].unk2, 1, 0, 2);
    } else if (temp == 3) {
        func_801522B0(&D_8015EDB4[18].unk3, &D_8015EDB4[18].unk2, 1, 0);
    }
    temp = D_8015EDB4[19].unk84[2];
    if (temp == 1) {
        func_80151F94(&D_8015EDB4[19].unk3, &D_8015EDB4[19].unk2, 2, 7, 3);
    } else if (temp == 2) {
        func_80153068(&D_8015EDB4[19].unk3, &D_8015EDB4[19].unk2, 2, 7, 4);
    } else if (temp == 3) {
        func_801522B0(&D_8015EDB4[19].unk3, &D_8015EDB4[19].unk2, 2, 7);
    }
    temp = D_8015EDB4[20].unk84[2];
    if (temp == 1) {
        func_80151F94(&D_8015EDB4[20].unk3, &D_8015EDB4[20].unk2, 3, 14, 2);
    } else if (temp == 2) {
        func_80153068(&D_8015EDB4[20].unk3, &D_8015EDB4[20].unk2, 3, 14, 3);
    } else if (temp == 3) {
        func_801522B0(&D_8015EDB4[20].unk3, &D_8015EDB4[20].unk2, 3, 14);
    }
    if (D_8015EDB4[17].unk84[2] == 1 && D_8015EDB4[19].unk84[2] == 0 && D_8015EDB4[18].unk84[2] == 0 && D_8015EDB4[20].unk84[2] == 0) {
        func_80152C60(4);
    }
}

void func_801534E0(s32 arg0) {
    s32 r;
    s32 m;

    r = func_800AE0D0();
    if (arg0 == 1) {
        if (D_8015EDB4[17].unk84[2] != 0) {
            D_8015EDB4[18].unk84[2] = 0;
            return;
        }
        m = r % 100;
        if (m < 0x32) {
            D_8015EDB4[18].unk84[2] = 1;
            return;
        }
        if (m >= 0x32 && m < 0x46) {
            D_8015EDB4[18].unk84[2] = 2;
            return;
        }
        if (m >= 0x46) {
            D_8015EDB4[18].unk84[2] = 3;
        }
    } else if (arg0 == 2) {
        if (D_8015EDB4[17].unk84[2] != 0) {
            D_8015EDB4[19].unk84[2] = 0;
            return;
        }
        m = r % 100;
        if (m < 0x32) {
            D_8015EDB4[19].unk84[2] = 1;
            return;
        }
        if (m >= 0x32 && m < 0x46) {
            D_8015EDB4[19].unk84[2] = 2;
            return;
        }
        if (m >= 0x46) {
            D_8015EDB4[19].unk84[2] = 3;
        }
    } else if (arg0 == 3) {
        if (D_8015EDB4[17].unk84[2] != 0) {
            D_8015EDB4[20].unk84[2] = 0;
            return;
        }
        m = r % 100;
        if (m < 0x32) {
            D_8015EDB4[20].unk84[2] = 1;
            return;
        }
        if (m >= 0x32 && m < 0x46) {
            D_8015EDB4[20].unk84[2] = 2;
            return;
        }
        if (m >= 0x46) {
            D_8015EDB4[20].unk84[2] = 3;
        }
    }
}

void func_801536BC(u32 arg0) {
    if (arg0 < D_8015EDB4[23].unk3) {
        func_8015131C(D_8015EDB4[23].unk2);
        D_8015EDB4[23].unk2 = D_8015EDB4[23].unk2 + 1;
        D_8015EDB4[23].unk3 = 0;
    }
    if (D_8015EDB4[23].unk2 >= 0x7CU) {
        D_8015EDB4[23].unk2 = 0;
        D_8015EDB4[23].unk3 = D_8015EDB4[23].unk2;
        return;
    }
    D_8015EDB4[23].unk3 = D_8015EDB4[23].unk3 + 1;
}

void func_80153774(s32 arg0) {
    if (arg0 < 0xA) {
        func_80146F74(0, 8, 0, 0, 0, 0, 0, 0xC8, 0xC8);
        return;
    }
    if ((arg0 >= 0xA) && (arg0 < 0x14)) {
        func_80146F74(0, 8, -1, 0, 0, 0, 0, 0xC8, 0xC8);
        return;
    }
    if ((arg0 >= 0x14) && (arg0 < 0x1E)) {
        func_80146F74(0, 8, -2, 0, 0, 0, 0, 0xC8, 0xC8);
        return;
    }
    if ((arg0 >= 0x20) && (arg0 < 0x2A)) {
        func_80146F74(0, -8, 0xF, 0, 0, 0, 0, 0xC8, 0xC8);
        return;
    }
    if ((arg0 >= 0x2A) && (arg0 < 0x34)) {
        func_80146F74(0, -8, 0xA, 0, 0, 0, 0, 0xC8, 0xC8);
        return;
    }
    if ((arg0 >= 0x34) && (arg0 < 0x48)) {
        func_80146F74(0, -8, 0, 0, 0, 0, 0, 0xC8, 0xC8);
        return;
    }
    if ((arg0 >= 0x48) && (arg0 < 0x52)) {
        func_80146F74(0, -8, -1, 0, 0, 0, 0, 0xC8, 0xC8);
        return;
    }
    if ((arg0 >= 0x52) && (arg0 < 0x5C)) {
        func_80146F74(0, -8, -2, 0, 0, 0, 0, 0xC8, 0xC8);
        return;
    }
    if ((arg0 >= 0x5E) && (arg0 < 0x68)) {
        func_80146F74(0, 8, 0xF, 0, 0, 0, 0, 0xC8, 0xC8);
        return;
    }
    if ((arg0 >= 0x68) && (arg0 < 0x72)) {
        func_80146F74(0, 8, 0xA, 0, 0, 0, 0, 0xC8, 0xC8);
        return;
    }
    if ((arg0 >= 0x72) && (arg0 < 0x7C)) {
        func_80146F74(0, 8, 0, 0, 0, 0, 0, 0xC8, 0xC8);
    }
}
