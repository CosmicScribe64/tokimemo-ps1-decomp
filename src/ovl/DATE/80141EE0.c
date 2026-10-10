#include "common.h"
#include "ovl/DATE.h"

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80141EE0", func_80141EE0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80141EE0", func_801423C4);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80141EE0", func_801428D8);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80141EE0", func_80142DEC);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80141EE0", func_80143300);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80141EE0", func_80143814);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80141EE0", func_80143D28);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80141EE0", func_8014420C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80141EE0", func_80144720);

void func_801448D4(void) {
    func_80083808();
    switch (D_800E7389) {
    case 0:
        func_801449D4();
        break;
    case 1:
        func_80144E20();
        break;
    case 2:
        func_80149584();
        break;
    case 3:
        func_80145D14();
        break;
    default:
        func_80046500();
        break;
    }
    check_k_scroll();
    k_disp_inc2();
    if ((D_800E7389 != 0) || (D_800E738A != 0x45)) {
        func_80066C08(2);
    }
    message_window_show();
    func_80066334();
    hizuke_show();
    func_80083A10();
    func_80047560();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80141EE0", func_801449D4);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80141EE0", func_80144E20);
