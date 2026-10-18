/* T-24041 source-only baseline; aligned cost 512, not matching. */
void func_800455C4(void) {
    typedef struct CdLocation {
        u8 minute;
        u8 second;
        u8 sector;
        u8 track;
    } CdLocation; /* size 4; address consumed by the SDK position conversion/control calls. */
    extern u32 *D_801255BC;
    extern u16 D_800B3D64;
    void func_80046478();
    u32 sp48;
    s32 sp44;
    CdLocation position;
    u8 status[8];
    s32 sp2C;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_v1;
    u16 temp_t6_2;
    u32 temp_a1;
    u32 temp_v0;
    u8 temp_t0;
    u8 temp_t6;

    if (D_80125128 != D_80125129) {
        func_800463E8();
        if (D_8012512B == -1) {
            temp_t0 = D_80125130[D_80125128].cmd;
            switch (temp_t0) {
            case 0:
                if (func_80087954(1, status) != 0) {
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
                if (func_80087954(1, status) != 0) {
                    cd_read_status = 0;
                    D_801255B4 = 0;
                    if (func_800879D0(0xD, (s32) D_80125130[D_80125128].data, (s32) &D_801255D0) == 1) {
                        D_80125128 += 1;
                        D_8012512B = -1;
                    }
                }
                break;
            case 9:
                if (func_80087954(1, status) != 0) {
                    cd_read_status = 0;
                    D_801255B4 = 0;
                    if (func_800879D0(9, 0, (s32) &D_801255D0) == 1) {
                        D_8012512B = 1;
                    }
                }
                break;
            case 14:
                if (func_80087954(1, status) != 0) {
                    cd_read_status = 0;
                    D_801255B4 = 0;
                    if (func_800879D0(0xE, (s32) D_80125130[D_80125128].data, (s32) &D_801255D0) == 1) {
                        D_80125128 += 1;
                        D_8012512B = -1;
                    }
                }
                break;
            case 2:
                if (func_80087954(1, status) != 0) {
                    cd_read_status = 0;
                    D_801255B4 = 0;
                    func_80087EE8(D_80125130[D_80125128].arg, (s32) &position);
                    if (func_800879D0(2, (s32) &position, (s32) &D_801255D0) == 1) {
                        D_80125128 += 1;
                        D_8012512B = -1;
                    }
                }
                break;
            case 21:
                if (func_80087954(1, status) != 0) {
                    cd_read_status = 0;
                    D_801255B4 = 0;
                    func_80087EE8(D_80125130[D_80125128].arg, (s32) &position);
                    if (func_80087B1C(0x15, (s32) &position) == 1) {
                        D_8012512B = 1;
                    }
                }
                break;
            case 27:
                if (func_80087954(1, status) != 0) {
                    cd_read_status = 0;
                    D_801255B4 = 0;
                    func_800879A8(0);
                    func_80087E08(0);
                    func_80088070();
                    func_80087EE8(D_80125130[D_80125128].arg, (s32) &position);
                    while (func_80087C64(2, (s32) &position, 0) == 0) {
                    }
                    while (func_800880C0(0xC8) == 0) {
                    }
                    D_801255B4 = 0;
                    D_8012512B = 1;
                }
                break;
            case 10:
                cd_read_status = 0;
                D_801255B4 = 0;
                while (func_800879D0(0xA, 0, 0) == 0) {
                    }
                D_80125128 += 1;
                D_8012512B = -1;
                break;
            case 6:
                if (func_80087954(1, status) != 0) {
                    cd_read_status = 0;
                    D_801255B4 = 0;
                    temp_v0 = *(u32 *)D_80125130[D_80125128].data;
                    temp_v1 = temp_v0 & 0xFFFFFF;
                    temp_a1 = temp_v0 >> 0x18;
                    sp44 = temp_v1 + 0x80000000;
                    D_801255BC = (u32 *)(temp_v1 + (temp_a1 << 0xB) + 0x7FFFFFFC);
                    sp48 = temp_a1;
                    func_800879A8(0);
                    func_80087E08(0);
                    D_801255B8 = 0x84;
                    while (func_80087C64(0xE, (s32) &D_801255B8, 0) == 0) {
                    }
                    func_80087EE8(D_80125130[D_80125128].arg, (s32) &position);
                    while (func_80087C64(2, (s32) &position, 0) == 0) {
                    }
                    func_80088070();
                    while (func_80087E6C((s32) sp48, sp44, 0x80) == 0) {
                    }
                    D_8012512B = 1;
                }
                break;
            }
        } else if (D_8012512B == 1) {
            temp_t6 = D_80125130[D_80125128].cmd;
            switch (temp_t6) {
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
                    temp_v0_2 = func_80087E4C(1, D_801255C0);
                    D_800B3D68 = temp_v0_2;
                    if (temp_v0_2 <= 0) {
                        if (temp_v0_2 == 0) {
                            if (D_800B3D60 != 0) {
                                if ((*D_801255BC & 0xFFFF0000) != 0x10120000) {
                                    cd_read_status = 0;
                                    temp_t6_2 = D_800B3D64 + 1;
                                    D_800B3D64 = temp_t6_2;
                                    if ((temp_t6_2 & 3) == 3) {
                                        while (func_800879D0(0xA, 0, 0) == 0) {
                    }
                                        var_v1 = 0;
                                        do {
                                            sp2C = var_v1;
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
                        } else if (temp_v0_2 < 0) {
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
                    while (func_800879D0(1, 0, (s32) &D_801255D0) == 0) {
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
