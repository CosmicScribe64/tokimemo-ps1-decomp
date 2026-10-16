void func_8013F468(void) {
    s16 screen_xy[8];
    s32 spA0[8];
    s16 sp9C;
    s16 sp9A;
    s16 sp98;
    s32 sp6C[1];
    s32 sp68[1];
    s16 *var_s1;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_fp;
    s32 var_s0;
    s32 var_s7;
    u8 *var_s2;

    (*(s32 *)&spA0) = (s32) D_80129C40;
    *(s32 *)((unsigned char *)(&spA0) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (4));
    *(s32 *)((unsigned char *)(&spA0) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (8));
    *(s32 *)((unsigned char *)(&spA0) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (12));
    *(s32 *)((unsigned char *)(&spA0) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (16));
    *(s32 *)((unsigned char *)(&spA0) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (20));
    *(s32 *)((unsigned char *)(&spA0) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (24));
    *(s32 *)((unsigned char *)(&spA0) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (28));
    func_800A1100((s32) &spA0);
    func_800A1070(&spA0);
    sp9C = -0x7000;
    var_fp = 0;
    var_s7 = 0;
    do {
        var_s1 = &screen_xy[0];
        var_s2 = var_s7 + var_s1;
        sp9A = D_8015EDB4->unk76 + var_fp;
        var_s0 = 0;
loop_2:
        sp98 = (D_8015EDB4->unk74 + var_s0) - 0x100;
        func_800AB170((s32) &sp98, (s32) var_s2, (s32) &sp6C, (s32) &sp68);
        var_s1 += 4;
        var_s0 += 0x200;
        var_s2 += 8;
        if (var_s1 != &sp98) {
            goto loop_2;
        }
        var_s7 += 4;
        var_fp += 0x200;
    } while (var_s7 != 8);
    temp_v0 = GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(temp_v0);
    SetSemiTrans(temp_v0, 1);
    *(s8 *)((unsigned char *)temp_v0 + (4)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0 + (5)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0 + (6)) = 0x80;
    *(s16 *)((unsigned char *)temp_v0 + (22)) = func_8009ECB0(0, 1, 0x340, 0x100);
    *(s16 *)((unsigned char *)temp_v0 + (14)) = func_8009ED74(0x10, 0x1EF);
    *(s8 *)((unsigned char *)temp_v0 + (20)) = 0x9F;
    *(s8 *)((unsigned char *)temp_v0 + (36)) = 0x9F;
    *(s8 *)((unsigned char *)temp_v0 + (12)) = 0x81;
    *(s8 *)((unsigned char *)temp_v0 + (28)) = 0x81;
    *(s8 *)((unsigned char *)temp_v0 + (29)) = 0x1F;
    *(s8 *)((unsigned char *)temp_v0 + (37)) = 0x1F;
    *(s8 *)((unsigned char *)temp_v0 + (13)) = 1;
    *(s8 *)((unsigned char *)temp_v0 + (21)) = 1;
    *(s16 *)((unsigned char *)temp_v0 + (8)) = screen_xy[0];
    *(s16 *)((unsigned char *)temp_v0 + (10)) = screen_xy[1];
    *(s16 *)((unsigned char *)temp_v0 + (16)) = screen_xy[2];
    *(s16 *)((unsigned char *)temp_v0 + (18)) = screen_xy[3];
    *(s16 *)((unsigned char *)temp_v0 + (24)) = screen_xy[4];
    *(s16 *)((unsigned char *)temp_v0 + (26)) = screen_xy[5];
    *(s16 *)((unsigned char *)temp_v0 + (32)) = screen_xy[6];
    *(s16 *)((unsigned char *)temp_v0 + (34)) = screen_xy[7];
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 0xA) + 4, temp_v0, 0x1F);
    if (D_800E7378 & 8) {
        temp_v0_2 = GetWorkBase(0x28, D_8011ECA0);
        func_8009F040(temp_v0_2);
        SetSemiTrans(temp_v0_2, 1);
        *(s8 *)((unsigned char *)temp_v0_2 + (4)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0_2 + (5)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0_2 + (6)) = 0x80;
        *(s16 *)((unsigned char *)temp_v0_2 + (22)) = func_8009ECB0(0, 1, 0x340, 0x100);
        *(s16 *)((unsigned char *)temp_v0_2 + (14)) = func_8009ED74(0x10, 0x1EF);
        *(s8 *)((unsigned char *)temp_v0_2 + (29)) = 0x3F;
        *(s8 *)((unsigned char *)temp_v0_2 + (37)) = 0x3F;
        *(s8 *)((unsigned char *)temp_v0_2 + (13)) = 0x21;
        *(s8 *)((unsigned char *)temp_v0_2 + (21)) = 0x21;
        *(s8 *)((unsigned char *)temp_v0_2 + (12)) = 0x81;
        *(s8 *)((unsigned char *)temp_v0_2 + (20)) = 0x9F;
        *(s8 *)((unsigned char *)temp_v0_2 + (28)) = 0x81;
        *(s8 *)((unsigned char *)temp_v0_2 + (36)) = 0x9F;
        *(s16 *)((unsigned char *)temp_v0_2 + (8)) = screen_xy[0];
        *(s16 *)((unsigned char *)temp_v0_2 + (10)) = screen_xy[1];
        *(s16 *)((unsigned char *)temp_v0_2 + (16)) = screen_xy[2];
        *(s16 *)((unsigned char *)temp_v0_2 + (18)) = screen_xy[3];
        *(s16 *)((unsigned char *)temp_v0_2 + (24)) = screen_xy[4];
        *(s16 *)((unsigned char *)temp_v0_2 + (26)) = screen_xy[5];
        *(s16 *)((unsigned char *)temp_v0_2 + (32)) = screen_xy[6];
        *(s16 *)((unsigned char *)temp_v0_2 + (34)) = screen_xy[7];
        AddPrim(D_800E8CA0 + (D_8011ECA0 << 0xA) + 4, temp_v0_2, 0x3F);
    }
}
