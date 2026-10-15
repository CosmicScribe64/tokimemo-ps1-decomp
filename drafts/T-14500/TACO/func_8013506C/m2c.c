extern ? D_800E6480;
extern s32 D_800E7378;

void func_8013506C(void) {
    s32 sp3C;
    GameState *var_s0;
    GameState *var_s0_2;
    s32 temp_hi;
    s32 temp_v0;
    s32 var_v1;
    u32 temp_a3;
    u32 temp_v1;

    var_s0 = &D_800E6280;
    var_v1 = 0;
loop_1:
    var_v1 += 1;
    if (*(s32 *)((unsigned char *)var_s0 + (6204)) == 0) {
        temp_hi = func_800AE0D0() % 320;
        sp3C = temp_hi;
        *(s32 *)((unsigned char *)var_s0 + (6204)) = (s32) ((temp_hi << 0x10) + (func_800AE0D0() % 240));
    } else {
        var_s0 += 4;
        if (var_v1 < 0x80) {
            goto loop_1;
        }
    }
    var_s0_2 = &D_800E6280;
    do {
        temp_v1 = *(u32 *)((unsigned char *)var_s0_2 + (6204));
        temp_a3 = temp_v1 >> 0x10;
        if (temp_v1 != 0) {
            temp_v0 = temp_v1 & 0xFFFF;
            func_80143574(temp_a3 - 0xA0, temp_v0 - 0x78, ((temp_a3 * temp_v0) + D_800E7378) & 0xFFFFFF);
        }
        var_s0_2 += 4;
    } while (var_s0_2 != &D_800E6480);
}
