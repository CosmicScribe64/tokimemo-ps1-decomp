/* FAKE: u8 views keep one promoted counter range as in func_8013987C; original source unknown. T-24035. */
void func_80134418(void) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        ((DisplayActor44 *)D_80120650)[i].unk_07 = *(u8 *)&D_800E6280.unk_110B;
    }
    *(u8 *)&D_800E6280.unk_110B += 2;
    if (*(u8 *)&D_800E6280.unk_110B > 0x80) {
        /* FAKE: clear/add preserves the promoted cap range instead of splitting the constant; two redundant narrowing instructions remain. T-24035. */
        *(u8 *)&D_800E6280.unk_110B *= 0;
        *(u8 *)&D_800E6280.unk_110B += 0x80;
        for (i = 0; i < 0x40; i++) {
            ((DisplayActor44 *)D_80120650)[i].unk_07 = *(u8 *)&D_800E6280.unk_110B;
        }
        func_8004284C();
    }
}