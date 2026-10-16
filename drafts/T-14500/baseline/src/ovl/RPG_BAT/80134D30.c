#include "common.h"
#include "ovl/RPG_BAT.h"

void func_80134D30(void) {
    switch (D_8015EDAC) {
    case 0:
        func_800AE0F0(D_8015E74C, "　　　　ハーメルンのホラ吹き　　　　");
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
        func_801359FC();
        return;
    case 4:
        func_8013D1D0();
        return;
    case 5:
        func_80134E78();
        return;
    case 6:
        func_80135358();
        return;
    case 7:
        func_8013D350();
        func_8013E7C0(0x34, 2, 1, 0);
        func_8014EBA8();
        return;
    case 8:
        func_8013D290();
        return;
    case 9:
        func_8013D59C();
        return;
    case 10:
        func_8014EBEC();
        /* fallthrough */
    default:
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80134D30", func_80134E78);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80134D30", func_80135358);

void func_801359FC(void) {
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013E7C0(0x34, 3, 5, 1);
        func_8014EBD0();
        break;
    case 1:
        func_8014ED80(0xF);
        break;
    case 2:
        func_8014EBD0();
        break;
    case 3:
        func_8014EDFC(0x34);
        break;
    case 4:
        func_8013E7C0(0x34, 7, 1, 1);
        func_8013F0F4(0x50D, 1, 0);
        func_8014EBD0();
        break;
    case 5:
        func_8014ED80(0x4B);
        break;
    case 6:
        func_8014EBA8();
        break;
    }
}
