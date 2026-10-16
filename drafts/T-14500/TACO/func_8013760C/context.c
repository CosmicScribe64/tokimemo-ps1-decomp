#include "common.h"
#include "ovl/TACO.h"

s32 func_80137590(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013760C();
        break;
    case 1:
        func_8013788C();
        break;
    }
    if (D_800E6280.unk_10F8 & 0x10) {
        func_801432F0("DEMO PLAY", -0x40, 0, 2);
    }
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7378;
extern s8 D_800E62B6;
extern s8 D_800E62B7;
extern s8 D_800E62B8;
extern u8 D_800E738D;
/* end bulk_m2c declarations */
void func_8013760C(void) {
    s32 temp_t9;
    s32 var_s0;

    switch (D_800E738D) {
    case 0:
        func_8013AFDC();
        D_8015F4F0 = 0x80;
        D_8015EDCC = 0;
        D_8015EDF0 = 0;
        D_8015EDB0 = 0x80;
        D_8015EDEC = 0x4650;
        func_80134884();
        D_8015EDBC = 0x10;
        D_800E62B6 = 0;
        D_800E62B7 = 0;
        D_800E62B8 = 0;
        func_800591D8(1);
        k_reset(0);
        back_clear_switch(1);
        func_80143A74();
        func_8014394C();
        func_80059048();
        func_801347A8();
        func_80143AF4();
        var_s0 = 0;
        do {
            func_80099E30(0, &D_8015EDB4->pad0[var_s0] + 0x14);
            func_801438F0(0x801AC000, 0, (s32) &D_8015EDB4->pad0[var_s0]);
            var_s0 += 0x88;
        } while (var_s0 != 0x29F8);
        func_801322AC();
        func_80132220();
        func_801334BC();
        D_8011ED17 &= 0x7F;
        D_800E738D += 1;
        break;
    case 1:
        if (D_8015EDBC == 0) {
            func_8015ABF0();
            D_8015EDB0 = 0x80;
            D_8015EDB4->unk84[1] = 0xFF;
            *(s8 *)((unsigned char *)D_8015EDB4 + (1221)) = 0xFF;
            temp_t9 = D_800E7378 & 3;
            switch (temp_t9) {
            case 0:
                D_8015EDB4->unk72 = 0;
                break;
            case 1:
                D_8015EDB4->unk72 = 1;
                break;
            }
            load_csr_tp(0x80162000, 0xE, 1, 0, 0, 0x100, 0x100);
            load_csr_tp(0x80172000, 0xC, 1, 0, 0, 0x100, 0x100);
            load_csr_tp(0x80182000, 0xA, 1, 0, 0, 0x80, 0x80);
            func_8004284C();
        }
        break;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80137590", func_8013788C);
