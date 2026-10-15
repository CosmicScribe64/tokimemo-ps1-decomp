void func_80134800(s32 arg0) {
    void *sp2C;
    s32 temp_hi;
    s32 temp_hi_2;
    s32 temp_hi_3;
    s32 temp_hi_4;
    s32 temp_hi_5;
    s32 temp_t6;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_v1;
    void *temp_a1;

    temp_a1 = (arg0 * 0x30) + D_8013984C;
    if (*(s32 *)((unsigned char *)temp_a1 + (36)) <= 0) {
        sp2C = temp_a1;
        temp_v0 = func_800AE0D0();
        temp_v1 = *(s32 *)((unsigned char *)temp_a1 + (8));
        switch (temp_v1) {                          /* irregular */
        case 0:
            if ((temp_v0 % 100) < 0x50) {
                *(s32 *)((unsigned char *)temp_a1 + (40)) = 0;
            } else {
                *(s32 *)((unsigned char *)temp_a1 + (40)) = 1;
            }
            break;
        case 1:
            temp_hi = temp_v0 % 100;
            if (temp_hi < 0xA) {
                *(s32 *)((unsigned char *)temp_a1 + (40)) = 0;
            } else if (temp_hi < 0x5A) {
                *(s32 *)((unsigned char *)temp_a1 + (40)) = 1;
            } else {
block_16:
                *(s32 *)((unsigned char *)temp_a1 + (40)) = 2;
            }
            break;
        case 2:
            if ((temp_v0 % 100) < 0x14) {
                *(s32 *)((unsigned char *)temp_a1 + (40)) = 1;
            } else {
                goto block_16;
            }
            break;
        }
        sp2C = temp_a1;
        temp_v0_2 = func_800AE0D0();
        var_v1 = *(s32 *)((unsigned char *)temp_a1 + (44));
        if ((var_v1 == 0) || (var_v1 == 1)) {
            temp_hi_2 = temp_v0_2 % 100;
            if (temp_hi_2 < 0x19) {
                *(s32 *)((unsigned char *)temp_a1 + (44)) = 0;
            } else if (temp_hi_2 < 0x28) {
                *(s32 *)((unsigned char *)temp_a1 + (44)) = 1;
            } else if (temp_hi_2 < 0x46) {
                *(s32 *)((unsigned char *)temp_a1 + (44)) = 0x10;
            } else {
                *(s32 *)((unsigned char *)temp_a1 + (44)) = 0x20;
            }
            var_v1 = *(s32 *)((unsigned char *)temp_a1 + (44));
        } else if (var_v1 & 0x50) {
            temp_hi_3 = temp_v0_2 % 100;
            if (temp_hi_3 < 0x28) {
                *(s32 *)((unsigned char *)temp_a1 + (44)) = 0;
            } else if (temp_hi_3 < 0x50) {
                *(s32 *)((unsigned char *)temp_a1 + (44)) = 1;
            } else {
                *(s32 *)((unsigned char *)temp_a1 + (44)) = 0x20;
            }
            var_v1 = *(s32 *)((unsigned char *)temp_a1 + (44));
        } else if (var_v1 & 0xA0) {
            temp_hi_4 = temp_v0_2 % 100;
            if (temp_hi_4 < 0x28) {
                *(s32 *)((unsigned char *)temp_a1 + (44)) = 0;
                var_v1 = 0;
            } else {
                var_v1 = 0x10;
                if (temp_hi_4 < 0x50) {
                    *(s32 *)((unsigned char *)temp_a1 + (44)) = 1;
                    var_v1 = 1;
                } else {
                    *(s32 *)((unsigned char *)temp_a1 + (44)) = 0x10;
                }
            }
        }
        if ((var_v1 & 0x10) || (var_v1 & 0x20)) {
            sp2C = temp_a1;
            temp_hi_5 = func_800AE0D0() % 100;
            if (temp_hi_5 < 0x21) {
                var_v1 = *(s32 *)((unsigned char *)temp_a1 + (44)) | 1;
            } else if (temp_hi_5 < 0x42) {
                var_v1 = *(s32 *)((unsigned char *)temp_a1 + (44)) | 2;
            } else {
                var_v1 = *(s32 *)((unsigned char *)temp_a1 + (44)) | 4;
            }
            *(s32 *)((unsigned char *)temp_a1 + (44)) = var_v1;
        }
        if ((var_v1 == 0) || (var_v1 == 1)) {
            sp2C = temp_a1;
            temp_t6 = (func_800AE0D0() % 25) + 0x28;
            *(s32 *)((unsigned char *)temp_a1 + (36)) = temp_t6;
            if (*(s32 *)((unsigned char *)temp_a1 + (40)) >= 4) {
                *(s32 *)((unsigned char *)temp_a1 + (36)) = (s32) (temp_t6 / 4);
            }
        } else if (var_v1 & 1) {
            *(s32 *)((unsigned char *)temp_a1 + (36)) = 0x19;
        } else if (var_v1 & 2) {
            *(s32 *)((unsigned char *)temp_a1 + (36)) = 0x32;
        } else if (var_v1 & 4) {
            *(s32 *)((unsigned char *)temp_a1 + (36)) = 0x64;
        }
    }
}
