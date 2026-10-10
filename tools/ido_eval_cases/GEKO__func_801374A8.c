#include "common.h"
#include "ovl/GEKO.h"

void func_80084D3C();

void func_801374A8(void) {
    if (D_80122CDC != 0) {
        *(s16 *)(D_800E6280 + D_800E71DF * 0x38 + 0x1C2) -= 1;
        *(s16 *)(D_800E6280 + D_800E71DF * 0x38 + 0x1C6) += 5;
        func_80084D3C(0x38);
        D_8014508C = 0;
        func_80137560();
        return;
    }
    D_8014508C = 1;
    D_80145060 += 1;
    func_8004284C();
}
