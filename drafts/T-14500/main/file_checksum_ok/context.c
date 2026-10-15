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

INCLUDE_ASM("asm/nonmatchings/main/800451D0", func_80045414);

INCLUDE_ASM("asm/nonmatchings/main/800451D0", func_800455C4);

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

/* NON_MATCHING: T-12010, 17 listing lines; zero stores and local homes. */
#ifdef NON_MATCHING
s32 func_800460FC(void) {
    u8 status[8];
    u8 local[4];
    s32 saved;
    u8 *index = &D_80125128;
    s32 retries;
    WordBytes4 *state;

    saved = D_801255D0.word;
    state = &D_801255D0;
    retries = 0;
    /* FAKE: reuse the zero retry value for the byte-clear chain while
     * investigating the original v0 stores. T-12010 */
    state->bytes[0] = state->bytes[1] = state->bytes[2] = retries;
    state->bytes[3] = 0;
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
#else
INCLUDE_ASM("asm/nonmatchings/main/800451D0", func_800460FC);
#endif

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

s32 file_checksum_ok(arg0, arg1)
u8 arg0;
u8 *arg1;
{
    u32 lo;
    s32 temp_t0;
    u32 var_v0;
    u32 temp_a2;
    u32 var_v1;
    u8 *var_a0;
    u8 *var_a3;
    u8 *temp_a0;
    var_v0 = 0;
    var_v1 = 0;
    if (((arg0 << 0xB) - 4) != 0)
    {
        var_a0 = arg1;
        var_a3 = arg1 + 1;
        do
        {
            var_v1 += 2;
            var_v0 += (*var_a0) + (*var_a3);
            temp_t0 = var_v0 & 0xFFFF;
            var_a0 += 2;
            var_a3 += 2;
            var_v0 = temp_t0;
        }
        while (var_v1 < ((arg0 << 0xB) - 4));
    }
    temp_a0 = var_v1 + arg1;
    temp_a2 = temp_a0[0];
    lo = temp_a0[1];
    if (var_v0 != ((temp_a2 << 8) + lo))
    {
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
