void func_801424A8(s32 arg0, s32 arg1) {
    s32 sp98;
    s32 sp96;
    s32 sp92;
    s32 sp9A;
    s32 sp94;
    s32 sp9E;
    s32 sp9C;
    s32 spA8[8];
    s16 spA4;
    s16 spA2;
    s16 spA0;
    s16 sp90;
    s32 sp80;
    s32 sp7C[1];
    s32 sp78[1];
    u16 sp6C;
    u8 *sp64;
    s32 sp60;
    s32 sp54;
    s16 temp_v0_2;
    s32 temp_s1;
    s32 temp_s7;
    s32 temp_t8;
    s32 var_fp;
    s32 var_s0;
    s32 var_s1;
    u8 *temp_v1;
    u8 *temp_v0;
    u8 *temp_v0_3;
    u8 *var_s2;

    sp6C = D_8015F590;
    if (arg1 >= 0x80) {
        var_fp = 0x800;
    } else {
        var_fp = (s32) (0x18000 - (arg1 << 9)) / 256;
    }
    (*(s32 *)&spA8) = (s32) D_80129C40;
    *(s32 *)((unsigned char *)(&spA8) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (4));
    *(s32 *)((unsigned char *)(&spA8) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (8));
    *(s32 *)((unsigned char *)(&spA8) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (12));
    *(s32 *)((unsigned char *)(&spA8) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (16));
    *(s32 *)((unsigned char *)(&spA8) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (20));
    *(s32 *)((unsigned char *)(&spA8) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (24));
    *(s32 *)((unsigned char *)(&spA8) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (28));
    func_800A1100(&spA8);
    func_800A1070(&spA8);
    temp_v0 = (arg0 * 0x14) + &D_8015E770;
    sp60 = 0;
    sp64 = temp_v0;
    sp80 = 0;
    sp54 = 0;
    spA4 = *(s16 *)((unsigned char *)temp_v0 + (4));
    temp_s7 = var_fp / 2;
    do {
        spA2 = (*(s16 *)((unsigned char *)sp64 + (2)) + sp60) - temp_s7;
        var_s0 = 0;
        var_s1 = 0;
        var_s2 = sp54 + &sp90;
loop_5:
        spA0 = ((*(s32 *)sp64) + var_s1) - temp_s7;
        func_800AB170(&spA0, var_s2, &sp7C, &sp78);
        var_s0 += 1;
        var_s1 += var_fp;
        var_s2 += 8;
        if (var_s0 != 2) {
            goto loop_5;
        }
        temp_t8 = sp80 + 1;
        sp54 += 4;
        sp60 += var_fp;
        sp80 = temp_t8;
    } while (temp_t8 != 2);
    if ((sp98 >= -0x2C0) && (sp90 < 0x2C1)) {
        temp_v0_2 = *(&D_800E6298 + (D_8011ECA0 * 2));
        if ((sp96 >= (temp_v0_2 - 0x200)) && ((temp_v0_2 + 0x200) >= sp9E)) {
            temp_s1 = ((u8) *(u8 *)((unsigned char *)sp64 + (18)) >> 2) & 1;
            temp_v0_3 = GetWorkBase(0x28, D_8011ECA0);
            func_8009F040(temp_v0_3);
            SetSemiTrans(temp_v0_3, 1);
            *(s8 *)((unsigned char *)temp_v0_3 + (4)) = 0x80;
            *(s8 *)((unsigned char *)temp_v0_3 + (5)) = 0x80;
            *(s8 *)((unsigned char *)temp_v0_3 + (6)) = 0x80;
            *(s16 *)((unsigned char *)temp_v0_3 + (22)) = func_8009ECB0(0, 1, 0x380, 0x100);
            *(s16 *)((unsigned char *)temp_v0_3 + (14)) = func_8009ED74(0x50, 0x1E7);
            temp_v1 = temp_s1 + &sp6C;
            *(s8 *)((unsigned char *)temp_v0_3 + (13)) = 0;
            *(u8 *)((unsigned char *)temp_v0_3 + (12)) = (u8) *temp_v1;
            *(s8 *)((unsigned char *)temp_v0_3 + (21)) = 0;
            *(s8 *)((unsigned char *)temp_v0_3 + (20)) = (s8) (*temp_v1 + 0xF);
            *(s8 *)((unsigned char *)temp_v0_3 + (29)) = 0xF;
            *(u8 *)((unsigned char *)temp_v0_3 + (28)) = (u8) *temp_v1;
            *(s8 *)((unsigned char *)temp_v0_3 + (37)) = 0xF;
            *(s8 *)((unsigned char *)temp_v0_3 + (36)) = (s8) (*temp_v1 + 0xF);
            *(s16 *)((unsigned char *)temp_v0_3 + (8)) = sp90;
            *(s16 *)((unsigned char *)temp_v0_3 + (10)) = sp92;
            *(s16 *)((unsigned char *)temp_v0_3 + (16)) = sp94;
            *(s16 *)((unsigned char *)temp_v0_3 + (18)) = sp96;
            *(s16 *)((unsigned char *)temp_v0_3 + (24)) = sp98;
            *(s16 *)((unsigned char *)temp_v0_3 + (26)) = sp9A;
            *(s16 *)((unsigned char *)temp_v0_3 + (32)) = sp9C;
            *(s16 *)((unsigned char *)temp_v0_3 + (34)) = sp9E;
            AddPrim((D_8011ECA0 << 0xA) + 0x10 + &D_800E8CA0, temp_v0_3);
        }
    }
}
