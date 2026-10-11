#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801563A0", func_801563A0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801563A0", func_801564F0);

void func_8015675C(void) {
    switch (D_8015EE04) {
    case 0:
        func_8013E7C0(0x1D, 5, 5, 1);
        func_8014F230();
        break;
    case 1:
        if (D_80120E1C == 2) {
            func_8013F15C(0x513, 0, 1);
        }
        func_8014F3C8(0x1D);
        break;
    case 2:
        func_80143C28(6, 0);
        break;
    case 3:
        func_801563A0();
        break;
    case 4:
        func_801564F0();
        break;
    case 5:
        func_8014F350(0x14);
        break;
    case 6:
        func_80143CE8(3, 0);
        break;
    case 7:
        func_8014F350(0x14);
        break;
    case 8:
        func_8014F278();
        break;
    }
}
