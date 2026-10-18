void func_8014265C(void) {
    func_8013D5D8();
    if (D_800E6280.unk_1104.w++ == 0) {
        *(DisplayActor44 *)D_801206D8 = *(DisplayActor44 *)D_80120650;
        *(DisplayActor44 *)D_801206D8 = *(DisplayActor44 *)D_80120650;
        D_801206EE = 4;
    }
    if ((u16)D_800CA150 == 7 && D_800CA154 == 1 && check_end_k() == 0) {
        D_801206DA = 1;
    } else {
        D_801206DA = 0;
        D_801206F0 = 0;
    }
}
