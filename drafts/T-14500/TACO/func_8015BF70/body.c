void func_8015BF70(s32 arg0) {
    TcActor *a;
    u32 n;
    s32 i;
    u8 *id;
    u8 t;
    s32 *rec;

    a = &D_8015EDB4[arg0];
    n = 0;
    if (a->unk82 >= 0xF) {
        a->unk82 = 0;
        a = &D_8015EDB4[arg0];
    }
    t = a->unk84[2];
    id = D_80161D9C;
    i = 0;
    do {
        if (arg0 == *id) {
            rec = D_80161E60[t] + n * 2;
            func_8015B1B8(i, rec[0], rec[1], a->unk82);
            n++;
            a = &D_8015EDB4[arg0];
            t = a->unk84[2];
        }
        i++;
        if (n >= D_80161E20[t]) {
            break;
        }
        id++;
    } while (i != 0x40);
}
