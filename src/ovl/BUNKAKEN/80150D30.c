#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_80150D30(void) {
    switch (D_80122EB8) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        switch (D_800E62BE) {                       /* switch 2; irregular */
        case 0x60:                                  /* switch 2 */
            func_80042908(0x7B);
            return;
        case 0x61:                                  /* switch 2 */
            func_80042908(0x7C);
            return;
        default:                                    /* switch 2 */
            func_80046500();
            return;
        }
        break;
    case 1:                                         /* switch 1 */
        func_80042908(0x7D);
        return;
    case 2:                                         /* switch 1 */
        func_80042908(0x7E);
        return;
    case 3:                                         /* switch 1 */
        switch (D_800E62BE) {                       /* switch 3; irregular */
        case 0x60:                                  /* switch 3 */
            func_80042908(0x7F);
            return;
        case 0x61:                                  /* switch 3 */
            func_80042908(0x7E);
            return;
        }
        break;
    case 4:                                         /* switch 1 */
        switch (D_800E62BE) {                       /* switch 4; irregular */
        case 0x60:                                  /* switch 4 */
            func_80042908(0x80);
            return;
        case 0x61:                                  /* switch 4 */
            func_80042908(0x81);
            return;
        }
        break;
    case 5:                                         /* switch 1 */
        func_80042908(0x82);
        return;
    default:                                        /* switch 1 */
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80150D30", func_80150E88);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80150D30", func_80151104);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80150D30", func_801513E8);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80150D30", func_80151738);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80150D30", func_80151A78);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80150D30", func_80151EC0);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80150D30", func_801520B4);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80150D30", func_80152300);

void func_80152618(void) {
    func_800847B8(9);
    func_80132444();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
