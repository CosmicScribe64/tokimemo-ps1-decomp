/* T-17110 hand draft of RPG_BAT func_8014BFE4 (128 funcdiff rows, frame 0x68 right).
 * Display of a number with up to 4 digits (records 64+12*slot..+11). Open: head chain
 * (`arg1 += 4/9/13`) takes a copy where the original compares a3; the original keeps div in a1
 * with a spill and idx+8 in s5 across the first call; t = arg0 * 12 is spilled alone at sp+0x50
 * and t + 0x40 is recomputed after the loop. Needs: D_8015EE5C.. externs (RPG_BAT.h). */
void func_8014BFE4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 i;
    s32 idx;
    s32 div;

    arg1 -= 0xA0;
    arg2 -= 0x78;
    if (arg3 >= 10) {
        if (arg3 < 100) {
            arg1 += 4;
        } else if (arg3 < 1000) {
            arg1 += 9;
        } else {
            arg1 += 13;
        }
    }
    switch (arg0) {
    case 0:
        D_8015EE5C = 0;
        D_8015EE60 = 1;
        D_8015EE64 = 0;
        break;
    case 1:
        D_8015EE68 = 0;
        D_8015EE6C = 1;
        D_8015EE70 = 0;
        break;
    case 2:
        D_8015EE74 = 0;
        D_8015EE78 = 1;
        D_8015EE7C = 0;
        break;
    case 3:
        D_8015EE80 = 0;
        D_8015EE84 = 1;
        D_8015EE88 = 0;
        break;
    }
    idx = arg0 * 12 + 0x40;
    div = 1;
    for (i = 0; i < 4; i++) {
        ((DisplayActor44 *) D_8011ECD0)[64 + arg0 * 12 + i].unk_26 = arg1 - i * 9;
        ((DisplayActor44 *) D_8011ECD0)[64 + arg0 * 12 + i].unk_2A = arg2;
        ((DisplayActor44 *) D_8011ECD0)[68 + arg0 * 12 + i].unk_26 = arg1 - i * 9;
        ((DisplayActor44 *) D_8011ECD0)[68 + arg0 * 12 + i].unk_2A = arg2;
        ((DisplayActor44 *) D_8011ECD0)[72 + arg0 * 12 + i].unk_26 = arg1 - i * 9;
        ((DisplayActor44 *) D_8011ECD0)[72 + arg0 * 12 + i].unk_2A = arg2;
        if (arg3 / div != 0) {
            func_8013E864(idx + 4, arg3 / div % 10 + 0x27, 0);
            func_8013E864(idx + 8, arg3 / div % 10 + 0x27, 0);
            if (arg4 == 0) {
                func_8013E864(idx, arg3 / div % 10 + 0x1D, 0);
            } else {
                func_8013E864(idx, arg3 / div % 10 + 0x13, 0);
            }
        } else {
            ((DisplayActor44 *) D_8011ECD0)[idx].unk_26 = 0xA0;
            ((DisplayActor44 *) D_8011ECD0)[idx].unk_2A = 0x78;
            ((DisplayActor44 *) D_8011ECD0)[idx + 4].unk_26 = 0xA0;
            ((DisplayActor44 *) D_8011ECD0)[idx + 4].unk_2A = 0x78;
            ((DisplayActor44 *) D_8011ECD0)[idx + 8].unk_26 = 0xA0;
            ((DisplayActor44 *) D_8011ECD0)[idx + 8].unk_2A = 0x78;
        }
        idx++;
        div *= 10;
    }
    if (arg3 == 0) {
        ((DisplayActor44 *) D_8011ECD0)[64 + arg0 * 12].unk_2A = arg2;
        ((DisplayActor44 *) D_8011ECD0)[68 + arg0 * 12].unk_2A = arg2;
        ((DisplayActor44 *) D_8011ECD0)[72 + arg0 * 12].unk_2A = arg2;
        ((DisplayActor44 *) D_8011ECD0)[64 + arg0 * 12].unk_26 = arg1;
        ((DisplayActor44 *) D_8011ECD0)[68 + arg0 * 12].unk_26 = arg1;
        ((DisplayActor44 *) D_8011ECD0)[72 + arg0 * 12].unk_26 = arg1;
        func_8013E864(arg0 * 12 + 0x44, 0x27, 0);
        idx = arg0 * 12 + 0x40;
        func_8013E864(idx + 8, 0x27, 0);
        if (arg4 == 0) {
            func_8013E864(idx, 0x1D, 0);
        } else {
            func_8013E864(idx, 0x13, 0);
        }
    }
}
