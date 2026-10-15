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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80137590", func_8013760C);

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7200;
extern s32 D_800E7208;
extern u32 D_800E7380;
/* end bulk_m2c declarations */
void func_8013788C(void) {
    s32 temp_t7;
    s32 var_at;
    s32 var_v1;
    u32 temp_t8;

    func_80154960();
    if (D_800E7208 != 0) {
        func_80143AF4();
        func_801347A8();
        func_80059048();
        func_8015ABF0();
        func_800591D8(0);
        func_80042908(2);
    } else {
        D_800E7208 |= 0x20;
        temp_t8 = D_800E7380 & 0x1FF;
        temp_t7 = D_800E7200 | 0x20;
        var_v1 = temp_t7;
        if (temp_t8 >= 0x79U) {
            var_at = temp_t8 < 0xABU;
            if (temp_t8 < 0x8CU) {
                var_v1 = temp_t7 | 0x2000;
                goto block_5;
            }
        } else {
block_5:
            var_at = temp_t8 < 0xABU;
        }
        if ((var_at == 0) && (temp_t8 < 0xBEU)) {
            var_v1 |= 0x4000;
        }
        if ((temp_t8 >= 0xDDU) && (temp_t8 < 0xF0U)) {
            var_v1 |= 0x8000;
        }
        if ((temp_t8 >= 0x10FU) && (temp_t8 < 0x122U)) {
            var_v1 |= 0x1000;
        }
        if ((temp_t8 >= 0x191U) && (temp_t8 < 0x1B8U)) {
            var_v1 |= 0x3000;
        }
        if ((temp_t8 >= 0x1CDU) && (temp_t8 < 0x1E0U)) {
            var_v1 |= 0xC000;
        }
        if ((temp_t8 >= 0x191U) && (temp_t8 < 0x1B8U)) {
            var_v1 |= 0x6000;
        }
        D_800E7200 = var_v1;
        if (temp_t8 >= 0x1CDU) {
            D_800E7200 = var_v1;
            if (temp_t8 < 0x1E0U) {
                D_800E7200 = var_v1 | 0x9000;
            }
        }
        func_8013A790(1, 1);
        func_8013F250();
        func_80132FE8();
    }
    if ((u32) D_800E7380 >= 0x709U) {
        func_80143AF4();
        func_801347A8();
        func_80059048();
        func_8015ABF0();
        func_800591D8(0);
        func_80042908(2);
    }
}
