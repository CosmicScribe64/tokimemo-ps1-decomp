? func_800A1070(? *);                               /* extern */
extern ? D_800E6298;
extern u16 D_8015F580;

void func_801413B8(Tc14 *arg0, s32 arg1) {
    ? spA8;
    s16 spA4;
    s16 spA2;
    s16 spA0;
    s16 sp90;
    ? sp7C;
    ? sp78;
    u16 sp6C;
    s32 sp60;
    Tc14 *temp_s7;
    s16 *var_s1;
    s16 temp_v0;
    s32 temp_s2;
    s32 temp_v0_2;
    s32 var_fp;
    s32 var_s0;
    s32 var_s1_2;
    u8 *temp_v1;
    void *var_s2;

    sp6C = D_8015F580;
    spA8.unk0 = (s32) D_80129C40.unk0;
    *(s32 *)((unsigned char *)(&spA8) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (4));
    *(s32 *)((unsigned char *)(&spA8) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (8));
    *(s32 *)((unsigned char *)(&spA8) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (12));
    *(s32 *)((unsigned char *)(&spA8) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (16));
    *(s32 *)((unsigned char *)(&spA8) + (20)) = (s32) D_80129C40.t[0];
    *(s32 *)((unsigned char *)(&spA8) + (24)) = (s32) D_80129C40.t[1];
    *(s32 *)((unsigned char *)(&spA8) + (28)) = (s32) D_80129C40.t[2];
    func_800A1100((s32) &spA8);
    func_800A1070(&spA8);
    temp_s7 = &arg0[arg1];
    sp60 = 0;
    var_fp = 0;
    spA4 = temp_s7->unk4;
    do {
        var_s1 = &sp90;
        spA2 = (temp_s7->unk2 + sp60) - 0x28;
        var_s2 = var_fp + var_s1;
        var_s0 = 0;
loop_2:
        spA0 = (temp_s7->unk0 + var_s0) - 0x28;
        func_800AB170((s32) &spA0, (s32) var_s2, (s32) &sp7C, (s32) &sp78);
        var_s1 += 8;
        var_s0 += 0x50;
        var_s2 += 8;
        if (var_s1 != &spA0) {
            goto loop_2;
        }
        var_fp += 4;
        sp60 += 0x50;
    } while (var_fp != 8);
    if ((sp98 >= -0x2C0) && (sp90 < 0x2C1)) {
        temp_v0 = *(&D_800E6298 + (D_8011ECA0 * 2));
        if (sp96 >= (temp_v0 - 0x200)) {
            if ((temp_v0 + 0x200) >= sp9E) {
                temp_s2 = ((u8) temp_s7->unk12 >> 2) & 1;
                var_s1_2 = (s32) (D_80122754 - temp_s7->unk4) / 127;
                if (var_s1_2 >= 0x100) {
                    var_s1_2 = 0xFF;
                } else if (var_s1_2 <= 0) {
                    var_s1_2 = 1;
                }
                temp_v0_2 = func_800490F0(0x28, D_8011ECA0);
                func_8009F040(temp_v0_2);
                func_8009EF84(temp_v0_2, 1);
                *(s8 *)((unsigned char *)temp_v0_2 + (4)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_2 + (5)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_2 + (6)) = 0x80;
                *(s16 *)((unsigned char *)temp_v0_2 + (22)) = func_8009ECB0(0, 1, 0x380, 0x100);
                *(s16 *)((unsigned char *)temp_v0_2 + (14)) = func_8009ED74(0x80, 0x1E4);
                temp_v1 = temp_s2 + &sp6C;
                *(s8 *)((unsigned char *)temp_v0_2 + (13)) = 1;
                *(u8 *)((unsigned char *)temp_v0_2 + (12)) = (u8) *temp_v1;
                *(s8 *)((unsigned char *)temp_v0_2 + (21)) = 1;
                *(s8 *)((unsigned char *)temp_v0_2 + (20)) = (s8) (*temp_v1 + 0xE);
                *(s8 *)((unsigned char *)temp_v0_2 + (29)) = 0xF;
                *(u8 *)((unsigned char *)temp_v0_2 + (28)) = (u8) *temp_v1;
                *(s8 *)((unsigned char *)temp_v0_2 + (37)) = 0xF;
                *(s8 *)((unsigned char *)temp_v0_2 + (36)) = (s8) (*temp_v1 + 0xE);
                *(s16 *)((unsigned char *)temp_v0_2 + (8)) = sp90;
                *(s16 *)((unsigned char *)temp_v0_2 + (10)) = sp92;
                *(s16 *)((unsigned char *)temp_v0_2 + (16)) = sp94;
                *(s16 *)((unsigned char *)temp_v0_2 + (18)) = sp96;
                *(s16 *)((unsigned char *)temp_v0_2 + (24)) = sp98;
                *(s16 *)((unsigned char *)temp_v0_2 + (26)) = sp9A;
                *(s16 *)((unsigned char *)temp_v0_2 + (32)) = sp9C;
                *(s16 *)((unsigned char *)temp_v0_2 + (34)) = sp9E;
                func_8009EED0(&D_800E8CA0[(D_8011ECA0 << 0xA) + (var_s1_2 * 4)], temp_v0_2);
            }
        }
    }
}
