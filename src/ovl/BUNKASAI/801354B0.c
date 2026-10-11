#include "common.h"
#include "ovl/BUNKASAI.h"

void func_801354B0(void) {
    BunkSelectWords *work = (BunkSelectWords *)&D_80122EB8;
    s32 selector = work->unk_00;
    work->unk_04 = 1;
    D_800E6280.unk_1BC[1].unk_0C.f.b2 = 1;
    switch (selector) {
    case 0:
        if (D_800E6280.unk_03E == 0x61) {
            func_80042908(0x2C);
            return;
        }
        func_80042908(0x2B);
        return;
    case 1:
        func_80042908(0x2D);
        return;
    default:
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/801354B0", func_80135554);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/801354B0", func_80135914);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/801354B0", func_80135C4C);

void func_80136230(void) {
    func_800847B8(1);
    func_801320D0();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}
