#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80134D30", func_80134D30);

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
