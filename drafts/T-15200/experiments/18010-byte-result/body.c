void func_80118010(void) {
    u8 temp_v0;
    s16 sp32;
    EventStateView *g;
    u32 temp_t2;
    u32 temp_t8;

    g = (EventStateView *)D_800B0860;
    temp_v0 = func_8002328C(g->unk_EE6);
    if (D_800B1AF9++ == 0) {
        temp_t8 = temp_v0 & 0x7F;
        D_80124354 = ((temp_t8 >= 2U) + (temp_t8 >= 3U) + (temp_t8 >= 4U)) * 3;
    }
    sp32 = ((s16 *)&D_801243A4)[D_800EECBC] + D_80124354;
    if (D_800B0F45 != 0) {
        sp32 = 0xA;
    }
    if ((D_800B0F45 != 0) && ((u32) (temp_v0 & 0x7F) < 2U) && (g->unk_EE6 == 6)) {
        sp32 = 0x12;
    } else if ((D_800EECE4 >= 0) && (D_800EECE4 == *(u8 *)((unsigned char *)(&D_800B0860[g->unk_EE6]) + (1749))) && (D_800B1AF4 == 0x50) && ((u8) D_80125310 == 0)) {
        sp32 = 0xB;
        if ((D_800B0F45 != 0) && ((u32) (temp_v0 & 0x7F) < 2U) && (g->unk_EE6 == 6)) {
            sp32 = 0x13;
        }
        if (D_800B1AF9 == 1) {
            func_8004A8EC(3);
            g->unk_1A4[g->unk_EE6].unk_02 -= 1;
            g->unk_1A4[g->unk_EE6].unk_06 -= 2;
            g->unk_1A4[g->unk_EE6].unk_0A += 5;
            func_8004C250(0x34);
            func_80033E88();
        }
    }
    D_8009471C = (u8 *) &sp32;
    D_80094720 = &D_80124358;
    temp_t2 = func_8002328C(g->unk_EE6) & 0x7F;
    if ((g->unk_EE6 == 1) && (temp_t2 < 2U) && (*(s16 *)&D_80124358 == 3)) {
        func_8004A8EC(1);
        func_80033E88();
    }
    D_80094724 = D_80124258;
    D_80094728 = D_801242C4;
    D_8009472C = D_80124330;
    func_80049B20(D_800EECBC, 1, 0);
}
