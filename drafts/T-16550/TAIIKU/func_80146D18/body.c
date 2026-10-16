s32 func_80146D18(void) {
    s32 sp48[1];
    TkS16x4 sp40;
    s32 *var_s0;
    DisplayActor44 *var_v0_2;
    DisplayActor44 *var_v0_3;
    TaiikuController *var_v1;
    TaiikuController *var_v1_2;
    s16 *var_v1_3;
    s16 temp_t4;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_v0;
    u8 *var_v0_4;


    sp40 = D_8014A5F4;
    if (*(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7074)) == 0) {
        var_v0_2 = ((DisplayActor44 *)D_8011ECD0);
        if (*(s16 *)((unsigned char *)(&D_8014A3DC) + (2)) >= 0x781) {
            var_v1 = D_8014A400;
            do {
                *(s8 *)((unsigned char *)var_v0_2 + (7074)) = 0x40;
                *(s8 *)((unsigned char *)var_v0_2 + (7075)) = 0xA4;
                if (var_v1->unk_01 == 1) {
                    *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7143)) = 0U;
                }
                var_v1 += 1;
                var_v0_2 += 1;
            } while (var_v1 != D_8014A454);
        }
    }
    if (((DisplayActor44 *)D_8011ECD0)->unk0 == 0x4F) {
        *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7143)) = (u8) (*(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7143)) | 0x40);
    }
    var_v1_2 = D_8014A400;
    var_a0 = 0;
    do {
        if ((var_v1_2->unk_01 == 1) && (*(s16 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)[var_a0]) + (6634)) >= (*(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7178)) + 0x30))) {
            *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7143)) = 0U;
            *(s8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7142)) = 0;
        }
        var_a0 += 1;
        var_v1_2 += 1;
    } while (var_a0 < 3);
    var_v0_3 = ((DisplayActor44 *)D_8011ECD0);
    var_v1_3 = sp40.v;
    do {
        temp_t4 = *var_v1_3;
        var_v1_3 += 2;
        var_v0_3 += 1;
        *(s16 *)((unsigned char *)var_v0_3 + (7042)) = (s16) ((temp_t4 - (*(s16 *)((unsigned char *)(&D_8014A3DC) + (2)) * 2)) - 0xA0);
    } while ((u32) var_v1_3 < (u32) &sp48);
    if ((*(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7314)) < -0xDC) && (*(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7279)) != 0)) {
        *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7279)) = 0U;
    }
    if (((DisplayActor44 *)D_8011ECD0)->unk0 == 0x4F) {
        *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7178)) = (s16) (*(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7110)) + 0x20);
    }
    var_v0 = 0;
    if ((((DisplayActor44 *)D_8011ECD0)->unk0 + 0x40) >= ((DisplayActor44 *)D_8011ECD0)->unk0) {
        if (*D_8014A3D8 == 0) {
            *(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7143)) = 0xE4U;
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7164)) = 0x4F;
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7178)) = (s16) (*(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (7246)) + 0x20);
        }
        var_a0_2 = 0;
        if (*D_8014A3D8 != 0) {
            var_v0_4 = D_8014A3D8;
            do {
                var_a0_2 += 1;
                var_v0_4 += 1;
            } while (*(u8 *)((unsigned char *)var_v0_4 + (1)) != 0);
        }
        D_8014A3D8[var_a0_2] = 1;
        if ((var_a0_2 + 1) < 4) {
            var_s0 = &*(s32 *)((unsigned char *)(&D_8014A3D8[var_a0_2]) + (1));
            do {
                var_s0 += 1;
                *(s8 *)((unsigned char *)var_s0 + (-1)) = func_80146FA0();
            } while (var_s0 != &D_8014A3DC);
        }
        var_v0 = 1;
    }
    return var_v0;
}
