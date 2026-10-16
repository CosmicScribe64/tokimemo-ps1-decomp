void func_80138CCC(void) {
    typedef struct { s32 w[32]; } M2cBlock80; /* m2c memcpy of 128 bytes */
    s32 spD8[1];
    s32 spB8[1];
    u8 sp5B;
    s32 sp4C[1];
    s32 *var_s0;
    GameState *var_v0;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 var_s1_2;
    s32 var_s1_3;
    s32 var_s1_4;
    s8 var_s1;
    u8 *var_s0_2;
    u8 *var_s0_3;
    u8 *var_s0_4;
    u8 var_v1;

    *(M2cBlock80 *)(&sp78[0]) = *(M2cBlock80 *)(&D_80139DA0);
    sp5C[0].unk0 = (s32) D_80139E20.unk0;
    var_v1 = 0;
    *(s32 *)((unsigned char *)(&sp5C[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80139E20) + (4));
    var_v0 = &D_800E6280;
    *(s32 *)((unsigned char *)(&sp5C[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80139E20) + (8));
    var_s1 = 0;
    *(s32 *)((unsigned char *)(&sp5C[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80139E20) + (12));
    *(s32 *)((unsigned char *)(&sp5C[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80139E20) + (16));
    *(s32 *)((unsigned char *)(&sp5C[0]) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80139E20) + (20));
    *(u16 *)((unsigned char *)(&sp5C[0]) + (24)) = (u16) D_80139E20.value[0xC];
    do {
        if (((u32) (var_v0->unk_1BC[0].unk_0C.w << 0x1E) >> 0x1F) == 1) {
            *(&sp4C + var_v1) = var_s1;
            var_v1 = (var_v1 + 1) & 0xFF;
        }
        var_s1 += 1;
        var_v0 += 0x38;
    } while (var_s1 < 0xB);
    var_s0 = &spB8;
    if ((D_800E7D2F != 0) || (((u32) (D_800E66B0 << 0x14) >> 0x1D) != 0)) {
        *(&sp4C + var_v1) = 0xB;
        var_v1 = (var_v1 + 1) & 0xFF;
    }
    if (D_800E7D30 != 0) {
        *(&sp4C + var_v1) = 0xC;
        var_v1 = (var_v1 + 1) & 0xFF;
    }
    do {
        sp5B = var_v1;
        var_s0 += 2;
        *(s16 *)((unsigned char *)var_s0 + (-2)) = (s16) (&sp5C[0])[*(&sp4C + (func_800AE0D0() % var_v1))];
    } while ((u32) var_s0 < (u32) &spD8);
    var_s0_2 = D_8011ECD0;
    var_s1_2 = 0;
    do {
        *(s16 *)((unsigned char *)var_s0_2 + (7178)) = (s16) (*(s16 *)((unsigned char *)var_s0_2 + (7178)) + 1);
        if (*(s16 *)((unsigned char *)var_s0_2 + (7178)) >= 0xC1) {
            temp_v1 = func_800AE0D0() & 0x3F;
            *(s16 *)((unsigned char *)var_s0_2 + (7164)) = (s16) (&sp78[0])[temp_v1];
            *(s16 *)((unsigned char *)var_s0_2 + (7178)) = (s16) (*(s16 *)((unsigned char *)(D_8011ECD0 + (((s32) (var_s1_2 + 1) % 4) * 0x44)) + (7178)) - 0x60);
            if ((temp_v1 & 1) && (temp_v1 < 0x20)) {
                *(s16 *)((unsigned char *)var_s0_2 + (7182)) = -0x38;
            } else {
                *(s16 *)((unsigned char *)var_s0_2 + (7182)) = -0x48;
            }
            *(s32 *)((unsigned char *)var_s0_2 + (7196)) = 0x01000000;
        }
        var_s1_2 += 1;
        var_s0_2 += 0x44;
    } while (var_s1_2 < 4);
    var_s0_3 = D_8011ECD0;
    var_s1_3 = 0;
    do {
        *(s32 *)((unsigned char *)var_s0_3 + (7448)) = (s32) (*(s32 *)((unsigned char *)var_s0_3 + (7448)) + 0xFFFF2000);
        if (*(s16 *)((unsigned char *)var_s0_3 + (7450)) < -0xC0) {
            temp_v1_2 = func_800AE0D0() & 0x3F;
            *(s16 *)((unsigned char *)var_s0_3 + (7436)) = (s16) (&sp78[0])[temp_v1_2];
            *(s16 *)((unsigned char *)var_s0_3 + (7450)) = (s16) (*(s16 *)((unsigned char *)(D_8011ECD0 + (((s32) (var_s1_3 + 3) % 4) * 0x44)) + (7450)) + 0x60);
            if ((temp_v1_2 & 1) && (temp_v1_2 < 0x20)) {
                *(s16 *)((unsigned char *)var_s0_3 + (7454)) = -0x10;
            } else {
                *(s16 *)((unsigned char *)var_s0_3 + (7454)) = -0x20;
            }
            *(s32 *)((unsigned char *)var_s0_3 + (7468)) = 0x01000000;
        }
        var_s1_3 += 1;
        var_s0_3 += 0x44;
    } while (var_s1_3 < 4);
    var_s0_4 = D_8011ECD0;
    var_s1_4 = 0;
    do {
        *(s32 *)((unsigned char *)var_s0_4 + (7720)) = (s32) (*(s32 *)((unsigned char *)var_s0_4 + (7720)) + 0xC000);
        if (*(s16 *)((unsigned char *)var_s0_4 + (7722)) >= 0xC1) {
            temp_v1_3 = func_800AE0D0() & 0x3F;
            *(s16 *)((unsigned char *)var_s0_4 + (7708)) = (s16) (&sp78[0])[temp_v1_3];
            *(s16 *)((unsigned char *)var_s0_4 + (7722)) = (s16) (*(s16 *)((unsigned char *)(D_8011ECD0 + (((s32) (var_s1_4 + 1) % 4) * 0x44)) + (7722)) - 0x60);
            if ((temp_v1_3 & 1) && (temp_v1_3 < 0x20)) {
                *(s16 *)((unsigned char *)var_s0_4 + (7726)) = 0x18;
            } else {
                *(s16 *)((unsigned char *)var_s0_4 + (7726)) = 8;
            }
            *(s32 *)((unsigned char *)var_s0_4 + (7740)) = 0x01000000;
        }
        var_s1_4 += 1;
        var_s0_4 += 0x44;
    } while (var_s1_4 != 4);
}
