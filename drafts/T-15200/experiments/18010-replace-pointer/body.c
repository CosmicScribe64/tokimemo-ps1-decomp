void func_80118010(void) {
    u8 *r;
    s16 sp32;
    u8 temp_v0;

    temp_v0 = func_8002328C(((EventStateView *)D_800B0860)->unk_EE6);
    if (((EventMovieState *)&D_800B1AF0)->unk_09++ == 0) {
        D_80124354 = (((temp_v0 & 0x7F) >= 2U) + ((temp_v0 & 0x7F) >= 3U) + ((temp_v0 & 0x7F) >= 4U)) * 3;
    }
    sp32 = ((s16 *)&D_801243A4)[D_800EECBC] + D_80124354;
    if (((EventStateView *)D_800B0860)->pad_6CB[0x1A] != 0) {
        sp32 = 0xA;
    }
    if ((((EventStateView *)D_800B0860)->pad_6CB[0x1A] != 0) && ((u32) (temp_v0 & 0x7F) < 2U) && (((EventStateView *)D_800B0860)->unk_EE6 == 6)) {
        sp32 = 0x12;
    } else if ((D_800EECE4 >= 0) && (D_800EECE4 == ((EventStateView *)D_800B0860)->pad_6CB[((EventStateView *)D_800B0860)->unk_EE6 + 0xA]) && (((EventMovieState *)&D_800B1AF0)->pad04 == 0x50) && ((u8) D_80125310 == 0)) {
        sp32 = 0xB;
        if ((((EventStateView *)D_800B0860)->pad_6CB[0x1A] != 0) && ((u32) (temp_v0 & 0x7F) < 2U) && (((EventStateView *)D_800B0860)->unk_EE6 == 6)) {
            sp32 = 0x13;
        }
        if (((EventMovieState *)&D_800B1AF0)->unk_09 == 1) {

            func_8004A8EC(3);
            r = D_800B0860 + ((EventStateView *)D_800B0860)->unk_EE6 * 0x34;
            *(s16 *)(r + 0x1A6) -= 1;
            r = D_800B0860 + ((EventStateView *)D_800B0860)->unk_EE6 * 0x34;
            *(s16 *)(r + 0x1AA) -= 2;
            r = D_800B0860 + ((EventStateView *)D_800B0860)->unk_EE6 * 0x34;
            *(s16 *)(r + 0x1AE) += 5;
            func_8004C250(sizeof(Rec34));
            func_80033E88();
        }
    }
    D_8009471C = (u8 *) &sp32;
    D_80094720 = &D_80124358;
    temp_v0 = func_8002328C(((EventStateView *)D_800B0860)->unk_EE6);
    if ((((EventStateView *)D_800B0860)->unk_EE6 == 1) && ((temp_v0 & 0x7F) < 2U) && (*(s16 *)&D_80124358 == 3)) {
        func_8004A8EC(1);
        func_80033E88();
    }
    D_80094724 = D_80124258;
    D_80094728 = D_801242C4;
    D_8009472C = D_80124330;
    func_80049B20(D_800EECBC, 1, 0);
}
