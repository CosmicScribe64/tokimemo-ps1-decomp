#include "common.h"
#include "ovl/TACO.h"

void func_80132220(void) {
    D_8015EDB4->unk74 = 0;
    D_8015EDB4->unk76 = 0;
    D_8015EDB4->unk78 = 0x800;
    D_8015EDB4[8].unk74 = 0;
    D_8015EDB4[8].unk76 = 0;
    D_8015EDB4[8].unk78 = 0x800;
    D_8015EDB4->unk2 = 0;
    D_8015EDB4[8].unk2 = 0;
    D_8015EDB4->unk3 = 0;
    D_8015EDB4->unk68 = 0;
    D_8015EDB4[8].unk68 = 0;
}

void func_801322AC(void) {
    func_801438F0(0x8019C800, 0, D_8015EDB4);
    D_8015EDB4->unk0[1] = 0x80;
    D_8015EDB4->unk6C = 0x400;
    D_8015EDB4->unk6E = 0x400;
    D_8015EDB4->unk70 = 0x400;
    func_801438F0(0x8019C800, 1, &D_8015EDB4[8]);
    D_8015EDB4[8].unk0[1] = 0x80;
    D_8015EDB4[8].unk6C = 0x400;
    D_8015EDB4[8].unk6E = 0x400;
    D_8015EDB4[8].unk70 = 0x400;
    D_8015EDB4->unk7A = 0;
    D_8015EDB4[8].unk7A = 0;
    D_8015EDB4->unk72 = -1;
    D_8015EDB4[8].unk72 = -1;
    D_8015EDB4->unk82 = -1;
    D_8015EDB4[8].unk82 = -1;
    D_8015EDB4->unk7C = 4;
    D_8015EDB4->unk7E = 3;
    D_8015EDB4->unk80 = 0;
    D_8015EDB4->unk84[1] = 0xFF;
    D_8015EDB4[8].unk84[1] = 0x80;
    func_80132220();
    D_8015EDD0 = 0;
    D_8015EDD4 = 0;
}

