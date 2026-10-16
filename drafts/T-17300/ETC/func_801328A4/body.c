void func_801328A4(s32 arg0, s32 arg1) {
    s16 sp340E;
    s16 sp340C;
    s16 sp340A;
    s16 sp3408;
    s32 sp33F0[1];
    s32 sp22D0[1];
    s32 sp90[1];
    s32 sp6C;
    s32 *var_s0;
    s32 *var_s1;
    s16 temp_t3;
    s32 temp_t5;
    s32 temp_t5_2;
    s32 temp_t6;
    s32 var_fp;
    s32 var_s0_2;
    s32 var_s0_3;
    u16 *temp_a0;
    u16 *temp_a0_2;
    u16 *var_s2;
    u32 temp_a2;
    u32 temp_a3;
    u32 temp_lo;
    u8 *temp_a0_3;
    u8 *temp_s1;
    u8 *temp_s5;
    u8 *temp_v0;
    u8 *temp_v1;
    u8 *var_s4;
    u8 *var_s4_2;

    sp3408 = 0x157;
    sp340A = 0x113;
    sp340C = 0x112;
    sp340E = 0xA0;
    func_8009C8E0((RECT *) &sp3408, (void *)0x80198000);
    func_8009C674(0);
    var_s2 = (u16 *)0x80198000;
    var_s4 = (void *)0x80198000;
    var_fp = 0;
    do {
        var_s0 = &sp90;
        if (var_fp == 0) {
            var_s1 = &sp22D0;
            do {
                *(u16 *)((unsigned char *)var_s0 + (8768)) = (u16) *var_s2;
                func_80132880((s32) var_s1);
                (*(s32 *)var_s0) = (u16) *var_s2;
                func_80132880((s32) var_s0);
                var_s1 += 0x10;
                var_s0 += 0x10;
                var_s2 += 2;
            } while (var_s1 != &sp33F0);
        }
        temp_t5 = var_fp + 1;
        sp6C = temp_t5;
        var_s0_2 = 0;
        temp_s1 = ((temp_t5 % 3) * 0x1120) + &sp90;
        temp_s5 = ((var_fp % 3) * 0x1120) + &sp90;
loop_5:
        temp_a0 = temp_s1 + var_s0_2;
        if (var_fp == 0x9F) {
            temp_a0_2 = temp_s1 + var_s0_2;
            *temp_a0_2 = *(temp_s5 + var_s0_2);
            func_80132880((s32) temp_a0_2);
        } else {
            *temp_a0 = *var_s2;
            func_80132880((s32) temp_a0);
            var_s2 += 2;
        }
        var_s0_2 += 0x10;
        if (var_s0_2 < 0x1120) {
            goto loop_5;
        }
        var_s4_2 = var_s4 + 2;
        var_s0_3 = 0x10;
loop_10:
        temp_v0 = temp_s5 + var_s0_3;
        temp_v1 = (((s32) (var_fp + 2) % 3) * 0x1120) + &sp90 + var_s0_3;
        temp_a0_3 = temp_s1 + var_s0_3;
        temp_a3 = (u32) (*(s32 *)((unsigned char *)temp_v0 + (24)) + *(s32 *)((unsigned char *)temp_v1 + (8)) + *(s32 *)((unsigned char *)temp_a0_3 + (8)) + *(s32 *)((unsigned char *)temp_v0 + (-8))) >> 2;
        temp_lo = *(s32 *)((unsigned char *)temp_v0 + (4)) * arg0;
        temp_a2 = (u32) (*(s32 *)((unsigned char *)temp_v0 + (20)) + *(s32 *)((unsigned char *)temp_v1 + (4)) + *(s32 *)((unsigned char *)temp_a0_3 + (4)) + *(s32 *)((unsigned char *)temp_v0 + (-12))) >> 2;
        temp_t5_2 = *(s32 *)((unsigned char *)temp_v0 + (28)) + *(s32 *)((unsigned char *)temp_v1 + (12)) + *(s32 *)((unsigned char *)temp_a0_3 + (12));
        temp_t6 = *(s32 *)((unsigned char *)temp_v0 + (-4));
        var_s0_3 += 0x10;
        var_s4_2 += 2;
        temp_t3 = (((temp_lo / 100U) + ((u32) (temp_a2 * arg1) / 100U)) & 0x1F) | ((((u32) (*(s32 *)((unsigned char *)temp_v0 + (8)) * arg0) / 100U) + ((u32) (temp_a3 * arg1) / 100U)) & 0x3E0) | ((((u32) (*(s32 *)((unsigned char *)temp_v0 + (12)) * arg0) / 100U) + ((u32) (((u32) (temp_t5_2 + temp_t6) >> 2) * arg1) / 100U)) & 0x7C00);
        *(s16 *)((unsigned char *)var_s4_2 + (-2)) = temp_t3;
        *(s16 *)((unsigned char *)var_s4_2 + (-2)) = (s16) ((temp_t3 & 0xFFFF) | 0x8000);
        if (var_s0_3 != 0x1110) {
            goto loop_10;
        }
        var_fp = sp6C;
        var_s4 = var_s4_2 + 2;
    } while (var_fp != 0xA0);
    func_8009C884((RECT *) &sp3408, (void *)0x80198000);
    func_8009C674(0);
}
