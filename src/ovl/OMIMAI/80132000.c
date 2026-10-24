#include "ovl/OMIMAI.h"

void func_80132000(void) {
    func_80083808();
    switch (D_800E6280.stage) {
    case 0:
        func_801322E4();
        break;
    case 1:
        func_80132D44();
        break;
    default:
        func_80046500();
        break;
    }
    check_k_scroll();
    k_disp_inc2();
    message_window_show();
    func_80066334();
    hizuke_show();
    func_80083A10();
}
