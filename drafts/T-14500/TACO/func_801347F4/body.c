/* NON_MATCHING: T-10520, only "move s2,s3; addiu s2,s2,8" against one addiu and the lui/addiu order of the loop constants differ (7 lines); a K&R definition with `s32 arg0` fixes the prologue but the header check rejects it */
void func_801347F4(s32 arg0) {
    s32 b;
    TcObj50 *p;
    Tc10 *q;

    arg0 += 4;
    func_8009B3C0(arg0);
    b = arg0;
    b += 8;
    p = D_80127480;
    q = D_80127080;
    do {
        func_8009B430(b, (u8 *)q, 0);
        q->unk4[0] = (s32)p;
        p++;
        q++;
        q[-1].unk0 = 0x80000000;
    } while (p != (TcObj50 *)D_80128880);
}
