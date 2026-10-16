? func_8009EFB0(s32, ?);                            /* extern */
extern ? D_8015F5A0;
extern ? D_8015F5B4;
extern u8 D_8015F5E8;
extern u8 D_8015F5E9;
extern u8 D_8015F5EA;
extern u8 D_8015F5EB;
extern u8 D_8015F5EC;
extern u8 D_8015F5ED;
extern u8 D_8015F5EE;
extern u8 D_8015F5EF;

void func_80139424(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32 sp58;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 sp48;
    s32 sp44;
    s32 sp40;
    s32 sp3C;
    s32 sp38;
    s16 temp_v1;
    s16 temp_v1_2;
    s16 temp_v1_3;
    s32 temp_ra;
    s32 temp_t5;
    s32 temp_t7;
    s32 temp_t8;
    s32 temp_v0;
    s32 temp_v0_3;
    s32 var_t0;
    s32 var_t1;
    s32 var_t2;
    s32 var_t4;
    s8 temp_v0_5;
    s8 temp_v0_6;
    s8 temp_v0_7;
    s8 var_t8;
    u32 temp_s1;
    void *temp_v0_2;
    void *temp_v0_4;

    temp_s1 = arg0 & 0xFF;
    if (arg8 == 3) {
        var_t0 = 0xC;
    } else {
        var_t0 = 0xB;
    }
    sp38 = var_t0;
    temp_v0 = func_800490F0(0x28, D_8011ECA0);
    func_8009F040(temp_v0);
    func_8009EFB0(temp_v0, 0);
    func_8009EF84(temp_v0, 0);
    *(s8 *)((unsigned char *)temp_v0 + (6)) = (s8) arg3;
    *(s8 *)((unsigned char *)temp_v0 + (5)) = (s8) arg3;
    *(s8 *)((unsigned char *)temp_v0 + (4)) = (s8) arg3;
    temp_v1 = D_800E6280.unk_014[D_8011ECA0];
    temp_t7 = (s32) (var_t0 * arg6) / 8192;
    temp_ra = (temp_v1 + arg4) - temp_t7;
    temp_t5 = temp_v1 + arg4 + temp_t7;
    sp50 = temp_t5;
    sp48 = temp_ra;
    if (arg8 != 1) {
        temp_v1_2 = D_800E6280.unk_018[D_8011ECA0];
        temp_t8 = (s32) (arg7 * 0xC) / 8192;
        var_t2 = (temp_v1_2 + arg5) - temp_t8;
        var_t4 = temp_v1_2 + arg5 + temp_t8;
        var_t1 = var_t2;
    } else {
        temp_v1_3 = D_800E6280.unk_018[D_8011ECA0];
        var_t2 = (temp_v1_3 + arg5) - ((s32) (arg7 * 0xC) / 4096);
        var_t4 = temp_v1_3 + arg5;
        var_t1 = var_t2;
    }
    sp58 = temp_ra;
    sp38 = var_t0;
    sp54 = var_t1;
    sp4C = var_t2;
    sp44 = var_t4;
    sp3C = var_t4;
    sp40 = temp_t5;
    *(s16 *)((unsigned char *)temp_v0 + (22)) = func_8009ECB0(0, 0, 0x340, 0x180);
    *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) var_t1;
    *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) temp_ra;
    *(s16 *)((unsigned char *)temp_v0 + (18)) = (s16) var_t2;
    *(s16 *)((unsigned char *)temp_v0 + (16)) = (s16) sp50;
    *(s16 *)((unsigned char *)temp_v0 + (26)) = (s16) var_t4;
    *(s16 *)((unsigned char *)temp_v0 + (34)) = (s16) var_t4;
    *(s16 *)((unsigned char *)temp_v0 + (32)) = (s16) temp_t5;
    *(s16 *)((unsigned char *)temp_v0 + (24)) = (s16) sp48;
    if ((temp_s1 >= 0x30U) && (temp_s1 < 0x3AU)) {
        temp_v0_2 = (((temp_s1 - 0x30) & 0xFF) * 2) + &D_8015F5A0;
        *(s8 *)((unsigned char *)temp_v0 + (12)) = (s8) (temp_v0_2->unk0 + 0x80);
        *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) *(u8 *)((unsigned char *)temp_v0_2 + (1));
        *(s8 *)((unsigned char *)temp_v0 + (20)) = (s8) (temp_v0_2->unk0 + var_t0 + 0x80);
        *(u8 *)((unsigned char *)temp_v0 + (21)) = (u8) *(u8 *)((unsigned char *)temp_v0_2 + (1));
        *(s8 *)((unsigned char *)temp_v0 + (28)) = (s8) (temp_v0_2->unk0 + 0x80);
        *(s8 *)((unsigned char *)temp_v0 + (29)) = (s8) (*(u8 *)((unsigned char *)temp_v0_2 + (1)) + 0xC);
        *(s8 *)((unsigned char *)temp_v0 + (36)) = (s8) (temp_v0_2->unk0 + var_t0 + 0x80);
        *(s8 *)((unsigned char *)temp_v0 + (37)) = (s8) (*(u8 *)((unsigned char *)temp_v0_2 + (1)) + 0xC);
    } else {
        temp_v0_3 = temp_s1 - 0x41;
        if ((u32) (temp_v0_3 & 0xFF) < 0x1AU) {
            temp_v0_4 = ((temp_v0_3 & 0xFF) * 2) + &D_8015F5B4;
            *(s8 *)((unsigned char *)temp_v0 + (12)) = (s8) (temp_v0_4->unk0 + 0x80);
            *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) *(u8 *)((unsigned char *)temp_v0_4 + (1));
            *(s8 *)((unsigned char *)temp_v0 + (20)) = (s8) (temp_v0_4->unk0 + var_t0 + 0x80);
            *(u8 *)((unsigned char *)temp_v0 + (21)) = (u8) *(u8 *)((unsigned char *)temp_v0_4 + (1));
            *(s8 *)((unsigned char *)temp_v0 + (28)) = (s8) (temp_v0_4->unk0 + 0x80);
            *(s8 *)((unsigned char *)temp_v0 + (29)) = (s8) (*(u8 *)((unsigned char *)temp_v0_4 + (1)) + 0xC);
            *(s8 *)((unsigned char *)temp_v0 + (36)) = (s8) (temp_v0_4->unk0 + var_t0 + 0x80);
            var_t8 = *(u8 *)((unsigned char *)temp_v0_4 + (1)) + 0xC;
            goto block_24;
        }
        if (temp_s1 == 0x2E) {
            *(s8 *)((unsigned char *)temp_v0 + (12)) = (s8) (D_8015F5E8 + 0x80);
            *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) D_8015F5E9;
            *(s8 *)((unsigned char *)temp_v0 + (20)) = (s8) (D_8015F5E8 + var_t0 + 0x80);
            *(u8 *)((unsigned char *)temp_v0 + (21)) = (u8) D_8015F5E9;
            *(s8 *)((unsigned char *)temp_v0 + (28)) = (s8) (D_8015F5E8 + 0x80);
            *(s8 *)((unsigned char *)temp_v0 + (29)) = (s8) (D_8015F5E9 + 0xC);
            *(s8 *)((unsigned char *)temp_v0 + (36)) = (s8) (D_8015F5E8 + var_t0 + 0x80);
            var_t8 = D_8015F5E9 + 0xC;
            goto block_24;
        }
        if (temp_s1 == 0x2D) {
            *(s8 *)((unsigned char *)temp_v0 + (12)) = (s8) (D_8015F5EA + 0x80);
            *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) D_8015F5EB;
            *(s8 *)((unsigned char *)temp_v0 + (20)) = (s8) (D_8015F5EA + var_t0 + 0x80);
            *(u8 *)((unsigned char *)temp_v0 + (21)) = (u8) D_8015F5EB;
            *(s8 *)((unsigned char *)temp_v0 + (28)) = (s8) (D_8015F5EA + 0x80);
            *(s8 *)((unsigned char *)temp_v0 + (29)) = (s8) (D_8015F5EB + 0xC);
            *(s8 *)((unsigned char *)temp_v0 + (36)) = (s8) (D_8015F5EA + var_t0 + 0x80);
            var_t8 = D_8015F5EB + 0xC;
            goto block_24;
        }
        if (temp_s1 == 0x21) {
            *(s8 *)((unsigned char *)temp_v0 + (12)) = (s8) (D_8015F5EE + 0x80);
            *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) D_8015F5EF;
            *(s8 *)((unsigned char *)temp_v0 + (20)) = (s8) (D_8015F5EE + var_t0 + 0x80);
            *(u8 *)((unsigned char *)temp_v0 + (21)) = (u8) D_8015F5EF;
            *(s8 *)((unsigned char *)temp_v0 + (28)) = (s8) (D_8015F5EE + 0x80);
            *(s8 *)((unsigned char *)temp_v0 + (29)) = (s8) (D_8015F5EF + 0xC);
            *(s8 *)((unsigned char *)temp_v0 + (36)) = (s8) (D_8015F5EE + var_t0 + 0x80);
            var_t8 = D_8015F5EF + 0xC;
            goto block_24;
        }
        if (temp_s1 == 0x40) {
            temp_v0_5 = var_t0 + 0xA6;
            *(s8 *)((unsigned char *)temp_v0 + (20)) = temp_v0_5;
            *(s8 *)((unsigned char *)temp_v0 + (36)) = temp_v0_5;
            *(s8 *)((unsigned char *)temp_v0 + (12)) = 0xA6;
            *(s8 *)((unsigned char *)temp_v0 + (28)) = 0xA6;
            *(s8 *)((unsigned char *)temp_v0 + (29)) = 0xDF;
            *(s8 *)((unsigned char *)temp_v0 + (37)) = 0xDF;
            *(u8 *)((unsigned char *)temp_v0 + (13)) = 0xD3U;
            *(u8 *)((unsigned char *)temp_v0 + (21)) = 0xD3U;
        } else if (temp_s1 == 0x2A) {
            temp_v0_6 = var_t0 + 0xBE;
            *(s8 *)((unsigned char *)temp_v0 + (20)) = temp_v0_6;
            *(s8 *)((unsigned char *)temp_v0 + (36)) = temp_v0_6;
            *(s8 *)((unsigned char *)temp_v0 + (12)) = 0xBE;
            *(s8 *)((unsigned char *)temp_v0 + (28)) = 0xBE;
            *(s8 *)((unsigned char *)temp_v0 + (29)) = 0xDF;
            *(s8 *)((unsigned char *)temp_v0 + (37)) = 0xDF;
            *(u8 *)((unsigned char *)temp_v0 + (13)) = 0xD3U;
            *(u8 *)((unsigned char *)temp_v0 + (21)) = 0xD3U;
        } else if (temp_s1 == 0x23) {
            temp_v0_7 = var_t0 + 0xEE;
            *(s8 *)((unsigned char *)temp_v0 + (20)) = temp_v0_7;
            *(s8 *)((unsigned char *)temp_v0 + (36)) = temp_v0_7;
            *(s8 *)((unsigned char *)temp_v0 + (12)) = 0xEE;
            *(s8 *)((unsigned char *)temp_v0 + (28)) = 0xEE;
            *(s8 *)((unsigned char *)temp_v0 + (29)) = 0xDF;
            *(s8 *)((unsigned char *)temp_v0 + (37)) = 0xDF;
            *(u8 *)((unsigned char *)temp_v0 + (13)) = 0xD3U;
            *(u8 *)((unsigned char *)temp_v0 + (21)) = 0xD3U;
        } else {
            *(s8 *)((unsigned char *)temp_v0 + (12)) = (s8) (D_8015F5EC + 0x80);
            *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) D_8015F5ED;
            *(s8 *)((unsigned char *)temp_v0 + (20)) = (s8) (D_8015F5EC + var_t0 + 0x80);
            *(u8 *)((unsigned char *)temp_v0 + (21)) = (u8) D_8015F5ED;
            *(s8 *)((unsigned char *)temp_v0 + (28)) = (s8) (D_8015F5EC + 0x80);
            *(s8 *)((unsigned char *)temp_v0 + (29)) = (s8) (D_8015F5ED + 0xC);
            *(s8 *)((unsigned char *)temp_v0 + (36)) = (s8) (D_8015F5EC + var_t0 + 0x80);
            var_t8 = D_8015F5ED + 0xC;
block_24:
            *(s8 *)((unsigned char *)temp_v0 + (37)) = var_t8;
        }
    }
    *(s16 *)((unsigned char *)temp_v0 + (14)) = func_8009ED74(arg1 * 0x10, 0x1EF);
    func_8009EED0(&D_800E8CA0[(D_8011ECA0 << 0xA) + (arg2 * 4)], temp_v0);
    func_800494BC(0x340, 0x180, arg2, 0, 0);
}
