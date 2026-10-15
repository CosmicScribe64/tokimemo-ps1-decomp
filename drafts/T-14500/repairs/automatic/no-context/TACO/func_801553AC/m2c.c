? func_800AE120(s32);                               /* extern */
? func_8015ACCC(?, ?, s32, s32, ?, ?, ?, ?, s32, s32); /* extern */
extern void *D_8015EDB4;
extern s32 D_801604C0;
extern s32 D_801604CC;
extern ? D_801604D4;

void func_801553AC(void) {
    s16 sp54;
    s16 sp52;
    s16 sp50;
    s16 sp46;
    ? sp40;

    func_800AE120(D_801604C0);
    sp50 = *(s16 *)((unsigned char *)D_8015EDB4 + (116));
    if (*(s16 *)((unsigned char *)D_8015EDB4 + (116)) > 0) {
        sp52 = 1;
    } else {
        sp52 = -1;
    }
    sp54 = -0x2BC;
    sp46 = 0;
    if (((s32) D_801604CC % 3) == 0) {
        func_8015ACCC(4, 2, subroutine_arg2, subroutine_arg3, (unaligned s32) D_801604D4.unk0, (unaligned s32) *(? *)((unsigned char *)(&D_801604D4) + (4)), (unaligned s32) sp40.unk0, (unaligned s32) *(? *)((unsigned char *)(&sp40) + (4)), 6, 0xE0);
    } else {
        func_8015ACCC(4, 2, subroutine_arg2, subroutine_arg3, (unaligned s32) D_801604D4.unk0, (unaligned s32) *(? *)((unsigned char *)(&D_801604D4) + (4)), (unaligned s32) sp40.unk0, (unaligned s32) *(? *)((unsigned char *)(&sp40) + (4)), 6, 0x50);
    }
    sp54 = -0x384;
    sp46 = 0x14;
    func_8015ACCC(4, 2, subroutine_arg2, subroutine_arg3, (unaligned s32) D_801604D4.unk0, (unaligned s32) *(? *)((unsigned char *)(&D_801604D4) + (4)), (unaligned s32) sp40.unk0, (unaligned s32) *(? *)((unsigned char *)(&sp40) + (4)), 6, 0);
    sp54 = -0x1F4;
    sp46 = 0x14;
    func_8015ACCC(4, 2, subroutine_arg2, subroutine_arg3, (unaligned s32) D_801604D4.unk0, (unaligned s32) *(? *)((unsigned char *)(&D_801604D4) + (4)), (unaligned s32) sp40.unk0, (unaligned s32) *(? *)((unsigned char *)(&sp40) + (4)), 6, 0);
    sp54 = -0x44C;
    sp46 = 0x28;
    func_8015ACCC(4, 2, subroutine_arg2, subroutine_arg3, (unaligned s32) D_801604D4.unk0, (unaligned s32) *(? *)((unsigned char *)(&D_801604D4) + (4)), (unaligned s32) sp40.unk0, (unaligned s32) *(? *)((unsigned char *)(&sp40) + (4)), 6, 0);
    sp54 = -0x12C;
    sp46 = 0x28;
    func_8015ACCC(4, 2, subroutine_arg2, subroutine_arg3, (unaligned s32) D_801604D4.unk0, (unaligned s32) *(? *)((unsigned char *)(&D_801604D4) + (4)), (unaligned s32) sp40.unk0, (unaligned s32) *(? *)((unsigned char *)(&sp40) + (4)), 6, 0);
    D_801604CC += 1;
}
