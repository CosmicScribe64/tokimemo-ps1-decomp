#include "common.h"
#include "ovl/TACO.h"

s32 func_801335A0(a, b)
s32 a;
s32 b;
{
    s32 v;
    u8 *p;

    if (D_8015EDB0 != 0) {
        if (a == 1 && (b & 0xFF) < 0xA) {
            v = (b & 0xFF) & 0xF;
            p = &D_8015E220 + v;
            if (*p != 0) {
                return 0;
            }
            *p = func_800AE0D0() % (u32)(D_8015E22C[v] * 2 + 1);
        }
        v = b & 0xFF;
    } else {
        v = b & 0xFF;
    }
    if (a & 1) {
        func_80044750((v | 0x500) & 0xFFFF);
    } else {
        func_80044750((v | 0x600) & 0xFFFF);
    }
}

void func_80133694(void) {
    s32 i;

    if (D_8015E220 != 0) {
        D_8015E220 -= 1;
    }
    if (D_8015E221 != 0) {
        D_8015E221 -= 1;
    }
    for (i = 0; i < 8; i++) {
        if (D_8015E222[i] != 0) {
            D_8015E222[i] -= 1;
        }
    }
}

/* bulk_m2c declarations (T-11190) */
extern TcPair8 D_8015E238[];
/* end bulk_m2c declarations */
#ifdef NON_MATCHING
/* NON_MATCHING: T-15300 bulk_m2c draft */
void func_80133738(void) {
    s32 var_v1;
    TcPair8 *temp_v0;

    var_v1 = 3;
    if ((D_800E6280.unk_1109 & 1) && (D_800E6280.unk_110A == 1)) {
        if ((D_8015EDBC == 0) && (D_800E6280.unk_F88 & 0x10) && (D_800E6280.unk_10F8 & 0xFF)) {
            if (func_80045288() == 1) {
                    if (func_800460EC() & 2) {
                    if (D_8015EDB4[8].unk78 < D_8015EDB4->unk78) {
                        var_v1 = (func_800AE0D0() & 1) + 2;
                    } else if (D_800E6280.unk_10F4 & 1) {
                        var_v1 = func_800AE0D0() % 6;
                    }
                    temp_v0 = &D_8015E238[var_v1];
                    func_80046290(temp_v0->a, temp_v0->b, 8);
                    func_80044750(0x300);
                }
            }
        }
    }
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801335A0", func_80133738);
#endif
