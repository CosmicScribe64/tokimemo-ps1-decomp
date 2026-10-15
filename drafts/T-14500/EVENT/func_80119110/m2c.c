? func_80014CA8(?, ?);                              /* extern */
? func_8003E638(?);                                 /* extern */
extern u8 D_800B1AF4;

void func_80119110(void) {
    void *temp_v0;
    void *temp_v0_2;
    void *temp_v0_3;
    void *temp_v0_4;

    if ((D_800B1AF0 == 0) && (D_800EECE4 >= 0) && (D_800B1AF4 != 0x60)) {
        temp_v0 = D_800B0860 + (D_800B1746 * 0x34) + D_800EECE4;
        *(u8 *)((unsigned char *)temp_v0 + (440)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (440)) + 1);
    }
    if (D_800B1AF0 == 0) {
        if (D_800EECE4 >= 0) {
            *(s8 *)((unsigned char *)(&D_800B0860[D_800B1746]) + (1749)) = (s8) D_800EECE4;
        }
        if (D_800EECE4 == 7) {
            *(s8 *)((unsigned char *)(&D_800B0860[D_800B1746]) + (1749)) = 6;
        }
        if (D_800EECE4 == 5) {
            *(s8 *)((unsigned char *)(&D_800B0860[D_800B1746]) + (1749)) = 4;
        }
        if ((D_800EECE4 == 0xB) || (D_800EECE4 == 0xC) || (D_800EECE4 == 0xD)) {
            *(s8 *)((unsigned char *)(&D_800B0860[D_800B1746]) + (1749)) = 0xA;
        }
    }
    if ((D_800B0966 >= 0x23) || ((D_800B0966 >= 0x1E) && (D_800B1746 != 7))) {
        temp_v0_2 = D_800B0860 + (D_800B1746 * 0x34);
        *(s16 *)((unsigned char *)temp_v0_2 + (430)) = (s16) (*(s16 *)((unsigned char *)temp_v0_2 + (430)) - 0x28);
    } else {
        if (D_801243B0 == 0x12) {
            temp_v0_3 = D_800B0860 + (D_800B1746 * 0x34);
            *(s16 *)((unsigned char *)temp_v0_3 + (422)) = (s16) (*(s16 *)((unsigned char *)temp_v0_3 + (422)) + 1);
        }
        if ((D_800B0896 == 5) && ((D_800B0897 == 4) || (D_800B0897 == 5))) {
            temp_v0_4 = D_800B0860 + (D_800B1746 * 0x34);
            if (*(s32 *)((unsigned char *)temp_v0_4 + (432)) & 0x10) {
                *(s16 *)((unsigned char *)temp_v0_4 + (422)) = (s16) (*(s16 *)((unsigned char *)temp_v0_4 + (422)) + 2);
            }
        }
    }
    func_8004C250(D_800B0860);
    D_800B0F2C = D_801243B0;
    func_80014CA8(0, 0);
    func_8003E638(1);
}
