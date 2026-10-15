void func_80134270(void) {
    typedef struct { s32 w[17]; } M2cBlock44; /* m2c memcpy of 68 bytes */
    Rec24 *temp_v0;
    s16 temp_v1;
    s16 var_t6;
    s16 var_v1;
    s32 temp_at;
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
    D_800E753C = 1;
    D_800E7540 = 1;
    D_800E7544 = 1;
    D_800E7548 = 1;
    D_800E754C = 1;
    D_800E7550 = 1;
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
        temp_t7 = &D_800E6280 + var_v1;
        temp_v1 = var_v1 + 1;
        *(s16 *)((unsigned char *)temp_v0 + (148)) = (s16) (((var_v1 % 3) << 7) - 0x80);
        temp_at = temp_v1 < 6;
        *(s16 *)((unsigned char *)temp_v0 + (150)) = (s16) (((var_v1 / 3) << 7) - 0x78);
        var_v1 = temp_v1;
        *(s8 *)((unsigned char *)temp_t7 + (4265)) = 0xB;
    } while (temp_at != 0);
    func_80048F64(0x60);
    D_80120651 = 0xC;
    D_80120652 = 0;
    D_80120688 = 0;
    D_80120653 = 0x84;
    D_8012065C = (u8 *) D_8013C4F8;
    D_80120660 = (u8 *) D_8013C530;
    D_80120684 = (u8 *) D_8013C4C0;
    D_80120664 = D_8013C568;
    D_80120666 = 0;
    D_80120668 = 0;
    D_80120656 = 1;
    D_80120693 = 0x11;
    D_80120676 = -0x20;
    D_8012067A = 0x28;
    D_80120657 = 0;
    D_80120655 = 1;
    D_80120654 = 0;
    *(M2cBlock44 *)(D_80120694) = *(M2cBlock44 *)(D_80120650);
    D_80120697 = 4;
    D_801206BA = 0x10;
    switch (D_800E71DF) {
    case 0:
        D_801206A0 = D_8013C4FC;
        D_801206A4 = D_8013C534;
        D_801206C8 = D_8013C4C4;
        D_801206A8 = D_8013C56C;
        break;
    case 1:
        D_801206A0 = D_8013C500;
        D_801206A4 = D_8013C538;
        D_801206C8 = D_8013C4C8;
        D_801206A8 = D_8013C570;
        break;
    case 2:
        D_801206A0 = D_8013C504;
        D_801206A4 = D_8013C53C;
        var_t6 = D_8013C574;
        D_801206C8 = D_8013C4CC;
block_17:
        D_801206A8 = var_t6;
        break;
    case 3:
        D_801206A0 = D_8013C508;
        D_801206A4 = D_8013C540;
        D_801206C8 = D_8013C4D0;
        D_801206A8 = D_8013C578;
        break;
    case 4:
        D_801206A0 = D_8013C50C;
        D_801206A4 = D_8013C544;
        D_801206C8 = D_8013C4D4;
        D_801206A8 = D_8013C57C;
        break;
    case 5:
        D_801206A0 = D_8013C510;
        D_801206A4 = D_8013C548;
        D_801206C8 = D_8013C4D8;
        D_801206A8 = D_8013C580;
        break;
    case 6:
        D_801206A0 = D_8013C514;
        D_801206A4 = D_8013C54C;
        D_801206C8 = D_8013C4DC;
        D_801206A8 = D_8013C584;
        break;
    case 7:
        D_801206A0 = D_8013C518;
        D_801206A4 = D_8013C550;
        var_t6 = D_8013C588;
        D_801206C8 = D_8013C4E0;
        goto block_17;
    case 8:
        D_801206A0 = D_8013C51C;
        D_801206A4 = D_8013C554;
        D_801206C8 = D_8013C4E4;
        D_801206A8 = D_8013C58C;
        break;
    case 9:
        D_801206A0 = D_8013C520;
        D_801206A4 = D_8013C558;
        D_801206C8 = D_8013C4E8;
        D_801206A8 = D_8013C590;
        break;
    case 10:
        D_801206A0 = D_8013C524;
        D_801206A4 = D_8013C55C;
        D_801206C8 = D_8013C4EC;
        D_801206A8 = D_8013C594;
        break;
    case 12:
        D_801206A0 = D_8013C528;
        D_801206A4 = D_8013C560;
        D_801206C8 = D_8013C4F0;
        D_801206A8 = D_8013C598;
        break;
    default:
        D_801206A0 = D_8013C52C;
        D_801206A4 = D_8013C564;
        var_t6 = D_8013C59C;
        D_801206C8 = D_8013C4F4;
        goto block_17;
    }
    D_800E62B7 = 0;
    D_800E62B6 = 0;
    D_800E62B8 = 0;
    func_8004284C();
}
