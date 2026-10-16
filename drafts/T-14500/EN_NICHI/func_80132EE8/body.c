void func_80132EE8(void) {
    s32 *var_v0_2;
    s32 temp_t0;
    s32 temp_t7;
    s32 temp_v1;
    s32 var_a0;
    s32 var_a1;
    u8 *var_v0;

    temp_t7 = D_80139AFC + 1;
    D_80139AFC = temp_t7;
    if ((D_80139AF4 >= 0x50) && (temp_t7 >= 0x1E)) {
        var_a0 = D_80139B04;
        var_a1 = D_80139B00;
        var_v0 = D_8013984C;
        do {
            temp_v1 = *var_v0;
            temp_t0 = temp_v1 & 0x7F;
            if (temp_v1 & 0x80) {
                *var_v0 = temp_t0;
                if (temp_t0 == 4) {
                    var_a0 -= 1;
                } else {
                    var_a1 -= 1;
                }
            }
            var_v0 += 0x30;
        } while (var_v0 != D_80139A2C);
        D_80139B00 = var_a1;
        D_80139B04 = var_a0;
        D_800E71FA = D_80121556;
        D_800E71FC = D_8012155A;
        func_80135A48((s32) D_8011ECF6, (s32) D_8011ECFA);
        func_801369F4(0x502);
        D_80139AF8 = 3;
    }
    if (D_80139AFC >= 0x3C) {
        D_800E71FA = D_80121556;
        D_800E71FC = D_8012155A;
        D_80139AFC = 0;
        D_80139AF8 = 0;
        func_801369F4(0x50D);
        if (*D_8013984C & 0x80) {
            *D_8013984C = 0;
        }
        var_v0_2 = &D_801398AC;
        if (D_8013987C & 0x80) {
            D_8013987C = 0;
        }
        do {
            if ((*(s32 *)var_v0_2) & 0x80) {
                (*(s32 *)var_v0_2) = 0;
            }
            if (*(s32 *)((unsigned char *)var_v0_2 + (48)) & 0x80) {
                *(s32 *)((unsigned char *)var_v0_2 + (48)) = 0;
            }
            if (*(s32 *)((unsigned char *)var_v0_2 + (96)) & 0x80) {
                *(s32 *)((unsigned char *)var_v0_2 + (96)) = 0;
            }
            if (*(s32 *)((unsigned char *)var_v0_2 + (144)) & 0x80) {
                *(s32 *)((unsigned char *)var_v0_2 + (144)) = 0;
            }
            var_v0_2 += 0xC0;
        } while (var_v0_2 != D_80139A2C);
    }
}
