#include "common.h"
#include "ovl/TAIIKU.h"

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80132000", func_80132000);

/* bulk_m2c declarations (T-11190) */
extern u8 D_80148EC0;
/* end bulk_m2c declarations */
void func_80132970(void) {
    u8 *var_s1;
    u8 temp_t3;

    var_s1 = &D_80148EC0;
    do {
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_00 = *var_s1;
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_02 = 0x13;
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_01 = 0;
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = get_g_zyotai_h(*var_s1);
        if ((var_s1 == (u8 *)0x80148EC2) && (D_800E6280.unk_1BC[*var_s1].unk_0A >= 0x4C)) {
            D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 4;
        }
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_04 = D_800E6280.unk_03E;
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_05 = D_800E6280.unk_03F;
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_06 = D_800E6280.unk_040;
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_07 = D_800E6280.unk_041;
        temp_t3 = D_800E6280.unk_F5E + 1;
        D_800E6280.unk_F5E = temp_t3;
        var_s1 += 1;
        if ((u32) (temp_t3 & 0xFF) >= 0xFFU) {
            D_800E6280.unk_F5E = 0xFE;
        }
    } while (var_s1 != (u8 *)0x80148EC3);
}
