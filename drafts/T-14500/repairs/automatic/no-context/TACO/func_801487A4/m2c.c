? func_800A0ECC(? *, s16 *, s16 *);                 /* extern */
? func_8015CF30(s32, ? *);                          /* extern */
? func_8015D0D0(s32, ? *);                          /* extern */
? func_8015D270(s32, ? *);                          /* extern */
extern s32 D_800E7384;
extern ? D_80125C60;
extern ? D_80127080;
extern ? D_80127480;
extern s32 D_80127498;
extern s32 D_8012749C;
extern s32 D_801274A0;
extern s16 D_80128880;
extern s16 D_80128882;
extern s16 D_80128884;
extern void *D_8015EDB4;

void func_801487A4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    ? sp5C;
    s16 sp58;
    s16 sp56;
    s16 sp54;
    s16 sp4C;
    s32 sp44;
    s32 sp40;
    s32 sp3C;
    s32 temp_t0;
    s32 temp_v1;
    s32 temp_v1_2;
    u8 temp_v0;
    u8 var_a0;
    void *temp_a0;
    void *temp_v0_2;
    void *temp_v0_3;
    void *temp_v0_4;
    void *temp_v0_5;
    void *temp_v0_6;

    temp_v0 = *(u8 *)((unsigned char *)D_8015EDB4 + (2314));
    temp_t0 = arg1 + 0x10;
    if (temp_v0 == 1) {
        temp_v0_2 = arg2 + (arg1 * 8);
        sp44 = (s32) temp_v0_2->unk0;
        temp_a0 = (arg1 * 2) + arg3;
        sp40 = (s32) *(s16 *)((unsigned char *)temp_v0_2 + (2));
        temp_v1 = temp_t0 * 0x88;
        *(u8 *)((unsigned char *)(D_8015EDB4 + temp_v1) + (132)) = (u8) temp_a0->unk0;
        if ((arg0 == 0x1C) && (*(u8 *)((unsigned char *)D_8015EDB4 + (2310)) == 1)) {
            *(u8 *)((unsigned char *)(D_8015EDB4 + temp_v1) + (132)) = (u8) *(u8 *)((unsigned char *)temp_a0 + (1));
        }
        if (((arg0 == 0x1C) || (arg0 == 0x1B) || (arg0 == 0x15)) && (*(u8 *)((unsigned char *)D_8015EDB4 + (2990)) == 1)) {
            *(u8 *)((unsigned char *)(D_8015EDB4 + temp_v1) + (132)) = (u8) *(u8 *)((unsigned char *)temp_a0 + (1));
        }
    } else if ((temp_v0 == 0x1D) && ((temp_v0_3 = arg2 + (arg1 * 8), sp44 = (s32) *(s16 *)((unsigned char *)temp_v0_3 + (4)), sp40 = (s32) *(s16 *)((unsigned char *)temp_v0_3 + (6)), temp_v1_2 = temp_t0 * 0x88, *(u8 *)((unsigned char *)(D_8015EDB4 + temp_v1_2) + (132)) = (u8) *(u8 *)((unsigned char *)((arg1 * 2) + arg3) + (1)), (arg0 == 0x1B)) || (arg0 == 0x1C)) && (*(u8 *)((unsigned char *)D_8015EDB4 + (3126)) != 0) && (*(u8 *)((unsigned char *)D_8015EDB4 + (3262)) == 0)) {
        *(s8 *)((unsigned char *)(D_8015EDB4 + temp_v1_2) + (132)) = 0;
    }
    sp5C.unk0 = (s32) D_80125C60.unk0;
    *(s32 *)((unsigned char *)(&sp5C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (4));
    *(s32 *)((unsigned char *)(&sp5C) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (8));
    *(s32 *)((unsigned char *)(&sp5C) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (12));
    *(s32 *)((unsigned char *)(&sp5C) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (16));
    *(s32 *)((unsigned char *)(&sp5C) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (20));
    *(s32 *)((unsigned char *)(&sp5C) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (24));
    *(s32 *)((unsigned char *)(&sp5C) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (28));
    sp3C = temp_t0 * 0x88;
    func_8015D0D0((s32) (D_80128880 << 0xC) / 360, &sp5C);
    func_8015CF30((s32) (D_80128884 << 0xC) / 360, &sp5C);
    func_8015D270((s32) (D_80128882 << 0xC) / 360, &sp5C);
    temp_v0_4 = (arg0 * 0x50) + &D_80127480;
    sp54 = (s16) *(s32 *)((unsigned char *)temp_v0_4 + (24));
    sp56 = (s16) *(s32 *)((unsigned char *)temp_v0_4 + (28));
    sp58 = (s16) *(s32 *)((unsigned char *)temp_v0_4 + (32));
    func_800A0ECC(&sp5C, &sp54, &sp4C);
    *(s16 *)((unsigned char *)(D_8015EDB4 + sp3C) + (116)) = (s16) (D_80127498 + sp4C + sp44);
    *(s16 *)((unsigned char *)(D_8015EDB4 + sp3C) + (118)) = (s16) (D_8012749C + sp4E + sp40);
    *(s16 *)((unsigned char *)(D_8015EDB4 + sp3C) + (120)) = (s16) (D_801274A0 + sp50);
    temp_v0_5 = sp3C + D_8015EDB4;
    var_a0 = *(u8 *)((unsigned char *)temp_v0_5 + (133));
    if ((var_a0 < 0x15U) && (*(s16 *)((unsigned char *)temp_v0_5 + (130)) < *(s16 *)((unsigned char *)temp_v0_5 + (122)))) {
        *(u8 *)((unsigned char *)temp_v0_5 + (133)) = (u8) *((arg1 * 2) + arg4);
        temp_v0_6 = sp3C + D_8015EDB4;
        *(s16 *)((unsigned char *)temp_v0_6 + (130)) = (s16) (*(s16 *)((unsigned char *)temp_v0_6 + (130)) + 1);
        var_a0 = *(u8 *)((unsigned char *)(D_8015EDB4 + sp3C) + (133));
    }
    if (var_a0 == 0) {
        if (!(D_800E7384 & 1)) {
            *(&D_80127080 + (arg0 * 0x10)) = 0x40000000;
            return;
        }
        *(&D_80127080 + (arg0 * 0x10)) = 0x80000000;
    }
}
