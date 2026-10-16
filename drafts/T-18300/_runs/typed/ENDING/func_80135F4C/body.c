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
    u8 temp_t3_2;
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
    load_palette(D_8013C604, 0x11, 1, 2, 0);
    LoadSquare(0x140U, 0U, 0x40U, 0x80U, D_8013C608);
    LoadSquare(0x180U, 0U, 0x40U, 0x80U, D_8013C60C);
    LoadSquare(0x1C0U, 0U, 0x40U, 0x80U, D_8013C610);
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
    load_palette(D_8013C600, 0x12, 1, 3, 0);
    var_s1_2 = 0;
    do {
        func_80048F64(var_s1_2 + 0x60);
        temp_s0 = &((DisplayActor44 *)D_8011ECD0)[var_s1_2];
        temp_s0[96].unk_01 = 0xC;
        temp_s0[96].unk_02 = 1;
        temp_s0[96].unk_38 = 0x51000000;
        temp_s0[96].unk_03 = 0x84;
        *(s32 *)&temp_s0[96].unk_0C = (s32) D_8013C5F4;
        *(s32 *)&temp_s0[96].unk_10 = (s32) D_8013C5F8;
        *(s32 *)&temp_s0[96].unk_34 = (s32) D_8013C5F0;
        temp_s0[96].unk_14 = (s16) D_8013C5FC;
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
    load_palette(D_8013C630, 0x13, 1, 2, 0);
    LoadSquare(0x140U, 0x80 & 0xFFFF, 0x40U, 0x80 & 0xFFFF, D_8013C634);
    LoadSquare(0x180U, 0x80 & 0xFFFF, 0x40U, 0x80 & 0xFFFF, D_8013C638);
    LoadSquare(0x1C0U, 0x80 & 0xFFFF, 0x40U, 0x80 & 0xFFFF, D_8013C644);
    LoadSquare(0x200U, 0x80 & 0xFFFF, 0x40U, 0x80 & 0xFFFF, D_8013C648);
    LoadSquare(0x240U, 0x80 & 0xFFFF, 0x40U, 0x80 & 0xFFFF, D_8013C64C);
    LoadSquare(0x280U, 0x80 & 0xFFFF, 0x40U, 0x80 & 0xFFFF, D_8013C650);
    LoadSquare(0x140U, 0x100U, 0x40U, 0x80 & 0xFFFF, D_8013C654);
    LoadSquare(0x180U, 0x100U, 0x40U, 0x80 & 0xFFFF, D_8013C658);
    LoadSquare(0x1C0U, 0x100U, 0x40U, 0x80 & 0xFFFF, D_8013C65C);
    LoadSquare(0x200U, 0x100U, 0x40U, 0x80 & 0xFFFF, D_8013C660);
    LoadSquare(0x240U, 0x100U, 0x40U, 0x80 & 0xFFFF, D_8013C63C);
    LoadSquare(0x280U, 0x100U, 0x40U, 0x80 & 0xFFFF, D_8013C640);
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
        *(s8 *)((unsigned char *)((u8 *)&D_800E6280 + var_s1_3) + (4265)) = 0xB;
        temp_at_2 = temp_s1 < 0xC;
        temp_t4 = ((var_s1_3 % 3) << 7) - 0xA0;
        temp_t7 = ((var_s1_3 / 3) << 7) - 0x188;
        var_s1_3 = temp_s1;
        *(s16 *)((unsigned char *)temp_v0_2 + (148)) = temp_t4;
        *(s16 *)((unsigned char *)temp_v0_2 + (150)) = temp_t7;
    } while (temp_at_2 != 0);
    func_80132000();
    temp_t3_2 = D_800E6280.unk_F5F;
    switch (temp_t3_2) {
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
        if (!(D_800E6280.unk_1BC[1].unk_0C.w & 0x4000)) {
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
        if (!(D_800E6280.unk_53C[6] & 1)) {
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
    func_80046290(0, 0, (s32) D_800E6280.unk_F5F);
    func_80044750(0x300);
    func_8004284C();
}
