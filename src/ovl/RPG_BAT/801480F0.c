#include "common.h"
#include "ovl/RPG_BAT.h"

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_801480F0(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E904, "紐緒「これでもくらいなさい！");
        func_8014B738(D_8015E9CC, 1, 0xAA);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x12, 0x04000019, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F220();
        func_8014F230();
        return;
    }
}

void func_801481C8(void) {
    switch (D_8015EE04) {
    case 0:
        func_80143C28(9, 0);
        return;
    case 1:
        func_8013E7C0(0x1D, 3, 1, 1);
        func_8013E97C(0x1E, 0xD8, 0x88);
        func_8013E810(0x1E, 6, 5, 1);
        func_80144480();
        func_8013EFD0(D_8015EBAC);
        func_801477E0();
        func_8013F15C(0x50E, 1, 0);
        func_8014F230();
        return;
    case 2:
        func_8014F3C8(0x1E);
        return;
    case 3:
        func_8013E810(0x1E, 7, 1, 1);
        func_8014F230();
        return;
    case 4:
        func_801480F0();
        return;
    case 5:
        func_8013E7C0(0x1D, 4, 5, 1);
        func_8014F230();
        return;
    case 6:
        func_8014F3C8(0x1D);
        return;
    case 7:
        func_8013E7C0(0x1D, 5, 1, 1);
        func_8013F15C(0x50F, 1, 0);
        func_8014F230();
        return;
    case 8:
        if (D_8015EBAC == 0) {
            func_80143DA8(0x1E, 0xD8, 0x88, D_8015EC28, D_8015EC2C, 0x3C);
            return;
        }
        func_80143DA8(0x1E, 0xD8, 0x88, D_8015EC28, D_8015EC2C, 0x28);
        return;
    case 9:
        func_8014F350(0x1E);
        return;
    case 10:
        if (D_8015EDC4 & 0x100000) {
            func_8013E97C(0x1E, D_8015EC28, D_8015EC2C - 0x30);
        } else {
            func_8013E97C(0x1E, D_8015EC28 - 4, 0x5E);
        }
        func_8013E810(0x1E, 8, 5, 1);
        func_8013F15C(0x510, 1, 0);
        func_8014F230();
        return;
    case 11:
        func_8014F3C8(0x1E);
        return;
    case 12:
        func_8013E7C0(0x1E, 0xFF, 1, 0);
        func_8013EA60(0, D_8015EBAC, D_8015EBB0, 1);
        func_8014F230();
        return;
    case 13:
        func_8014F350(0x3C);
        return;
    case 14:
        func_80143CE8(1, 0);
        return;
    case 15:
        func_8014F350(0x14);
        return;
    case 16:
        func_8014F278();
        /* fallthrough */
    default:
        return;
    }
}
