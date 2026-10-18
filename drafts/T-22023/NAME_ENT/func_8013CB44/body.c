/* T-22023: 60 diff lines (about 28 rows): register-only. In case 2 the original keeps D_800E7D36 in a ugen temp ($t2) where this draft promotes it to $v0, which shifts every later temporary by one. Loop shape, pad frame and g-> access match. */
void func_8013CB44(void) {
    s32 pad[2]; /* FAKE: unused words, the original frame has them. T-22023 */
    s32 i;
    s32 slot;
    GameState *g = &D_800E6280;

    switch (g->unk_110D) {
    case 0:
        func_80053CE0();
        g->unk_110D++;
        break;
    case 1:
        if (func_80054284() == 3) {
            func_80053D10();
            g->unk_110D++;
        } else if (((u32) g->unk_1120 >= 0x201U) || (g->unk_1115 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 2:
        if (func_800552DC(0) == -1) {
            g->unk_110D++;
        } else {
            func_8004E58C();
            func_8013BB10();
            slot = 0xFF;
            if (D_800E7D14[D_800E7D36] == 1) {
                slot = D_800E7D36 - 1;
            } else {
                for (i = 1; i < 14; i++) {
                    if (D_800E7D10.slot_status[i] == 1) {
                        slot = i - 1;
                        D_800E7D36 = i;
                        break;
                    }
                }
            }
            if (slot != 0xFF) {
                func_8013B51C(slot);
            }
            func_80139CB4();
            func_8004EA98();
            func_8004EAD4(0);
            D_8014D0D4 = 1;
            func_8004284C();
        }
        break;
    case 3:
        func_8004E58C();
        func_8004E788(-0x40, 0x20, 0xF, (s32) "システムファイルは", 0);
        func_8004E788(-0x40, 0x30, 0xF, (s32) "壊れています。", 0);
        func_8004E884(2);
        func_80053D10();
        g->unk_110D++;
        break;
    case 4:
        if (g->unk_F88 & 0x60) {
            g->unk_110D++;
        }
        break;
    case 5:
        func_8004E58C();
        func_8004E788(-0x40, 0x20, 0xF, (s32) "システムファイルを", 0);
        func_8004E788(-0x40, 0x30, 0xF, (s32) "作り直しますか？", 0);
        func_8004E788(-0x40, 0x40, 0xF, (s32) D_800B3C70, 0);
        func_8004E788(0x14, 0x40, 1, (s32) func_8006CA9C(), 0);
        func_8004E788(-0x40, 0x50, 0xF, (s32) D_800B3C88, 0);
        func_8004E788(0x14, 0x50, 2, (s32) func_8006CAE0(), 0);
        func_8004E884(6);
        g->unk_110D++;
        break;
    case 6:
        if (g->unk_F88 & 0x20) {
            g->unk_110D++;
        } else if (g->unk_F88 & 0x40) {
            for (i = 0; i < 4; i++) {
                if (func_80054704(0) == 1) {
                    break;
                }
            }
            func_80042908(1);
        }
        break;
    case 7:
        func_80042908(0xD0);
        break;
    }
}
