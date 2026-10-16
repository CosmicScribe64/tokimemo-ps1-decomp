extern s8 D_800B5A9C;
extern u8 D_800B5AA0;

void func_80065B0C(s32 a) {
    s32 sp4C;
    s8 sp4B;
    DisplayActor44 *var_s0;
    s16 *temp_s0;
    s16 temp_v0_3;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_at;
    s32 var_s1;
    s32 var_s2_2;
    s8 temp_s3;
    u8 *var_s3;
    u8 var_a0;
    u8 var_s2;
    void *temp_s0_2;
    void *temp_s0_3;

    temp_s3 = D_800E6280.unk_1093;
    var_s1 = 0;
    var_s2 = 0;
    sp4B = temp_s3;
    if (D_800E6280.unk_F80 & 1) {
        temp_v0 = D_800E6280.unk_F80 & 0xA000;
        if (((u8) D_800E6280.unk_F6C % 3U) == 2) {
            if (temp_v0 != 0x2000) {
                if (temp_v0 != 0x8000) {
                    var_a0 = D_800E6280.unk_F6F;
                } else {
                    var_a0 = D_800E6280.unk0;
                    var_s1 = -(s32) var_a0;
                }
            } else {
                var_a0 = D_800E6280.unk0;
                var_s1 = (s32) var_a0;
            }
            temp_v0_2 = D_800E6280.unk_F80 & 0x5000;
            switch (temp_v0_2) {                    /* irregular */
            case 0x1000:
                var_s2 = (u8) -(s32) var_a0;
                break;
            case 0x4000:
                var_s2 = var_a0;
                break;
            }
            if (func_80065964((s32) temp_s3, var_s1, (s32) var_s2) == 0) {
                var_s1 = 0;
                var_s2 = 0;
            }
            temp_s0 = D_80125C10 + ((s8) D_800E6280.unk0 * 4);
            *temp_s0 += var_s1;
            temp_s0_2 = D_80125C10 + ((s8) D_800E6280.unk0 * 4);
            *(s16 *)((unsigned char *)temp_s0_2 + (2)) = (s16) (*(s16 *)((unsigned char *)temp_s0_2 + (2)) + var_s2);
            D_8011ECD0->unk_26 += var_s1;
            D_8011ECD0->unk_2A += var_s2;
            func_800654D0((s32) (u8) D_800E6280.unk_F6D);
            temp_s0_3 = D_80125C10 + (D_800E6280.unk_1093 * 4);
            if (temp_s0_3->unk0 >= 0x81) {
                temp_s0_3->unk0 = 0x80;
            }
            if (temp_s0_3->unk0 < -0x8A) {
                temp_s0_3->unk0 = -0x8A;
            }
            temp_v0_3 = *(s16 *)((unsigned char *)temp_s0_3 + (2));
            var_at = temp_v0_3 < 0x5D;
            if (temp_v0_3 < -0x5C) {
                *(s16 *)((unsigned char *)temp_s0_3 + (2)) = -0x5C;
                var_at = *(s16 *)((unsigned char *)temp_s0_3 + (2)) < 0x5D;
            }
            if (var_at == 0) {
                *(s16 *)((unsigned char *)temp_s0_3 + (2)) = 0x5C;
            }
        }
    } else {
        menu_check(0, (s32) D_8011ECD0->unk_26, (s32) D_8011ECD0->unk_2A);
        func_8004FC10(0);
    }
    if ((u8) sp4B != D_800B5A9C) {
        D_800B5A9C = temp_s3;
        D_800B5AA0 = 0x20;
    } else if (D_800B5AA0 != 0) {
        D_800B5AA0 -= 1;
    }
    sp4C = 0xE;
    if (D_800E6280.unk0 == 0xF) {
        sp4C = 0xF;
    }
    if ((u8) D_800E6280.unk_F6C == 2) {
        sp4C = 0xF;
    }
    var_s2_2 = 0;
    if (sp4C > 0) {
        var_s0 = D_8011ECD0;
        var_s3 = D_80125C90;
        do {
            if (*var_s3 == 0) {
                *(s8 *)((unsigned char *)var_s0 + (5442)) = 0;
                *(s8 *)((unsigned char *)var_s0 + (4419)) = 0x51;
                *(s8 *)((unsigned char *)var_s0 + (4353)) = 3;
            } else {
                *(s8 *)((unsigned char *)var_s0 + (5442)) = 1;
                *(s8 *)((unsigned char *)var_s0 + (4353)) = 4;
                *(s8 *)((unsigned char *)var_s0 + (5441)) = 3;
                if (*(&D_800E6280.unk_F90[0].unk_06 + (var_s2_2 * 0x10)) != 0) {
                    *(s8 *)((unsigned char *)var_s0 + (4419)) = 0x52;
                    *(s8 *)((unsigned char *)var_s0 + (5441)) = 2;
                    *(s8 *)((unsigned char *)var_s0 + (4353)) = 3;
                    if (a == 0) {
                        *(s16 *)((unsigned char *)var_s0 + (5466)) = (s16) (rsin((0x20 - D_800B5AA0) << 6) + 0x1000);
                        *(s16 *)((unsigned char *)var_s0 + (5468)) = (s16) (rsin((0x20 - D_800B5AA0) << 6) + 0x1000);
                    } else {
                        *(s16 *)((unsigned char *)var_s0 + (5466)) = 0x1000;
                        goto block_42;
                    }
                } else {
                    *(s8 *)((unsigned char *)var_s0 + (4419)) = 0x53;
                    if (a == 0) {
                        *(s16 *)((unsigned char *)var_s0 + (5466)) = 0x1000;
                        *(s16 *)((unsigned char *)var_s0 + (5468)) = 0x1000;
                        *(s16 *)((unsigned char *)var_s0 + (4378)) = 0x1000;
                        *(s16 *)((unsigned char *)var_s0 + (4380)) = 0x1000;
                    } else {
                        *(s16 *)((unsigned char *)var_s0 + (5466)) = 0x1000;
block_42:
                        *(s16 *)((unsigned char *)var_s0 + (5468)) = 0x1000;
                    }
                }
            }
            var_s2_2 += 1;
            var_s3 += 1;
            var_s0 += 0x44;
        } while (var_s2_2 != sp4C);
    }
    if (D_800E6280.unk_035 != 0) {
        *(s16 *)((unsigned char *)D_8011ECD0 + (6346)) = 0xE;
        if (*(s16 *)((unsigned char *)D_8011ECD0 + (6348)) >= 4) {
            goto block_48;
        }
    } else {
        D_8011ECD0->unk0 = 0xD;
        if (*(s16 *)((unsigned char *)D_8011ECD0 + (6348)) >= 6) {
block_48:
            *(s16 *)((unsigned char *)D_8011ECD0 + (6348)) = 0;
        }
    }
}
