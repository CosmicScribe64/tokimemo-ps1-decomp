extern s32 D_800E7378;
extern ? D_8015F5A0;

void func_801430D0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0;
    s32 var_s1;
    s32 var_s2;
    s32 var_v0;
    void *temp_v0_2;

    if (arg0 < 0) {
        var_s1 = -arg0;
        var_v0 = -1;
    } else {
        var_s1 = arg0;
        var_v0 = 1;
    }
    if ((var_v0 != -1) || !(D_800E7378 & 4)) {
        var_s2 = 0;
        if (var_s1 >= 0) {
loop_7:
            temp_v0 = func_800490F0(0x14, D_8011ECA0);
            func_8009F0A4(temp_v0);
            *(s16 *)((unsigned char *)temp_v0 + (16)) = 0xC;
            *(s16 *)((unsigned char *)temp_v0 + (18)) = 0x10;
            func_8009EF84(temp_v0, 0);
            *(s8 *)((unsigned char *)temp_v0 + (4)) = 0x80;
            *(s8 *)((unsigned char *)temp_v0 + (5)) = 0x80;
            *(s8 *)((unsigned char *)temp_v0 + (6)) = 0x80;
            *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + arg1) - var_s2);
            *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg2);
            temp_v0_2 = ((var_s1 % 10) * 2) + &D_8015F5A0;
            *(s8 *)((unsigned char *)temp_v0 + (12)) = (s8) (temp_v0_2->unk0 + 0x80);
            *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) *(u8 *)((unsigned char *)temp_v0_2 + (1));
            *(s16 *)((unsigned char *)temp_v0 + (14)) = func_8009ED74(arg3 * 0x10, 0x1EF);
            func_8009EED0((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0);
            if (var_s1 >= 0xA) {
                var_s2 += 0xC;
                var_s1 = var_s1 / 10;
                if (var_s1 >= 0) {
                    goto loop_7;
                }
            }
        }
        func_800494BC(0x340, 0x180, 1, 0, 0);
    }
}
