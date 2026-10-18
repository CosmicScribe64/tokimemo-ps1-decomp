/* NON_MATCHING: T-24073, for-loop initializer-order control,4 resolved rows. */
void func_8014407C(s32 arg0) {
    s32 x;
    s32 remainder;
    s32 i;
    Rec24 *record;

    record = &D_801217D0[2];
    x = -0xB0;
    for (i = 2; i != 5; i++) {
        record->unk_00 = 0;
        record->unk_04 = x;
        remainder = func_800AE0D0() % 112;
        record->unk_06 = remainder - 0x78;
        record++;
        x += 0x6C;
        /* FAKE: unsigned SDK width view shares 0x80 with unk_0A, as matched MAIN80084E90. T-24073. */
        *(u16 *)&record[-1].unk_08 = 0x80;
        record[-1].unk_0A = 0x80;
        record[-1].unk_0C = i + 3;
        record[-1].unk_0E = 0;
        record[-1].unk_0F = 0;
        record[-1].unk_10 = 0;
        record[-1].unk_12 = 0x1F1;
        record[-1].unk_14 = 0;
        record[-1].unk_15 = 0;
        record[-1].unk_16 = 0;
        record[-1].unk_18 = 0;
        record[-1].unk_1A = 0;
        record[-1].unk_1C = 0x1000;
        record[-1].unk_1E = 0x1000;
        record[-1].unk_20 = 0;
    }
    D_800E6280.unk_10A5[2] = 0xFF;
    D_800E6280.unk_10A5[3] = 0xFE;
    D_800E6280.unk_10A5[4] = 0xFF;
    if (arg0 == 0xE) {
        D_801217D0[2].unk_06 = -0x18;
        D_801217D0[3].unk_06 = -0x64;
        D_801217D0[4].unk_06 = -0x28;
    }
}
