void func_801355AC(void) {
    DisplayActor44 *var_v0;
    s8 temp_t6;

    func_80066334();
    *(u8 *)((unsigned char *)D_8011ECD0 + (6535)) = (u8) D_800E6280.unk_110B;
    *(u8 *)((unsigned char *)D_8011ECD0 + (6671)) = (u8) D_800E6280.unk_110B;
    temp_t6 = ((u8) D_800E6280.unk_110B - 2) & 0xFF;
    D_800E6280.unk_110B = temp_t6;
    if (temp_t6 == 0) {
        var_v0 = D_8011ECD0 + 0x1980;
        do {
            var_v0 += 0x110;
            *(s8 *)((unsigned char *)var_v0 + (-197)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-129)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-61)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-265)) = 0;
        } while (var_v0 != D_80121750);
        D_800E6280.unk_110B = 0;
        func_8004284C();
    }
}
