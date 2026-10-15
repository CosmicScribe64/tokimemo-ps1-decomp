/* FAKE: split pointer setup and empty test preserve address temporaries. T-10850 */
void func_801511C0(s32 arg0, s32 arg1)
{
  s32 i;
  s32 zero;
  s32 n;
  s32 *p;
  s32 *base;
  i = 0;
  n = arg0;
  base = D_8016024C;
  zero = 0;
  p = base;
  p = p + arg1;
  if (arg1)
  {
  }
  do
  {
    func_801478F8(n, *p);
    i += 1;
    n += 1;
    p += 1;
  }
  while (i != 6);
  if (arg1 != zero)
  {
    func_801478F8(0x1B, D_80160280);
    return;
  }
  func_801478F8(0x1A, D_80160264);
}
