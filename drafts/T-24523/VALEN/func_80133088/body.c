/* Source-only checkpoint, NOT MATCHED: aligned edit cost 14. T-24523. */
extern s32 D_80122D48;

void func_80133088(void) {
    /* FAKE: selector and pointer phases do not overlap; sharing their word keeps
     * the text id at the original spill offset. T-24523 */
    union {
        u32 mode;
        GameState *state;
    } phase;
    s32 id;

    phase.mode = D_800E6280.unk_10F8 & 7;
    if (phase.mode == 0) {
        phase.mode = 3;
    } else if (phase.mode < 4) {
        phase.mode = 0;
    } else {
        phase.mode = 1;
    }
    if (D_800E6280.unk_F5F == 0) {
        /* FAKE: this redundant narrowing retains the original move/mask sequence. T-24523 */
        phase.mode = (u8)D_800E6280.unk_10F8;
        phase.mode = phase.mode & 1;
    }
    /* FAKE: forward labels retain the original location-block ordering. T-24523 */
    if (phase.mode == 0) goto location_zero;
    if (phase.mode == 1) goto location_one;
    D_80134548 = phase.mode;
    if (phase.mode == 2) goto location_two;
    goto location_other;
location_zero:
    D_80134548 = phase.mode;
    func_800AE0F0(D_800CA1DC, "廊下");
    id = 0x4045;
    goto location_ready;
location_one:
    D_80134548 = phase.mode;
    func_800AE0F0(D_800CA1DC, "教室");
    id = 0x4055;
    goto location_ready;
location_two:
    func_800AE0F0(D_800CA1DC, "校舎裏");
    id = 0x407A;
    goto location_ready;
location_other:
    func_800AE0F0(D_800CA1DC, "屋上");
    id = 0x4084;
location_ready:
    phase.state = &D_800E6280;
    if (phase.state->unk_F5F != 0) {
        if (D_80134520 + 1 == D_8013453C) {
            func_800AE0F0(D_800CA1DC, "校門前");
            id = 0x4032;
        }
        if ((D_80134520 + 2 == D_8013453C) && (D_80134524[D_80134520 + 1] == 0)) {
            id = 0x4032;
            func_800AE0F0(D_800CA1DC, "校門前");
        }
    }
    if ((D_80134520 + 1 == D_8013453C) && (phase.state->unk_F5F == 0) && ((phase.state->unk_1BC[0].unk_02 >= 0x46) || (D_80134544 != 0))) {
        func_800AE0F0(D_800CA1DC, "自宅前");
        id = 0x414C;
    }
    if ((phase.state->unk_F5F == 9) && (D_80134544 == 1)) {
        id = 0x4055;
        func_800AE0F0(D_800CA1DC, "教室");
    }
    if (phase.state->unk_F5F == 0) {
        if (phase.state->unk_1BC[0].unk_02 >= 0x46) {
            D_80122D48 = 1;
        } else {
            D_80122D48 = 2;
        }
    } else {
        if (get_h_tokimeki(phase.state->unk_F5F) >= 0x46U) {
            D_80122D48 = 1;
        } else {
            D_80122D48 = 2;
        }
    }
    switch (id) {
    case 0x4045:
        func_80085B3C(6, 0);
        break;
    case 0x4055:
        func_80085B3C(6, 1);
        break;
    case 0x414C:
        func_80085B3C(6, 5);
        break;
    case 0x4032:
        func_80085B3C(6, 4);
        break;
    case 0x407A:
        func_80085B3C(6, 3);
        break;
    default:
        func_80085B3C(6, 2);
        break;
    }
    if ((D_800E6280.unk_F5F == 9) && (D_80134544 == 1)) {
        D_800E6280.unk_F5E -= 1;
        D_80122D48 = 1;
        func_80085B3C(6, 3);
    }
    func_8007ED84(id);
    if (get_h_tokimeki(D_800E6280.unk_F5F) >= 0x46U) {
        func_80083440(0);
    } else {
        func_80083440(1);
    }
    if (D_800E6280.unk_F5F == 0) {
        if (D_800E6280.unk_1BC[0].unk_02 >= 0x46) {
            func_80083440(0);
        } else {
            func_80083440(1);
        }
    }
    func_8004284C();
}
