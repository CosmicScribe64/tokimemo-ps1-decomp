/* NON_MATCHING: T-10720, same as TACO func_801347F4: the original keeps 'move s2,s3; addiu s2,s2,8' where this folds to one addiu (s3 = arg0 + 4), 5 lines */
void func_801406A8(arg0)
s32 arg0;
{
    TaiikuBig *q;
    u32 *p;
    s32 b;

    arg0 += 4;
    func_8009B3C0(arg0);
    b = arg0;
    b += 8;
    q = D_801227A0;
    p = D_80122640;
    do {
        func_8009B430(b, (u8 *)p, 0);
        *(TaiikuBig **)(p + 1) = q;
        q++;
        p += 4;
        p[-4] = 0x80000000;
    } while (q != D_80122CA0);
}
