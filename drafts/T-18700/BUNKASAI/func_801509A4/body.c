void func_801509A4(void) {
    BunkasaiGirlList list;
    u8 sp63;
    Rec38 *var_a0;
    s32 temp_v1;
    s32 var_v1;
    u8 var_a1;

    list = D_80160AA4;
    func_800AE120(D_800E6280.unk_10F4);
    var_a1 = 0;
    var_a0 = D_800E6280.unk_1BC;
    var_v1 = 0;
    do {
        if (((u32) (var_a0->unk_0C.w << 0x1E) >> 0x1F) == 1) {
            list.v[var_a1] = var_v1;
            var_a1 = (var_a1 + 1) & 0xFF;
        }
        var_v1 += 1;
        var_a0 += 1;
    } while (var_v1 != 0xB);
    sp63 = var_a1;
    temp_v1 = list.v[func_800AE0D0() % var_a1];
    D_80122EBC = temp_v1;
    switch (temp_v1) {
    case 0:
        func_80042908(0xF);
        return;
    case 1:
        func_80042908(0x1C);
        return;
    case 2:
        func_80042908(0x2E);
        return;
    case 3:
        func_80042908(0x41);
        return;
    case 4:
        func_80042908(0x55);
        return;
    case 5:
        func_80042908(0x61);
        return;
    case 6:
        func_80042908(0x6D);
        return;
    case 7:
        func_80042908(0x79);
        return;
    case 8:
        func_80042908(0x85);
        return;
    case 9:
        func_80042908(0x91);
        return;
    case 10:
        func_80042908(0x9A);
        return;
    default:
        func_80046500();
        return;
    }
}
