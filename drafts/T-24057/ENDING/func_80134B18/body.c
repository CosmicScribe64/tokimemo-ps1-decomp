/* NON_MATCHING: T-24057, source-only scheduling lead.
 * FAKE: explicit GameState byte views preserve the promoted colour register. */
void func_80134B18(void) {
    s16 i;

    (*(u8 *)&D_800E6280.unk_036) += 8;
    if ((*(u8 *)&D_800E6280.unk_036) == 0 || (*(u8 *)&D_800E6280.unk_036) == 7) {
        (*(u8 *)&D_800E6280.unk_036) = 0xFF;
    }
    (*(u8 *)&D_800E6280.unk_037) = (*(u8 *)&D_800E6280.unk_038) = (*(u8 *)&D_800E6280.unk_036);
    ((DisplayActor44 *)D_80120650)->unk_07 += 4;
    ((DisplayActor44 *)D_80120694)->unk_07 += 4;
    for (i = 4; i < 0xA; i++) {
        func_80084C98(i, ((DisplayActor44 *)D_80120650)->unk_07);
    }
    if (((DisplayActor44 *)D_80120650)->unk_07 >= 0x80) {
        ((DisplayActor44 *)D_80120650)->unk_02 = 5;
        func_8004284C();
    }
}
