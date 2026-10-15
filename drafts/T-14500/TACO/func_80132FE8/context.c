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
    D_8015EDB4->pad0[1] = 0x80;
    D_8015EDB4->unk6C = 0x400;
    D_8015EDB4->unk6E = 0x400;
    D_8015EDB4->unk70 = 0x400;
    func_801438F0(0x8019C800, 1, &D_8015EDB4[8]);
    D_8015EDB4[8].pad0[1] = 0x80;
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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80132220", func_8013240C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80132220", func_801328B0);

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7200;
extern s32 D_800E7208;
extern s32 D_800E7378;
/* end bulk_m2c declarations */
void func_80132FE8(s32 arg0, s32 arg1) {
    s32 sp30;
    TcActor *sp2C;
    s16 temp_v0;
    s16 temp_v0_2;
    s32 var_a0;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a2;
    s32 var_t8;
    s32 var_v1;

    sp2C = D_8015EDB4;
    if (D_8015EDB4->unk2 & 0x40) {
        var_a1 = sp30;
    } else {
        func_8013240C(arg0, arg1);
        func_801328B0();
        func_80140000();
        func_80140DF0();
        var_a1_2 = 0;
        if (D_8015EDD8 != 0) {
            if ((D_800E7200 & 0x20) && (((u8) D_8015EDE0 % 3U) == 0) && !(D_800E7378 & 3)) {
                var_a1_2 = 0x20;
            }
            if (D_800E7200 & 0x40) {
                var_t8 = D_800E7200 & 0x80;
                if (((u8) D_8015EDE0 % 3U) == 1) {
                    var_t8 = D_800E7200 & 0x80;
                    if (!(D_800E7378 & 3)) {
                        var_a1_2 |= 0x20;
                        goto block_11;
                    }
                }
            } else {
block_11:
                var_t8 = D_800E7200 & 0x80;
            }
            if ((var_t8 != 0) && (((u8) D_8015EDE0 % 3U) == 2) && !(D_800E7378 & 3)) {
                var_a1_2 |= 0x20;
            }
            var_v1 = D_800E7208 & 0x80;
            var_a0 = D_800E7208 & 0x40;
            var_a2 = D_800E7208 & 0x20;
        } else {
            var_a2 = D_800E7208 & 0x20;
            var_a0 = D_800E7208 & 0x40;
            if ((var_a2 != 0) && (((u8) D_8015EDE0 % 3U) == 0)) {
                var_a1_2 = 0x20;
            }
            if (var_a0 != 0) {
                var_v1 = D_800E7208 & 0x80;
                if (((u8) D_8015EDE0 % 3U) == 1) {
                    var_a1_2 |= 0x20;
                    goto block_23;
                }
            } else {
block_23:
                var_v1 = D_800E7208 & 0x80;
            }
            if ((var_v1 != 0) && (((u8) D_8015EDE0 % 3U) == 2)) {
                var_a1_2 |= 0x20;
            }
        }
        if ((var_a0 != 0) && (((u8) D_8015EDE4 % 3U) == 0)) {
            var_a1_2 |= 0x40;
        }
        if ((var_v1 != 0) && (((u8) D_8015EDE4 % 3U) == 1)) {
            var_a1_2 |= 0x40;
        }
        if ((var_a2 != 0) && (((u8) D_8015EDE4 % 3U) == 2)) {
            var_a1_2 |= 0x40;
        }
        temp_v0 = sp2C->unk72;
        if (temp_v0 == 0) {
            sp30 = var_a1_2;
            func_80141170(0, var_a1_2);
            var_a1 = var_a1_2;
        } else if (temp_v0 == 1) {
            sp30 = var_a1_2;
            func_80141290(0, var_a1_2);
            var_a1 = var_a1_2;
        } else {
            sp30 = var_a1_2;
            func_80141050(0, var_a1_2);
            var_a1 = var_a1_2;
        }
        temp_v0_2 = sp2C->unk82;
        if (temp_v0_2 == 0) {
            sp30 = var_a1;
            func_801404FC(0, var_a1);
        } else if (temp_v0_2 == 1) {
            sp30 = var_a1;
            func_80140364(0, var_a1);
        }
    }
    func_80141DE0(var_a1);
    func_8013B990();
    func_8013D348();
    func_8013D3F4();
    func_8013DDE4();
    func_8013C6D4();
    func_8013E4FC();
}

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
