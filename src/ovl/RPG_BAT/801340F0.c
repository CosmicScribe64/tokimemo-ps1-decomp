#include "common.h"
#include "ovl/RPG_BAT.h"

void func_801340F0(void) {
    switch (D_8015EDF0) {
    case 1:
        func_8013415C();
        return;
    case 2:
        func_80134570();
        return;
    case 3:
        func_80134984();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801340F0", func_8013415C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801340F0", func_80134570);

void func_80134984(void) {
    switch (D_8015EE04) {
    case 0:
        func_800AE0F0(D_8015E904, "　　　　　　好雄の秘薬　　　　　　");
        func_8014B738((s32) D_8015E9CC, 1, 0x3C);
        func_8014F230();
        return;
    case 1:
        func_8014F448(1);
        return;
    case 2:
        func_8013EA60(2, 0x23, 0x270F, 0);
        D_8015EDEC = D_8015EDF8;
        func_8013F15C(0x502, 1, 0);
        func_8014F230();
        return;
    case 3:
        func_8014F350(0x1E);
        return;
    case 4:
        func_8014F278();
        /* fallthrough */
    default:
        return;
    }
}
