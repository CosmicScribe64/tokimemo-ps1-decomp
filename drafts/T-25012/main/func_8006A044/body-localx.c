void func_8006A044(arg0, arg1)
s32 arg0;
s32 arg1;
{
    u8 x;

    if (D_800E6280.unk_03F != 2 && D_800E6280.unk_03F != 5 && D_800E6280.unk_03F != 8 && D_800E6280.unk_03F != 11) {
        x = D_800E6280.unk_F64[1] | 0x40;
    } else {
        x = D_800E6280.unk_F64[1] | 0x20;
    }
    if (D_800E6280.unk_03E == 0x5F && D_800E6280.unk_03F == 4) {
        x |= 0x60;
    }
    D_800E6280.unk_F64[1] = x;
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
        D_800E6280.unk_F64[1] = x;
        if (D_800E6280.unk_03E == 0x61) {
            x |= 1;
            D_800E6280.unk_F64[1] = x;
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
        D_800E6280.unk_F64[1] = x;
        if (D_800E6280.unk_03E == 0x61) {
            x |= 2;
        }
        x |= 0x80;
        D_800E6280.unk_F64[1] = x;
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
        D_800E6280.unk_F64[1] = x;
        if (D_800E6280.unk_03E == 0x61) {
            x |= 4;
            D_800E6280.unk_F64[1] = x;
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
        D_800E6280.unk_F64[1] = x;
        if (D_800E6280.unk_03E == 0x61) {
            x |= 8;
            D_800E6280.unk_F64[1] = x;
        }
        break;
    case 1:
        if (D_800E6280.unk_03E == 0x60) {
            D_800E6280.unk_F64[0] |= 8;
        }
        if (D_800E6280.unk_03E == 0x61) {
            D_800E6280.unk_F64[0] |= 0x80;
        }
        D_800E6280.unk_F64[1] = x;
        if (D_800E6280.unk_03E == 0x62) {
            x |= 8;
            D_800E6280.unk_F64[1] = x;
        }
        break;
    }
}
