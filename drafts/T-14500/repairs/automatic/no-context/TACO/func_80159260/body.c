void func_80159260(void) {
    s16 sp5A;
    s16 sp58;
    s16 sp54;
    s16 sp52;
    s16 sp50;
    s32 sp44;
    u32 temp_hi;

    func_800AE120(D_801604C0);
    sp44 = func_800AE0D0();
    sp50 = ((func_800AE0D0() % 2500) + (sp44 % 1500)) - 0x7D0;
    sp44 = func_800AE0D0();
    sp52 = ((func_800AE0D0() % 2000) + (sp44 % 1000)) - 0x7D0;
    sp54 = (func_800AE0D0() % 5000) - 0x61A8;
    sp58 = (func_800AE0D0() % 200) - 0x64;
    sp5A = (func_800AE0D0() % 200) - 0x64;
    temp_hi = func_800AE0D0() % 5U;
    switch (temp_hi) {
    case 0:
        func_8015ACCC(2, 0x20, subroutine_arg2, subroutine_arg3, sp50, *(s32 *)((unsigned char *)(&sp50) + (4)), D_801604E4.unk0, *(s32 *)((unsigned char *)(&D_801604E4) + (4)), 4, 0x10);
        return;
    case 1:
        func_8015ACCC(2, 0x20, subroutine_arg2, subroutine_arg3, sp50, *(s32 *)((unsigned char *)(&sp50) + (4)), D_801604E4.unk0, *(s32 *)((unsigned char *)(&D_801604E4) + (4)), 4, 0x10);
        return;
    case 2:
        func_8015ACCC(2, 0x20, subroutine_arg2, subroutine_arg3, sp50, *(s32 *)((unsigned char *)(&sp50) + (4)), D_801604E4.unk0, *(s32 *)((unsigned char *)(&D_801604E4) + (4)), 4, 0);
        return;
    case 3:
        func_8015ACCC(2, 0x20, subroutine_arg2, subroutine_arg3, sp50, *(s32 *)((unsigned char *)(&sp50) + (4)), D_801604E4.unk0, *(s32 *)((unsigned char *)(&D_801604E4) + (4)), 4, 0);
        return;
    case 4:
        func_8015ACCC(2, 0x20, subroutine_arg2, subroutine_arg3, sp50, *(s32 *)((unsigned char *)(&sp50) + (4)), D_801604E4.unk0, *(s32 *)((unsigned char *)(&D_801604E4) + (4)), 4, 0);
        /* fallthrough */
    default:
        return;
    }
}
