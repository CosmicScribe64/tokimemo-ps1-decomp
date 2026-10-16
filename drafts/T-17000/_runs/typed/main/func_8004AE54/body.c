void func_8004AE54(s32 a0, s32 a1, s32 a2, s32 a3) {
    s32 sp5C;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_4;
    s32 var_s1;
    s32 var_s2;
    s32 var_s5;
    u8 *temp_v0_3;

    var_s5 = 0;
    if ((u8) D_800B3D80 >= 4U) {
        if (D_800B3220 != 0) {
            temp_v0 = abs();
            var_s1 = temp_v0;
            if (temp_v0 >= 0) {
                sp5C = (a3 & 7) + 0x1F8;
                var_s2 = 0 * 4;
loop_4:
                temp_v0_2 = GetWorkBase(0x10, D_8011ECA0);
                func_8009F07C(temp_v0_2);
                SetSemiTrans(temp_v0_2, 0);
                *(s8 *)((unsigned char *)temp_v0_2 + (4)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_2 + (5)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_2 + (6)) = 0x80;
                *(s16 *)((unsigned char *)temp_v0_2 + (8)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + a1) - var_s2);
                *(s16 *)((unsigned char *)temp_v0_2 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + a2);
                temp_v0_3 = D_800B3D84 + ((var_s1 % 10) * 2);
                *(u8 *)((unsigned char *)temp_v0_2 + (12)) = (u8) (*(s32 *)temp_v0_3);
                *(u8 *)((unsigned char *)temp_v0_2 + (13)) = (u8) *(u8 *)((unsigned char *)temp_v0_3 + (1));
                *(s16 *)((unsigned char *)temp_v0_2 + (14)) = func_8009ED74(((a3 & 8) * 4) + 0x100, sp5C);
                AddPrim((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0_2);
                if (var_s1 < 0xA) {
                    var_s5 += 1;
                } else {
                    var_s5 += 1;
                    var_s2 += 5;
                    var_s1 = var_s1 / 10;
                    if (var_s1 >= 0) {
                        goto loop_4;
                    }
                }
            }
            if (a0 < 0) {
                sp5C = (a3 & 7) + 0x1F8;
                temp_v0_4 = GetWorkBase(0x10, D_8011ECA0);
                func_8009F07C(temp_v0_4);
                SetSemiTrans(temp_v0_4, 0);
                *(s8 *)((unsigned char *)temp_v0_4 + (4)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_4 + (5)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_4 + (6)) = 0x80;
                *(s16 *)((unsigned char *)temp_v0_4 + (8)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + a1) - (var_s5 * 5));
                *(s16 *)((unsigned char *)temp_v0_4 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + a2);
                *(s8 *)((unsigned char *)temp_v0_4 + (12)) = (s8) (D_800B3D96 + 8);
                *(u8 *)((unsigned char *)temp_v0_4 + (13)) = (u8) D_800B3D97;
                *(s16 *)((unsigned char *)temp_v0_4 + (14)) = func_8009ED74(((a3 & 8) * 4) + 0x100, sp5C);
                AddPrim((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0_4);
            }
            dtd_on_tpage(0x2C0, 0x100, 1, 0, 0);
        }
    }
}
