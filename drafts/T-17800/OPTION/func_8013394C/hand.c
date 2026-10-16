s32 func_8013394C(void) {
    s32 i;
    s32 j;
    u32 sel;

    /* one table: as1 keeps the loads after the store only when both go through D_8011ECD0 (T-9020) */
    D_8011ECD0[0x1C2B] = 0x80;
    func_8004F984(1, *(s16 *)&D_8011ECD0[0x26], *(s16 *)&D_8011ECD0[0x2A]);
    func_8004FC10(1);
    func_801320C0();
    sel = D_800E6280.unk_1094;
    for (i = 0, j = 0; i < 3; i++, j += 2) {
        if (i == sel) {
            ((OptSpr44 *)D_80120650)[i + 11].unk_18 = j + 0xD;
        } else {
            ((OptSpr44 *)D_80120650)[i + 11].unk_18 = j + 0xE;
        }
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (sel) {
        case 0:
            k_speed_set(0);
            D_800E6280.unk_1124 |= 0x80;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 1:
            k_speed_set(1);
            D_800E6280.unk_1124 |= 0x80;
            func_8004284C();
            func_80044750(0x501);
            break;
        case 2:
            k_speed_set(3);
            D_800E6280.unk_1124 |= 0x80;
            func_8004284C();
            func_80044750(0x501);
            break;
        }
    }
    if (D_800E6280.unk_F88 & 0x40) {
        func_8004284C();
        D_800E6280.unk_110D = 2;
    }
}
