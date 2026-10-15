? func_800A1070(? *);                               /* extern */
extern ? D_800E6298;
extern ? D_80129C40;

void func_80140ADC(s32 arg0, s32 arg1) {
    ? spA0;
    s16 sp9C;
    s16 sp9A;
    s16 sp98;
    s16 sp88;
    ? sp74;
    ? sp70;
    s32 sp64;
    s16 *var_s1;
    s16 temp_s3;
    s16 temp_v0;
    s32 temp_v0_2;
    s32 var_fp;
    s32 var_s0;
    s32 var_s2_2;
    s8 temp_a1;
    s8 temp_v1;
    void *temp_s7;
    void *var_s2;

    spA0.unk0 = (s32) D_80129C40.unk0;
    *(s32 *)((unsigned char *)(&spA0) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (4));
    *(s32 *)((unsigned char *)(&spA0) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (8));
    *(s32 *)((unsigned char *)(&spA0) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (12));
    *(s32 *)((unsigned char *)(&spA0) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (16));
    *(s32 *)((unsigned char *)(&spA0) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (20));
    *(s32 *)((unsigned char *)(&spA0) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (24));
    *(s32 *)((unsigned char *)(&spA0) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (28));
    func_800A1100((s32) &spA0);
    func_800A1070(&spA0);
    temp_s7 = (arg1 * 0x14) + arg0;
    sp64 = 0;
    var_fp = 0;
    sp9C = *(s16 *)((unsigned char *)temp_s7 + (4));
    do {
        var_s1 = &sp88;
        sp9A = (*(s16 *)((unsigned char *)temp_s7 + (2)) + sp64) - 0x10;
        var_s2 = var_fp + var_s1;
        var_s0 = 0;
loop_2:
        sp98 = (temp_s7->unk0 + var_s0) - 0x10;
        func_800AB170((s32) &sp98, (s32) var_s2, (s32) &sp74, (s32) &sp70);
        var_s1 += 8;
        var_s0 += 0x20;
        var_s2 += 8;
        if (var_s1 != &sp98) {
            goto loop_2;
        }
        var_fp += 4;
        sp64 += 0x20;
    } while (var_fp != 8);
    if ((sp90 >= -0x2C0) && (sp88 < 0x2C1)) {
        temp_v0 = *(&D_800E6298 + (D_8011ECA0 * 2));
        if (sp8E >= (temp_v0 - 0x200)) {
            if ((temp_v0 + 0x200) >= sp96) {
                temp_s3 = ((u8) *(u8 *)((unsigned char *)temp_s7 + (18)) >> 4) & 1;
                var_s2_2 = (s32) (D_80122754 - *(s16 *)((unsigned char *)temp_s7 + (4))) / 127;
                if (var_s2_2 >= 0x100) {
                    var_s2_2 = 0xFF;
                    goto block_11;
                }
                if (var_s2_2 > 0) {
block_11:
                    temp_v0_2 = func_800490F0(0x28, D_8011ECA0);
                    func_8009F040(temp_v0_2);
                    func_8009EF84(temp_v0_2, 0);
                    *(s8 *)((unsigned char *)temp_v0_2 + (4)) = 0x80;
                    *(s8 *)((unsigned char *)temp_v0_2 + (5)) = 0x80;
                    *(s8 *)((unsigned char *)temp_v0_2 + (6)) = 0x80;
                    *(s16 *)((unsigned char *)temp_v0_2 + (22)) = func_8009ECB0(1, 1, 0x3C0, 0x100);
                    temp_v1 = temp_s3 << 5;
                    temp_a1 = temp_v1 + 0x20;
                    *(s16 *)((unsigned char *)temp_v0_2 + (14)) = func_8009ED74(0x100, 0x1F8);
                    *(s8 *)((unsigned char *)temp_v0_2 + (20)) = temp_a1;
                    *(s8 *)((unsigned char *)temp_v0_2 + (36)) = temp_a1;
                    *(s8 *)((unsigned char *)temp_v0_2 + (13)) = 0x60;
                    *(s8 *)((unsigned char *)temp_v0_2 + (21)) = 0x60;
                    *(s8 *)((unsigned char *)temp_v0_2 + (12)) = temp_v1;
                    *(s8 *)((unsigned char *)temp_v0_2 + (28)) = temp_v1;
                    *(s8 *)((unsigned char *)temp_v0_2 + (29)) = 0x80;
                    *(s8 *)((unsigned char *)temp_v0_2 + (37)) = 0x80;
                    *(s16 *)((unsigned char *)temp_v0_2 + (8)) = sp88;
                    *(s16 *)((unsigned char *)temp_v0_2 + (10)) = sp8A;
                    *(s16 *)((unsigned char *)temp_v0_2 + (16)) = sp8C;
                    *(s16 *)((unsigned char *)temp_v0_2 + (18)) = sp8E;
                    *(s16 *)((unsigned char *)temp_v0_2 + (24)) = sp90;
                    *(s16 *)((unsigned char *)temp_v0_2 + (26)) = sp92;
                    *(s16 *)((unsigned char *)temp_v0_2 + (32)) = sp94;
                    *(s16 *)((unsigned char *)temp_v0_2 + (34)) = sp96;
                    func_8009EED0(&D_800E8CA0[(D_8011ECA0 << 0xA) + (var_s2_2 * 4)], temp_v0_2, 0x60);
                }
            }
        }
    }
}
