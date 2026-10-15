#include "common.h"
#include "ovl/SHOUGATU.h"

/* T-9740: integer definition without an explicit return matches the original. */
s32 func_8013E330(void) {
    s32 idx;
    ShoCalendarFn28 table;

    table = D_8014605C;
    idx = D_800E6280.unk_110A;
    table.f[idx](0x80);
    ((ShoSprite44 *)D_8011ECD0)[96].unk_07 = D_800B593C;
    if (((ShoSprite44 *)D_8011ECD0)[96].unk_18 == 0 && ((ShoSprite44 *)D_8011ECD0)[96].unk_08 == 0) {
        ((ShoSprite44 *)D_8011ECD0)[96].unk_16 += 1;
        if (((ShoSprite44 *)D_8011ECD0)[96].unk_16 >= 3) {
            ((ShoSprite44 *)D_8011ECD0)[96].unk_16 = 0;
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

/* bulk_m2c declarations (T-11190) */
extern s16 D_800E6396;
extern s32 D_800E7378;
extern s32 D_80122D48;
extern s32 D_801460EC;
extern s32 D_801460FC;
extern s8 D_800E69A2;
extern u8 D_800E644A;
extern u8 D_800E69A0;
extern u8 D_800E69A1;
extern u8 D_800E71DF;
extern u8 D_800E7388;
extern u8 D_800E7389;
extern u8 D_80146054;
void func_8013EE30(void);
extern s32 sp;
/* end bulk_m2c declarations */
void func_8013E6E4(void) {
    s32 sp50[4];
    s32 sp40[4];
    s16 temp_s0;
    s16 temp_t5;
    s16 var_s0;
    s16 var_s0_2;
    s16 var_s0_3;
    s16 var_s1;
    s32 temp_lo;
    s32 temp_v0;
    s32 var_v0;
    u32 temp_v1;
    u8 temp_t7;

    (*(s32 *)&sp50) = (s32) D_801460EC;
    *(s32 *)((unsigned char *)(&sp50) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_801460EC) + (8));
    *(s32 *)((unsigned char *)(&sp50) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801460EC) + (4));
    *(u16 *)((unsigned char *)(&sp50) + (12)) = (u16) *(u16 *)((unsigned char *)(&D_801460EC) + (12));
    (*(s32 *)&sp40) = (s32) D_801460FC;
    *(s32 *)((unsigned char *)(&sp40) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_801460FC) + (8));
    var_s1 = 0;
    var_s0 = 0;
    *(s32 *)((unsigned char *)(&sp40) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801460FC) + (4));
    *(u16 *)((unsigned char *)(&sp40) + (12)) = (u16) *(u16 *)((unsigned char *)(&D_801460FC) + (12));
    do {
        temp_lo = var_s0 * 0x38;
        var_s0 += 1;
        var_s1 += ((u32) (D_800E6280.unk_1BC[0].unk_0C.b[temp_lo] << 0x1E) >> 0x1F) != 0;
    } while (var_s0 < 0xB);
    var_s0_2 = 0;
loop_3:
    temp_t7 = ((u32) (D_800E7378 + var_s0_2) % 10U) + 1;
    D_800E71DF = temp_t7;
    func_800AE0B0("num%d girl%d\n", var_s1, temp_t7 & 0xFF);
    temp_v1 = (u32) (D_800E6280.unk_1BC[D_800E71DF].unk_0C.w << 0x1E) >> 0x1F;
    var_s0_2 += 1;
    if (temp_v1 == 0) {
        if ((u8) D_800E71DF >= 9U) {
            goto loop_3;
        }
    }
    if ((D_800E71DF == 5) && (((u8) D_800E644A >> 4) == 8)) {
        goto loop_3;
    }
    if ((D_800E71DF == 6) && (((u8) D_800E644A >> 4) == 9)) {
        goto loop_3;
    }
    if ((temp_v1 == 0) && (var_s1 >= 2)) {
        goto loop_3;
    }
    if (D_80146050 != 0) {
        D_800E71DF = 9;
    }
    D_80146054 = D_800E71DF;
    D_80145F2C = 0;
    temp_v0 = get_g_zyotai_s(D_800E71DF);
    if ((D_800E71DF == 6) && ((u32) (temp_v0 & 0x7F) >= 2U)) {
        D_80145F2C += 1;
    }
    if ((temp_v0 & 0x7F) == 4) {
        D_80145F2C += 1;
    }
    func_8013EE30();
    func_800847B8(D_800E71DF);
    if (!(D_800E6280.unk_1BC[D_800E71DF].unk_0C.w & 2) || ((D_80146050 != 0) && (D_800E71DF == 9))) {
        D_800E6280.unk_1BC[D_800E71DF].unk_0C.b[0] |= 2;
        D_800E69A0 = D_800E7388;
        D_800E69A1 = D_800E7389;
        D_800E69A2 = 0x11;
        D_80122D48 = 3;
        if (D_80146050 != 0) {
            func_80085B3C(8, D_800E71DF * 0xA);
        } else {
            func_80085B3C(4, 0);
        }
        func_80042908(5);
        return;
    }
    var_s0_3 = 0;
    D_800E6280.unk_1BC[D_800E71DF].unk_06 += 4;
loop_26:
    var_v0 = var_s0_3 * 2;
    temp_s0 = var_s0_3 + 1;
    if (D_800E6396 >= *(s16 *)((unsigned char *)(sp + var_v0) + (80))) {
        temp_t5 = temp_s0;
        var_s0_3 = temp_t5;
        if (temp_t5 >= 7) {
            var_v0 = temp_t5 * 2;
        } else {
            goto loop_26;
        }
    }
    D_800E6280.unk_1BC[D_800E71DF].unk_02 += *(s16 *)((unsigned char *)(sp + var_v0) + (64));
    func_80085B3C(4, 0);
    check_para_limit();
    func_8004284C();
}

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
