/* T-24301 draft, ~66 rows: control flow right; the record pointer takes $v0 (original $v1), the li 22
 * and the F5F reload are scheduled earlier, and the frame wants two scalars above `names`. */
void func_80133C50(void) {
    s32 state;
    u32 have;
    u32 want;
    GekoNames12 names;

    names = D_80144D8C;
    func_800AE0F0(&D_800CA280, names.name[D_800E6280.unk_0F4.h >> 12]);
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[0] |= 1;
    if (D_80144C50 == 10) {
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[0] |= 0x40;
    }
    D_80144C3C = 0x16;
    state = get_g_zyotai_s(D_800E6280.unk_F5F);
    if (D_80144C50 == 2) {
        D_80144C50 = 0;
        if (D_800E6280.unk_F5F == 0) {
            have = D_800E6280.unk_0F4.h >> 12;
            want = D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[2] >> 4;
            if (have == want && have < 6) {
                func_80132A4C(2);
            } else if (have != want && have < 6 && want < 6) {
                func_80132A4C(3);
            } else if (have < 6 && want >= 6) {
                func_80132A4C(4);
            } else {
                func_80132A4C(5);
            }
        } else if (D_800E6280.unk_F5F == 1 || D_800E6280.unk_F5F == 2 || D_800E6280.unk_F5F == 3) {
            have = D_800E6280.unk_0F4.h >> 12;
            want = D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[2] >> 4;
            if (have == want) {
                func_80132A4C(2);
            } else if (have != want && have < 6) {
                func_80132A4C(3);
            } else {
                func_80132A4C(5);
            }
        } else if (D_800E6280.unk_F5F == 4 || D_800E6280.unk_F5F == 5 || D_800E6280.unk_F5F == 10) {
            if ((D_800E6280.unk_0F4.h >> 12) < 6) {
                func_80132A4C(3);
            } else if ((state & 0x7F) < 2) {
                func_80132A4C(4);
            } else {
                func_80132A4C(5);
            }
        } else if (D_800E6280.unk_F5F == 6 || D_800E6280.unk_F5F == 7 || D_800E6280.unk_F5F == 8 || D_800E6280.unk_F5F == 9) {
            if ((D_800E6280.unk_0F4.h >> 12) < 6) {
                func_80132A4C(2);
            } else {
                func_80132A4C(3);
            }
        }
    }
    func_8004284C();
}
