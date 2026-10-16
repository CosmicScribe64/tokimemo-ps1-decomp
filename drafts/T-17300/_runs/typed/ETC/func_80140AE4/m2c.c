? func_80044750(?);                                 /* extern */
? func_80140BC4();                                  /* extern */
? func_80140DC8();                                  /* extern */
? func_80140E80();                                  /* extern */
s32 func_80143240();                                /* extern */
extern u8 D_800B3CFC;
extern ? D_800E6280;
extern s32 D_801500D8;
extern s32 D_801500DC;

void func_80140AE4(void) {
    func_80140BC4();
    func_80140DC8();
    func_80140E80();
    if (D_801500DC == 0) {
        if ((func_80143240() == 1) && (*(s32 *)((unsigned char *)(&D_800E6280) + (3968)) & 0x09000000)) {
            if (D_801500D8 != 1) {
                func_80044750(0x50E);
            }
            D_801500D8 = 1;
        }
    } else if (D_801500DC == 1) {
        if (D_800B3CFC == 1) {
            func_80044750(0x50E);
            if (D_801500D8 == 1) {
                D_801500D8 = 3;
            } else {
                D_801500D8 = 2;
            }
        }
    }
}
