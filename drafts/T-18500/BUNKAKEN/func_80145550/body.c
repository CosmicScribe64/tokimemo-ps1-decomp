void func_80145550(void) {
    GameState *g = &D_800E6280;
    s32 spDA;
    BunkakenBytes9x6 spA4;
    BunkakenBytes11x6 sp60;
    BunkakenBytes11x6 sp1C;
    s32 temp_t8;
    u8 var_v0;
    u8 var_v0_2;

    spA4 = D_80155F9C;
    sp60 = D_80155FD4;
    sp1C = D_80156018;
    temp_t8 = (g->unk_03E - 0x5F) & 0xFF;
    if (D_80122EBC == 0xB) {
        g->unk_1BC[0].unk_06 += 2;
        switch (temp_t8) {
        default:
            var_v0 = spDA;
            break;
        case 0:
            var_v0 = *(spA4.b + D_80122EB8);
            break;
        case 1:
            var_v0 = *(sp60.b + D_80122EB8);
            break;
        case 2:
            var_v0 = *(sp1C.b + D_80122EB8);
            break;
        }
        if (var_v0 != 0) {
            var_v0 = (var_v0 - 1) & 0xFF;
        }
        g->unk_1BC[0].unk_02 = (s16) g->unk_000 + var_v0;
    } else if (D_80122EBC < 0xB) {
        g->unk_1BC[D_80122EBC].unk_06 += 2;
        switch (temp_t8) {
        default:
            var_v0_2 = spDA;
            break;
        case 0:
            var_v0_2 = spA4.b[D_80122EBC][D_80122EB8];
            break;
        case 1:
            var_v0_2 = sp60.b[D_80122EBC][D_80122EB8];
            break;
        case 2:
            var_v0_2 = sp1C.b[D_80122EBC][D_80122EB8];
            break;
        }
        if (var_v0_2 != 0) {
            var_v0_2 = (var_v0_2 - 1) & 0xFF;
        }
        g->unk_1BC[D_80122EBC].unk_02 += var_v0_2;
    }
    if (D_80122EA4 != 0xFF) {
        g->unk_1BC[D_80122EA4].unk_06 -= 0xA;
        g->unk_1BC[D_80122EA4].unk_0A += 0xA;
    }
}
