extern ? D_8015EE05;
extern ? D_8015EE06;

void func_8013A580(s32 arg0) {
    s32 sp64;
    s32 sp50;
    s32 sp4C;
    s32 sp48;
    s32 sp44;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_t0;
    s32 temp_t6;

    switch (D_8015F4D8) {                           /* irregular */
    case 0:
        sp64 = 1;
        break;
    case 1:
        sp64 = 4;
        break;
    case 2:
        sp64 = 3;
        break;
    }
    temp_lo = (s32) (arg0 << 7) / 480;
    sp50 = temp_lo;
    temp_lo_2 = (s32) (arg0 * 0x7000) / 480;
    temp_t6 = (s32) (temp_lo_2 * 0xC) / 4096;
    temp_t0 = -0x28 - (arg0 / 16);
    sp44 = temp_t0;
    sp48 = temp_t6;
    sp4C = temp_lo_2;
    func_80139424((s32) *(D_8015EE04 + (D_8015F4D8 * 4)), sp64, 3, temp_lo, -temp_t6, temp_t0, temp_lo_2, temp_lo_2, 0);
    func_80139424((s32) *(&D_8015EE05 + (D_8015F4D8 * 4)), sp64, 3, temp_lo, 0, sp44, temp_lo_2, temp_lo_2, 0);
    func_80139424((s32) *(&D_8015EE06 + (D_8015F4D8 * 4)), sp64, 3, temp_lo, sp48, sp44, temp_lo_2, temp_lo_2, 0);
}
