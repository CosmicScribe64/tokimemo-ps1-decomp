#include "common.h"
#include "ovl/SHOUGATU.h"

void func_80084D3C();

void func_80134730(void) {
    func_800847B8(D_80122CDC);
    *(s16 *)(D_800E6280 + D_80122CDC * 0x38 + 0x1BE) += 5;
    *(s16 *)(D_800E6280 + D_80122CDC * 0x38 + 0x1C2) += 1;
    func_80084D3C(0x38, D_80122CDC);
    D_800E71DF = 0xE;
    func_8004284C();
}
