#include "common.h"
#include "ovl/BUNKAKEN.h"

#ifdef NON_MATCHING
/* NON_MATCHING: T-10320, only the switch selector register ($v0 vs $v1) differs */
void func_801428D0(void) {
    D_80122EBC = 3;
    switch (D_80122EB8) {
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            if (func_8008667C(3) == 4 && !D_800E6280.unk_1BC[3].unk_0C.f.b2) {
                D_80122EBC = 0xFF;
                func_80042908(5);
                return;
            }
            func_80042908(0x36);
            return;
        case 0x60:
            if (func_8008667C(3) == 4 && !D_800E6280.unk_1BC[3].unk_0C.f.b2) {
                D_80122EBC = 0xFF;
                func_80042908(5);
                return;
            }
            func_80042908(0x37);
            return;
        case 0x61:
            if (func_8008667C(3) == 4 && !D_800E6280.unk_1BC[3].unk_0C.f.b2) {
                D_80122EBC = 0xFF;
                func_80042908(5);
                return;
            }
            func_80042908(0x38);
            return;
        }
        break;
    case 5:
        func_80042908(0x39);
        return;
    default:
        func_80046500();
        return;
    }
}
#else
INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/801428D0", func_801428D0);
#endif

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/801428D0", func_80142A40);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/801428D0", func_80142FD4);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/801428D0", func_80143494);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/801428D0", func_801438F8);

void func_80143D68(void) {
    func_800847B8(3);
    func_801321D4();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
