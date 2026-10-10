#include "common.h"
#include "game.h"
#include "main_only.h"


void func_8005B798(void) {
    switch (D_800E738A) {                           /* irregular */
    case 0:
        func_8005B830();
        break;
    case 1:
        func_8005B8A0();
        break;
    case 2:
        func_8005B8E0();
        break;
    }
    func_80068EC0();
    parameter_show();
    hizuke_show();
    message_window_show();
    func_80066334();
    func_8006BA40();
}
