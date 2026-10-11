#include "common.h"
#include "ovl/BUNKASAI.h"

void func_8013B190(void) {
    BunkModePair *state;

    state = (BunkModePair *)&D_80122EB8;
    state->unk_04 = 2;
    D_800E6280.unk_1BC[2].unk_0C.f.b2 = 1;
    switch (state->unk_00) {
    case 2:
        func_80042908(0x3C);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x3D);
            return;
        case 0x60:
            func_80042908(0x3F);
            return;
        case 0x61:
            func_80042908(0x40);
            return;
        }
        break;
    default:
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8013B190", func_8013B25C);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8013B190", func_8013B688);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8013B190", func_8013BB94);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/8013B190", func_8013C0AC);

void func_8013C3C4(void) {
    func_800847B8(2);
    func_80132138();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}
