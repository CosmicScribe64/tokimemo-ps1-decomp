void func_80134800(s32 arg0) {
    s32 r;

    if (D_8013984C[arg0].unk_24 <= 0) {
        r = func_800AE0D0();
        switch (D_8013984C[arg0].unk_08) {
        case 0:
            if (r % 100 < 80) {
                D_8013984C[arg0].unk_28 = 0;
            } else {
                D_8013984C[arg0].unk_28 = 1;
            }
            break;
        case 1:
            if (r % 100 < 10) {
                D_8013984C[arg0].unk_28 = 0;
            } else if (r % 100 < 90) {
                D_8013984C[arg0].unk_28 = 1;
            } else {
                D_8013984C[arg0].unk_28 = 2;
            }
            break;
        case 2:
            if (r % 100 < 20) {
                D_8013984C[arg0].unk_28 = 1;
            } else {
                D_8013984C[arg0].unk_28 = 2;
            }
            break;
        }
        r = func_800AE0D0();
        if (D_8013984C[arg0].unk_2C == 0 || D_8013984C[arg0].unk_2C == 1) {
            if (r % 100 < 25) {
                D_8013984C[arg0].unk_2C = 0;
            } else if (r % 100 < 40) {
                D_8013984C[arg0].unk_2C = 1;
            } else if (r % 100 < 70) {
                D_8013984C[arg0].unk_2C = 0x10;
            } else {
                D_8013984C[arg0].unk_2C = 0x20;
            }
        } else if (D_8013984C[arg0].unk_2C & 0x50) {
            if (r % 100 < 40) {
                D_8013984C[arg0].unk_2C = 0;
            } else if (r % 100 < 80) {
                D_8013984C[arg0].unk_2C = 1;
            } else {
                D_8013984C[arg0].unk_2C = 0x20;
            }
        } else if (D_8013984C[arg0].unk_2C & 0xA0) {
            if (r % 100 < 40) {
                D_8013984C[arg0].unk_2C = 0;
            } else if (r % 100 < 80) {
                D_8013984C[arg0].unk_2C = 1;
            } else {
                D_8013984C[arg0].unk_2C = 0x10;
            }
        }
        if ((D_8013984C[arg0].unk_2C & 0x10) || (D_8013984C[arg0].unk_2C & 0x20)) {
            r = func_800AE0D0() % 100;
            if (r < 33) {
                D_8013984C[arg0].unk_2C |= 1;
            } else if (r < 66) {
                D_8013984C[arg0].unk_2C |= 2;
            } else {
                D_8013984C[arg0].unk_2C |= 4;
            }
        }
        if (D_8013984C[arg0].unk_2C == 0 || D_8013984C[arg0].unk_2C == 1) {
            D_8013984C[arg0].unk_24 = func_800AE0D0() % 25 + 40;
            if (D_8013984C[arg0].unk_28 >= 4) {
                D_8013984C[arg0].unk_24 /= 4;
            }
        } else if (D_8013984C[arg0].unk_2C & 1) {
            D_8013984C[arg0].unk_24 = 25;
        } else if (D_8013984C[arg0].unk_2C & 2) {
            D_8013984C[arg0].unk_24 = 50;
        } else if (D_8013984C[arg0].unk_2C & 4) {
            D_8013984C[arg0].unk_24 = 100;
        }
    }
}
