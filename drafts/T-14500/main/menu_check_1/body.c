/* FAKE: duplicate GameState pointer steers record/base allocation. T-12050 (inherited T-10850) */
s8 menu_check_1(arg0, arg1, arg2)
    s32 arg0;
    s16 arg1;
    s16 arg2;
{
    GameState *g;
    s32 cnt;
    s32 sel;
    s32 d;
    u32 i;
    GameState *new_var;
    g = &D_800E6280;
    cnt = 0;
    sel = -1;
    new_var = g;
    i = 0;
    if (((MenuSelection5 *) (&new_var->unk_1090))->count[arg0] != 0)
    {
        do
        {
            d = arg1 - ((GsRecF90 (*)[2]) new_var->unk_F90)[i][arg0].unk_00;
            ((GsRecF90 (*)[2]) new_var->unk_F90)[i][arg0].unk_06 = 0;
            if (((s32) ((d - ((GsRecF90 (*)[2]) new_var->unk_F90)[i][arg0].unk_04) * d)) < 0)
            {
                d = arg2 - ((GsRecF90 (*)[2]) new_var->unk_F90)[i][arg0].unk_02;
                if (((s32) ((d - ((GsRecF90 (*)[2]) new_var->unk_F90)[i][arg0].unk_05) * d)) < 0)
                {
                    ((GsRecF90 (*)[2]) new_var->unk_F90)[i][arg0].unk_06 = 1;
                    cnt += 1;
                    sel = i;
                }
            }
            i += 1;
        }
        while (i < ((MenuSelection5 *) (&new_var->unk_1090))->count[arg0]);
    }
    if (cnt == 0)
    {
        ((MenuSelection5 *) (&new_var->unk_1090))->current[arg0] = -1;
    }
    if ((!(g->unk_F80 & 4)) && (sel != (-1)))
    {
        g->unk_1095[arg0] = sel;
    }
    return sel;
}
