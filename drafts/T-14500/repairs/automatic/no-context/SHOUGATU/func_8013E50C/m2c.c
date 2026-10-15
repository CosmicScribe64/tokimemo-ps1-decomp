? func_8004284C();                                  /* extern */
s32 func_80051A68(?, s16 (*)[], ?);                 /* extern */
? func_800847B8(u8);                                /* extern */
? func_80084D3C(void *, s16 (*)[], ? *, ?);         /* extern */
? func_80085B3C(?, ?);                              /* extern */
extern ? D_800E6280;
extern s16 D_800E6396;
extern s16 D_800E6636;
extern s32 D_800E6640;
extern u8 D_800E71DF;
extern s32 D_800E7378;
extern s32 D_80145AD4;
extern s32 D_80145B08;
extern s32 D_80145B3C;
extern s32 D_80145F20;
extern s32 D_80145F24;
extern s32 D_80145F28;
extern s16 D_80145F2C;
extern s8 D_80146050;
extern ? D_801460CC;
extern ? D_801460DC;

void func_8013E50C(void) {
    ? sp30;
    s16 temp_t8;
    s16 temp_v1;
    s16 var_v1;
    s32 var_v0;
    void *temp_a0;
    void *temp_a0_2;

    sp40[0].unk0 = (s32) D_801460CC.unk0;
    *(s32 *)((unsigned char *)(&sp40[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801460CC) + (4));
    *(s32 *)((unsigned char *)(&sp40[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_801460CC) + (8));
    *(u16 *)((unsigned char *)(&sp40[0]) + (12)) = (u16) *(u16 *)((unsigned char *)(&D_801460CC) + (12));
    sp30.unk0 = (s32) D_801460DC.unk0;
    *(s32 *)((unsigned char *)(&sp30) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_801460DC) + (8));
    *(s32 *)((unsigned char *)(&sp30) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801460DC) + (4));
    *(u16 *)((unsigned char *)(&sp30) + (12)) = (u16) *(u16 *)((unsigned char *)(&D_801460DC) + (12));
    D_80146050 = 0;
    if (!(D_800E6640 & 2) && (D_800E6636 >= 0x14) && (D_800E7378 & 1)) {
        D_80146050 = 1;
    }
    D_800E71DF = 0;
    D_80145F2C = ((func_80051A68(0, (s16 (*)[]) &sp40[0], 0) & 0x7F) == 4) * 4;
    D_80145F20 = D_80145AD4;
    D_80145F24 = D_80145B08;
    D_80145F28 = D_80145B3C;
    func_800847B8(D_800E71DF);
    var_v1 = 0;
    temp_a0 = &D_800E6280 + (D_800E71DF * 0x38);
    *(s16 *)((unsigned char *)temp_a0 + (450)) = (s16) (*(s16 *)((unsigned char *)temp_a0 + (450)) + 4);
loop_5:
    var_v0 = var_v1 * 2;
    temp_v1 = var_v1 + 1;
    if (D_800E6396 >= (&sp40[0])[var_v1]) {
        temp_t8 = temp_v1;
        var_v1 = temp_t8;
        if (temp_t8 >= 7) {
            var_v0 = temp_t8 * 2;
        } else {
            goto loop_5;
        }
    }
    temp_a0_2 = &D_800E6280 + (D_800E71DF * 0x38);
    *(s16 *)((unsigned char *)temp_a0_2 + (446)) = (s16) (*(s16 *)((unsigned char *)temp_a0_2 + (446)) + *(s16 *)((unsigned char *)(sp + var_v0) + (48)));
    func_80084D3C(temp_a0_2, (s16 (*)[]) &sp40[0], &D_800E6280, 0x38);
    func_80085B3C(4, 0);
    func_8004284C();
}
