void RotMatrix();
void ScaleMatrix();
void MulMatrix();
s32 abs();

s32 func_80062948(void) {
    SVECTOR rot;
    MATRIX m;
    MATRIX s;
    VECTOR scale;
    s32 i;
    s32 j;
    s32 n;

    for (i = 0; i < 13; i++) {
        j = i & 3;
        n = j + 1;
        rot.vz = -(D_800E6280.unk_10F8 * (j + 2)) * 2 - 2;
        rot.vx = rsin(D_800E6280.unk_10F8 * 10 * n) / 12 + 0x7FD;
        rot.vy = func_800A0140(D_800E6280.unk_10F8 * 10 * n) / 12 - 5;
        m = *(MATRIX *)D_80125C60;
        s = *(MATRIX *)D_80125C60;
        RotMatrix(&rot, &m);
        m.t[0] = 0;
        m.t[1] = 0;
        m.t[2] = 0;
        scale.vx = 0x1000;
        scale.vy = 0x1000;
        scale.vz = 0x1000;
        ScaleMatrix(&s, &scale);
        MulMatrix(&m, &s);
        m.t[0] = (&D_801230D0)[i + 1];
        m.t[1] = rsin((D_800E6280.unk_10F8 & 0xFFF) * (j + 10)) / 32 + -(&D_801230D0)[i + 1] / 2;
        m.t[2] = (&D_801230D0)[i + 1] * -4 - 0x190;
        *(MATRIX *)(D_801227A4 + i * 0x50) = m;
        (&D_801230D0)[i + 1] = (abs(rsin(D_800E6280.unk_10F8)) & 1) + (&D_801230D0)[i + 1] + 2;
        D_801227A0[i].unk0 = 0;
    }
}
