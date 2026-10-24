/* exit_flow.c: src/ovl/TT/801367C0.c:4 advances private exit substep and timer, sets scene exit flag q[0x55] after timer reaches 31. Original object 0x801367C0-0x80136830 (config/objects/TT.txt). */
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
