extern s16 D_800E638E;
extern s16 D_800E6396;
extern s32 D_800E66E8;
extern s32 D_800E6758;

void func_8013D820(void) {
    s32 var_t6;
    u32 temp_v1;

    temp_v1 = (u32) (D_800E66E8 << 0x14) >> 0x1D;
    if (D_800E638E >= (s16) ((temp_v1 * 0x19) + 0x96)) {
        var_t6 = temp_v1 < 2U;
        if (((u32) (D_800E6758 << 0x14) >> 0x1D) != 0) {
            func_8004284C();
            func_8004284C();
            func_8004284C();
            func_8004284C();
            return;
        }
        goto block_4;
    }
    var_t6 = temp_v1 < 2U;
block_4:
    if (D_800E6396 >= (s16) (((temp_v1 * 0x1E) - ((var_t6 ^ 1) * 0xA)) + 0x46)) {
        func_8004284C();
        return;
    }
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
}
