extern s32 D_800E7200;
extern s32 D_800E7208;
extern s8 D_80120CF7;
extern s16 D_80120D0C;
extern s8 D_80120D3B;
extern s16 D_80120D50;
extern s8 D_80120D7F;
extern s8 D_80120DC3;

void func_80135650(void) {
    if ((D_8014CCC8 == 2) || (D_8014CCC8 == 3)) {
        D_80120D0C = 0x19;
        D_80120D50 = 0x1B;
        D_80120CF7 = 0x80;
        D_80120D3B = 0x80;
        D_80120D7F = 0;
        D_80120DC3 = 0;
        if (D_8011ECF6 >= 0x89) {
            if (D_8011ECF6 < 0x98) {
                if ((D_8011ECFA >= -0x2F) && (D_8011ECFA < -0x20)) {
                    D_80120D0C = 0x1A;
                    if (D_800E7200 & 0x20) {
                        D_80120D7F = 0x80;
                    }
                    if (D_800E7208 & 0x20) {
                        if (D_8014CCAC > 0) {
                            D_8014CCAC -= 1;
                        }
                        func_801338DC(0);
                    }
                } else if ((D_8011ECFA >= 0x11) && (D_8011ECFA < 0x20)) {
                    D_80120D50 = 0x1C;
                    if (D_800E7200 & 0x20) {
                        D_80120DC3 = 0x80;
                    }
                    if (D_800E7208 & 0x20) {
                        D_8014CCAC += 1;
                        func_801338DC(0);
                    }
                }
            }
        }
    } else {
        D_80120CF7 = 0;
        D_80120D3B = 0;
        D_80120D7F = 0;
        D_80120DC3 = 0;
    }
}
