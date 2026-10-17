void func_80135F4C(void) {
    EdShorts14 sp7C;
    EdShorts14 sp60;
    DisplayActor44 *temp_s0;
    Rec24 *temp_v0_2;
    s16 *temp_at;
    s16 temp_s1;
    s16 temp_t2;
    s16 temp_t4;
    s16 temp_t7;
    s16 var_s1;
    s16 var_s1_2;
    s16 var_s1_3;
    s32 temp_at_2;
    s32 temp_hi;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_s2;
    s32 temp_t9;
    u8 *temp_t3;
    Rec24 *temp_v0;

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
    var_s1 = 0;
    do {
        temp_lo = var_s1 * 0x24;
        temp_t2 = var_s1 + 5;
        temp_t9 = var_s1 << 7;
        temp_t3 = (u8 *)&D_800E6280 + var_s1;
        var_s1 += 1;
        temp_v0 = (Rec24 *)((u8 *)D_801217D0 + temp_lo);
        temp_v0[16].unk_00 = 0x01000000;
        temp_v0[16].unk_08 = 0x80;
        temp_v0[16].unk_0A = 0x80;
        temp_v0[16].unk_0E = 0;
        temp_v0[16].unk_0F = 0;
        temp_v0[16].unk_10 = 0;
        temp_v0[16].unk_0C = temp_t2;
        temp_v0[16].unk_12 = 0x1F1;
        temp_v0[16].unk_14 = 0x80;
        temp_v0[16].unk_15 = 0x80;
        temp_v0[16].unk_16 = 0x80;
        temp_v0[16].unk_18 = 0;
        temp_v0[16].unk_1A = 0;
        temp_v0[16].unk_1C = 0x1000;
        temp_v0[16].unk_1E = 0x1000;
        temp_v0[16].unk_20 = 0;
        temp_v0[16].unk_04 = (s16) (temp_t9 - 0xA0);
        temp_v0[16].unk_06 = -0xBC;
        *(s8 *)((unsigned char *)temp_t3 + (4277)) = 0xD;
    } while (var_s1 < 3);
    load_palette(D_8013C5F0.unk_10[0], 0x12, 1, 3, 0);
    var_s1_2 = 0;
    do {
        func_80048F64(var_s1_2 + 0x60);
        temp_s0 = &((DisplayActor44 *)D_8011ECD0)[var_s1_2];
        temp_s0[96].unk_01 = 0xC;
        temp_s0[96].unk_02 = 1;
        temp_s0[96].unk_38 = 0x51000000;
        temp_s0[96].unk_03 = 0x84;
        *(s32 *)&temp_s0[96].unk_0C = (s32) D_8013C5F0.unk_04;
        *(s32 *)&temp_s0[96].unk_10 = (s32) D_8013C5F0.unk_08;
        *(s32 *)&temp_s0[96].unk_34 = (s32) D_8013C5F0.unk_00;
        temp_s0[96].unk_14 = (s16) D_8013C5F0.unk_0C;
        temp_s0[96].unk_16 = (s16) (func_800AE0D0() % 5);
        temp_hi = func_800AE0D0() % 7;
        temp_s2 = var_s1_2 * 2;
        temp_s0[96].unk_06 = 1;
        temp_s0[96].unk_43 = 0x12;
        temp_s0[96].unk_18 = (s16) temp_hi;
        temp_s0[96].unk_26 = (s16) (sp7C.v[var_s1_2] - 0xA0);
        temp_s0[96].unk_07 = 0x40;
        temp_s0[96].unk_2A = (s16) (sp60.v[var_s1_2] - 0xBC);
        temp_s0[96].unk_05 = 0;
        temp_s0[96].unk_04 = 8;
        temp_at = &D_8013CA1C[var_s1_2];
        var_s1_2 += 1;
        *temp_at = (s16) (func_800AE0D0() % 60);
    } while (var_s1_2 < 0xE);
    func_80134DE0();
    load_palette(D_8013C620->unk_10[0], 0x13, 1, 2, 0);
    LoadSquare(0x140U, 0x80 & 0xFFFF, 0x40U, 0x80 & 0xFFFF, D_8013C620->unk_10[1]);
    LoadSquare(0x180U, 0x80 & 0xFFFF, 0x40U, 0x80 & 0xFFFF, D_8013C620->unk_10[2]);
    LoadSquare(0x1C0U, 0x80 & 0xFFFF, 0x40U, 0x80 & 0xFFFF, D_8013C620->unk_10[5]);
    LoadSquare(0x200U, 0x80 & 0xFFFF, 0x40U, 0x80 & 0xFFFF, D_8013C620->unk_10[6]);
    LoadSquare(0x240U, 0x80 & 0xFFFF, 0x40U, 0x80 & 0xFFFF, D_8013C620->unk_10[7]);
    LoadSquare(0x280U, 0x80 & 0xFFFF, 0x40U, 0x80 & 0xFFFF, D_8013C620->unk_10[8]);
    LoadSquare(0x140U, 0x100U, 0x40U, 0x80 & 0xFFFF, D_8013C620->unk_10[9]);
    LoadSquare(0x180U, 0x100U, 0x40U, 0x80 & 0xFFFF, D_8013C620->unk_10[0xA]);
    LoadSquare(0x1C0U, 0x100U, 0x40U, 0x80 & 0xFFFF, D_8013C620->unk_10[0xB]);
    LoadSquare(0x200U, 0x100U, 0x40U, 0x80 & 0xFFFF, D_8013C620->unk_10[0xC]);
    LoadSquare(0x240U, 0x100U, 0x40U, 0x80 & 0xFFFF, D_8013C620->unk_10[3]);
    LoadSquare(0x280U, 0x100U, 0x40U, 0x80 & 0xFFFF, D_8013C620->unk_10[4]);
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
    var_s1_3 = 0;
    do {
        temp_v0_2 = &D_801217D0[var_s1_3];
        temp_v0_2[4].unk_00 = 0x41000000;
        temp_lo_2 = var_s1_3 / 6;
        temp_v0_2[4].unk_08 = 0x80;
        temp_v0_2[4].unk_0A = 0x80;
        temp_v0_2[4].unk_0E = 0;
        temp_v0_2[4].unk_10 = 0;
        temp_v0_2[4].unk_12 = 0x1F3;
        temp_v0_2[4].unk_14 = 0x80;
        temp_v0_2[4].unk_15 = 0x80;
        temp_v0_2[4].unk_16 = 0x80;
        temp_v0_2[4].unk_18 = 0;
        temp_v0_2[4].unk_1A = 0;
        temp_v0_2[4].unk_1C = 0x1000;
        temp_v0_2[4].unk_1E = 0x1000;
        temp_v0_2[4].unk_20 = 0;
        temp_v0_2[4].unk_0F = (s8) (0x80 - (temp_lo_2 << 7));
        temp_v0_2[4].unk_0C = (s16) ((var_s1_3 % 6) + (temp_lo_2 * 0x10) + 5);
        temp_s1 = var_s1_3 + 1;
        *(s8 *)((unsigned char *)((u8 *)&D_800E6280 + var_s1_3) + (4265)) = 0xB;
        temp_at_2 = temp_s1 < 0xC;
        temp_t4 = ((var_s1_3 % 3) << 7) - 0xA0;
        temp_t7 = ((var_s1_3 / 3) << 7) - 0x188;
        var_s1_3 = temp_s1;
        temp_v0_2[4].unk_04 = temp_t4;
        temp_v0_2[4].unk_06 = temp_t7;
    } while (temp_at_2 != 0);
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
        if (!(D_800E6280.unk_1BC[1].unk_0C.w & 0x4000)) {
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
