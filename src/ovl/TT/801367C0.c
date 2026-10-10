#include "common.h"
#include "ovl/TT.h"

void func_801367C0(void) {
    u8 *q = D_80158A60;
    u8 *p = D_80158A64;

    switch (*(u16 *) (p + 2)) {
    case 0:
        *(u16 *) (p + 0x26) = 0;
        *(u16 *) (p + 2) = 0x40;
        break;
    case 0x40:
        *(u16 *) (p + 0x26) += 1;
        if (*(u16 *) (p + 0x26) >= 0x1FU) {
            q[0x55] = 1;
        }
        break;
    }
}
