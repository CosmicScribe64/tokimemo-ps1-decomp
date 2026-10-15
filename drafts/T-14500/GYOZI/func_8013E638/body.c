/* FAKE: empty counter test steers allocation; the opaque call keeps only
 * the base argument to avoid a redundant stride reload. T-10850. */
void func_8013E638(void)
{
    s16 i;
    i = 0;
    if (i && i)
    {
    }
    do
    {
        if ((D_800F53A0.girl[i].unk_10[1] & 1) && (D_800F53A0.unk_F2F != i))
        {
            D_800F53A0.girl[i].unk_0A += 1;
        }
        i += 1;
    }
    while (i < 0xB);
    D_800F53A0.girl[D_800F53A0.unk_F2F].unk_06 += 8;
    D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0A += 8;
    func_8008FB00(&D_800F53A0);
    func_80050D60(0, 0);
    func_80081D30(1);
}
