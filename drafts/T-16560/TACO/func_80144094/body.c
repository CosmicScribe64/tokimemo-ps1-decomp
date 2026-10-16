void func_80144094(void) {
    MATRIX sp268;
    s16 sp264;
    s16 sp262;
    s16 sp260;
    s32 sp8C[1];
    s32 sp74[1];
    s32 sp70[1];
    s32 sp68;
    s32 sp64;
    s32 sp54;
    s32 *var_s2;
    s16 temp_v0;
    s16 var_fp;
    s32 temp_t8;
    s32 temp_v0_2;
    s32 var_a0;
    s32 var_s0;
    s32 var_s3;
    s32 var_s5;
    u8 *temp_v0_3;
    u8 *temp_v1;
    u8 *var_s1;
    u8 *var_s1_2;

    sp68 = D_8015F5F8;
    sp64 = D_8015F5FC;
    sp268 = D_80129C40;
    func_800A1100((s32) &sp268);
    func_800A1070(&sp268);
    var_fp = 0;
    sp54 = 0;
    sp262 = D_8015F5F0.b;
    do {
        var_s0 = 0x3000;
        if (sp54 == 0x20) {
            sp264 = var_fp;
        } else {
            sp264 = var_fp + D_8015F5F0.c;
        }
        var_s2 = &sp8C;
        var_s1 = sp54 + var_s2;
loop_5:
        sp260 = var_s0 + D_8015F5F0.a;
        func_800AB170((s32) &sp260, (s32) var_s1, (s32) &sp74, (s32) &sp70);
        var_s2 += 9;
        var_s0 -= 0x800;
        var_s1 += 0x24;
        if (var_s2 != &sp260) {
            goto loop_5;
        }
        var_fp -= 0x1000;
        temp_t8 = sp54 + 4;
        sp54 = temp_t8;
    } while (temp_t8 < 0x24);
    var_s5 = 0;
    sp54 = 0;
    do {
        var_s3 = 0;
        var_s1_2 = sp54 + &sp8C;
loop_9:
        if ((*(s16 *)((unsigned char *)var_s1_2 + (40)) >= 0) && (*(s16 *)((unsigned char *)var_s1_2 + (4)) < 0x201)) {
            temp_v0 = D_800E6280.unk_018[D_8011ECA0];
            if ((*(s16 *)((unsigned char *)var_s1_2 + (2)) >= (temp_v0 - 0x200)) && ((temp_v0 + 0x400) >= *(s16 *)((unsigned char *)var_s1_2 + (6)))) {
                temp_v0_2 = GetWorkBase(0x28, D_8011ECA0);
                func_8009F040(temp_v0_2);
                *(s8 *)((unsigned char *)temp_v0_2 + (4)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_2 + (5)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_2 + (6)) = 0x80;
                *(s16 *)((unsigned char *)temp_v0_2 + (22)) = func_8009ECB0(1, 0, 0x200, 0x100);
                *(s16 *)((unsigned char *)temp_v0_2 + (14)) = func_8009ED74(0, 0x1ED);
                var_a0 = (var_s3 + var_s5 + D_8015F5F0.d) & 7;
                if (var_a0 >= 4) {
                    var_a0 = 0;
                }
                temp_v0_3 = var_a0 + &sp68;
                temp_v1 = var_a0 + &sp64;
                *(u8 *)((unsigned char *)temp_v0_2 + (12)) = (u8) *temp_v0_3;
                *(u8 *)((unsigned char *)temp_v0_2 + (13)) = (u8) *temp_v1;
                *(s8 *)((unsigned char *)temp_v0_2 + (20)) = (s8) (*temp_v0_3 + 0x3F);
                *(u8 *)((unsigned char *)temp_v0_2 + (21)) = (u8) *temp_v1;
                *(u8 *)((unsigned char *)temp_v0_2 + (28)) = (u8) *temp_v0_3;
                *(s8 *)((unsigned char *)temp_v0_2 + (29)) = (s8) (*temp_v1 + 0x3F);
                *(s8 *)((unsigned char *)temp_v0_2 + (36)) = (s8) (*temp_v0_3 + 0x3F);
                *(s8 *)((unsigned char *)temp_v0_2 + (37)) = (s8) (*temp_v1 + 0x3F);
                *(s16 *)((unsigned char *)temp_v0_2 + (8)) = (s16) (*(s32 *)var_s1_2);
                *(s16 *)((unsigned char *)temp_v0_2 + (10)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (2));
                *(s16 *)((unsigned char *)temp_v0_2 + (16)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (4));
                *(s16 *)((unsigned char *)temp_v0_2 + (18)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (6));
                *(s16 *)((unsigned char *)temp_v0_2 + (24)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (36));
                *(s16 *)((unsigned char *)temp_v0_2 + (26)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (38));
                *(s16 *)((unsigned char *)temp_v0_2 + (32)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (40));
                *(s16 *)((unsigned char *)temp_v0_2 + (34)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (42));
                AddPrim(D_800E8CA0 + (D_8011ECA0 << 0xA) + 0x3F8, temp_v0_2);
            }
        }
        var_s3 += 1;
        var_s1_2 += 0x24;
        if (var_s3 != 0xC) {
            goto loop_9;
        }
        var_s5 += 1;
        sp54 += 4;
    } while (var_s5 != 8);
}
