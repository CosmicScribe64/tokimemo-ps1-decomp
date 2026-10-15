extern ? D_800E6284;
extern ? D_800E6290;
extern u8 D_800E62BC;
extern u8 D_800E62BD;
extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E62C1;
extern u16 D_800E62C2;
extern ? D_800E62C4;
extern ? D_800E6344;
extern ? D_800E643C;
extern ? D_800E67BC;
extern ? D_800E67CC;
extern ? D_800E67EC;
extern u8 D_800E68EA;
extern u8 D_800E68EB;
extern ? D_800E68EC;
extern ? D_800E691C;
extern ? D_800E69DD;
extern ? D_800E69DE;
extern ? D_800E69E6;
extern ? D_800E69EE;
extern ? D_800E69F6;
extern s8 D_800E71DE;
extern ? D_800E71DF;
extern s32 D_800E71E4;
extern s32 D_800E71E8;
extern u8 D_800E71EC;
extern u8 D_800E71ED;
extern u8 D_800E71EE;
extern u8 D_800E71EF;
extern u8 D_800E71F0;
extern u8 D_800E71F3;
extern u8 D_800E71F5;
extern ? D_800E71F6;
extern u8 D_800E7312;
extern u8 D_800E7397;

s32 func_80055AFC(s32 arg0) {
    void *sp30;
    void *sp2C;
    ? *var_t0;
    ? *var_t1;
    ? *var_t2;
    ? *var_t8;
    ? *var_t8_2;
    ? *var_v0;
    ? *var_v0_2;
    ? *var_v0_4;
    ? *var_v0_5;
    ? *var_v1_10;
    ? temp_at;
    ? temp_at_2;
    GameState *var_v0_3;
    GameState *var_v0_6;
    GameState *var_v0_7;
    GameState *var_v0_8;
    GameState *var_v0_9;
    s32 *var_v1_4;
    s32 *var_v1_5;
    s32 temp_t6;
    s32 temp_t6_2;
    u8 temp_t6_3;
    u8 temp_t7;
    u8 temp_t8_2;
    u8 temp_t9;
    u8 temp_t9_2;
    void *temp_ra;
    void *temp_t5;
    void *temp_t8;
    void *var_a0;
    void *var_a1;
    void *var_a2;
    void *var_a3;
    void *var_t6;
    void *var_t9;
    void *var_v1;
    void *var_v1_2;
    void *var_v1_3;
    void *var_v1_6;
    void *var_v1_7;
    void *var_v1_8;
    void *var_v1_9;

    if (func_80055A38(0) == -1) {
        return -1;
    }
    temp_t6 = D_800E7397 & 1;
    temp_t5 = (temp_t6 * 0xF00) + D_80123120;
    if (*(u8 *)((unsigned char *)temp_t5 + (517)) == 0) {
        return -1;
    }
    if (*(u8 *)((unsigned char *)temp_t5 + (518)) == 0) {
        return -1;
    }
    var_v0 = &D_800E62C4;
    D_800E62BD = *(u8 *)((unsigned char *)temp_t5 + (516));
    D_800E62BE = *(u8 *)((unsigned char *)temp_t5 + (517));
    D_800E62BF = *(u8 *)((unsigned char *)temp_t5 + (518));
    D_800E62C0 = *(u8 *)((unsigned char *)temp_t5 + (519));
    D_800E62C1 = *(u8 *)((unsigned char *)temp_t5 + (520));
    temp_ra = (temp_t6 * 0xF00) + D_80123120;
    var_v1 = temp_ra + 0x20C;
    D_800E62C2 = *(u16 *)((unsigned char *)temp_t5 + (522));
    do {
        var_t6 = var_v1;
        var_t8 = var_v0;
loop_8:
        temp_at = (unaligned s32) var_t6->unk0;
        var_t6 += 0xC;
        *var_t8 = (unaligned s32) temp_at;
        var_t8 += 0xC;
        *(? *)((unsigned char *)var_t8 + (-8)) = (unaligned s32) *(? *)((unsigned char *)var_t6 + (-8));
        *(? *)((unsigned char *)var_t8 + (-4)) = (unaligned s32) *(? *)((unsigned char *)var_t6 + (-4));
        if (var_t6 != (var_v1 + 0x3C)) {
            goto loop_8;
        }
        var_v0 += 0x40;
        var_t8->unk0 = (unaligned s32) var_t6->unk0;
        var_v1 += 0x40;
    } while ((u32) var_v0 < (u32) &D_800E6344);
    M2C_MEMCPY_ALIGNED(&D_800E6344, temp_t5 + 0x28C, 0x78);
    var_v0_2 = &D_800E643C;
    var_v1_2 = temp_ra + 0x308;
    *(s32 *)((unsigned char *)(&D_800E6344) + (120)) = (s32) *(s32 *)((unsigned char *)(temp_t5 + 0x78) + (652));
    do {
        M2C_MEMCPY_ALIGNED(var_v0_2, var_v1_2, 0x34);
        temp_t8 = var_v0_2 + 0x30;
        var_v0_2 += 0x38;
        temp_t6_2 = *(s32 *)((unsigned char *)(var_v1_2 + 0x30) + (4));
        var_v1_2 += 0x38;
        *(s32 *)((unsigned char *)temp_t8 + (4)) = temp_t6_2;
    } while ((u32) var_v0_2 < (u32) &D_800E67BC);
    var_v0_3 = &D_800E6280;
    var_v1_3 = temp_ra;
    do {
        temp_t9 = *(u8 *)((unsigned char *)var_v1_3 + (1672));
        var_v0_3 += 1;
        var_v1_3 += 1;
        var_v0_3->unk_1BC[0xF].unk_14[0x23] = temp_t9;
    } while ((u32) var_v0_3 < (u32) &D_800E6290);
    var_v0_4 = &D_800E67CC;
    var_v1_4 = temp_ra + 0x698;
    do {
        var_v0_4 += 4;
        *(s32 *)((unsigned char *)var_v0_4 + (-4)) = (s32) *var_v1_4;
        var_v1_4 += 4;
    } while ((u32) var_v0_4 < (u32) &D_800E67EC);
    var_v0_5 = &D_800E68EC;
    var_v1_5 = temp_ra + 0x6B8;
    do {
        var_v0_5 += 4;
        *(s32 *)((unsigned char *)var_v0_5 + (-4)) = (s32) *var_v1_5;
        var_v1_5 += 4;
    } while ((u32) var_v0_5 < (u32) &D_800E691C);
    var_t8_2 = &D_800E67EC;
    var_t9 = temp_t5;
    do {
        temp_at_2 = (unaligned s32) *(? *)((unsigned char *)var_t9 + (1768));
        var_t9 += 0xC;
        *var_t8_2 = (unaligned s32) temp_at_2;
        var_t8_2 += 0xC;
        *(? *)((unsigned char *)var_t8_2 + (-8)) = (unaligned s32) *(? *)((unsigned char *)var_t9 + (1760));
        *(? *)((unsigned char *)var_t8_2 + (-4)) = (unaligned s32) *(? *)((unsigned char *)var_t9 + (1764));
    } while (var_t9 != (temp_t5 + 0xFC));
    var_t8_2->unk0 = (unaligned s32) *(? *)((unsigned char *)var_t9 + (1768));
    sp2C = temp_ra;
    sp30 = temp_t5;
    k_speed_set(D_800E68EB);
    var_v0_6 = &D_800E6280;
    var_v1_6 = temp_ra;
    D_800B6708 = D_800E68EA;
    do {
        temp_t6_3 = *(u8 *)((unsigned char *)var_v1_6 + (2024));
        var_v0_6 += 1;
        var_v1_6 += 1;
        var_v0_6->unk_727 = temp_t6_3;
    } while ((u32) var_v0_6 < (u32) &D_800E6284);
    var_v0_7 = &D_800E6280;
    var_v1_7 = temp_ra;
    do {
        temp_t9_2 = *(u8 *)((unsigned char *)var_v1_7 + (2028));
        var_v0_7 += 1;
        var_v1_7 += 1;
        var_v0_7->unk_728[3] = temp_t9_2;
    } while ((u32) var_v0_7 < (u32) &D_800E6290);
    var_v0_8 = &D_800E6280;
    var_v1_8 = temp_ra;
    do {
        temp_t8_2 = *(u8 *)((unsigned char *)var_v1_8 + (2044));
        var_v0_8 += 1;
        var_v1_8 += 1;
        var_v0_8->unk_72C[0xF] = temp_t8_2;
    } while ((u32) var_v0_8 < (u32) &D_800E6290);
    var_v0_9 = &D_800E6280;
    var_v1_9 = temp_ra;
    do {
        temp_t7 = *(u8 *)((unsigned char *)var_v1_9 + (2060));
        var_v0_9 += 1;
        var_v1_9 += 1;
        var_v0_9->unk_73C[0xF] = temp_t7;
    } while ((u32) var_v0_9 < (u32) &D_800E6290);
    sp2C = temp_ra;
    sp30 = temp_t5;
    bzero(&D_800E69DE, 0x800);
    var_v1_10 = &D_800E69F6;
    var_t2 = &D_800E69EE;
    var_t1 = &D_800E69E6;
    var_t0 = &D_800E69DE;
    var_a0 = temp_ra + 0x81C;
    var_a1 = temp_ra + 0x824;
    var_a2 = temp_ra + 0x82C;
    var_a3 = temp_ra + 0x834;
    do {
        var_t0->unk0 = (unaligned s32) var_a0->unk0;
        *(? *)((unsigned char *)var_t0 + (4)) = (unaligned s32) *(? *)((unsigned char *)var_a0 + (4));
        var_t1->unk0 = (unaligned s32) var_a1->unk0;
        *(? *)((unsigned char *)var_t1 + (4)) = (unaligned s32) *(? *)((unsigned char *)var_a1 + (4));
        var_t2->unk0 = (unaligned s32) var_a2->unk0;
        *(? *)((unsigned char *)var_t2 + (4)) = (unaligned s32) *(? *)((unsigned char *)var_a2 + (4));
        var_v1_10 += 0x20;
        *(? *)((unsigned char *)var_v1_10 + (-28)) = (unaligned s32) *(? *)((unsigned char *)var_a3 + (4));
        *(? *)((unsigned char *)var_v1_10 + (-32)) = (unaligned s32) var_a3->unk0;
        var_a0 += 0x20;
        var_a1 += 0x20;
        var_a2 += 0x20;
        var_a3 += 0x20;
        var_t0 += 0x20;
        var_t1 += 0x20;
        var_t2 += 0x20;
    } while (var_v1_10 != &D_800E71F6);
    sp30 = temp_t5;
    D_800E71DE = func_8006C988((s32) &D_800E71DF, (s32) &D_800E69DD, 1, (s32) var_a3);
    D_800E71EC = *(u8 *)((unsigned char *)temp_t5 + (4124));
    D_800E71ED = *(u8 *)((unsigned char *)temp_t5 + (4125));
    D_800E7312 = *(u8 *)((unsigned char *)temp_t5 + (4126));
    D_800E71EE = *(u8 *)((unsigned char *)temp_t5 + (4127));
    D_800E62BC = *(u8 *)((unsigned char *)temp_t5 + (4128));
    D_800E71E8 = *(s32 *)((unsigned char *)temp_t5 + (4132));
    D_800E71EF = *(u8 *)((unsigned char *)temp_t5 + (4136));
    D_800E71F0 = *(u8 *)((unsigned char *)temp_t5 + (4137));
    D_800E71F3 = *(u8 *)((unsigned char *)temp_t5 + (4138));
    D_800E71F5 = *(u8 *)((unsigned char *)temp_t5 + (4139));
    D_800E71E4 = *(s32 *)((unsigned char *)temp_t5 + (4140));
    return 1;
}
