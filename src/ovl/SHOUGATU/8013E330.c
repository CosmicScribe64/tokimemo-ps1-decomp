#include "common.h"
#include "ovl/SHOUGATU.h"

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

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013E330", func_8013E50C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013E330", func_8013E6E4);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013E330", func_8013EA98);

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
