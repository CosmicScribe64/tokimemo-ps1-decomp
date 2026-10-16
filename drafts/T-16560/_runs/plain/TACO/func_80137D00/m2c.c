extern s32 D_800E7378;
extern u8 D_800E738D;
extern void *D_801FAA84;

void func_80137D00(void) {
    switch (D_800E738D) {                           /* irregular */
    case 0:
        if (D_8015EDEC < 3) {
            func_80137CBC((s32) D_8015EDEC);
            D_8015EDEC += 1;
            D_800E738D += 1;
        } else {
            func_8004284C();
            *(s16 *)((unsigned char *)D_801FAA84 + (10)) = 0x28;
            *(s16 *)((unsigned char *)D_801FAA84 + (22)) = 0xFF00;
            *(s16 *)((unsigned char *)D_801FAA84 + (36)) = 0xFF00;
            *(s16 *)((unsigned char *)D_801FAA84 + (50)) = 0xFF00;
            *(s16 *)((unsigned char *)D_801FAA84 + (64)) = 0xFF00;
            func_80137B14(2);
            D_8011ED3E -= 0x30;
        }
        break;
    case 1:
        if (func_800460EC() & 4) {
            D_800E738D += 1;
        }
        break;
    case 2:
        if (func_800460EC() & 2) {
            D_800E738D = 0;
        }
        break;
    }
    func_800578F4(2);
    if (D_800E7378 & 1) {
        D_8011ED3E -= 1;
    }
}
