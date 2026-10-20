/* T-24515 source-only indexed-record reconstruction; not a verified match. */
void func_801398EC(void) {
    s32 tmp;
    s32 i;

    tmp = D_8014D0D4 % 3;
    for (i = 0; i < 13; i++) {
        if (tmp == 0) {
            if (((NameRecordF00 *)D_80123120)[D_800E8BEF].flags[i] == 0) {
                D_801217D0[i].unk_00 = 0;
                D_801217D0[i].unk_0E = (i / 8) * 0x30 + ((D_800E6280.unk_10F8 >> 2) % 3) * 0x10 + 0x80;
            } else {
                D_801217D0[i].unk_00 = 0x80000000;
            }
        } else {
            D_801217D0[i].unk_00 = 0x80000000;
        }
    }
}
