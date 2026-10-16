extern s32 D_8015ECC4;

void func_80148714(void) {
    s32 *var_s1;
    s32 *var_v0_2;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_s1;
    s32 temp_s2;
    s32 temp_s2_2;
    s32 temp_t4;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a1;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_v0;
    u32 *var_v1;

    switch (D_8015EDB0.unk_00) {                    /* irregular */
    case 0:
        D_8015EC50 = D_8015EC28 - 0xB0;
        D_8015EC54 = D_8015EC2C - 0x58;
        if (D_8015EBAC == 1) {
            D_801215FD = 4;
            D_80121685 = 4;
        }
        if (D_8015EBAC == 0) {
            D_80121685 = 4;
        }
        func_8013F0F4(0x507, 1, 0);
        func_8014EBD0();
        return;
    case 1:
        var_a1 = D_8015EDB4;
        var_v0 = 0;
        if (var_a1 >= 0x3E8) {
            var_v0 = 1;
        }
        var_s0 = var_v0;
        if (var_v0 < 6) {
            var_s1 = &(&D_8015EBAC)[var_v0 * 2];
            do {
                temp_s2 = var_s0 + 0x32;
                temp_lo = (s32) (*(s32 *)((unsigned char *)var_s1 + (200)) * D_8015EC50) / 1000;
                D_8015EC68 = temp_lo;
                temp_lo_2 = (s32) (*(s32 *)((unsigned char *)var_s1 + (200)) * D_8015EC54) / 1000;
                D_8015EC6C = temp_lo_2;
                func_8013E97C(temp_s2, temp_lo, temp_lo_2);
                func_8013E7C0(temp_s2, 6, 1, 1);
                var_s0 += 1;
                var_s1 += 8;
            } while (var_s0 != 6);
            var_a1 = D_8015EDB4;
        }
        if (var_a1 < 0x3E8) {
            if (D_8015EBAC == 0) {
                var_a1 += 0x3C;
            } else {
                var_a1 += 0x28;
            }
            D_8015EC74 = var_a1;
        }
        var_v0_2 = &D_8015ED34;
        var_v1 = &D_8015ED38;
        do {
            temp_t4 = *var_v0_2;
            var_v0_2 -= 4;
            var_v1 -= 4;
            *(s32 *)((unsigned char *)var_v1 + (4)) = temp_t4;
        } while ((u32) &D_8015EC70 < (u32) var_v0_2);
        var_s0_2 = 1;
        if (D_8015ECC4 == D_8015EC74) {
            D_8015EDB4 = var_a1;
            var_a0 = 0x33;
            do {
                func_8013E7C0(var_a0, 6, 1, 0);
                var_s0_2 += 1;
                var_a0 = var_s0_2 + 0x32;
            } while (var_s0_2 != 6);
            var_a1 = D_8015EDB4;
        }
        if ((var_a1 >= 0x3E8) && (var_a1 < 0x7D0)) {
            D_8015EDB4 = var_a1;
            func_8013E97C(0x32, D_8015EC28, D_8015EC2C);
            func_8013E810(0x32, 7, 5, 1);
            var_a1 = 0x7D1;
        } else if (!(D_8012139A & 1)) {
            var_a1 = 0x3E8;
            D_8015EDB8 += 1;
        }
        D_8015EDB4 = var_a1;
        if (var_a1 >= 0x3E8) {
            func_8013F0F4(0x508, 0xA, 1);
            var_s0_3 = 0;
            do {
                if (func_8013E9A8(0x14) == 0) {
                    temp_s2_2 = func_8013E9A8(0x20);
                    temp_s1 = var_s0_3 + 0x28;
                    func_8013E97C(temp_s1, (temp_s2_2 + D_8015EC28) - 0x10, (func_8013E9A8(0x20) + D_8015EC2C) - 0x10);
                    func_8013E810(temp_s1, 8, 1, 1);
                }
                var_s0_3 += 1;
            } while (var_s0_3 != 4);
        }
        var_s0_4 = 0;
        if (D_8015EDB8 >= 8) {
            var_a0_2 = 0x28;
            do {
                func_8013E810(var_a0_2, 0xFF, 1, 0);
                var_s0_4 += 1;
                var_a0_2 = var_s0_4 + 0x28;
            } while (var_s0_4 != 4);
            func_8014EBA8();
        }
        return;
    }
}
