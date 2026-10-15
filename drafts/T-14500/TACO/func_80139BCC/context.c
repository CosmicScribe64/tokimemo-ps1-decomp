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

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7200;
extern s32 D_800E7208;
extern s32 D_8015D98C;
extern s32 D_8015D990;
extern s32 D_8015F4B0;
/* end bulk_m2c declarations */
void func_80139BCC(void) {
    s32 sp40;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_lo;
    s32 temp_v1;
    s32 var_a1;
    s32 var_at;
    s32 var_t0;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v1;

    temp_a1 = D_8015F4E4;
    if (temp_a1 < 0) {
        var_v1 = temp_a1 + 0x50;
    } else {
        var_v1 = 0x50 - temp_a1;
    }
    D_8015F4E4 = temp_a1;
    sp40 = var_v1;
    func_80139FB8(D_8015F4E0, temp_a1, 0xA0, 0);
    temp_lo = (s32) (var_v1 * 0x7000) / 80;
    func_80139424((s32) *(&D_8015F4B0 + ((s32) D_8015F4E0 % 40)), 6, 4, 0x80, 0, 0x3C, temp_lo, temp_lo, 1);
    func_8013A580(0x1E0);
    func_8013A790(0, 0);
    temp_a2 = D_800E7200 & 0x2000;
    temp_a0 = D_800E7200 & 0x8000;
    if ((temp_a2 != 0) && (D_8015F4E8 < 0)) {
        D_8015F4E8 = 1;
    }
    var_v0 = D_8015F4E8;
    if ((temp_a0 != 0) && (var_v0 > 0)) {
        var_v0 = -1;
    }
    switch (var_v0) {
    default:
        var_a1 = D_8015F4E4;
        break;
    case -2:
        var_a1 = D_8015F4E4 + 0xA;
        break;
    case -1:
        var_a1 = D_8015F4E4 + 5;
        break;
    case 1:
        var_a1 = D_8015F4E4 - 5;
        break;
    case 2:
        var_a1 = D_8015F4E4 - 0xA;
        break;
    }
    D_8015F4E8 = var_v0;
    if (var_a1 == 0) {
        if (!(D_800E7200 & 0xA000)) {
            var_v0 = 0;
        }
        if (temp_a2 != 0) {
            var_v0 += 1;
        }
        var_at = var_v0 < 3;
        if (temp_a0 != 0) {
            var_v0 -= 1;
            var_at = var_v0 < 3;
        }
        if (var_at == 0) {
            var_v0 = 2;
        }
        D_8015F4E8 = var_v0;
        if (var_v0 < -2) {
            D_8015F4E8 = -2;
        }
    }
    if (var_a1 >= 0x33) {
        D_8015F4E0 += 1;
        var_a1 = -0x32;
    }
    D_8015F4E4 = var_a1;
    var_t0 = D_8015F4E0;
    if (var_a1 < -0x32) {
        D_8015F4E4 = 0x32;
        var_t0 -= 1;
    }
    D_8015F4E0 = var_t0;
    if (var_t0 >= 0x28) {
        D_8015F4E0 = 0;
    }
    if (D_8015F4E0 < 0) {
        D_8015F4E0 = 0x27;
    }
    temp_v1 = D_800E7208;
    if ((temp_v1 & 0x20) && !(temp_v1 & 0x40)) {
        if (D_8015F4E0 == 0x27) {
            func_800AE090((void *) D_8015EE04[(D_8015F4D8 * 4) + D_8015F4DC], &D_8015D98C, 1);
            func_8004284C();
        } else if (D_8015F4DC != 3) {
            func_800AE090(&D_8015EE04[(D_8015F4D8 * 4) + D_8015F4DC], D_8015F4E0 + &D_8015F4B0, 1);
            D_8015F4DC += 1;
        }
    }
    var_v0_2 = temp_v1 & 0x40;
    if ((temp_v1 & 0x800) && (var_v0_2 == 0)) {
        func_800AE090((void *) D_8015EE04[(D_8015F4D8 * 4) + D_8015F4DC], &D_8015D990, 1);
        func_8004284C();
        var_v0_2 = D_800E7208 & 0x40;
    }
    if ((var_v0_2 != 0) && (D_8015F4DC != 0)) {
        func_800AE090(&*(s32 *)((unsigned char *)(&D_8015EE04[(D_8015F4D8 * 4) + D_8015F4DC]) + (-1)), " ", 1);
        D_8015F4DC -= 1;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80139170", func_80139FB8);

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
