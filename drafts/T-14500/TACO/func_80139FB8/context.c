#include "common.h"
#include "ovl/TACO.h"

void func_80139170(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013920C();
        return;
    case 1:
        func_80139AB0();
        return;
    case 2:
        func_80139BCC();
        return;
    case 3:
        func_80139B54();
        return;
    case 4:
        func_80139394();
        return;
    case 5:
        func_801393F4();
        return;
    }
}

void func_8013920C(void) {
    s32 previous;

    func_8013A764();
    func_8013AFDC();
    func_800450F4(0, 0x202);
    D_800E6280.unk_036 = 0;
    D_800E6280.unk_037 = 0;
    D_800E6280.unk_038 = 0;
    back_clear_switch(1);
    D_8015EDB0 = 0;
    /* FAKE: the scalar address view and shared source line retain the two
     * score-load registers and their order. Original spelling unknown. T-9530 */
    func_80059048(); previous = *(s32 *)&D_8015EDF8;
    if (previous < D_8015EDF0) {
        D_8015F4D8 = 0;
        D_8015EE00 = D_8015EDFC;
        D_8015EDF8 = D_8015EDF0;
        D_8015EDFC = previous;
        func_800AE090(D_8015EE0C, D_8015EE08, 4);
        func_800AE090(D_8015EE08, D_8015EE04, 4);
        func_800AE090(D_8015EE04, "   ", 4);
    } else if (D_8015EDFC < D_8015EDF0) {
        D_8015F4D8 = 1;
        D_8015EE00 = D_8015EDFC;
        D_8015EDFC = D_8015EDF0;
        func_800AE090(D_8015EE0C, D_8015EE08, 4);
        func_800AE090(D_8015EE08, "   ", 4);
    } else {
        D_8015F4D8 = 2;
        D_8015EE00 = D_8015EDF0;
        func_800AE090(D_8015EE0C, "   ", 4);
    }
    func_8004284C();
}

void func_80139394(void) {
    D_800E6280.unk_110D += 1;
    func_8013A790(0, 0);
    if ((u32)(D_800E6280.unk_110D * 3) >= 0x81U) {
        func_8004284C();
    }
}

