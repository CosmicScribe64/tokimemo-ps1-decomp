void func_80139BCC(void) {
    s32 sp40;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_lo;
    s32 temp_v1;
    s32 var_a1;
    s32 var_at;
    s32 var_t0;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v1;

    temp_a1 = D_8015F4E4;
    if (temp_a1 < 0) {
        var_v1 = temp_a1 + 0x50;
    } else {
        var_v1 = 0x50 - temp_a1;
    }
    D_8015F4E4 = temp_a1;
    sp40 = var_v1;
    func_80139FB8(D_8015F4E0, temp_a1, 0xA0, 0);
    temp_lo = (s32) (var_v1 * 0x7000) / 80;
    func_80139424((s32) *(&D_8015F4B0 + ((s32) D_8015F4E0 % 40)), 6, 4, 0x80, 0, 0x3C, temp_lo, temp_lo, 1);
    func_8013A580(0x1E0);
    func_8013A790(0, 0);
    temp_a2 = D_800E6280.unk_F80 & 0x2000;
    temp_a0 = D_800E6280.unk_F80 & 0x8000;
    if ((temp_a2 != 0) && (D_8015F4E8 < 0)) {
        D_8015F4E8 = 1;
    }
    var_v0 = D_8015F4E8;
    if ((temp_a0 != 0) && (var_v0 > 0)) {
        var_v0 = -1;
    }
    switch (var_v0) {
    default:
        var_a1 = D_8015F4E4;
        break;
    case -2:
        var_a1 = D_8015F4E4 + 0xA;
        break;
    case -1:
        var_a1 = D_8015F4E4 + 5;
        break;
    case 1:
        var_a1 = D_8015F4E4 - 5;
        break;
    case 2:
        var_a1 = D_8015F4E4 - 0xA;
        break;
    }
    D_8015F4E8 = var_v0;
    if (var_a1 == 0) {
        if (!(D_800E6280.unk_F80 & 0xA000)) {
            var_v0 = 0;
        }
        if (temp_a2 != 0) {
            var_v0 += 1;
        }
        var_at = var_v0 < 3;
        if (temp_a0 != 0) {
            var_v0 -= 1;
            var_at = var_v0 < 3;
        }
        if (var_at == 0) {
            var_v0 = 2;
        }
        D_8015F4E8 = var_v0;
        if (var_v0 < -2) {
            D_8015F4E8 = -2;
        }
    }
    if (var_a1 >= 0x33) {
        D_8015F4E0 += 1;
        var_a1 = -0x32;
    }
    D_8015F4E4 = var_a1;
    var_t0 = D_8015F4E0;
    if (var_a1 < -0x32) {
        D_8015F4E4 = 0x32;
        var_t0 -= 1;
    }
    D_8015F4E0 = var_t0;
    if (var_t0 >= 0x28) {
        D_8015F4E0 = 0;
    }
    if (D_8015F4E0 < 0) {
        D_8015F4E0 = 0x27;
    }
    temp_v1 = D_800E6280.unk_F88;
    if ((temp_v1 & 0x20) && !(temp_v1 & 0x40)) {
        if (D_8015F4E0 == 0x27) {
            func_800AE090((void *) D_8015EE04[(D_8015F4D8 * 4) + D_8015F4DC], &D_8015D98C, 1);
            func_8004284C();
        } else if (D_8015F4DC != 3) {
            func_800AE090(&D_8015EE04[(D_8015F4D8 * 4) + D_8015F4DC], D_8015F4E0 + &D_8015F4B0, 1);
            D_8015F4DC += 1;
        }
    }
    var_v0_2 = temp_v1 & 0x40;
    if ((temp_v1 & 0x800) && (var_v0_2 == 0)) {
        func_800AE090((void *) D_8015EE04[(D_8015F4D8 * 4) + D_8015F4DC], &D_8015D990, 1);
        func_8004284C();
        var_v0_2 = D_800E6280.unk_F88 & 0x40;
    }
    if ((var_v0_2 != 0) && (D_8015F4DC != 0)) {
        func_800AE090(&*(s32 *)((unsigned char *)(&D_8015EE04[(D_8015F4D8 * 4) + D_8015F4DC]) + (-1)), " ", 1);
        D_8015F4DC -= 1;
    }
}
