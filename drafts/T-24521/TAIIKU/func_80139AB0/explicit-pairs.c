/* FAKE: T-24521, experimental scalar/offset views retained only as source-only evidence; real aggregate fields unresolved. */
void func_80139AB0(void) {
    s32 i;
    s32 j;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 p;
    s32 q;
    s32 k;
    TaiHalves *pos;
    u16 *frames;
    u8 *marks;
    u8 *states;

    D_80149964 = 0;
    D_80149960 = 0;
    D_80149968.unk_1C[0] = 0;
    D_80149968.unk_1C[1] = 0x100;
    D_80149968.unk_14 = 0;
    *(s16 *)((unsigned char *)(&D_80149968) + (26)) = 0x80;
    *(s16 *)((unsigned char *)(&D_80149968) + (24)) = 0;
    D_80149968.unk_00 = 0;
    D_80149968.unk_04 = 0;
    D_80149968.unk_0C = 0;
    D_80149968.unk_10 = 0;
    D_80149940 = 0;
    D_80149948 = 0;
    D_8014994C = 0;
    D_801499C0.word = 0;
    D_80122EC4 = 0;
    D_80149944 = 1;
    D_801499E0 = 0;
    D_801499DC = 0;
    D_80149988.unk_00 = 0;
    D_80149988.unk_04 = 0;
    D_80149990.unk_04 = 0;
    D_80149990.unk_08 = 0;
    D_80149990.unk_00 = 1;
    D_80149988.unk_06 = 0;
    D_8014999C = 0;
    D_801499A0.unk_00 = 0;
    D_801499A0.unk_08 = 0;
    D_801499A0.unk_0C = 0;
    D_801499A0.unk_02 = 0;
    D_801499A0.unk_04 = -0x20;
    D_801499A0.unk_06 = 0x30;
    D_801499B0->unk0 = 0;
    D_801499B0->val = 0;
    func_800AE120(D_800E6280.unk_10F4);
    D_80149958 = 0;
    *(s16 *)((unsigned char *)(&D_80149958) + (2)) = 0;
    *(s16 *)((unsigned char *)(&D_80149958) + (4)) = 0;
    ((TaiikuCamera0C *)D_80122DD0)->x = 0x118;
    ((TaiikuCamera0C *)D_80122DD0)->y = -0x100;
    ((TaiikuCamera0C *)D_80122DD0)->angle = 0;
    pos = D_801499B4;
    frames = D_801499C4;
    marks = D_801499CC;
    do {
        *marks++ = 0;
        *frames++ = 0;
        pos->word = 0;
        pos++;
    } while (pos < &D_801499C0);
    states = D_801499D4;
    do {
        *states++ = 0xFF;
    } while (states < &D_801499D8);
    for (i = 0; i < 80; i++) {
        D_801499E4[i] = (i >> 2) * 8 - 24;
        D_80149A84[i] = (i >> 2) * 8 - 16;
    }
    i = 0;
    do {
        a = -160 - i * 8;
        b = -80 - i * 4;
        c = -168 - i * 8;
        d = -84 - i * 4;
        p = i * 4 + 80;
        q = (i + 1) * 4 + 80;
        j = 0;
        do {
            k = i * 4 + j;
            D_80149B24[k] = a + p * j;
            D_80149BC4[k] = b + p * j;
            D_80149C64[k] = c + q * j;
            D_80149D04[k] = d + q * j;
            D_80149B24[k + 1] = a + p * (j + 1);
            D_80149BC4[k + 1] = b + p * (j + 1);
            D_80149C64[k + 1] = c + q * (j + 1);
            D_80149D04[k + 1] = d + q * (j + 1);
            j += 2;
        } while (j != 4);
        i++;
    } while (i != 20);
}
