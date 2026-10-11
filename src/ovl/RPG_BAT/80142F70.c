#include "common.h"
#include "ovl/RPG_BAT.h"

void func_80142F70(void) {
    func_80043914(D_8015EED4, 0x10, 1, 4, 0);
    func_80043914(D_8015EEE4, 0x1E, 1, 4, 0);
    func_80043914(D_8015EEE4, 0x1F, 1, 4, 0xF);
    switch (D_8015EB98) {                           /* switch 1; irregular */
    case 0x10000:                                   /* switch 1 */
        func_80043914(D_8015EED4, 0x11, 1, 1, 1);
        func_80043914(D_8015EED4, 0x17, 1, 3, 1);
        func_80043914(D_8015EEDC, 0x18, 1, 4, 0);
        func_80043914(D_8015EED8, 0x13, 1, 3, 0);
        func_80043914(D_8015EED8, 0x12, 1, 4, 0);
        break;
    case 0x20000:                                   /* switch 1 */
        func_80043914(D_8015EED4, 0x12, 1, 1, 2);
        func_80043914(D_8015EED4, 0x17, 1, 3, 2);
        func_80043914(D_8015EEDC, 0x18, 1, 4, 1);
        func_80043914(D_8015EED8, 0x13, 1, 4, 1);
        break;
    case 0x40000:                                   /* switch 1 */
        func_80043914(D_8015EED4, 0x13, 1, 1, 3);
        func_80043914(D_8015EED4, 0x17, 1, 3, 3);
        func_80043914(D_8015EEDC, 0x18, 1, 4, 2);
        func_80043914(D_8015EED8, 0x12, 1, 4, 2);
        break;
    case 0x80000000:                                /* switch 1 */
        func_80043914(D_8015EED4, 0x16, 1, 1, 6);
        func_80043914(D_8015EED4, 0x17, 1, 3, 6);
        func_80043914(D_8015EED8, 0x12, 1, 4, 5);
        break;
    case 0x8000000:                                 /* switch 1 */
        func_80043914(D_8015EED4, 0x11, 1, 1, 1);
        func_80043914(D_8015EED4, 0x17, 1, 3, 1);
        func_80043914(D_8015EED8, 0x12, 1, 4, 4);
        break;
    case 0x100000:                                  /* switch 1 */
        func_80043914(D_8015EED4, 0x15, 1, 1, 5);
        func_80043914(D_8015EED4, 0x14, 1, 1, 4);
        func_80043914(D_8015EED4, 0x17, 1, 3, 4);
        func_80043914(D_8015EED8, 0x12, 1, 4, 3);
        func_80043914(D_8015EEEC, 0x13, 1, 1, 0);
        break;
    }
    if (D_8015EBA4 == 1) {
        switch (D_8015EBA0) {                       /* switch 2 */
        case 0:                                     /* switch 2 */
            func_80043914(D_8015EEE4, 0x1D, 1, 4, 6);
            break;
        case 1:                                     /* switch 2 */
            func_80043914(D_8015EEE4, 0x1D, 1, 4, 7);
            break;
        case 2:                                     /* switch 2 */
            func_80043914(D_8015EEE4, 0x1D, 1, 4, 9);
            break;
        case 3:                                     /* switch 2 */
            func_80043914(D_8015EEE4, 0x1D, 1, 4, 8);
            break;
        case 4:                                     /* switch 2 */
            func_80043914(D_8015EEE4, 0x1D, 1, 4, 0xA);
            break;
        case 5:                                     /* switch 2 */
            func_80043914(D_8015EEE4, 0x1D, 1, 4, 0xB);
            break;
        case 6:                                     /* switch 2 */
            func_80043914(D_8015EEE4, 0x1D, 1, 4, 1);
            break;
        case 7:                                     /* switch 2 */
            func_80043914(D_8015EEE4, 0x1D, 1, 4, 2);
            break;
        case 8:                                     /* switch 2 */
            func_80043914(D_8015EEE4, 0x1D, 1, 4, 4);
            break;
        case 9:                                     /* switch 2 */
            func_80043914(D_8015EEE4, 0x1D, 1, 4, 5);
            break;
        case 10:                                    /* switch 2 */
            func_80043914(D_8015EEE4, 0x1D, 1, 4, 3);
            break;
        case 11:                                    /* switch 2 */
            func_80043914(D_8015EEE4, 0x1D, 1, 4, 0xC);
            break;
        }
    }
    switch (D_8015EB9C) {                           /* switch 3 */
    case 0:                                         /* switch 3 */
        func_80043914(D_8015EEE0, 0x1C, 1, 4, 0);
        break;
    case 1:                                         /* switch 3 */
        func_80043914(D_8015EEE0, 0x1C, 1, 4, 1);
        break;
    case 2:                                         /* switch 3 */
        if (D_8015EB98 != 0x80000000) {
            func_80043914(D_8015EEE0, 0x1C, 1, 4, 2);
        } else {
            func_80043914(D_8015EEE0, 0x1C, 1, 4, 3);
        }
        break;
    case 3:                                         /* switch 3 */
        func_80043914(D_8015EEE0, 0x1C, 1, 4, 3);
        break;
    case 4:                                         /* switch 3 */
        func_80043914(D_8015EEE0, 0x1C, 1, 4, 4);
        break;
    case 5:                                         /* switch 3 */
        func_80043914(D_8015EEE0, 0x1C, 1, 4, 5);
        break;
    case 6:                                         /* switch 3 */
        func_80043914(D_8015EEE0, 0x1C, 1, 4, 6);
        break;
    case 7:                                         /* switch 3 */
        func_80043914(D_8015EEE0, 0x1C, 1, 4, 7);
        break;
    case 8:                                         /* switch 3 */
        func_80043914(D_8015EEE0, 0x1C, 1, 4, 8);
        break;
    case 9:                                         /* switch 3 */
        func_80043914(D_8015EEE0, 0x1C, 1, 4, 9);
        break;
    case 10:                                        /* switch 3 */
        func_80043914(D_8015EEE0, 0x1C, 1, 4, 0xA);
        break;
    case 13:                                        /* switch 3 */
        func_80043914(D_8015EEE0, 0x1C, 1, 4, 0xD);
        break;
    case 14:
        break;
    }
    func_80042808();
}
