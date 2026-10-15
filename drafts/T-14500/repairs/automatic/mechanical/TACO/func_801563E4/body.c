void func_801563E4(s32 arg0) {
    s16 sp5A;
    s16 sp58;
    s16 sp54;
    s16 sp52;
    s16 sp50;
    s16 sp4C;
    s16 sp4A;
    s16 sp48;
    s32 sp40;
    s32 temp_hi;
    s32 temp_hi_2;
    s32 temp_hi_3;
    s32 temp_hi_4;

    func_800AE120(D_801604C0);
    if (arg0 == 0) {
        sp40 = func_800AE0D0();
        sp50 = ((func_800AE0D0() % 4000) + D_8015EDB4->unk74 + (sp40 % 2000)) - 0xBB8;
        sp40 = func_800AE0D0();
        sp52 = ((func_800AE0D0() % 2500) + D_8015EDB4->unk76 + (sp40 % 1500)) - 0x7D0;
        sp54 = (func_800AE0D0() % 20000) + 0x8000;
        temp_hi = func_800AE0D0() % 30;
        sp4A = 0x800;
        sp48 = 0;
        sp4C = 0;
        sp58 = temp_hi + 0x28;
        sp5A = (s16) (func_800AE0D0() % 2);
        if (D_801604C0 < 0xF0) {
            func_8015ACCC(7, 7, /* TcPos+0x0 */ subroutine_arg2, /* TcPos+0x4 */ subroutine_arg3, /* TcPos+0x0 */ sp50, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&sp50) + (4)), /* TcPos+0x0 */ sp48, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&sp48) + (4)), 5, 0x50);
            return;
        }
        func_8015ACCC(7, 7, /* TcPos+0x0 */ subroutine_arg2, /* TcPos+0x4 */ subroutine_arg3, /* TcPos+0x0 */ sp50, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&sp50) + (4)), /* TcPos+0x0 */ sp48, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&sp48) + (4)), 5, 0);
        return;
    }
    sp52 = -0x384;
    sp54 = -0x6590;
    sp4A = 0x800;
    sp48 = 0;
    sp4C = 0;
    temp_hi_2 = func_800AE0D0() % 20;
    sp50 = 0;
    sp58 = temp_hi_2 + 0x1E;
    sp5A = (s16) (func_800AE0D0() % 2);
    D_8015EDB4[func_8015ACCC(7, 7, /* TcPos+0x0 */ subroutine_arg2, /* TcPos+0x4 */ subroutine_arg3, /* TcPos+0x0 */ sp50.unk0, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&sp50) + (4)), /* TcPos+0x0 */ sp48.unk0, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&sp48) + (4)), 0x14, 0)].unk84[0] = 4;
    temp_hi_3 = func_800AE0D0() % 20;
    sp50 = -0x3E8;
    sp58 = temp_hi_3 + 0x1E;
    sp5A = (s16) (func_800AE0D0() % 2);
    D_8015EDB4[func_8015ACCC(7, 7, /* TcPos+0x0 */ subroutine_arg2, /* TcPos+0x4 */ subroutine_arg3, /* TcPos+0x0 */ sp50.unk0, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&sp50) + (4)), /* TcPos+0x0 */ sp48.unk0, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&sp48) + (4)), 0x14, 0)].unk84[0] = 4;
    temp_hi_4 = func_800AE0D0() % 20;
    sp50 = 0x3E8;
    sp58 = temp_hi_4 + 0x1E;
    sp5A = (s16) (func_800AE0D0() % 2);
    D_8015EDB4[func_8015ACCC(7, 7, /* TcPos+0x0 */ subroutine_arg2, /* TcPos+0x4 */ subroutine_arg3, /* TcPos+0x0 */ sp50.unk0, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&sp50) + (4)), /* TcPos+0x0 */ sp48.unk0, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&sp48) + (4)), 0x14, 0)].unk84[0] = 4;
}
