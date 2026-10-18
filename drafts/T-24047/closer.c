void func_80135F4C(void) {
    s16 i;
    u16 size;
    s32 row;
    EdShorts14 sp7C;
    EdShorts14 sp60;

    /* FAKE: unsigned views of the width halfwords avoid a second size
     * constant range; the shared Rec24 view declares these as s16. T-24047. */
    sp7C = D_8013CA60;
    sp60 = D_8013CA7C;
    D_8013C358 = 0;
    D_8013C35C = 0;
    func_800847B8(D_800E6280.unk_F5F);
    func_80134D20();
    tpage_buf_clear_all();
    D_800E6280.unk_1228[0x1C] = 1;
    D_800E6280.unk_1228[0x1D] = 1;
    D_800E6280.unk_1228[0x1E] = 1;
    D_800E6280.unk_1228[5] = 1;
    D_800E6280.unk_1228[6] = 1;
    D_800E6280.unk_1228[7] = 1;
    load_palette(D_8013C5F0.unk_10[1], 0x11, 1, 2, 0);
    LoadSquare(0x140U, 0U, 0x40U, 0x80U, D_8013C5F0.unk_10[2]);
    LoadSquare(0x180U, 0U, 0x40U, 0x80U, D_8013C5F0.unk_10[3]);
    LoadSquare(0x1C0U, 0U, 0x40U, 0x80U, D_8013C5F0.unk_10[4]);
    size = 0x80;
    for (i = 0; i < 3; i++) {
        D_801217D0[i + 16].unk_00 = 0x01000000;
        *(u16 *)&D_801217D0[i + 16].unk_08 = size;
        D_801217D0[i + 16].unk_0A = (u32)size;
        D_801217D0[i + 16].unk_0E = 0;
        D_801217D0[i + 16].unk_0F = 0;
        D_801217D0[i + 16].unk_10 = 0;
        D_801217D0[i + 16].unk_0C = i + 5;
        D_801217D0[i + 16].unk_12 = 0x1F1;
        D_801217D0[i + 16].unk_14 = (u32)size;
        D_801217D0[i + 16].unk_15 = (u32)size;
        D_801217D0[i + 16].unk_16 = (u32)size;
        D_801217D0[i + 16].unk_18 = 0;
        D_801217D0[i + 16].unk_1A = 0;
        D_801217D0[i + 16].unk_1C = 0x1000;
        D_801217D0[i + 16].unk_1E = 0x1000;
        D_801217D0[i + 16].unk_20 = 0;
        D_801217D0[i + 16].unk_04 = (i << 7) - 0xA0;
        D_801217D0[i + 16].unk_06 = -0xBC;
        /* FAKE: address roundtrip preserves the original store dependency
         * against the Rec24 updates. Direct member access moves it. T-24047. */
        *(u8 *)(u32)&D_800E6280.unk_10A5[i + 16] = 0xD;
    }
    load_palette(D_8013C5F0.unk_10[0], 0x12, 1, 3, 0);
    for (i = 0; i < 14; i++) {
        func_80048F64(i + 96);
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_01 = 0xC;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_02 = 1;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_38 = 0x51000000;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_03 = 0x84;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_0C = (void *)D_8013C5F0.unk_04;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_10 = (void *)D_8013C5F0.unk_08;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_34 = (void *)D_8013C5F0.unk_00;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_14 = D_8013C5F0.unk_0C;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_16 = rand() % 5;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_18 = rand() % 7;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_06 = 1;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_43 = 0x12;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_26 = sp7C.v[i] - 0xA0;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_2A = sp60.v[i] - 0xBC;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_07 = 0x40;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_05 = 0;
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_04 = 8;
        D_8013CA1C[i] = rand() % 60;
    }
    func_80134DE0();
    load_palette(D_8013C620->unk_10[0], 0x13, 1, 2, 0);
    LoadSquare(0x140U, size, 0x40U, size, D_8013C620->unk_10[1]);
    LoadSquare(0x180U, size, 0x40U, size, D_8013C620->unk_10[2]);
    LoadSquare(0x1C0U, size, 0x40U, size, D_8013C620->unk_10[5]);
    LoadSquare(0x200U, size, 0x40U, size, D_8013C620->unk_10[6]);
    LoadSquare(0x240U, size, 0x40U, size, D_8013C620->unk_10[7]);
    LoadSquare(0x280U, size, 0x40U, size, D_8013C620->unk_10[8]);
    LoadSquare(0x140U, 0x100U, 0x40U, size, D_8013C620->unk_10[9]);
    LoadSquare(0x180U, 0x100U, 0x40U, size, D_8013C620->unk_10[0xA]);
    LoadSquare(0x1C0U, 0x100U, 0x40U, size, D_8013C620->unk_10[0xB]);
    LoadSquare(0x200U, 0x100U, 0x40U, size, D_8013C620->unk_10[0xC]);
    LoadSquare(0x240U, 0x100U, 0x40U, size, D_8013C620->unk_10[3]);
    LoadSquare(0x280U, 0x100U, 0x40U, size, D_8013C620->unk_10[4]);
    D_800E6280.unk_1228[0x25] = 1;
    D_800E6280.unk_1228[0x26] = 1;
    D_800E6280.unk_1228[0x27] = 1;
    D_800E6280.unk_1228[0x28] = 1;
    D_800E6280.unk_1228[0x29] = 1;
    D_800E6280.unk_1228[0x2A] = 1;
    D_800E6280.unk_1228[0x15] = 1;
    D_800E6280.unk_1228[0x16] = 1;
    D_800E6280.unk_1228[0x17] = 1;
    D_800E6280.unk_1228[0x18] = 1;
    D_800E6280.unk_1228[0x19] = 1;
    D_800E6280.unk_1228[0x1A] = 1;
    for (i = 0; i < 12; i++) {
        D_801217D0[i + 4].unk_00 = 0x41000000;
        *(u16 *)&D_801217D0[i + 4].unk_08 = size;
        D_801217D0[i + 4].unk_0A = (u32)size;
        D_801217D0[i + 4].unk_0E = 0;
        row = i / 6;
        D_801217D0[i + 4].unk_10 = 0;
        D_801217D0[i + 4].unk_0F = 0x80 - (row << 7);
        D_801217D0[i + 4].unk_12 = 0x1F3;
        D_801217D0[i + 4].unk_14 = (u32)size;
        D_801217D0[i + 4].unk_15 = (u32)size;
        D_801217D0[i + 4].unk_16 = (u32)size;
        D_801217D0[i + 4].unk_18 = 0;
        D_801217D0[i + 4].unk_1A = 0;
        D_801217D0[i + 4].unk_1C = 0x1000;
        D_801217D0[i + 4].unk_1E = 0x1000;
        D_801217D0[i + 4].unk_20 = 0;
        D_801217D0[i + 4].unk_0C = (i % 6) + ((i / 6) * 0x10) + 5;
        D_801217D0[i + 4].unk_04 = ((i % 3) << 7) - 0xA0;
        D_801217D0[i + 4].unk_06 = ((i / 3) << 7) - 0x188;
        D_800E6280.unk_10A5[i + 4] = 0xB;
    }
    func_80132000();
    switch (D_800E6280.unk_F5F) {
    case 0:
        D_8013C34C.unk_00 = D_8013C280[0][2];
        D_8013C34C.unk_04 = D_8013C280[1][2];
        D_8013C34C.unk_08 = D_8013C280[2][2];
        func_80137160(D_8013C620->unk_00, D_8013C620->unk_04, D_8013C620->unk_08, (s32) D_8013C620->unk_0C);
        break;
    case 1:
        D_8013C34C.unk_00 = D_8013C280[0][8];
        D_8013C34C.unk_04 = D_8013C280[1][8];
        D_8013C34C.unk_08 = D_8013C280[2][8];
        if (!(D_800E6280.unk_1BC[1].unk_0C.f.b14)) {
            func_80137160(D_8013C620[1].unk_00, D_8013C620[1].unk_04, D_8013C620[1].unk_08, (s32) D_8013C620[1].unk_0C);
        } else {
            func_80137160(D_8013C620[0xD].unk_00, D_8013C620[0xD].unk_04, D_8013C620[0xD].unk_08, (s32) D_8013C620[0xD].unk_0C);
            D_8013C358 = 1;
        }
        break;
    case 2:
        D_8013C34C.unk_00 = D_8013C280[0][4];
        D_8013C34C.unk_04 = D_8013C280[1][4];
        D_8013C34C.unk_08 = D_8013C280[2][4];
        func_80137160(D_8013C620[2].unk_00, D_8013C620[2].unk_04, D_8013C620[2].unk_08, (s32) D_8013C620[2].unk_0C);
        break;
    case 3:
        D_8013C34C.unk_00 = D_8013C280[0][6];
        D_8013C34C.unk_04 = D_8013C280[1][6];
        D_8013C34C.unk_08 = D_8013C280[2][6];
        func_80137160(D_8013C620[3].unk_00, D_8013C620[3].unk_04, D_8013C620[3].unk_08, (s32) D_8013C620[3].unk_0C);
        break;
    case 4:
        D_8013C34C.unk_00 = D_8013C280[0][0xC];
        D_8013C34C.unk_04 = D_8013C280[1][0xC];
        D_8013C34C.unk_08 = D_8013C280[2][0xC];
        func_80137160(D_8013C620[4].unk_00, D_8013C620[4].unk_04, D_8013C620[4].unk_08, (s32) D_8013C620[4].unk_0C);
        break;
    case 5:
        D_8013C34C.unk_00 = D_8013C280[0][9];
        D_8013C34C.unk_04 = D_8013C280[1][9];
        D_8013C34C.unk_08 = D_8013C280[2][9];
        func_80137160(D_8013C620[5].unk_00, D_8013C620[5].unk_04, D_8013C620[5].unk_08, (s32) D_8013C620[5].unk_0C);
        break;
    case 6:
        D_8013C34C.unk_00 = D_8013C280[0][0xA];
        D_8013C34C.unk_04 = D_8013C280[1][0xA];
        D_8013C34C.unk_08 = D_8013C280[2][0xA];
        if (!(D_800E6280.unk_53C[6] & 1)) {
            func_80137160(D_8013C620[6].unk_00, D_8013C620[6].unk_04, D_8013C620[6].unk_08, (s32) D_8013C620[6].unk_0C);
        } else {
            func_80137160(D_8013C620[0xE].unk_00, D_8013C620[0xE].unk_04, D_8013C620[0xE].unk_08, (s32) D_8013C620[0xE].unk_0C);
            D_8013C358 = 1;
        }
        break;
    case 7:
        D_8013C34C.unk_00 = D_8013C280[0][7];
        D_8013C34C.unk_04 = D_8013C280[1][7];
        D_8013C34C.unk_08 = D_8013C280[2][7];
        func_80137160(D_8013C620[7].unk_00, D_8013C620[7].unk_04, D_8013C620[7].unk_08, (s32) D_8013C620[7].unk_0C);
        break;
    case 8:
        D_8013C34C.unk_00 = D_8013C280[0][1];
        D_8013C34C.unk_04 = D_8013C280[1][1];
        D_8013C34C.unk_08 = D_8013C280[2][1];
        func_80137160(D_8013C620[8].unk_00, D_8013C620[8].unk_04, D_8013C620[8].unk_08, (s32) D_8013C620[8].unk_0C);
        break;
    case 9:
        D_8013C34C.unk_00 = D_8013C280[0][0xB];
        D_8013C34C.unk_04 = D_8013C280[1][0xB];
        D_8013C34C.unk_08 = D_8013C280[2][0xB];
        func_80137160(D_8013C620[9].unk_00, D_8013C620[9].unk_04, D_8013C620[9].unk_08, (s32) D_8013C620[9].unk_0C);
        break;
    case 10:
        D_8013C34C.unk_00 = D_8013C280[0][0xE];
        D_8013C34C.unk_04 = D_8013C280[1][0xE];
        D_8013C34C.unk_08 = D_8013C280[2][0xE];
        func_80137160(D_8013C620[0xA].unk_00, D_8013C620[0xA].unk_04, D_8013C620[0xA].unk_08, (s32) D_8013C620[0xA].unk_0C);
        break;
    case 11:
        D_8013C34C.unk_00 = D_8013C280[0][0x10];
        D_8013C34C.unk_04 = D_8013C280[1][0x10];
        D_8013C34C.unk_08 = D_8013C280[2][0x10];
        func_80137160(D_8013C620[0xB].unk_00, D_8013C620[0xB].unk_04, D_8013C620[0xB].unk_08, (s32) D_8013C620[0xB].unk_0C);
        break;
    default:
        D_8013C34C.unk_00 = D_8013C280[0][5];
        D_8013C34C.unk_04 = D_8013C280[1][5];
        D_8013C34C.unk_08 = D_8013C280[2][5];
        func_80137160(D_8013C620[0xC].unk_00, D_8013C620[0xC].unk_04, D_8013C620[0xC].unk_08, (s32) D_8013C620[0xC].unk_0C);
        break;
    }
    D_8013CA38 = 1;
    func_80046290(0, 0, (s32) D_800E6280.unk_F5F);
    func_80044750(0x300);
    func_8004284C();
}
