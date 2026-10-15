? func_800AE100(? *, u8 *);                         /* extern */
extern s32 D_800B3568;
extern ? D_800B3778;
extern u16 D_800E62C2;
extern s32 D_800E66EC;
extern s32 D_800E7CBC;
extern s32 D_800E7CCC;
extern s32 D_800E7CDC;
extern s32 D_800E7CEC;
extern s16 D_801207BC;
extern s16 D_80120800;
extern s16 D_80120888;
extern ? D_80146FD0;

void func_8013D03C(s32 arg0, ? arg1) {
    s32 spCC;
    ? spAC;
    void *sp50;
    s32 temp_s0;
    s32 temp_v0_2;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s1_3;
    s32 var_s1_4;
    s32 var_v1;
    u32 temp_t4_2;
    u8 *var_s0;
    u8 *var_s0_2;
    u8 temp_t0;
    u8 temp_t4;
    u8 temp_t7;
    void *temp_v0;

    if (arg0 == 0xB) {
        spCC = 0xC;
    } else {
        spCC = arg0;
    }
    temp_t4 = D_800E6280.unk_1BC[spCC].unk_0C.b[0] | 8;
    temp_t7 = temp_t4 | 0x40;
    D_800E6280.unk_1BC[spCC].unk_0C.b[0] = temp_t4;
    temp_t0 = temp_t7 | 4;
    D_800E6280.unk_1BC[spCC].unk_0C.b[0] = temp_t7;
    D_800E6280.unk_1BC[spCC].unk_0C.b[0] = temp_t0;
    D_800E6280.unk_1BC[spCC].unk_0C.b[0] = temp_t0 | 0x80;
    D_800E7CBC = 0x801B01B0;
    D_800E7CCC = 0x801B01C0;
    D_800E7CDC = 0;
    D_800E7CEC = 0x801B0200;
    sp50 = (spCC * 0x38) + &D_800E6280;
    var_s1 = 0;
    do {
        func_80043914(D_800E7CEC, (var_s1 + 0x10) & 0xFF, 1, 2, var_s1);
        var_s1 += 1;
    } while (var_s1 < 0x10);
    func_80048E78();
    var_s0 = D_8011ECD0;
    var_s1_2 = 0;
    do {
        func_80048F64(var_s1_2 + 0x60);
        *(s8 *)((unsigned char *)var_s0 + (6529)) = 4;
        *(s8 *)((unsigned char *)var_s0 + (6530)) = 0x40;
        *(s32 *)((unsigned char *)var_s0 + (6584)) = 0x40000000;
        *(s8 *)((unsigned char *)var_s0 + (6531)) = 0x84;
        *(u8 **)((unsigned char *)var_s0 + (6540)) = sprite_frame_table;
        *(u8 **)((unsigned char *)var_s0 + (6544)) = D_800C9A14;
        *(u8 **)((unsigned char *)var_s0 + (6580)) = D_800C9730;
        var_s1_2 += 1;
        var_s0 += 0x44;
        *(s8 *)((unsigned char *)var_s0 + (6464)) = 8;
        *(s8 *)((unsigned char *)var_s0 + (6465)) = 1;
        *(s16 *)((unsigned char *)var_s0 + (6480)) = (s16) D_800C9A54;
    } while (var_s1_2 < 8);
    temp_v0 = (spCC * 7) + &D_800B3778;
    D_80120668 = temp_v0->unk0 + 0x4E;
    D_80120676 = 0x27;
    D_8012067A = -0x5A;
    D_801206AC = *(u8 *)((unsigned char *)temp_v0 + (1)) + 0x4E;
    D_801206BA = 0x31;
    D_801206BE = -0x5A;
    D_801206F0 = *(u8 *)((unsigned char *)temp_v0 + (2)) + 0x4E;
    D_801206FE = 0x3B;
    D_80120702 = -0x5A;
    D_80120734 = *(u8 *)((unsigned char *)temp_v0 + (3)) + 0x4E;
    D_80120742 = 0x4C;
    D_80120746 = -0x5A;
    D_80120778 = *(u8 *)((unsigned char *)temp_v0 + (4)) + 0x4E;
    D_80120786 = 0x56;
    D_8012078A = -0x5A;
    D_801207BC = *(u8 *)((unsigned char *)temp_v0 + (5)) + 0x4E;
    D_801207CA = 0x60;
    D_801207CE = -0x5A;
    D_80120800 = *(u8 *)((unsigned char *)temp_v0 + (6)) + 0x4E;
    D_8012080E = 0x6A;
    D_80120812 = -0x5A;
    D_80120844 = 0x41;
    D_80120852 = -0x10;
    D_80120856 = -0x1B;
    var_s0_2 = D_8011EEF0;
    var_s1_3 = 8;
    do {
        func_80048F64(var_s1_3 + 0x60);
        *(s8 *)((unsigned char *)var_s0_2 + (6529)) = 4;
        *(s8 *)((unsigned char *)var_s0_2 + (6530)) = 0x40;
        *(s32 *)((unsigned char *)var_s0_2 + (6584)) = 0x01000000;
        *(s8 *)((unsigned char *)var_s0_2 + (6531)) = 0x84;
        *(s32 *)((unsigned char *)var_s0_2 + (6540)) = (s32) D_800E7CCC;
        *(s32 *)((unsigned char *)var_s0_2 + (6544)) = (s32) D_800E7CDC;
        var_s1_3 += 1;
        var_s0_2 += 0x44;
        *(s16 *)((unsigned char *)var_s0_2 + (6480)) = 0;
        *(s8 *)((unsigned char *)var_s0_2 + (6464)) = 8;
        *(s8 *)((unsigned char *)var_s0_2 + (6465)) = 0x80;
        *(s32 *)((unsigned char *)var_s0_2 + (6512)) = (s32) D_800E7CBC;
    } while (var_s1_3 != 9);
    D_80120888 = (s16) arg0;
    D_80120896 = 0x40;
    D_8012089A = -0x28;
    if (arg0 == 0xB) {
        func_8004E788(-0x80, -0x62, 3, D_800B3568, 0);
    } else {
        func_8004E788(-0x80, -0x62, 3, D_800B3538[arg0], 0);
    }
    func_8004E788(-0x20, -0x62, 5, (s32) "電話番号", 0);
    func_8004E788(-0x8A, -0x4E, 4, (s32) "誕生日", 0);
    func_800AE0F0(&spAC, *(void **)((unsigned char *)(&D_800B32CC[*(s32 *)((unsigned char *)sp50 + (460)) & 0xF]) + (-4)));
    func_800AE100(&spAC, "月");
    temp_t4_2 = (u32) (*(s32 *)((unsigned char *)sp50 + (460)) << 0x17) >> 0x1B;
    if (temp_t4_2 >= 0xAU) {
        func_800AE100(&spAC, D_800B3288[(s32) temp_t4_2 / 10]);
    } else {
        func_800AE100(&spAC, D_800B3644);
    }
    func_800AE100(&spAC, D_800B3288[(s32) ((u32) (*(s32 *)((unsigned char *)sp50 + (460)) << 0x17) >> 0x1B) % 10]);
    func_800AE100(&spAC, "日");
    func_8004E788(-0x58, -0x4E, 3, (s32) &spAC, 0);
    func_8004E788(-0x8A, -0x1E, 4, (s32) "ＢＷＨ", 0);
    if ((u16) D_800E62C2 >= 0x2DAU) {
        var_s1_4 = 2;
    } else {
        var_s1_4 = 0;
        if ((u16) D_800E62C2 >= 0x16DU) {
            var_s1_4 = 1;
        }
    }
    func_8004E788(-0x58, -0x1E, 3, (((arg0 * 3) + var_s1_4) * 0x12) + 0x801B2334, 0);
    func_8004E788(-0x8A, -0x3E, 4, (s32) "星座", 0);
    M2C_MEMCPY_ALIGNED(&sp58[0], &D_80146FD0, 0x40);
    temp_v0_2 = *(s32 *)((unsigned char *)sp50 + (460));
    var_v1 = (temp_v0_2 & 0xF) - 1;
    if ((u32) (&sp58[0])[var_v1] < (u32) ((u32) (temp_v0_2 << 0x17) >> 0x1B)) {
        var_v1 += 1;
    }
    func_8004E788(-0x58, -0x3E, 3, ((var_v1 % 12) * 0xA) + 0x801B22BC, 0);
    func_8004E788(-0x8A, -0x2E, 4, (s32) "血液型", 0);
    if (arg0 == 0xB) {
        func_8004E788(-0x58, -0x2E, 3, (((u32) (D_800E66EC << 0x15) >> 0x1E) * 8) + 0x801B229C, 0);
    } else {
        func_8004E788(-0x58, -0x2E, 3, (((u32) (*(s32 *)((unsigned char *)sp50 + (460)) << 0x15) >> 0x1E) * 8) + 0x801B229C, 0);
    }
    func_8004E788(-0x8A, -0xE, 4, (s32) "クラブ", 0);
    if (arg0 == 0xB) {
        func_8004E788(-0x58, -0xE, 3, (s32) "私設部", 0);
    } else {
        func_8004E788(-0x58, -0xE, 3, (((u8) *(u8 *)((unsigned char *)sp50 + (458)) >> 4) * 0xC) + 0x801B2200, 0);
    }
    func_8004E788(-0x8A, 2, 4, (s32) "備考", 0);
    temp_s0 = (((arg0 * 6) + (var_s1_4 * 2)) << 5) + 0x801B25F4;
    func_8004E788(-0x58, 2, 3, temp_s0, 0);
    func_8004E788(-0x58, 0x12, 3, temp_s0 + 0x20, 0);
}
