#include "common.h"
#include "ovl/GEKO.h"


void func_8013BB4C(void) {
    func_80044750(0xCF);
    D_800E7384 += 1;
    if ((u32) D_800E7384 < 0x40U) {
        return;
    }
    D_800B3D60 = 0;
    func_80044890(0, 0, 0, 0xD989, 0xD949, 0xD941);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}
