void func_8013358C(void) {
    EnObj *temp_v0_2;
    EnObj *temp_v0_3;
    EnObj *temp_v0_5;
    EnObj *temp_v0_6;
    s32 temp_a2;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_a3;
    s32 var_v0;
    u8 *var_t3;
    u8 temp_v0;
    u8 temp_v0_4;

    func_801330D0();
    var_v0 = D_80139AFC + 1;
    if (var_v0 >= 0x19) {
        temp_a2 = D_80139AF4 + 1;
        D_80139AF4 = temp_a2;
        var_v0 = 0;
        if (temp_a2 >= 0x50) {
            D_80139AFC = 0;
            D_80139AF8 = 3;
            return;
        }
    }
    D_80139AFC = var_v0;
    if (!(D_800E7200 & 0x20)) {
        D_80139AFC = var_v0;
        if (var_v0 >= 4) {
            func_80135A48((s32) D_8011ECF6, (s32) D_8011ECFA);
            func_801369F4(0x502);
            D_80139AF8 = 0;
            D_80121531 = 3;
            D_80139AFC = 0;
            func_801337E0();
            var_a0 = D_80139B04;
            var_a1 = D_80139B00;
            var_a2 = D_80139AF4;
            var_t3 = D_80139B08;
            var_a3 = 0;
            do {
                temp_v0 = (*(s32 *)var_t3);
                if (temp_v0 == 2) {
                    temp_v0_2 = &D_8013984C[var_a3];
                    temp_v1 = temp_v0_2->unk_00;
                    D_80139AF8 = 2;
                    if (temp_v1 == 4) {
                        var_a2 += 0x50;
                        var_a0 += 1;
                    } else {
                        var_a2 += 0x10;
                        var_a1 += 1;
                    }
                    temp_v0_2->unk_00 = temp_v1 | 0x80;
                } else if (temp_v0 == 1) {
                    temp_v0_3 = &D_8013984C[var_a3];
                    temp_v1_2 = temp_v0_3->unk_00;
                    D_80139AF8 = 2;
                    if (temp_v1_2 == 4) {
                        var_a0 += 1;
                    } else {
                        var_a1 += 1;
                    }
                    temp_v0_3->unk_00 = temp_v1_2 | 0x80;
                }
                temp_v0_4 = var_t3[1];
                if (temp_v0_4 == 2) {
                    temp_v0_5 = &D_8013984C[var_a3];
                    temp_v1_3 = *(s32 *)((unsigned char *)temp_v0_5 + (48));
                    D_80139AF8 = 2;
                    if (temp_v1_3 == 4) {
                        var_a2 += 0x50;
                        var_a0 += 1;
                    } else {
                        var_a2 += 0x10;
                        var_a1 += 1;
                    }
                    *(s32 *)((unsigned char *)temp_v0_5 + (48)) = (s32) (temp_v1_3 | 0x80);
                } else if (temp_v0_4 == 1) {
                    temp_v0_6 = &D_8013984C[var_a3];
                    temp_v1_4 = *(s32 *)((unsigned char *)temp_v0_6 + (48));
                    D_80139AF8 = 2;
                    if (temp_v1_4 == 4) {
                        var_a0 += 1;
                    } else {
                        var_a1 += 1;
                    }
                    *(s32 *)((unsigned char *)temp_v0_6 + (48)) = (s32) (temp_v1_4 | 0x80);
                }
                var_a3 += 2;
                var_t3 += 2;
            } while (var_a3 != 0xA);
            D_80139AF4 = var_a2;
            D_80139B00 = var_a1;
            D_80139B04 = var_a0;
        }
    }
}
