? func_800A1070(? *);                               /* extern */
extern s16 D_8015F5F2;

void func_80144430(void) {
    ? spB0;
    s16 spAC;
    s16 spAA;
    s16 spA8;
    ? sp80;
    ? sp6C;
    ? sp68;
    s32 sp60;
    ? *var_s2;
    s32 temp_v0;
    s32 var_fp;
    s32 var_fp_2;
    s32 var_s0;
    s32 var_s3;
    s8 temp_a0;
    s8 temp_t2;
    void *var_s1;
    void *var_s1_2;

    spB0.unk0 = (s32) D_80129C40.unk0;
    *(s32 *)((unsigned char *)(&spB0) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (4));
    *(s32 *)((unsigned char *)(&spB0) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (8));
    *(s32 *)((unsigned char *)(&spB0) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (12));
    *(s32 *)((unsigned char *)(&spB0) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (16));
    *(s32 *)((unsigned char *)(&spB0) + (20)) = (s32) D_80129C40.t[0];
    *(s32 *)((unsigned char *)(&spB0) + (24)) = (s32) D_80129C40.t[1];
    *(s32 *)((unsigned char *)(&spB0) + (28)) = (s32) D_80129C40.t[2];
    func_800A1100((s32) &spB0);
    func_800A1070(&spB0);
    spAC = -0x7FFF;
    var_fp = 0;
    sp60 = 0;
    do {
        var_s2 = &sp80;
        spAA = D_8015F5F2 + (sp60 / 4) + 0x80;
        var_s1 = var_fp + var_s2;
        var_s0 = 0x4000;
loop_2:
        spA8 = var_s0 + D_8012274C;
        func_800AB170((s32) &spA8, (s32) var_s1, (s32) &sp6C, (s32) &sp68);
        var_s2 += 8;
        var_s0 -= 0x2000;
        var_s1 += 8;
        if (var_s2 != &spA8) {
            goto loop_2;
        }
        var_fp += 4;
        sp60 -= 0x1000;
    } while (var_fp < 8);
    var_fp_2 = 0;
    do {
        var_s1_2 = var_fp_2 + &sp80;
        var_s3 = 0;
loop_6:
        temp_v0 = func_800490F0(0x28, D_8011ECA0);
        func_8009F040(temp_v0);
        *(s8 *)((unsigned char *)temp_v0 + (4)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0 + (5)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0 + (6)) = 0x80;
        *(s16 *)((unsigned char *)temp_v0 + (22)) = func_8009ECB0(1, 0, 0x200, 0x100);
        temp_t2 = (var_s3 & 1) << 6;
        temp_a0 = temp_t2 + 0x3F;
        *(s16 *)((unsigned char *)temp_v0 + (14)) = func_8009ED74(0, 0x1EE);
        *(s8 *)((unsigned char *)temp_v0 + (13)) = temp_a0;
        *(s8 *)((unsigned char *)temp_v0 + (29)) = temp_a0;
        *(s8 *)((unsigned char *)temp_v0 + (21)) = temp_t2;
        *(s8 *)((unsigned char *)temp_v0 + (37)) = temp_t2;
        *(s8 *)((unsigned char *)temp_v0 + (12)) = 0;
        *(s8 *)((unsigned char *)temp_v0 + (20)) = 0;
        *(s8 *)((unsigned char *)temp_v0 + (28)) = 0xFF;
        *(s8 *)((unsigned char *)temp_v0 + (36)) = 0xFF;
        *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) var_s1_2->unk0;
        *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (2));
        *(s16 *)((unsigned char *)temp_v0 + (16)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (4));
        *(s16 *)((unsigned char *)temp_v0 + (18)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (6));
        *(s16 *)((unsigned char *)temp_v0 + (24)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (8));
        *(s16 *)((unsigned char *)temp_v0 + (26)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (10));
        *(s16 *)((unsigned char *)temp_v0 + (32)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (12));
        *(s16 *)((unsigned char *)temp_v0 + (34)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (14));
        func_8009EED0(D_800E8CA0 + (D_8011ECA0 << 0xA) + 0x3F4, temp_v0);
        var_s3 += 1;
        var_s1_2 += 8;
        if (var_s3 != 4) {
            goto loop_6;
        }
        var_fp_2 += 4;
    } while (var_fp_2 != 4);
}
