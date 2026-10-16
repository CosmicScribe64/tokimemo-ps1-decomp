extern ? D_801500A0;

void func_8013FC64(s32 a, s32 b, s32 c, s32 d) {
    s32 sp64;
    s32 sp5C;
    s32 temp_v0;
    s32 temp_v0_3;
    s32 var_s1;
    s32 var_s2;
    s32 var_s5;
    void *temp_v0_2;

    if (a < 0) {
        sp64 = 1;
        var_s1 = -a;
    } else {
        sp64 = 0;
        var_s1 = a;
    }
    var_s5 = 0;
    if (var_s1 >= 0) {
        sp5C = d * 0x10;
        var_s2 = 0;
loop_5:
        temp_v0 = func_800490F0(0x14, D_8011ECA0);
        func_8009F0A4(temp_v0);
        *(s16 *)((unsigned char *)temp_v0 + (16)) = 8;
        *(s16 *)((unsigned char *)temp_v0 + (18)) = 8;
        func_8009EF84(temp_v0, 0);
        *(s8 *)((unsigned char *)temp_v0 + (4)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0 + (5)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0 + (6)) = 0x80;
        *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + b) - var_s2);
        *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + c);
        temp_v0_2 = &D_801500A0 + ((var_s1 % 10) * 2);
        *(u8 *)((unsigned char *)temp_v0 + (12)) = (u8) temp_v0_2->unk0;
        *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) *(u8 *)((unsigned char *)temp_v0_2 + (1));
        *(s16 *)((unsigned char *)temp_v0 + (14)) = func_8009ED74(sp5C, 0x1FD);
        func_8009EED0((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0);
        if (var_s1 >= 0xA) {
            var_s5 += 1;
            var_s2 += 8;
            var_s1 = var_s1 / 10;
            if (var_s1 >= 0) {
                goto loop_5;
            }
        }
    }
    if (sp64 != 0) {
        sp5C = d * 0x10;
        temp_v0_3 = func_800490F0(0x14, D_8011ECA0);
        func_8009F0A4(temp_v0_3);
        *(s16 *)((unsigned char *)temp_v0_3 + (16)) = 8;
        *(s16 *)((unsigned char *)temp_v0_3 + (18)) = 8;
        func_8009EF84(temp_v0_3, 0);
        *(s8 *)((unsigned char *)temp_v0_3 + (4)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0_3 + (5)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0_3 + (6)) = 0x80;
        *(s16 *)((unsigned char *)temp_v0_3 + (8)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + b) - ((var_s5 + 1) * 8));
        *(s16 *)((unsigned char *)temp_v0_3 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + c);
        *(u8 *)((unsigned char *)temp_v0_3 + (12)) = (u8) D_801500B4;
        *(u8 *)((unsigned char *)temp_v0_3 + (13)) = (u8) D_801500B5;
        *(s16 *)((unsigned char *)temp_v0_3 + (14)) = func_8009ED74(sp5C, 0x1FD);
        func_8009EED0((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0_3);
    }
    func_800494BC(0x3C0, 0, 1, 0, 1);
}
