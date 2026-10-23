#define MAIN_API_OVERRIDE_func_800AE0B0 /* FAKE: integer result keeps the retry flag in v1. T-25038 */
#define MAIN_API_OVERRIDE_func_800847B8 /* FAKE: integer return view keeps E50C's loop counter in v1. T-25033 */
#include "common.h"
#include "ovl/SHOUGATU.h"

s32 func_800847B8();
s32 func_800AE0B0(void *arg0, ...);

/* Existing exception: EVENT owns a table at this overlapping address; moving
 * this scalar to main_api.h needs its override first. T-11590, T-25038 */
extern s32 D_80122D48;

/* T-9740: integer definition without an explicit return matches the original. */
s32 func_8013E330(void) {
    s32 idx;
    ShoCalendarFn28 table;

    table = D_8014605C;
    idx = D_800E6280.unk_110A;
    table.f[idx](0x80);
    ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = D_800B593C;
    if (((DisplayActor44 *)D_8011ECD0)[96].unk_18 == 0 && ((DisplayActor44 *)D_8011ECD0)[96].unk_08 == 0) {
        ((DisplayActor44 *)D_8011ECD0)[96].unk_16 += 1;
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_16 >= 3) {
            ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 0;
        }
    }
}

void func_8013E410(void) {
    u8 kind;

    kind = func_80051A68(0) & 0x7F;
    if ((D_80146050 != 0) || (kind == 4)) {
        func_8013D2A8();
        return;
    }
    func_8004284C();
}

void func_8013E464(void) {
    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    if (((u8)func_80051A68(D_800E6280.unk_F5F) & 0x7F) != 4) {
        func_8004284C();
        return;
    }
    func_8013D2A8();
}

void func_8013E4B4(void) {
    if (D_80146050 != 0) {
        if (D_80145F2C == 1) {
            D_80145F2C += 1;
        } else {
            D_80145F2C += 2;
        }
    }
    func_8004284C();
}

void func_8013E50C(void) {
    s16 i;
    s32 girl;
    ShoShorts7 sp40;
    ShoShorts7 sp30;

    sp40 = D_801460CC;
    sp30 = D_801460DC;
    /* FAKE: multiply by zero keeps this byte promoted across the conditional;
     * literal zero loses the entry a2 carrier and its later reload. T-25033 */
    girl = D_800E6280.unk_F5F * 0;
    D_800E6280.unk_F5F = girl;
    D_80146050 = 0;
    if (D_800E6280.unk_1BC[9].unk_0C.f.b1 == 0 &&
        D_800E6280.unk_1BC[9].unk_02 >= 20 && (D_800E6280.unk_10F8 & 1)) {
        D_80146050 = 1;
    }
    /* FAKE: narrowing spends the target's expression temporaries; the byte
     * argument view keeps the later index-reload base in a2. T-25033 */
    D_80145F2C = (((u8)func_80051A68(*(u8 *)&D_800E6280.unk_F5F) & 0x7F) == 4) * 4;
    D_80145F20 = D_80145AD4;
    D_80145F24 = D_80145B08;
    D_80145F28 = D_80145B3C;
    func_800847B8(D_800E6280.unk_F5F);
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 4;
    for (i = 0; i < 7; i++) {
        if (D_800E6280.unk_114.unk_02 < sp40.v[i]) {
            break;
        }
    }
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_02 += sp30.v[i];
    check_para_limit();
    func_80085B3C(4, 0);
    func_8004284C();
}

