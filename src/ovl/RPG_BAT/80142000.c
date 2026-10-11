#include "common.h"
#include "ovl/RPG_BAT.h"

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_80142000(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E904, "藤崎「やめてっ！");
        func_8014B738(D_8015E9CC, 1, 0x64);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1F00001E, 0x07000020, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013E7C0(0x1D, 0, 1, 1);
        func_8013F220();
        func_8014F230();
        return;
    }
}

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
