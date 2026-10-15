? func_8004F984(?, s16, s16);                       /* extern */
extern s32 D_800E7200;
extern u8 D_800E7313;
extern s32 D_8013E7C4;

void func_8013B568(void) {
    s32 *var_s1;
    s32 var_s0;
    s8 var_t0;
    void *var_v1;

    func_8004F984(0, D_8011ECF6, D_8011ECFA);
    func_8004FC10(0);
    func_80050B54(0);
    var_s1 = &D_8013E7C4;
    var_s0 = -0x60;
    do {
        func_8004B338(*var_s1, -0x20, var_s0);
        var_s0 += 0xC;
        var_s1 += 4;
    } while (var_s0 != 0x60);
    func_8004AE28((s32) D_8013E740, 0x20, -0x60);
    func_8004AE28((s32) D_800E6280.unk_1BC[D_8013E740].unk_02, 0x20, -0x54);
    func_8004AE28((s32) D_800E6280.unk_1BC[D_8013E740].unk_06, 0x20, -0x48);
    func_8004AE28((s32) D_800E6280.unk_1BC[D_8013E740].unk_0A, 0x20, -0x3C);
    func_8004AE28(D_800E6280.unk_1BC[D_8013E740].unk_0C.w & 1, 0x20, -0x30);
    func_8004AE28((s32) ((u32) (D_800E6280.unk_1BC[D_8013E740].unk_0C.w << 0x1E) >> 0x1F), 0x20, -0x24);
    func_8004AE28((s32) ((u32) (D_800E6280.unk_1BC[D_8013E740].unk_0C.w << 0x1D) >> 0x1F), 0x20, -0x18);
    func_8004AE28((s32) ((u32) (D_800E6280.unk_1BC[D_8013E740].unk_0C.w << 0x1C) >> 0x1F), 0x20, -0xC);
    func_8004AE28((s32) D_800E6280.unk_1BC[D_8013E740].unk_0C.b[3], 0x20, 0);
    func_8004AE28((s32) ((u32) (D_800E6280.unk_1BC[D_8013E740].unk_0C.w << 0x19) >> 0x1F), 0x20, 0xC);
    func_8004AE28((s32) ((u8) D_800E6280.unk_1BC[D_8013E740].unk_0C.b[0] >> 7), 0x20, 0x18);
    func_8004AE28(D_800E6280.unk_1BC[D_8013E740].unk_0C.b[1] & 1, 0x20, 0x24);
    func_8004AE28((s32) ((u32) (D_800E6280.unk_1BC[D_8013E740].unk_0C.w << 0x14) >> 0x1D), 0x20, 0x30);
    func_8004AE28((s32) ((u8) D_800E6280.unk_1BC[D_8013E740].unk_0C.b[2] >> 4), 0x20, 0x3C);
    func_8004AE28((s32) ((u32) (D_800E6280.unk_1BC[D_8013E740].unk_0C.w << 0x13) >> 0x1F), 0x20, 0x48);
    func_8004AE28((s32) ((u32) (D_800E6280.unk_1BC[D_8013E740].unk_0C.w << 0x12) >> 0x1F), 0x20, 0x54);
    if (D_800E7200 & 0x90) {
        switch (D_800E7313) {
        case 0:
            D_8013E740 = func_8013A550((s32) D_8013E740, 0xB, 0);
            func_80042940(0);
            break;
        case 1:
            D_800E6280.unk_1BC[D_8013E740].unk_02 = func_8013A550((s32) D_800E6280.unk_1BC[D_8013E740].unk_02, 0x7F, 0);
            break;
        case 2:
            D_800E6280.unk_1BC[D_8013E740].unk_06 = func_8013A550((s32) D_800E6280.unk_1BC[D_8013E740].unk_06, 0x7F, 0);
            break;
        case 3:
            D_800E6280.unk_1BC[D_8013E740].unk_0A = func_8013A550((s32) D_800E6280.unk_1BC[D_8013E740].unk_0A, 0x7F, 0);
            break;
        case 4:
            D_800E6280.unk_1BC[D_8013E740].unk_0C.b[0] = (func_8013A550(D_800E6280.unk_1BC[D_8013E740].unk_0C.w & 1, 1, 0) & 1) | (D_800E6280.unk_1BC[D_8013E740].unk_0C.b[0] & 0xFFFE);
            break;
        case 5:
            D_800E6280.unk_1BC[D_8013E740].unk_0C.b[0] = ((func_8013A550((s32) ((u32) (D_800E6280.unk_1BC[D_8013E740].unk_0C.w << 0x1E) >> 0x1F), 1, 0) * 2) & 2) | (D_800E6280.unk_1BC[D_8013E740].unk_0C.b[0] & 0xFFFD);
            break;
        case 6:
            D_800E6280.unk_1BC[D_8013E740].unk_0C.b[0] = ((func_8013A550((s32) ((u32) (D_800E6280.unk_1BC[D_8013E740].unk_0C.w << 0x1D) >> 0x1F), 1, 0) * 4) & 4) | (D_800E6280.unk_1BC[D_8013E740].unk_0C.b[0] & 0xFFFB);
            break;
        case 7:
            D_800E6280.unk_1BC[D_8013E740].unk_0C.b[0] = ((func_8013A550((s32) ((u32) (D_800E6280.unk_1BC[D_8013E740].unk_0C.w << 0x1C) >> 0x1F), 1, 0) * 8) & 8) | (D_800E6280.unk_1BC[D_8013E740].unk_0C.b[0] & 0xFFF7);
            break;
        case 8:
            D_800E6280.unk_1BC[D_8013E740].unk_0C.b[3] = func_8013A550((s32) D_800E6280.unk_1BC[D_8013E740].unk_0C.b[3], 0x3E7, 0);
            break;
        case 9:
            D_800E6280.unk_1BC[D_8013E740].unk_0C.b[0] = ((func_8013A550((s32) ((u32) (D_800E6280.unk_1BC[D_8013E740].unk_0C.w << 0x19) >> 0x1F), 1, 0) << 6) & 0x40) | (D_800E6280.unk_1BC[D_8013E740].unk_0C.b[0] & 0xFFBF);
            break;
        case 10:
            D_800E6280.unk_1BC[D_8013E740].unk_0C.b[0] = (func_8013A550((s32) ((u8) D_800E6280.unk_1BC[D_8013E740].unk_0C.b[0] >> 7), 1, 0) << 7) | (D_800E6280.unk_1BC[D_8013E740].unk_0C.b[0] & 0xFF7F);
            break;
        case 11:
            var_v1 = &D_800E6280 + (D_8013E740 * 0x38);
            var_t0 = (func_8013A550(D_800E6280.unk_1BC[D_8013E740].unk_0C.b[1] & 1, 1, 0) & 1) | (D_800E6280.unk_1BC[D_8013E740].unk_0C.b[1] & 0xFFFE);
block_21:
            *(s8 *)((unsigned char *)var_v1 + (457)) = var_t0;
            break;
        case 12:
            D_800E6280.unk_1BC[D_8013E740].unk_0C.b[1] = ((func_8013A550((s32) ((u32) (D_800E6280.unk_1BC[D_8013E740].unk_0C.w << 0x14) >> 0x1D), 7, 0) * 2) & 0xE) | (D_800E6280.unk_1BC[D_8013E740].unk_0C.b[1] & 0xFFF1);
            break;
        case 13:
            D_800E6280.unk_1BC[D_8013E740].unk_0C.b[2] = (func_8013A550((s32) ((u8) D_800E6280.unk_1BC[D_8013E740].unk_0C.b[2] >> 4), 0xB, 0) * 0x10) | (D_800E6280.unk_1BC[D_8013E740].unk_0C.b[2] & 0xFF0F);
            break;
        case 14:
            D_800E6280.unk_1BC[D_8013E740].unk_0C.b[1] = ((func_8013A550((s32) ((u32) (D_800E6280.unk_1BC[D_8013E740].unk_0C.w << 0x13) >> 0x1F), 1, 0) * 0x10) & 0x10) | (D_800E6280.unk_1BC[D_8013E740].unk_0C.b[1] & 0xFFEF);
            break;
        case 15:
            var_v1 = &D_800E6280 + (D_8013E740 * 0x38);
            var_t0 = ((func_8013A550((s32) ((u32) (D_800E6280.unk_1BC[D_8013E740].unk_0C.w << 0x12) >> 0x1F), 1, 0) << 5) & 0x20) | (D_800E6280.unk_1BC[D_8013E740].unk_0C.b[1] & 0xFFDF);
            goto block_21;
        }
    }
}