void func_8013E6E4(void) {
    s16 i;
    s16 count;
    s32 status;
    ShoShorts7 sp50;
    ShoShorts7 sp40;

    sp50 = D_801460EC;
    sp40 = D_801460FC;
    count = 0;
    for (i = 0; i < 11; i++) {
        count += D_800E6280.unk_1BC[i].unk_0C.f.b1 >= 1;
    }
    i = 0;
    do {
        /* FAKE: multiplication by one preserves the original sum operand order. T-25038 */
        D_800E6280.unk_F5F = (D_800E6280.unk_10F8 * 1 + i) % 10U + 1;
        func_800AE0B0("num%d girl%d\n", count, D_800E6280.unk_F5F);
        i++;
        if (!D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1 && D_800E6280.unk_F5F >= 9) {
            continue;
        }
        if (D_800E6280.unk_F5F == 5 && (D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) == 8) {
            continue;
        }
        /* FAKE: a separate byte view preserves the second selector reload. T-25038 */
        if (*(u8 *)&D_800E6280.unk_F5F == 6 && (D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) == 9) {
            continue;
        }
        if (!D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1 && count >= 2) {
            continue;
        }
        break;
    } while (1);
    if (D_80146050 != 0) {
        D_800E6280.unk_F5F = 9;
    }
    D_80146054 = D_800E6280.unk_F5F;
    D_80145F2C = 0;
    /* FAKE: narrowing spends the original temporary slots; the byte argument
     * view prevents selector promotion through the club filters. T-25038 */
    status = (u8)func_80051A68(*(u8 *)&D_800E6280.unk_F5F);
    if (D_800E6280.unk_F5F == 6 && (status & 0x7F) >= 2U) {
        D_80145F2C++;
    }
    if ((status & 0x7F) == 4) {
        D_80145F2C++;
    }
    func_8013EE30();
    func_800847B8(D_800E6280.unk_F5F);
    if (!D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1 || (D_80146050 != 0 && D_800E6280.unk_F5F == 9)) {
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1 = 1;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = 0x11;
        D_80122D48 = 3;
        if (D_80146050 != 0) {
            func_80085B3C(8, D_800E6280.unk_F5F * 10);
        } else {
            func_80085B3C(4, 0);
        }
        func_80042908(5);
        return;
    }
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 4;
    for (i = 0; i < 7; i++) {
        if (D_800E6280.unk_114.unk_02 < sp50.v[i]) {
            break;
        }
    }
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_02 += sp40.v[i];
    func_80085B3C(4, 0);
    check_para_limit();
    func_8004284C();
}

void func_8013EA98(void) {
    s16 i;
    s16 count;
    u32 status;
    ShoShorts7 sp50;
    ShoShorts7 sp40;

    sp50 = D_8014610C;
    sp40 = D_8014611C;
    count = 0;
    for (i = 0; i < 11;) {
        count += D_800E6280.unk_1BC[i++].unk_0C.f.b1 >= 1;
    }
    status = 0;
    /* FAKE (T-25501): the "* 0" terms in this loop are references that uopt
     * counts and ugen folds away. The b1 re-read makes the >= 9 test an alias
     * hole of the selector; the selector and counter terms set the colouring
     * order of the loop ranges (selector piece in t4, counter in v0). */
    do {
        D_800E6280.unk_F5F = (D_800E6280.unk_10F8 + status) % 10U + 1;
        status = (s16)(status + 1);
        if (D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1 || D_800E6280.unk_F5F < 9 + D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1 * 0) {
            if (*(u8 *)&D_800E6280.unk_F5F != 5 || ((D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) + D_800E6280.unk_F5F * 0 + status * 0) != 8) {
                if (D_800E6280.unk_F5F != 6 || ((D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) + *(u8 *)&D_800E6280.unk_F5F * 0 + status * 0) != 9) {
                    if (D_80146054 != *(u8 *)&D_800E6280.unk_F5F) {
                        if (D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1) {
                            break;
                        }
                        if (count < 3) {
                            break;
                        }
                    }
                }
            }
        }
    } while (1);
    /* FAKE (T-25501): the counter holds the forced choice so the compares
     * keep their operand order; the record read is an alias hole, and the
     * forced store and the next two reads go through the D_800E71DF alias
     * of unk_F5F, which gives the v1 piece of the original. */
    status = D_80145FE4;
    if ((status == 7 || status == 9) && D_80146054 != status + D_800E6280.unk_1BC[status * 0].unk_06 * 0) {
        D_800E71DF = status + *(u8 *)&D_800E6280.unk_F5F * 0;
    }
    D_80146058 = D_800E71DF;
    D_80145F2C = 0;
    /* FAKE: narrowing as in func_8013E6E4. T-25038 */
    status = (u8)func_80051A68(D_800E71DF);
    if (D_800E6280.unk_F5F == 6 && (status & 0x7F) >= 2U) {
        D_80145F2C++;
    }
    if ((status & 0x7F) == 4) {
        D_80145F2C++;
    }
    func_8013EE30();
    /* FAKE (T-25501): the byte view passes the selector itself, which pins
     * its range to a0 here and forces uopt to split the loop piece. */
    func_800847B8(*(u8 *)&D_800E6280.unk_F5F);
    if (!D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1) {
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b1 = 1;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = 0x18;
        D_80122D48 = 3;
        func_80085B3C(4, 0);
        func_80042908(5);
    } else {
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 4;
        for (i = 0; i < 7; i++) {
            if (D_800E6280.unk_114.unk_02 < sp50.v[i]) {
                break;
            }
        }
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_02 += sp40.v[i];
        func_80085B3C(4, 0);
        func_8004284C();
    }
    check_para_limit();
}

