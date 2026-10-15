void func_8013493C(void) {
    s32 sp64[1];
    s32 sp54[1];
    s32 sp44[1];
    s32 sp34[1];
    s32 *var_v0;
    s32 *var_v0_2;
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

    if (D_800E738D == 0) {
        icon_disp_switch(0);
        D_8012093F &= ~0x80;
        var_v0 = &D_8011ED14;
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
        } while (var_v0 != &D_8011EF34);
        temp_t6 = ((s16) *D_80120912 / 9) * 8;
        *D_80120912 = temp_t6;
        if (temp_t6 < 9) {
            D_800E738D += 1;
        }
    } else if (D_800E738D == 2) {
        icon_disp_switch(0);
        D_8012093F &= 0xFF7F;
        var_v0_2 = &D_8011ED14;
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
        } while (var_v0_2 != &D_8011EF34);
        temp_t0_2 = (s32) (*D_80120912 * 9) / 8;
        D_80120914 = (s16) ((s32) (D_80120914 * 9) / 8);
        *D_80120912 = (s16) temp_t0_2;
        if ((s16) temp_t0_2 >= 0x4000) {
            D_800E738D = 1;
        }
    } else {
        func_801325A0((s32) &sp34);
        menu_set(0, 8, &sp34, &sp44, &sp54, &sp64);
        func_80042908(0);
        func_80042940(2);
    }
    func_80132384();
    D_801208FB = 0;
}
