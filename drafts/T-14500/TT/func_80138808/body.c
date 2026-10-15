void func_80138808(void) {
    s32 var_a2;
    s32 var_at;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    u16 temp_v0;
    u16 temp_v0_2;
    u8 *temp_s0;
    u8 *temp_s1;
    u8 *var_a0;
    u8 *var_a0_2;
    u8 *var_a1;
    u8 *var_a1_2;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 *var_v0_3;
    u8 *var_v1;
    u8 *var_v1_2;

    temp_s1 = D_80158A8C;
    temp_v0 = *(u16 *)((unsigned char *)temp_s1 + (6));
    temp_s0 = D_80158AA4;
    switch (temp_v0) {
    case 0x0:
        *(s8 *)((unsigned char *)temp_s1 + (835)) = 1;
        var_s0 = 2;
        var_v0 = temp_s1 + 0x28;
        do {
            *(s8 *)((unsigned char *)var_v0 + (64)) = 0x80;
            *(s8 *)((unsigned char *)var_v0 + (60)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (61)) = 2;
            var_s0 += 1;
            var_v0 += 0x14;
            *(s8 *)((unsigned char *)var_v0 + (42)) = (s8) D_80152C8C;
        } while (var_s0 < 4);
        var_v0_2 = temp_s1;
        var_a1 = temp_s1;
        var_v1 = temp_s1;
        var_a2 = 0;
        var_a0 = D_80158AAC + 0x18;
        do {
            *(u8 **)((unsigned char *)var_v0_2 + (216)) = D_80151A50;
            *(s8 *)((unsigned char *)var_v0_2 + (220)) = 0;
            *(s8 *)((unsigned char *)var_v0_2 + (221)) = 2;
            *(s8 *)((unsigned char *)var_v0_2 + (226)) = 0;
            *(s8 *)((unsigned char *)var_v0_2 + (232)) = 0;
            *(s8 *)((unsigned char *)var_a1 + (792)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (696)) = 8;
            var_a2 += 4;
            *(s16 *)((unsigned char *)var_v1 + (600)) = (s16) (*(s16 *)((unsigned char *)var_a0 + (30)) - *(s16 *)((unsigned char *)temp_s1 + (20)));
            *(s16 *)((unsigned char *)var_v1 + (648)) = (s16) ((*(s16 *)((unsigned char *)var_a0 + (158)) - *(s16 *)((unsigned char *)temp_s1 + (22))) - 8);
            var_v0_2 += 0x14;
            var_a1 += 1;
            var_v1 += 2;
            var_a0 -= 4;
            *(s8 *)((unsigned char *)var_v0_2 + (204)) = 0x80;
        } while (var_a2 < 0x1C);
        *(u8 **)((unsigned char *)temp_s1 + (376)) = D_80151A50;
        *(s8 *)((unsigned char *)temp_s1 + (380)) = 0;
        *(s8 *)((unsigned char *)temp_s1 + (381)) = 2;
        *(s8 *)((unsigned char *)temp_s1 + (386)) = 0;
        *(s8 *)((unsigned char *)temp_s1 + (392)) = 0;
        *(s8 *)((unsigned char *)temp_s1 + (800)) = 0;
        *(s16 *)((unsigned char *)temp_s1 + (712)) = 8;
        var_s0_2 = 1;
        *(s16 *)((unsigned char *)temp_s1 + (616)) = (s16) (*(s16 *)((unsigned char *)D_80158AAC + (3890)) - *(s16 *)((unsigned char *)temp_s1 + (20)));
        *(s8 *)((unsigned char *)temp_s1 + (384)) = 0x80;
        *(s16 *)((unsigned char *)temp_s1 + (664)) = (s16) ((*(s16 *)((unsigned char *)D_80158AAC + (4018)) - *(s16 *)((unsigned char *)temp_s1 + (22))) - 8);
        var_v0_3 = temp_s1 + 0x14;
        var_a1_2 = temp_s1 + 1;
        var_v1_2 = temp_s1 + 2;
        var_a0_2 = D_80158AAC + 0xF10;
        do {
            *(u8 **)((unsigned char *)var_v0_3 + (376)) = D_80151A50;
            *(s8 *)((unsigned char *)var_v0_3 + (380)) = 0;
            *(s8 *)((unsigned char *)var_v0_3 + (381)) = 2;
            *(s8 *)((unsigned char *)var_v0_3 + (386)) = 0;
            *(s8 *)((unsigned char *)var_v0_3 + (392)) = 0;
            *(s8 *)((unsigned char *)var_a1_2 + (800)) = 0;
            *(s16 *)((unsigned char *)var_v1_2 + (712)) = 8;
            var_s0_2 += 2;
            *(s16 *)((unsigned char *)var_v1_2 + (616)) = (s16) (*(s16 *)((unsigned char *)var_a0_2 + (30)) - *(s16 *)((unsigned char *)temp_s1 + (20)));
            var_v0_3 += 0x28;
            *(s16 *)((unsigned char *)var_v1_2 + (664)) = (s16) ((*(s16 *)((unsigned char *)var_a0_2 + (158)) - *(s16 *)((unsigned char *)temp_s1 + (22))) - 8);
            *(s8 *)((unsigned char *)var_v0_3 + (372)) = 0;
            *(s8 *)((unsigned char *)var_v0_3 + (366)) = 0;
            *(s8 *)((unsigned char *)var_v0_3 + (361)) = 2;
            *(s8 *)((unsigned char *)var_v0_3 + (360)) = 0;
            *(u8 **)((unsigned char *)var_v0_3 + (356)) = D_80151A50;
            *(s8 *)((unsigned char *)var_v0_3 + (344)) = 0x80;
            *(s8 *)((unsigned char *)var_a1_2 + (801)) = 0;
            *(s16 *)((unsigned char *)var_v1_2 + (714)) = 8;
            var_a1_2 += 2;
            *(s16 *)((unsigned char *)var_v1_2 + (618)) = (s16) (*(s16 *)((unsigned char *)var_a0_2 + (26)) - *(s16 *)((unsigned char *)temp_s1 + (20)));
            var_v1_2 += 4;
            *(s16 *)((unsigned char *)var_v1_2 + (662)) = (s16) ((*(s16 *)((unsigned char *)var_a0_2 + (154)) - *(s16 *)((unsigned char *)temp_s1 + (22))) - 8);
            *(s8 *)((unsigned char *)var_v0_3 + (364)) = 0x80;
            var_a0_2 -= 8;
        } while (var_s0_2 != 7);
        *(u16 *)((unsigned char *)temp_s1 + (876)) = 0U;
        *(u16 *)((unsigned char *)temp_s1 + (980)) = 8U;
        *(s16 *)((unsigned char *)temp_s1 + (6)) = 0x40;
        break;
    case 0x40:
        func_80142AF0(&D_80152C8C, 2);
        func_80142AF0(&D_80152C8C, 3);
        if ((*(u16 *)((unsigned char *)temp_s1 + (876)) & 7) == 7) {
            temp_v0_2 = *(u16 *)((unsigned char *)temp_s1 + (980));
            var_at = temp_v0_2 < 0x13U;
            if (temp_v0_2 < 0xFU) {
                func_80133AD0(0x50A);
                *(s8 *)((unsigned char *)(temp_s1 + (*(u16 *)((unsigned char *)temp_s1 + (980)) * 0x14)) + (64)) = 0x80;
                *(s8 *)((unsigned char *)(&temp_s1[*(u16 *)((unsigned char *)temp_s1 + (980))]) + (784)) = 1;
                *(s8 *)((unsigned char *)(temp_s1 + (*(u16 *)((unsigned char *)temp_s1 + (980)) * 0x14)) + (224)) = 0x80;
                *(s8 *)((unsigned char *)(&temp_s1[*(u16 *)((unsigned char *)temp_s1 + (980))]) + (792)) = 1;
                *(s8 *)((unsigned char *)(temp_s0 + (*(u16 *)((unsigned char *)temp_s1 + (980)) * 0x64)) + (3400)) = 0;
                *(s8 *)((unsigned char *)(temp_s0 + (*(u16 *)((unsigned char *)temp_s1 + (980)) * 0x64)) + (2600)) = 0;
                var_at = (u16) *(u16 *)((unsigned char *)temp_s1 + (980)) < 0x13U;
            }
            if (var_at == 0) {
                *(s16 *)((unsigned char *)temp_s1 + (4)) = 0x51;
                *(s16 *)((unsigned char *)temp_s1 + (6)) = 0;
            }
            *(u16 *)((unsigned char *)temp_s1 + (980)) = (u16) (*(u16 *)((unsigned char *)temp_s1 + (980)) + 1);
        }
        var_s0_3 = 8;
        do {
            func_80142AF0(&D_80152C8C, var_s0_3);
            var_s0_3 += 1;
        } while (var_s0_3 < 0xF);
        var_s0_4 = 0x10;
        do {
            func_80142AF0(&D_80152C8C, var_s0_4);
            var_s0_4 += 1;
        } while (var_s0_4 != 0x17);
        break;
    }
    *(u16 *)((unsigned char *)temp_s1 + (876)) = (u16) (*(u16 *)((unsigned char *)temp_s1 + (876)) + 1);
}
