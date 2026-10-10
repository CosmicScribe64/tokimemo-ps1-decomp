#include "common.h"
#include "ovl/GEKO.h"

void func_8007C8A4();
void func_8007EC68();

void func_8013BC74(void) {
    s32 sp28;

    sp28 = (s32) D_800E738A;
    func_8007C8A4();
    if (sp28 != D_800E738A) {
        D_800E738A -= 1;
        func_8007EC68();
    }
}
