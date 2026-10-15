void func_801400B0(void) {
    s32 sp6C;
    s32 sp68;
    ? sp5C;
    s32 temp_t6;
    s32 temp_t7;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_a2;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_s0_5;
    s32 var_s1;
    s32 var_t7;
    s32 var_t8;
    s32 var_t9;
    u16 temp_t6_2;
    u16 temp_t8;
    u16 temp_t9;
    u16 temp_v0;
    u8 *temp_s0;
    u8 *temp_s2;
    u8 *var_a0;
    u8 *var_a0_2;
    u8 *var_a1;
    u8 *var_a1_2;
    u8 *var_v1;
    u8 *var_v1_2;
    void *temp_v0_3;
    void *temp_v1_2;
    void *temp_v1_3;

    temp_s2 = D_80158A8C;
    temp_v0 = *(u16 *)((unsigned char *)temp_s2 + (8));
    temp_s0 = D_80158A74;
    switch (temp_v0) {                              /* irregular */
    default:
        var_s0 = 0x10;
        if (temp_v0 != 0x81) {
            var_s0_2 = 0x10;
            if (temp_v0 != 0xA0) {

            } else {
                var_a1 = temp_s2 + 0x10;
                do {
                    var_s0_2 += 1;
                    var_a1 += 1;
                    *(s8 *)((unsigned char *)var_a1 + (783)) = 0;
                } while (var_s0_2 != 0x13);
                *(s32 *)((unsigned char *)temp_s2 + (1000)) = 0;
                *(s16 *)((unsigned char *)temp_s2 + (6)) = 0;
                *(u16 *)((unsigned char *)temp_s2 + (8)) = 0U;
                *(s32 *)((unsigned char *)temp_s2 + (996)) = (s32) *(s32 *)((unsigned char *)temp_s2 + (32));
            }
        } else {
            do {
                func_80142AF0(D_80155DF0, var_s0);
                var_s0 += 1;
            } while (var_s0 != 0x13);
            if ((u16) *(u16 *)((unsigned char *)temp_s2 + (870)) >= 0x40U) {
                *(s16 *)((unsigned char *)temp_s2 + (8)) = 0xA0;
            }
        }
        break;
    case 0x0:
        if (temp_v0 == 0) {
            func_80133AD0(0x512);
            *(u16 *)((unsigned char *)temp_s2 + (870)) = 0U;
            var_a2 = 0x140;
            var_v1 = temp_s2 + 0x140;
            var_a1_2 = temp_s2 + 0x10;
            var_a0 = temp_s2 + 0x20;
            do {
                *(u8 **)((unsigned char *)var_v1 + (56)) = D_80151A50;
                *(s8 *)((unsigned char *)var_v1 + (60)) = 0;
                *(s8 *)((unsigned char *)var_v1 + (61)) = 0xA;
                *(s8 *)((unsigned char *)var_v1 + (62)) = 0x10;
                *(s8 *)((unsigned char *)var_v1 + (64)) = 0x60;
                *(s8 *)((unsigned char *)var_v1 + (67)) = 0xF;
                *(s8 *)((unsigned char *)var_v1 + (72)) = 0;
                *(s8 *)((unsigned char *)var_a1_2 + (784)) = 1;
                var_a2 += 0x14;
                var_v1 += 0x14;
                var_a1_2 += 1;
                var_a0 += 2;
                *(s16 *)((unsigned char *)var_a0 + (678)) = 7;
                *(s16 *)((unsigned char *)var_a0 + (582)) = 0;
                *(s16 *)((unsigned char *)var_a0 + (630)) = 8;
            } while (var_a2 != 0x17C);
            *(s32 *)((unsigned char *)temp_s2 + (1004)) = 0x80000;
            *(s32 *)((unsigned char *)temp_s2 + (1008)) = -0x10000;
            *(s32 *)((unsigned char *)temp_s2 + (1012)) = 0x400;
            *(s32 *)((unsigned char *)temp_s2 + (1016)) = 0;
            *(s8 *)((unsigned char *)temp_s2 + (835)) = 0;
            *(s16 *)((unsigned char *)temp_s2 + (8)) = 0x40;
        }
        break;
    case 0x40:
        var_s0_3 = 0x10;
        if (*(s32 *)((unsigned char *)D_80158A8C + (1004)) >= 0x200001) {
            *(s32 *)((unsigned char *)temp_s2 + (1012)) = -0x400;
        } else {
            *(s32 *)((unsigned char *)temp_s2 + (1012)) = 0x400;
        }
        var_a0_2 = temp_s2 + 0x20;
        if ((*(s32 *)((unsigned char *)temp_s2 + (1004)) >= 0x200001) && (*(s32 *)((unsigned char *)temp_s2 + (1008)) <= 0)) {
            *(s32 *)((unsigned char *)temp_s2 + (1012)) = 0;
            *(s16 *)((unsigned char *)temp_s2 + (8)) = 0x50;
        }
        temp_v0_2 = *(s32 *)((unsigned char *)temp_s2 + (1008));
        temp_v1 = *(s32 *)((unsigned char *)temp_s2 + (1016)) + 0x4000;
        *(s32 *)((unsigned char *)temp_s2 + (1004)) = (s32) (*(s32 *)((unsigned char *)temp_s2 + (1004)) + temp_v0_2);
        temp_t7 = -temp_v1;
        *(s32 *)((unsigned char *)temp_s2 + (1016)) = temp_v1;
        *(s32 *)((unsigned char *)temp_s2 + (1008)) = (s32) (temp_v0_2 + *(s32 *)((unsigned char *)temp_s2 + (1012)));
        var_t8 = temp_t7 >> 0x10;
        if (temp_t7 < 0) {
            var_t8 = (s32) (temp_t7 + 0xFFFF) >> 0x10;
        }
        *(s16 *)((unsigned char *)temp_s2 + (618)) = (s16) var_t8;
        var_t9 = temp_v1 >> 0x10;
        if (temp_v1 < 0) {
            var_t9 = (s32) (temp_v1 + 0xFFFF) >> 0x10;
        }
        *(s16 *)((unsigned char *)temp_s2 + (620)) = (s16) var_t9;
        do {
            temp_t6 = *(s32 *)((unsigned char *)temp_s2 + (1004));
            var_s0_3 += 1;
            var_t7 = temp_t6 >> 0x10;
            if (temp_t6 < 0) {
                var_t7 = (s32) (temp_t6 + 0xFFFF) >> 0x10;
            }
            var_a0_2 += 2;
            *(s16 *)((unsigned char *)var_a0_2 + (630)) = (s16) var_t7;
        } while (var_s0_3 != 0x13);
        break;
    case 0x50:
        *(u16 *)((unsigned char *)temp_s2 + (870)) = 0U;
        *(s16 *)((unsigned char *)temp_s2 + (8)) = 0x51;
        break;
    case 0x51:
        if ((u16) *(u16 *)((unsigned char *)D_80158A8C + (870)) >= 0x20U) {
            *(u16 *)((unsigned char *)temp_s2 + (870)) = 0U;
            *(s16 *)((unsigned char *)temp_s2 + (8)) = 0x52;
        }
        break;
    case 0x52:
        *(u16 *)((unsigned char *)temp_s2 + (870)) = 0U;
        *(u16 *)((unsigned char *)temp_s2 + (980)) = 0U;
        *(u16 *)((unsigned char *)temp_s2 + (982)) = 1U;
        *(u16 *)((unsigned char *)temp_s2 + (984)) = 2U;
        *(s16 *)((unsigned char *)temp_s2 + (8)) = 0x53;
        break;
    case 0x53:
        if ((*(u8 *)((unsigned char *)D_80158A8C + (382)) == 0x12) && !(*(u16 *)((unsigned char *)D_80158A8C + (870)) & 7)) {
            func_80133AD0(0x513);
        }
        if ((*(u8 *)((unsigned char *)temp_s2 + (382)) == 0x12) && ((*(u16 *)((unsigned char *)temp_s2 + (870)) & 7) == 7)) {
            sp68 = (*(s16 *)((unsigned char *)(temp_s2 + (*(u16 *)((unsigned char *)temp_s2 + (980)) * 2)) + (616)) + *(s16 *)((unsigned char *)temp_s2 + (20))) << 0x10;
            sp6C = (*(s16 *)((unsigned char *)(temp_s2 + (*(u16 *)((unsigned char *)temp_s2 + (980)) * 2)) + (664)) + *(s16 *)((unsigned char *)temp_s2 + (22))) << 0x10;
            var_s0_4 = 0;
            var_s1 = func_801320F0((s32) &sp5C, (s32) (temp_s0 + 0x14));
            do {
                temp_v0_3 = temp_s2 + (*(u16 *)((unsigned char *)temp_s2 + (980)) * 2);
                func_80133E30(*(s16 *)((unsigned char *)temp_v0_3 + (616)) + *(s16 *)((unsigned char *)temp_s2 + (20)), *(s16 *)((unsigned char *)temp_v0_3 + (664)) + *(s16 *)((unsigned char *)temp_s2 + (22)), 0x18, var_s1, 0xA0);
                temp_v1_2 = temp_s2 + (*(u16 *)((unsigned char *)temp_s2 + (982)) * 2);
                func_80133E30(*(s16 *)((unsigned char *)temp_v1_2 + (616)) + *(s16 *)((unsigned char *)temp_s2 + (20)), *(s16 *)((unsigned char *)temp_v1_2 + (664)) + *(s16 *)((unsigned char *)temp_s2 + (22)), 0x18, var_s0_4, 0xA0);
                temp_v1_3 = temp_s2 + (*(u16 *)((unsigned char *)temp_s2 + (984)) * 2);
                func_80133E30(*(s16 *)((unsigned char *)temp_v1_3 + (616)) + *(s16 *)((unsigned char *)temp_s2 + (20)), *(s16 *)((unsigned char *)temp_v1_3 + (664)) + *(s16 *)((unsigned char *)temp_s2 + (22)), 0x18, var_s0_4 + 0x200, 0xA0);
                var_s0_4 += 0x400;
                var_s1 += 0x400;
            } while (var_s0_4 != 0x1000);
        }
        func_8013F988();
        temp_t9 = *(u16 *)((unsigned char *)temp_s2 + (980)) + 1;
        *(u16 *)((unsigned char *)temp_s2 + (980)) = temp_t9;
        if ((u32) (temp_t9 & 0xFFFF) >= 3U) {
            *(u16 *)((unsigned char *)temp_s2 + (980)) = 0U;
        }
        temp_t8 = *(u16 *)((unsigned char *)temp_s2 + (982)) + 1;
        *(u16 *)((unsigned char *)temp_s2 + (982)) = temp_t8;
        if ((u32) (temp_t8 & 0xFFFF) >= 3U) {
            *(u16 *)((unsigned char *)temp_s2 + (982)) = 0U;
        }
        temp_t6_2 = *(u16 *)((unsigned char *)temp_s2 + (984)) + 1;
        *(u16 *)((unsigned char *)temp_s2 + (984)) = temp_t6_2;
        if ((u32) (temp_t6_2 & 0xFFFF) >= 3U) {
            *(u16 *)((unsigned char *)temp_s2 + (984)) = 0U;
        }
        if ((u16) *(u16 *)((unsigned char *)temp_s2 + (870)) >= 0x180U) {
            *(s16 *)((unsigned char *)temp_s2 + (8)) = 0x80;
        }
        break;
    case 0x80:
        func_80133AD0(0x50A);
        *(s8 *)((unsigned char *)temp_s2 + (835)) = 1;
        var_s0_5 = 0x10;
        var_v1_2 = temp_s2 + 0x140;
        do {
            var_s0_5 += 1;
            var_v1_2 += 0x14;
            *(s8 *)((unsigned char *)var_v1_2 + (41)) = 4;
            *(s8 *)((unsigned char *)var_v1_2 + (42)) = 0x34;
            *(s8 *)((unsigned char *)var_v1_2 + (40)) = 0;
            *(s8 *)((unsigned char *)var_v1_2 + (44)) = 0x80;
        } while (var_s0_5 != 0x13);
        *(u16 *)((unsigned char *)temp_s2 + (870)) = 0U;
        *(s16 *)((unsigned char *)temp_s2 + (8)) = 0x81;
        break;
    }
    *(u16 *)((unsigned char *)temp_s2 + (870)) = (u16) (*(u16 *)((unsigned char *)temp_s2 + (870)) + 1);
}
