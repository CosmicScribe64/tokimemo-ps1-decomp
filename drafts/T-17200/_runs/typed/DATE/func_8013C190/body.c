void func_8013C190(void) {
    s32 sp58;
    s32 sp54;
    DateTbl20 sp38;

    sp38 = D_8015B78C;
    func_80083440((s32) D_800CA21C[D_80122CDC]);
    func_8013C410((s16) (3 - D_800CA22C[D_80122CDC]), &sp58, &sp54);
    D_800E6280.unk_1BC[D_800E6280.unk_F5F] = (s32) (D_800E6280.unk_1BC[D_800E6280.unk_F5F] + ((s32) (((3 - D_800CA22C[D_80122CDC]) * sp58 * sp54) << 0x10) / 16));
    func_800AE0B0("zatu%d basho%d\n", sp58, sp54);
    func_8013C410((s16) (2 - D_800CA22C[D_80122CDC]), &sp58, &sp54);
    *(s32 *)((unsigned char *)(&D_800E6280.unk_1BC[D_800E6280.unk_F5F]) + (4)) = (s32) (*(s32 *)((unsigned char *)(&D_800E6280.unk_1BC[D_800E6280.unk_F5F]) + (4)) + ((s32) (((2 - D_800CA22C[D_80122CDC]) * sp58 * sp54) << 0x10) / 16));
    func_8013C410((s16) (2 - D_800CA22C[D_80122CDC]), &sp58, &sp54);
    *(s32 *)((unsigned char *)(&D_800E6280.unk_1BC[D_800E6280.unk_F5F]) + (8)) = (s32) (*(s32 *)((unsigned char *)(&D_800E6280.unk_1BC[D_800E6280.unk_F5F]) + (8)) + ((s32) ((sp38.w[D_800CA22C[D_80122CDC]] * sp58 * sp54) << 0x10) / 16));
    check_para_limit();
    func_8004284C();
}
