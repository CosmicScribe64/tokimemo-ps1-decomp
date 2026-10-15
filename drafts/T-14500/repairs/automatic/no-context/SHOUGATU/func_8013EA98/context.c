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

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013E330", func_8013E6E4);

/* bulk_m2c declarations (T-11190) */
extern s16 D_800E6396;
extern s16 D_80145F2C;
extern s32 D_800E6280;
extern s32 D_800E7378;
extern s32 D_80122D48;
extern s32 D_8014610C;
extern s32 D_8014611C;
extern s8 D_800E69A2;
extern u8 D_800E644A;
extern u8 D_800E69A0;
extern u8 D_800E69A1;
extern u8 D_800E71DF;
extern u8 D_800E7388;
extern u8 D_800E7389;
extern u8 D_80145FE4;
extern u8 D_80146054;
extern u8 D_80146058;
s32 func_8004284C();
s32 func_80042908();
s32 func_800847B8();
s32 func_80085B3C();
extern s32 sp;
void func_8013EE30(void);
/* end bulk_m2c declarations */
void func_8013EA98(void) {
    s32 sp30[4];
    s16 temp_t8_3;
    s16 temp_v0;
    s16 temp_v0_3;
    s16 var_a0;
    s16 var_v0;
    s16 var_v0_2;
    s16 var_v0_3;
    s32 temp_v0_2;
    s32 var_v1;
    u32 temp_hi;
    u32 temp_t7;
    u32 temp_t8_2;
    u8 temp_t8;
    u8 *temp_a2;
    u8 *temp_a2_2;

    sp40[0].unk0 = (s32) D_8014610C;
    *(s32 *)((unsigned char *)(&sp40[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014610C) + (4));
    *(s32 *)((unsigned char *)(&sp40[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8014610C) + (8));
    *(u16 *)((unsigned char *)(&sp40[0]) + (12)) = (u16) *(u16 *)((unsigned char *)(&D_8014610C) + (12));
    (*(s32 *)&sp30) = (s32) D_8014611C;
    *(s32 *)((unsigned char *)(&sp30) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8014611C) + (8));
    var_a0 = 0;
    var_v0 = 0;
    *(s32 *)((unsigned char *)(&sp30) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014611C) + (4));
    *(u16 *)((unsigned char *)(&sp30) + (12)) = (u16) *(u16 *)((unsigned char *)(&D_8014611C) + (12));
    do {
        temp_v0 = var_v0 + 1;
        var_a0 += ((u32) (*(s32 *)((unsigned char *)(&D_800E6280 + (var_v0 * 0x38)) + (456)) << 0x1E) >> 0x1F) != 0;
        var_v0 = temp_v0;
    } while (temp_v0 < 0xB);
    var_v0_2 = 0;
loop_3:
    temp_hi = (u32) (D_800E7378 + var_v0_2) % 10U;
    var_v0_2 += 1;
    temp_t8 = temp_hi + 1;
    D_800E71DF = temp_t8;
    temp_t7 = (u32) (*(s32 *)((unsigned char *)(&D_800E6280 + ((temp_t8 & 0xFF) * 0x38)) + (456)) << 0x1E) >> 0x1F;
    if ((temp_t7 == 0) && ((u32) (temp_t8 & 0xFF) >= 9U)) {
block_12:
        goto loop_3;
    }
    if ((D_800E71DF == 5) && (((u8) D_800E644A >> 4) == 8)) {
        goto block_12;
    }
    if ((D_800E71DF == 6) && (((u8) D_800E644A >> 4) == 9)) {
        goto block_12;
    }
    if (D_80146054 == D_800E71DF) {
        goto block_12;
    }
    if ((temp_t7 == 0) && (var_a0 >= 3)) {
        goto block_12;
    }
    if (((D_80145FE4 == 7) || (D_80145FE4 == 9)) && (D_80146054 != D_80145FE4)) {
        D_800E71DF = D_80145FE4;
    }
    D_80146058 = D_800E71DF;
    D_80145F2C = 0;
    temp_v0_2 = get_g_zyotai_s(D_800E71DF, 0xA, 5, 8);
    temp_t8_2 = temp_v0_2 & 0x7F;
    if ((D_800E71DF == 6) && (temp_t8_2 >= 2U)) {
        D_80145F2C += 1;
    }
    if ((temp_v0_2 & 0x7F) == 4) {
        D_80145F2C += 1;
    }
    func_8013EE30();
    func_800847B8(D_800E71DF);
    temp_a2 = &D_800E6280 + (D_800E71DF * 0x38);
    if (!(*(s32 *)((unsigned char *)temp_a2 + (456)) & 2)) {
        *(s32 *)((unsigned char *)temp_a2 + (456)) = (s8) ((u8) *(s32 *)((unsigned char *)temp_a2 + (456)) | 2);
        D_800E69A0 = D_800E7388;
        D_800E69A1 = D_800E7389;
        D_800E69A2 = 0x18;
        D_80122D48 = 3;
        func_80085B3C(4, 0, temp_a2);
        func_80042908(5);
    } else {
        *(s16 *)((unsigned char *)temp_a2 + (450)) = (s16) (*(s16 *)((unsigned char *)temp_a2 + (450)) + 4);
        var_v0_3 = 0;
loop_25:
        var_v1 = var_v0_3 * 2;
        temp_v0_3 = var_v0_3 + 1;
        if (D_800E6396 >= (&sp40[0])[var_v0_3]) {
            temp_t8_3 = temp_v0_3;
            var_v0_3 = temp_t8_3;
            if (temp_t8_3 >= 7) {
                var_v1 = temp_t8_3 * 2;
            } else {
                goto loop_25;
            }
        }
        temp_a2_2 = &D_800E6280 + (D_800E71DF * 0x38);
        *(s16 *)((unsigned char *)temp_a2_2 + (446)) = (s16) (*(s16 *)((unsigned char *)temp_a2_2 + (446)) + *(s16 *)((unsigned char *)(sp + var_v1) + (48)));
        func_80085B3C(4, 0, temp_a2_2);
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
