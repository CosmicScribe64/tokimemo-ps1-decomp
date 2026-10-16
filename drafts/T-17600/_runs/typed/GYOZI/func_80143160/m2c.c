extern s32 D_80148970;

void func_80143160(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s16 temp_v0;

    if (D_800F647A == 0) {
        D_80148970 = *(s32 *)((unsigned char *)(&D_800F53A0.girl[D_800F62CF]) + (8));
        D_800F53A0.girl[D_800F62CF].unk_0A = 0x32;
    }
    switch (D_800F62CF) {
    case 0:
        func_80052060(arg0, arg1, arg2, arg3);
        break;
    case 1:
        func_80143314();
        break;
    case 2:
        func_801433E0();
        break;
    case 3:
        func_80143520();
        break;
    case 4:
        func_801435C4();
        break;
    case 5:
        func_801436E4();
        break;
    case 6:
        func_80143780();
        break;
    case 7:
        func_80143858();
        break;
    case 8:
        func_80143940();
        break;
    case 9:
        func_801439F0();
        break;
    case 10:
        func_80052060(arg0, arg1, arg2, arg3);
        break;
    }
    *(u8 *)((unsigned char *)D_80129F40 + (6535)) = (u8) D_801317EB;
    if ((*(s16 *)((unsigned char *)D_80129F40 + (6552)) == 0) && (*(s16 *)((unsigned char *)D_80129F40 + (6536)) == 0)) {
        temp_v0 = *(s16 *)((unsigned char *)D_80129F40 + (6550)) + 1;
        *(s16 *)((unsigned char *)D_80129F40 + (6550)) = temp_v0;
        if (temp_v0 >= 3) {
            *(s16 *)((unsigned char *)D_80129F40 + (6550)) = 0;
        }
    }
}
