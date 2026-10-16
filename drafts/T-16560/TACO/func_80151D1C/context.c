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

/* bulk_m2c declarations (T-11190) */
extern s32 D_80160340;
extern s32 D_801603D4;
/* end bulk_m2c declarations */
s32 func_80151D1C(u32 arg0) {
    s32 *var_s1;
    s32 *var_s1_2;
    s32 *var_s2;
    s32 *var_s2_2;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 var_s0;
    s32 var_s0_2;
    u8 var_v1;

    var_s0 = 0x16;
    if (arg0 < (u8) D_8015EDB4[16].unk3) {
        var_s2 = &D_801603D4;
        var_s1 = &D_80160340;
        do {
            func_801472D4(var_s0, (s32) D_8015EDB4[16].unk2, *(s32 *)((unsigned char *)var_s1 + (-8)), *(s32 *)((unsigned char *)var_s1 + (-4)), (s32) *(u8 *)((unsigned char *)var_s2 + (-2)), (s32) *(u8 *)((unsigned char *)var_s2 + (-1)));
            var_s0 += 1;
            var_s1 += 2;
            var_s2 += 2;
        } while (var_s0 != 0x1C);
        var_v1 = D_8015EDB4[16].unk2;
        if ((var_v1 % 10U) == 0) {
            func_801335A0(0, 0x502);
            var_v1 = D_8015EDB4[16].unk2;
        }
        D_8015EDB4[16].unk2 = (u8) (var_v1 + 1);
        D_8015EDB4[16].unk3 = 0U;
        if (((u8) D_8015EDB4[16].unk2 % 3U) == 0) {
            temp_s0 = func_800AE0D0();
            func_80147C98(0, 0x16, (s16) ((temp_s0 & 0x1FF) - 0x100), (s16) ((func_800AE0D0() & 0x1FF) - 0x100), 0x3E8, 0xC8, 0xC4);
            temp_s0_2 = func_800AE0D0();
            func_80147C98(0, 0x17, (s16) ((temp_s0_2 & 0x1FF) - 0x100), (s16) ((func_800AE0D0() & 0x1FF) - 0x100), 0x3E8, 0xC8, 0xC4);
        }
    }
    if ((u8) D_8015EDB4[16].unk2 >= 0x1EU) {
        D_8015EDB4[16].unk2 = 0U;
        var_s2_2 = &D_801603D4;
        var_s1_2 = &D_80160340;
        var_s0_2 = 0x16;
        D_8015EDB4[16].unk3 = (u8) D_8015EDB4[16].unk2;
        do {
            func_801472D4(var_s0_2, 0, *(s32 *)((unsigned char *)var_s1_2 + (-8)), *(s32 *)((unsigned char *)var_s1_2 + (-4)), (s32) *(u8 *)((unsigned char *)var_s2_2 + (-2)), (s32) *(u8 *)((unsigned char *)var_s2_2 + (-1)));
            var_s0_2 += 1;
            var_s1_2 += 2;
            var_s2_2 += 2;
        } while (var_s0_2 != 0x1C);
        if (D_8015EDB4[17].unk84[2] == 1) {
            D_8015EDB4[16].unk84[2] = 1;
        }
    } else {
        D_8015EDB4[16].unk3 = (u8) (D_8015EDB4[16].unk3 + 1);
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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_801523D0);

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
        func_801478F8(0x23, (s32) D_80160444);
        return;
    }
    D_8015EDB4[17].unk3 = D_8015EDB4[17].unk3 + 1;
}

void func_80152764(s32 arg0) {
    if (arg0 < 5) {
        func_80147674(0x23, D_80160444, 1, 0, 0);
        return;
    }
    if ((arg0 >= 5) && (arg0 < 0xF)) {
        func_80147674(0x23, D_80160444, 2, 0, 0);
        return;
    }
    if ((arg0 >= 0xF) && (arg0 < 0x14)) {
        func_80147674(0x23, D_80160444, 1, 0, 0);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80152808);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80152944);

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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80152DB4);

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
