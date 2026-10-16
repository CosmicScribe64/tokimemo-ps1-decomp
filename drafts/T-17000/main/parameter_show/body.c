void parameter_show(void) {
    s32 sp3C;
    DisplayActor44 *var_v0;
    DisplayActor44 *var_v0_2;
    s16 temp_v1;
    s16 temp_v1_2;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 var_t0;

    if ((u8) D_800E6280.unk_F6E != 0) {
        var_t0 = 4;
    } else {
        var_t0 = 8;
    }
    if (((DisplayActor44 *)D_8011ECD0)[32].unk_03 & 0x80) {
        sp3C = var_t0;
        func_80049A40(-0x84, -0x61, 0x108, 0x2A, 0xC, 0x30E00, 0);
        dtd_on(0xC);
        var_v0 = &((DisplayActor44 *)D_8011ECD0)[32];
        do {
            temp_v1 = var_v0->unk_2E;
            if (temp_v1 == 1) {
                temp_lo = 0x1000 / var_t0;
                var_v0->unk_1A -= temp_lo;
                var_v0->unk_08 += temp_lo;
                if (temp_lo >= var_v0->unk_1A) {
                    var_v0->unk_2E = 2;
                }
            } else if (temp_v1 == 2) {
                temp_lo_2 = 0x1000 / var_t0;
                var_v0->unk_1A += temp_lo_2;
                var_v0->unk_08 -= temp_lo_2;
                if (var_v0->unk_1A >= 0x1000) {
                    var_v0->unk_1A = 0x1000;
                    var_v0->unk_2E = 0;
                    var_v0->unk_08 = 0;
                }
            }
            var_v0 += 1;
        } while ((u32) var_v0 < (u32) (&((DisplayActor44 *)D_8011ECD0)[56]));
        var_v0_2 = &((DisplayActor44 *)D_8011ECD0)[56];
        do {
            temp_v1_2 = var_v0_2->unk_2E;
            if (temp_v1_2 == 1) {
                temp_lo_3 = 0x1000 / var_t0;
                *(s16 *)((unsigned char *)var_v0_2 + (28)) = (s16) (*(s16 *)((unsigned char *)var_v0_2 + (28)) - temp_lo_3);
                if (temp_lo_3 >= *(s16 *)((unsigned char *)var_v0_2 + (28))) {
                    var_v0_2->unk_2E = 2;
                }
            } else if (temp_v1_2 == 2) {
                *(s16 *)((unsigned char *)var_v0_2 + (28)) = (s16) (*(s16 *)((unsigned char *)var_v0_2 + (28)) + (0x1000 / var_t0));
                if (*(s16 *)((unsigned char *)var_v0_2 + (28)) >= 0x1000) {
                    *(s16 *)((unsigned char *)var_v0_2 + (28)) = 0x1000;
                    var_v0_2->unk_2E = 0;
                }
            }
            var_v0_2 += 1;
        } while (var_v0_2 != (&((DisplayActor44 *)D_8011ECD0)[59]));
    }
}
