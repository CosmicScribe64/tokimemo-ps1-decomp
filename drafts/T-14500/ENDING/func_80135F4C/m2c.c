? func_800482FC();                                  /* extern */
extern s32 D_800E6480;
extern u8 D_800E67C2;
extern u8 D_800E71DF;
extern s32 D_800E74BC;
extern s32 D_800E74C0;
extern s32 D_800E74C4;
extern s32 D_800E74FC;
extern s32 D_800E7500;
extern s32 D_800E7504;
extern s32 D_800E7508;
extern s32 D_800E750C;
extern s32 D_800E7510;
extern s32 D_800E7518;
extern s32 D_800E751C;
extern s32 D_800E7520;
extern s32 D_800E753C;
extern s32 D_800E7540;
extern s32 D_800E7544;
extern s32 D_800E7548;
extern s32 D_800E754C;
extern s32 D_800E7550;
extern ? D_8013CA60;
extern ? D_8013CA7C;

void func_80135F4C(void) {
    ? sp7C;
    ? sp60;
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
    void *temp_s0;
    void *temp_t3;
    void *temp_v0;

    sp7C.unk0 = (s32) D_8013CA60.unk0;
    *(s32 *)((unsigned char *)(&sp7C) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8013CA60) + (8));
    *(s32 *)((unsigned char *)(&sp7C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8013CA60) + (4));
    *(s32 *)((unsigned char *)(&sp7C) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8013CA60) + (12));
    *(s32 *)((unsigned char *)(&sp7C) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8013CA60) + (16));
    *(s32 *)((unsigned char *)(&sp7C) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_8013CA60) + (24));
    *(s32 *)((unsigned char *)(&sp7C) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_8013CA60) + (20));
    sp60.unk0 = (s32) D_8013CA7C.unk0;
    *(s32 *)((unsigned char *)(&sp60) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8013CA7C) + (8));
    *(s32 *)((unsigned char *)(&sp60) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8013CA7C) + (4));
    *(s32 *)((unsigned char *)(&sp60) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8013CA7C) + (16));
    *(s32 *)((unsigned char *)(&sp60) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8013CA7C) + (12));
    *(s32 *)((unsigned char *)(&sp60) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_8013CA7C) + (24));
    *(s32 *)((unsigned char *)(&sp60) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_8013CA7C) + (20));
    D_8013C358 = 0;
    D_8013C35C = 0;
    func_800847B8(D_800E71DF);
    func_80134D20();
    func_800482FC();
    D_800E7518 = 1;
    D_800E751C = 1;
    D_800E7520 = 1;
    D_800E74BC = 1;
    D_800E74C0 = 1;
    D_800E74C4 = 1;
    func_80043914(D_8013C604, 0x11, 1, 2, 0);
    func_8004435C(0x140U, 0U, 0x40U, 0x80U, D_8013C608);
    func_8004435C(0x180U, 0U, 0x40U, 0x80U, D_8013C60C);
    func_8004435C(0x1C0U, 0U, 0x40U, 0x80U, D_8013C610);
    var_s1 = 0;
    do {
        temp_lo = var_s1 * 0x24;
        temp_t2 = var_s1 + 5;
        temp_t9 = var_s1 << 7;
        temp_t3 = &D_800E6280 + var_s1;
        var_s1 += 1;
        temp_v0 = D_801217D0 + temp_lo;
        *(s32 *)((unsigned char *)temp_v0 + (576)) = 0x01000000;
        *(s16 *)((unsigned char *)temp_v0 + (584)) = 0x80;
        *(s16 *)((unsigned char *)temp_v0 + (586)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0 + (590)) = 0;
        *(s8 *)((unsigned char *)temp_v0 + (591)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (592)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (588)) = temp_t2;
        *(s16 *)((unsigned char *)temp_v0 + (594)) = 0x1F1;
        *(s8 *)((unsigned char *)temp_v0 + (596)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0 + (597)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0 + (598)) = 0x80;
        *(s16 *)((unsigned char *)temp_v0 + (600)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (602)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (604)) = 0x1000;
        *(s16 *)((unsigned char *)temp_v0 + (606)) = 0x1000;
        *(s32 *)((unsigned char *)temp_v0 + (608)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (580)) = (s16) (temp_t9 - 0xA0);
        *(s16 *)((unsigned char *)temp_v0 + (582)) = -0xBC;
        *(s8 *)((unsigned char *)temp_t3 + (4277)) = 0xD;
    } while (var_s1 < 3);
    func_80043914(D_8013C600, 0x12, 1, 3, 0);
    var_s1_2 = 0;
    do {
        func_80048F64(var_s1_2 + 0x60);
        temp_s0 = (var_s1_2 * 0x44) + D_8011ECD0;
        *(s8 *)((unsigned char *)temp_s0 + (6529)) = 0xC;
        *(s8 *)((unsigned char *)temp_s0 + (6530)) = 1;
        *(s32 *)((unsigned char *)temp_s0 + (6584)) = 0x51000000;
        *(s8 *)((unsigned char *)temp_s0 + (6531)) = 0x84;
        *(s32 *)((unsigned char *)temp_s0 + (6540)) = (s32) D_8013C5F4;
        *(s32 *)((unsigned char *)temp_s0 + (6544)) = (s32) D_8013C5F8;
        *(s32 *)((unsigned char *)temp_s0 + (6580)) = (s32) D_8013C5F0;
        *(s16 *)((unsigned char *)temp_s0 + (6548)) = (s16) D_8013C5FC;
        *(s16 *)((unsigned char *)temp_s0 + (6550)) = (s16) (func_800AE0D0() % 5);
        temp_hi = func_800AE0D0() % 7;
        temp_s2 = var_s1_2 * 2;
        *(s8 *)((unsigned char *)temp_s0 + (6534)) = 1;
        *(s8 *)((unsigned char *)temp_s0 + (6595)) = 0x12;
        *(s16 *)((unsigned char *)temp_s0 + (6552)) = (s16) temp_hi;
        *(s16 *)((unsigned char *)temp_s0 + (6566)) = (s16) (*(s16 *)((unsigned char *)(sp + temp_s2) + (124)) - 0xA0);
        *(s8 *)((unsigned char *)temp_s0 + (6535)) = 0x40;
        *(s16 *)((unsigned char *)temp_s0 + (6570)) = (s16) (*(s16 *)((unsigned char *)(sp + temp_s2) + (96)) - 0xBC);
        *(s8 *)((unsigned char *)temp_s0 + (6533)) = 0;
        *(s8 *)((unsigned char *)temp_s0 + (6532)) = 8;
        temp_at = &D_8013CA1C[var_s1_2];
        var_s1_2 += 1;
        *temp_at = (s16) (func_800AE0D0() % 60);
    } while (var_s1_2 < 0xE);
    func_80134DE0();
    func_80043914(D_8013C630, 0x13, 1, 2, 0);
    func_8004435C(0x140U, 0x80 & 0xFFFF, 0x40U, 0x80 & 0xFFFF, D_8013C634);
    func_8004435C(0x180U, 0x80 & 0xFFFF, 0x40U, 0x80 & 0xFFFF, D_8013C638);
    func_8004435C(0x1C0U, 0x80 & 0xFFFF, 0x40U, 0x80 & 0xFFFF, D_8013C644);
    func_8004435C(0x200U, 0x80 & 0xFFFF, 0x40U, 0x80 & 0xFFFF, D_8013C648);
    func_8004435C(0x240U, 0x80 & 0xFFFF, 0x40U, 0x80 & 0xFFFF, D_8013C64C);
    func_8004435C(0x280U, 0x80 & 0xFFFF, 0x40U, 0x80 & 0xFFFF, D_8013C650);
    func_8004435C(0x140U, 0x100U, 0x40U, 0x80 & 0xFFFF, D_8013C654);
    func_8004435C(0x180U, 0x100U, 0x40U, 0x80 & 0xFFFF, D_8013C658);
    func_8004435C(0x1C0U, 0x100U, 0x40U, 0x80 & 0xFFFF, D_8013C65C);
    func_8004435C(0x200U, 0x100U, 0x40U, 0x80 & 0xFFFF, D_8013C660);
    func_8004435C(0x240U, 0x100U, 0x40U, 0x80 & 0xFFFF, D_8013C63C);
    func_8004435C(0x280U, 0x100U, 0x40U, 0x80 & 0xFFFF, D_8013C640);
    D_800E753C = 1;
    D_800E7540 = 1;
    D_800E7544 = 1;
    D_800E7548 = 1;
    D_800E754C = 1;
    D_800E7550 = 1;
    D_800E74FC = 1;
    D_800E7500 = 1;
    D_800E7504 = 1;
    D_800E7508 = 1;
    D_800E750C = 1;
    D_800E7510 = 1;
    var_s1_3 = 0;
    do {
        temp_v0_2 = &D_801217D0[var_s1_3];
        *(s32 *)((unsigned char *)temp_v0_2 + (144)) = 0x41000000;
        temp_lo_2 = var_s1_3 / 6;
        *(s16 *)((unsigned char *)temp_v0_2 + (152)) = 0x80;
        *(s16 *)((unsigned char *)temp_v0_2 + (154)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0_2 + (158)) = 0;
        *(s16 *)((unsigned char *)temp_v0_2 + (160)) = 0;
        *(s16 *)((unsigned char *)temp_v0_2 + (162)) = 0x1F3;
        *(s8 *)((unsigned char *)temp_v0_2 + (164)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0_2 + (165)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0_2 + (166)) = 0x80;
        *(s16 *)((unsigned char *)temp_v0_2 + (168)) = 0;
        *(s16 *)((unsigned char *)temp_v0_2 + (170)) = 0;
        *(s16 *)((unsigned char *)temp_v0_2 + (172)) = 0x1000;
        *(s16 *)((unsigned char *)temp_v0_2 + (174)) = 0x1000;
        *(s32 *)((unsigned char *)temp_v0_2 + (176)) = 0;
        *(s8 *)((unsigned char *)temp_v0_2 + (159)) = (s8) (0x80 - (temp_lo_2 << 7));
        *(s16 *)((unsigned char *)temp_v0_2 + (156)) = (s16) ((var_s1_3 % 6) + (temp_lo_2 * 0x10) + 5);
        temp_s1 = var_s1_3 + 1;
        *(s8 *)((unsigned char *)(&D_800E6280 + var_s1_3) + (4265)) = 0xB;
        temp_at_2 = temp_s1 < 0xC;
        temp_t4 = ((var_s1_3 % 3) << 7) - 0xA0;
        temp_t7 = ((var_s1_3 / 3) << 7) - 0x188;
        var_s1_3 = temp_s1;
        *(s16 *)((unsigned char *)temp_v0_2 + (148)) = temp_t4;
        *(s16 *)((unsigned char *)temp_v0_2 + (150)) = temp_t7;
    } while (temp_at_2 != 0);
    func_80132000();
    switch (D_800E71DF) {
    case 0:
        D_8013C34C = D_8013C288;
        D_8013C350 = D_8013C2CC;
        D_8013C354 = D_8013C310;
        func_80137160(D_8013C620, D_8013C624, D_8013C628, (s32) D_8013C62C);
        break;
    case 1:
        D_8013C34C = D_8013C2A0;
        D_8013C350 = D_8013C2E4;
        D_8013C354 = D_8013C328;
        if (!(D_800E6480 & 0x4000)) {
            func_80137160(D_8013C664, D_8013C668, D_8013C66C, (s32) D_8013C670);
        } else {
            func_80137160(D_8013C994, D_8013C998, D_8013C99C, (s32) D_8013C9A0);
            D_8013C358 = 1;
        }
        break;
    case 2:
        D_8013C34C = D_8013C290;
        D_8013C350 = D_8013C2D4;
        D_8013C354 = D_8013C318;
        func_80137160(D_8013C6A8, D_8013C6AC, D_8013C6B0, (s32) D_8013C6B4);
        break;
    case 3:
        D_8013C34C = D_8013C298;
        D_8013C350 = D_8013C2DC;
        D_8013C354 = D_8013C320;
        func_80137160(D_8013C6EC, D_8013C6F0, D_8013C6F4, (s32) D_8013C6F8);
        break;
    case 4:
        D_8013C34C = D_8013C2B0;
        D_8013C350 = D_8013C2F4;
        D_8013C354 = D_8013C338;
        func_80137160(D_8013C730, D_8013C734, D_8013C738, (s32) D_8013C73C);
        break;
    case 5:
        D_8013C34C = D_8013C2A4;
        D_8013C350 = D_8013C2E8;
        D_8013C354 = D_8013C32C;
        func_80137160(D_8013C774, D_8013C778, D_8013C77C, (s32) D_8013C780);
        break;
    case 6:
        D_8013C34C = D_8013C2A8;
        D_8013C350 = D_8013C2EC;
        D_8013C354 = D_8013C330;
        if (!(D_800E67C2 & 1)) {
            func_80137160(D_8013C7B8, D_8013C7BC, D_8013C7C0, (s32) D_8013C7C4);
        } else {
            func_80137160(D_8013C9D8, D_8013C9DC, D_8013C9E0, (s32) D_8013C9E4);
            D_8013C358 = 1;
        }
        break;
    case 7:
        D_8013C34C = D_8013C29C;
        D_8013C350 = D_8013C2E0;
        D_8013C354 = D_8013C324;
        func_80137160(D_8013C7FC, D_8013C800, D_8013C804, (s32) D_8013C808);
        break;
    case 8:
        D_8013C34C = D_8013C284;
        D_8013C350 = D_8013C2C8;
        D_8013C354 = D_8013C30C;
        func_80137160(D_8013C840, D_8013C844, D_8013C848, (s32) D_8013C84C);
        break;
    case 9:
        D_8013C34C = D_8013C2AC;
        D_8013C350 = D_8013C2F0;
        D_8013C354 = D_8013C334;
        func_80137160(D_8013C884, D_8013C888, D_8013C88C, (s32) D_8013C890);
        break;
    case 10:
        D_8013C34C = D_8013C2B8;
        D_8013C350 = D_8013C2FC;
        D_8013C354 = D_8013C340;
        func_80137160(D_8013C8C8, D_8013C8CC, D_8013C8D0, (s32) D_8013C8D4);
        break;
    case 11:
        D_8013C34C = D_8013C2C0;
        D_8013C350 = D_8013C304;
        D_8013C354 = D_8013C348;
        func_80137160(D_8013C90C, D_8013C910, D_8013C914, (s32) D_8013C918);
        break;
    default:
        D_8013C34C = D_8013C294;
        D_8013C350 = D_8013C2D8;
        D_8013C354 = D_8013C31C;
        func_80137160(D_8013C950, D_8013C954, D_8013C958, (s32) D_8013C95C);
        break;
    }
    D_8013CA38 = 1;
    func_80046290(0, 0, (s32) D_800E71DF);
    func_80044750(0x300);
    func_8004284C();
}
