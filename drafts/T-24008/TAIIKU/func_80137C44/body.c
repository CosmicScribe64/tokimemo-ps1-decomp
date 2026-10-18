#define DISP(n) (((DisplayActor44 *)D_8011ECD0)[n])
s32 func_80137C44(void) {
    s32 i;
    s32 mode;
    u8 selected;
    u8 frame;
    u8 slot;
    s32 x;
    s16 off;
    TaiRunnerA *runner;
    DisplayActor44 *actor;

    mode = D_80122E94;
    selected = D_801491D8;
    frame = D_80149208.player.unk_04;
    slot = D_801491DC;

    /* FAKE: four-runner view of player plus cpu[3], as matched neighbors. T-24008 */
    for (i = 0, runner = &D_80149208.player; i < 4; i++, runner++) {
        if (runner->unk_00 != 0) {
            actor = &DISP(i);
            actor[107].unk_26 = actor[96].unk_26 - 8;
            if (i == 0) {
                off = D_801492B8[frame];
                if (mode == 1) {
                    DISP(107).unk_18 = ((DISP(46 + selected).unk_16 >> 3) * 6) + off;
                } else {
                    DISP(107).unk_18 = ((DISP(46 + runner->unk_02).unk_16 >> 3) * 6) + off;
                }
            } else {
                actor[107].unk_18 = ((DISP(46 + runner->unk_02).unk_16 >> 3) * 6) + D_801492B8[runner->unk_03];
            }
            if (i + 0x90 == actor[96].unk_18) {
                if (mode == 1) {
                    actor[107].unk_18 = D_80149320[slot] * 6 + 5;
                } else {
                    actor[107].unk_18 = ((DISP(46 + runner->unk_02).unk_16 >> 3) * 6) + 5;
                }
            }
        }
    }
    for (i = 0; i < 3; i++) {
        if (((D_80149208.cpu[i].unk_05 == 2 && DISP(108 + i).unk_02 != 0) || D_80149208.cpu[i].unk_00 == 1) && (x = D_80149208.cpu[i].unk_20.halves.high - D_80149298.halves.high) < 0x140 && x >= -0x13F) {
            DISP(108 + i).unk_03 = 0xA4;
        } else {
            DISP(108 + i).unk_03 = 0;
        }
    }
}
#undef DISP
