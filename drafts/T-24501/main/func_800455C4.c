#define MAIN_API_OVERRIDE_func_800AD950 /* SDK return-value diagnostic */
typedef struct { u8 head; u8 tail; u8 status; s8 phase; u8 pad[4]; } CdRingView;
s32 func_800AD950();
/* T-24501 diagnostic: resolved edit cost 26, 691 target instructions.
 * The SDK wait function returns s32; guard must precede all TU includes.
 * Queue header layout recovers alias-dependent scheduling; command array
 * stays canonical to preserve relocation addends. Not production-ready. */
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

    if (((CdRingView *)&D_80125128)->tail != ((CdRingView *)&D_80125128)->head) {
        func_800463E8();
        if (((CdRingView *)&D_80125128)->phase == -1) {
            switch (D_80125130[((CdRingView *)&D_80125128)->head].cmd) {
            case 0:
                if (func_80087954(1, &sp30) != 0) {
                    D_801255B4 = 0;
                    if (func_800879D0(9, 0, &D_801255D0) == 1) {
                        ((CdRingView *)&D_80125128)->phase = 1;
                    }
                }
                break;
            case 3:
                func_8008B750(0x7F, 0x7F);
                ((CdRingView *)&D_80125128)->status = 0;
                D_801255B4 = 0;
                if (func_800879D0(3, 0, 0) == 1) {
                    ((CdRingView *)&D_80125128)->phase = -1;
                    ((CdRingView *)&D_80125128)->head += 1;
                }
                break;
            case 13:
                if (func_80087954(1, &sp30) != 0) {
                    ((CdRingView *)&D_80125128)->status = 0;
                    D_801255B4 = 0;
                    if (func_800879D0(0xD, D_80125130[((CdRingView *)&D_80125128)->head].data, &D_801255D0) == 1) {
                        ((CdRingView *)&D_80125128)->phase = -1;
                        ((CdRingView *)&D_80125128)->head += 1;
                    }
                }
                break;
            case 9:
                if (func_80087954(1, &sp30) != 0) {
                    ((CdRingView *)&D_80125128)->status = 0;
                    D_801255B4 = 0;
                    if (func_800879D0(9, 0, &D_801255D0) == 1) {
                        ((CdRingView *)&D_80125128)->phase = 1;
                    }
                }
                break;
            case 14:
                if (func_80087954(1, &sp30) != 0) {
                    ((CdRingView *)&D_80125128)->status = 0;
                    D_801255B4 = 0;
                    if (func_800879D0(0xE, D_80125130[((CdRingView *)&D_80125128)->head].data, &D_801255D0) == 1) {
                        ((CdRingView *)&D_80125128)->phase = -1;
                        ((CdRingView *)&D_80125128)->head += 1;
                    }
                }
                break;
            case 2:
                if (func_80087954(1, &sp30) != 0) {
                    ((CdRingView *)&D_80125128)->status = 0;
                    D_801255B4 = 0;
                    func_80087EE8(D_80125130[((CdRingView *)&D_80125128)->head].arg, sp38);
                    if (func_800879D0(2, sp38, &D_801255D0) == 1) {
                        ((CdRingView *)&D_80125128)->phase = -1;
                        ((CdRingView *)&D_80125128)->head += 1;
                    }
                }
                break;
            case 21:
                if (func_80087954(1, &sp30) != 0) {
                    ((CdRingView *)&D_80125128)->status = 0;
                    D_801255B4 = 0;
                    func_80087EE8(D_80125130[((CdRingView *)&D_80125128)->head].arg, sp38);
                    if (func_80087B1C(0x15, sp38) == 1) {
                        ((CdRingView *)&D_80125128)->phase = 1;
                    }
                }
                break;
            case 27:
                if (func_80087954(1, &sp30) != 0) {
                    ((CdRingView *)&D_80125128)->status = 0;
                    D_801255B4 = 0;
                    func_800879A8(0);
                    func_80087E08(0);
                    func_80088070();
                    func_80087EE8(D_80125130[((CdRingView *)&D_80125128)->head].arg, sp38);
                    if (func_80087C64(2, sp38, 0) == 0) {
                        do {

                        } while (func_80087C64(2, sp38, 0) == 0);
                    }
                    if (func_800880C0(0xC8) == 0) {
                        do {

                        } while (func_800880C0(0xC8) == 0);
                    }
                    D_801255B4 = 0;
                    ((CdRingView *)&D_80125128)->phase = 1;
                }
                break;
            case 10:
                ((CdRingView *)&D_80125128)->status = 0;
                D_801255B4 = 0;
                if (func_800879D0(0xA, 0, 0) == 0) {
                    do {

                    } while (func_800879D0(0xA, 0, 0) == 0);
                }
                ((CdRingView *)&D_80125128)->phase = -1;
                ((CdRingView *)&D_80125128)->head += 1;
                break;
            case 6:
                if (func_80087954(1, &sp30) != 0) {
                    ((CdRingView *)&D_80125128)->status = 0;
                    D_801255B4 = 0;
                    sp44 = (*(u32 *)D_80125130[((CdRingView *)&D_80125128)->head].data & 0xFFFFFF) + 0x80000000;
                    sp48 = *(u32 *)D_80125130[((CdRingView *)&D_80125128)->head].data >> 24;
                    D_801255BC = (u32 *)((*(u32 *)D_80125130[((CdRingView *)&D_80125128)->head].data & 0xFFFFFF) + (sp48 << 11) + 0x7FFFFFFC);
                    func_800879A8(0);
                    func_80087E08(0);
                    D_801255B8 = 0x84;
                    if (func_80087C64(0xE, &D_801255B8, 0) == 0) {
                        do {

                        } while (func_80087C64(0xE, &D_801255B8, 0) == 0);
                    }
                    func_80087EE8(D_80125130[((CdRingView *)&D_80125128)->head].arg, sp38);
                    if (func_80087C64(2, sp38, 0) == 0) {
                        do {

                        } while (func_80087C64(2, sp38, 0) == 0);
                    }
                    func_80088070();
                    if (func_80087E6C((s32) sp48, sp44, 0x80) == 0) {
                        do {

                        } while (func_80087E6C((s32) sp48, sp44, 0x80) == 0);
                    }
                    ((CdRingView *)&D_80125128)->phase = 1;
                }
                break;
            }
        } else if (((CdRingView *)&D_80125128)->phase == 1) {
            switch (D_80125130[((CdRingView *)&D_80125128)->head].cmd) {
            case 0:
                if ((func_80087954(1, D_801255D0.bytes) != 0) && (func_800879D0(1, 0, &D_801255D0) == 1) && !(D_801255D0.bytes[0] & 0x40)) {
                    D_801255B4 = 2;
                    ((CdRingView *)&D_80125128)->head += 1;
                    ((CdRingView *)&D_80125128)->phase = -1;
                }
                break;
            case 9:
                if ((func_80087954(1, D_801255D0.bytes) != 0) && (func_800879D0(1, 0, &D_801255D0) == 1) && !(D_801255D0.bytes[0] & 0x40)) {
                    D_801255B4 = 2;
                    ((CdRingView *)&D_80125128)->head += 1;
                    ((CdRingView *)&D_80125128)->phase = -1;
                }
                break;
            case 21:
                if ((func_800879D0(1, 0, &D_801255D0) == 1) && !(D_801255D0.bytes[0] & 0x40)) {
                    D_801255B4 = 2;
                    ((CdRingView *)&D_80125128)->head += 1;
                    ((CdRingView *)&D_80125128)->phase = -1;
                }
                break;
            case 6:
                if (func_80087954(1, D_801255D0.bytes) != 0) {
                    D_800B3D68 = func_80087E4C(1, D_801255C0);
                    if (D_800B3D68 <= 0) {
                        if (D_800B3D68 == 0) {
                            if (D_800B3D60 != 0) {
                                if ((*D_801255BC & 0xFFFF0000) != 0x10120000) {
                                    ((CdRingView *)&D_80125128)->status = 0;
                                    if ((++D_800B3D64 & 3) == 3) {
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
                                    ((CdRingView *)&D_80125128)->phase = -1;
                                } else {
                                    ((CdRingView *)&D_80125128)->phase = -1;
                                    ((CdRingView *)&D_80125128)->head += 1;
                                    D_801255B4 = 2;
                                    ((CdRingView *)&D_80125128)->status |= 1;
                                    D_801255B5 += 1;
                                }
                            } else {
                                ((CdRingView *)&D_80125128)->phase = -1;
                                ((CdRingView *)&D_80125128)->head += 1;
                                D_801255B4 = 2;
                                ((CdRingView *)&D_80125128)->status |= 1;
                                D_801255B5 += 1;
                            }
                        } else if (D_800B3D68 < 0) {
                            ((CdRingView *)&D_80125128)->phase = -1;
                            D_801255B4 = 0;
                            ((CdRingView *)&D_80125128)->status = 0;
                        }
                    }
                }
                break;
            case 27:
                if (func_80087954(1, D_801255D0.bytes) == 5) {
                    ((CdRingView *)&D_80125128)->phase = -1;
                    func_80046478();
                } else {
                    if (func_800879D0(1, 0, &D_801255D0) == 0) {
                        do {

                        } while (func_800879D0(1, 0, &D_801255D0) == 0);
                    }
                    if (D_801255D0.bytes[0] & 0x20) {
                        ((CdRingView *)&D_80125128)->status = 0;
                        ((CdRingView *)&D_80125128)->phase = -1;
                        if (D_80125130[((CdRingView *)&D_80125128)->head].data[15] != 0xFF) {
                            D_801255B4 = 4;
                        }
                        ((CdRingView *)&D_80125128)->head += 1;
                    }
                }
                break;
            }
        }
    } else if (func_80087954(1, 0) == 5) {
        if (func_80046094() == 0x1B) {
            ((CdRingView *)&D_80125128)->phase = -1;
            if (((CdRingView *)&D_80125128)->head != 0) {
                ((CdRingView *)&D_80125128)->head -= 1;
            } else {
                ((CdRingView *)&D_80125128)->head = 0x2F;
            }
            func_80046478();
        } else {
            func_800879D0(1, 0, 0);
        }
    } else if (D_801255B4 == 4) {
        D_801255B0 = func_800460FC();
    }
    if ((u8) ((CdRingView *)&D_80125128)->head >= 0x30U) {
        ((CdRingView *)&D_80125128)->head = 0;
    }
}