#define MAIN_API_OVERRIDE_func_80045414 /* K&R u8 arg0 narrows the command byte on entry (T-24123). */
#define MAIN_API_OVERRIDE_func_800AD950 /* implicit int declaration (VSync returns int): a void prototype gives the
                                          * func_800455C4 wait-loop counter $v0 instead of $v1 (T-25123) */
#include "common.h"
#include "game.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/data/main/800451D0.data", D_800B3D60);
INCLUDE_RODATA("asm/data/main/800451D0.data", D_800B3D64);
INCLUDE_RODATA("asm/data/main/800451D0.data", D_800B3D68);
INCLUDE_RODATA("asm/data/main/800451D0.data", D_800B3D6C);
s32 D_800B3D70[4] = { 0, 0, 0, 0 };

s32 func_800451D0(void) {
    return D_801255D8;
}

void func_800451E0(s32 arg0) {
    D_801255D8 = arg0;
}

void func_800451EC(void) {
    cd_read_status = 0x80;
    D_800B3D68 = func_80087E4C(1, D_801255C0);
}

void func_80045224(void) {
    switch (func_80087954(1, D_801255C8)) {
    case 2:
        cd_read_status |= 2;
        break;
    case 5:
        D_8012512B = -1;
        break;
    }
}

s32 func_80045288(void) {
    if (D_80125129 == D_80125128 && D_8012512B == -1) {
        return 1;
    }
    return 0;
}

void func_800452C4(void) {
    D_80125128 = 0;
    D_80125129 = 0;
    cd_read_status = 2;
    D_8012512B = -1;
    D_801255B0 = 0;
    D_801255B4 = 0;
    D_801255B5 = 0;
    D_800B3D60 = 0;
    D_800B3D6C = 0;
}

s32 func_80045318(arg0, arg1, arg2)
u8 arg0;
s32 arg1;
s32 arg2;
{
    s32 src;

    /* FAKE: the copy of arg2 keeps it in $a3 (move a3,a2) instead of its stack home. T-11330 */
    src = arg2;
    if (((D_80125129 + 1) % 0x30U) == (D_80125128 % 0x30U)) {
        return -1;
    }
    D_80125130[D_80125129].cmd = arg0;
    D_80125130[D_80125129].arg = arg1;
    D_80125130[D_80125129].data[15] = 0;
    if (src == 1) {
        D_80125130[D_80125129].data[15] = 0xFF;
    } else if (src != 0) {
        memcpy(D_80125130[D_80125129].data, src, 0x10);
    }
    /* FAKE: `D = D + 1` rather than `D++`, which gives another register order. T-11330 */
    D_80125129 = D_80125129 + 1;
    if (D_80125129 < 0x30U) {
        return 1;
    }
    D_80125129 = 0;
}

void func_80045414(arg0, arg1, arg2)
u8 arg0;
s32 arg1;
u8 *arg2;
{
    u8 buf[0x20];

    arg1 = ((u32) arg1 >= 0x4AB51U) ? 0x4AB50 : arg1;
    switch (arg0) {
    case 0:
        func_80045318(0, 0, 0);
        break;
    case 9:
        func_80045318(9, 0, 0);
        break;
    case 6:
        D_800B3D6C = arg1;
        func_80045318(6, arg1, (s32) arg2);
        break;
    case 14:
        func_80045318(0xE, 0, (s32) arg2);
        break;
    case 13:
        if (arg1 == 0) {
            func_80045318(0xD, 0, (s32) arg2);
        } else {
            func_80045318(0xD, 0, (s32) arg2);
            func_80045318(2, arg1, 0);
            func_80045318(0x1B, arg1, 0);
            D_800B3D6C = arg1;
        }
        break;
    case 2:
        func_80045318(2, arg1, 0);
        break;
    case 21:
        func_80045318(2, arg1, 0);
        func_80045318(0x15, arg1, 0);
        break;
    case 10:
        func_80045318(0xA, arg1, 0);
        break;
    case 3:
        buf[0x10] = 5;
        func_80045318(0xE, 0, (s32) &buf[0x10]);
        func_80045318(2, arg1, 0);
        func_80045318(3, 0, 0);
        break;
    case 27:
        break;
    }
}

/* FAKE: view of the queue header bytes D_80125128..D_8012512B through one symbol; the load and
 * store order of the original needs one symbol, the real aggregate is not declared yet (T-24501). */
typedef struct CdRingView {
    /* 0x00 */ u8 head;
    /* 0x01 */ u8 tail;
    /* 0x02 */ u8 status;
    /* 0x03 */ s8 phase;
    /* 0x04 */ u8 pad[4];
} CdRingView; /* size 0x08 */
extern u16 D_800B3D64;
extern u32 *D_801255BC;
void func_80046478(void);

