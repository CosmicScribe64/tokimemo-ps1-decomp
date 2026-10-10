#include "common.h"
#include "ovl/GYOZI.h"

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801372B0", func_801372B0);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801372B0", func_80137524);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801372B0", func_80137798);

void func_80137854(void) {
    u32 temp_t8;

    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    temp_t8 = (u8)func_8005E0E0(5) & 0x7F;
    if ((D_800F62CF == 5) && (temp_t8 < 2U)) {
        if (D_800F6474++ == 0) {
            D_80145EB4 = 0x2A;
        }
        func_801343A4();
        return;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801372B0", func_801378D0);

void func_80137A0C(void) {
    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    if ((u32) ((u8)func_8005E0E0(D_800F62CF) & 0x7F) >= 2U) {
        D_80145EB4 += 2;
    }
    if (D_800F53DE == 0x62) {
        D_80145EB4 += 1;
    }
    func_8004DE1C();
}

void func_80137A80(void) {
    func_8008A0D4(0x418C);
    func_8004DE1C();
}

void func_80137AA8(void) {
    if (D_80145F60 == 0) {
        D_800F53A0.girl[D_800F62CF].unk_0A += 1;
    }
    func_8008FB00();
    func_80050D60(0, 0);
    func_80081D30(1);
}

void func_80137B18(void) {
    func_80051DD8(0x13, 0x801B0000, 0xA3C8);
    func_801372B0();
    func_8004DE1C();
}

void func_80137B50(void) {
    D_8012E66C = D_80146364;
    func_8004DE1C();
}

void func_80137B7C(void) {
    D_80146364 = (u8) D_8012E66C;
    D_800D9248 = 0;
    D_800D924C = 0;
    func_801372B0();
    D_800D9258 = D_80146224;
    D_800D925C = D_80146258;
    D_800D9260 = D_8014628C;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801372B0", func_80137BEC);

void func_80137CAC(void) {
    u32 temp_t6;

    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    temp_t6 = (u8)func_8005E0E0(D_800F62CF) & 0x7F;
    if (temp_t6 < 2U) {
        D_800D9248 = 6;
    } else if (temp_t6 == 2) {
        D_800D9248 = 8;
    } else if (temp_t6 == 3) {
        D_800D9248 = 0xA;
    } else {
        D_800D9248 = 0xC;
    }
    func_8004DE1C();
}

void func_80137D3C(void) {
    D_800D9248 = (D_8012E66C * 2) + 0xD;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801372B0", func_80137D70);

void func_80137E24(void) {
    D_800D9248 = 0x1D;
    D_800D9258 = D_80146358;
    D_800D925C = D_8014635C;
    D_800D9260 = D_80146360;
    func_80137E84();
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801372B0", func_80137E84);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801372B0", func_80137F48);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801372B0", func_80137FEC);

void func_801381A4(void) {
    func_80051DD8(3, 0x801B0000, 0xA3FB);
    func_80137524();
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801372B0", func_801381DC);

void func_80138264(void) {
    D_800D9248 = 1;
    D_800D924C = 0;
    func_8013829C();
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801372B0", func_8013829C);
