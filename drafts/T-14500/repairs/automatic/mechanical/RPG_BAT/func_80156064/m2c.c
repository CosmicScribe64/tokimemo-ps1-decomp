extern ? D_8011F110;
extern s16 D_801213F6;
extern s16 D_801213F8;
extern s16 D_80121402;
extern s16 D_80121406;

void func_80156064(void) {
    s16 temp_v0;
    s16 temp_v0_3;
    s32 *var_s1;
    s32 *var_s1_2;
    s32 temp_s3;
    s32 temp_t4;
    s32 temp_v0_2;
    s32 var_s2;
    u8 *var_s0;
    u8 *var_s0_2;
    u8 *var_s0_3;

    func_8013F0F4(0x50F, 0, 1);
    var_s1 = &D_8015EBBC;
    switch (D_8015EDB0.unk_00) {                    /* switch 1; irregular */
    case 0:                                         /* switch 1 */
        var_s0 = D_8011EDE0;
        do {
            var_s0 += 0x110;
            *(s32 *)((unsigned char *)var_s1 + (204)) = 0;
            *(s8 *)((unsigned char *)var_s0 + (6325)) = 5;
            *(s32 *)((unsigned char *)var_s1 + (208)) = 0;
            *(s8 *)((unsigned char *)var_s0 + (6393)) = 5;
            *(s32 *)((unsigned char *)var_s1 + (212)) = 0;
            *(s8 *)((unsigned char *)var_s0 + (6461)) = 5;
            var_s1 += 0x10;
            *(s32 *)((unsigned char *)var_s1 + (184)) = 0;
            *(s8 *)((unsigned char *)var_s0 + (6257)) = 5;
        } while (var_s0 != &D_8011F110);
        func_8014EBD0();
        return;
    case 1:                                         /* switch 1 */
        D_80121406 -= 2;
        temp_v0 = (D_8015EDB4 << 6) + 0x1000;
        D_801213F6 = temp_v0;
        D_801213F8 = temp_v0;
        D_8015EDB4 += 1;
        var_s0_2 = D_8011EDE0;
        var_s1_2 = &D_8015EBBC;
        var_s2 = 4;
        do {
            if ((*(s32 *)((unsigned char *)var_s1_2 + (200)) == 0) && (func_8013E9A8(0x2D) == 0)) {
                *(s32 *)((unsigned char *)var_s1_2 + (200)) = 1;
                temp_s3 = (func_8013E9A8(0x80) + D_80121402) - 0x40;
                func_8013E97C(var_s2, temp_s3 + 0xA0, (D_80121406 - func_8013E9A8(0x28)) + 0x78);
                func_8013E810(var_s2, func_8013E9A8(2) + 8, 1, 1);
            }
            temp_v0_2 = *(s32 *)((unsigned char *)var_s1_2 + (200));
            temp_t4 = temp_v0_2 + 1;
            if (temp_v0_2 != 0) {
                *(s32 *)((unsigned char *)var_s1_2 + (200)) = temp_t4;
                if (temp_t4 >= 0x1F) {
                    switch (temp_t4) {              /* switch 2 */
                    case 31:                        /* switch 2 */
                        func_8013E810(var_s2, 0xFF, 1, 0);
                        break;
                    case 32:                        /* switch 2 */
                        func_8013E810(var_s2, 0xFF, 1, 1);
                        break;
                    case 33:                        /* switch 2 */
                        func_8013E810(var_s2, 0xFF, 1, 0);
                        break;
                    case 34:                        /* switch 2 */
                        func_8013E810(var_s2, 0xFF, 1, 1);
                        break;
                    case 35:                        /* switch 2 */
                        func_8013E810(var_s2, 0xFF, 1, 0);
                        break;
                    case 36:                        /* switch 2 */
                        func_8013E810(var_s2, 0xFF, 1, 1);
                        break;
                    case 37:                        /* switch 2 */
                        func_8013E810(var_s2, 0xFF, 1, 0);
                        break;
                    }
                }
            }
            *(s16 *)((unsigned char *)var_s0_2 + (6570)) = (s16) (*(s16 *)((unsigned char *)var_s0_2 + (6570)) + 1);
            var_s2 += 1;
            temp_v0_3 = (D_8015EDB4 << 6) + 0x1000;
            var_s1_2 += 4;
            var_s0_2 += 0x44;
            *(s16 *)((unsigned char *)var_s0_2 + (6486)) = temp_v0_3;
            *(s16 *)((unsigned char *)var_s0_2 + (6488)) = temp_v0_3;
        } while (var_s2 < 0x10);
        if (D_80121406 < -0xC0) {
            func_8013E810(0x33, 0xFF, 1, 0);
            var_s0_3 = D_8011EDE0;
            do {
                var_s0_3 += 0x110;
                *(s16 *)((unsigned char *)var_s0_3 + (6350)) = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6352)) = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6418)) = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6420)) = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6486)) = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6488)) = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6282)) = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6284)) = 0x1000;
            } while (var_s0_3 != &D_8011F110);
            D_801213F6 = 0x1000;
            D_801213F8 = 0x1000;
            func_8014EBA8();
        }
        return;
    }
}
