void func_801377E8(arg0)
s32 arg0;
{
    s32 a;
    u8 b;

    if (arg0 != 0) {
        if (D_8014CC9C == 0xFF) {
            D_8014CC9C = func_800AE0D0() % 12;
        }
        b = D_8014CCA0;
        if (b == 0xFF) {
            b = func_800AE0D0() % 31;
        }
        if (D_8014CCA4 == 0xFF) {
            D_8014CCA0 = b;
            D_8014CCA4 = func_800AE0D0() % 3;
            b = D_8014CCA0;
        }
        a = D_8014CC9C + 1;
        if (a == 2) {
            if (b + 1 >= 0x1D) {
                b = 0x1B;
            }
        }
        if (a == 4) {
            if (b + 1 >= 0x1F) {
                b = 0x1D;
            }
        }
        if (a == 6) {
            if (b + 1 >= 0x1F) {
                b = 0x1D;
            }
        }
        if (a == 9 && b + 1 >= 0x1F) {
            b = 0x1D;
        }
        D_8014CCA0 = b;
        if (a == 0xB) {
            D_8014CCA0 = b;
            if (b + 1 >= 0x1F) {
                D_8014CCA0 = 0x1D;
            }
        }
    } else {
        if (D_8014CC90 == 0xFF) {
            D_8014CC90 = func_800AE0D0() % 12;
        }
        b = D_8014CC94;
        if (b == 0xFF) {
            b = func_800AE0D0() % 31;
        }
        if (D_8014CC98 == 0xFF) {
            D_8014CC94 = b;
            D_8014CC98 = func_800AE0D0() % 3;
            b = D_8014CC94;
        }
        a = D_8014CC90 + 1;
        if (a == 2) {
            if (b + 1 >= 0x1D) {
                b = 0x1B;
            }
        }
        if (a == 4) {
            if (b + 1 >= 0x1F) {
                b = 0x1D;
            }
        }
        if (a == 6) {
            if (b + 1 >= 0x1F) {
                b = 0x1D;
            }
        }
        if (a == 9 && b + 1 >= 0x1F) {
            b = 0x1D;
        }
        D_8014CC94 = b;
        if (a == 0xB) {
            D_8014CC94 = b;
            if (b + 1 >= 0x1F) {
                D_8014CC94 = 0x1D;
            }
        }
    }
}
