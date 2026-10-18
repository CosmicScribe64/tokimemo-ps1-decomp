#define DISP(n) (((DisplayActor44 *)D_8011ECD0)[n])
void func_801365C8(void) {
    s32 i;
    s32 j;
    s32 span;

    for (i = 0; i < 2; i++) {
        ++D_801492A8[i];
        if (D_801492A8[i] >= 121U) {
            D_801492A8[i] = 0;
        } else if (D_801492A8[i] == 10) {
            D_801492A4[i] = func_800AE0D0() % 20 + 10;
        } else if (D_801492A8[i] == 27) {
            D_801492A4[i] = 0xFF;
        } else if (D_801492A8[i] >= 19U && D_801492A4[i] != 0xFF) {
            DISP(46 + D_801492A4[i]).unk_2A++;
        } else if (D_801492A8[i] >= 11U && D_801492A4[i] != 0xFF) {
            DISP(46 + D_801492A4[i]).unk_2A--;
        }
    }
    for (i = 0, span = 0; i < 30; i++, span += 32) {
        if (span >= 320) {
            span -= 320;
        }
        DISP(46 + i).unk_26 = span + 480 - D_80149298.halves.high;
        if (i >= 10 && i < 20) {
            DISP(46 + i).unk_26 += 16;
        }
        if (i == 19) {
            DISP(46 + i).unk_02 = 0;
            DISP(46 + i).unk_03 = 0;
        } else if (DISP(46 + i).unk_26 >= -207 && DISP(46 + i).unk_26 < 321) {
            DISP(46 + i).unk_02 = 1;
            DISP(46 + i).unk_03 = 0xA4;
        } else {
            DISP(46 + i).unk_02 = 0;
            DISP(46 + i).unk_03 = 0;
        }
        /* FAKE: contiguous four-runner view as in matching neighbors. T-24008 */
        j = 0;
        while (1) {
            if (i == D_801491D8 && DISP(107).unk_02 != 0 && D_80122E94 == 1) {
                DISP(46 + i).unk_02 = 0;
                DISP(46 + i).unk_03 = 0;
            } else if (i == (&D_80149208.player)[j].unk_02 && DISP(107 + j).unk_02 != 0) {
                if (j == 0) {
                    if (D_80122E94 == 0xFF) {
                        DISP(46 + i).unk_02 = 0;
                        DISP(46 + i).unk_03 = 0;
                    }
                } else {
                    DISP(46 + i).unk_02 = 0;
                    DISP(46 + i).unk_03 = 0;
                }
            }
            if (i == (&D_80149208.player)[j].unk_02 && DISP(107 + j).unk_02 == 0) {
                DISP(46 + i).unk_02 = 1;
                DISP(46 + i).unk_03 = 0xA4;
            }
            j++;
            if (j == 4) {
                break;
            }
        }
    }
}
#undef DISP
