#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80142000", func_80142000);

void func_801420EC(void) {
    switch (D_8015EE00.unk_04) {
    case 0:
        func_80143C28(2, 0);
        break;
    case 1:
        func_8014F350(0x14);
        break;
    case 2:
        func_8013E7C0(0x1D, 5, 1, 1);
        func_8014F230();
        break;
    case 3:
        func_80142000();
        break;
    case 4:
        func_8013E7C0(0x1D, 0, 1, 1);
        func_8014F350(0x28);
        break;
    case 5:
        func_80143CE8(3, 0);
        break;
    case 6:
        func_8014F350(0x14);
        break;
    case 7:
        func_8014F278();
        break;
    }
}
