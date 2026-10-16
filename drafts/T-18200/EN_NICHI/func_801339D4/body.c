void func_801339D4(s32 arg0) {
    EnObj *sp28;
    EnObj *temp_v1;
    s32 temp_a0;
    s32 temp_hi;
    s32 temp_t7;
    s32 temp_t8;
    s32 temp_t9;
    s32 var_a0;
    s32 var_t3;
    s32 var_t8;

    temp_v1 = &D_8013984C[arg0];
    temp_v1->unk_24 -= 1;
    if (temp_v1->unk_00 == 4) {
        sp28 = temp_v1;
        *(s32 *)((unsigned char *)temp_v1 + (20)) = (s32) (*(s32 *)((unsigned char *)temp_v1 + (20)) + ((s32) (func_800A0140(temp_v1->unk_1C) << 0xF) >> 0xC));
        var_t8 = *(s32 *)((unsigned char *)temp_v1 + (24)) + ((s32) (func_800A0070(temp_v1->unk_1C) << 0xF) >> 0xC);
    } else {
        temp_t9 = temp_v1->unk_28;
        switch (temp_t9) {
        case 0:
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0xC000;
            break;
        case 1:
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0xF000;
            break;
        case 2:
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0x12000;
            break;
        case 3:
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0x15000;
            break;
        case 4:
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0x18000;
            break;
        case 5:
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0x1B000;
            break;
        case 6:
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0x1E000;
            break;
        case 7:
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0x21000;
            break;
        case 8:
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0x24000;
            break;
        }
        temp_a0 = temp_v1->unk_2C;
        if (temp_a0 == 1) {
            temp_hi = (s32) temp_v1->unk_24 % 40;
            if (temp_hi < 0xA) {
                temp_v1->unk_1C += 0x1E;
                goto block_20;
            }
            if (temp_hi >= 0x14) {
                var_t3 = temp_a0 & 0x10;
                if (temp_hi < 0x1E) {
                    temp_v1->unk_1C -= 0x1E;
                    goto block_20;
                }
            } else {
                goto block_20;
            }
        } else {
block_20:
            var_t3 = temp_v1->unk_2C & 0x10;
        }
        if (var_t3 != 0) {
            temp_v1->unk_1C += 0x14;
        }
        if (temp_v1->unk_2C & 0x20) {
            temp_v1->unk_1C -= 0x14;
        }
        switch (temp_v1->unk_2C) {
        case 0x40:
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0xC000;
            temp_v1->unk_1C += 0x100;
            break;
        case 0x41:
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0xC000;
            temp_v1->unk_1C += 0x200;
            break;
        case 0x80:
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0xC000;
            temp_v1->unk_1C -= 0x100;
            break;
        case 0x81:
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0xC000;
            temp_v1->unk_1C -= 0x200;
            break;
        }
        var_a0 = temp_v1->unk_1C;
        temp_t7 = var_a0 - 0x1000;
        if (var_a0 >= 0x1000) {
            temp_v1->unk_1C = temp_t7;
            var_a0 = temp_t7;
        }
        temp_t8 = var_a0 + 0x1000;
        if (var_a0 < 0) {
            temp_v1->unk_1C = temp_t8;
            var_a0 = temp_t8;
        }
        sp28 = temp_v1;
        *(s32 *)((unsigned char *)temp_v1 + (20)) = (s32) (*(s32 *)((unsigned char *)temp_v1 + (20)) + ((s32) (*(s32 *)((unsigned char *)temp_v1 + (32)) * func_800A0140(var_a0)) >> 0xC));
        var_t8 = *(s32 *)((unsigned char *)temp_v1 + (24)) + ((s32) (*(s32 *)((unsigned char *)temp_v1 + (32)) * func_800A0070(temp_v1->unk_1C)) >> 0xC);
    }
    *(s32 *)((unsigned char *)temp_v1 + (24)) = var_t8;
}
