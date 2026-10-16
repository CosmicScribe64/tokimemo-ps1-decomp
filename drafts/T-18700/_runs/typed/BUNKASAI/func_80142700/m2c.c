extern s8 D_800CA2B1;
extern s8 D_800CA2B2;
extern ? D_800CA2B3;
extern ? D_80122ED0;

void func_80142700(void) {
    ? sp3C;
    ? *var_v1;
    s32 temp_v0;
    u32 temp_t1;
    u32 temp_t4;
    u32 temp_t5;
    u32 temp_t6;
    u8 *var_v0;
    u8 *var_v0_2;

    sp3C.unk0 = (s32) D_80160520.unk0;
    *(s32 *)((unsigned char *)(&sp3C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80160520) + (4));
    *(? *)((unsigned char *)(&sp3C) + (7)) = (unaligned s32) M2C_LWL(D_80160520.v[0xA]);
    func_80083808();
    switch (D_80122EC0) {
    case 0:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        func_8008585C();
        *D_800CA2B0 = (u8) ((u32) (D_800E6280.unk_1BC[0].unk_0C.w << 0x1E) >> 0x1F);
        D_800CA2B1 = (s8) ((u32) (D_800E6280.unk_1BC[1].unk_0C.w << 0x1E) >> 0x1F);
        var_v0 = &D_800E6280.unk_044[1].unk_20[4];
        var_v1 = &D_800CA2B3;
        D_800CA2B2 = (s8) ((u32) (D_800E6280.unk_1BC[2].unk_0C.w << 0x1E) >> 0x1F);
        do {
            temp_t5 = (u32) (*(s32 *)((unsigned char *)var_v0 + (456)) << 0x1E) >> 0x1F;
            temp_t4 = (u32) (*(s32 *)((unsigned char *)var_v0 + (624)) << 0x1E) >> 0x1F;
            temp_t1 = (u32) (*(s32 *)((unsigned char *)var_v0 + (568)) << 0x1E) >> 0x1F;
            temp_t6 = (u32) (*(s32 *)((unsigned char *)var_v0 + (512)) << 0x1E) >> 0x1F;
            var_v0 += 0xE0;
            *(s8 *)((unsigned char *)var_v1 + (1)) = (s8) temp_t6;
            *(s8 *)((unsigned char *)var_v1 + (2)) = (s8) temp_t1;
            *(s8 *)((unsigned char *)var_v1 + (3)) = (s8) temp_t4;
            var_v1 += 4;
            *(s8 *)((unsigned char *)var_v1 + (-4)) = (s8) temp_t5;
        } while (var_v0 != &D_800E6280.unk_1BC[3].unk_04);
        func_800573AC(&D_800E6280.unk_1BC[3].unk_04);
        func_80064E84();
        func_8004E9F4(0);
        func_8006612C((s32) "文化祭");
        func_80065F34(0);
        func_8006764C(0);
        func_800649D4();
        func_80064FA4(0);
        func_80063930(0);
        D_80122EBC = 0xFF;
        D_80122EA4 = 0xFF;
        D_80122E98 = 0;
        D_80122EC0 += 1;
        break;
    case 1:
        func_80057390(0);
        D_80122EC0 += 1;
        break;
    case 2:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            func_8005742C(0x4029, 1);
            D_80122EC0 += 1;
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            func_80065F34(1);
            D_80122EC0 += 1;
        }
        break;
    case 4:
        func_8007E81C();
        func_80142B98();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_80084E4C();
            func_800847B8(0U);
            func_801323A8();
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_801604F0;
            D_800CA164 = (s32) D_801604F4;
            D_800CA168 = (s32) D_801604F8;
            D_80122EC0 += 1;
        }
        break;
    case 6:
        func_80085A60();
        func_80142B98();
        break;
    case 7:
        func_80150930();
        break;
    case 8:
        temp_v0 = D_800E6280.unk_1BC[0].unk_10.w;
        if ((D_800E6280.unk_03F == (temp_v0 & 0xF)) && (D_800E6280.unk_040 == ((u32) (temp_v0 << 0x17) >> 0x1B)) && !(D_800E6280.unk_71E & 1)) {
            D_80122EC0 = 9;
            D_800E6280.unk_1109 = 0;
            D_800E6280.unk_110A = 0;
        } else if (D_800E6280.unk_1109 == 0) {
            func_80044750(0xBF);
            func_800674B0();
            func_80042808();
        } else if (func_80044E8C() == 1) {
            func_80142BE8();
            func_801433C0();
            var_v0_2 = D_80122DD0;
            do {
                var_v0_2 += 0x10;
                *(s32 *)((unsigned char *)var_v0_2 + (-12)) = 0;
                *(s32 *)((unsigned char *)var_v0_2 + (-8)) = 0;
                *(s32 *)((unsigned char *)var_v0_2 + (-4)) = 0;
                *(s32 *)((unsigned char *)var_v0_2 + (-16)) = 0;
            } while (var_v0_2 != &D_80122ED0);
            func_8008585C();
            func_800573AC();
            func_80057390(0);
            func_80072B5C(1);
        }
        break;
    case 9:
        func_80147214();
        break;
    default:
        func_80046500();
        break;
    }
    if (D_80122E98 == 0) {
        func_80086424();
    }
}
