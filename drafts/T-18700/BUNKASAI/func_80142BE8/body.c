void func_80142BE8(void) {
    s32 i;
    u32 bonus;
    u8 amount;
    BunkasaiColumns sel;
    BunkasaiGirlColumns first;
    BunkasaiGirlColumns second;
    BunkasaiGirlColumns third;
    u32 col;

    sel = D_8016052C;
    first = D_80160534;
    second = D_80160584;
    third = D_801605D4;
    col = D_800E6280.unk_0F4.h >> 12;
    bonus = ((&D_800E6280.unk_0FC)[sel.v[col]].unk_02 / 20 +
             (u32) (D_800E6280.unk_66C[col].unk_00 >> 3) / (u32) (D_800E6280.unk_03E - 94)) & 0xFF;
    if (bonus >= 11) {
        bonus = 10;
    }
    switch ((D_800E6280.unk_03E - 95) & 0xFF) {
    case 0:
        D_800E6280.unk_1BC[0].unk_02 += bonus + (u8)(first.v[0][col] >> 1);
        for (i = 1; i < 13; i++) {
            D_800E6280.unk_1BC[i].unk_02 += bonus;
            D_800E6280.unk_1BC[i].unk_02 += (u8)(first.v[i][D_800E6280.unk_0F4.h >> 12] >> 1);
        }
        amount = first.v[D_80122EBC][D_800E6280.unk_0F4.h >> 12];
        break;
    case 1:
        D_800E6280.unk_1BC[0].unk_02 += bonus + (u8)(second.v[0][col] >> 1);
        for (i = 1; i < 13; i++) {
            D_800E6280.unk_1BC[i].unk_02 += bonus;
            D_800E6280.unk_1BC[i].unk_02 += (u8)(second.v[i][D_800E6280.unk_0F4.h >> 12] >> 1);
        }
        amount = second.v[D_80122EBC][D_800E6280.unk_0F4.h >> 12];
        break;
    case 2:
        D_800E6280.unk_1BC[0].unk_02 += bonus + (u8)(third.v[0][col] >> 1);
        for (i = 1; i < 13; i++) {
            D_800E6280.unk_1BC[i].unk_02 += bonus;
            D_800E6280.unk_1BC[i].unk_02 += (u8)(third.v[i][D_800E6280.unk_0F4.h >> 12] >> 1);
        }
        amount = third.v[D_80122EBC][D_800E6280.unk_0F4.h >> 12];
        break;
    }
    if (amount != 0) {
        amount--;
    }
    D_800E6280.unk_1BC[D_80122EBC].unk_06 += 2;
    D_800E6280.unk_1BC[D_80122EBC].unk_02 += amount;
}
