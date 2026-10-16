extern u8 D_8012097F;
extern u8 D_801209C3;
extern u8 D_80120A07;
extern u8 D_80120A4B;
extern s16 D_80120B72;
extern u8 D_80120B9F;
extern u8 D_8014920A;
extern u8 D_8014922E;
extern u8 D_80149252;
extern ? D_80149274;
extern u8 D_80149276;

void func_80135F3C(void) {
    ? sp68;
    TaiikuSlots *var_s1;
    TaiikuSlots *var_s1_2;
    s16 temp_a0;
    s16 temp_a0_2;
    s16 temp_a1;
    s16 temp_v0_2;
    s16 temp_v1_2;
    s16 var_v1;
    s32 temp_hi;
    s32 temp_hi_2;
    s32 temp_hi_3;
    s32 temp_lo;
    s32 temp_s7;
    s32 var_a0;
    s32 var_a1;
    s32 var_s5;
    u8 *var_v0;
    u8 temp_t4;
    u8 temp_t7;
    u8 temp_v0;
    u8 var_a2;
    u8 var_a2_2;
    void *temp_s1;
    void *temp_v1;
    void *var_s0;

    M2C_MEMCPY_ALIGNED(&spF0[0], &D_80149728, 0x2C);
    spD8[0].unk0 = (s32) D_80149754.unk0;
    *(s32 *)((unsigned char *)(&spD8[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80149754) + (4));
    *(s32 *)((unsigned char *)(&spD8[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80149754) + (8));
    *(s32 *)((unsigned char *)(&spD8[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80149754) + (12));
    *(s32 *)((unsigned char *)(&spD8[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80149754) + (16));
    *(u16 *)((unsigned char *)(&spD8[0]) + (20)) = (u16) D_80149754.v[0xA];
    spC0[0].unk0 = (s32) D_8014976C.unk0;
    *(s32 *)((unsigned char *)(&spC0[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014976C) + (4));
    *(s32 *)((unsigned char *)(&spC0[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8014976C) + (8));
    *(s32 *)((unsigned char *)(&spC0[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8014976C) + (12));
    *(s32 *)((unsigned char *)(&spC0[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8014976C) + (16));
    *(u16 *)((unsigned char *)(&spC0[0]) + (20)) = (u16) D_8014976C.v[0xA];
    M2C_MEMCPY_ALIGNED(&sp68, &D_80149784, 0x58);
    temp_t4 = ((func_800AE0D0() % 20) + 0xA) & 0xFF;
    var_a2 = temp_t4;
    if (temp_t4 == 0x13) {
        D_8014920A = temp_t4;
        do {
            temp_t7 = ((func_800AE0D0() % 20) + 0xA) & 0xFF;
            var_a2 = temp_t7;
            D_8014920A = temp_t7;
        } while (temp_t7 == 0x13);
    }
    *(s16 *)((unsigned char *)(D_8011ECD0 + (var_a2 * 0x44)) + (3150)) = (s16) (&spD8[0])[D_801491EC];
    D_8014920A = var_a2;
    var_s1 = &D_80149208;
    do {
        temp_hi = func_800AE0D0() % 30;
        var_s1->pad24[2] = (u8) temp_hi;
        if ((temp_hi & 0xFF) == 0x13) {
            do {
                temp_hi_2 = func_800AE0D0() % 30;
                var_s1->pad24[2] = (u8) temp_hi_2;
            } while ((temp_hi_2 & 0xFF) == 0x13);
        }
        var_s1 += 0x24;
    } while ((u32) var_s1 < (u32) &D_80149274);
    temp_v0 = func_8008647C();
    D_801491DC = temp_v0;
    if ((func_8008667C(temp_v0 & 0xFF) < 2) && (D_801491DC != (&spF0[0])[D_801491EC])) {
        var_a2_2 = D_801491DC;
        D_801491D8 = (u8) (func_800AE0D0() % 10);
        temp_v1 = D_8011ECD0 + (D_801491D8 * 0x44);
        *(s16 *)((unsigned char *)temp_v1 + (3150)) = (s16) (&spC0[0])[var_a2_2];
        D_80120B72 = (s16) ((s16) *(s16 *)((unsigned char *)temp_v1 + (3150)) >> 3);
        temp_a0 = *(s16 *)((unsigned char *)temp_v1 + (3150));
        D_80120B9F = *(&sp68 + ((temp_a0 >> 3) * 8) + (temp_a0 & 7));
        var_a1 = *(s32 *)((unsigned char *)(sp + (D_801491EC * 4)) + (240));
    } else {
        var_a2_2 = 0xFF;
        var_a1 = *(s32 *)((unsigned char *)(sp + (D_801491EC * 4)) + (240));
    }
    D_801491DC = var_a2_2;
    if (var_a2_2 == var_a1) {
        D_801491DC = 0xFF;
    }
    var_a0 = 0;
    do {
        temp_s7 = var_a0 + 1;
        if (temp_s7 > 0) {
            temp_s1 = (var_a0 * 0x24) + &D_80149208;
            var_v1 = *(s16 *)((unsigned char *)(D_8011ECD0 + (*(u8 *)((unsigned char *)temp_s1 + (38)) * 0x44)) + (3150));
            var_s0 = (var_a0 * 0x24) - 0x24 + &D_80149208;
            var_s5 = 0x24;
            do {
                if ((var_v1 == *(s16 *)((unsigned char *)(D_8011ECD0 + (*(u8 *)((unsigned char *)var_s0 + (38)) * 0x44)) + (3150))) || (var_v1 == *(s16 *)((unsigned char *)(D_8011ECD0 + (D_801491D8 * 0x44)) + (3150)))) {
                    do {
loop_18:
                        temp_hi_3 = func_800AE0D0() % 30;
                        *(u8 *)((unsigned char *)temp_s1 + (38)) = (u8) temp_hi_3;
                        var_v1 = *(s16 *)((unsigned char *)(D_8011ECD0 + ((temp_hi_3 & 0xFF) * 0x44)) + (3150));
                        if (var_v1 == *(s16 *)((unsigned char *)(D_8011ECD0 + (*(u8 *)((unsigned char *)var_s0 + (38)) * 0x44)) + (3150))) {
                            goto loop_18;
                        }
                    } while (var_v1 == *(s16 *)((unsigned char *)(D_8011ECD0 + (D_801491D8 * 0x44)) + (3150)));
                }
                var_s5 += 0x24;
                var_s0 -= 0x24;
            } while ((temp_s7 * 0x24) >= var_s5);
        }
        var_a0 = temp_s7;
    } while (temp_s7 < 3);
    var_v0 = D_8011ECD0;
    var_s1_2 = &D_80149208;
    do {
        var_v0 += 0x44;
        temp_lo = var_s1_2->pad00[2] * 0x44;
        var_s1_2 += 0x24;
        *(s16 *)((unsigned char *)var_v0 + (7230)) = (s16) ((s16) *(s16 *)((unsigned char *)(&D_8011ECD0[temp_lo]) + (3150)) >> 3);
    } while ((u32) var_v0 < (u32) D_8011EDE0);
    temp_v0_2 = *(s16 *)((unsigned char *)(D_8011ECD0 + (D_8014920A * 0x44)) + (3150));
    D_8012097F = *(&sp68 + ((temp_v0_2 >> 3) * 8) + (temp_v0_2 & 7));
    temp_v1_2 = *(s16 *)((unsigned char *)(D_8011ECD0 + (D_8014922E * 0x44)) + (3150));
    D_801209C3 = *(&sp68 + ((temp_v1_2 >> 3) * 8) + (temp_v1_2 & 7));
    temp_a0_2 = *(s16 *)((unsigned char *)(D_8011ECD0 + (D_80149252 * 0x44)) + (3150));
    D_80120A07 = *(&sp68 + ((temp_a0_2 >> 3) * 8) + (temp_a0_2 & 7));
    temp_a1 = *(s16 *)((unsigned char *)(D_8011ECD0 + (D_80149276 * 0x44)) + (3150));
    D_80120A4B = *(&sp68 + ((temp_a1 >> 3) * 8) + (temp_a1 & 7));
}
