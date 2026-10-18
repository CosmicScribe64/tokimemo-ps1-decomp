/* NON_MATCHING: T-24067; source-only cost8/8 rows, not a verified C match. */
void func_80141ABC(void) {
    s32 count;
    u8 value;
    s32 i;

    func_800AE080(D_800E7D10.unk_058, 0x800);
    /* FAKE: retain this count-initializer/for source-line group for the
     * original preheader schedule. Original line layout unknown. T-24067 */
    count = 0; for (i = 0; i < 256; i++) {
        value = D_800E6280.unk_75E[i].unk_02;
        func_800AE0B0("%d %d %d %d %d \n", D_800E6280.unk_75E[i].unk_04,
            D_800E6280.unk_75E[i].unk_05, D_800E6280.unk_75E[i].unk_06, value, value);
        if (!D_800E6280.unk_03E || !D_800E6280.unk_03F || !D_800E6280.unk_040 ||
            (D_800E6280.unk_F5F & 0xF) != D_800E6280.unk_75E[i].unk_00) {
            continue;
        }
        ((GsRec75E *)D_800E7D10.unk_058)[count] = D_800E6280.unk_75E[i];
        count++;
    }
    D_800E7D10.unk_027[1] = count;
    D_800E7D10.unk_027[0] = 1;
    func_800AE0B0("ALBUM_MODE IN SAVE %d \n", D_800E7D10.unk_027[0]);
}
