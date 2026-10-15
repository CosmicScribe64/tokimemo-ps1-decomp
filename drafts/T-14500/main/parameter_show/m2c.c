extern u8 D_800E71EE;
extern ? D_8011F550;
extern ? D_8011FBB0;
extern ? D_8011FC7C;

void parameter_show(void) {
    s32 sp3C;
    ? *var_v0;
    ? *var_v0_2;
    s16 temp_v1;
    s16 temp_v1_2;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 var_t0;

    if (D_800E71EE != 0) {
        var_t0 = 4;
    } else {
        var_t0 = 8;
    }
    if (D_8011F553 & 0x80) {
        sp3C = var_t0;
        func_80049A40(-0x84, -0x61, 0x108, 0x2A, 0xC, 0x30E00, 0);
        dtd_on(0xC);
        var_v0 = &D_8011F550;
        do {
            temp_v1 = *(s16 *)((unsigned char *)var_v0 + (46));
            if (temp_v1 == 1) {
                temp_lo = 0x1000 / var_t0;
                *(s16 *)((unsigned char *)var_v0 + (26)) = (s16) (*(s16 *)((unsigned char *)var_v0 + (26)) - temp_lo);
                *(s16 *)((unsigned char *)var_v0 + (8)) = (s16) (*(s16 *)((unsigned char *)var_v0 + (8)) + temp_lo);
                if (temp_lo >= *(s16 *)((unsigned char *)var_v0 + (26))) {
                    *(s16 *)((unsigned char *)var_v0 + (46)) = 2;
                }
            } else if (temp_v1 == 2) {
                temp_lo_2 = 0x1000 / var_t0;
                *(s16 *)((unsigned char *)var_v0 + (26)) = (s16) (*(s16 *)((unsigned char *)var_v0 + (26)) + temp_lo_2);
                *(s16 *)((unsigned char *)var_v0 + (8)) = (s16) (*(s16 *)((unsigned char *)var_v0 + (8)) - temp_lo_2);
                if (*(s16 *)((unsigned char *)var_v0 + (26)) >= 0x1000) {
                    *(s16 *)((unsigned char *)var_v0 + (26)) = 0x1000;
                    *(s16 *)((unsigned char *)var_v0 + (46)) = 0;
                    *(s16 *)((unsigned char *)var_v0 + (8)) = 0;
                }
            }
            var_v0 += 0x44;
        } while ((u32) var_v0 < (u32) &D_8011FBB0);
        var_v0_2 = &D_8011FBB0;
        do {
            temp_v1_2 = *(s16 *)((unsigned char *)var_v0_2 + (46));
            if (temp_v1_2 == 1) {
                temp_lo_3 = 0x1000 / var_t0;
                *(s16 *)((unsigned char *)var_v0_2 + (28)) = (s16) (*(s16 *)((unsigned char *)var_v0_2 + (28)) - temp_lo_3);
                if (temp_lo_3 >= *(s16 *)((unsigned char *)var_v0_2 + (28))) {
                    *(s16 *)((unsigned char *)var_v0_2 + (46)) = 2;
                }
            } else if (temp_v1_2 == 2) {
                *(s16 *)((unsigned char *)var_v0_2 + (28)) = (s16) (*(s16 *)((unsigned char *)var_v0_2 + (28)) + (0x1000 / var_t0));
                if (*(s16 *)((unsigned char *)var_v0_2 + (28)) >= 0x1000) {
                    *(s16 *)((unsigned char *)var_v0_2 + (28)) = 0x1000;
                    *(s16 *)((unsigned char *)var_v0_2 + (46)) = 0;
                }
            }
            var_v0_2 += 0x44;
        } while (var_v0_2 != &D_8011FC7C);
    }
}
