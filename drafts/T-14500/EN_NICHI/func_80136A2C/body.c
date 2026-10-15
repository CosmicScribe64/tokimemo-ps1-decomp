/* FAKE: empty test separates the loop constant live ranges. T-10850 */
void func_80136A2C(void)
{
  s32 i;
  if (D_800B3CFC != 0)
  {
    D_800B3CF8 = 0;
    D_800B3CFC = 0;
    D_80139B18 += 1;
  }
  if (D_80139B18 > 0)
  {
    for (i = 0; i < 5; i++)
    {
      if ((((EnObj *) D_8013984C)[i * 2].unk_00 > 0) && (((EnObj *) D_8013984C)[i * 2].unk_00 < 4))
      {
        if (D_80139B18 < 3)
        {
          ((EnObj *) D_8013984C)[i * 2].unk_00 = ((EnObj *) D_8013984C)[i * 2].unk_04 + 1;
        }
        else
        {
          ((EnObj *) D_8013984C)[i * 2].unk_00 = 4;
          ((EnObj *) D_8013984C)[i * 2].unk_04 = 0;
        }
      }
      if ((((EnObj *) D_8013984C)[(i * 2) + 1].unk_00 > 0) && (((EnObj *) D_8013984C)[(i * 2) + 1].unk_00 < 4))
      {
        if (D_80139B18 < 3)
        {
          if (!i)
          {
          }
          ((EnObj *) D_8013984C)[(i * 2) + 1].unk_00 = ((EnObj *) D_8013984C)[(i * 2) + 1].unk_04 + 1;
        }
        else
        {
          ((EnObj *) D_8013984C)[(i * 2) + 1].unk_00 = 4;
          ((EnObj *) D_8013984C)[(i * 2) + 1].unk_04 = 0;
        }
      }
    }

  }
}