void func_8013EE30(void) {
    switch (D_800E6280.unk_F5F) {
    case 1:
        D_80145F20 = D_80145AE4;
        D_80145F24 = D_80145B18;
        D_80145F28 = D_80145B4C;
        return;
    case 2:
        D_80145F20 = D_80145AD8;
        D_80145F24 = D_80145B0C;
        D_80145F28 = D_80145B40;
        return;
    case 3:
        D_80145F20 = D_80145ADC;
        D_80145F24 = D_80145B10;
        D_80145F28 = D_80145B44;
        return;
    case 4:
        D_80145F20 = D_80145AF4;
        D_80145F24 = D_80145B28;
        D_80145F28 = D_80145B5C;
        return;
    case 5:
        D_80145F20 = D_80145AE8;
        D_80145F24 = D_80145B1C;
        D_80145F28 = D_80145B50;
        return;
    case 6:
        D_80145F20 = D_80145AEC;
        D_80145F24 = D_80145B20;
        D_80145F28 = D_80145B54;
        return;
    case 7:
        D_80145F20 = D_80145AE0;
        D_80145F24 = D_80145B14;
        D_80145F28 = D_80145B48;
        return;
    case 8:
        D_80145F20 = D_80145AD0;
        D_80145F24 = D_80145B04;
        D_80145F28 = D_80145B38;
        return;
    case 9:
        D_80145F20 = D_80145AF0;
        D_80145F24 = D_80145B24;
        D_80145F28 = D_80145B58;
        return;
    case 10:
        D_80145F20 = D_80145B00;
        D_80145F24 = D_80145B34;
        D_80145F28 = D_80145B68;
        /* fallthrough */
    default:
        return;
    }
}

void func_8013F070(void) {
    switch (D_800E6280.unk_F5F) {
    case 0:
        func_80062CD0(0x56C0);
        break;
    case 1:
        func_80062CD0(0x5E7C);
        break;
    case 2:
        func_80062CD0(0x5369);
        break;
    case 3:
        func_80062CD0(0x59EA);
        break;
    case 4:
        func_80062CD0(0x6827);
        break;
    case 5:
        func_80062CD0(0x611F);
        break;
    case 6:
        func_80062CD0(0x6395);
        break;
    case 7:
        func_80062CD0(0x5C60);
        break;
    case 8:
        func_80062CD0(0x51A7);
        break;
    case 9:
        if (D_80146050 != 0) {
            func_80062CD0(0x6638);
        } else {
            func_80062CD0(0x65DE);
        }
        break;
    case 10:
        func_80062CD0(0x6A9D);
        break;
    }
    func_8004284C();
}
