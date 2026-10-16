extern ? D_8015F4B0;

void func_80139FB8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_lo;
    s32 temp_s0;
    s32 temp_s1;
    s32 temp_s5;
    s32 temp_s5_2;
    s32 temp_s5_3;
    s32 temp_t1;
    s32 temp_t4;
    s32 temp_t7;
    s32 temp_t8;
    s32 temp_t9;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s2;
    s32 var_s3;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v0_3;
    s32 var_v0_4;
    s32 var_v0_5;

    temp_t7 = (s32) (D_8015F4E4 * -0x400) / 512;
    temp_s5 = (s32) (func_800A0070(temp_t7) * arg2) >> 0xC;
    temp_t9 = (s32) (func_800A0140(temp_t7) * arg2) >> 0xC;
    if (temp_t9 < -0x100) {
        var_v0 = 0;
    } else {
        var_v0 = (s32) ((temp_t9 << 0xC) + 0x100000) / 96;
    }
    func_80139424((s32) *(&D_8015F4B0 + ((s32) D_8015F4E0 % 40)), 2, 5, (s32) ((temp_t9 << 5) + 0x2800) / 160, temp_s5, (temp_t9 >> 2) + 0x32, var_v0, var_v0, 0);
    var_s2 = 1;
    var_s3 = 0x1000;
    do {
        temp_lo = var_s3 / 40;
        temp_v0 = temp_lo - D_8015F4E4;
        if (temp_v0 < 0x200) {
            var_s0 = (s32) (temp_v0 << 0xA) / 512;
        } else {
            var_s0 = (s32) ((temp_v0 * 2) + 0x800) / 3;
        }
        temp_s5_2 = (s32) (func_800A0070(var_s0) * arg2) >> 0xC;
        temp_t4 = (s32) (func_800A0140(var_s0) * arg2) >> 0xC;
        if (temp_t4 < -0x100) {
            var_v0_2 = 0;
        } else {
            var_v0_2 = (s32) ((temp_t4 << 0xC) + 0x100000) / 96;
        }
        func_80139424((s32) *(&D_8015F4B0 + ((s32) (D_8015F4E0 + var_s2) % 40)), 0, 5, (s32) ((temp_t4 << 5) + 0x2800) / 160, temp_s5_2, (temp_t4 >> 2) + 0x32, var_v0_2, var_v0_2, 0);
        temp_v1 = temp_lo + D_8015F4E4;
        if (temp_v1 < 0x200) {
            var_v0_3 = (s32) (temp_v1 << 0xA) / 512;
        } else {
            var_v0_3 = (s32) ((temp_v1 * 2) + 0x800) / 3;
        }
        temp_s0 = 0x1000 - var_v0_3;
        temp_s1 = (s32) (func_800A0070(temp_s0) * arg2) >> 0xC;
        temp_t1 = (s32) (func_800A0140(temp_s0) * arg2) >> 0xC;
        if (temp_t1 < -0x100) {
            var_v0_4 = 0;
        } else {
            var_v0_4 = (s32) ((temp_t1 << 0xC) + 0x100000) / 96;
        }
        func_80139424((s32) *(&D_8015F4B0 + ((s32) ((D_8015F4E0 - var_s2) + 0x28) % 40)), 0, 5, (s32) ((temp_t1 << 5) + 0x2800) / 160, temp_s1, (temp_t1 >> 2) + 0x32, var_v0_4, var_v0_4, 0);
        var_s2 += 1;
        var_s3 += 0x1000;
    } while (var_s2 != 0x14);
    temp_v0_2 = 0x800 - D_8015F4E4;
    if (temp_v0_2 < 0x200) {
        var_s0_2 = (s32) (temp_v0_2 << 0xA) / 512;
    } else {
        var_s0_2 = (s32) ((temp_v0_2 * 2) + 0x800) / 3;
    }
    temp_s5_3 = (s32) (func_800A0070(var_s0_2) * arg2) >> 0xC;
    temp_t8 = (s32) (func_800A0140(var_s0_2) * arg2) >> 0xC;
    if (temp_t8 < -0x100) {
        var_v0_5 = 0;
    } else {
        var_v0_5 = (s32) ((temp_t8 << 0xC) + 0x100000) / 96;
    }
    func_80139424((s32) *(&D_8015F4B0 + ((s32) (D_8015F4E0 + var_s2) % 40)), 0, 5, (s32) ((temp_t8 << 5) + 0x2800) / 160, temp_s5_3, (temp_t8 >> 2) + 0x32, var_v0_5, var_v0_5, 0);
}
