void func_801432F0(u8 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0;
    s32 var_s2;
    u32 temp_t2;
    u8 *var_s1;
    u8 temp_t5;
    u8 temp_v1;
    u8 *temp_v0_2;
    u8 *temp_v0_3;

    var_s1 = arg0;
    var_s2 = arg1;
    if (*arg0 != 0) {
        do {
            temp_v0 = GetWorkBase(0x14, D_8011ECA0);
            func_8009F0A4(temp_v0);
            SetSemiTrans(temp_v0, 0);
            *(s8 *)((unsigned char *)temp_v0 + (4)) = 0x80;
            *(s8 *)((unsigned char *)temp_v0 + (5)) = 0x80;
            *(s8 *)((unsigned char *)temp_v0 + (6)) = 0x80;
            *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + var_s2);
            var_s2 += 0xC;
            *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg2);
            temp_v1 = (*(s32 *)var_s1);
            if ((temp_v1 >= 0x30U) && (temp_v1 < 0x3AU)) {
                temp_v0_2 = &D_8015F5A0 + (((temp_v1 - 0x30) & 0xFF) * 2);
                *(s8 *)((unsigned char *)temp_v0 + (12)) = (s8) ((*(s32 *)temp_v0_2) + 0x80);
                *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) *(u8 *)((unsigned char *)temp_v0_2 + (1));
            } else {
                temp_t2 = (temp_v1 - 0x41) & 0xFF;
                if (temp_t2 < 0x1AU) {
                    temp_v0_3 = (temp_t2 * 2) + &D_8015F5B4;
                    *(s8 *)((unsigned char *)temp_v0 + (12)) = (s8) ((*(s32 *)temp_v0_3) + 0x80);
                    *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) *(u8 *)((unsigned char *)temp_v0_3 + (1));
                } else if (temp_v1 == 0x2E) {
                    *(s8 *)((unsigned char *)temp_v0 + (12)) = (s8) (D_8015F5E8 + 0x80);
                    *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) D_8015F5E9;
                } else if (temp_v1 == 0x2D) {
                    *(s8 *)((unsigned char *)temp_v0 + (12)) = (s8) (D_8015F5EA + 0x80);
                    *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) D_8015F5EB;
                } else if (temp_v1 == 0x20) {
                    *(s8 *)((unsigned char *)temp_v0 + (12)) = (s8) (D_8015F5EC + 0x80);
                    *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) D_8015F5ED;
                } else {
                    *(s8 *)((unsigned char *)temp_v0 + (12)) = (s8) (D_8015F5EE + 0x80);
                    *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) D_8015F5EF;
                }
            }
            *(s16 *)((unsigned char *)temp_v0 + (18)) = 0x10;
            *(s16 *)((unsigned char *)temp_v0 + (16)) = 0xC;
            *(s16 *)((unsigned char *)temp_v0 + (14)) = func_8009ED74(arg3 * 0x10, 0x1EF);
            AddPrim((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0);
            temp_t5 = *(u8 *)((unsigned char *)var_s1 + (1));
            var_s1 += 1;
        } while (temp_t5 != 0);
    }
    dtd_on_tpage(0x340, 0x180, 1, 0, 0);
}
