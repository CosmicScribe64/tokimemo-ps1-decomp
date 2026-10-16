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

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E6294;
extern s32 D_800E6298;
/* end bulk_m2c declarations */
void func_8013AD64(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp3C;
    s32 sp38;
    s32 sp34;
    s32 sp30;
    s16 var_v1_2;
    s32 temp_t3;
    s32 temp_t6;
    s32 var_t0;
    s32 var_t1;
    s32 var_v1;
    u8 *temp_v0;

    temp_t6 = (arg2 >> 0x10) & 0xFF;
    var_t0 = (arg2 >> 8) & 0xFF;
    temp_t3 = arg2 & 0xFF;
    var_v1 = temp_t6;
    var_t1 = temp_t3;
    if (arg3 < 2) {
        if (arg1 < 0x80) {
            var_v1 = (s32) (temp_t6 * arg1) / 128;
            var_t0 = (s32) (var_t0 * arg1) / 128;
            var_t1 = (s32) (temp_t3 * arg1) / 128;
        }
        var_v1 = (s32) (var_v1 * D_8015F4F0) / 128;
        var_t0 = (s32) (var_t0 * D_8015F4F0) / 128;
        var_t1 = (s32) (var_t1 * D_8015F4F0) / 128;
    }
    sp3C = var_v1;
    sp38 = var_t0;
    sp34 = var_t1;
    temp_v0 = GetWorkBase(0xC, D_8011ECA0);
    func_8009F0B8(temp_v0);
    SetSemiTrans(temp_v0, 0);
    *(s8 *)((unsigned char *)temp_v0 + (4)) = (s8) var_v1;
    *(s8 *)((unsigned char *)temp_v0 + (5)) = (s8) var_t0;
    *(s8 *)((unsigned char *)temp_v0 + (6)) = (s8) var_t1;
    sp30 = *(&D_800E6294 + (D_8011ECA0 * 2)) + ((s32) (arg1 * func_800A0070(arg0)) >> 0xC);
    if (arg3 == 0) {
        var_v1_2 = (*(&D_800E6298 + (D_8011ECA0 * 2)) + ((s32) (arg1 * func_800A0140(arg0)) >> 0xC)) - 0x28;
    } else {
        var_v1_2 = *(&D_800E6298 + (D_8011ECA0 * 2)) + ((s32) (arg1 * func_800A0140(arg0)) >> 0xC);
    }
    *(s16 *)((unsigned char *)temp_v0 + (10)) = var_v1_2;
    *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) sp30;
    if (arg3 < 2) {
        AddPrim((D_8011ECA0 << 0xA) + 0x3F8 + D_800E8CA0, temp_v0);
        return;
    }
    AddPrim((D_8011ECA0 << 0xA) + 0x40 + D_800E8CA0, temp_v0);
}

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
