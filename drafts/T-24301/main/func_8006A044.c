/* T-24301 draft, 113-124 rows: K&R (arg0, arg1) gives the original's home stores; the F64[1] value
 * wants $v0 and the month $v1 (swapped here), which shifts every temporary. A rewrite with plain |= on
 * the promoted F64[1] is 19 instructions short (the original stores the byte back in every case). */
void func_8006A044(arg0, arg1)
s32 arg0;
s32 arg1;
{
    u8 var_v0;

    if ((D_800E6280.unk_03F != 2) && (D_800E6280.unk_03F != 5) && (D_800E6280.unk_03F != 8) && (D_800E6280.unk_03F != 0xB)) {
        var_v0 = (D_800E6280.unk_F64[1] | 0x40) & 0xFF;
    } else {
        var_v0 = (D_800E6280.unk_F64[1] | 0x20) & 0xFF;
    }
    if ((D_800E6280.unk_03E == 0x5F) && (D_800E6280.unk_03F == 4)) {
        var_v0 = (var_v0 | 0x60) & 0xFF;
    }
    D_800E6280.unk_F64[1] = var_v0;
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
        D_800E6280.unk_F64[1] = var_v0;
        if (D_800E6280.unk_03E == 0x61) {
            D_800E6280.unk_F64[1] = (var_v0 | 1) & 0xFF;
            return;
        }
    default:
        return;
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
            var_v0 = (var_v0 | 2) & 0xFF;
        }
        D_800E6280.unk_F64[1] = (var_v0 | 0x80) & 0xFF;
        return;
    case 8:
    case 9:
    case 10:
        if (D_800E6280.unk_03E == 0x5F) {
            D_800E6280.unk_F64[0] |= 4;
        }
        if (D_800E6280.unk_03E == 0x60) {
            D_800E6280.unk_F64[0] |= 0x40;
        }
        D_800E6280.unk_F64[1] = var_v0;
        if (D_800E6280.unk_03E == 0x61) {
            D_800E6280.unk_F64[1] = (var_v0 | 4) & 0xFF;
            return;
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
        D_800E6280.unk_F64[1] = var_v0;
        if (D_800E6280.unk_03E == 0x61) {
            D_800E6280.unk_F64[1] = (var_v0 | 8) & 0xFF;
            return;
        }
        break;
    case 1:
        if (D_800E6280.unk_03E == 0x60) {
            D_800E6280.unk_F64[0] |= 8;
        }
        if (D_800E6280.unk_03E == 0x61) {
            D_800E6280.unk_F64[0] |= 0x80;
        }
        D_800E6280.unk_F64[1] = var_v0;
        if (D_800E6280.unk_03E == 0x62) {
            D_800E6280.unk_F64[1] = (var_v0 | 8) & 0xFF;
        }
        break;
    }
}
