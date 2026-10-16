void func_801546FC(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 *var_s0;
    s32 temp_s1;
    s32 temp_t9;
    s32 temp_v0;
    s32 temp_v0_2;
    u32 temp_s2;

    temp_s1 = *(s32 *)((unsigned char *)arg0 + (4));
    temp_s2 = *(u32 *)((unsigned char *)arg0 + (12));
    var_s0 = ((u8 *)arg0) + 0x24;
loop_1:
    if (func_8015431C((s32) var_s0) != 0) {
        temp_v0 = (*(s32 *)var_s0);
        if ((temp_v0 == 0x24000507) || (temp_v0 == 0x34000609)) {
            temp_v0_2 = *(s32 *)((unsigned char *)var_s0 + (8));
            temp_t9 = (temp_v0_2 & 0xFFFF) + (temp_v0_2 & 0x01E00000) + ((arg1 & 0xFFFF & 0x1F) << 0x10);
            *(s32 *)((unsigned char *)var_s0 + (4)) = (s32) ((*(s32 *)((unsigned char *)var_s0 + (4)) & 0xFFFF) + ((arg3 << 0x16) + (((arg2 >> 4) & 0x3F) << 0x10)));
            var_s0 += 8;
            *var_s0 = temp_t9;
        }
        var_s0 += 4;
        if ((temp_s1 == 0) || (temp_s2 >= (u32) var_s0)) {
            if ((temp_s1 == 0) && ((u32) ((temp_s2 * 4) + var_s0 + 0xC) < (u32) var_s0)) {

            } else {
                goto loop_1;
            }
        }
    }
}
