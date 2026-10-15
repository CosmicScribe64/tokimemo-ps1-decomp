void func_801339D4(s32 arg0) {
    void *sp28;
    s32 temp_a0;
    s32 temp_hi;
    s32 temp_t7;
    s32 temp_t8;
    s32 var_a0;
    s32 var_t3;
    s32 var_t8;
    u32 temp_t9;
    void *temp_v1;

    temp_v1 = (arg0 * 0x30) + D_8013984C;
    *(s32 *)((unsigned char *)temp_v1 + (36)) = (s32) (*(s32 *)((unsigned char *)temp_v1 + (36)) - 1);
    if (temp_v1->unk0 == 4) {
        sp28 = temp_v1;
        *(s32 *)((unsigned char *)temp_v1 + (20)) = (s32) (*(s32 *)((unsigned char *)temp_v1 + (20)) + ((s32) (func_800A0140(*(s32 *)((unsigned char *)temp_v1 + (28))) << 0xF) >> 0xC));
        var_t8 = *(s32 *)((unsigned char *)temp_v1 + (24)) + ((s32) (func_800A0070(*(s32 *)((unsigned char *)temp_v1 + (28))) << 0xF) >> 0xC);
    } else {
        temp_t9 = *(u32 *)((unsigned char *)temp_v1 + (40));
        switch (temp_t9) {                          /* switch 1 */
        case 0:                                     /* switch 1 */
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0xC000;
            break;
        case 1:                                     /* switch 1 */
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0xF000;
            break;
        case 2:                                     /* switch 1 */
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0x12000;
            break;
        case 3:                                     /* switch 1 */
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0x15000;
            break;
        case 4:                                     /* switch 1 */
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0x18000;
            break;
        case 5:                                     /* switch 1 */
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0x1B000;
            break;
        case 6:                                     /* switch 1 */
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0x1E000;
            break;
        case 7:                                     /* switch 1 */
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0x21000;
            break;
        case 8:                                     /* switch 1 */
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0x24000;
            break;
        }
        temp_a0 = *(s32 *)((unsigned char *)temp_v1 + (44));
        if (temp_a0 == 1) {
            temp_hi = (s32) *(s32 *)((unsigned char *)temp_v1 + (36)) % 40;
            if (temp_hi < 0xA) {
                *(s32 *)((unsigned char *)temp_v1 + (28)) = (s32) (*(s32 *)((unsigned char *)temp_v1 + (28)) + 0x1E);
                goto block_20;
            }
            if (temp_hi >= 0x14) {
                var_t3 = temp_a0 & 0x10;
                if (temp_hi < 0x1E) {
                    *(s32 *)((unsigned char *)temp_v1 + (28)) = (s32) (*(s32 *)((unsigned char *)temp_v1 + (28)) - 0x1E);
                    goto block_20;
                }
            } else {
                goto block_20;
            }
        } else {
block_20:
            var_t3 = *(s32 *)((unsigned char *)temp_v1 + (44)) & 0x10;
        }
        if (var_t3 != 0) {
            *(s32 *)((unsigned char *)temp_v1 + (28)) = (s32) (*(s32 *)((unsigned char *)temp_v1 + (28)) + 0x14);
        }
        if (*(s32 *)((unsigned char *)temp_v1 + (44)) & 0x20) {
            *(s32 *)((unsigned char *)temp_v1 + (28)) = (s32) (*(s32 *)((unsigned char *)temp_v1 + (28)) - 0x14);
        }
        switch (*(s32 *)((unsigned char *)temp_v1 + (44))) { /* switch 2; irregular */
        case 0x40:                                  /* switch 2 */
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0xC000;
            *(s32 *)((unsigned char *)temp_v1 + (28)) = (s32) (*(s32 *)((unsigned char *)temp_v1 + (28)) + 0x100);
            break;
        case 0x41:                                  /* switch 2 */
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0xC000;
            *(s32 *)((unsigned char *)temp_v1 + (28)) = (s32) (*(s32 *)((unsigned char *)temp_v1 + (28)) + 0x200);
            break;
        case 0x80:                                  /* switch 2 */
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0xC000;
            *(s32 *)((unsigned char *)temp_v1 + (28)) = (s32) (*(s32 *)((unsigned char *)temp_v1 + (28)) - 0x100);
            break;
        case 0x81:                                  /* switch 2 */
            *(s32 *)((unsigned char *)temp_v1 + (32)) = 0xC000;
            *(s32 *)((unsigned char *)temp_v1 + (28)) = (s32) (*(s32 *)((unsigned char *)temp_v1 + (28)) - 0x200);
            break;
        }
        var_a0 = *(s32 *)((unsigned char *)temp_v1 + (28));
        temp_t7 = var_a0 - 0x1000;
        if (var_a0 >= 0x1000) {
            *(s32 *)((unsigned char *)temp_v1 + (28)) = temp_t7;
            var_a0 = temp_t7;
        }
        temp_t8 = var_a0 + 0x1000;
        if (var_a0 < 0) {
            *(s32 *)((unsigned char *)temp_v1 + (28)) = temp_t8;
            var_a0 = temp_t8;
        }
        sp28 = temp_v1;
        *(s32 *)((unsigned char *)temp_v1 + (20)) = (s32) (*(s32 *)((unsigned char *)temp_v1 + (20)) + ((s32) (*(s32 *)((unsigned char *)temp_v1 + (32)) * func_800A0140(var_a0)) >> 0xC));
        var_t8 = *(s32 *)((unsigned char *)temp_v1 + (24)) + ((s32) (*(s32 *)((unsigned char *)temp_v1 + (32)) * func_800A0070(*(s32 *)((unsigned char *)temp_v1 + (28)))) >> 0xC);
    }
    *(s32 *)((unsigned char *)temp_v1 + (24)) = var_t8;
}
