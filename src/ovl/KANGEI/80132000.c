#include "common.h"
#include "ovl/KANGEI.h"

void func_80132000(void) {
    func_80083808();
    if (D_800E6280.unk_1109 == 0) {
        func_80132214();
    } else {
        func_80046500();
    }
    check_k_scroll();
    k_disp_inc2();
    func_80066C08(2);
    message_window_show();
    func_80066334();
    hizuke_show();
    func_80083A10();
}
