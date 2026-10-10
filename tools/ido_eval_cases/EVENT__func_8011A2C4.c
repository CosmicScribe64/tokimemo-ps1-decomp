#include "common.h"
#include "ovl/EVENT.h"


void func_8011A2C4(void) {
    if ((D_800B1746 == 0xA) && (D_800EECBC == 1) && (D_8012531C == 1)) {
        func_8004CF30();
        return;
    }
    func_80011DFC();
}
