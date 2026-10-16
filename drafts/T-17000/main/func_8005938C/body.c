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
    MATRIX spC4;
    MATRIX spA4;
    s32 sp84[1];
    s32 sp64[1];
    DisplayActor44 *var_s0;
    s16 *temp_s4;
    s16 temp_v0;
    s16 temp_v0_2;
    s16 temp_v0_3;

    if (*(s32 *)((unsigned char *)(&D_800E6280) + (4252)) < 0xA0) {
        var_s0 = &((DisplayActor44 *)D_8011ECD0)[*(s32 *)((unsigned char *)(&D_800E6280) + (4252))];
        do {
            if ((var_s0->unk_00 != 0) && (var_s0->unk_01 & 0x80)) {
                spF8 = *(s16 *)((unsigned char *)var_s0 + (104));
                spF4 = *(s16 *)((unsigned char *)var_s0 + (100));
                spF6 = *(s16 *)((unsigned char *)var_s0 + (102));
                spC4 = *(MATRIX *)D_80125C60;
                spA4 = *(MATRIX *)D_80125C60;
                temp_s4 = &var_s0->unk_18;
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
                if (var_s0->unk_01 & 0x10) {
                    *(s16 *)((unsigned char *)var_s0 + (116)) = (s16) (*(s16 *)((unsigned char *)var_s0 + (116)) + *(s16 *)((unsigned char *)var_s0 + (124)));
                    *(s16 *)((unsigned char *)var_s0 + (118)) = (s16) (*(s16 *)((unsigned char *)var_s0 + (118)) + *(s16 *)((unsigned char *)var_s0 + (126)));
                    *(s16 *)((unsigned char *)var_s0 + (120)) = (s16) (*(s16 *)((unsigned char *)var_s0 + (120)) + *(s16 *)((unsigned char *)var_s0 + (128)));
                }
                spD8 = (s32) *(s16 *)((unsigned char *)var_s0 + (116));
                spDC = (s32) *(s16 *)((unsigned char *)var_s0 + (118));
                spE0 = (s32) *(s16 *)((unsigned char *)var_s0 + (120));
                *(MATRIX *)temp_s4 = spC4;
                *(s32 *)((unsigned char *)var_s0 + (20)) = 0;
                func_80099B10(*(s32 *)((unsigned char *)var_s0 + (8)), (s32) &sp64, (s32) &sp84);
                func_80099F04((s32) &sp64);
                func_8009A200(*(s32 *)((unsigned char *)var_s0 + (8)), (s32) &sp84);
                func_80099ED8((s32) &sp84);
                func_8009A4D0((s32) &var_s0->unk_04, (s32) ((D_8011ECA0 * 0x14) + &D_800E8C70), 6, 0x1F800000);
            }
            var_s0 += 2;
        } while ((u32) var_s0 < (u32) D_80121750);
    }
}
