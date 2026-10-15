extern u8 D_800E62B5;
extern u8 D_800E71F4;
extern s8 D_800E7312;
extern u8 D_800E8BF1;

s32 func_80042AC8(void) {
    u8 temp_v0;
    u8 var_v1;

    var_v1 = D_800E71F4;
    temp_v0 = var_v1;
    if (*D_800E8BF0 == 0) {
        if (((u32) ((D_800E8BF1 & 0xF) * 2) < 0x41U) && (var_v1 = 1, ((D_800E8BF1 & 0xF0) == 0x10))) {
            var_v1 = 0;
            D_800E7312 = 0;
        }
    } else if ((*D_800E8BF0 != 0xFF) || (D_800E62B5 == 0)) {
        var_v1 = 0xFF & 0xFF;
    }
    D_800E71F4 = var_v1;
    if ((temp_v0 != var_v1) && ((var_v1 != 0xFF) || (D_800E71F4 = var_v1, (temp_v0 != 0xFF)))) {
        if (var_v1 == 0) {
            D_800E71F4 = var_v1;
            func_80042C30();
            func_80042C74();
            func_80042BD4();
            return 1;
        }
        D_800E71F4 = var_v1;
        if (var_v1 == 1) {
            func_80042C74();
            func_80042C30();
        }
        return 1;
    }
    return 0;
}
