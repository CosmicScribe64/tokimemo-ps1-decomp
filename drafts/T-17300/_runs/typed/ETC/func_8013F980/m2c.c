? func_8009B3C0(?);                                 /* extern */
? func_8009B430(void *, s32 *, ?);                  /* extern */
? func_800AE0B0(? *, s32);                          /* extern */
extern s32 D_80122640;
extern ? *D_80122644;
extern ? D_801227A0;
extern ? D_80122CA0;
extern s32 D_801500C8;
extern s32 D_801500CC;
extern s32 D_801500D0;
extern s32 D_801500D4;

void func_8013F980(void) {
    ? *var_s1;
    s32 *var_s0;
    void *temp_s2;

    func_8009B3C0(0x801FA804);
    func_800AE0B0("NOBJ %x\n", *(s32 *)0x801FA808);
    temp_s2 = (s32 *)0x801FA808 + 4;
    var_s1 = &D_801227A0;
    var_s0 = &D_80122640;
    do {
        func_8009B430(temp_s2, var_s0, 0);
        *(? **)((unsigned char *)var_s0 + (4)) = var_s1;
        var_s1 += 0x50;
        var_s0 += 0x10;
        *(s32 *)((unsigned char *)var_s0 + (-16)) = 0x80000000;
    } while (var_s1 != &D_80122CA0);
    func_8009B430(temp_s2, &D_80122640, 0);
    D_80122644 = &D_801227A0;
    D_80122640 = 0;
    D_801500C8 = 0x9F;
    D_801500CC = -0x78;
    D_801500D0 = 0;
    D_801500D4 = 0xC00;
}
