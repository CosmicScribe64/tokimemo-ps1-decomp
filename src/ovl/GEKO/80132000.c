#include "common.h"
#include "ovl/GEKO.h"

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80132000", func_80132000);

void func_80132244(void) {
    func_80083808();
    switch (D_800E7389) {                           /* irregular */
    case 0:
        func_801323E0();
        break;
    case 1:
        func_801338F0();
        break;
    default:
        func_80046500();
        break;
    }
    check_k_scroll();
    k_disp_inc2();
    func_80066C08(2);
    message_window_show();
    func_80066334();
    hizuke_show();
    func_80083A10();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80132000", func_801322E8);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80132000", func_80132364);
