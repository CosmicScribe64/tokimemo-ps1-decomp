void func_801553AC(void) {
    s16 sp54;
    s16 sp52;
    s16 sp50;
    s16 sp46;
    ? sp40;

    func_800AE120(D_801604C0);
    sp50 = D_8015EDB4->unk74;
    if (D_8015EDB4->unk74 > 0) {
        sp52 = 1;
    } else {
        sp52 = -1;
    }
    sp54 = -0x2BC;
    sp46 = 0;
    if (((s32) D_801604CC % 3) == 0) {
        func_8015ACCC(4, 2, /* TcPos+0x0 */ subroutine_arg2, /* TcPos+0x4 */ subroutine_arg3, /* TcPos+0x0 */ (unaligned s32) D_801604D4.unk0, /* TcPos+0x4 */ (unaligned s32) *(? *)((unsigned char *)(&D_801604D4) + (4)), /* TcPos+0x0 */ (unaligned s32) sp40.unk0, /* TcPos+0x4 */ (unaligned s32) *(? *)((unsigned char *)(&sp40) + (4)), 6, 0xE0);
    } else {
        func_8015ACCC(4, 2, /* TcPos+0x0 */ subroutine_arg2, /* TcPos+0x4 */ subroutine_arg3, /* TcPos+0x0 */ (unaligned s32) D_801604D4.unk0, /* TcPos+0x4 */ (unaligned s32) *(? *)((unsigned char *)(&D_801604D4) + (4)), /* TcPos+0x0 */ (unaligned s32) sp40.unk0, /* TcPos+0x4 */ (unaligned s32) *(? *)((unsigned char *)(&sp40) + (4)), 6, 0x50);
    }
    sp54 = -0x384;
    sp46 = 0x14;
    func_8015ACCC(4, 2, /* TcPos+0x0 */ subroutine_arg2, /* TcPos+0x4 */ subroutine_arg3, /* TcPos+0x0 */ (unaligned s32) D_801604D4.unk0, /* TcPos+0x4 */ (unaligned s32) *(? *)((unsigned char *)(&D_801604D4) + (4)), /* TcPos+0x0 */ (unaligned s32) sp40.unk0, /* TcPos+0x4 */ (unaligned s32) *(? *)((unsigned char *)(&sp40) + (4)), 6, 0);
    sp54 = -0x1F4;
    sp46 = 0x14;
    func_8015ACCC(4, 2, /* TcPos+0x0 */ subroutine_arg2, /* TcPos+0x4 */ subroutine_arg3, /* TcPos+0x0 */ (unaligned s32) D_801604D4.unk0, /* TcPos+0x4 */ (unaligned s32) *(? *)((unsigned char *)(&D_801604D4) + (4)), /* TcPos+0x0 */ (unaligned s32) sp40.unk0, /* TcPos+0x4 */ (unaligned s32) *(? *)((unsigned char *)(&sp40) + (4)), 6, 0);
    sp54 = -0x44C;
    sp46 = 0x28;
    func_8015ACCC(4, 2, /* TcPos+0x0 */ subroutine_arg2, /* TcPos+0x4 */ subroutine_arg3, /* TcPos+0x0 */ (unaligned s32) D_801604D4.unk0, /* TcPos+0x4 */ (unaligned s32) *(? *)((unsigned char *)(&D_801604D4) + (4)), /* TcPos+0x0 */ (unaligned s32) sp40.unk0, /* TcPos+0x4 */ (unaligned s32) *(? *)((unsigned char *)(&sp40) + (4)), 6, 0);
    sp54 = -0x12C;
    sp46 = 0x28;
    func_8015ACCC(4, 2, /* TcPos+0x0 */ subroutine_arg2, /* TcPos+0x4 */ subroutine_arg3, /* TcPos+0x0 */ (unaligned s32) D_801604D4.unk0, /* TcPos+0x4 */ (unaligned s32) *(? *)((unsigned char *)(&D_801604D4) + (4)), /* TcPos+0x0 */ (unaligned s32) sp40.unk0, /* TcPos+0x4 */ (unaligned s32) *(? *)((unsigned char *)(&sp40) + (4)), 6, 0);
    D_801604CC += 1;
}
