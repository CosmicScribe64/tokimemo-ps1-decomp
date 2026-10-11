#include "common.h"
#include "ovl/BUNKASAI.h"

void func_80146030(void) {
    s32 selector;
    BunkSelectWords *work;

    work = (BunkSelectWords *)&D_80122EB8;
    selector = work->unk_00;
    work->unk_04 = 3;
    D_800E6280.unk_1BC[3].unk_0C.f.b2 = 1;
    switch (selector) {
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x51);
            return;
        case 0x60:
            func_80042908(0x52);
            return;
        case 0x61:
            func_80042908(0x53);
            return;
        }
        break;
    case 5:
        func_80042908(0x54);
        return;
    default:
        func_80046500();
        return;
    }
}
INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80146030", func_80146100);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80146030", func_801464A8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80146030", func_801466FC);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80146030", func_80146A78);

void func_80146F38(void) {
    func_800847B8(3);
    func_80132208();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}
