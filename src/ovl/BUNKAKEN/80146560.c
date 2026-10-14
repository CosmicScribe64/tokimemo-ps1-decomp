#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_80146560(void) {
    switch (D_80122EB8) {                           /* switch 1 */
    case 0:                                         /* switch 1 */
        switch (D_800E62BE) {                       /* switch 2; irregular */
        case 0x5F:                                  /* switch 2 */
            func_80042908(0x56);
            return;
        case 0x60:                                  /* switch 2 */
            func_80042908(0x57);
            return;
        case 0x61:                                  /* switch 2 */
            func_80042908(0x58);
            return;
        default:                                    /* switch 2 */
            func_80046500();
            return;
        }
        break;
    case 1:                                         /* switch 1 */
        func_80042908(0x59);
        return;
    case 2:                                         /* switch 1 */
        func_80042908(0x5A);
        return;
    case 3:                                         /* switch 1 */
        switch (D_800E62BE) {                       /* switch 3; irregular */
        case 0x5F:                                  /* switch 3 */
            func_80042908(0x5B);
            return;
        case 0x60:                                  /* switch 3 */
            func_80042908(0x5C);
            return;
        case 0x61:                                  /* switch 3 */
            func_80042908(0x5D);
            return;
        }
        break;
    case 4:                                         /* switch 1 */
        switch (D_800E62BE) {                       /* switch 4; irregular */
        case 0x5F:                                  /* switch 4 */
            func_80042908(0x5E);
            return;
        case 0x60:                                  /* switch 4 */
            func_80042908(0x5F);
            return;
        case 0x61:                                  /* switch 4 */
            func_80042908(0x60);
            return;
        }
        break;
    case 5:                                         /* switch 1 */
        func_80042908(0x61);
        return;
    default:                                        /* switch 1 */
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80146560", func_80146700);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80146560", func_801469DC);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80146560", func_80146C8C);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80146560", func_80146F70);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80146560", func_801472C4);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80146560", func_801475CC);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80146560", func_80147914);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80146560", func_80148094);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80146560", func_801483A8);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80146560", func_80148594);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80146560", func_801487DC);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80146560", func_80148A28);

void func_80148E68(void) {
    func_800847B8(6);
    func_8013230C();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
