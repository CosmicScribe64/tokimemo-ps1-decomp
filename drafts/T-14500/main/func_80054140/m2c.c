extern u8 D_800E62B5;
extern u8 D_800E7395;
extern u8 D_800E7398;
extern u8 D_800E739C;
extern u8 D_800E739D;
extern s32 D_800E73A0;

s32 func_80054140(void) {
    u8 temp_a0;
    u8 temp_t0;
    u8 temp_v0;

    D_800E73A0 += 1;
    if (D_800E62B5 != 0) {
        temp_v0 = D_800E739C;
        if (temp_v0 == 0) {
            D_800E7395 = 0;
            D_800E739C = temp_v0;
            D_800E7398 = temp_v0;
            if (func_80054AF4((s32) temp_v0) == 1) {
                D_800E8BEE = 1;
                func_80053CC0();
            } else {
                temp_t0 = (D_800E739D + 1) & 0xFF;
                D_800E739D = temp_t0;
                if (temp_t0 >= 3U) {
                    D_800E8BEE = 0;
                    func_80053CC0();
                }
            }
        } else if (temp_v0 < 0xFU) {
            D_800E739C = temp_v0;
            temp_a0 = temp_v0 & 0xFF;
            D_800E7398 = temp_a0;
            if (func_80054AF4((s32) temp_a0) == 1) {
                D_800E7D14[D_800E7398] = 1;
            } else {
                D_800E7D14[D_800E7398] = 0;
            }
            D_800E739C += 1;
        } else {
            D_800E62B5 = 0;
            D_800E7395 = 2;
        }
    }
    return (s32) D_800E7395;
}
