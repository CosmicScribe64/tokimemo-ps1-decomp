void func_80133CA8(void) {
    s16 temp_v1;
    s16 temp_v1_2;
    s32 var_s2;
    u32 temp_t7;
    u8 *var_s0;
    u8 *var_s1;

    var_s1 = D_8011ECD0;
    var_s0 = D_8013984C;
    var_s2 = 0;
    do {
        if ((*(s32 *)var_s0) & 0x80) {
            *(s8 *)((unsigned char *)var_s1 + (8705)) = 3;
            *(s32 *)((unsigned char *)var_s0 + (16)) = (s32) (*(s32 *)((unsigned char *)var_s0 + (16)) + 4);
            func_80133F00(var_s2, 1);
        } else {
            *(s8 *)((unsigned char *)var_s1 + (8705)) = 7;
            *(s32 *)((unsigned char *)var_s0 + (16)) = (s32) (*(s32 *)((unsigned char *)var_s0 + (16)) + 1);
            func_80133F00(var_s2, 0);
        }
        if (*(s32 *)((unsigned char *)var_s0 + (16)) >= 0xF) {
            *(s32 *)((unsigned char *)var_s0 + (16)) = 0;
            *(s32 *)((unsigned char *)var_s0 + (12)) = (s32) (*(s32 *)((unsigned char *)var_s0 + (12)) + 1);
        }
        temp_t7 = (*(s32 *)var_s0) & 0x7F;
        switch (temp_t7) {
        default:
            *(s8 *)((unsigned char *)var_s1 + (8707)) = 0;
            break;
        case 1:
        case 2:
        case 3:
            if (*(s32 *)((unsigned char *)var_s0 + (12)) >= 6) {
                *(s32 *)((unsigned char *)var_s0 + (12)) = 0;
            }
            temp_v1 = *(&D_80139A34 + (func_80134008(var_s2) * 0xC) + (*(s32 *)((unsigned char *)var_s0 + (12)) * 2));
            if (temp_v1 & 0x80) {
                *(s8 *)((unsigned char *)var_s1 + (8707)) = 0x85;
            } else {
                *(s8 *)((unsigned char *)var_s1 + (8707)) = 0x84;
            }
            *(s16 *)((unsigned char *)var_s1 + (8728)) = (s16) (temp_v1 & 0x7F);
            *(s16 *)((unsigned char *)var_s1 + (8742)) = (s16) *(s16 *)((unsigned char *)var_s0 + (22));
            *(u8 *)((unsigned char *)var_s1 + (8771)) = (u8) D_80139A2C[(*(s32 *)var_s0) & 0x7F];
            *(s16 *)((unsigned char *)var_s1 + (8746)) = (s16) *(s16 *)((unsigned char *)var_s0 + (26));
            break;
        case 4:
            if (*(s32 *)((unsigned char *)var_s0 + (12)) >= 4) {
                *(s32 *)((unsigned char *)var_s0 + (12)) = 0;
            }
            temp_v1_2 = *(s16 *)((unsigned char *)(&D_80139A34 + (func_80134008(var_s2) * 0xC) + (*(s32 *)((unsigned char *)var_s0 + (12)) * 2)) + (96));
            if (temp_v1_2 & 0x80) {
                *(s8 *)((unsigned char *)var_s1 + (8707)) = 0x85;
            } else {
                *(s8 *)((unsigned char *)var_s1 + (8707)) = 0x84;
            }
            *(s16 *)((unsigned char *)var_s1 + (8728)) = (s16) (temp_v1_2 & 0x7F);
            *(s16 *)((unsigned char *)var_s1 + (8742)) = (s16) *(s16 *)((unsigned char *)var_s0 + (22));
            *(u8 *)((unsigned char *)var_s1 + (8771)) = (u8) D_80139A2C[(*(s32 *)var_s0) & 0x7F];
            *(s16 *)((unsigned char *)var_s1 + (8746)) = (s16) *(s16 *)((unsigned char *)var_s0 + (26));
            break;
        }
        var_s2 += 1;
        var_s0 += 0x30;
        var_s1 += 0x44;
    } while (var_s2 != 0xA);
}
