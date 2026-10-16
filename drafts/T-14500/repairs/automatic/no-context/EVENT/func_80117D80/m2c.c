? func_80011DFC(s32, u8, u8, s16);                  /* extern */
? func_80033E88();                                  /* extern */
s32 func_80049B20(s32, ?, ?);                       /* extern */
? func_8004A8EC(?, u8, u8, s16);                    /* extern */
extern s16 *D_8009471C;
extern s16 *D_80094720;
extern s32 D_80094724;
extern s32 D_80094728;
extern s32 D_8009472C;
extern s16 D_800B095E;
extern u8 D_800B1746;
extern s32 D_800EECBC;
extern s32 D_801241D8;
extern s32 D_801241E8;
extern s32 D_801241F8;
extern s16 D_8012438C;
extern s16 D_80124394;
extern s16 D_8012439C;
extern s16 D_801243A4;
extern u8 D_801243B0;
extern u8 D_8012531C;

void func_80117D80(void) {
    s16 sp2A;
    s16 sp28;

    D_8009471C = &sp2A;
    D_80094724 = D_801241D8;
    D_80094728 = D_801241E8;
    sp2A = 0;
    D_8009472C = D_801241F8;
    if ((D_800EECBC == 0) && (D_800B095E < 0x28) && (D_8012531C == 1) && (D_801243B0 == 0x10) && ((D_800B1746 == 0) || (D_800B1746 == 1) || (D_800B1746 == 3) || (D_800B1746 == 4) || (D_800B1746 == 5) || (D_800B1746 == 8) || (D_800B1746 == 9))) {
        D_8012439C = 3;
        D_8012438C = 3;
        D_80124394 = 3;
        D_801243A4 = 2;
        func_8004A8EC(3, D_8012531C, D_801243B0, D_800B095E);
        func_80033E88();
        sp28 = 5;
        D_80094720 = &sp28;
        if (func_80049B20(D_800EECBC, 0, 0) != 0) {
            func_80011DFC();
        }
    } else if ((D_800EECBC == 0) && (D_800B095E < 0x28) && (D_8012531C == 0) && (D_801243B0 == 0x13)) {
        switch (D_800B1746);                        /* irregular */
    } else if ((D_800EECBC == 0) && (D_800B095E < 0x64) && (D_8012531C == 0) && (D_801243B0 == 0x14) && ((D_800B1746 == 0) || (D_800B1746 == 4) || (D_800B1746 == 6) || (D_800B1746 == 7) || (D_800B1746 == 8) || (D_800B1746 == 0xA))) {
    case 1:
    case 3:
    case 6:
    case 0:
    case 7:
        D_8012439C = 3;
        D_8012438C = 3;
        D_80124394 = 3;
        D_801243A4 = 2;
        func_8004A8EC(3, D_8012531C, D_801243B0, D_800B095E);
        func_80033E88();
        sp28 = 6;
        D_80094720 = &sp28;
        if (func_80049B20(D_800EECBC, 0, 0) != 0) {
            func_80011DFC();
        }
    } else {
        func_80011DFC(D_800EECBC, D_8012531C, D_801243B0, D_800B095E);
    }
}