void func_800455C4(void) {
    s32 i; /* home 0x4C unused: the loop test is strength-reduced (see the FAKE at the loop) */
    u32 sp48;
    s32 sp44;
    u16 pad16; /* FAKE: unused frame home at 0x40, not a source recovery. T-24501 */
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
                                        /* FAKE: `* 1` makes uopt strength-reduce the test into an induction
                                         * temporary, spilled to the 0x2C slot below the locals as in the
                                         * original; the plain counter spills to its own home. T-25123 */
                                        for (i = 0; i * 1 != 0x200; i++) {
                                            func_800AD950(0);
                                        }
                                    }
                                    ((CdRingView *)&D_80125128)->phase = -1;
                                } else {
                                    ((CdRingView *)&D_80125128)->phase = -1;
                                    ((CdRingView *)&D_80125128)->head += 1;
                                    D_801255B4 = 2;
                                    ((CdRingView *)&D_80125128)->status |= 1;
                                    /* FAKE: D_801255B5 through the D_801255B4 symbol: one object in the
                                     * original (aggregate unknown); as1 then keeps the status load
                                     * before the D_801255B4 store. T-25123 */
                                    ((u8 *)&D_801255B4)[1] += 1;
                                }
                            } else {
                                ((CdRingView *)&D_80125128)->phase = -1;
                                ((CdRingView *)&D_80125128)->head += 1;
                                D_801255B4 = 2;
                                ((CdRingView *)&D_80125128)->status |= 1;
                                /* FAKE: as above, D_801255B5 through D_801255B4. T-25123 */
                                ((u8 *)&D_801255B4)[1] += 1;
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

u8 func_80046094(void) {
    return D_80125130[(D_80125128 + 0x2FU) % 0x30U].cmd;
}

u8 cd_read_sync_status(void) {
    return cd_read_status;
}

u8 func_800460DC(void) {
    return D_801255B5;
}

u8 func_800460EC(void) {
    return D_801255B4;
}

s32 func_800460FC(void) {
    u8 pad[8]; /* FAKE: unused pad places local/saved/status at 0x44/0x3C/0x34 in the 0x58 frame. T-24111 */
    s32 retries;
    WordBytes4 *state;
    u8 local[4];
    u8 *index = &D_80125128;
    s32 saved;
    u8 status[8];

    saved = D_801255D0.word;
    state = &D_801255D0;
    /* FAKE: same-line zero init and 4-byte clear chain emit move v0, zero and sb v0 for bytes 2..0. T-24111 */
    retries = 0; (&state->bytes[0])[0] = (&state->bytes[1])[0] = (&state->bytes[2])[0] = (&state->bytes[3])[0] = 0;
    if (func_80087954(1, status) != 0) {
        while (func_80087C64(0x10, 0, state) == 0) {
            retries++;
            if (retries >= 16) {
                if (func_80046094() == 0x1B) {
                    D_8012512B = -1;
                    if ((*index) != 0) {
                        (*index) -= 1;
                    } else {
                        (*index) = 0x2F;
                    }
                }
                state->word = saved;
                break;
            }
        }
    }
    local[0] = state->bytes[0];
    local[1] = state->bytes[1];
    local[2] = state->bytes[2];
    retries = func_80087FF0(local);
    if (func_80046684(state->bytes[0]) >= 0x4B) return 0;
    if (func_80046684(state->bytes[1]) >= 0x3C) return 0;
    if (func_80046684(state->bytes[2]) >= 0x4B) return 0;
    if (retries > 0) return retries;
    return 0;
}

s32 func_80046274(void) {
    return D_801255B0;
}

s32 *func_80046284(void) {
    return D_800B3D70;
}

void func_80046290(s32 arg0, s32 arg1, s32 arg2) {
    s32 idx = arg2 & 0xF;

    D_800B3D70[0] = idx;
    D_800B3D70[2] = arg0;
    D_800B3D70[3] = arg1;
    D_800B3D70[1] = D_800B3648[idx];
}

void func_800462BC(u8 arg0, s32 arg1, s32 *arg2) {
    *arg2 = arg1;
    ((u8 *)arg2)[3] = arg0;
}

void func_800462C8(u8 arg0, s32 arg1, s32 arg2) {
    /* FAKE: size 0x20 is taken from the original frame (local at 0x28 in 0x48);
     * the real type of this buffer is unknown. T-0016 */
    u8 buf[0x20];

    func_80044750(6);
    func_800462BC(arg0, arg1, (s32 *)buf);
    func_80045414(6, arg2, buf);
    D_800B3D60 = 0;
}

void cd_read_sectors(u8 arg0, s32 arg1, s32 arg2) {
    /* FAKE: size 0x20 is taken from the original frame (local at 0x28 in 0x48);
     * the real type of this buffer is unknown. T-0016 */
    u8 buf[0x20];

    func_80044750(6);
    func_800462BC(arg0, arg1, (s32 *)buf);
    func_80045414(6, arg2, buf);
    D_800B3D60 = 1;
}

s32 file_checksum_ok(blocks, buf)
u8 blocks;
u8 *buf;
{
    u32 lo;
    u16 sum;
    u32 hi;
    u32 i;
    u8 *p0;
    u8 *p1;
    u8 *end;

    sum = 0;
    i = 0;
    if (((blocks << 11) - 4) != 0) {
        p0 = buf;
        p1 = buf + 1;
        /* FAKE: goto into for-loop body omits the redundant entry test while keeping the u16 sum narrowing in the for-step after sltu. T-24087 */
        goto loop;
        for (; i < (blocks << 11) - 4; i += 2, p0 += 2, p1 += 2) {
        loop:
            sum += *p0 + *p1;
        }
    }
    end = i + buf;
    hi = end[0];
    lo = end[1];
    if (sum != ((hi << 8) + lo)) {
        return 0;
    }
    return 1;
}

/* s32 fallthrough preserves original constant scheduling (T-9710). */
s32 func_800463E8(void) {
    if ((D_8011ED17 & 0x80) && (D_800E6280.unk_10F8 & 4)) {
        func_80049FF0(0x9E, 0x5A, 0x9E, 0x62, 0xFFFF00, 0xFFFF00, 0x80);
        func_80049FF0(0x9D, 0x5A, 0x9D, 0x62, 0xFFFF00, 0xFFFF00, 0x80);
    }
}

void func_80046478(void) {
    s32 i;

    D_801255B8 = 0x84;
    while (func_80087C64(0xE, &D_801255B8, 0) == 0) {
    }
    for (i = 0; i != 0x200; i++) {
        func_800AD950(0);
    }
}
