void *func_800490F0(?, s32);                        /* extern */
? func_8009EED0(void *, void *);                    /* extern */
? func_8009EF84(void *, ?);                         /* extern */
? func_8009EFB0(void *, ?);                         /* extern */
? func_8009F004(void *);                            /* extern */
s32 func_800A0070(s32);                             /* extern */
s32 func_800A0140(s32);                             /* extern */
extern ? D_800E8CA0;
extern s32 D_8011ECA0;

void func_80153B38(void **arg0, ? arg1, u32 arg2, s32 arg3) {
    s16 sp7C;
    s16 sp7A;
    s16 sp78;
    s32 sp5C;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_t1;
    s32 var_s2;
    s32 var_s2_2;
    s8 temp_fp;
    s8 temp_s3;
    s8 temp_s4;
    s8 temp_s5;
    s8 temp_s7;
    void **var_s1;
    void *temp_s0;
    void *temp_s0_2;
    void *temp_s0_3;
    void *temp_v0;

    var_s2 = 0;
    do {
        if (var_s2 == 0) {
            sp78 = 0x100;
            sp7A = 0x78;
            sp7C = 0;
        } else {
            temp_s0 = (var_s2 * 8) + &sp78;
            temp_lo = (s32) (var_s2 * 0x9000) / 360;
            temp_s0->unk0 = (s16) (((s32) (func_800A0140(temp_lo) * arg3) / 4096) + 0x100);
            temp_lo_2 = func_800A0070(temp_lo) * arg3;
            *(s16 *)((unsigned char *)temp_s0 + (4)) = 0;
            *(s16 *)((unsigned char *)temp_s0 + (2)) = (s16) ((temp_lo_2 / 4096) + 0x78);
        }
        var_s2 += 1;
    } while (var_s2 < 0x2A);
    temp_t1 = arg2 & 0xFF;
    temp_fp = (arg2 >> 8) & 0xFF;
    temp_s7 = (arg2 >> 0x10) & 0xFF;
    var_s1 = arg0;
    temp_s5 = (temp_t1 & 0xFF) - 0x25;
    temp_s4 = (temp_fp & 0xFF) - 0x25;
    temp_s3 = (temp_s7 & 0xFF) - 0x25;
    sp5C = temp_t1;
    var_s2_2 = 0;
    do {
        temp_v0 = func_800490F0(0x1C, D_8011ECA0);
        *var_s1 = temp_v0;
        func_8009F004(temp_v0);
        func_8009EF84(*var_s1, 1);
        func_8009EFB0(*var_s1, 1);
        temp_s0_2 = &sp78 + (var_s2_2 * 8);
        if (D_8011ECA0 == 0) {
            *(s16 *)((unsigned char *)(*var_s1) + (8)) = (s16) *(s16 *)((unsigned char *)temp_s0_2 + (8));
            *(s16 *)((unsigned char *)(*var_s1) + (10)) = (s16) (*(s16 *)((unsigned char *)temp_s0_2 + (10)) + 0xF0);
            *(s16 *)((unsigned char *)(*var_s1) + (16)) = sp78;
            *(s16 *)((unsigned char *)(*var_s1) + (18)) = (s16) (sp7A + 0xF0);
            *(s16 *)((unsigned char *)(*var_s1) + (24)) = (s16) *(s16 *)((unsigned char *)temp_s0_2 + (16));
            *(s16 *)((unsigned char *)(*var_s1) + (26)) = (s16) (*(s16 *)((unsigned char *)temp_s0_2 + (18)) + 0xF0);
        }
        if (D_8011ECA0 == 1) {
            temp_s0_3 = &sp78 + (var_s2_2 * 8);
            *(s16 *)((unsigned char *)(*var_s1) + (8)) = (s16) *(s16 *)((unsigned char *)temp_s0_3 + (8));
            *(s16 *)((unsigned char *)(*var_s1) + (10)) = (s16) *(s16 *)((unsigned char *)temp_s0_3 + (10));
            *(s16 *)((unsigned char *)(*var_s1) + (16)) = sp78;
            *(s16 *)((unsigned char *)(*var_s1) + (18)) = sp7A;
            *(s16 *)((unsigned char *)(*var_s1) + (24)) = (s16) *(s16 *)((unsigned char *)temp_s0_3 + (16));
            *(s16 *)((unsigned char *)(*var_s1) + (26)) = (s16) *(s16 *)((unsigned char *)temp_s0_3 + (18));
        }
        *(s8 *)((unsigned char *)(*var_s1) + (4)) = temp_s3;
        *(s8 *)((unsigned char *)(*var_s1) + (5)) = temp_s4;
        *(s8 *)((unsigned char *)(*var_s1) + (6)) = temp_s5;
        *(s8 *)((unsigned char *)(*var_s1) + (12)) = temp_s7;
        *(s8 *)((unsigned char *)(*var_s1) + (13)) = temp_fp;
        *(s8 *)((unsigned char *)(*var_s1) + (14)) = (s8) sp5C;
        *(s8 *)((unsigned char *)(*var_s1) + (20)) = temp_s3;
        *(s8 *)((unsigned char *)(*var_s1) + (21)) = temp_s4;
        *(s8 *)((unsigned char *)(*var_s1) + (22)) = temp_s5;
        func_8009EED0((D_8011ECA0 << 0xA) + 0x10 + &D_800E8CA0, *var_s1);
        var_s2_2 += 1;
        var_s1 += 4;
    } while (var_s2_2 != 0x28);
}
