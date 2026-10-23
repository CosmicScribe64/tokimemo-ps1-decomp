void func_80134270(void) {
    Rec24 *temp_v0;
    s16 temp_v1;
    s16 var_t6;
    s16 var_v1;
    s32 temp_at;
    u8 *temp_t7;

    func_8009C5E0(1);
    func_80133C10();
    load_palette(D_8013C5A0[0], 0x10, 1, 2, 0);
    load_palette(D_8013C5A0[0], 0x11, 1, 2, 1);
    LoadSquare(0x140U, 0x80U, 0x40U, 0x80U, D_8013C5A0[1]);
    LoadSquare(0x180U, 0x80U, 0x40U, 0x80U, D_8013C5A0[2]);
    LoadSquare(0x1C0U, 0x80U, 0x40U, 0x80U, D_8013C5A0[3]);
    LoadSquare(0x200U, 0x80U, 0x40U, 0x80U, D_8013C5A0[4]);
    LoadSquare(0x240U, 0x80U, 0x40U, 0x80U, D_8013C5A0[5]);
    LoadSquare(0x280U, 0x80U, 0x40U, 0x80U, D_8013C5A0[6]);
    D_800E6280.unk_1228[0x25] = 1;
    D_800E6280.unk_1228[0x26] = 1;
    D_800E6280.unk_1228[0x27] = 1;
    D_800E6280.unk_1228[0x28] = 1;
    D_800E6280.unk_1228[0x29] = 1;
    D_800E6280.unk_1228[0x2A] = 1;
    var_v1 = 0;
    do {
        temp_v0 = &D_801217D0[var_v1];
        temp_v0[4].unk_0C = (s16) (var_v1 + 5);
        temp_v0[4].unk_00 = 0x41000000;
        temp_v0[4].unk_08 = 0x80;
        temp_v0[4].unk_0A = 0x80;
        temp_v0[4].unk_0E = 0;
        temp_v0[4].unk_0F = 0x80;
        temp_v0[4].unk_10 = 0;
        temp_v0[4].unk_12 = 0x1F0;
        temp_v0[4].unk_14 = 0;
        temp_v0[4].unk_15 = 0;
        temp_v0[4].unk_16 = 0;
        temp_v0[4].unk_18 = 0;
        temp_v0[4].unk_1A = 0;
        temp_v0[4].unk_1C = 0x1000;
        temp_v0[4].unk_1E = 0x1000;
        temp_v0[4].unk_20 = 0;
        temp_t7 = (u8 *)&D_800E6280 + var_v1;
        temp_v1 = var_v1 + 1;
        temp_v0[4].unk_04 = (s16) (((var_v1 % 3) << 7) - 0x80);
        temp_at = temp_v1 < 6;
        temp_v0[4].unk_06 = (s16) (((var_v1 / 3) << 7) - 0x78);
        var_v1 = temp_v1;
        *(s8 *)((unsigned char *)temp_t7 + (4265)) = 0xB;
    } while (temp_at != 0);
    func_80048F64(0x60);
    ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 0xC;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0x84;
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_0C = (s32) D_8013C4C0.unk_38[0];
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_10 = (s32) D_8013C4C0.unk_70[0];
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_34 = (s32) D_8013C4C0.unk_00[0];
    ((DisplayActor44 *)D_8011ECD0)[96].unk_14 = (s16) D_8013C4C0.unk_A8[0].unk_00;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_06 = 1;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_43 = 0x11;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0x20;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = 0x28;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 1;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 0;
    ((DisplayActor44 *)D_8011ECD0)[97] = ((DisplayActor44 *)D_8011ECD0)[96];
    ((DisplayActor44 *)D_8011ECD0)[97].unk_03 = 4;
    ((DisplayActor44 *)D_8011ECD0)[97].unk_26 = 0x10;
    switch (D_800E6280.unk_F5F) {
    case 0:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[1];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[1];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[1];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C4C0.unk_A8[1].unk_00;
        break;
    case 1:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[2];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[2];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[2];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C4C0.unk_A8[2].unk_00;
        break;
    case 2:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[3];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[3];
        var_t6 = D_8013C4C0.unk_A8[3].unk_00;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[3];
block_17:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = var_t6;
        break;
    case 3:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[4];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[4];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[4];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C4C0.unk_A8[4].unk_00;
        break;
    case 4:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[5];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[5];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[5];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C4C0.unk_A8[5].unk_00;
        break;
    case 5:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[6];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[6];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[6];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C4C0.unk_A8[6].unk_00;
        break;
    case 6:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[7];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[7];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[7];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C4C0.unk_A8[7].unk_00;
        break;
    case 7:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[8];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[8];
        var_t6 = D_8013C4C0.unk_A8[8].unk_00;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[8];
        goto block_17;
    case 8:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[9];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[9];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[9];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C4C0.unk_A8[9].unk_00;
        break;
    case 9:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[0xA];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[0xA];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[0xA];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C4C0.unk_A8[0xA].unk_00;
        break;
    case 10:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[0xB];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[0xB];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[0xB];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C4C0.unk_A8[0xB].unk_00;
        break;
    case 12:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[0xC];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[0xC];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[0xC];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C4C0.unk_A8[0xC].unk_00;
        break;
    default:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4C0.unk_38[0xD];
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C4C0.unk_70[0xD];
        var_t6 = D_8013C4C0.unk_A8[0xD].unk_00;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C0.unk_00[0xD];
        goto block_17;
    }
    D_800E6280.unk_037 = 0;
    D_800E6280.unk_036 = 0;
    D_800E6280.unk_038 = 0;
    func_8004284C();
}
