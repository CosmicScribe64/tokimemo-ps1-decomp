#include "common.h"
#include "ovl/RPG_BAT.h"

void func_801368B0(void) {
    switch (D_8015EDAC) {
    case 0:
        func_800AE0F0(D_8015E74C, "　　　　　　メガトン辞書　　　　　　");
        func_8014B738(&D_8015E814, 1, 0x5A);
        func_80144480();
        func_8013EEAC(D_8015EBAC);
        func_80148F70();
        func_8014EBA8();
        func_8014EBA8();
        break;
    case 2:
        func_8013D418();
        break;
    case 3:
        func_8013E7C0(0x34, 3, 5, 1);
        func_8014EBA8();
        break;
    case 4:
        func_8014EDBC(0x34);
        if (D_80121438 == 1) {
            func_8013F0F4(0x500, 0, 5);
        }
        if (D_80121438 == 9) {
            func_8013F0F4(0x501, 0, 1);
        }
        if (D_80121438 == 0xB) {
            func_8013F0F4(0x502, 0, 2);
        }
        break;
    case 5:
        func_8013E7C0(0x34, 2, 1, 1);
        func_8014EBA8();
        break;
    case 6:
        func_8014ED44(0x3C);
        break;
    case 7:
        func_80136A9C();
        break;
    case 8:
        func_80136BD8();
        break;
    case 9:
        func_80136CCC();
        break;
    case 10:
        func_8013D3E4();
        func_8014EBA8();
        break;
    case 11:
        func_8014ED44(0x14);
        break;
    case 12:
        func_8013D59C();
        break;
    case 13:
        func_8014EBEC();
        break;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801368B0", func_80136A9C);

void func_80136BD8(void) {
    func_8013F0F4(0x504, 0, 4);
    if (D_8015EDB0.unk_04 < 0x14) {
        if (!(D_8015EDB0.unk_04 & 1)) {
            func_8013F3F0(0, 4);
        } else {
            func_8013F3F0(0, -4);
        }
    }
    if (D_8015EDB0.unk_04 == 0xF) {
        func_8013E97C(0x2A, D_8015EC28, D_8015EC6C + 0x78);
        func_8013E810(0x2A, 5, 5, 1);
    }
    D_8015EDB0.unk_04 += 1;
    if (!(D_8012117A & 1)) {
        func_8013E810(0x2A, 0xFF, 1, 0);
        func_8014EBA8();
    }
}
INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801368B0", func_80136CCC);
