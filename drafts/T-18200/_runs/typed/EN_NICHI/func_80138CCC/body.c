void func_80138CCC(void) {
    EnHalf13 sp5C;
    EnHalf64 sp78;
    s32 spD8[1];
    s32 spB8[1];
    u8 sp5B;
    s32 sp4C[1];
    s32 *var_s0;
    DisplayActor44 *var_s0_2;
    DisplayActor44 *var_s0_3;
    DisplayActor44 *var_s0_4;
    GameState *var_v0;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 var_s1_2;
    s32 var_s1_3;
    s32 var_s1_4;
    s8 var_s1;
    u8 var_v1;

    sp78 = D_80139DA0;
    sp5C = D_80139E20;
    var_v1 = 0;
    var_v0 = &D_800E6280;
    var_s1 = 0;
    do {
        if (((u32) (var_v0->unk_1BC[0].unk_0C.w << 0x1E) >> 0x1F) == 1) {
            *(&sp4C + var_v1) = var_s1;
            var_v1 = (var_v1 + 1) & 0xFF;
        }
        var_s1 += 1;
        var_v0 += 0x38;
    } while (var_s1 < 0xB);
    var_s0 = &spB8;
    if ((D_800E7D2F != 0) || (((u32) (D_800E6280.unk_1BC[0xB].unk_0C.w << 0x14) >> 0x1D) != 0)) {
        *(&sp4C + var_v1) = 0xB;
        var_v1 = (var_v1 + 1) & 0xFF;
    }
    if (D_800E7D30 != 0) {
        *(&sp4C + var_v1) = 0xC;
        var_v1 = (var_v1 + 1) & 0xFF;
    }
    do {
        sp5B = var_v1;
        var_s0 += 2;
        *(s16 *)((unsigned char *)var_s0 + (-2)) = (s16) sp5C.value[*(&sp4C + (func_800AE0D0() % var_v1))];
    } while ((u32) var_s0 < (u32) &spD8);
    var_s0_2 = ((DisplayActor44 *)D_8011ECD0);
    var_s1_2 = 0;
    do {
        var_s0_2[105].unk_26 = (s16) (var_s0_2[105].unk_26 + 1);
        if (var_s0_2[105].unk_26 >= 0xC1) {
            temp_v1 = func_800AE0D0() & 0x3F;
            var_s0_2[105].unk_18 = (s16) sp78.value[temp_v1];
            var_s0_2[105].unk_26 = (s16) (*(s16 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)[(s32) (var_s1_2 + 1) % 4]) + (7178)) - 0x60);
            if ((temp_v1 & 1) && (temp_v1 < 0x20)) {
                var_s0_2[105].unk_2A = -0x38;
            } else {
                var_s0_2[105].unk_2A = -0x48;
            }
            var_s0_2[105].unk_38 = 0x01000000;
        }
        var_s1_2 += 1;
        var_s0_2 += 1;
    } while (var_s1_2 < 4);
    var_s0_3 = ((DisplayActor44 *)D_8011ECD0);
    var_s1_3 = 0;
    do {
        *(s32 *)((u8 *)&var_s0_3[109] + 36) = (s32) (*(s32 *)((u8 *)&var_s0_3[109] + 36) + 0xFFFF2000);
        if (var_s0_3[109].unk_26 < -0xC0) {
            temp_v1_2 = func_800AE0D0() & 0x3F;
            var_s0_3[109].unk_18 = (s16) sp78.value[temp_v1_2];
            var_s0_3[109].unk_26 = (s16) (*(s16 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)[(s32) (var_s1_3 + 3) % 4]) + (7450)) + 0x60);
            if ((temp_v1_2 & 1) && (temp_v1_2 < 0x20)) {
                var_s0_3[109].unk_2A = -0x10;
            } else {
                var_s0_3[109].unk_2A = -0x20;
            }
            var_s0_3[109].unk_38 = 0x01000000;
        }
        var_s1_3 += 1;
        var_s0_3 += 1;
    } while (var_s1_3 < 4);
    var_s0_4 = ((DisplayActor44 *)D_8011ECD0);
    var_s1_4 = 0;
    do {
        *(s32 *)((u8 *)&var_s0_4[113] + 36) = (s32) (*(s32 *)((u8 *)&var_s0_4[113] + 36) + 0xC000);
        if (var_s0_4[113].unk_26 >= 0xC1) {
            temp_v1_3 = func_800AE0D0() & 0x3F;
            var_s0_4[113].unk_18 = (s16) sp78.value[temp_v1_3];
            var_s0_4[113].unk_26 = (s16) (*(s16 *)((unsigned char *)(&((DisplayActor44 *)D_8011ECD0)[(s32) (var_s1_4 + 1) % 4]) + (7722)) - 0x60);
            if ((temp_v1_3 & 1) && (temp_v1_3 < 0x20)) {
                var_s0_4[113].unk_2A = 0x18;
            } else {
                var_s0_4[113].unk_2A = 8;
            }
            var_s0_4[113].unk_38 = 0x01000000;
        }
        var_s1_4 += 1;
        var_s0_4 += 1;
    } while (var_s1_4 != 4);
}
