s32 func_80055AFC(s32 arg0) {
    typedef struct { s32 w[30]; } M2cBlock78; /* m2c memcpy of 120 bytes */
    typedef struct { s32 w[13]; } M2cBlock34; /* m2c memcpy of 52 bytes */
    void *sp30;
    void *sp2C;
    s32 temp_at;
    s32 temp_at_2;
    GameState *var_t8;
    u8 *var_v0;
    GameState *var_v0_3;
    u8 *var_v0_6;
    u8 *var_v0_7;
    GameState *var_v0_8;
    GameState *var_v0_9;
    GsRec66C *var_v0_5;
    GsRec75E *var_t0;
    GsRec75E *var_t1;
    GsRec75E *var_t2;
    GsRec75E *var_v1_10;
    GsWord *var_v0_4;
    Rec38 *var_v0_2;
    s32 *var_v1_4;
    s32 *var_v1_5;
    s32 temp_t6;
    s32 temp_t6_2;
    u8 *temp_t8;
    u8 *var_t8_2;
    u8 temp_t6_3;
    u8 temp_t7;
    u8 temp_t8_2;
    u8 temp_t9;
    u8 temp_t9_2;
    u8 *temp_ra;
    u8 *temp_t5;
    u8 *var_a0;
    u8 *var_a1;
    u8 *var_a2;
    u8 *var_a3;
    u8 *var_t6;
    u8 *var_t9;
    u8 *var_v1;
    u8 *var_v1_2;
    u8 *var_v1_3;
    u8 *var_v1_6;
    u8 *var_v1_7;
    u8 *var_v1_8;
    u8 *var_v1_9;

    if (func_80055A38(0) == -1) {
        return -1;
    }
    temp_t6 = D_800E6280.unk0 & 1;
    temp_t5 = (temp_t6 * 0xF00) + D_80123120;
    if (*(u8 *)((unsigned char *)temp_t5 + (517)) == 0) {
        return -1;
    }
    if (*(u8 *)((unsigned char *)temp_t5 + (518)) == 0) {
        return -1;
    }
    var_v0 = (u8 *)&D_800E6280;
    D_800E6280.unk0 = (u8) *(u8 *)((unsigned char *)temp_t5 + (516));
    D_800E6280.unk_03E = *(u8 *)((unsigned char *)temp_t5 + (517));
    D_800E6280.unk_03F = *(u8 *)((unsigned char *)temp_t5 + (518));
    D_800E6280.unk_040 = *(u8 *)((unsigned char *)temp_t5 + (519));
    D_800E6280.unk_041 = *(u8 *)((unsigned char *)temp_t5 + (520));
    temp_ra = (temp_t6 * 0xF00) + D_80123120;
    var_v1 = temp_ra + 0x20C;
    D_800E6280.unk_042 = *(u16 *)((unsigned char *)temp_t5 + (522));
    do {
        var_t6 = var_v1;
        var_t8 = var_v0;
loop_8:
        temp_at = (*(s32 *)var_t6);
        var_t6 += 0xC;
        *var_t8 = temp_at;
        var_t8 += 0xC;
        *(s32 *)((unsigned char *)var_t8 + (-8)) = *(s32 *)((unsigned char *)var_t6 + (-8));
        *(s32 *)((unsigned char *)var_t8 + (-4)) = *(s32 *)((unsigned char *)var_t6 + (-4));
        if (var_t6 != (var_v1 + 0x3C)) {
            goto loop_8;
        }
        var_v0 += 0x40;
        var_t8->unk0 = (*(s32 *)var_t6);
        var_v1 += 0x40;
    } while ((u32) var_v0 < (u32) D_800E6280.unk_0C4);
    *(M2cBlock78 *)(D_800E6280.unk_0C4) = *(M2cBlock78 *)(temp_t5 + 0x28C);
    var_v0_2 = D_800E6280.unk_1BC;
    var_v1_2 = temp_ra + 0x308;
    *(s32 *)((unsigned char *)D_800E6280.unk_0C4 + (120)) = (s32) *(s32 *)((unsigned char *)(temp_t5 + 0x78) + (652));
    do {
        *(M2cBlock34 *)(var_v0_2) = *(M2cBlock34 *)(var_v1_2);
        temp_t8 = &var_v0_2->unk_14[0x1C];
        var_v0_2 += 1;
        temp_t6_2 = *(s32 *)((unsigned char *)(var_v1_2 + 0x30) + (4));
        var_v1_2 += 0x38;
        *(s32 *)((unsigned char *)temp_t8 + (4)) = temp_t6_2;
    } while ((u32) var_v0_2 < (u32) D_800E6280.unk_53C);
    var_v0_3 = &D_800E6280;
    var_v1_3 = temp_ra;
    do {
        temp_t9 = *(u8 *)((unsigned char *)var_v1_3 + (1672));
        var_v0_3 += 1;
        var_v1_3 += 1;
        var_v0_3->unk_1BC[0xF].unk_14[0x23] = temp_t9;
    } while ((u32) var_v0_3 < (u32) &D_800E6280.unk_006[0xA]);
    var_v0_4 = D_800E6280.unk_54C;
    var_v1_4 = temp_ra + 0x698;
    do {
        var_v0_4 += 4;
        *(s32 *)((unsigned char *)var_v0_4 + (-4)) = (s32) *var_v1_4;
        var_v1_4 += 1;
    } while ((u32) var_v0_4 < (u32) D_800E6280.unk_56C);
    var_v0_5 = D_800E6280.unk_66C;
    var_v1_5 = temp_ra + 0x6B8;
    do {
        var_v0_5 += 1;
        *(s32 *)((unsigned char *)var_v0_5 + (-4)) = (s32) *var_v1_5;
        var_v1_5 += 1;
    } while ((u32) var_v0_5 < (u32) D_800E6280.unk_69C);
    var_t8_2 = D_800E6280.unk_56C;
    var_t9 = temp_t5;
    do {
        temp_at_2 = *(s32 *)((unsigned char *)var_t9 + (1768));
        var_t9 += 0xC;
        *var_t8_2 = temp_at_2;
        var_t8_2 += 0xC;
        *(s32 *)((unsigned char *)var_t8_2 + (-8)) = *(s32 *)((unsigned char *)var_t9 + (1760));
        *(s32 *)((unsigned char *)var_t8_2 + (-4)) = *(s32 *)((unsigned char *)var_t9 + (1764));
    } while (var_t9 != (temp_t5 + 0xFC));
    (*(s32 *)var_t8_2) = *(s32 *)((unsigned char *)var_t9 + (1768));
    sp2C = temp_ra;
    sp30 = temp_t5;
    k_speed_set(D_800E6280.unk_668[3]);
    var_v0_6 = (u8 *)&D_800E6280;
    var_v1_6 = temp_ra;
    D_800B6708 = D_800E6280.unk_668[2];
    do {
        temp_t6_3 = *(u8 *)((unsigned char *)var_v1_6 + (2024));
        var_v0_6 += 1;
        var_v1_6 += 1;
        *(u8 *)(var_v0_6 + 0x727) = temp_t6_3;
    } while ((u32) var_v0_6 < (u32) &D_800E6280.unk_004);
    var_v0_7 = (u8 *)&D_800E6280;
    var_v1_7 = temp_ra;
    do {
        temp_t9_2 = *(u8 *)((unsigned char *)var_v1_7 + (2028));
        var_v0_7 += 1;
        var_v1_7 += 1;
        *(u8 *)(var_v0_7 + 0x72B) = temp_t9_2;
    } while ((u32) var_v0_7 < (u32) &D_800E6280.unk_006[0xA]);
    var_v0_8 = &D_800E6280;
    var_v1_8 = temp_ra;
    do {
        temp_t8_2 = *(u8 *)((unsigned char *)var_v1_8 + (2044));
        var_v0_8 += 1;
        var_v1_8 += 1;
        var_v0_8->unk_72C[0xF] = temp_t8_2;
    } while ((u32) var_v0_8 < (u32) &D_800E6280.unk_006[0xA]);
    var_v0_9 = &D_800E6280;
    var_v1_9 = temp_ra;
    do {
        temp_t7 = *(u8 *)((unsigned char *)var_v1_9 + (2060));
        var_v0_9 += 1;
        var_v1_9 += 1;
        var_v0_9->unk_73C[0xF] = temp_t7;
    } while ((u32) var_v0_9 < (u32) &D_800E6280.unk_006[0xA]);
    sp2C = temp_ra;
    sp30 = temp_t5;
    bzero(D_800E6280.unk_75E, 0x800);
    var_v1_10 = &D_800E6280.unk_75E[3];
    var_t2 = &D_800E6280.unk_75E[2];
    var_t1 = &D_800E6280.unk_75E[1];
    var_t0 = D_800E6280.unk_75E;
    var_a0 = temp_ra + 0x81C;
    var_a1 = temp_ra + 0x824;
    var_a2 = temp_ra + 0x82C;
    var_a3 = temp_ra + 0x834;
    do {
        var_t0->unk_00 = (*(s32 *)var_a0);
        var_t0->unk_04 = *(s32 *)((unsigned char *)var_a0 + (4));
        var_t1->unk_00 = (*(s32 *)var_a1);
        var_t1->unk_04 = *(s32 *)((unsigned char *)var_a1 + (4));
        var_t2->unk_00 = (*(s32 *)var_a2);
        var_t2->unk_04 = *(s32 *)((unsigned char *)var_a2 + (4));
        var_v1_10 += 4;
        *(s32 *)((unsigned char *)var_v1_10 + (-28)) = *(s32 *)((unsigned char *)var_a3 + (4));
        *(s32 *)((unsigned char *)var_v1_10 + (-32)) = (*(s32 *)var_a3);
        var_a0 += 0x20;
        var_a1 += 0x20;
        var_a2 += 0x20;
        var_a3 += 0x20;
        var_t0 += 4;
        var_t1 += 4;
        var_t2 += 4;
    } while (var_v1_10 != &D_800E6280.unk_F76);
    sp30 = temp_t5;
    D_800E6280.unk_F5E = func_8006C988((s32) &D_800E6280.unk_F5F, (s32) &D_800E6280.unk_75D, 1, (s32) var_a3);
    D_800E6280.unk_F6C = (s8) *(u8 *)((unsigned char *)temp_t5 + (4124));
    D_800E6280.unk_F6D = (s8) *(u8 *)((unsigned char *)temp_t5 + (4125));
    D_800E6280.unk_1092 = *(u8 *)((unsigned char *)temp_t5 + (4126));
    D_800E6280.unk_F6E = (s8) *(u8 *)((unsigned char *)temp_t5 + (4127));
    D_800E6280.unk_03C = (s8) *(u8 *)((unsigned char *)temp_t5 + (4128));
    D_800E6280.unk_F68.w = *(s32 *)((unsigned char *)temp_t5 + (4132));
    D_800E6280.unk_F6F = *(u8 *)((unsigned char *)temp_t5 + (4136));
    D_800E6280.unk_F70 = (s8) *(u8 *)((unsigned char *)temp_t5 + (4137));
    D_800E6280.unk_F73 = (s8) *(u8 *)((unsigned char *)temp_t5 + (4138));
    D_800E6280.unk_F75 = *(u8 *)((unsigned char *)temp_t5 + (4139));
    D_800E6280.unk_F64[0] = (s32) *(s32 *)((unsigned char *)temp_t5 + (4140));
    return 1;
}
