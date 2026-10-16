void func_80136988(void) {
    s32 temp_v1;
    s32 var_at;
    s32 var_v0;

    temp_v1 = D_800E6280.unk_F88;
    if (temp_v1 & 0x5000) {
        func_801335A0(0, 0x500);
    }
    if (temp_v1 & 0x1000) {
        D_8015F3F0 -= 1;
    }
    var_v0 = D_8015F3F0;
    if (temp_v1 & 0x4000) {
        var_v0 += 1;
    }
    var_at = var_v0 < 5;
    if (var_v0 < 0) {
        var_v0 = 4;
        var_at = 4 < 5;
    }
    if (var_at == 0) {
        var_v0 = 0;
    }
    D_8015F3F0 = var_v0;
    if (temp_v1 & 0xA020) {
        D_8015F3F0 = var_v0;
        if (var_v0 != 4) {
            func_801335A0(0, 0x501);
        }
        switch (D_8015F3F0) {
        case 0:
            func_801368D8((s32) &D_8015EDD8, 2);
            break;
        case 1:
            func_801368D8((s32) &D_8015EDDC, 3);
            break;
        case 2:
            func_801368D8((s32) &D_8015EDE0, 3);
            break;
        case 3:
            func_801368D8((s32) &D_8015EDE4, 3);
            break;
        case 4:
            if (temp_v1 & 0x20) {
                func_801335A0(0, 0x502);
                func_8004284C();
            }
            break;
        }
    }
    if (temp_v1 & 0x840) {
        func_801335A0(0, 0x502);
        func_8004284C();
    }
}
