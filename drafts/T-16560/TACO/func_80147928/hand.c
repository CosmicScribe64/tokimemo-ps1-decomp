/* T-16560: hand clean-up of the redraft (named locals, byte reads, obj/pos declared first).
 * 32 funcdiff rows left: the stack home of the slot * 0x88 temporary (0x28, original 0x2C) and
 * the temporaries of the tail. Not in src. */
void func_80147928(s32 arg0, s32 arg1, s32 arg2, void *arg3, void *arg4, s32 arg5) {
    TcObj50 *obj;
    TcPos *pos;
    s32 slot;
    s32 base;
    u8 *p;

    slot = arg1 + 0x10;
    func_80143B34(slot);
    base = arg2 + 4;
    func_8009B3C0(base);
    func_8009B430(base + 8, (u8 *)&D_8015EDB4[slot].unk4, arg0 - 1, base);
    D_8015EDB4[slot].unk4.unk0 = 0x80000000;
    D_8015EDB4[slot].unk0[1] = 0x80;
    obj = &D_80127480[arg0];
    D_8015EDB4[slot].unk74 = obj->unk18;
    D_8015EDB4[slot].unk76 = obj->unk1C;
    D_8015EDB4[slot].unk78 = obj->unk20;
    pos = &D_80128880[arg0];
    D_8015EDB4[slot].unk64 = pos->x;
    D_8015EDB4[slot].unk66 = pos->y;
    D_8015EDB4[slot].unk68 = pos->z;
    p = (u8 *)arg4 + arg1 * 2;
    D_8015EDB4[slot].unk84[1] = p[0];
    D_8015EDB4[slot].unk7A = p[1];
    D_8015EDB4[slot].unk82 = 1;
    D_8015EDB4[slot].unk84[0] = ((u8 *)arg3)[arg1 * arg5];
    D_8015EDB4[slot].unk84[3] = 1;
    D_8015EDB4[slot].unk4.unk4[0] = (s32)&D_80127480[arg0];
}
