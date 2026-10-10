#include "common.h"
#include "ovl/GEKO.h"

void func_80134C90(void) {
    func_80083808();
    if (D_800E6280.unk_1109 == 0) {
        func_801351F8();
    } else {
        func_80046500();
    }
    check_k_scroll();
    k_disp_inc2();
    message_window_show();
    func_80066334();
    hizuke_show();
}
