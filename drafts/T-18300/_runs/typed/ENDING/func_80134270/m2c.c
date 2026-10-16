extern s16 D_8013C568;
extern s16 D_8013C56C;
extern s16 D_8013C570;
extern s16 D_8013C574;
extern s16 D_8013C578;
extern s16 D_8013C57C;
extern s16 D_8013C580;
extern s16 D_8013C584;
extern s16 D_8013C588;
extern s16 D_8013C58C;
extern s16 D_8013C590;
extern s16 D_8013C594;
extern s16 D_8013C598;
extern s16 D_8013C59C;

void func_80134270(void) {
    Rec24 *temp_v0;
    s16 *var_at;
    s16 temp_v1;
    s16 var_t6;
    s16 var_v1;
    s32 temp_at;
    u8 temp_t9;
    void *temp_t7;

    func_8009C5E0(1);
    func_80133C10();
    func_80043914(D_8013C5A0, 0x10, 1, 2, 0);
    func_80043914(D_8013C5A0, 0x11, 1, 2, 1);
    func_8004435C(0x140U, 0x80U, 0x40U, 0x80U, D_8013C5A4);
    func_8004435C(0x180U, 0x80U, 0x40U, 0x80U, D_8013C5A8);
    func_8004435C(0x1C0U, 0x80U, 0x40U, 0x80U, D_8013C5AC);
    func_8004435C(0x200U, 0x80U, 0x40U, 0x80U, D_8013C5B0);
    func_8004435C(0x240U, 0x80U, 0x40U, 0x80U, D_8013C5B4);
    func_8004435C(0x280U, 0x80U, 0x40U, 0x80U, D_8013C5B8);
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
        temp_t7 = &D_800E6280 + var_v1;
        temp_v1 = var_v1 + 1;
        *(s16 *)((unsigned char *)temp_v0 + (148)) = (s16) (((var_v1 % 3) << 7) - 0x80);
        temp_at = temp_v1 < 6;
        *(s16 *)((unsigned char *)temp_v0 + (150)) = (s16) (((var_v1 / 3) << 7) - 0x78);
        var_v1 = temp_v1;
        *(s8 *)((unsigned char *)temp_t7 + (4265)) = 0xB;
    } while (temp_at != 0);
    func_80048F64(0x60);
    *(s8 *)((unsigned char *)D_8011ECD0 + (6529)) = 0xC;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6530)) = 0;
    *(s32 *)((unsigned char *)D_8011ECD0 + (6584)) = 0;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6531)) = 0x84;
    *(s32 *)((unsigned char *)D_8011ECD0 + (6540)) = (s32) D_8013C4F8;
    *(s32 *)((unsigned char *)D_8011ECD0 + (6544)) = (s32) D_8013C530;
    *(s32 *)((unsigned char *)D_8011ECD0 + (6580)) = (s32) D_8013C4C0;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6548)) = (s16) D_8013C568;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6550)) = 0;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6552)) = 0;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6534)) = 1;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6595)) = 0x11;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6566)) = -0x20;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6570)) = 0x28;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6535)) = 0;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6533)) = 1;
    *(s8 *)((unsigned char *)D_8011ECD0 + (6532)) = 0;
    M2C_MEMCPY_ALIGNED(D_8011ECD0 + 0x19C4, D_8011ECD0 + 0x1980, 0x44);
    *(s8 *)((unsigned char *)D_8011ECD0 + (6599)) = 4;
    temp_t9 = D_800E6280.unk_F5F;
    *(s16 *)((unsigned char *)D_8011ECD0 + (6634)) = 0x10;
    switch (temp_t9) {
    case 0:
        *(s32 *)((unsigned char *)D_8011ECD0 + (6608)) = (s32) D_8013C4FC;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6612)) = (s32) D_8013C534;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6648)) = (s32) D_8013C4C4;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6616)) = (s16) D_8013C56C;
        break;
    case 1:
        *(s32 *)((unsigned char *)D_8011ECD0 + (6608)) = (s32) D_8013C500;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6612)) = (s32) D_8013C538;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6648)) = (s32) D_8013C4C8;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6616)) = (s16) D_8013C570;
        break;
    case 2:
        *(s32 *)((unsigned char *)D_8011ECD0 + (6608)) = (s32) D_8013C504;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6612)) = (s32) D_8013C53C;
        var_t6 = D_8013C574;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6648)) = (s32) D_8013C4CC;
        var_at = D_8011ECD0 + 0x19D8;
block_17:
        *var_at = var_t6;
        break;
    case 3:
        *(s32 *)((unsigned char *)D_8011ECD0 + (6608)) = (s32) D_8013C508;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6612)) = (s32) D_8013C540;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6648)) = (s32) D_8013C4D0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6616)) = (s16) D_8013C578;
        break;
    case 4:
        *(s32 *)((unsigned char *)D_8011ECD0 + (6608)) = (s32) D_8013C50C;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6612)) = (s32) D_8013C544;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6648)) = (s32) D_8013C4D4;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6616)) = (s16) D_8013C57C;
        break;
    case 5:
        *(s32 *)((unsigned char *)D_8011ECD0 + (6608)) = (s32) D_8013C510;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6612)) = (s32) D_8013C548;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6648)) = (s32) D_8013C4D8;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6616)) = (s16) D_8013C580;
        break;
    case 6:
        *(s32 *)((unsigned char *)D_8011ECD0 + (6608)) = (s32) D_8013C514;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6612)) = (s32) D_8013C54C;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6648)) = (s32) D_8013C4DC;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6616)) = (s16) D_8013C584;
        break;
    case 7:
        *(s32 *)((unsigned char *)D_8011ECD0 + (6608)) = (s32) D_8013C518;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6612)) = (s32) D_8013C550;
        var_t6 = D_8013C588;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6648)) = (s32) D_8013C4E0;
        var_at = D_8011ECD0 + 0x19D8;
        goto block_17;
    case 8:
        *(s32 *)((unsigned char *)D_8011ECD0 + (6608)) = (s32) D_8013C51C;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6612)) = (s32) D_8013C554;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6648)) = (s32) D_8013C4E4;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6616)) = (s16) D_8013C58C;
        break;
    case 9:
        *(s32 *)((unsigned char *)D_8011ECD0 + (6608)) = (s32) D_8013C520;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6612)) = (s32) D_8013C558;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6648)) = (s32) D_8013C4E8;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6616)) = (s16) D_8013C590;
        break;
    case 10:
        *(s32 *)((unsigned char *)D_8011ECD0 + (6608)) = (s32) D_8013C524;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6612)) = (s32) D_8013C55C;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6648)) = (s32) D_8013C4EC;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6616)) = (s16) D_8013C594;
        break;
    case 12:
        *(s32 *)((unsigned char *)D_8011ECD0 + (6608)) = (s32) D_8013C528;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6612)) = (s32) D_8013C560;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6648)) = (s32) D_8013C4F0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6616)) = (s16) D_8013C598;
        break;
    default:
        *(s32 *)((unsigned char *)D_8011ECD0 + (6608)) = (s32) D_8013C52C;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6612)) = (s32) D_8013C564;
        var_t6 = D_8013C59C;
        *(s32 *)((unsigned char *)D_8011ECD0 + (6648)) = (s32) D_8013C4F4;
        var_at = D_8011ECD0 + 0x19D8;
        goto block_17;
    }
    D_800E6280.unk_037 = 0;
    D_800E6280.unk_036 = 0;
    D_800E6280.unk_038 = 0;
    func_8004284C();
}
