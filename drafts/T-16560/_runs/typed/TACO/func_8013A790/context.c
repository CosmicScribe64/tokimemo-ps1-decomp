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

void func_8013A790(s32 arg0, s32 arg1) {
    s32 sp44;
    s32 *temp_v0_2;
    s32 *temp_v0_4;
    s32 *var_v0;
    s32 *var_v0_4;
    s32 *var_v0_8;
    s32 temp_a0;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 temp_v1_5;
    s32 temp_v1_6;
    s32 temp_v1_7;
    s32 temp_v1_8;
    s32 var_a2;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s2;
    s32 var_s4;
    s32 var_v1;
    s32 var_v1_2;
    u8 temp_v0;
    u8 *temp_v0_3;
    u8 *temp_v0_5;
    u8 *var_v0_2;
    u8 *var_v0_3;
    u8 *var_v0_5;
    u8 *var_v0_6;
    u8 *var_v0_7;

    var_s4 = 0x7530;
    var_a2 = 0;
    if (arg1 == 0) {
        if (D_800E6280.unk_F80 & 1) {
            var_s4 = 0x2710;
        } else if (D_800E6280.unk_F80 & 2) {
            var_s4 = 0xC350;
        }
        if (D_800E6280.unk_F80 & 4) {
            var_a2 = 0x28;
        }
        if (D_800E6280.unk_F80 & 8) {
            var_a2 = -0x28;
        }
    } else if (arg1 == 1) {
        temp_v0 = D_8015EDB4->unk2;
        if ((temp_v0 & 1) == 1) {
            var_a2 = 0x28;
        }
        if ((temp_v0 & 2) == 2) {
            var_a2 = -0x28;
        }
    } else if (arg1 == 2) {
        var_a2 = 0x28;
        var_s4 = 0x4E20;
    } else if (arg1 == 3) {
        var_a2 = 0x28;
        var_s4 = 0x2710;
    }
    var_s2 = 0;
    if (var_a2 > 0) {
        var_s0 = 0;
        do {
            temp_v0_2 = D_8015F4EC + var_s0;
            *temp_v0_2 += var_a2;
            var_v0 = D_8015F4EC + var_s0;
            temp_v1 = *var_v0;
            if (temp_v1 >= 0x1001) {
                *var_v0 = temp_v1 - 0x1000;
                var_v0 = D_8015F4EC + var_s0;
            }
            *(s32 *)((unsigned char *)var_v0 + (8)) = (s32) (*(s32 *)((unsigned char *)var_v0 + (8)) + var_a2);
            var_v0_2 = D_8015F4EC + var_s0;
            temp_v1_2 = *(s32 *)((unsigned char *)var_v0_2 + (8));
            if (temp_v1_2 >= 0x1001) {
                *(s32 *)((unsigned char *)var_v0_2 + (8)) = (s32) (temp_v1_2 - 0x1000);
                var_v0_2 = D_8015F4EC + var_s0;
            }
            *(s32 *)((unsigned char *)var_v0_2 + (16)) = (s32) (*(s32 *)((unsigned char *)var_v0_2 + (16)) + var_a2);
            var_v0_3 = D_8015F4EC + var_s0;
            temp_v1_3 = *(s32 *)((unsigned char *)var_v0_3 + (16));
            if (temp_v1_3 >= 0x1001) {
                *(s32 *)((unsigned char *)var_v0_3 + (16)) = (s32) (temp_v1_3 - 0x1000);
                var_v0_3 = D_8015F4EC + var_s0;
            }
            *(s32 *)((unsigned char *)var_v0_3 + (24)) = (s32) (*(s32 *)((unsigned char *)var_v0_3 + (24)) + var_a2);
            temp_v0_3 = D_8015F4EC + var_s0;
            temp_v1_4 = *(s32 *)((unsigned char *)temp_v0_3 + (24));
            var_s0 += 0x20;
            if (temp_v1_4 >= 0x1001) {
                *(s32 *)((unsigned char *)temp_v0_3 + (24)) = (s32) (temp_v1_4 - 0x1000);
            }
        } while (var_s0 != 0x320);
    }
    if (var_a2 < 0) {
        var_s0_2 = 0;
        do {
            temp_v0_4 = D_8015F4EC + var_s0_2;
            *temp_v0_4 += var_a2;
            var_v0_4 = D_8015F4EC + var_s0_2;
            temp_v1_5 = *var_v0_4;
            if (temp_v1_5 < 0) {
                *var_v0_4 = temp_v1_5 + 0x1000;
                var_v0_4 = D_8015F4EC + var_s0_2;
            }
            *(s32 *)((unsigned char *)var_v0_4 + (8)) = (s32) (*(s32 *)((unsigned char *)var_v0_4 + (8)) + var_a2);
            var_v0_5 = D_8015F4EC + var_s0_2;
            temp_v1_6 = *(s32 *)((unsigned char *)var_v0_5 + (8));
            if (temp_v1_6 < 0) {
                *(s32 *)((unsigned char *)var_v0_5 + (8)) = (s32) (temp_v1_6 + 0x1000);
                var_v0_5 = D_8015F4EC + var_s0_2;
            }
            *(s32 *)((unsigned char *)var_v0_5 + (16)) = (s32) (*(s32 *)((unsigned char *)var_v0_5 + (16)) + var_a2);
            var_v0_6 = D_8015F4EC + var_s0_2;
            temp_v1_7 = *(s32 *)((unsigned char *)var_v0_6 + (16));
            if (temp_v1_7 < 0) {
                *(s32 *)((unsigned char *)var_v0_6 + (16)) = (s32) (temp_v1_7 + 0x1000);
                var_v0_6 = D_8015F4EC + var_s0_2;
            }
            *(s32 *)((unsigned char *)var_v0_6 + (24)) = (s32) (*(s32 *)((unsigned char *)var_v0_6 + (24)) + var_a2);
            temp_v0_5 = D_8015F4EC + var_s0_2;
            temp_v1_8 = *(s32 *)((unsigned char *)temp_v0_5 + (24));
            var_s0_2 += 0x20;
            if (temp_v1_8 < 0) {
                *(s32 *)((unsigned char *)temp_v0_5 + (24)) = (s32) (temp_v1_8 + 0x1000);
            }
        } while (var_s0_2 != 0x320);
    }
    var_s0_3 = 0;
    do {
        temp_a0 = var_s2 + 1;
        var_v0_7 = D_8015F4EC + var_s0_3;
        var_v1 = *(s32 *)((unsigned char *)var_v0_7 + (4));
        if (var_v1 == 0) {
            sp44 = temp_a0;
            if ((func_800AE0D0() % temp_a0) == 0) {
                var_s2 = temp_a0;
                *(D_8015F4EC + var_s0_3) = func_800AE0D0() % 4096;
                if (arg0 == 0) {
                    *(s32 *)((unsigned char *)(D_8015F4EC + var_s0_3) + (4)) = 0x3A980;
                } else {
                    *(s32 *)((unsigned char *)(D_8015F4EC + var_s0_3) + (4)) = 0x3E8;
                }
            }
            var_v0_7 = D_8015F4EC + var_s0_3;
            var_v1 = *(s32 *)((unsigned char *)var_v0_7 + (4));
        } else {
            var_s2 = temp_a0;
        }
        if (arg0 == 0) {
            *(s32 *)((unsigned char *)var_v0_7 + (4)) = (s32) (var_v1 - ((s32) (var_v1 * 0x3E8) / var_s4));
            var_v0_8 = D_8015F4EC + var_s0_3;
            var_v1_2 = *(s32 *)((unsigned char *)var_v0_8 + (4));
            if (var_v1_2 < 0xBB9) {
                *(s32 *)((unsigned char *)var_v0_8 + (4)) = 0;
                var_v0_8 = D_8015F4EC + var_s0_3;
                var_v1_2 = *(s32 *)((unsigned char *)var_v0_8 + (4));
            }
        } else {
            *(s32 *)((unsigned char *)var_v0_7 + (4)) = (s32) (var_v1 + ((s32) (var_v1 * 0x3E8) / var_s4));
            var_v0_8 = D_8015F4EC + var_s0_3;
            var_v1_2 = *(s32 *)((unsigned char *)var_v0_8 + (4));
            if (var_v1_2 >= 0x3A980) {
                *(s32 *)((unsigned char *)var_v0_8 + (4)) = 0;
                var_v0_8 = D_8015F4EC + var_s0_3;
                var_v1_2 = *(s32 *)((unsigned char *)var_v0_8 + (4));
            }
        }
        if (var_v1_2 != 0) {
            func_8013AD64(*var_v0_8, var_v1_2 / 1000, -1, arg1);
        }
        var_s0_3 += 8;
    } while (var_s0_3 != 0x320);
}

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
