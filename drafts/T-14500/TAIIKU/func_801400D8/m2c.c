extern ? D_80149DF4;

void func_801400D8(void) {
    u8 *sp3C;
    s32 sp38;
    u8 *sp34;
    s16 var_a2;
    s16 var_a3;
    s16 var_a3_2;
    u8 *var_a1;
    u8 *var_a1_2;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 *var_v0_3;
    u8 temp_a0;
    u8 temp_v1;
    u8 temp_v1_2;

    var_v0 = D_8011ECD0;
    do {
        var_v0 += 0x44;
        *(s8 *)((unsigned char *)var_v0 + (6463)) = 0xA4;
        *(s8 *)((unsigned char *)var_v0 + (6462)) = 0x40;
        *(s8 *)((unsigned char *)var_v0 + (6465)) = 0x80;
        *(s8 *)((unsigned char *)var_v0 + (6461)) = 3;
        *(s32 *)((unsigned char *)var_v0 + (6516)) = 0x01000000;
        *(s8 *)((unsigned char *)var_v0 + (6467)) = 0x80;
        *(s32 *)((unsigned char *)var_v0 + (6472)) = (s32) D_80149FE4;
        *(s32 *)((unsigned char *)var_v0 + (6476)) = (s32) D_8014A01C;
        *(? **)((unsigned char *)var_v0 + (6512)) = &D_80149DF4;
        *(s16 *)((unsigned char *)var_v0 + (6480)) = (s16) D_8014A054;
        *(s16 *)((unsigned char *)var_v0 + (6486)) = 0x1000;
        *(s16 *)((unsigned char *)var_v0 + (6488)) = 0x1000;
        *(s8 *)((unsigned char *)var_v0 + (6466)) = 2;
    } while ((u32) var_v0 < (u32) D_8011EEF0);
    var_a1 = D_801499D4;
    var_v0_2 = D_8011ECD0;
    var_a3 = -0x78;
    do {
        temp_v1 = *var_a1;
        *(s16 *)((unsigned char *)var_v0_2 + (6566)) = var_a3;
        *(s16 *)((unsigned char *)var_v0_2 + (6570)) = -8;
        switch (temp_v1) {                          /* switch 1; irregular */
        case 0:                                     /* switch 1 */
            *(s16 *)((unsigned char *)var_v0_2 + (6552)) = (s16) (D_80149940 * 7);
            break;
        case 1:                                     /* switch 1 */
            *(s16 *)((unsigned char *)var_v0_2 + (6552)) = 0x4D;
            *(s8 *)((unsigned char *)var_v0_2 + (6595)) = 0x1C;
            break;
        case 2:                                     /* switch 1 */
            *(s16 *)((unsigned char *)var_v0_2 + (6552)) = 0x4D;
            *(s8 *)((unsigned char *)var_v0_2 + (6595)) = 0x1D;
            break;
        case 3:                                     /* switch 1 */
            *(s16 *)((unsigned char *)var_v0_2 + (6552)) = 0x4D;
            break;
        default:                                    /* switch 1 */
            sp3C = var_v0_2;
            sp34 = var_a1;
            sp38 = (s32) var_a3;
            func_80046500();
            break;
        }
        var_a1 += 1;
        var_v0_2 += 0x44;
        var_a3 += 0x40;
    } while ((u32) var_a1 < (u32) &D_801499D8);
    var_a1_2 = D_801499D4;
    var_v0_3 = D_8011ECD0;
    var_a3_2 = -0x78;
    var_a2 = -0x38;
    do {
        temp_v1_2 = var_a1_2->unk0;
        *(s16 *)((unsigned char *)var_v0_3 + (6838)) = var_a3_2;
        *(s16 *)((unsigned char *)var_v0_3 + (6842)) = -8;
        *(s16 *)((unsigned char *)var_v0_3 + (6824)) = 0x58;
        switch (temp_v1_2) {                        /* switch 2; irregular */
        case 0:                                     /* switch 2 */
            break;
        case 1:                                     /* switch 2 */
            *(s8 *)((unsigned char *)var_v0_3 + (6867)) = 0x1C;
            break;
        case 2:                                     /* switch 2 */
            *(s8 *)((unsigned char *)var_v0_3 + (6867)) = 0x1D;
            break;
        case 3:                                     /* switch 2 */
            *(s8 *)((unsigned char *)var_v0_3 + (6867)) = 0x1E;
            break;
        }
        temp_a0 = *(u8 *)((unsigned char *)var_a1_2 + (1));
        *(s16 *)((unsigned char *)var_v0_3 + (6906)) = var_a2;
        *(s16 *)((unsigned char *)var_v0_3 + (6910)) = -8;
        *(s16 *)((unsigned char *)var_v0_3 + (6892)) = 0x58;
        switch (temp_a0) {                          /* switch 3; irregular */
        case 0:                                     /* switch 3 */
            break;
        case 1:                                     /* switch 3 */
            *(s8 *)((unsigned char *)var_v0_3 + (6935)) = 0x1C;
            break;
        case 2:                                     /* switch 3 */
            *(s8 *)((unsigned char *)var_v0_3 + (6935)) = 0x1D;
            break;
        case 3:                                     /* switch 3 */
            *(s8 *)((unsigned char *)var_v0_3 + (6935)) = 0x1E;
            break;
        }
        var_a2 += 0x80;
        var_v0_3 += 0x88;
        var_a3_2 += 0x80;
        var_a1_2 += 2;
    } while (var_a2 != 0xC8);
}
