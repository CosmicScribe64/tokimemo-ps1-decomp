void func_801339D4(s32 arg0) {

    D_8013984C[arg0].unk_24--;
    if (D_8013984C[arg0].unk_00 == 4) {
        D_8013984C[arg0].unk_14.w += (func_800A0140(D_8013984C[arg0].unk_1C) * 0x8000) >> 12;
        D_8013984C[arg0].unk_18.w += (func_800A0070(D_8013984C[arg0].unk_1C) * 0x8000) >> 12;
    } else {
        switch (D_8013984C[arg0].unk_28) {
        case 0:
            D_8013984C[arg0].unk_20 = 0xC000;
            break;
        case 1:
            D_8013984C[arg0].unk_20 = 0xF000;
            break;
        case 2:
            D_8013984C[arg0].unk_20 = 0x12000;
            break;
        case 3:
            D_8013984C[arg0].unk_20 = 0x15000;
            break;
        case 4:
            D_8013984C[arg0].unk_20 = 0x18000;
            break;
        case 5:
            D_8013984C[arg0].unk_20 = 0x1B000;
            break;
        case 6:
            D_8013984C[arg0].unk_20 = 0x1E000;
            break;
        case 7:
            D_8013984C[arg0].unk_20 = 0x21000;
            break;
        case 8:
            D_8013984C[arg0].unk_20 = 0x24000;
            break;
        }
        if (D_8013984C[arg0].unk_2C == 1) {
            if (D_8013984C[arg0].unk_24 % 40 < 10) {
                D_8013984C[arg0].unk_1C += 30;
            } else if (D_8013984C[arg0].unk_24 % 40 >= 20 && D_8013984C[arg0].unk_24 % 40 < 30) {
                D_8013984C[arg0].unk_1C -= 30;
            }
        }
        if (D_8013984C[arg0].unk_2C & 0x10) {
            D_8013984C[arg0].unk_1C += 20;
        }
        if (D_8013984C[arg0].unk_2C & 0x20) {
            D_8013984C[arg0].unk_1C -= 20;
        }
        switch (D_8013984C[arg0].unk_2C) {
        case 0x40:
            D_8013984C[arg0].unk_20 = 0xC000;
            D_8013984C[arg0].unk_1C += 0x100;
            break;
        case 0x41:
            D_8013984C[arg0].unk_20 = 0xC000;
            D_8013984C[arg0].unk_1C += 0x200;
            break;
        case 0x80:
            D_8013984C[arg0].unk_20 = 0xC000;
            D_8013984C[arg0].unk_1C -= 0x100;
            break;
        case 0x81:
            D_8013984C[arg0].unk_20 = 0xC000;
            D_8013984C[arg0].unk_1C -= 0x200;
            break;
        }
        if (D_8013984C[arg0].unk_1C >= 0x1000) {
            D_8013984C[arg0].unk_1C -= 0x1000;
        }
        if (D_8013984C[arg0].unk_1C < 0) {
            D_8013984C[arg0].unk_1C += 0x1000;
        }
        D_8013984C[arg0].unk_14.w += (D_8013984C[arg0].unk_20 * func_800A0140(D_8013984C[arg0].unk_1C)) >> 12;
        D_8013984C[arg0].unk_18.w += (D_8013984C[arg0].unk_20 * func_800A0070(D_8013984C[arg0].unk_1C)) >> 12;
    }
}
