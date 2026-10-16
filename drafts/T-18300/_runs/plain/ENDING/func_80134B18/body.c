void func_80134B18(void) {
    s16 var_s0;
    u8 temp_t6;
    u8 var_v1;

    temp_t6 = (D_800E62B6 + 8) & 0xFF;
    var_v1 = temp_t6;
    if ((temp_t6 == 0) || (temp_t6 == 7)) {
        var_v1 = 0xFF;
    }
    D_800E62B8 = var_v1;
    D_800E62B7 = var_v1;
    D_8012069B += 4;
    D_800E62B6 = var_v1;
    D_80120657 += 4;
    var_s0 = 4;
    do {
        sprite_brightness(var_s0, D_80120657);
        var_s0 += 1;
    } while (var_s0 < 0xA);
    if ((u8) D_80120657 >= 0x80U) {
        D_80120652 = 5;
        func_8004284C();
    }
}