s16 func_8013240C(void) {
    s32 sp30;
    s32 sp2C;
    s16 temp_v0;
    s16 temp_v0_2;
    s16 temp_v0_3;
    s16 temp_v0_4;
    s32 var_a0;
    s32 var_t2;
    s32 var_t5;
    s32 var_t8;
    u32 temp_hi;
    u32 var_v0;
    u8 temp_t6;
    u8 temp_t7;

    var_a0 = 0x20;
    if (D_800E6280.unk_F80 & 0xA000) {
        var_t8 = D_800E6280.unk_F80 & 0x8000;
        if (D_800E6280.unk_F80 & 0x5000) {
            var_a0 = 0x2D;
            goto block_3;
        }
    } else {
block_3:
        var_t8 = D_800E6280.unk_F80 & 0x8000;
    }
    if (var_t8 != 0) {
        if (D_8015EDB4->unk74 >= 0x801) {
            temp_v0 = D_8015EDB4->unk74;
            D_8015EDB4->unk74 = temp_v0 + ((s32) (var_a0 * (0x1000 - temp_v0)) / 2048);
        } else {
            D_8015EDB4->unk74 += var_a0;
        }
    }
    var_t5 = D_800E6280.unk_F80 & 0x1000;
    if (D_800E6280.unk_F80 & 0x2000) {
        temp_v0_2 = D_8015EDB4->unk74;
        if (temp_v0_2 < -0x800) {
            D_8015EDB4->unk74 = temp_v0_2 + ((s32) (var_a0 * (-0x1000 - temp_v0_2)) / 2048);
        } else {
            D_8015EDB4->unk74 = temp_v0_2 - var_a0;
        }
        var_t5 = D_800E6280.unk_F80 & 0x1000;
    }
    var_t2 = D_800E6280.unk_F80 & 0x4000;
    if (var_t5 != 0) {
        temp_v0_3 = D_8015EDB4->unk76;
        if (temp_v0_3 < -0x700) {
            D_8015EDB4->unk76 = temp_v0_3 + ((s32) (var_a0 * (-0x800 - temp_v0_3)) / 256);
        } else {
            D_8015EDB4->unk76 = temp_v0_3 - var_a0;
        }
        var_t2 = D_800E6280.unk_F80 & 0x4000;
    }
    if (var_t2 != 0) {
        temp_v0_4 = D_8015EDB4->unk76;
        if (temp_v0_4 >= 0x81) {
            D_8015EDB4->unk76 = temp_v0_4 + ((s32) (var_a0 * (0x180 - temp_v0_4)) / 256);
        } else {
            D_8015EDB4->unk76 = temp_v0_4 + var_a0;
        }
    }
    if (D_8015EDB4->unk74 < -0x1000) {
        D_8015EDB4->unk74 = -0x1000;
    }
    if (D_8015EDB4->unk74 >= 0x1001) {
        D_8015EDB4->unk74 = 0x1000;
    }
    if (D_8015EDB4->unk76 < -0x800) {
        D_8015EDB4->unk76 = -0x800;
    }
    if (D_8015EDB4->unk76 >= 0x181) {
        D_8015EDB4->unk76 = 0x180;
    }
    temp_hi = (u8) D_8015EDDC % 3U;
    var_v0 = temp_hi;
    switch (temp_hi) {
    case 0:
        sp30 = 5;
        sp2C = 0xA;
        break;
    case 1:
        sp30 = 4;
        sp2C = 8;
        break;
    case 2:
        sp30 = 1;
        sp2C = 2;
        break;
    }
    if (D_8015EDB4->unk3 == 0) {
        if (D_800E6280.unk_F88 & sp30) {
            D_8015EDB4[8].unk2 = (u8) (D_8015EDB4[8].unk2 + 1);
            D_8015EDB4->unk2 = 0x81;
            D_8015EDB4->unk3 = 0x7F;
        }
        if (D_800E6280.unk_F88 & sp2C) {
            D_8015EDB4[8].unk2 = (u8) (D_8015EDB4[8].unk2 + 1);
            D_8015EDB4->unk2 = 0x82;
            D_8015EDB4->unk3 = 0x7F;
        }
    }
    if (D_8015EDB4->unk2 & 1) {
        var_v0 = (u32) D_8015EDB4->unk3;
        D_8015EDB4->unk68 += var_v0;
        D_8015EDB4->unk74 += 8;
        if (var_v0 < 0xFU) {
            if ((D_8015EDB4->unk68 & 0xFFF) >= 0xFE1) {
                D_8015EDB4->unk3 = 0;
                D_8015EDB4->unk68 = 0;
                D_8015EDB4->unk2 = 0;
            }
        } else {
            temp_t6 = var_v0 - 1;
            D_8015EDB4->unk3 = temp_t6;
            if ((u32) (temp_t6 & 0xFF) < 0x40U) {
                D_8015EDB4->unk2 &= 0x7F;
            }
        }
    }
    if (D_8015EDB4->unk2 & 2) {
        var_v0 = (u32) D_8015EDB4->unk3;
        D_8015EDB4->unk68 -= var_v0;
        D_8015EDB4->unk74 -= 8;
        if (var_v0 < 0xFU) {
            if ((D_8015EDB4->unk68 & 0xFFF) < 0x1F) {
                D_8015EDB4->unk3 = 0;
                D_8015EDB4->unk68 = 0;
                D_8015EDB4->unk2 = 0;
            }
        } else {
            temp_t7 = var_v0 - 1;
            D_8015EDB4->unk3 = temp_t7;
            if ((u32) (temp_t7 & 0xFF) < 0x40U) {
                D_8015EDB4->unk2 &= 0x7F;
            }
        }
    }
    if (D_8015EDB4->unk2 & 4) {
        var_v0 = (u32) D_8015EDB4->unk7A;
        D_8015EDB4->unk7A = var_v0 - 1;
        switch (var_v0) {
        case 2:
        case 10:
        case 14:
        case 15:
            var_v0 = func_801430B0();
            break;
        case 0:
            D_8015EDB4->unk2 &= 0xFB;
            break;
        }
    }
    return (s16) var_v0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80132220", func_801328B0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80132220", func_80132FE8);

void func_80133374(void) {
    if (D_8015EDCC < 3U) {
        D_8015EDEC -= 1;
    }
    if (D_8015EDEC == 0) {
        func_80042908(0x2D);
    }
    if (D_800E6280.unk_F88 & 0x800) {
        func_80042908(0x2D);
    }
    if (D_8015EDB4->unk84[1] == 0) {
        func_80042908(0x2D);
    }
}

void func_80133410(s32 arg0) {
    s32 i;

    if (arg0 == 0) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 16; i++) D_8015E270[i].unk13 &= ~0x20;
    } else {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 16; i++) D_8015E3B0[i].unk13 &= ~0x20;
    }
}

void func_801334BC(void) {
    s32 i;

    D_80160460 = 0;
    D_80160464 = 0; for (i = 0; i < 16; i++) { /* FAKE: store and loop head on one source line, as1 then schedules the store after the pointer setup */
        D_8015E270[i].unk13 = 0;
        D_8015E3B0[i].unk13 = 0;
        D_8015E4F0[i].unk13 = 0;
        D_8015E630[i].unk13 = 0;
        D_8015E770[i].unk13 = 0;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80132220", func_8013356C);
