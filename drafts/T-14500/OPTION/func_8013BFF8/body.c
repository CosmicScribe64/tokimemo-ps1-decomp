void func_8013BFF8(void) {
    s32 *var_s1;
    s32 var_s0;

    menu_check(0, D_8011ECF6, D_8011ECFA);
    func_8004FC10(0);
    menu_bar_show(0);
    var_s1 = &D_8013E804;
    var_s0 = -0x60;
    do {
        func_8004B338(*var_s1, -0x20, var_s0);
        var_s0 += 0xC;
        var_s1 += 4;
    } while (var_s0 != 0x60);
    func_8004AE28((s32) D_800E691D, 0x20, -0x60);
    func_8004AE28((s32) D_800E691E, 0x20, -0x54);
    func_8004AE28(D_800E6280.unk_54C[0].b[3] & 1, 0x20, -0x30);
    func_8004AE28((s32) ((u32) (D_800E6280.unk_54C[0].w * 8) >> 0x1C), 0x20, -0x24);
    func_8004AE28(D_800E6280.unk_54C[0].w & 0x1F, 0x20, -0x18);
    func_8004AE28((s32) D_800E69A4, 0x20, 0);
    func_8004AE28((s32) *(u8 *)((unsigned char *)(&D_800E69A4) + (1)), 0x20, 0xC);
    func_8004AE28((s32) *(u8 *)((unsigned char *)(&D_800E69A4) + (2)), 0x20, 0x18);
    func_8004AE28((s32) *(u8 *)((unsigned char *)(&D_800E69A4) + (3)), 0x20, 0x24);
    func_8004AE28((s32) D_800E71DF, 0x20, 0x30);
    func_8004AE28((s32) D_800E69DD, 0x20, 0x3C);
    func_8004AE28(D_80122CD4, 0x20, 0x48);
    func_8004AE28(D_80122CD0, 0x20, 0x54);
    if (D_800E7200 & 0x90) {
        switch (D_800E7313) {
        case 0:
            D_800E691D = func_8013A550((s32) D_800E691D, 0xA, 0);
            break;
        case 1:
            D_800E691E = func_8013A550((s32) D_800E691E, 0x24, 0);
            break;
        case 4:
            D_800E67CF = (func_8013A550(D_800E67CF & 1, 1, 0) & 1) | (D_800E67CF & 0xFFFE);
            break;
        case 5:
            D_800E67CF = ((func_8013A550((s32) ((u32) (D_800E6280.unk_54C[0].w * 8) >> 0x1C), 0xA, 0) * 2) & 0x1E) | (D_800E67CF & 0xFFE1);
            break;
        case 6:
            D_800E6280.unk_54C[0].b[0] = (func_8013A550(D_800E6280.unk_54C[0].w & 0x1F, 0x15, 0) & 0x1F) | (D_800E6280.unk_54C[0].b[0] & 0xFFE0);
            break;
        case 8:
            D_800E69A4 = func_8013A550((s32) D_800E69A4, 0x15, 0);
            break;
        case 9:
            *(u8 *)((unsigned char *)(&D_800E69A4) + (1)) = func_8013A550((s32) *(u8 *)((unsigned char *)(&D_800E69A4) + (1)), 0x15, 0);
            break;
        case 10:
            *(u8 *)((unsigned char *)(&D_800E69A4) + (2)) = func_8013A550((s32) *(u8 *)((unsigned char *)(&D_800E69A4) + (2)), 0x15, 0);
            break;
        case 11:
            *(u8 *)((unsigned char *)(&D_800E69A4) + (3)) = func_8013A550((s32) *(u8 *)((unsigned char *)(&D_800E69A4) + (3)), 0x15, 0);
            break;
        case 12:
            D_800E71DF = func_8013A550((s32) D_800E71DF, 0xA, 0);
            break;
        case 13:
            D_800E69DD = func_8013A550((s32) D_800E69DD, 0xA, 0);
            break;
        case 14:
            D_80122CD4 = func_8013A550(D_80122CD4, 0xC, 0);
            break;
        case 15:
            D_80122CD0 = func_8013A550(D_80122CD0, 8, 1);
            break;
        }
    }
}
