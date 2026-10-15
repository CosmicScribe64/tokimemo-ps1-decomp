/* NON_MATCHING: T-10720, instruction shape and stack layout match; only register assignment differs (the two local table copies take a3/t1 and the temp v takes a2 where the original has a2/t0 and reuses a0), about 50 lines */
void func_8013C780(void) {
    TaiikuU11 a;
    TaiikuS11 b;
    s32 v;

    a = D_80149F50;
    b = D_80149F68;
    if (a.v[D_80149940] < D_801499A0.unk_00 && (D_80149940 != 2 || (D_80149940 == 2 && D_80149960 > 0) || D_80149990.unk_00 != 0)) {
        if (D_8014999C >= 3 && D_80149990.unk_00 != 0 && (D_80149950 == 2 || D_80149950 == 4 || D_80149950 == 6)) {
            v = 0;
        } else if (D_8014999C >= 5) {
            v = 0;
        } else {
            v = D_8014999C + 1;
        }
        D_801499A0.unk_00 = 0;
        D_8014999C = v;
    }
    D_801206AC = D_80149940 * 6 + D_8014999C + 6;
    if (D_80149990.unk_00 != 0 && (D_80149950 == 2 || D_80149950 == 4 || D_80149950 == 6)) {
        v = D_8014999C;
        if (v >= 3) {
            v = 0;
        }
        D_801206AC = b.v[D_80149940] + v;
        D_8014999C = v;
        if (D_80149940 == 2 && D_80120697 == 0xA5) {
            D_801206AC += 4;
        }
    }
}
