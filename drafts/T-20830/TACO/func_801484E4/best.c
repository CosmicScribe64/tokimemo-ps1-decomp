void func_801484E4(void) {
    s32 pad[2]; /* FAKE: reserves the 8 bytes of frame the original has below the spill slot; real local unknown. T-19013 */
    s32 t;
    u8 a;
    u8 b;
    s32 i;
    GameState *g = &D_800E6280;

    t = g->unk_1104.w;
    if (t < 0x78) {
        if (t % 60 == 0) {
            func_801335A0(0, 0x508);
        }
        func_80147E80(0);
    }
    if (t == 0x64) {
        for (i = 1; i < 0x1F; i++) D_80127080[i].unk0 = 0x80000000;
    }
    if (t < 0x78) {
        func_80153E78(1, 0xE0, 0xE0, 0xE0);
    } else if (t >= 0x78 && t < 0xB0) {
        a = 0x2C0 - t * 4;
        func_80153E78(1, a, a, a);
    } else if (t >= 0xB0 && t < 0xF0) {
        b = t * 4 - 0x2C0;
        func_80153E78(2, b, b, b);
    } else if (t == 0xF0) {
        func_80143730(0x80162000, 0xE);
        func_80143730(0x80172000, 0xC);
        func_80042908(0x15);
    }
    /* FAKE: the volatile access keeps this block out of the counter's register range (the original has plain ugen code here); real source unknown. T-19013 */
    *(volatile s32 *)&g->unk_1104.w += 1;
}
