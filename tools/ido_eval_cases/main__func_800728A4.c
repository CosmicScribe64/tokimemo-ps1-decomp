#include "common.h"
#include "game.h"
#include "main_only.h"

void func_80072944();

void func_800728A4(void) {
    switch (D_800E7389) {                           /* irregular */
    case 0:
        func_80072944();
        break;
    case 1:
        D_800E699C += 1;
        func_80042878(0x31);
        break;
    }
    func_800578F4(get_last_gamen_mode());
    parameter_show();
    message_window_show();
    hizuke_show();
    func_80065B0C(0);
    func_80067870();
    func_80066C08(1);
}
