#include "common.h"
#include "ovl/RPG_BAT.h"

s32 func_80159D50(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E904, "古式「お父様ーーっ！！");
        func_8014B738(D_8015E9CC, 1, 0xAA);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x05000000, 0x01000007, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F220();
        func_8014F258();
        return;
    }
}

void func_80159E28(void) {
    switch (D_8015EE04) {
    case 0:
        func_800AE0F0(D_8015E904, "　　　　　　おやじの怒り　　　　　　");
        func_8014B738(D_8015E9CC, 1, 0x5A);
        func_80144480();
        func_8013EFD0(D_8015EBAC);
        func_801477E0();
        func_8014F230();
        return;
    case 1:
        func_8014F448(1);
        return;
    case 2:
        func_80143C28(0, 0);
        return;
    case 3:
        func_8015A11C();
        return;
    case 4:
        func_80143ECC();
        return;
    case 5:
        func_8013E97C(0x1D, 0x160, 0x68);
        func_8013E810(0x1D, 4, 1, 1);
        func_8014F230();
        return;
    case 6:
        if (D_8015EDC4 & 0x100000) {
            func_80143DA8(0x1D, 0x160, 0x68, 0xA8, 0x68, 0x96);
            return;
        }
        func_80143DA8(0x1D, 0x160, 0x68, 0x90, 0x68, 0x96);
        return;
    case 7:
        func_8013E97C(0x1E, D_8015EC28, D_8015EC2C);
        func_8013E810(0x1E, 5, 1, 1);
        func_8014F230();
        return;
    case 8:
        func_8013F15C(0x512, 0xF, 1);
        func_8014F350(0x37);
        return;
    case 9:
        func_8013E810(0x1E, 0xFF, 1, 0);
        func_8013EA60(0, D_8015EBAC, D_8015EBB0, 1);
        func_8014F230();
        return;
    case 10:
        func_8014F350(0x1E);
        return;
    case 11:
        if (!(D_8015EE0C & 1)) {
            func_8013E810(0x1D, 0xFF, 1, 1);
        } else {
            func_8013E810(0x1D, 0xFF, 1, 0);
        }
        D_8015EE0C += 1;
        if (D_8015EE0C >= 0x3C) {
            func_8013E97C(0x1D, -0x20, 0);
            func_8013E7C0(0x1D, 1, 1, 0);
            func_8014F230();
            return;
        }
        break;
    case 12:
        func_80143F68();
        return;
    case 13:
        func_80143CE8(1, 0);
        return;
    case 14:
        func_8014F350(0x14);
        return;
    case 15:
        func_8014F278();
        break;
    }
}

void func_8015A11C(void) {
    switch (D_8015EE08) {
    case 0:
        func_8013E7C0(0x1D, 1, 1, 1);
        func_8014F258();
        return;
    case 1:
        func_8014F38C(0x14);
        return;
    case 2:
        func_8013E7C0(0x1D, 3, 1, 1);
        func_8014F258();
        return;
    case 3:
        func_80159D50();
        return;
    case 4:
        func_8013E7C0(0x1D, 1, 1, 1);
        func_8014F230();
        return;
    }
}
