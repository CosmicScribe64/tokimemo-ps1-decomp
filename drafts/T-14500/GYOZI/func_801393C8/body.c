void func_801393C8(void) {
    s32 sp28;
    s32 var_a2;
    s32 var_v1;
    u32 temp_v0;
    u32 temp_v0_2;
    u8 temp_a1;
    u8 temp_v0_3;
    u8 var_v0;

    temp_v0 = D_800F6468 & 7;
    if (temp_v0 == 0) {
        var_v0 = 3;
    } else {
        var_v0 = 1;
        if (temp_v0 < 4U) {
            var_v0 = 0;
        }
    }
    if (D_800F62CF == 0) {
        var_v0 = D_800F6468 & 1;
    }
    if (var_v0 != 0) {
        if (var_v0 != 1) {
            D_80147244 = var_v0;
            if (var_v0 != 2) {
                func_800BCE10(&D_800D92E0, "屋上");
                var_a2 = 0x4023;
            } else {
                func_800BCE10(&D_800D92E0, "校舎裏");
                var_a2 = 0x401A;
            }
        } else {
            D_80147244 = var_v0;
            func_800BCE10(&D_800D92E0, "教室");
            var_a2 = 0x3FFF;
        }
    } else {
        D_80147244 = var_v0;
        func_800BCE10(&D_800D92E0, "廊下");
        var_a2 = 0x3FED;
    }
    var_v1 = D_8014723C - 1;
    if (D_80147220 == var_v1) {
        func_800BCE10(&D_800D92E0, "校門前", var_a2);
        var_a2 = 0x3FDA;
        var_v1 = D_8014723C - 1;
    }
    if ((D_80147220 == var_v1) && (D_800F62CF == 0)) {
        sp28 = var_a2;
        temp_v0_2 = func_8005EBE0(D_800F62CF, D_800F62CF, var_a2);
        if (temp_v0_2 >= 0x46U) {
            func_800BCE10(&D_800D92E0, "自宅前", var_a2);
            var_a2 = 0x40ED;
        }
    }
    temp_a1 = D_800F62CF;
    if ((temp_a1 == 9) && (D_80147240 == 1) && ((u8) D_80147244 >= 2U)) {
        func_800BCE10(&D_800D92E0, "教室", var_a2);
        var_a2 = 0x3FF5;
    }
    temp_v0_3 = D_80147244;
    if (temp_a1 == 9) {
        D_80147244 = temp_v0_3;
        if (D_80147240 == 1) {
            D_80147244 = temp_v0_3;
            if (temp_v0_3 < 2U) {
                sp28 = 0x3FED;
                func_800BCE10(&D_800D92E0, "廊下", 0x3FED);
                var_a2 = 0x3FED;
            }
        }
    }
    switch (var_a2) {
    case 0x3FED:
        sp28 = var_a2;
        func_80090960(6, 0, var_a2);
        break;
    case 0x3FF5:
        sp28 = var_a2;
        func_80090960(6, 1, var_a2);
        break;
    case 0x40ED:
        sp28 = var_a2;
        func_80090960(6, 5, var_a2);
        break;
    case 0x3FDA:
        sp28 = var_a2;
        func_80090960(6, 4, var_a2);
        break;
    case 0x401A:
        sp28 = var_a2;
        func_80090960(6, 3, var_a2);
        break;
    default:
        sp28 = var_a2;
        func_80090960(6, 2, var_a2);
        break;
    }
    func_8008A0D4(var_a2);
    func_8004DE1C();
}
