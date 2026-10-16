void func_8014AD9C(s32 arg0, s16 *arg1, s32 arg2) {
    MATRIX matrix;
    TaiikuBig *coord;
    SVECTOR *rotation;

    arg1 += arg2 * 6;
    matrix = *(MATRIX *)D_80125C60;
    coord = &D_801227A0[arg0];
    matrix.t[0] = ((MATRIX *)coord->unk4)->t[0] + arg1[0];
    matrix.t[1] = ((MATRIX *)coord->unk4)->t[1] + arg1[1];
    matrix.t[2] = ((MATRIX *)coord->unk4)->t[2] + arg1[2];
    rotation = &((SVECTOR *)D_8015DDD0)[arg0];
    rotation->vx += arg1[3];
    arg1 += 4;
    func_8015A640((rotation->vx << 12) / 360, &matrix);
    rotation->vz += *arg1;
    arg1++;
    func_8015A7E0((rotation->vz << 12) / 360, &matrix);
    rotation->vy += *arg1;
    func_8015A980((rotation->vy << 12) / 360, &matrix);
    *(MATRIX *)coord->unk4 = matrix;
    coord->unk0 = 0;
}