void func_801393F4(void) {
    func_80042908(5);
    func_8004284C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80139170", func_80139424);

void func_80139AB0(void) {
    if (D_8015F4F8 < 0x100) {
        D_8015F4F0 = D_8015F4F8 / 2;
    } else {
        D_8015F4F0 = 0x80;
    }
    func_8013A790(0, 0);
    func_80139FB8(0x41, 0, 0x280 - D_8015F4F8, 0);
    D_8015F4F8 += 4;
    if (D_8015F4F8 >= 0x1E1) {
        func_8004284C();
    }
}

void func_80139B54(void) {
    func_8013A790(0, 0);
    func_8013A580(D_8015F4F8);
    func_80139FB8(D_8015F4E0, 0, 0x280 - D_8015F4F8, 0);
    D_8015F4F8 -= 8;
    if (D_8015F4F8 < 0) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80139170", func_80139BCC);

/* bulk_m2c declarations (T-11190) */
extern s32 D_8015F4B0;
/* end bulk_m2c declarations */
void func_80139FB8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_lo;
    s32 temp_s0;
    s32 temp_s1;
    s32 temp_s5;
    s32 temp_s5_2;
    s32 temp_s5_3;
    s32 temp_t1;
    s32 temp_t4;
    s32 temp_t7;
    s32 temp_t8;
    s32 temp_t9;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s2;
    s32 var_s3;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v0_3;
    s32 var_v0_4;
    s32 var_v0_5;

    temp_t7 = (s32) (D_8015F4E4 * -0x400) / 512;
    temp_s5 = (s32) (func_800A0070(temp_t7) * arg2) >> 0xC;
    temp_t9 = (s32) (func_800A0140(temp_t7) * arg2) >> 0xC;
    if (temp_t9 < -0x100) {
        var_v0 = 0;
    } else {
        var_v0 = (s32) ((temp_t9 << 0xC) + 0x100000) / 96;
    }
    func_80139424((s32) *(&D_8015F4B0 + ((s32) D_8015F4E0 % 40)), 2, 5, (s32) ((temp_t9 << 5) + 0x2800) / 160, temp_s5, (temp_t9 >> 2) + 0x32, var_v0, var_v0, 0);
    var_s2 = 1;
    var_s3 = 0x1000;
    do {
        temp_lo = var_s3 / 40;
        temp_v0 = temp_lo - D_8015F4E4;
        if (temp_v0 < 0x200) {
            var_s0 = (s32) (temp_v0 << 0xA) / 512;
        } else {
            var_s0 = (s32) ((temp_v0 * 2) + 0x800) / 3;
        }
        temp_s5_2 = (s32) (func_800A0070(var_s0) * arg2) >> 0xC;
        temp_t4 = (s32) (func_800A0140(var_s0) * arg2) >> 0xC;
        if (temp_t4 < -0x100) {
            var_v0_2 = 0;
        } else {
            var_v0_2 = (s32) ((temp_t4 << 0xC) + 0x100000) / 96;
        }
        func_80139424((s32) *(&D_8015F4B0 + ((s32) (D_8015F4E0 + var_s2) % 40)), 0, 5, (s32) ((temp_t4 << 5) + 0x2800) / 160, temp_s5_2, (temp_t4 >> 2) + 0x32, var_v0_2, var_v0_2, 0);
        temp_v1 = temp_lo + D_8015F4E4;
        if (temp_v1 < 0x200) {
            var_v0_3 = (s32) (temp_v1 << 0xA) / 512;
        } else {
            var_v0_3 = (s32) ((temp_v1 * 2) + 0x800) / 3;
        }
        temp_s0 = 0x1000 - var_v0_3;
        temp_s1 = (s32) (func_800A0070(temp_s0) * arg2) >> 0xC;
        temp_t1 = (s32) (func_800A0140(temp_s0) * arg2) >> 0xC;
        if (temp_t1 < -0x100) {
            var_v0_4 = 0;
        } else {
            var_v0_4 = (s32) ((temp_t1 << 0xC) + 0x100000) / 96;
        }
        func_80139424((s32) *(&D_8015F4B0 + ((s32) ((D_8015F4E0 - var_s2) + 0x28) % 40)), 0, 5, (s32) ((temp_t1 << 5) + 0x2800) / 160, temp_s1, (temp_t1 >> 2) + 0x32, var_v0_4, var_v0_4, 0);
        var_s2 += 1;
        var_s3 += 0x1000;
    } while (var_s2 != 0x14);
    temp_v0_2 = 0x800 - D_8015F4E4;
    if (temp_v0_2 < 0x200) {
        var_s0_2 = (s32) (temp_v0_2 << 0xA) / 512;
    } else {
        var_s0_2 = (s32) ((temp_v0_2 * 2) + 0x800) / 3;
    }
    temp_s5_3 = (s32) (func_800A0070(var_s0_2) * arg2) >> 0xC;
    temp_t8 = (s32) (func_800A0140(var_s0_2) * arg2) >> 0xC;
    if (temp_t8 < -0x100) {
        var_v0_5 = 0;
    } else {
        var_v0_5 = (s32) ((temp_t8 << 0xC) + 0x100000) / 96;
    }
    func_80139424((s32) *(&D_8015F4B0 + ((s32) (D_8015F4E0 + var_s2) % 40)), 0, 5, (s32) ((temp_t8 << 5) + 0x2800) / 160, temp_s5_3, (temp_t8 >> 2) + 0x32, var_v0_5, var_v0_5, 0);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80139170", func_8013A580);

void func_8013A764(void) {
    D_8015F4DC = 0;
    D_8015F4E0 = 0;
    D_8015F4E4 = 0;
    D_8015F4E8 = 0;
    D_8015F4F8 = 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80139170", func_8013A790);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80139170", func_8013AD64);

void func_8013AFDC(void) {
    s32 i;

    func_800AE120(D_800E6280.unk_10F4);
    D_8015F4EC = (TacoRnd *)D_800E6280.unk_163C;
    D_8015F4F0 = 0;
    D_8015F4F4 = 0;
    for (i = 0; i < 0x64; i++) {
        if (i < 0x50) {
            if (func_800AE0D0() % 5 < 4) {
                D_8015F4EC[i].a = func_800AE0D0() % 4096;
                D_8015F4EC[i].b = i * 1000 * i / 30;
            } else {
                D_8015F4EC[i].b = 0;
            }
        } else {
            D_8015F4EC[i].b = 0;
        }
    }
}
