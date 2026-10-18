/* T-24050 source-only typed selection draft: 80 edits, not a C match. */
s32 func_80134808(void) {
    u8 best = 0xFF;
    u32 bestState = 4;
    s16 i;
    u8 state;
    u32 friendship;
    u32 affection;
    u32 bestAffection;

    for (i = 0; i < 11; i++) {
        /* FAKE: byte-offset spelling avoids promotion of a shared GameState base (T-24050). */
        if (!((Rec38 *)((u8 *)D_800E6280.unk_1BC + i * sizeof(Rec38)))->unk_0C.f.b1) {
            continue;
        }
        if ((D_800E6280.unk_71E & 0x80) && i + 1 == D_80122D40) {
            continue;
        }
        state = get_g_zyotai_s(i) & 0x7F;
        if (state < bestState) {
            best = i;
            bestState = state;
        } else if (state == bestState) {
            /* Retain the target's duplicate tie guard; original source rationale is unknown. */
            if (state == bestState) {
                friendship = get_h_yuukou(i) & 0xFF;
                affection = get_h_tokimeki(i) & 0xFF;
                bestAffection = get_h_tokimeki(best) & 0xFF;
                if (get_h_yuukou(best) + bestAffection < affection + friendship) {
                    best = i;
                    bestState = state;
                }
            }
        }
    }
    state = get_g_zyotai_s(best) & 0x7F;
    D_80122D40 = 0;
    if (state >= 2 || best == 0xFF || (D_800E6280.unk_71E & 8)) {
        func_80042940(0x17);
        return 0;
    }
    D_800E6280.unk_F5F = best;
    func_800847B8(best);
    func_80134F84();
    func_80134A14();
    return func_8004284C();
}
