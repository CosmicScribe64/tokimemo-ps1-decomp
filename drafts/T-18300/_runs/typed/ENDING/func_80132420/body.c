s32 func_80132420(void *arg0, u8 arg1, s32 arg2, s32 arg3, u16 arg4, u16 arg5, u8 arg6) {
    s16 sp256;
    s32 sp50[1];
    s32 *temp_v1;
    s32 *temp_v1_2;
    s32 *temp_v1_3;
    s32 *temp_v1_4;
    s32 temp_t6;
    s32 temp_t6_2;
    s32 temp_t7;
    s32 temp_t8;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    u8 *temp_v0_10;
    u8 *temp_v0_7;
    u8 *temp_v0_8;
    u8 *temp_v0_9;

    if (arg1 != 0xFF) {
        func_800AE0F0(&sp50, arg0);
    }
    D_80122CF0 = 0;
    D_800CA2A8 = 0;
    switch (arg1) {
    case 0x0:
        D_8013C360 = 0;
        k_speed_set(0x10);
        sp256 = -0x66;
        set_kanji_string(-0x66, 0x48, 0, (s32) "『 ", 0);
        temp_v0 = dialogue_render(-0x58, 0x48, 0, (s32) &sp50, 0, &D_8013BF04, &D_8013BF08, 0);
        D_80122D00 = 1;
        return temp_v0;
    case 0x6:
        D_8013C360 = 0;
        k_speed_set(0x10);
        sp256 = -0x66;
        set_kanji_string(-0x66, 0x48, 0, (s32) "（ ", 0);
        temp_v0_2 = dialogue_render(-0x58, 0x48, 0, (s32) &sp50, 0, "）", &D_8013BF14, 0);
        D_80122D00 = 1;
        return temp_v0_2;
    case 0x1:
        D_8013C360 = 0;
        k_speed_set(8);
        temp_v1 = arg3 + (arg4 * 4);
        temp_t6 = arg5 * 4;
        if (*(s32 *)*(s32 *)(*temp_v1 + temp_t6) != 0) {
            if ((D_800CA2AC++ == 0) && (D_80122CE4 == 0)) {
                temp_v0_7 = *(s32 *)(*temp_v1 + temp_t6);
                func_80046290((*(s32 *)temp_v0_7), *(s32 *)((unsigned char *)temp_v0_7 + (4)), (s32) D_800E6280.unk_F5F);
                func_80044750(0x300);
            }
            if (!(func_800460EC() & 4) && (D_80122CE4 == 0)) {
                D_800CA2A8 = 1;
                return 0;
            }
            D_80122CF0 = 1;
            D_800CA2AC = 0;
            goto block_17;
        }
block_17:
        set_kanji_string(-0x90, 0x48, (s32) arg6, (s32) "「 ", 0);
        temp_v0_3 = dialogue_render(-0x82, 0x48, (s32) arg6, (s32) &sp50, 0, &D_8013BF1C, &D_8013BF20, 0);
        D_80122CE4 = 1;
        return temp_v0_3;
    case 0xC:
        D_8013C360 = 0;
        k_speed_set(8);
        temp_v1_2 = arg3 + (arg4 * 4);
        temp_t6_2 = arg5 * 4;
        if (*(s32 *)*(s32 *)(*temp_v1_2 + temp_t6_2) != 0) {
            if ((D_800CA2AC++ == 0) && (D_80122CE4 == 0)) {
                temp_v0_8 = *(s32 *)(*temp_v1_2 + temp_t6_2);
                func_80046290((*(s32 *)temp_v0_8), *(s32 *)((unsigned char *)temp_v0_8 + (4)), (s32) D_800E6280.unk_F5F);
                func_80044750(0x300);
            }
            if (!(func_800460EC() & 4) && (D_80122CE4 == 0)) {
                D_800CA2A8 = 1;
                return 0;
            }
            D_80122CF0 = 1;
            D_800CA2AC = 0;
            goto block_26;
        }
block_26:
        set_kanji_string(-0x90, 0x48, (s32) arg6, (s32) "「 ", 0);
        temp_v0_4 = dialogue_render(-0x82, 0x48, (s32) arg6, (s32) &sp50, 0, &D_8013BF28, &D_8013BF2C, 0);
        D_80122D18 = 1;
        return temp_v0_4;
    case 0xD:
        D_8013C360 = 0;
        if ((u8) D_8013C364 < 0x3CU) {
            D_8013C364 = ((u8) D_8013C364 + 1) & 0xFF;
            return 0;
        }
        D_8013C364 = 0;
        k_speed_set(8);
        temp_v1_3 = arg3 + (arg4 * 4);
        temp_t7 = arg5 * 4;
        if (*(s32 *)*(s32 *)(*temp_v1_3 + temp_t7) != 0) {
            if ((D_800CA2AC++ == 0) && (D_80122CE4 == 0)) {
                temp_v0_9 = *(s32 *)(*temp_v1_3 + temp_t7);
                func_80046290((*(s32 *)temp_v0_9), *(s32 *)((unsigned char *)temp_v0_9 + (4)), (s32) D_800E6280.unk_F5F);
                func_80044750(0x300);
            }
            if (!(func_800460EC() & 4) && (D_80122CE4 == 0)) {
                D_800CA2A8 = 1;
                return 0;
            }
            D_80122CF0 = 1;
            D_800CA2AC = 0;
            goto block_37;
        }
block_37:
        temp_v0_5 = dialogue_render(-0x82, (s32) D_800CA2C0, (s32) arg6, (s32) &sp50, 0, &D_8013BF30, &D_8013BF34, 0);
        D_80122D18 = 1;
        return temp_v0_5;
    case 0xF:
        D_8013C360 = 0;
        if ((u8) D_8013C364 < 0x3CU) {
            D_8013C364 = ((u8) D_8013C364 + 1) & 0xFF;
            return 0;
        }
        D_8013C364 = 0;
        k_speed_set(8);
        temp_v1_4 = arg3 + (arg4 * 4);
        temp_t8 = arg5 * 4;
        if (*(s32 *)*(s32 *)(*temp_v1_4 + temp_t8) != 0) {
            if ((D_800CA2AC++ == 0) && (D_80122CE4 == 0)) {
                temp_v0_10 = *(s32 *)(*temp_v1_4 + temp_t8);
                func_80046290((*(s32 *)temp_v0_10), *(s32 *)((unsigned char *)temp_v0_10 + (4)), (s32) D_800E6280.unk_F5F);
                func_80044750(0x300);
            }
            if (!(func_800460EC() & 4) && (D_80122CE4 == 0)) {
                D_800CA2A8 = 1;
                return 0;
            }
            D_80122CF0 = 1;
            D_800CA2AC = 0;
            goto block_48;
        }
block_48:
        temp_v0_6 = dialogue_render(-0x82, (s32) D_800CA2C0, (s32) arg6, (s32) &sp50, 0, &D_8013BF38, &D_8013BF3C, 0);
        D_80122CE4 = 1;
        return temp_v0_6;
    case 0xFF:
        return func_8004284C();
    default:
        return func_80046500();
    }
}
