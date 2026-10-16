? func_8009EFB0(s32, ?);                            /* extern */

void func_80135FBC(s32 arg0, s32 arg1) {
    s32 sp70;
    s32 sp6C;
    s32 sp68;
    s32 sp64;
    s32 sp60;
    s32 sp5C;
    s32 sp58;
    s32 sp54;
    s32 temp_a0;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_t0;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_fp;
    s32 var_s3;
    s32 var_s5;
    s32 var_s7;
    s8 temp_t2;
    s8 var_s1;
    s8 var_s2;
    s8 var_s4;
    s8 var_s6;
    void *temp_v0;

    temp_lo = arg0 * 0x19;
    temp_v0 = (arg0 * 0x60) + (arg1 * 8) + D_801392AC;
    temp_lo_2 = arg1 * 0x19;
    sp54 = (*(s16 *)((unsigned char *)temp_v0 + (2)) + temp_lo) - 0x19;
    sp64 = (*(s16 *)((unsigned char *)temp_v0 + (10)) + temp_lo) - 0x19;
    sp5C = (*(s16 *)((unsigned char *)temp_v0 + (98)) + temp_lo) - 1;
    sp6C = (*(s16 *)((unsigned char *)temp_v0 + (106)) + temp_lo) - 1;
    sp60 = (*(s16 *)((unsigned char *)temp_v0 + (102)) + temp_lo_2) - 0x19;
    sp58 = (*(s16 *)((unsigned char *)temp_v0 + (6)) + temp_lo_2) - 0x19;
    temp_a0 = temp_lo + D_801392A4;
    temp_t0 = temp_lo_2 + D_801392A8;
    var_s4 = temp_a0 + 0x27;
    var_s5 = temp_a0 + 0x3F;
    temp_t2 = temp_t0 + 0x27;
    var_fp = temp_t0 + 0x3F;
    sp68 = (*(s16 *)((unsigned char *)temp_v0 + (14)) + temp_lo_2) - 1;
    sp70 = (*(s16 *)((unsigned char *)temp_v0 + (110)) + temp_lo_2) - 1;
    var_s1 = var_s4;
    var_s3 = var_s5;
    var_s2 = temp_t2;
    var_s6 = temp_t2;
    var_s7 = var_fp;
    if (var_s4 >= 0xC1) {
        do {
            var_s1 -= 0x80;
            var_s3 -= 0x80;
            var_s4 -= 0x80;
            var_s5 -= 0x80;
        } while (var_s1 >= 0xC1);
    }
    if (temp_t2 >= 0xC1) {
        do {
            var_s2 -= 0x80;
            var_s6 -= 0x80;
            var_s7 -= 0x80;
            var_fp -= 0x80;
        } while (var_s2 >= 0xC1);
    }
    temp_v0_2 = func_800490F0(0x28, D_8011ECA0, temp_lo, temp_lo_2);
    func_8009F040(temp_v0_2);
    func_8009EFB0(temp_v0_2, 0);
    func_8009EF84(temp_v0_2, 1);
    *(s8 *)((unsigned char *)temp_v0_2 + (4)) = (s8) D_8013921C;
    *(s8 *)((unsigned char *)temp_v0_2 + (5)) = (s8) D_8013921C;
    *(s8 *)((unsigned char *)temp_v0_2 + (6)) = (s8) D_8013921C;
    *(s16 *)((unsigned char *)temp_v0_2 + (14)) = func_8009ED74(0, 0x1FF);
    *(s16 *)((unsigned char *)temp_v0_2 + (22)) = func_8009ECB0(1, 4, 0x1C0, 0);
    *(s16 *)((unsigned char *)temp_v0_2 + (8)) = (s16) sp54;
    temp_v1 = (D_8011ECA0 * -0xF0) + 0xF0;
    *(s16 *)((unsigned char *)temp_v0_2 + (10)) = (s16) (sp58 + temp_v1);
    *(s16 *)((unsigned char *)temp_v0_2 + (16)) = (s16) (sp5C + 1);
    *(s16 *)((unsigned char *)temp_v0_2 + (18)) = (s16) (sp60 + temp_v1);
    *(s16 *)((unsigned char *)temp_v0_2 + (24)) = (s16) sp64;
    *(s16 *)((unsigned char *)temp_v0_2 + (26)) = (s16) (sp68 + temp_v1 + 1);
    *(s16 *)((unsigned char *)temp_v0_2 + (32)) = (s16) (sp6C + 1);
    *(s16 *)((unsigned char *)temp_v0_2 + (34)) = (s16) (sp70 + temp_v1 + 1);
    *(s8 *)((unsigned char *)temp_v0_2 + (20)) = (s8) (var_s3 + 1);
    *(s8 *)((unsigned char *)temp_v0_2 + (36)) = (s8) (var_s5 + 1);
    *(s8 *)((unsigned char *)temp_v0_2 + (37)) = (s8) (var_fp + 1);
    *(s8 *)((unsigned char *)temp_v0_2 + (29)) = (s8) (var_s7 + 1);
    *(s8 *)((unsigned char *)temp_v0_2 + (28)) = var_s4;
    *(s8 *)((unsigned char *)temp_v0_2 + (21)) = var_s6;
    *(s8 *)((unsigned char *)temp_v0_2 + (13)) = var_s2;
    *(s8 *)((unsigned char *)temp_v0_2 + (12)) = var_s1;
    func_8009EED0((D_8011ECA0 << 0xA) + 0x14 + D_800E8CA0, temp_v0_2);
}
