#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80148F70", func_80148F70);

/* bulk_m2c declarations (T-11190) */
extern s16 D_800E638E;
/* end bulk_m2c declarations */
void func_801495DC(void) {
    s32 sp30;
    s32 sp2C;
    s16 var_v1;
    s32 temp_v0;
    s32 var_v1_2;

    if (D_800E638E >= 0x12D) {
        var_v1 = 0x12C;
    } else {
        var_v1 = D_800E638E;
    }
    sp30 = (s32) var_v1;
    sp2C = func_8013EA00(0x64, 0x64) + ((s32) (var_v1 * 9) / 2);
    temp_v0 = func_8013E9A8(0x1000);
    if (temp_v0 == 0) {
        var_v1_2 = 0x270F;
    } else if (temp_v0 < 0x40) {
        var_v1_2 = 0;
    } else if (temp_v0 < 0x10E) {
        var_v1_2 = func_8013EA00(0x1F4, 0x3E8) + sp2C;
    } else {
        var_v1_2 = sp2C;
    }
    if (var_v1_2 >= 0x2710) {
        var_v1_2 = 0x270F;
    }
    D_8015EBB0 = var_v1_2;
    if (D_8015EDC4 & 0x600000) {
        D_8015EBB0 = var_v1_2;
        if (D_8015EBA4 == 0) {
            D_8015EBB0 = var_v1_2;
            if (var_v1_2 != 0) {
                D_8015EDC0 -= 0x5DC;
            }
        }
    }
}
