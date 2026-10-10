#include "common.h"
#include "ovl/TACO.h"

void func_8014BCEC();
void func_8014BED8();

void func_8014EDCC(s32 arg0) {
    if (arg0 == 1) {
        *((u8 *)D_8015EDB4 + 0xBAE) = 1;
        func_8014BED8();
    } else if ((arg0 >= 2) && (arg0 < 0x32)) {
        if (*((u8 *)D_8015EDB4 + 0xBAE) == 1) {
            func_8014BCEC(0);
        }
    }
}
