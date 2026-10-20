/* T-24501 draft: full-TU resolved edit cost439. Correct Ring18 fields, eight-byte
 * CD status buffer and pointer argument avoid stale m2c casts. Still differs
 * structurally by one instruction and many register/scheduling rows. These
 * scratch-only declarations need canonical header review before production. */
extern u32 *D_801255BC;
extern u16 D_800B3D64;
extern s32 D_800B3D68;
void func_80046478();
void func_800455C4(void) {
    u32 sp48;
    s32 sp44;
    s32 var_v1;
    u16 temp_t6_2;
    s32 pad; /* FAKE: measured frame home, not a source recovery. T-24501 */
    s32 sp38[1];
    u8 sp30[8];

    if (D_80125128 != D_80125129) {
        func_800463E8();
        if (D_8012512B == -1) {
            switch (D_80125130[D_80125128].cmd) {
            case 0:
                if (func_80087954(1, &sp30) != 0) {
                    D_801255B4 = 0;
                    if (func_800879D0(9, 0, (s32) &D_801255D0) == 1) {
                        D_8012512B = 1;
                    }
                }
                break;
            case 3:
                func_8008B750(0x7F, 0x7F);
                cd_read_status = 0;
                D_801255B4 = 0;
                if (func_800879D0(3, 0, 0) == 1) {
                    D_80125128 += 1;
                    D_8012512B = -1;
                }
                break;
            case 13:
                if (func_80087954(1, &sp30) != 0) {
                    cd_read_status = 0;
                    D_801255B4 = 0;
                    if (func_800879D0(0xD, (s32) D_80125130[D_80125128].data, (s32) &D_801255D0) == 1) {
                        D_80125128 += 1;
                        D_8012512B = -1;
                    }
                }
                break;
            case 9:
                if (func_80087954(1, &sp30) != 0) {
                    cd_read_status = 0;
                    D_801255B4 = 0;
                    if (func_800879D0(9, 0, (s32) &D_801255D0) == 1) {
                        D_8012512B = 1;
                    }
                }
                break;
            case 14:
                if (func_80087954(1, &sp30) != 0) {
                    cd_read_status = 0;
                    D_801255B4 = 0;
                    if (func_800879D0(0xE, (s32) D_80125130[D_80125128].data, (s32) &D_801255D0) == 1) {
                        D_80125128 += 1;
                        D_8012512B = -1;
                    }
                }
                break;
            case 2:
                if (func_80087954(1, &sp30) != 0) {
                    cd_read_status = 0;
                    D_801255B4 = 0;
                    func_80087EE8(D_80125130[D_80125128].arg, sp38);
                    if (func_800879D0(2, sp38, (s32) &D_801255D0) == 1) {
                        D_80125128 += 1;
                        D_8012512B = -1;
                    }
                }
                break;
            case 21:
                if (func_80087954(1, &sp30) != 0) {
                    cd_read_status = 0;
                    D_801255B4 = 0;
                    func_80087EE8(D_80125130[D_80125128].arg, sp38);
                    if (func_80087B1C(0x15, sp38) == 1) {
                        D_8012512B = 1;
                    }
                }
                break;
            case 27:
                if (func_80087954(1, &sp30) != 0) {
                    cd_read_status = 0;
                    D_801255B4 = 0;
                    func_800879A8(0);
                    func_80087E08(0);
                    func_80088070();
                    func_80087EE8(D_80125130[D_80125128].arg, sp38);
                    if (func_80087C64(2, sp38, 0) == 0) {
                        do {

                        } while (func_80087C64(2, sp38, 0) == 0);
                    }
                    if (func_800880C0(0xC8) == 0) {
                        do {

                        } while (func_800880C0(0xC8) == 0);
                    }
                    D_801255B4 = 0;
                    D_8012512B = 1;
                }
                break;
            case 10:
                cd_read_status = 0;
                D_801255B4 = 0;
                if (func_800879D0(0xA, 0, 0) == 0) {
                    do {

                    } while (func_800879D0(0xA, 0, 0) == 0);
                }
                D_80125128 += 1;
                D_8012512B = -1;
                break;
            case 6:
                if (func_80087954(1, &sp30) != 0) {
                    cd_read_status = 0;
                    D_801255B4 = 0;
                    sp44 = (*(u32 *)D_80125130[D_80125128].data & 0xFFFFFF) + 0x80000000;
                    sp48 = *(u32 *)D_80125130[D_80125128].data >> 24;
                    D_801255BC = (u32 *)(sp44 + (sp48 << 11) - 4);
                    func_800879A8(0);
                    func_80087E08(0);
                    D_801255B8 = 0x84;
                    if (func_80087C64(0xE, (s32) &D_801255B8, 0) == 0) {
                        do {

                        } while (func_80087C64(0xE, (s32) &D_801255B8, 0) == 0);
                    }
                    func_80087EE8(D_80125130[D_80125128].arg, sp38);
                    if (func_80087C64(2, sp38, 0) == 0) {
                        do {

                        } while (func_80087C64(2, sp38, 0) == 0);
                    }
                    func_80088070(2);
                    if (func_80087E6C((s32) sp48, sp44, 0x80) == 0) {
                        do {

                        } while (func_80087E6C((s32) sp48, sp44, 0x80) == 0);
                    }
                    D_8012512B = 1;
                }
                break;
            }
        } else if (D_8012512B == 1) {
            switch (D_80125130[D_80125128].cmd) {
            case 0:
                if ((func_80087954(1, D_801255D0.bytes) != 0) && (func_800879D0(1, 0, (s32) &D_801255D0) == 1) && !(D_801255D0.bytes[0] & 0x40)) {
                    D_801255B4 = 2;
                    D_80125128 += 1;
                    D_8012512B = -1;
                }
                break;
            case 9:
                if ((func_80087954(1, D_801255D0.bytes) != 0) && (func_800879D0(1, 0, (s32) &D_801255D0) == 1) && !(D_801255D0.bytes[0] & 0x40)) {
                    D_801255B4 = 2;
                    D_80125128 += 1;
                    D_8012512B = -1;
                }
                break;
            case 21:
                if ((func_800879D0(1, 0, (s32) &D_801255D0) == 1) && !(D_801255D0.bytes[0] & 0x40)) {
                    D_801255B4 = 2;
                    D_80125128 += 1;
                    D_8012512B = -1;
                }
                break;
            case 6:
                if (func_80087954(1, D_801255D0.bytes) != 0) {
                    D_800B3D68 = func_80087E4C(1, D_801255C0);
                    if (D_800B3D68 <= 0) {
                        if (D_800B3D68 == 0) {
                            if (D_800B3D60 != 0) {
                                if ((*D_801255BC & 0xFFFF0000) != 0x10120000) {
                                    cd_read_status = 0;
                                    temp_t6_2 = D_800B3D64 + 1;
                                    D_800B3D64 = temp_t6_2;
                                    if ((temp_t6_2 & 3) == 3) {
                                        if (func_800879D0(0xA, 0, 0) == 0) {
                                            do {

                                            } while (func_800879D0(0xA, 0, 0) == 0);
                                        }
                                        var_v1 = 0;
                                        do {
                                            func_800AD950(0);
                                            var_v1 += 1;
                                        } while (var_v1 != 0x200);
                                    }
                                    D_8012512B = -1;
                                } else {
                                    D_80125128 += 1;
                                    D_801255B4 = 2;
                                    cd_read_status |= 1;
                                    D_801255B5 += 1;
                                    D_8012512B = -1;
                                }
                            } else {
                                D_80125128 += 1;
                                D_801255B4 = 2;
                                cd_read_status |= 1;
                                D_801255B5 += 1;
                                D_8012512B = -1;
                            }
                        } else if (D_800B3D68 < 0) {
                            D_801255B4 = 0;
                            cd_read_status = 0;
                            D_8012512B = -1;
                        }
                    }
                }
                break;
            case 27:
                if (func_80087954(1, D_801255D0.bytes) == 5) {
                    D_8012512B = -1;
                    func_80046478(1);
                } else {
                    if (func_800879D0(1, 0, (s32) &D_801255D0) == 0) {
                        do {

                        } while (func_800879D0(1, 0, (s32) &D_801255D0) == 0);
                    }
                    if (D_801255D0.bytes[0] & 0x20) {
                        cd_read_status = 0;
                        D_8012512B = -1;
                        if (D_80125130[D_80125128].data[15] != 0xFF) {
                            D_801255B4 = 4;
                        }
                        D_80125128 += 1;
                    }
                }
                break;
            }
        }
    } else if (func_80087954(1, 0) == 5) {
        if (func_80046094() == 0x1B) {
            D_8012512B = -1;
            if (D_80125128 != 0) {
                D_80125128 -= 1;
            } else {
                D_80125128 = 0x2F;
            }
            func_80046478(1);
        } else {
            func_800879D0(1, 0, 0);
        }
    } else if (D_801255B4 == 4) {
        D_801255B0 = func_800460FC();
    }
    if ((u8) D_80125128 >= 0x30U) {
        D_80125128 = 0;
    }
}
