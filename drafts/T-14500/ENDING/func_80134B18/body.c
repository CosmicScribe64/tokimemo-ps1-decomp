/* NON_MATCHING: T-10520, only the register of the promoted D_80120657 differs ($v0 here, $a1 in the original: one range across the calls), 9 lines */
void func_80134B18(void) {
    s16 i;

    D_800E6280.unk_036 += 8;
    if (D_800E6280.unk_036 == 0 || D_800E6280.unk_036 == 7) {
        D_800E6280.unk_036 = 0xFF;
    }
    D_800E6280.unk_037 = D_800E6280.unk_038 = D_800E6280.unk_036;
    D_80120657 += 4;
    D_8012069B += 4;
    for (i = 4; i < 0xA; i++) {
        func_80084C98(i, D_80120657);
    }
    if (D_80120657 >= 0x80) {
        D_80120652 = 5;
        func_8004284C();
    }
}
