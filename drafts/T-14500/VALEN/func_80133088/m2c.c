u32 func_80052648(u8);                              /* extern */
extern s16 D_800E643E;
extern u8 D_800E71DE;
extern u8 D_800E71DF;
extern s32 D_800E7378;
extern s32 D_80122D48;
extern ? D_80134526;
extern s8 D_80134548;

void func_80133088(void) {
    s32 sp28;
    s16 temp_v0_2;
    s32 var_a2;
    s32 var_a2_2;
    s8 var_v0;
    u32 temp_v0;

    temp_v0 = D_800E7378 & 7;
    if (temp_v0 == 0) {
        var_v0 = 3;
    } else {
        var_v0 = 1;
        if (temp_v0 < 4U) {
            var_v0 = 0;
        }
    }
    if (D_800E71DF == 0) {
        var_v0 = D_800E7378 & 1;
    }
    if (var_v0 != 0) {
        if (var_v0 != 1) {
            D_80134548 = var_v0;
            if (var_v0 != 2) {
                func_800AE0F0(D_800CA1DC, "屋上");
                var_a2 = 0x4084;
            } else {
                func_800AE0F0(D_800CA1DC, "校舎裏");
                var_a2 = 0x407A;
            }
        } else {
            D_80134548 = var_v0;
            func_800AE0F0(D_800CA1DC, "教室");
            var_a2 = 0x4055;
        }
    } else {
        D_80134548 = var_v0;
        func_800AE0F0(D_800CA1DC, "廊下");
        var_a2 = 0x4045;
    }
    if (D_800E71DF != 0) {
        temp_v0_2 = D_80134520;
        if ((temp_v0_2 + 1) == D_8013453C) {
            func_800AE0F0(D_800CA1DC, "校門前");
            var_a2 = 0x4032;
        }
        if (((temp_v0_2 + 2) == D_8013453C) && (*(&D_80134526 + (temp_v0_2 * 2)) == 0)) {
            sp28 = 0x4032;
            func_800AE0F0(D_800CA1DC, "校門前");
            var_a2 = 0x4032;
        }
    }
    if (((D_80134520 + 1) == D_8013453C) && (D_800E71DF == 0) && ((D_800E643E >= 0x46) || (D_80134544 != 0))) {
        func_800AE0F0(D_800CA1DC, "自宅前");
        var_a2 = 0x414C;
    }
    if ((D_800E71DF == 9) && (D_80134544 == 1)) {
        sp28 = 0x4055;
        func_800AE0F0(D_800CA1DC, "教室");
        var_a2 = 0x4055;
    }
    if (D_800E71DF == 0) {
        if (D_800E643E >= 0x46) {
            D_80122D48 = 1;
        } else {
            D_80122D48 = 2;
        }
    } else {
        sp28 = var_a2;
        if (func_80052648(D_800E71DF) >= 0x46U) {
            D_80122D48 = 1;
        } else {
            D_80122D48 = 2;
        }
    }
    switch (var_a2) {                               /* irregular */
    case 0x4045:
        sp28 = var_a2;
        func_80085B3C(6, 0);
        var_a2_2 = var_a2;
        break;
    case 0x4055:
        sp28 = var_a2;
        func_80085B3C(6, 1);
        var_a2_2 = var_a2;
        break;
    case 0x414C:
        sp28 = var_a2;
        func_80085B3C(6, 5);
        var_a2_2 = var_a2;
        break;
    case 0x4032:
        sp28 = var_a2;
        func_80085B3C(6, 4);
        var_a2_2 = var_a2;
        break;
    case 0x407A:
        sp28 = var_a2;
        func_80085B3C(6, 3);
        var_a2_2 = var_a2;
        break;
    default:
        sp28 = var_a2;
        func_80085B3C(6, 2);
        var_a2_2 = var_a2;
        break;
    }
    if ((D_800E71DF == 9) && (D_80134544 == 1)) {
        D_800E71DE -= 1;
        D_80122D48 = 1;
        sp28 = var_a2_2;
        func_80085B3C(6, 3);
    }
    func_8007ED84(var_a2_2);
    if (func_80052648(D_800E71DF) >= 0x46U) {
        func_80083440(0U);
    } else {
        func_80083440(1U);
    }
    if (D_800E71DF == 0) {
        if (D_800E643E >= 0x46) {
            func_80083440(0U);
        } else {
            func_80083440(1U);
        }
    }
    func_8004284C();
}
