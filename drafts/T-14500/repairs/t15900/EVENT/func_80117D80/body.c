void func_80117D80(void) {
    EventStateView *g;
    s16 sp2A;
    s16 sp28;

    g = (EventStateView *)D_800B0860;

    /* FAKE: discarded entry assignment suppresses compare CVT rewriting;
     * otherwise the first branch operands are exchanged. T-14300 */
    sp2A = D_8012531C;
    D_8009471C = (u8 *)&sp2A;
    D_80094724 = D_801241D8;
    D_80094728 = D_801241E8;
    sp2A = 0;
    D_8009472C = D_801241F8;
    if ((D_800EECBC == 0) && ((*(s16 *)((u8 *)g + 0xFE)) < 0x28) && (D_8012531C == 1) && (D_801243B0 == 0x10) && ((g->unk_EE6 == 0) || (g->unk_EE6 == 1) || (g->unk_EE6 == 3) || (g->unk_EE6 == 4) || (g->unk_EE6 == 5) || (g->unk_EE6 == 8) || (g->unk_EE6 == 9))) {
        D_8012439C = 3;
        D_8012438C = 3;
        D_80124394 = 3;
        D_801243A4 = 2;
        func_8004A8EC(3);
        func_80033E88();
        sp28 = 5;
        D_80094720 = (u8 *) &sp28;
        if (func_80049B20(D_800EECBC, 0, 0) != 0) {
            func_80011DFC();
        }
    } else if (((D_800EECBC == 0) && ((*(s16 *)((u8 *)g + 0xFE)) < 0x28) && (D_8012531C == 0) && (D_801243B0 == 0x13) &&
                ((g->unk_EE6 == 0) || (g->unk_EE6 == 1) || (g->unk_EE6 == 3) || (g->unk_EE6 == 6) || (g->unk_EE6 == 7))) ||
               ((D_800EECBC == 0) && ((*(s16 *)((u8 *)g + 0xFE)) < 0x64) && (D_8012531C == 0) && (D_801243B0 == 0x14) &&
                ((g->unk_EE6 == 0) || (g->unk_EE6 == 4) || (g->unk_EE6 == 6) || (g->unk_EE6 == 7) || (g->unk_EE6 == 8) || (g->unk_EE6 == 0xA)))) {
        D_8012439C = 3;
        D_8012438C = 3;
        D_80124394 = 3;
        D_801243A4 = 2;
        func_8004A8EC(3);
        func_80033E88();
        sp28 = 6;
        D_80094720 = (u8 *) &sp28;
        if (func_80049B20(D_800EECBC, 0, 0) != 0) {
            func_80011DFC();
        }
    } else {
        func_80011DFC();
    }
}
