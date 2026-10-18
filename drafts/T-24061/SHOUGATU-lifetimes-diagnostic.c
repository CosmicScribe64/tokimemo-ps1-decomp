/* T-24061 source-only unmatched draft. See README.md for reproduction and negatives. */
/* FAKE: diagnostic source shape only; not claimed as recovered original C. */
void func_801359FC(void) {
    u32 seed;
    u8 selection;
    ShoStr7x5 labels;

    labels = D_80143E80;
    seed = *(s32 *)&D_800E6280.unk_10F8 & 7;
    selection = seed;
    if (seed != 0) {
        selection = seed - 1;
    }
    if (*(u8 *)&D_800E6280.unk_F5F == 3 && selection == 3) {
        selection = 4;
    }
    D_80143DD8 = selection;
    func_800AE0F0(D_800CA248, labels.s[selection]);
    selection = D_80143DD8;
    if (selection == 0) {
        *(s16 *)&D_800E6280.unk_11C.unk_02 -= 5;
    }
    D_80143DD8 = selection;
    if (selection == 4) {
        *(s16 *)&D_800E6280.unk_11C.unk_02 += 5;
    }
    check_para_limit();
}
