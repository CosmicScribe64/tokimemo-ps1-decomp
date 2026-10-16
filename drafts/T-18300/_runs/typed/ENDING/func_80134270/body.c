void func_80134270(void) {
    typedef struct { s32 w[17]; } M2cBlock44; /* m2c memcpy of 68 bytes */
    Rec24 *temp_v0;
    s16 *var_at;
    s16 temp_v1;
    s16 var_t6;
    s16 var_v1;
    s32 temp_at;
    u8 temp_t9;
    u8 *temp_t7;

    func_8009C5E0(1);
    func_80133C10();
    load_palette(D_8013C5A0, 0x10, 1, 2, 0);
    load_palette(D_8013C5A0, 0x11, 1, 2, 1);
    LoadSquare(0x140U, 0x80U, 0x40U, 0x80U, D_8013C5A4);
    LoadSquare(0x180U, 0x80U, 0x40U, 0x80U, D_8013C5A8);
    LoadSquare(0x1C0U, 0x80U, 0x40U, 0x80U, D_8013C5AC);
    LoadSquare(0x200U, 0x80U, 0x40U, 0x80U, D_8013C5B0);
    LoadSquare(0x240U, 0x80U, 0x40U, 0x80U, D_8013C5B4);
    LoadSquare(0x280U, 0x80U, 0x40U, 0x80U, D_8013C5B8);
    D_800E6280.unk_1228[0x25] = 1;
    D_800E6280.unk_1228[0x26] = 1;
    D_800E6280.unk_1228[0x27] = 1;
    D_800E6280.unk_1228[0x28] = 1;
    D_800E6280.unk_1228[0x29] = 1;
    D_800E6280.unk_1228[0x2A] = 1;
    var_v1 = 0;
    do {
        temp_v0 = &D_801217D0[var_v1];
        *(s16 *)((unsigned char *)temp_v0 + (156)) = (s16) (var_v1 + 5);
        *(s32 *)((unsigned char *)temp_v0 + (144)) = 0x41000000;
        *(s16 *)((unsigned char *)temp_v0 + (152)) = 0x80;
        *(s16 *)((unsigned char *)temp_v0 + (154)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0 + (158)) = 0;
        *(s8 *)((unsigned char *)temp_v0 + (159)) = 0x80;
        *(s16 *)((unsigned char *)temp_v0 + (160)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (162)) = 0x1F0;
        *(s8 *)((unsigned char *)temp_v0 + (164)) = 0;
        *(s8 *)((unsigned char *)temp_v0 + (165)) = 0;
        *(s8 *)((unsigned char *)temp_v0 + (166)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (168)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (170)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (172)) = 0x1000;
        *(s16 *)((unsigned char *)temp_v0 + (174)) = 0x1000;
        *(s32 *)((unsigned char *)temp_v0 + (176)) = 0;
        temp_t7 = (u8 *)&D_800E6280 + var_v1;
        temp_v1 = var_v1 + 1;
        *(s16 *)((unsigned char *)temp_v0 + (148)) = (s16) (((var_v1 % 3) << 7) - 0x80);
        temp_at = temp_v1 < 6;
        *(s16 *)((unsigned char *)temp_v0 + (150)) = (s16) (((var_v1 / 3) << 7) - 0x78);
        var_v1 = temp_v1;
        *(s8 *)((unsigned char *)temp_t7 + (4265)) = 0xB;
    } while (temp_at != 0);
    func_80048F64(0x60);
    ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 0xC;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0x84;
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_0C = (s32) D_8013C4F8;
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_10 = (s32) D_8013C530;
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_34 = (s32) D_8013C4C0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_14 = (s16) D_8013C568;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_06 = 1;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_43 = 0x11;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0x20;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = 0x28;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 1;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 0;
    *(M2cBlock44 *)(&((DisplayActor44 *)D_8011ECD0)[97]) = *(M2cBlock44 *)(&((DisplayActor44 *)D_8011ECD0)[96]);
    ((DisplayActor44 *)D_8011ECD0)[97].unk_03 = 4;
    temp_t9 = D_800E6280.unk_F5F;
    ((DisplayActor44 *)D_8011ECD0)[97].unk_26 = 0x10;
    switch (temp_t9) {
    case 0:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C4FC;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C534;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C4;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C56C;
        break;
    case 1:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C500;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C538;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4C8;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C570;
        break;
    case 2:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C504;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C53C;
        var_t6 = D_8013C574;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4CC;
        var_at = (DisplayActor44 *)(D_8011ECD0 + 0x19D8);
block_17:
        *var_at = var_t6;
        break;
    case 3:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C508;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C540;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4D0;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C578;
        break;
    case 4:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C50C;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C544;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4D4;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C57C;
        break;
    case 5:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C510;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C548;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4D8;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C580;
        break;
    case 6:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C514;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C54C;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4DC;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C584;
        break;
    case 7:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C518;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C550;
        var_t6 = D_8013C588;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4E0;
        var_at = (DisplayActor44 *)(D_8011ECD0 + 0x19D8);
        goto block_17;
    case 8:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C51C;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C554;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4E4;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C58C;
        break;
    case 9:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C520;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C558;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4E8;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C590;
        break;
    case 10:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C524;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C55C;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4EC;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C594;
        break;
    case 12:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C528;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C560;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4F0;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_8013C598;
        break;
    default:
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_8013C52C;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_8013C564;
        var_t6 = D_8013C59C;
        *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_8013C4F4;
        var_at = (DisplayActor44 *)(D_8011ECD0 + 0x19D8);
        goto block_17;
    }
    D_800E6280.unk_037 = 0;
    D_800E6280.unk_036 = 0;
    D_800E6280.unk_038 = 0;
    func_8004284C();
}
