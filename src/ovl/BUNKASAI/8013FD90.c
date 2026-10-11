#include "common.h"
#include "ovl/BUNKASAI.h"

void func_8013FD90(void) {
    /* FAKE: D_80122EBC reached through D_80122EB8 keeps the selector load behind this store (as1 does not
     * move a load over a store through the same symbol); the real source has one object. T-9140 */
    (&D_80122EB8)[1] = 0xA;
    switch (D_80122EB8) {
    case 0:
        if (D_800E6280.unk_03E == 0x60) {
            func_80042908(0x9B);
            return;
        }
        func_80042908(0x9C);
        return;
    case 1:
        func_80042908(0x9D);
        return;
    case 2:
        func_80042908(0x9E);
        return;
    case 3:
        if (D_800E6280.unk_03E == 0x60) {
            func_80042908(0x9F);
            return;
        }
        func_80042908(0x9E);
        return;
    case 4:
        if (D_800E6280.unk_03E == 0x60) {
            func_80042908(0xA0);
            return;
        }
        func_80042908(0xA1);
        return;
    case 5:
        func_80042908(0xA2);
        return;
    default:
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8013FD90", func_8013FEB4);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8013FD90", func_801401E8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8013FD90", func_80140558);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8013FD90", func_80140958);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8013FD90", func_80140F4C);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8013FD90", func_801417DC);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8013FD90", func_80141F70);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8013FD90", func_80142298);

void func_80142694(void) {
    func_800847B8(10);
    func_80132374();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}
