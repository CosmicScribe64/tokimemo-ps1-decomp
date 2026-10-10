#include "common.h"
#include "game.h"
#include "main_only.h"


void uwasa_main(void) {
    switch (D_800E7389) {                           /* irregular */
    case 0:
        func_80060B24();
        break;
    case 1:
        uwasa0();
        break;
    }
    func_80065B0C(1);
    parameter_show();
    hizuke_show();
    message_window_show();
    func_80066334();
    func_800578F4(0);
    func_80066C08(1);
}
