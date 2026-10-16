void func_8005938C(void) {
    s16 spF8;
    s16 spF6;
    s16 spF4;
    s32 spEC;
    s32 spE8;
    s32 spE4;
    s32 spE0;
    s32 spDC;
    s32 spD8;
    s32 spC4[8];
    s32 spA4[8];
    s32 sp84[1];
    s32 sp64[1];
    s16 temp_v0;
    s16 temp_v0_2;
    s16 temp_v0_3;
    u8 *temp_s4;
    u8 *var_s0;

    if (D_800E731C < 0xA0) {
        var_s0 = (D_800E731C * 0x44) + D_8011ECD0;
        do {
            if (((*(s32 *)var_s0) != 0) && (*(u8 *)((unsigned char *)var_s0 + (1)) & 0x80)) {
                spF8 = *(s16 *)((unsigned char *)var_s0 + (104));
                spF4 = *(s16 *)((unsigned char *)var_s0 + (100));
                spF6 = *(s16 *)((unsigned char *)var_s0 + (102));
                (*(s32 *)&spC4) = (s32) (*(s32 *)D_80125C60);
                *(s32 *)((unsigned char *)(&spC4) + (8)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (8));
                *(s32 *)((unsigned char *)(&spC4) + (16)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (16));
                *(s32 *)((unsigned char *)(&spC4) + (4)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (4));
                *(s32 *)((unsigned char *)(&spC4) + (24)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (24));
                *(s32 *)((unsigned char *)(&spA4) + (4)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (4));
                (*(s32 *)&spA4) = (s32) (*(s32 *)D_80125C60);
                *(s32 *)((unsigned char *)(&spC4) + (12)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (12));
                *(s32 *)((unsigned char *)(&spA4) + (8)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (8));
                *(s32 *)((unsigned char *)(&spA4) + (12)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (12));
                *(s32 *)((unsigned char *)(&spC4) + (20)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (20));
                *(s32 *)((unsigned char *)(&spA4) + (20)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (20));
                *(s32 *)((unsigned char *)(&spA4) + (16)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (16));
                temp_s4 = var_s0 + 0x18;
                *(s32 *)((unsigned char *)(&spC4) + (28)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (28));
                *(s32 *)((unsigned char *)(&spA4) + (24)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (24));
                *(s32 *)((unsigned char *)(&spA4) + (28)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (28));
                RotMatrix((s32) &spF4, (s32) &spC4);
                temp_v0 = *(s16 *)((unsigned char *)var_s0 + (108));
                if (temp_v0 != 0) {
                    spE4 = (s32) temp_v0;
                } else {
                    spE4 = 0x1000;
                }
                temp_v0_2 = *(s16 *)((unsigned char *)var_s0 + (110));
                if (temp_v0_2 != 0) {
                    spE8 = (s32) temp_v0_2;
                } else {
                    spE8 = 0x1000;
                }
                temp_v0_3 = *(s16 *)((unsigned char *)var_s0 + (112));
                if (temp_v0_3 != 0) {
                    spEC = (s32) temp_v0_3;
                } else {
                    spEC = 0x1000;
                }
                ScaleMatrix((s32) &spA4, (s32) &spE4);
                MulMatrix((s32) &spC4, (s32) &spA4);
                if (*(u8 *)((unsigned char *)var_s0 + (1)) & 0x10) {
                    *(s16 *)((unsigned char *)var_s0 + (116)) = (s16) (*(s16 *)((unsigned char *)var_s0 + (116)) + *(s16 *)((unsigned char *)var_s0 + (124)));
                    *(s16 *)((unsigned char *)var_s0 + (118)) = (s16) (*(s16 *)((unsigned char *)var_s0 + (118)) + *(s16 *)((unsigned char *)var_s0 + (126)));
                    *(s16 *)((unsigned char *)var_s0 + (120)) = (s16) (*(s16 *)((unsigned char *)var_s0 + (120)) + *(s16 *)((unsigned char *)var_s0 + (128)));
                }
                spD8 = (s32) *(s16 *)((unsigned char *)var_s0 + (116));
                spDC = (s32) *(s16 *)((unsigned char *)var_s0 + (118));
                spE0 = (s32) *(s16 *)((unsigned char *)var_s0 + (120));
                *(s32 *)((unsigned char *)var_s0 + (24)) = (s32) (*(s32 *)&spC4);
                *(s32 *)((unsigned char *)temp_s4 + (4)) = (s32) *(s32 *)((unsigned char *)(&spC4) + (4));
                *(s32 *)((unsigned char *)temp_s4 + (8)) = (s32) *(s32 *)((unsigned char *)(&spC4) + (8));
                *(s32 *)((unsigned char *)temp_s4 + (12)) = (s32) *(s32 *)((unsigned char *)(&spC4) + (12));
                *(s32 *)((unsigned char *)temp_s4 + (16)) = (s32) *(s32 *)((unsigned char *)(&spC4) + (16));
                *(s32 *)((unsigned char *)temp_s4 + (20)) = (s32) *(s32 *)((unsigned char *)(&spC4) + (20));
                *(s32 *)((unsigned char *)temp_s4 + (24)) = (s32) *(s32 *)((unsigned char *)(&spC4) + (24));
                *(s32 *)((unsigned char *)temp_s4 + (28)) = (s32) *(s32 *)((unsigned char *)(&spC4) + (28));
                *(s32 *)((unsigned char *)var_s0 + (20)) = 0;
                func_80099B10(*(s32 *)((unsigned char *)var_s0 + (8)), (s32) &sp64, (s32) &sp84);
                func_80099F04((s32) &sp64);
                func_8009A200(*(s32 *)((unsigned char *)var_s0 + (8)), (s32) &sp84);
                func_80099ED8((s32) &sp84);
                func_8009A4D0((s32) (var_s0 + 4), (s32) ((D_8011ECA0 * 0x14) + &D_800E8C70), 6, 0x1F800000);
            }
            var_s0 += 0x88;
        } while ((u32) var_s0 < (u32) D_80121750);
    }
}
