void func_80052F68(void *arg0, void *arg1) {
    ? sp5E;
    s16 sp4E;
    s16 sp4C;
    s16 sp4A;
    s16 *temp_t2;
    s16 *temp_t2_2;
    s16 *var_a2;
    s16 *var_a2_3;
    s16 *var_a2_4;
    s16 *var_v1_4;
    s16 temp_t1;
    s16 temp_t1_2;
    s16 temp_t7;
    s16 temp_t7_2;
    s16 temp_t7_3;
    s16 temp_t9;
    s16 var_v0;
    s32 temp_lo;
    s32 temp_v0;
    s32 var_t0_2;
    s32 var_t1;
    s32 var_t1_2;
    s32 var_v0_2;
    s32 var_v0_3;
    s32 var_v1_2;
    s32 var_v1_3;
    s8 *var_a3;
    s8 *var_t6;
    s8 *var_t9;
    s8 temp_t3;
    u8 *var_a2_2;
    u8 *var_a3_2;
    u8 *var_t0;
    u8 *var_t0_3;
    u8 *var_t1_3;
    u8 *var_t4;
    u8 *var_v1;
    u8 temp_t7_4;

    var_v1 = arg0 + 3;
    temp_t7 = *(u8 *)((unsigned char *)arg0 + (1)) * 4;
    sp4A = temp_t7;
    sp4A = temp_t7 + (D_800E6280.unk_1BC[1].unk_0A * D_800B4349->unk0);
    temp_t7_2 = *(u8 *)((unsigned char *)(arg0 + 1) + (1)) * 4;
    sp4C = temp_t7_2;
    var_t0 = &D_800E6280.unk_044[1].unk_20[4];
    var_a3 = &D_800B4349[2];
    var_a2 = &sp4E;
    sp4C = temp_t7_2 + (D_800E6280.unk_1BC[2].unk_0A * D_800B4349[1]);
    do {
        temp_lo = *(s16 *)((unsigned char *)var_t0 + (454)) * *var_a3;
        *var_a2 = *var_v1 * 4;
        temp_t7_3 = *var_a2;
        var_t0 += 0xE0;
        var_a2 += 8;
        var_v1 += 4;
        var_a3 += 4;
        *(s16 *)((unsigned char *)var_a2 + (-8)) = (s16) (temp_t7_3 + temp_lo);
        *(s16 *)((unsigned char *)var_a2 + (-6)) = (s16) (*(u8 *)((unsigned char *)var_v1 + (-3)) * 4);
        *(s16 *)((unsigned char *)var_a2 + (-6)) = (s16) (*(s16 *)((unsigned char *)var_a2 + (-6)) + (*(s16 *)((unsigned char *)var_t0 + (286)) * *(s8 *)((unsigned char *)var_a3 + (-3))));
        *(s16 *)((unsigned char *)var_a2 + (-4)) = (s16) (*(u8 *)((unsigned char *)var_v1 + (-2)) * 4);
        *(s16 *)((unsigned char *)var_a2 + (-4)) = (s16) (*(s16 *)((unsigned char *)var_a2 + (-4)) + (*(s16 *)((unsigned char *)var_t0 + (342)) * *(s8 *)((unsigned char *)var_a3 + (-2))));
        *(s16 *)((unsigned char *)var_a2 + (-2)) = (s16) (*(u8 *)((unsigned char *)var_v1 + (-1)) * 4);
        *(s16 *)((unsigned char *)var_a2 + (-2)) = (s16) (*(s16 *)((unsigned char *)var_a2 + (-2)) + (*(s16 *)((unsigned char *)var_t0 + (398)) * *(s8 *)((unsigned char *)var_a3 + (-1))));
    } while (var_t0 != &D_800E6280.unk_1BC[3].unk_04);
    var_v0 = D_800E6280.unk_11C.unk_02;
    if (var_v0 >= 0x8D) {
        var_v0 = 0x8C;
    }
    var_t4 = D_800B4361;
    var_a2_2 = D_800B4355;
    var_v1_2 = 1;
    sp4C += D_800B437C[var_v0 / 10];
    do {
        var_v0_2 = 0;
        var_t0_2 = 7 - 0;
loop_6:
        if (((u8) *var_a2_2 >> var_t0_2) & 1) {
            temp_t1 = *(s16 *)((unsigned char *)(&D_800E6280 + -(var_v0_2 * 4)) + (254));
            temp_t2 = &(&sp48[0])[var_v1_2];
            if (var_v0_2 == 7) {
                var_t1 = temp_t1 / 20;
            } else {
                var_t1 = temp_t1 / 10;
            }
            var_t9 = &D_800B436C[var_t1];
            if (var_t1 >= 0xF) {
                var_t9 = &D_800B436C[0xE];
            }
            temp_t3 = *var_t9;
            *temp_t2 += temp_t3;
            *temp_t2 += temp_t3 / 2;
        }
        if (((u8) *var_t4 >> var_t0_2) & 1) {
            temp_t1_2 = *(s16 *)((unsigned char *)(&D_800E6280 + -(var_v0_2 * 4)) + (254));
            temp_t2_2 = &(&sp48[0])[var_v1_2];
            if (var_v0_2 == 7) {
                var_t1_2 = temp_t1_2 / 20;
            } else {
                var_t1_2 = temp_t1_2 / 10;
            }
            var_t6 = &D_800B436C[var_t1_2];
            if (var_t1_2 >= 0xF) {
                var_t6 = &D_800B436C[0xE];
            }
            *temp_t2_2 += *var_t6;
        }
        var_v0_2 += 1;
        var_t0_2 = 7 - var_v0_2;
        if (var_v0_2 < 7) {
            goto loop_6;
        }
        var_v1_2 += 1;
        var_a2_2 += 1;
        var_t4 += 1;
    } while (var_v1_2 != 0xB);
    var_t1_3 = &D_800E6280.unk_039;
    var_a3_2 = &D_800E6280.unk_038;
    var_a2_3 = &sp4A;
    do {
        var_v1_3 = 4;
        var_t0_3 = var_a3_2 + 4;
        var_v0_3 = *(u8 *)((unsigned char *)var_t1_3 + (464)) + *(u8 *)((unsigned char *)var_t1_3 + (465)) + *(u8 *)((unsigned char *)var_t1_3 + (466));
loop_24:
        temp_t7_4 = *(u8 *)((unsigned char *)var_t0_3 + (467));
        var_v1_3 += 4;
        temp_v0 = var_v0_3 + *(u8 *)((unsigned char *)var_t0_3 + (464)) + *(u8 *)((unsigned char *)var_t0_3 + (465)) + *(u8 *)((unsigned char *)var_t0_3 + (466));
        var_t0_3 += 4;
        var_v0_3 = temp_v0 + temp_t7_4;
        if (var_v1_3 != 0x24) {
            goto loop_24;
        }
        var_a3_2 += 0x38;
        if (var_v0_3 >= 4) {
            *var_a2_3 += var_v0_3 * 0xA;
        } else {
            *var_a2_3 = 0;
        }
        var_t1_3 += 0x38;
        var_a2_3 += 2;
    } while ((u32) var_t1_3 < (u32) (&D_800E6280 + 0x269));
    *(s16 *)((unsigned char *)arg1 + (2)) = sp4A;
    var_a2_4 = &sp4E;
    var_v1_4 = arg1 + 6;
    *(s16 *)((unsigned char *)(arg1 + 2) + (2)) = sp4C;
    do {
        temp_t9 = *var_a2_4;
        var_a2_4 += 8;
        *var_v1_4 = temp_t9;
        var_v1_4 += 8;
        *(s16 *)((unsigned char *)var_v1_4 + (-6)) = (s16) *(s16 *)((unsigned char *)var_a2_4 + (-6));
        *(s16 *)((unsigned char *)var_v1_4 + (-4)) = (s16) *(s16 *)((unsigned char *)var_a2_4 + (-4));
        *(s16 *)((unsigned char *)var_v1_4 + (-2)) = (s16) *(s16 *)((unsigned char *)var_a2_4 + (-2));
    } while (var_a2_4 != &sp5E);
}
