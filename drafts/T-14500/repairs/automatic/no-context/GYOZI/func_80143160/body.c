void func_80143160(void) {
    s16 temp_v0;

    if (D_800F647A == 0) {
        D_80148970 = *(s32 *)((unsigned char *)(&D_800F53A0 + (D_800F62CF * 0x38)) + (432));
        *(s16 *)((unsigned char *)(&D_800F53A0 + (D_800F62CF * 0x38)) + (434)) = 0x32;
    }
    switch (D_800F62CF) {
    case 0:
        func_80052060();
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
        func_80052060();
        break;
    }
    D_8012B8C7 = D_801317EB;
    if ((D_8012B8D8 == 0) && (D_8012B8C8 == 0)) {
        temp_v0 = D_8012B8D6 + 1;
        D_8012B8D6 = temp_v0;
        if (temp_v0 >= 3) {
            D_8012B8D6 = 0;
        }
    }
}
