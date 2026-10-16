extern u32 D_800E7384;
extern u8 D_800E738C;
extern u8 D_800E738D;
void func_80134D94(void) {
    s32 sp3C;
    s16 var_t7;
    s32 temp_v0;
    u32 temp_lo;
    u32 temp_t0;
    u32 var_a0;
    u8 var_a0_2;
    u8 var_a1;

    temp_t0 = D_800E7384;
    if (temp_t0 == 0) {
        D_8012071E = 5;
    }
    if (temp_t0 == 0xFE) {
        D_80120697 = 0x80;
        D_80120696 = 5;
        D_8012069B = 0x80;
    }
    if (!(D_80120696 & 1)) {
        D_80120697 = 0;
    }
    if (temp_t0 == 0x29C) {
        D_801206DB = 0x80;
        D_801206DA = 5;
        D_801206DF = 0x80;
    }
    if ((temp_t0 >= 0xFFU) && (temp_t0 < 0x5A3U)) {
        var_a1 = D_800E738D;
        D_8012071E = 0x40;
        var_a0 = var_a1 / 14U;
        if (var_a0 >= 2U) {
            var_a1 = 0;
            var_a0 = 0;
        }
        switch (D_80122EAC) {
        case 0:
            D_80120734 = var_a0 + 0x5D;
            break;
        case 1:
            D_80120734 = var_a0 + 0x72;
            break;
        case 2:
            D_80120734 = var_a0 + 0x87;
            break;
        default:
        case 3:
            if (temp_t0 < 0x4C2U) {
                D_80120734 = var_a0 + 0x9D;
            } else {
                D_80120734 = var_a0 + 0x9F;
            }
            break;
        }
        D_800E738D = var_a1 + 1;
    }
    if ((temp_t0 == 0x4C2) && (D_80122EAC != 3)) {
        D_8012071F = 0;
        D_80120763 = 0x80;
        D_80120762 = 5;
        D_80120767 = 0x80;
    }
    if ((temp_t0 == 0x58F) && (D_80122EAC == 3)) {
        D_8012071F = 0;
        D_80120763 = 0x80;
        D_80120762 = 5;
        D_80120767 = 0x80;
    }
    if ((temp_t0 == 0x606) || (temp_t0 == 0x609) || (temp_t0 == 0x60C)) {
        func_80049A40(-0xA0, -0x78, 0x140, 0xA0, 2, 0xFFFFFF, 0x81);
        dtd_on_tpage(0, 0, 2, 0, 0);
    }
    if (temp_t0 == 0x606) {
        D_801206F0 = 0x48;
        D_801206E0 = 0;
    }
    if ((temp_t0 >= 0x60FU) && (temp_t0 < 0x68EU)) {
        temp_v0 = (0x68E - temp_t0) * 0x10201;
        sp3C = temp_v0;
        func_80049A40(-0x130, -0x78, 0x140, 0xA0, 2, temp_v0, 0x86);
        func_80049A40(0x10, -0x78, 0x140, 0xA0, 2, temp_v0, 0x85);
        dtd_on_tpage(0, 0, 2, 1, 0);
    }
    if (temp_t0 == 0x67E) {
        D_80120657 = 0x80;
        D_801206DF = 0x80;
        D_80120767 = 0x80;
    }
    if ((temp_t0 >= 0x723U) && (D_80122EAC == 3)) {
        var_a0_2 = D_800E738C;
        D_80120762 = 0x40;
        temp_lo = var_a0_2 / 14U;
        var_t7 = temp_lo + 0xA2;
        if (temp_lo >= 2U) {
            var_a0_2 = 0;
            var_t7 = 0xA2;
        }
        D_80120778 = var_t7;
        D_800E738C = var_a0_2 + 1;
    }
}
