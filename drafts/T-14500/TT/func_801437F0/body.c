void func_801437F0(void) {
    s32 var_s0;
    s8 var_a1;
    u16 temp_v0;
    u8 *temp_s1;
    u8 *var_s1;
    u8 *var_v0;

    temp_s1 = D_80158A74;
    temp_v0 = *(u16 *)((unsigned char *)temp_s1 + (6));
    switch (temp_v0) {
    case 0x0:
        func_80133AD0(0x60E);
        (*(s32 *)temp_s1) = 1;
        *(s32 *)((unsigned char *)temp_s1 + (32)) = 0;
        *(s32 *)((unsigned char *)temp_s1 + (36)) = 0xF80000;
        *(s32 *)((unsigned char *)temp_s1 + (40)) = 0;
        *(s32 *)((unsigned char *)temp_s1 + (44)) = 0xFFFE0000;
        *(s16 *)((unsigned char *)temp_s1 + (20)) = 0;
        *(s16 *)((unsigned char *)temp_s1 + (22)) = 0xF8;
        *(u8 **)((unsigned char *)temp_s1 + (56)) = D_80151A50;
        *(s8 *)((unsigned char *)temp_s1 + (60)) = 0;
        *(s8 *)((unsigned char *)temp_s1 + (61)) = 0;
        *(s8 *)((unsigned char *)temp_s1 + (62)) = 0;
        *(s8 *)((unsigned char *)temp_s1 + (64)) = 0x60;
        *(s8 *)((unsigned char *)temp_s1 + (72)) = 0;
        *(u16 *)((unsigned char *)temp_s1 + (80)) = (u16) (*(u16 *)((unsigned char *)temp_s1 + (80)) | 2);
        *(s8 *)((unsigned char *)temp_s1 + (82)) = 1;
        *(s8 *)((unsigned char *)temp_s1 + (86)) = 1;
        *(s16 *)((unsigned char *)temp_s1 + (6)) = 0x40;
        var_v0 = D_80158A7C;
        var_a1 = 5;
        do {
            *(s8 *)((unsigned char *)var_v0 + (62)) = var_a1;
            var_a1 += 5;
            var_v0 += 0x60;
            *(s8 *)((unsigned char *)var_v0 + (-96)) = 1;
            *(s16 *)((unsigned char *)var_v0 + (-94)) = 0;
            *(s16 *)((unsigned char *)var_v0 + (-92)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-84)) = 0;
            *(s16 *)((unsigned char *)var_v0 + (-86)) = 0;
            *(s16 *)((unsigned char *)var_v0 + (-88)) = 0;
            *(s16 *)((unsigned char *)var_v0 + (-90)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-81)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-82)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-83)) = 0;
            *(s16 *)((unsigned char *)var_v0 + (-80)) = 2;
            *(u8 **)((unsigned char *)var_v0 + (-40)) = D_80151A50;
            *(s8 *)((unsigned char *)var_v0 + (-36)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-35)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-32)) = 0x60;
        } while (var_a1 < 0xF);
        var_s1 = D_80158A78;
        var_s0 = 0;
        do {
            func_80147FD0((s32) var_s1);
            var_s0 += 1;
            var_s1 += 0xA4;
        } while (var_s0 != 3);
        break;
    case 0x40:
        if (*(s32 *)((unsigned char *)D_80158A74 + (36)) < 0xC00000) {
            *(s32 *)((unsigned char *)temp_s1 + (36)) = 0xC00000;
            *(s8 *)((unsigned char *)temp_s1 + (86)) = 0;
            *(u16 *)((unsigned char *)temp_s1 + (6)) = 0U;
            *(s8 *)((unsigned char *)temp_s1 + (13)) = 1;
        }
        break;
    }
}
