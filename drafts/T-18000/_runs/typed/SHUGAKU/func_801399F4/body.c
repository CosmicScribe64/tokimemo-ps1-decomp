void func_801399F4(void) {
    s32 var_t1;
    u8 temp_t6;

    temp_t6 = D_800E6280.unk_F5F;
    switch (temp_t6) {
    case 0:
        D_800CA2D0 = D_8013C424;
        D_800CA2D4 = D_8013C458;
        D_800CA2D8 = D_8013C48C;
        return;
    case 1:
        D_800CA2D0 = D_8013C434;
        D_800CA2D4 = D_8013C468;
        D_800CA2D8 = D_8013C49C;
        return;
    case 2:
        D_800CA2D0 = D_8013C428;
        D_800CA2D4 = D_8013C45C;
        D_800CA2D8 = D_8013C490;
        return;
    case 3:
        D_800CA2D0 = D_8013C42C;
        D_800CA2D4 = D_8013C460;
        D_800CA2D8 = D_8013C494;
        return;
    case 4:
        D_800CA2D0 = D_8013C448;
        var_t1 = D_8013C4B0;
        D_800CA2D4 = D_8013C47C;
block_13:
        D_800CA2D8 = var_t1;
    default:
        return;
    case 5:
        D_800CA2D0 = D_8013C438;
        D_800CA2D4 = D_8013C46C;
        D_800CA2D8 = D_8013C4A0;
        return;
    case 6:
        D_800CA2D0 = D_8013C440;
        D_800CA2D4 = D_8013C474;
        D_800CA2D8 = D_8013C4A8;
        if ((u32) (get_g_zyotai_s(6) & 0x7F) < 2U) {
            D_800CA2D0 = D_8013C43C;
            D_800CA2D4 = D_8013C470;
            D_800CA2D8 = D_8013C4A4;
            return;
        }
        break;
    case 7:
        D_800CA2D0 = D_8013C430;
        D_800CA2D4 = D_8013C464;
        D_800CA2D8 = D_8013C498;
        return;
    case 8:
        D_800CA2D0 = D_8013C420;
        D_800CA2D4 = D_8013C454;
        D_800CA2D8 = D_8013C488;
        return;
    case 9:
        D_800CA2D0 = D_8013C444;
        var_t1 = D_8013C4AC;
        D_800CA2D4 = D_8013C478;
        goto block_13;
    }
}
