extern s32 D_8015EBA8;

void func_8013CC90(void) {
    s32 temp_t7;
    s32 temp_t8;
    s32 var_v1;
    s32 var_v1_2;

    func_8014BB80();
    if (func_801445E8(0) < D_8015ED9C) {
        D_8015ED98 |= 0x10000;
        D_8015ED9C = 0;
    }
    if (!(D_8015ED98 & 0xFF) && !(D_8015EDC4 & 0xFF) && !(D_8015EE1C & 0xFF) && !(D_8015EDF0 & 0xFF) && (((u32) D_8015ED98 >> 0x18) != 0)) {
        func_8013D030();
        temp_t8 = D_8015ED98 & 0xFF000000;
        switch (temp_t8) {                          /* switch 1; irregular */
        default:                                    /* switch 1 */
            var_v1 = D_8015EBFC.unk_34;
            break;
        case 0x1000000:                             /* switch 1 */
            D_8015ED98 = 1;
            if (D_8015EB98 == 0x80000000) {
                D_8015EE00.unk_00 += 0x258;
            }
            var_v1 = D_8015EBFC.unk_34 + func_8013E9A8(0xF) + 0x1E;
            break;
        case 0x2000000:                             /* switch 1 */
            D_8015ED98 = 0x100;
            var_v1 = D_8015EBFC.unk_34 + func_8013E9A8(0xF) + 0x14;
            break;
        case 0x4000000:                             /* switch 1 */
            D_8015ED98 = 2;
            D_8015EBA8 = 0;
            var_v1 = D_8015EBFC.unk_00 - 0x64;
            break;
        }
        if (var_v1 >= 0xAB) {
            var_v1 = 0xAA;
        }
        D_8015EBFC.unk_34 = var_v1;
        if ((var_v1 >= 0x64) && (D_8015EBA4 == 1)) {
            D_8015EBA8 = 1;
        }
    }
    temp_t7 = D_8015ED98 & 0x1FF;
    var_v1_2 = temp_t7;
    switch (temp_t7) {                              /* switch 2; irregular */
    case 0x100:                                     /* switch 2 */
        func_8013AD50();
        var_v1_2 = D_8015ED98 & 0x1FF;
        break;
    case 0x104:                                     /* switch 2 */
        func_8013AD50();
        var_v1_2 = D_8015ED98 & 0x1FF;
        break;
    case 0x1:                                       /* switch 2 */
        func_8013AF1C();
        var_v1_2 = D_8015ED98 & 0x1FF;
        break;
    case 0x2:                                       /* switch 2 */
        func_8013D0D4();
        var_v1_2 = D_8015ED98 & 0x1FF;
        break;
    }
    if ((var_v1_2 == 0) && !(D_8015EDC4 & 0xFF) && !(D_8015EDF0 & 0xFF) && !(D_8015EE1C & 0xFF) && (!(D_800E6280.unk_F80 & 4) || !(D_800E6280.unk_F80 & 8) || !(D_8015EB98 & 0xF0000))) {
        func_8013D030();
        var_v1_2 = D_8015ED98 & 0x1FF;
    }
    if (var_v1_2 == 0) {
        if (!(D_8015EDC4 & 0xFF) && !(D_8015EE1C & 0xFF)) {
            if (D_8015EB98 & 0xF0000) {
                if ((D_800E6280.unk_F80 & 4) && (D_800E6280.unk_F80 & 8)) {
                    if (*(s16 *)((unsigned char *)D_8011ECD0 + (9814)) != 2) {
                        func_8013E7C0(0x30, 2, 1, 1);
                    }
                } else {
                    func_8013D030();
                }
            }
        }
    }
}
