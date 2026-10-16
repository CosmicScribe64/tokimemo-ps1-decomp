void func_80133CA8(void) {
    EnObj *var_s0;
    s16 temp_v1;
    s16 temp_v1_2;
    s32 var_s2;
    u32 temp_t7;
    u8 *var_s1;

    var_s1 = D_8011ECD0;
    var_s0 = D_8013984C;
    var_s2 = 0;
    do {
        if (var_s0->unk_00 & 0x80) {
            *(s8 *)((unsigned char *)var_s1 + (8705)) = 3;
            var_s0->unk_10 += 4;
            func_80133F00(var_s2, 1);
        } else {
            *(s8 *)((unsigned char *)var_s1 + (8705)) = 7;
            var_s0->unk_10 += 1;
            func_80133F00(var_s2, 0);
        }
        if (var_s0->unk_10 >= 0xF) {
            var_s0->unk_10 = 0;
            var_s0->unk_0C += 1;
        }
        temp_t7 = var_s0->unk_00 & 0x7F;
        switch (temp_t7) {
        default:
            *(s8 *)((unsigned char *)var_s1 + (8707)) = 0;
            break;
        case 1:
        case 2:
        case 3:
            if (var_s0->unk_0C >= 6) {
                var_s0->unk_0C = 0;
            }
            temp_v1 = D_80139A34[func_80134008(var_s2)][var_s0->unk_0C];
            if (temp_v1 & 0x80) {
                *(s8 *)((unsigned char *)var_s1 + (8707)) = 0x85;
            } else {
                *(s8 *)((unsigned char *)var_s1 + (8707)) = 0x84;
            }
            *(s16 *)((unsigned char *)var_s1 + (8728)) = (s16) (temp_v1 & 0x7F);
            *(s16 *)((unsigned char *)var_s1 + (8742)) = (s16) var_s0->unk_16;
            *(u8 *)((unsigned char *)var_s1 + (8771)) = (u8) D_80139A2C[var_s0->unk_00 & 0x7F];
            *(s16 *)((unsigned char *)var_s1 + (8746)) = (s16) var_s0->unk_1A;
            break;
        case 4:
            if (var_s0->unk_0C >= 4) {
                var_s0->unk_0C = 0;
            }
            temp_v1_2 = *(s16 *)((unsigned char *)(&D_80139A34[func_80134008(var_s2)][var_s0->unk_0C]) + (96));
            if (temp_v1_2 & 0x80) {
                *(s8 *)((unsigned char *)var_s1 + (8707)) = 0x85;
            } else {
                *(s8 *)((unsigned char *)var_s1 + (8707)) = 0x84;
            }
            *(s16 *)((unsigned char *)var_s1 + (8728)) = (s16) (temp_v1_2 & 0x7F);
            *(s16 *)((unsigned char *)var_s1 + (8742)) = (s16) var_s0->unk_16;
            *(u8 *)((unsigned char *)var_s1 + (8771)) = (u8) D_80139A2C[var_s0->unk_00 & 0x7F];
            *(s16 *)((unsigned char *)var_s1 + (8746)) = (s16) var_s0->unk_1A;
            break;
        }
        var_s2 += 1;
        var_s0 += 0x30;
        var_s1 += 0x44;
    } while (var_s2 != 0xA);
}
