extern u8 D_800E6377;

void func_8014A360(s32 arg0) {
    u8 var_a1;

    if (D_800E6377 != 0) {
        if (D_80150E98 != 0) {
            if (D_80150E98 != 1) {
                if (D_80150E98 == 2) {
                    if (func_8014A2C4(arg0, (s32) D_800E6377) < 2) {
                        var_a1 = 0x20;
                    } else if ((func_8014A2C4(M2C_ERROR(/* Read from unset register $a0 */), M2C_ERROR(/* Read from unset register $a1 */)) == 2) && (func_8014A32C() != 0)) {
                        var_a1 = 0x20;
                    } else {
                        var_a1 = 0xA0;
                    }
                } else {
                    goto block_16;
                }
            } else if (func_8014A2C4(arg0, (s32) D_800E6377) < 2) {
                var_a1 = 0x10;
            } else if ((func_8014A2C4(M2C_ERROR(/* Read from unset register $a0 */), M2C_ERROR(/* Read from unset register $a1 */)) == 2) && (func_8014A32C() != 0)) {
                var_a1 = 0x10;
            } else {
                var_a1 = 0x90;
            }
        } else {
block_16:
            if (func_8014A2C4(arg0, (s32) D_800E6377) < 2) {
                var_a1 = 8;
            } else if ((func_8014A2C4(M2C_ERROR(/* Read from unset register $a0 */), M2C_ERROR(/* Read from unset register $a1 */)) == 2) && (func_8014A32C() != 0)) {
                var_a1 = 8;
            } else {
                var_a1 = 0x88;
            }
        }
    } else if (D_80150E98 != 0) {
        if (D_80150E98 != 1) {
            if (D_80150E98 == 2) {
                if (func_8014A2C4(arg0, (s32) D_800E6377) < 2) {
                    var_a1 = 4;
                } else {
                    var_a1 = 0x84;
                }
            } else {
                goto block_31;
            }
        } else if (func_8014A2C4(arg0, (s32) D_800E6377) < 2) {
            var_a1 = 2;
        } else {
            var_a1 = 0x82;
        }
    } else {
block_31:
        var_a1 = 0x81;
        if (func_8014A2C4(arg0, (s32) D_800E6377) < 2) {
            var_a1 = 1;
        }
    }
    D_800E6377 = var_a1;
    func_800AE0B0("shinro %x \n", var_a1);
}
