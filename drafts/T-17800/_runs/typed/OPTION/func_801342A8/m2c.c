void func_801342A8(void) {
    Arrs4x8 sp38;
    OptActor44 *var_v0;
    OptActor44 *var_v0_2;
    s16 temp_t8;
    s32 temp_t4;
    u8 temp_t2;
    u8 temp_t6;

    func_8006509C();
    func_80064FA4(0);
    if (D_800E6280.unk_110D == 0) {
        func_80064FA4(0);
        var_v0 = D_8011ECD0;
        do {
            temp_t6 = *(u8 *)((unsigned char *)var_v0 + (7279));
            var_v0 += 0x44;
            *(s8 *)((unsigned char *)var_v0 + (7211)) = (s8) (temp_t6 & ~0x80);
        } while (var_v0 != (D_8011ECD0 + 0xCC));
        temp_t8 = ((s16) *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) / 9) * 8;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) = temp_t8;
        if (temp_t8 < 9) {
            D_800E6280.unk_110D += 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
        func_80064FA4(0);
        var_v0_2 = D_8011ECD0;
        do {
            temp_t2 = *(u8 *)((unsigned char *)var_v0_2 + (7279));
            var_v0_2 += 0x44;
            *(s8 *)((unsigned char *)var_v0_2 + (7211)) = (s8) (temp_t2 & ~0x80);
        } while (var_v0_2 != (D_8011ECD0 + 0xCC));
        temp_t4 = (s32) (*(s16 *)((unsigned char *)D_8011ECD0 + (7234)) * 9) / 8;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7236)) = (s16) ((s32) (*(s16 *)((unsigned char *)D_8011ECD0 + (7236)) * 9) / 8);
        *(s16 *)((unsigned char *)D_8011ECD0 + (7234)) = (s16) temp_t4;
        if ((s16) temp_t4 >= 0x4000) {
            D_800E6280.unk_110D = 1;
        }
    } else {
        func_801325A0(&sp38);
        func_8004F870(0, 8, &sp38, &sp38.b[0], &sp38.c[0], &sp38.d[0]);
        func_80042908(0);
        func_80042940(2);
    }
    func_80132384();
    *(s8 *)((unsigned char *)D_8011ECD0 + (7211)) = 0;
}
