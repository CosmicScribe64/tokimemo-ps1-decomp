void func_8013493C(void) {
    Arrs4x8 sp34;
    OptActor44 *var_v0;
    OptActor44 *var_v0_2;
    s16 temp_t6;
    s32 temp_t0_2;
    u8 temp_t0;
    u8 temp_t2;
    u8 temp_t2_2;
    u8 temp_t4;
    u8 temp_t4_2;
    u8 temp_t6_2;
    u8 temp_t8;
    u8 temp_t8_2;

    if (D_800E6280.unk_110D == 0) {
        func_80064FA4(0);
        *(u8 *)((unsigned char *)D_8011ECD0 + (7279)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (7279)) & ~0x80);
        var_v0 = D_8011ECD0 + 0x44;
        do {
            temp_t8 = *(u8 *)((unsigned char *)var_v0 + (7279));
            temp_t0 = *(u8 *)((unsigned char *)var_v0 + (7347));
            temp_t2 = *(u8 *)((unsigned char *)var_v0 + (7415));
            temp_t4 = *(u8 *)((unsigned char *)var_v0 + (7483));
            var_v0 += 0x110;
            *(s8 *)((unsigned char *)var_v0 + (7211)) = (s8) (temp_t4 & ~0x80);
            *(s8 *)((unsigned char *)var_v0 + (7143)) = (s8) (temp_t2 & ~0x80);
            *(s8 *)((unsigned char *)var_v0 + (7075)) = (s8) (temp_t0 & ~0x80);
            *(s8 *)((unsigned char *)var_v0 + (7007)) = (s8) (temp_t8 & ~0x80);
        } while (var_v0 != (D_8011ECD0 + 0x264));
        temp_t6 = ((s16) *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) / 9) * 8;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) = temp_t6;
        if (temp_t6 < 9) {
            D_800E6280.unk_110D += 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
        func_80064FA4(0);
        *(u8 *)((unsigned char *)D_8011ECD0 + (7279)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (7279)) & 0xFF7F);
        var_v0_2 = D_8011ECD0 + 0x44;
        do {
            temp_t2_2 = *(u8 *)((unsigned char *)var_v0_2 + (7279));
            temp_t4_2 = *(u8 *)((unsigned char *)var_v0_2 + (7347));
            temp_t6_2 = *(u8 *)((unsigned char *)var_v0_2 + (7415));
            temp_t8_2 = *(u8 *)((unsigned char *)var_v0_2 + (7483));
            var_v0_2 += 0x110;
            *(s8 *)((unsigned char *)var_v0_2 + (7211)) = (s8) (temp_t8_2 & ~0x80);
            *(s8 *)((unsigned char *)var_v0_2 + (7143)) = (s8) (temp_t6_2 & ~0x80);
            *(s8 *)((unsigned char *)var_v0_2 + (7075)) = (s8) (temp_t4_2 & ~0x80);
            *(s8 *)((unsigned char *)var_v0_2 + (7007)) = (s8) (temp_t2_2 & ~0x80);
        } while (var_v0_2 != (D_8011ECD0 + 0x264));
        temp_t0_2 = (s32) (*(s16 *)((unsigned char *)D_8011ECD0 + (7234)) * 9) / 8;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7236)) = (s16) ((s32) (*(s16 *)((unsigned char *)D_8011ECD0 + (7236)) * 9) / 8);
        *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) = (s16) temp_t0_2;
        if ((s16) temp_t0_2 >= 0x4000) {
            D_800E6280.unk_110D = 1;
        }
    } else {
        func_801325A0(&sp34);
        func_8004F870(0, 8, &sp34, &sp34.b[0], &sp34.c[0], &sp34.d[0]);
        func_80042908(0);
        func_80042940(2);
    }
    func_80132384();
    *(s8 *)((unsigned char *)D_8011ECD0 + (7211)) = 0;
}
