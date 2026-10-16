void func_801553AC(void) {
    TcPos sp50;
    TcPos sp40;

    func_800AE120(D_801604C0);
    sp50.x = D_8015EDB4->unk74;
    if (D_8015EDB4->unk74 > 0) {
        sp50.y = 1;
    } else {
        sp50.y = -1;
    }
    sp50.z = -0x2BC;
    sp40.unk6 = 0;
    if (((s32) D_801604CC % 3) == 0) {
        func_8015ACCC(4, 2, sp50, D_801604D4, sp40, 6, 0xE0);
    } else {
        func_8015ACCC(4, 2, /* TcPos+0x0 */ subroutine_arg2, /* TcPos+0x4 */ subroutine_arg3, /* TcPos+0x0 */ D_801604D4.unk0, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&D_801604D4) + (4)), /* TcPos+0x0 */ (*(s32 *)&sp40), /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&sp40) + (4)), 6, 0x50);
    }
    sp50.z = -0x384;
    sp40.unk6 = 0x14;
    func_8015ACCC(4, 2, sp50, D_801604D4, sp40, 6, 0);
    sp50.z = -0x1F4;
    sp40.unk6 = 0x14;
    func_8015ACCC(4, 2, sp50, D_801604D4, sp40, 6, 0);
    sp50.z = -0x44C;
    sp40.unk6 = 0x28;
    func_8015ACCC(4, 2, sp50, D_801604D4, sp40, 6, 0);
    sp50.z = -0x12C;
    sp40.unk6 = 0x28;
    func_8015ACCC(4, 2, sp50, D_801604D4, sp40, 6, 0);
    D_801604CC += 1;
}
