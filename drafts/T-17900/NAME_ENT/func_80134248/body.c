/* NON_MATCHING: T-10890, about 455 differing lines (T-11650 probe made it worse), character-page selection */
void func_80134248(s32 arg0) {
    s32 selected;
    u32 index;
    s32 column;
    u8 *dest;

    selected = D_8014CC70;
    switch (arg0) {
    case 0:
        if (selected >= 3) {
            selected = 2;
        }
        break;
    case 1:
        selected = *(u8 *)&D_8014CC70 - 3;
        if (selected >= 3) {
            selected = 2;
        }
        break;
    case 2:
        selected = *(u8 *)&D_8014CC70 - 6;
        if (selected >= 5) {
            selected = 4;
        }
        break;
    }
    switch (arg0) {
    case 0:
        dest = D_8014CC74;
        break;
    case 1:
        dest = D_8014CC7C;
        break;
    case 2:
        dest = D_8014CC84;
        break;
    }
    switch (D_8014CCC8) {
    case 0:
        column = (s32)D_8014CCC0 % 6;
        if (column == 5) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else if (D_8014CCC4 == 2 && (D_8014CCC0 == 9 || D_8014CCC0 == 10)) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else if (D_8014CCC4 == 3 && (D_8014CCC0 == 3 || D_8014CCC0 == 4)) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else if (D_8014CCC4 == 5 && D_8014CCC0 >= 11) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else {
            dest[selected * 2] = ((NameKanaPages44 *)&D_8014CD28)->text[(s32)D_8014CCC0 / 6 + D_8014CCC4 * 3][column * 2];
            dest[selected * 2 + 1] = ((NameKanaPages44 *)&D_8014CD28)->text[(s32)D_8014CCC0 / 6 + D_8014CCC4 * 3][((s32)D_8014CCC0 % 6) * 2 + 1];
        }
        break;
    case 1:
        column = (s32)D_8014CCC0 % 6;
        if (column == 5) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else if (D_8014CCC4 == 2 && (D_8014CCC0 == 9 || D_8014CCC0 == 10)) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else if (D_8014CCC4 == 3 && (D_8014CCC0 == 3 || D_8014CCC0 == 4)) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else if (D_8014CCC4 == 5 && D_8014CCC0 >= 11) {
            dest[selected * 2] = D_800B3644[0];
            dest[selected * 2 + 1] = D_800B3644[1];
        } else {
            dest[selected * 2] = ((NameKanaPages44 *)&D_8014CD6C)->text[(s32)D_8014CCC0 / 6 + D_8014CCC4 * 3][column * 2];
            dest[selected * 2 + 1] = ((NameKanaPages44 *)&D_8014CD6C)->text[(s32)D_8014CCC0 / 6 + D_8014CCC4 * 3][((s32)D_8014CCC0 % 6) * 2 + 1];
        }
        break;
    case 2:
        if (D_8014CCC0 < 16) {
            index = D_8014CCC4 * 16 + D_8014CCAC * 16 + D_8014CCC0;
            if (index < D_8014CE64[38]) {
                dest[selected * 2] = D_8014CE68[38][index * 2];
                dest[selected * 2 + 1] = D_8014CE68[38][index * 2 + 1];
            } else {
                dest[selected * 2] = D_800B3644[0];
                dest[selected * 2 + 1] = D_800B3644[1];
            }
        }
        break;
    case 3:
        if (D_8014CCC0 < 16) {
            if (D_8014CCB4 == 38) {
                index = D_8014CCC4 * 16 + D_8014CCAC * 16 + D_8014CCC0;
                if (index < D_8014CE64[D_8014CCB4]) {
                    dest[selected * 2] = D_8014CE68[D_8014CCB4][index * 2];
                    dest[selected * 2 + 1] = D_8014CE68[D_8014CCB4][index * 2 + 1];
                } else {
                    dest[selected * 2] = D_800B3644[0];
                    dest[selected * 2 + 1] = D_800B3644[1];
                }
            } else if (D_8014CCC4 == 0) {
                if (D_8014CCC0 < func_800AE0E0(D_8014CDB0[D_8014CCB4]) / 2) {
                    dest[selected * 2] = D_8014CDB0[D_8014CCB4][D_8014CCC0 * 2];
                    dest[selected * 2 + 1] = D_8014CDB0[D_8014CCB4][D_8014CCC0 * 2 + 1];
                } else {
                    dest[selected * 2] = D_800B3644[0];
                    dest[selected * 2 + 1] = D_800B3644[1];
                }
            } else {
                index = D_8014CCC4 * 16 + D_8014CCAC * 16 + D_8014CCC0 - 16;
                if (index < D_8014CE64[D_8014CCB4]) {
                    dest[selected * 2] = D_8014CE68[D_8014CCB4][index * 2];
                    dest[selected * 2 + 1] = D_8014CE68[D_8014CCB4][index * 2 + 1];
                } else {
                    dest[selected * 2] = D_800B3644[0];
                    dest[selected * 2 + 1] = D_800B3644[1];
                }
            }
        }
        break;
    }
    D_8014CC70++;
}
