void func_801370A0(void) {
    s16 *temp_s1;
    s16 temp_a0;
    s16 temp_a1;
    s16 temp_v0;
    s16 temp_v0_2;
    s16 var_s2;
    s16 var_s2_2;
    s32 temp_hi;
    u8 *temp_s0;
    u8 *temp_s0_2;

    if ((D_80120666 == 0) && (D_80120668 == 7) && (D_80120658 == 2)) {
        D_80120666 = 1;
        D_80120668 = 0;
        D_80120658 = 0;
        D_80120652 = 1;
    }
    var_s2 = 0x61;
    if ((D_80120666 == 2) && (D_80120668 == 1) && (D_80120658 == 4)) {
        D_80120666 = 3;
        D_80120668 = 0;
        D_80120658 = 0;
        D_80120652 = 1;
    }
    do {
        temp_s0 = D_8011ECD0 + (var_s2 * 0x44);
        temp_v0 = *(s16 *)((unsigned char *)temp_s0 + (24));
        if ((temp_v0 < 2) || (temp_v0 >= 8)) {
            *(s32 *)((unsigned char *)temp_s0 + (40)) = (s32) ((*(s32 *)((unsigned char *)temp_s0 + (40)) - ((D_80120666 == 3) * 0x50000)) + 0xFFFB0000);
        } else if ((temp_v0 == 4) || (temp_v0 == 7)) {
            *(s32 *)((unsigned char *)temp_s0 + (40)) = (s32) ((*(s32 *)((unsigned char *)temp_s0 + (40)) - ((D_80120666 == 3) * 0x30000)) + 0xFFFD0000);
        } else {
            *(s32 *)((unsigned char *)temp_s0 + (40)) = (s32) ((*(s32 *)((unsigned char *)temp_s0 + (40)) - ((D_80120666 == 3) << 0x11)) + 0xFFFE0000);
        }
        if (*(s16 *)((unsigned char *)temp_s0 + (42)) < -0x9F) {
            temp_hi = func_800AE0D0() % 320;
            *(s16 *)((unsigned char *)temp_s0 + (42)) = 0x3C;
            *(s16 *)((unsigned char *)temp_s0 + (38)) = (s16) (temp_hi - 0xA0);
            *(s16 *)((unsigned char *)temp_s0 + (24)) = (s16) (func_800AE0D0() % 10);
        }
        var_s2 += 1;
    } while (var_s2 < 0x74);
    var_s2_2 = 0;
    if ((u8) D_80120657 >= 0x78U) {
        do {
            temp_s1 = &D_80139F38 + (var_s2_2 * 2);
            temp_v0_2 = *temp_s1;
            temp_s0_2 = &D_80139F10 + (var_s2_2 * 4);
            temp_a0 = (*(s32 *)temp_s0_2);
            *(s16 *)((unsigned char *)temp_s0_2 + (2)) = (s16) (*(s16 *)((unsigned char *)temp_s0_2 + (2)) - (temp_v0_2 * 4 * ((D_80120666 == 3) + 1)));
            temp_a1 = *(s16 *)((unsigned char *)temp_s0_2 + (2));
            func_80049FF0((s32) temp_a0, (s32) temp_a1, (s32) temp_a0, (s32) (s16) ((temp_v0_2 * 0x14) + temp_a1), 0xFFFFFF, 0xFFFFFF, 0xB);
            if (*(s16 *)((unsigned char *)temp_s0_2 + (2)) < -0xAA) {
                *(s16 *)((unsigned char *)temp_s0_2 + (2)) = 0x3C;
                (*(s32 *)temp_s0_2) = (s16) ((func_800AE0D0() % 320) - 0xA0);
                *temp_s1 = (func_800AE0D0() % 5) + 1;
            }
            var_s2_2 += 1;
        } while (var_s2_2 < 0xA);
    }
}
