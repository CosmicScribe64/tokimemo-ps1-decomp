/* FAKE: T-25057 discarded unsigned tests retain record induction and remainder ranges until ugen; diagnostic only, not matched. */

void func_801398EC(void) {
    s32 tmp;
    s32 i;
    u32 tile;

    i = 0;
    tmp = D_8014D0D4 % 3;
    for (; i < 13; i++) {
        if ((u32)D_801217D0[i].unk_00 < 0) {}
        tile = (D_800E6280.unk_10F8 >> 2) % 3;
        if (tmp == 0) {
            if (((NameRecordF00 *)D_80123120)[D_800E8BEF].flags[i] == 0) {
                D_801217D0[i].unk_00 = 0;
                D_801217D0[i].unk_0E = (i / 8) * 0x30 + tile * 0x10 + 0x80;
                if (tile < 0) {}
            } else {
                D_801217D0[i].unk_00 = 0x80000000;
            }
        } else {
            D_801217D0[i].unk_00 = 0x80000000;
        }
    }
}
