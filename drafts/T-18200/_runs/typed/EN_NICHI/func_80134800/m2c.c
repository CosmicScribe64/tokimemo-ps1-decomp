void func_80134800(s32 arg0) {
    EnObj *sp2C;
    EnObj *temp_a1;
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

    temp_a1 = &D_8013984C[arg0];
    if (temp_a1->unk_24 <= 0) {
        sp2C = temp_a1;
        temp_v0 = func_800AE0D0();
        temp_v1 = temp_a1->unk_08;
        switch (temp_v1) {                          /* irregular */
        case 0:
            if ((temp_v0 % 100) < 0x50) {
                temp_a1->unk_28 = 0;
            } else {
                temp_a1->unk_28 = 1;
            }
            break;
        case 1:
            temp_hi = temp_v0 % 100;
            if (temp_hi < 0xA) {
                temp_a1->unk_28 = 0;
            } else if (temp_hi < 0x5A) {
                temp_a1->unk_28 = 1;
            } else {
block_16:
                temp_a1->unk_28 = 2;
            }
            break;
        case 2:
            if ((temp_v0 % 100) < 0x14) {
                temp_a1->unk_28 = 1;
            } else {
                goto block_16;
            }
            break;
        }
        sp2C = temp_a1;
        temp_v0_2 = func_800AE0D0();
        var_v1 = temp_a1->unk_2C;
        if ((var_v1 == 0) || (var_v1 == 1)) {
            temp_hi_2 = temp_v0_2 % 100;
            if (temp_hi_2 < 0x19) {
                temp_a1->unk_2C = 0;
            } else if (temp_hi_2 < 0x28) {
                temp_a1->unk_2C = 1;
            } else if (temp_hi_2 < 0x46) {
                temp_a1->unk_2C = 0x10;
            } else {
                temp_a1->unk_2C = 0x20;
            }
            var_v1 = temp_a1->unk_2C;
        } else if (var_v1 & 0x50) {
            temp_hi_3 = temp_v0_2 % 100;
            if (temp_hi_3 < 0x28) {
                temp_a1->unk_2C = 0;
            } else if (temp_hi_3 < 0x50) {
                temp_a1->unk_2C = 1;
            } else {
                temp_a1->unk_2C = 0x20;
            }
            var_v1 = temp_a1->unk_2C;
        } else if (var_v1 & 0xA0) {
            temp_hi_4 = temp_v0_2 % 100;
            if (temp_hi_4 < 0x28) {
                temp_a1->unk_2C = 0;
                var_v1 = 0;
            } else {
                var_v1 = 0x10;
                if (temp_hi_4 < 0x50) {
                    temp_a1->unk_2C = 1;
                    var_v1 = 1;
                } else {
                    temp_a1->unk_2C = 0x10;
                }
            }
        }
        if ((var_v1 & 0x10) || (var_v1 & 0x20)) {
            sp2C = temp_a1;
            temp_hi_5 = func_800AE0D0() % 100;
            if (temp_hi_5 < 0x21) {
                var_v1 = temp_a1->unk_2C | 1;
            } else if (temp_hi_5 < 0x42) {
                var_v1 = temp_a1->unk_2C | 2;
            } else {
                var_v1 = temp_a1->unk_2C | 4;
            }
            temp_a1->unk_2C = var_v1;
        }
        if ((var_v1 == 0) || (var_v1 == 1)) {
            sp2C = temp_a1;
            temp_t6 = (func_800AE0D0() % 25) + 0x28;
            temp_a1->unk_24 = temp_t6;
            if (temp_a1->unk_28 >= 4) {
                temp_a1->unk_24 = temp_t6 / 4;
            }
        } else if (var_v1 & 1) {
            temp_a1->unk_24 = 0x19;
        } else if (var_v1 & 2) {
            temp_a1->unk_24 = 0x32;
        } else if (var_v1 & 4) {
            temp_a1->unk_24 = 0x64;
        }
    }
}
