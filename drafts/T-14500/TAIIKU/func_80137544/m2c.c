? func_800863D4(?);                                 /* extern */
extern s32 D_800E7208;
extern s16 D_8012091E;
extern s8 D_80149209;
extern u8 D_8014920A;
extern u8 D_8014920F;

void func_80137544(void) {
    s16 var_a0;
    s16 var_a1;
    s16 var_v0;
    s16 var_v1;
    s32 temp_a3;
    s32 var_a2;
    s32 var_at;
    s32 var_at_2;
    s32 var_t2;

    func_800863D4(0x6A);
    if ((D_800B3CFC != 0) && (D_80122E94 == 0xFF)) {
        if (D_80122EA0 == 1) {
            func_80044750(0x60E);
        } else {
            func_80044750(0x50E);
        }
        D_80122E94 = 1;
    } else if (D_800E7208 & 0x20) {
        var_a2 = 0;
        if ((u8) D_8014920F >= 0x29U) {
            var_a0 = -0xA0;
            var_a1 = -0x80;
            if (D_80120922 < 0) {
                var_at = D_80120922 < -0x30;
                if (D_80120922 >= -0x30) {
                    var_t2 = 0;
                } else {
                    goto block_12;
                }
            } else {
                var_at = D_80120922 < -0x30;
block_12:
                if (var_at != 0) {
                    var_at_2 = D_80120922 < -0x40;
                    if (D_80120922 >= -0x40) {
                        var_t2 = 0xA;
                    } else {
                        goto block_16;
                    }
                } else {
                    var_at_2 = D_80120922 < -0x40;
block_16:
                    var_t2 = 0x80;
                    if ((var_at_2 != 0) && (D_80120922 >= -0x50)) {
                        var_t2 = 0x14;
                    }
                }
            }
            temp_a3 = D_8012091E + 0xB;
loop_20:
            var_v0 = var_a0;
            var_v1 = var_a1;
            if (var_t2 == 0xA) {
                var_v0 = var_a0 + 0x10;
                var_v1 = var_a1 + 0x10;
            }
            var_a0 += 0x20;
            if ((var_v0 < temp_a3) && (temp_a3 < var_v1)) {
                var_t2 = (var_t2 + var_a2) & 0xFF;
            } else {
                var_a2 += 1;
                var_a1 += 0x20;
                if (var_a2 != 0xA) {
                    goto loop_20;
                }
            }
            if ((D_80122E94 == 1) && (var_t2 == D_801491D8) && (D_801491DC != 0xFF)) {
                if (D_80122EA0 == 1) {
                    func_80044750(0x60C);
                } else {
                    func_80044750(0x50C);
                }
                D_80149209 = 1;
            } else if (var_t2 == D_8014920A) {
                func_80044750(0x601);
                D_80149209 = 1;
                D_80122E94 = 0xFF;
                D_801491DC = 0xFF;
            } else {
                func_80044750(0x600);
            }
            D_8014920F = 0;
        }
    }
}
