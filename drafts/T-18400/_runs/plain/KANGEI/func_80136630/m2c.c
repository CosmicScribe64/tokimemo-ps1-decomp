extern s32 D_800E7384;

void func_80136630(void) {
    s16 var_a0;
    s16 var_s0;
    u8 temp_t5;
    u8 temp_t6;

    if (D_800E7384 == 0) {
        D_80139E04 = 0;
        D_80139E08 = 0;
        D_800E7384 = 1;
        D_80139E00 = 0;
    } else if (D_800E7384 == 1) {
        D_80139E04 += 0x10;
        D_80139E08 += 0x10;
        temp_t6 = (D_80139E00 + 0x10) & 0xFF;
        D_80139E00 = temp_t6;
        if (temp_t6 >= 0xF0U) {
            D_80139E04 = 0xFF;
            D_80139E08 = 0xFF;
            D_800E7384 = 2;
            D_80139E00 = 0xFF;
            var_s0 = 0x3A;
            var_a0 = 0x3A;
            do {
                func_80084C98(var_a0, 0);
                var_s0 += 1;
                var_a0 = var_s0;
            } while (var_s0 < 0x40);
            D_800B593C = 0;
        }
    } else {
        D_80139E04 -= 8;
        D_80139E08 -= 8;
        temp_t5 = (D_80139E00 - 8) & 0xFF;
        D_80139E00 = temp_t5;
        if (temp_t5 < 9U) {
            D_80139E04 = 0;
            D_80139E08 = 0;
            D_80139E00 = 0;
            func_8004E9F4(1);
            func_80041584();
            func_8004284C();
        }
    }
    func_80136810(-0xA0, -0x78, 0x140, 0xA0, (u8) (s32) D_80139E00, (u8) (s32) D_80139E04, (u8) (s32) D_80139E08, 9, 1);
    func_800494BC(0, 0, 9, 1, 0);
}
