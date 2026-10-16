void func_8013BBF8(void) {
    RpgWordTbl10 sp58;
    RpgWordTbl10 sp30;
    s32 temp_t8;
    s32 temp_v0;
    s32 var_a0;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    s32 var_v1_4;
    s32 var_v1_5;

    sp58 = D_8015E550;
    sp30 = D_8015E578;
    temp_v0 = func_80043010();
    D_8015EBEC = temp_v0;
    if ((temp_v0 != D_8015EBF0) && (D_8015EBF0 = temp_v0, ((temp_v0 & 0xFFFF) != 0))) {
        if (temp_v0 == sp58.v[D_8015EBF8]) {
            var_v1 = D_8015EBF8 + 1;
        } else {
            var_v1 = 0;
        }
        if (temp_v0 == sp30.v[D_8015EBF4]) {
            var_a0 = D_8015EBF4 + 1;
        } else {
            var_a0 = 0;
        }
        D_8015EBF8 = var_v1;
        if (var_v1 == 0xA) {
            D_8015EBF8 = 0;
            if (D_8015EB98 != 0x80000000) {
                if ((D_8015EB98 != 0x10000) && (D_8015EB98 != 0x20000) && (D_8015EB98 != 0x40000)) {
                    if (D_8015EB98 != 0x100000) {
                        if (D_8015EB98 == 0x08000000) {
                            D_8015EDC0 = 1;
                            goto block_16;
                        }
                    } else {
                        var_v1_2 = D_8015ED94;
                        if ((var_v1_2 > 0) && (var_v1_2 < 0xA)) {
                            D_8015EDA0 = 0x1661;
                            var_v1_2 = 0x1661;
                        }
                        D_8015ED94 = var_v1_2;
                        goto block_22;
                    }
                } else {
block_16:
                    var_v1_3 = 0x1662;
                    if (D_8015ED94 >= 4) {
                        D_8015EDA0 = 0x1662;
                        goto block_28;
                    }
                }
            } else {
block_22:
                var_v1_4 = D_8015ED94;
                if ((var_v1_4 >= 2) && (var_v1_4 < 0xA)) {
                    D_8015EDA0 = 0x1661;
                    var_v1_4 = 0x1661;
                }
                D_8015ED94 = var_v1_4;
                if (var_v1_4 >= 0xA) {
                    temp_t8 = D_8015EDA0 + 1;
                    D_8015EDA0 = temp_t8;
                    D_8015ED94 = var_v1_4 + 1;
                    var_v1_3 = 0x270F;
                    if (temp_t8 >= 0x2710) {
                        D_8015EDA0 = 0x270F;
block_28:
                        D_8015ED94 = var_v1_3;
                    }
                }
            }
        }
        D_8015EBF4 = var_a0;
        if (var_a0 == 0xA) {
            D_8015EBF4 = 0;
            var_v1_5 = 2;
            switch (D_8015EB98) {
            default:
                var_v1_5 = 1;
                switch (D_8015EB98) {
                case 0x8000000:
                    break;
                case 0x100000:
block_39:
                    D_8015ED94 = var_v1_5;
                    break;
                }
                break;
            case 0x80000000:
                goto block_39;
            }
        }
    } else {
        ;
    }
}
