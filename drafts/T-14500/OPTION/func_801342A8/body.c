void func_801342A8(void) {
    s32 sp68[1];
    s32 sp58[1];
    s32 sp48[1];
    s32 sp38[1];
    s16 temp_t8;
    s32 temp_t4;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 temp_t2;
    u8 temp_t6;

    func_8006509C();
    icon_disp_switch(0);
    if (D_800E738D == 0) {
        icon_disp_switch(0);
        var_v0 = D_8011ECD0;
        do {
            temp_t6 = *(u8 *)((unsigned char *)var_v0 + (7279));
            var_v0 += 0x44;
            *(s8 *)((unsigned char *)var_v0 + (7211)) = (s8) (temp_t6 & ~0x80);
        } while (var_v0 != D_8011ED9C);
        temp_t8 = ((s16) *D_80120912 / 9) * 8;
        *D_80120912 = temp_t8;
        if (temp_t8 < 9) {
            D_800E738D += 1;
        }
    } else if (D_800E738D == 2) {
        icon_disp_switch(0);
        var_v0_2 = D_8011ECD0;
        do {
            temp_t2 = *(u8 *)((unsigned char *)var_v0_2 + (7279));
            var_v0_2 += 0x44;
            *(s8 *)((unsigned char *)var_v0_2 + (7211)) = (s8) (temp_t2 & ~0x80);
        } while (var_v0_2 != D_8011ED9C);
        temp_t4 = (s32) (*D_80120912 * 9) / 8;
        D_80120914 = (s16) ((s32) (D_80120914 * 9) / 8);
        *D_80120912 = (s16) temp_t4;
        if ((s16) temp_t4 >= 0x4000) {
            D_800E738D = 1;
        }
    } else {
        func_801325A0((s32) &sp38);
        menu_set(0, 8, &sp38, &sp48, &sp58, &sp68);
        func_80042908(0);
        func_80042940(2);
    }
    func_80132384();
    D_801208FB = 0;
}
