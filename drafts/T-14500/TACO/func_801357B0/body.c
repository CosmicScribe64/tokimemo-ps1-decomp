void func_801357B0(arg0, arg1, arg2, arg3, arg4, arg5, arg6)
u8 *arg0;
s32 arg1;
s32 arg2;
s32 arg3;
s32 arg4;
s32 arg5;
s32 arg6;
{
    u8 *p;
    s32 i;
    s32 q;
    s32 j;

    p = arg0;
    i = j = 0;
    q = arg5;
    for (; i < 0x111 && *p != 0; i += 0xD) {
        func_80139424(*p, arg3, 3, 0x80, q, arg6, arg1, arg2, arg4);
        p++;
        q += 0xD;
    }
}
