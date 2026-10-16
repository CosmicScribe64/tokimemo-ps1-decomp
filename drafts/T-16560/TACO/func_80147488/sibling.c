/* T-16560: copy of matched sibling TACO func_8015B3B0 (register-erased ratio 1.000), with
 * the header prototype changed to `void func_80147488(s32 arg0, s16 *arg1, s32 arg2);` and
 * the MATRIX local declared first (the original keeps it at sp+0x38, top of a 0x58 frame).
 * Score 10 (funcdiff --resolve): only the spill homes of obj and pos differ, 0x2C/0x28
 * against the original's 0x30/0x2C. A `s32 pad` before or after them grows the frame to 0x60;
 * declaring pos before obj gives the same 10 rows. Not in src. */
void func_80147488(s32 arg0, s16 *arg1, s32 arg2) {
    MATRIX matrix;
    TcTransformObj50 *obj;
    TcPos *pos;

    arg1 += arg2 * 6;
    matrix = *(MATRIX *)D_80125C60;
    obj = (TcTransformObj50 *)&D_80127480[arg0];
    matrix.t[0] = obj->transform.t[0] + arg1[0];
    matrix.t[1] = obj->transform.t[1] + arg1[1];
    matrix.t[2] = obj->transform.t[2] + arg1[2];
    pos = &D_80128880[arg0];
    pos->x += arg1[3];
    arg1 += 4;
    func_8015D0D0((pos->x << 12) / 360, &matrix);
    pos->z += *arg1;
    arg1++;
    func_8015CF30((pos->z << 12) / 360, &matrix);
    pos->y += *arg1;
    func_8015D270((pos->y << 12) / 360, &matrix);
    obj->transform = matrix;
    obj->unk_00 = 0;
}
