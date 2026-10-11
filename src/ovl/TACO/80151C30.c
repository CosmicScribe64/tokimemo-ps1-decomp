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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80151D1C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80151F94);

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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_801522B0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_801523D0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_8015266C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80152764);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80152808);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80152944);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80152C60);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80152DB4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80153068);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_801531E8);

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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80153774);
