/* Unmatched T-24503 checkpoint: aligned cost 44, not a literal match. */
void func_80133C50(void) {
    s32 v;
    u32 g;
    GekoNames12 names;
    u32 temp;

    names = D_80144D8C;
    func_800AE0F0(&D_800CA280, names.name[D_800E6280.unk_0F4.h >> 12]);
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b0 = 1;
    if (D_80144C50 == 10) {
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b6 = 1;
    }
    D_80144C3C = 0x16;
    v = get_g_zyotai_s(D_800E6280.unk_F5F);
    if (D_80144C50 == 2) {
        g = D_800E6280.unk_F5F;
        D_80144C50 = 0;
        if (g == 0) {
            v = D_800E6280.unk_0F4.h >> 12;
            temp = D_800E6280.unk_1BC[g].unk_0C.b[2] >> 4;
            g = temp;
            if (v == g && (u32)v < 6) {
                func_80132A4C(2);
            } else if (v != g && (u32)v < 6 && g < 6) {
                func_80132A4C(3);
            } else if ((u32)v < 6 && g >= 6) {
                func_80132A4C(4);
            } else {
                func_80132A4C(5);
            }
        } else if (g == 1 || g == 2 || g == 3) {
            v = D_800E6280.unk_0F4.h >> 12;
            /* FAKE: T-24503 empty test retains this club value before the
             * record load, restoring this branch's original temporary numbering. */
            if (v) {}
            g = D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[2] >> 4;
            if (v == g) {
                func_80132A4C(2);
            } else if (v != g && (u32)v < 6) {
                func_80132A4C(3);
            } else {
                func_80132A4C(5);
            }
        } else if (g == 4 || g == 5 || g == 10) {
            if ((D_800E6280.unk_0F4.h >> 12) < 6) {
                func_80132A4C(3);
            } else if ((u32)(v & 0x7F) < 2) {
                func_80132A4C(4);
            } else {
                func_80132A4C(5);
            }
        } else if (g == 6 || g == 7 || g == 8 || g == 9) {
            if ((D_800E6280.unk_0F4.h >> 12) < 6) {
                func_80132A4C(2);
            } else {
                func_80132A4C(3);
            }
        }
    }
    func_8004284C();
}
