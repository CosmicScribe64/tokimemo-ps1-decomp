? func_80044750(?);                                 /* extern */
extern s16 D_80120668;
extern s16 D_80120676;
extern s16 D_8012067A;
extern s16 D_80120778;
extern s16 D_80120786;
extern s16 D_8012078A;
extern u8 D_8014A110;
extern u32 D_8014A124;
extern u8 D_8014A13C;
extern s8 D_8014A13E;
extern u32 D_8014A140;
extern s32 D_8014A158;
extern ? D_8014A1D4;
extern ? D_8014A1E8;

void func_80143DD0(void) {
    u8 var_v0;

    if (((u32) D_8014A124 < (u32) D_8014A140) && (D_8014A158 != 0)) {
        if ((s32) D_8014A13C >= 9) {
            var_v0 = 2;
        } else {
            var_v0 = (D_8014A13C + 1) & 0xFF;
        }
        D_8014A140 = 0;
        D_8014A13C = var_v0;
        if (var_v0 == 6) {
            D_80120778 = 0x2E;
            if (D_8014A110 != 0) {
                func_80044750(0x804);
            } else {
                func_80044750(0x801);
            }
            D_8014A13E = 1;
        }
        if (D_8014A13C == 2) {
            D_80120778 = 0x2F;
            if (D_8014A110 != 0) {
                func_80044750(0x805);
            } else {
                func_80044750(0x802);
            }
        }
    }
    if (D_8014A158 != 0) {
        if (D_8014A110 != 0) {
            D_80120668 = *(&D_8014A1E8 + (D_8014A13C * 2));
        } else {
            D_80120668 = *(&D_8014A1D4 + (D_8014A13C * 2));
        }
        D_8014A13C = D_8014A13C;
    }
    D_80120786 = D_80120676;
    D_8012078A = D_8012067A;
}
