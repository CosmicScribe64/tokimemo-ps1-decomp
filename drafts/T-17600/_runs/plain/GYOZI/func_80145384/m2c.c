extern s16 D_8012B8C8;
extern u8 D_8012B94F;
extern s32 D_8012E694;

s32 func_80145384(void) {
    s32 var_v0;

    M2C_MEMCPY_ALIGNED(&sp28[0], &D_8014901C, 0xBC);
    var_v0 = (&sp28[0])[D_800F647A](0x3C);
    if ((u8) D_800F647A < 0x10U) {
        D_8012B8C7 = (s8) (u8) D_801317EB;
        if ((D_8012B8D8 == 0) && (D_8012B8C8 == 0)) {
            var_v0 = (s32) (s16) (D_8012B8D6 + 1);
            D_8012B8D6 = (s16) var_v0;
            if (var_v0 >= 3) {
                D_8012B8D6 = 0;
                var_v0 = 0;
            }
        }
    }
    if (D_8012E694 != 0) {
        D_8012B94B |= 0x80;
        D_8012B94F = (u8) D_8012B8C7;
        if (((u32) D_800F6468 % 300U) == 0) {
            var_v0 = func_80145520();
            D_8012B94A = 3;
        }
    } else {
        D_8012B94B &= 0xFF7F;
    }
    return var_v0;
}
