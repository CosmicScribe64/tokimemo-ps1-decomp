/* T-24301 draft, 86 rows; needs `s32 func_8006A044();` in main_api.h (implicit int: the promoted F64[1]
 * stays in $v0 at the exit). F64[1] |= on the plain field; the original also has seven `move` copies before
 * the ORs (161 vs 150 instructions) and loads F64[1] inside both arms of the first test. */
s32 func_8006A044(arg0, arg1)
s32 arg0;
s32 arg1;
{
    if (D_800E6280.unk_F64[1]) {} /* FAKE: T-24301, numbers F64[1] before the month */
    if (D_800E6280.unk_03F != 2 && D_800E6280.unk_03F != 5 && D_800E6280.unk_03F != 8 && D_800E6280.unk_03F != 11) {
        D_800E6280.unk_F64[1] |= 0x40;
    } else {
        D_800E6280.unk_F64[1] |= 0x20;
    }
    if (D_800E6280.unk_03E == 0x5F && D_800E6280.unk_03F == 4) {
        D_800E6280.unk_F64[1] |= 0x60;
    }
    switch (D_800E6280.unk_03F) {
    case 2:
    case 3:
    case 4:
        if (D_800E6280.unk_03E == 0x5F) {
            D_800E6280.unk_F64[0] |= 1;
        }
        if (D_800E6280.unk_03E == 0x60) {
            D_800E6280.unk_F64[0] |= 0x10;
        }
        if (D_800E6280.unk_03E == 0x61) {
            D_800E6280.unk_F64[1] |= 1;
        }
        break;
    case 5:
    case 6:
    case 7:
        if (D_800E6280.unk_03E == 0x5F) {
            D_800E6280.unk_F64[0] |= 2;
        }
        if (D_800E6280.unk_03E == 0x60) {
            D_800E6280.unk_F64[0] |= 0x20;
        }
        if (D_800E6280.unk_03E == 0x61) {
            D_800E6280.unk_F64[1] |= 2;
        }
        D_800E6280.unk_F64[1] |= 0x80;
        break;
    case 8:
    case 9:
    case 10:
        if (D_800E6280.unk_03E == 0x5F) {
            D_800E6280.unk_F64[0] |= 4;
        }
        if (D_800E6280.unk_03E == 0x60) {
            D_800E6280.unk_F64[0] |= 0x40;
        }
        if (D_800E6280.unk_03E == 0x61) {
            D_800E6280.unk_F64[1] |= 4;
        }
        break;
    case 11:
    case 12:
    case 13:
        if (D_800E6280.unk_03E == 0x5F) {
            D_800E6280.unk_F64[0] |= 8;
        }
        if (D_800E6280.unk_03E == 0x60) {
            D_800E6280.unk_F64[0] |= 0x80;
        }
        if (D_800E6280.unk_03E == 0x61) {
            D_800E6280.unk_F64[1] |= 8;
        }
        break;
    case 1:
        if (D_800E6280.unk_03E == 0x60) {
            D_800E6280.unk_F64[0] |= 8;
        }
        if (D_800E6280.unk_03E == 0x61) {
            D_800E6280.unk_F64[0] |= 0x80;
        }
        if (D_800E6280.unk_03E == 0x62) {
            D_800E6280.unk_F64[1] |= 8;
        }
        break;
    }
}
