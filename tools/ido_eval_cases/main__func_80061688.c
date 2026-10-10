#include "common.h"
#include "game.h"
#include "main_only.h"


void func_80061688(void) {
    switch (D_800E7389) {                           /* irregular */
    case 0:
        pre_syogatu_init();
        break;
    case 1:
        func_80061634();
        break;
    }
    hizuke_show();
    message_window_show();
    func_8006BA40();
    func_80066334();
    func_800578F4(1);
    func_80066C08(1);
}
