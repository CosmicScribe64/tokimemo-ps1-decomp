#include "common.h"
#include "ovl/SHOUGATU.h"

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013B210", func_8013B210);

void func_8013B484(void) {
    func_80083808();
    switch (D_800E7389) {
    case 0:
        func_8013B5B0();
        break;
    case 1:
        func_8013BA00();
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

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013B210", func_8013B528);
