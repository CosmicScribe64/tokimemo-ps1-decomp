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

/* The table is read as flat word pairs: `&D_8015E238[idx * 2]` takes the ugen temporaries of the
 * original; a pair-struct element (`&pairs[idx]`) gets them one register lower (T-16561). */
void func_80133738(void) {
    s32 *p;
    s32 idx;

    idx = 3;
    if ((D_800E6280.unk_1109 & 1) && (D_800E6280.unk_110A == 1)) {
        if ((D_8015EDBC == 0) && (D_800E6280.unk_F88 & 0x10) && (D_800E6280.unk_10F8 & 0xFF)) {
            if (func_80045288() == 1) {
                if (func_800460EC() & 2) {
                    if (D_8015EDB4[8].unk78 < D_8015EDB4->unk78) {
                        idx = (func_800AE0D0() & 1) + 2;
                    } else if (D_800E6280.unk_10F4 & 1) {
                        idx = func_800AE0D0() % 6;
                    }
                    p = &D_8015E238[idx * 2];
                    func_80046290(p[0], p[1], 8);
                    func_80044750(0x300);
                }
            }
        }
    }
}
