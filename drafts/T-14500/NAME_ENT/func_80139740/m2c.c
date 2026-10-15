extern ? D_8014D0F4;

s32 func_80139740(u32 arg0, s32 arg1, s32 arg2) {
    s32 temp_v0;
    s32 var_s2;
    u32 var_s1;
    void *temp_v0_2;

    var_s1 = arg0;
    var_s2 = 0;
loop_1:
    temp_v0 = func_800490F0(0x10, D_8011ECA0);
    func_8009F090(temp_v0);
    func_8009EF84(temp_v0, 0);
    *(s8 *)((unsigned char *)temp_v0 + (4)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0 + (5)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0 + (6)) = 0x80;
    *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + arg1) - var_s2);
    *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg2);
    temp_v0_2 = &D_8014D0F4 + ((var_s1 % 10U) * 2);
    *(u8 *)((unsigned char *)temp_v0 + (12)) = (u8) temp_v0_2->unk0;
    *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) *(u8 *)((unsigned char *)temp_v0_2 + (1));
    *(s16 *)((unsigned char *)temp_v0 + (14)) = func_8009ED74(0, 0x1F0);
    func_8009EED0((D_8011ECA0 << 0xA) + 0xC + D_800E8CA0, temp_v0);
    if (var_s1 >= 0xAU) {
        var_s2 += 8;
        var_s1 = var_s1 / 10U;
        goto loop_1;
    }
    func_800494BC(0x200, 0, 3, 0, 1);
    return 1;
}
