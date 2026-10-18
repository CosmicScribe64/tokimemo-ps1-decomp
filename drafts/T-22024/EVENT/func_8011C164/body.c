s32 func_8011C164(void) {
    s16 i;
    s32 cur;
    GsWord *pos;
    s16 size_variant;
    s32 coin;
    s32 scale;
    s32 step;
    s32 random_span;
    s32 divisor;

    if (D_8012531C == 1) {
        return 0;
    }
    i = 0;
    if ((D_8012531C != 2) && (((u8) D_800B0896 < 9U) || ((u8) D_800B0896 >= 0xCU))) {
        return 0;
    }
    do {
        pos = &D_80125868[i];
        cur = ((DisplayFixActor44 *) D_800E9620)[i + 98].unk_24.w;
        ((DisplayFixActor44 *) D_800E9620)[i + 98].unk_03 |= 0x80;
        ((DisplayFixActor44 *) D_800E9620)[i + 98].unk_28.w += ((DisplayFixActor44 *) D_800E9620)[i + 98].unk_30;
        step = ((DisplayFixActor44 *) D_800E9620)[i + 98].unk_2C + ((pos->w - cur) >> 8);
        ((DisplayFixActor44 *) D_800E9620)[i + 98].unk_2C = step;
        ((DisplayFixActor44 *) D_800E9620)[i + 98].unk_24.w = cur + step;
        if (((DisplayFixActor44 *) D_800E9620)[i + 98].unk_28.h[1] >= 0x29) {
            ((DisplayFixActor44 *) D_800E9620)[i + 98].unk_28.h[1] = -0x78;
            step = func_800789A0() % 2;
            scale = 0x1000 / ((s16) step * 2 + 2);
            size_variant = (s16) step;
            ((DisplayFixActor44 *) D_800E9620)[i + 98].unk_07 = 0x80;
            ((DisplayFixActor44 *) D_800E9620)[i + 98].unk_1A = (s16) scale;
            ((DisplayFixActor44 *) D_800E9620)[i + 98].unk_1C = (s16) scale;
            ((DisplayFixActor44 *) D_800E9620)[i + 98].unk_24.h[1] = (func_800789A0() % 300) - 0x96;
            pos->w = ((DisplayFixActor44 *) D_800E9620)[i + 98].unk_24.w;
            random_span = func_800789A0();
            coin = func_800789A0();
            divisor = size_variant + 1;
            pos->h[1] = (s16) pos->h[1] + (((((coin % 2) * 2) - 1) * (random_span % 8) + 8) / divisor);
            ((DisplayFixActor44 *) D_800E9620)[i + 98].unk_30 = 0x8000 / divisor;
        }
        i += 1;
    } while (i < 0x14);
}
