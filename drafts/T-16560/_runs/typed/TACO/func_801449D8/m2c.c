? func_80049450(?);                                 /* extern */
? func_8009EFB0(void *, ?);                         /* extern */
extern ? D_800E6298;

void func_801449D8(void *arg0, s32 arg1, s32 arg2) {
    void *sp7C;
    s16 temp_s3;
    s16 temp_v0_2;
    s16 temp_v1;
    s32 temp_t7;
    s32 temp_t9;
    s32 var_s4;
    void **var_s0;
    void *temp_s0;
    void *temp_v0;
    void *var_s1;

    var_s0 = &sp7C;
    var_s1 = arg0;
    var_s4 = 0;
    do {
        temp_v0 = func_800490F0(0x18, D_8011ECA0);
        *var_s0 = temp_v0;
        func_8009F02C((s32) temp_v0);
        func_8009EF84(*var_s0, 1);
        func_8009EFB0(*var_s0, 0);
        temp_t7 = var_s1->unk0 + arg1;
        temp_t9 = *(s32 *)((unsigned char *)var_s1 + (4)) + arg2;
        var_s1->unk0 = temp_t7;
        *(s32 *)((unsigned char *)var_s1 + (4)) = temp_t9;
        temp_s3 = *(&D_800E6298 + (D_8011ECA0 * 2)) + temp_t9;
        if ((temp_t7 < 0) || (temp_t7 >= 0x141)) {
            if (arg1 > 0) {
                var_s1->unk0 = 0;
            } else {
                var_s1->unk0 = 0x140;
            }
            *(s32 *)((unsigned char *)var_s1 + (4)) = (s32) ((func_800AE0D0() % 240) - 0x78);
        } else if ((temp_s3 < 0) || (temp_s3 >= 0x1E1)) {
            var_s1->unk0 = (s32) (func_800AE0D0() % 320);
            if (arg2 > 0) {
                *(s32 *)((unsigned char *)var_s1 + (4)) = -0x78;
            } else {
                *(s32 *)((unsigned char *)var_s1 + (4)) = 0x78;
            }
        }
        temp_v0_2 = temp_t7 + 1;
        *(s16 *)((unsigned char *)(*var_s0) + (8)) = (s16) temp_t7;
        temp_v1 = temp_s3 + 1;
        *(s16 *)((unsigned char *)(*var_s0) + (10)) = temp_s3;
        *(s16 *)((unsigned char *)(*var_s0) + (12)) = temp_v0_2;
        *(s16 *)((unsigned char *)(*var_s0) + (14)) = temp_s3;
        *(s16 *)((unsigned char *)(*var_s0) + (16)) = (s16) temp_t7;
        *(s16 *)((unsigned char *)(*var_s0) + (18)) = temp_v1;
        *(s16 *)((unsigned char *)(*var_s0) + (20)) = temp_v0_2;
        *(s16 *)((unsigned char *)(*var_s0) + (22)) = temp_v1;
        if ((func_800AE0D0() % 5) >= 3) {
            *(s8 *)((unsigned char *)(*var_s0) + (4)) = 0x80;
            *(s8 *)((unsigned char *)(*var_s0) + (5)) = 0x80;
            *(s8 *)((unsigned char *)(*var_s0) + (6)) = 0x80;
        } else {
            *(s8 *)((unsigned char *)(*var_s0) + (4)) = 0xFF;
            *(s8 *)((unsigned char *)(*var_s0) + (5)) = 0xFF;
            *(s8 *)((unsigned char *)(*var_s0) + (6)) = 0xFF;
        }
        if ((func_800AE0D0() % 3) > 0) {
            func_8009EED0((D_8011ECA0 << 0xA) + 0x190 + D_800E8CA0, *var_s0);
        }
        var_s4 += 8;
        var_s0 += 4;
        var_s1 += 8;
    } while (var_s4 != 0xC8);
    temp_s0 = func_800490F0(0xC, D_8011ECA0);
    func_8009D294((s32) temp_s0, 0, 0, func_8009ECB0(1, 1, 0, 0), 0);
    func_8009EED0((D_8011ECA0 << 0xA) + D_800E8CA0, temp_s0);
    func_80049450(0);
}
