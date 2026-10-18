/* NON_MATCHING: T-24067; source-only cost18 checkpoint, not a verified C match. */
void func_80141ABC(void) {
    GameState *cursor;
    s32 count;
    u8 value;
    u8 *format;
    GameState *end;

    func_800AE080(D_800E7D10.unk_058, 0x800);
    format = "%d %d %d %d %d \n";
    end = (GameState *)((u8 *)&D_800E6280 + 0x800);
    cursor = &D_800E6280;
    count = 0;
    do {
        value = cursor->unk_75E[0].unk_02;
        /* FAKE: eliminated identity test retains the format range so fmt/end/save
         * receive the original saved registers. Original source unknown. T-24067 */
        func_800AE0B0((void *)(format == 0 ? format : format), cursor->unk_75E[0].unk_04,
            cursor->unk_75E[0].unk_05, cursor->unk_75E[0].unk_06, value, value);
        if (D_800E6280.unk_03E && D_800E6280.unk_03F && D_800E6280.unk_040 &&
            (D_800E6280.unk_F5F & 0xF) == cursor->unk_75E[0].unk_00) {
            ((GsRec75E *)D_800E7D10.unk_058)[count] = cursor->unk_75E[0];
            count++;
        }
        cursor = (GameState *)((u8 *)cursor + 8);
    } while (cursor != end);
    D_800E7D10.unk_027[1] = count;
    D_800E7D10.unk_027[0] = 1;
    func_800AE0B0("ALBUM_MODE IN SAVE %d \n", D_800E7D10.unk_027[0]);
}
