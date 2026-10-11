#include "common.h"
#include "ovl/RPG_BAT.h"

void func_801484E0(void) {
    switch (D_8015EDAC) {
    case 0:
        func_800AE0F0(D_8015E74C, "　　　　　レッドカード乱舞　　　　　");
        func_8014B738((s32) &D_8015E814, 1, 0x5A);
        func_80144480();
        func_8013EEAC(D_8015EBAC);
        func_80148F70();
        func_8014EBA8();
        func_8014EBA8();
        return;
    case 2:
        func_8013D418();
        return;
    case 3:
        func_80148654();
        return;
    case 4:
        func_8013E7C0(0x34, 3, 1, 1);
        func_8014EBA8();
        return;
    case 5:
        func_8014ED44(0x20);
        return;
    case 6:
        func_8013E7C0(0x34, 5, 5, 1);
        func_8014EBA8();
        return;
    case 7:
        func_8014EDBC(0x34);
        return;
    case 8:
        func_80148714();
        return;
    case 9:
        func_80148B08();
        return;
    case 10:
        func_8014ED44(0x14);
        return;
    case 11:
        func_8013D59C();
        return;
    case 12:
        func_8014EBEC();
        /* fallthrough */
    default:
        return;
    }
}

void func_80148654(void) {
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013E7C0(0x34, 4, 5, 1);
        func_8013F0F4(0x506, 1, 0);
        func_8014EBD0();
        break;
    case 1:
        if (!(D_80121422 & 1)) {
            func_8013E7C0(0x34, 4, 5, 1);
            D_8015EDB0.unk_08 += 1;
        }
        if (D_8015EDB0.unk_08 >= 0xE) {
            func_8014EBA8();
        }
        break;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801484E0", func_80148714);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801484E0", func_80148B08);
