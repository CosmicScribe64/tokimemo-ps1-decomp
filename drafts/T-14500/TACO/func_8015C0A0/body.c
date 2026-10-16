/* FAKE: repeated empty tests split the actor pointer live range. T-10850 */
void func_8015C0A0(arg0, arg1)
s32 arg0;
s32 arg1;
{
  TcActor *a;
  s32 i;
  a = &D_8015EDB4[arg1];
  if (((!a) && (!a)) && (!a))
  {
  }
  if (a->pad0[1] & 0x80)
  {
    if (arg0 == 1)
    {
      if (a->unk84[2] < 0xB)
      {
        *((s32 *) (((u8 *) a) + 4)) = 0;
        return;
      }
      *((s32 *) (((u8 *) a) + 4)) = 0x80000000;
      for (i = 0; i < 0x40; i++)
      {
        if (arg1 == D_80161D9C[i])
        {
          D_80127080[i].unk0 = (u16) 0;
        }
      }

      return;
    }
    if (a->unk84[2] < 9)
    {
      *((s32 *) (((u8 *) a) + 4)) = 0x80000000;
      return;
    }
    *((s32 *) (((u8 *) a) + 4)) = 0x80000000;
    for (i = 0; i < 0x40; i++)
    {
      if (arg1 == D_80161D9C[i])
      {
        D_80127080[i].unk0 = 0x80000000;
      }
    }

  }
}
