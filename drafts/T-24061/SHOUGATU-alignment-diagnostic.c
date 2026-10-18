/* T-24061 source-only unmatched draft. See README.md for reproduction and negatives. */
/* FAKE: diagnostic source shape only; not claimed as recovered original C. */
void func_801359FC(void) {
    u32 storage[9];
    ShoStr7x5 *labels = (ShoStr7x5 *)storage;
    u32 selection;

    *labels = D_80143E80;
    selection = D_800E6280.unk_10F8 & 7;
    if (selection != 0) {
        selection = (u8)(selection - 1);
    }
    if (D_800E6280.unk_F5F == 3 && selection == 3) {
        selection = 4;
    }
    D_80143DD8 = selection;
    func_800AE0F0(D_800CA248, labels->s[selection]);
    selection = D_80143DD8;
    if (selection == 0) {
        D_800E6280.unk_11C.unk_02 -= 5;
    }
    D_80143DD8 = selection;
    if (selection == 4) {
        D_800E6280.unk_11C.unk_02 += 5;
    }
    check_para_limit();
}
