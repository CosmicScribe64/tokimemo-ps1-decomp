/* T-22022, 188 rows: cases are plain (no goto). The original copies the call result (move v1,v0; andi t6,v1,0x7F; delay slot move v1,t6) and so starts the case temps at t2; here v1 = v0 & 0x7F directly (a separate s32 local is folded by uopt) and the case blocks start at t0 with another load schedule. */
void func_8013E824(void) {
    s16 var_a0;
    s32 temp_v1;
    u32 temp_t6;

    temp_v1 = func_8005E0E0(D_800F62CF);
    temp_t6 = temp_v1 & 0x7F;
    var_a0 = 0;
    if (temp_t6 >= 2U) {
        var_a0 = 2;
    }
    if (temp_t6 >= 3U) {
        var_a0 += 2;
    }
    D_80148108 = var_a0;
    if (temp_t6 == 4) {
        D_80148108 = var_a0 + 2;
    }
    switch (D_800F62CF) {
    case 0:
        D_801480FC = D_80148064;
        D_80148100 = D_80148098;
        D_80148104 = D_801480CC;
        break;
    case 1:
        D_801480FC = D_80148078;
        D_80148100 = D_801480AC;
        D_80148104 = D_801480E0;
        break;
    case 2:
        D_801480FC = D_80148068;
        D_80148100 = D_8014809C;
        D_80148104 = D_801480D0;
        break;
    case 3:
        D_801480FC = D_80148070;
        D_80148100 = D_801480A4;
        D_80148104 = D_801480D8;
        break;
    case 4:
        D_801480FC = D_80148088;
        D_80148100 = D_801480BC;
        D_80148104 = D_801480F0;
        break;
    case 5:
        D_801480FC = D_8014807C;
        D_80148100 = D_801480B0;
        D_80148104 = D_801480E4;
        break;
    case 6:
        D_801480FC = D_80148080;
        D_80148100 = D_801480B4;
        D_80148104 = D_801480E8;
        break;
    case 7:
        D_801480FC = D_80148074;
        D_80148100 = D_801480A8;
        D_80148104 = D_801480DC;
        break;
    case 8:
        D_801480FC = D_80148060;
        D_80148100 = D_80148094;
        D_80148104 = D_801480C8;
        break;
    case 9:
        D_801480FC = D_80148084;
        D_80148100 = D_801480B8;
        D_80148104 = D_801480EC;
        break;
    case 10:
        D_801480FC = D_80148090;
        D_80148100 = D_801480C4;
        D_80148104 = D_801480F8;
        break;
    }
    func_8004DE1C();
}
