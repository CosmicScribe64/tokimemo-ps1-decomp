/* NON_MATCHING: T-12020, 4 differing listing rows (2 positions); f03 byte store and f0C clear schedule in reverse order. */
void func_80136BB4(void) {
    u16 limit;
    TaiikuU11 limits;
    u8 temp;

    limits = D_801497DC;
    if (D_80122E94 == 1) {
        limit = limits.v[D_801491DC];
    } else {
        limit = limits.v[((DisplayActor44 *)D_8011ECD0)[46 + D_80149208.pad00[2]].unk_16 >> 3];
    }
    if ((limit < D_80149208.f0E) && (D_80149208.f18 != 0)) {
        if (D_80149208.pad04[0] >= 5U) {
            D_80149208.pad04[0] = 0;
        } else {
            D_80149208.pad04[0]++;
        }
        D_80149208.f0E = 0;
    }
    if (((D_80149208.f0C >= 6U) && (D_80149208.f10 > 0)) || ((D_80149208.f0C >= 8U) && (((s32) D_801491E8 >> 1) < D_80149208.f18)) || ((D_80149208.f0C >= 11U) && (D_80149208.f18 != 0))) {
        temp = D_80149208.f03;
        if (temp >= 5U) {
            D_80149208.f03 = 0;
        } else {
            D_80149208.f03 = D_80149208.f03 + 1;
        }
        D_80149208.f0C = 0;
    }
    ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = *(s16 *)(D_801492AC + D_80149208.f03 * 2);
    if ((D_8014929C == 0) && (D_80149208.f10 <= 0)) {
        D_80149208.f03 = 5;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0x90;
    }
}
