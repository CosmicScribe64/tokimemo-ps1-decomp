void func_80133F50(void) {
    s32 i;
    s32 j;
    s32 old;
    s32 keep;
    s32 sel;

    ((DisplayActor44 *)D_8011ECD0)[106].unk_03 = 0x80;
    old = D_800E6280.unk_1094;
    func_8004F984(1, ((DisplayActor44 *)D_8011ECD0)->unk_26, ((DisplayActor44 *)D_8011ECD0)->unk_2A, old);
    func_8004FC10(1);
    func_801320C0();
    sel = D_800E6280.unk_1094;
    for (i = 0, j = 0; i < 3; i++, j += 2) {
        if (i == sel) {
            ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j + 0x15;
        } else {
            ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j + 0x16;
        }
    }
    if (old != sel) {
        switch (sel) {
        case 0:
            keep = (u8)D_800E6280.unk_F6C;
            D_800E6280.unk_F6C = 0;
            func_8006509C();
            /* FAKE: loop body on the for line, as in the matched func_80134210 (T-7020). T-20910 */
            for (i = 0; i < 16; i++) *(s16 *)&D_80125C10[i * 4] += 200;
            func_800654D0(D_800E6280.unk_F6D = 0xE);
            D_800E6280.unk_F6C = keep;
            break;
        case 1:
            keep = (u8)D_800E6280.unk_F6C;
            D_800E6280.unk_F6C = 1;
            func_8006509C();
            /* FAKE: loop body on the for line, as in the matched func_80134210 (T-7020). T-20910 */
            for (i = 0; i < 16; i++) *(s16 *)&D_80125C10[i * 4] += 200;
            func_800654D0(D_800E6280.unk_F6D = 0xE);
            D_800E6280.unk_F6C = keep;
            break;
        }
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1094) {
        case 0:
            D_800E6280.unk_F6C = 3;
            func_80134210();
            func_8004284C();
            func_80044750(0x501);
            break;
        case 1:
            D_800E6280.unk_F6C = 4;
            func_80134210();
            func_8004284C();
            func_80044750(0x501);
            break;
        case 2:
            D_800E6280.unk_F6C = 5;
            func_80134210();
            func_8004284C();
            func_80044750(0x501);
            break;
        }
    }
    if (D_800E6280.unk_F88 & 0x40) {
        func_80134210();
        func_8004284C();
        D_800E6280.unk_110D = 2;
    }
}
