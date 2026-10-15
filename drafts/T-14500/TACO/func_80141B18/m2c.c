? func_800A1070(? *);                               /* extern */
extern ? D_80129C40;

void func_80141B18(Tc14 *arg0, s32 arg1) {
    ? spB8;
    s16 spB4;
    s16 spB2;
    s16 spB0;
    s16 spA8;
    s16 spA4;
    s16 spA0;
    ? sp80;
    ? sp7C;
    u8 sp75;
    Tc14 *var_s0;
    s16 *var_s1;
    s16 *var_s2;
    s16 *var_v0;
    s16 temp_v1_2;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s8 temp_a0;
    s8 temp_v1;
    u8 temp_v0;

    temp_v0 = arg0->unk12;
    if (temp_v0 >= 0x81U) {
        sp75 = 0xFF - temp_v0;
    } else {
        sp75 = temp_v0;
    }
    spB8.unk0 = (s32) D_80129C40.unk0;
    *(s32 *)((unsigned char *)(&spB8) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (4));
    *(s32 *)((unsigned char *)(&spB8) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (8));
    *(s32 *)((unsigned char *)(&spB8) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (12));
    *(s32 *)((unsigned char *)(&spB8) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (16));
    *(s32 *)((unsigned char *)(&spB8) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (20));
    *(s32 *)((unsigned char *)(&spB8) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (24));
    *(s32 *)((unsigned char *)(&spB8) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (28));
    func_800A1100((s32) &spB8);
    func_800A1070(&spB8);
    temp_v0_2 = 1 - arg1;
    var_s0 = arg0;
    var_s2 = &spA0;
    var_s1 = &spA4;
    do {
        spB4 = var_s0->unk4 + (temp_v0_2 << 0xB);
        spB2 = var_s0->unk2 + (temp_v0_2 << 8);
        spB0 = var_s0->unk0;
        func_800AB170((s32) &spB0, (s32) var_s2, (s32) &sp80, (s32) &sp7C);
        spB4 = -0x7FFF;
        spB2 = var_s0->unk2 + 0x100;
        spB0 = var_s0->unk0;
        func_800AB170((s32) &spB0, (s32) var_s1, (s32) &sp80, (s32) &sp7C);
        var_s1 += 8;
        var_s0 += 0x14;
        var_s2 += 8;
    } while ((u32) var_s1 < (u32) &spB4);
    temp_v0_3 = func_800490F0(0x24, D_8011ECA0);
    func_8009F054(temp_v0_3);
    func_8009EF84(temp_v0_3, 1);
    temp_a0 = sp75 + 0x10;
    temp_v1 = sp75 + 0x20;
    *(s8 *)((unsigned char *)temp_v0_3 + (13)) = temp_a0;
    *(s8 *)((unsigned char *)temp_v0_3 + (21)) = temp_a0;
    *(s8 *)((unsigned char *)temp_v0_3 + (29)) = temp_a0;
    *(s8 *)((unsigned char *)temp_v0_3 + (5)) = 0x10;
    *(s8 *)((unsigned char *)temp_v0_3 + (4)) = temp_v1;
    *(s8 *)((unsigned char *)temp_v0_3 + (6)) = 0xC0;
    *(s8 *)((unsigned char *)temp_v0_3 + (12)) = temp_v1;
    *(s8 *)((unsigned char *)temp_v0_3 + (14)) = 0xC0;
    *(s8 *)((unsigned char *)temp_v0_3 + (20)) = 0x20;
    *(s8 *)((unsigned char *)temp_v0_3 + (22)) = 0xC0;
    *(s8 *)((unsigned char *)temp_v0_3 + (28)) = temp_v1;
    *(s8 *)((unsigned char *)temp_v0_3 + (30)) = 0xC0;
    var_v0 = &spA0;
    do {
        temp_v1_2 = *(s16 *)((unsigned char *)var_v0 + (8));
        if (var_v0->unk0 == temp_v1_2) {
            *(s16 *)((unsigned char *)var_v0 + (8)) = (s16) (temp_v1_2 + 1);
        }
        var_v0 += 4;
    } while (var_v0 != &spA8);
    *(s16 *)((unsigned char *)temp_v0_3 + (8)) = spA4;
    *(s16 *)((unsigned char *)temp_v0_3 + (10)) = spA6;
    *(s16 *)((unsigned char *)temp_v0_3 + (16)) = spA0;
    *(s16 *)((unsigned char *)temp_v0_3 + (18)) = spA2;
    *(s16 *)((unsigned char *)temp_v0_3 + (24)) = spAC;
    *(s16 *)((unsigned char *)temp_v0_3 + (26)) = spAE;
    *(s16 *)((unsigned char *)temp_v0_3 + (32)) = spA8;
    *(s16 *)((unsigned char *)temp_v0_3 + (34)) = spAA;
    func_8009EED0((D_8011ECA0 << 0xA) + 0x10 + D_800E8CA0, temp_v0_3);
    func_800494BC(0x340, 0x100, 4, 1, 0);
}
