s32 func_80146D4C(void) {
    s32 sp3C;
    s32 sp48;
    s32 sp44;
    s32 temp_a1;
    s32 temp_s0;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 var_a3;
    s32 var_s0;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    u8 *temp_a2;
    u8 *var_a2;

    sp44 = 0;
    var_s0 = 0;
    switch (*(u8 *)((unsigned char *)(&D_800E6280) + (65))) {
    case 0:
        sp44 = (s32) *(s16 *)((unsigned char *)(&D_800E6280) + (258));
        var_a3 = func_8006C334(0, 0, 1, 0x10);
        break;
    case 1:
        sp44 = (s32) *(s16 *)((unsigned char *)(&D_800E6280) + (262));
        var_a3 = func_8006C334(0, 0, 1, 0x10);
        break;
    case 2:
        sp44 = (s32) (*(s16 *)((unsigned char *)(&D_800E6280) + (262)) * 7) / 10;
        var_s0 = (s32) (*(s16 *)((unsigned char *)(&D_800E6280) + (274)) * 3) / 10;
        if (var_s0 >= 0x27) {
            var_s0 = 0x26;
        }
        var_a3 = func_8006C334(0, 1, 1, 0x10);
        break;
    case 3:
        var_s0 = (s32) (*(s16 *)((unsigned char *)(&D_800E6280) + (258)) * 3) / 10;
        sp44 = (s32) (*(s16 *)((unsigned char *)(&D_800E6280) + (274)) * 7) / 10;
        var_a3 = func_8006C334(0, 0, 1, 0x10);
        if (var_s0 >= 0x27) {
            var_s0 = 0x26;
        }
        break;
    case 4:
        sp44 = (s32) (*(s16 *)((unsigned char *)(&D_800E6280) + (266)) * 8) / 10;
        var_s0 = (s32) (*(s16 *)((unsigned char *)(&D_800E6280) + (274)) * 2) / 10;
        var_a3 = func_8006C334(0, 1, 0, 0x10);
        if (var_s0 >= 0x1A) {
            var_s0 = 0x19;
        }
        break;
    default:
        func_80046500(*(u8 *)((unsigned char *)(&D_800E6280) + (65)));
        var_a3 = sp3C;
        break;
    }
    temp_v0 = (sp44 + var_s0 + var_a3) - 8;
    var_v1_2 = temp_v0;
    if (*(u8 *)((unsigned char *)(&D_800E6280) + (245)) & 2) {
        var_v1_2 = temp_v0 / 2;
    }
    sp48 = var_v1_2;
    func_800AE0B0("main %d sub %d bio %d ret %d ", sp44, (void *) var_s0, var_a3, var_v1_2);
    var_v1_3 = var_v1_2;
    if (var_v1_3 < 0) {
        var_v1_3 = 1;
    }
    sp48 = var_v1_3;
    temp_v1 = func_80146400() + var_v1_3;
    sp48 = temp_v1;
    temp_s0 = func_801464D4(0xF);
    temp_v1_2 = ((func_800AE0D0() % 10) * temp_s0) + temp_v1;
    if (temp_v1_2 < 0x80) {
        var_a2 = &D_801230D0 + (*(u8 *)((unsigned char *)(&D_800E6280) + (65)) * 4);
        *(s32 *)((unsigned char *)var_a2 + (40)) = (s32) ((s32) (temp_v1_2 * 0x64) / 128);
    } else {
        var_a2 = &D_801230D0 + (*(u8 *)((unsigned char *)(&D_800E6280) + (65)) * 4);
        *(s32 *)((unsigned char *)var_a2 + (40)) = (s32) (0x64 - (func_800AE0D0() & 3));
    }
    func_800AE0B0("今日の成績 %d\t", *(s32 *)((unsigned char *)var_a2 + (40)), var_a2, (s32) *(u8 *)((unsigned char *)(&D_800E6280) + (65)));
    temp_a1 = *(s32 *)((unsigned char *)(&D_801230D0) + (60)) + *(s32 *)((unsigned char *)(&D_801230D0 + (*(u8 *)((unsigned char *)(&D_800E6280) + (65)) * 4)) + (40));
    *(s32 *)((unsigned char *)(&D_801230D0) + (60)) = temp_a1;
    func_800AE0B0("合計点 %d\n", temp_a1);
    temp_a2 = &D_801230D0 + (*(u8 *)((unsigned char *)(&D_800E6280) + (65)) * 4);
    temp_v1_3 = *(s32 *)((unsigned char *)temp_a2 + (40));
    if (temp_v1_3 < 0x28) {
        var_v1 = 0;
    } else if (temp_v1_3 < 0x3C) {
        var_v1 = 1;
    } else if (temp_v1_3 < 0x50) {
        var_v1 = 2;
    } else {
        var_v1 = 4;
        if (temp_v1_3 < 0x5F) {
            var_v1 = 3;
        }
    }
    if (var_v1 == 0) {
        *(s32 *)((unsigned char *)(&D_801230D0) + (36)) = (s32) (*(s32 *)((unsigned char *)(&D_801230D0) + (36)) + 1);
    }
    *(s32 *)((unsigned char *)temp_a2 + (16)) = var_v1;
    return var_v1;
}
