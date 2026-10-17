#define MAIN_API_OVERRIDE_func_80083B24 /* implicit int view: the call's $v0 result moves the reloaded unk_110A to $v1 in func_801350A4 (definition: void, T-11880) */
#include "common.h"
#include "ovl/DATE2.h"

s32 func_80083B24();
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A4C0);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A4C4);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A4C8);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5B0);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5B4);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5B8);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5BC);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5C0);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5C4);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5C8);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5CC);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5D0);
s16 D_8013A5D4[3] = { 0, 0, 0 };
s16 D_8013A5DC[3] = { 0, 0, 0 };
s16 D_8013A5E4[3] = { 0, 0, 0 };
s16 D_8013A5EC[3] = { 0, 0, 0 };

typedef struct {
    void (*f[44])();
} FnTbl44; /* size 0xB0 */
extern FnTbl44 D_8013A600;

typedef struct {
    void (*f[58])();
} FnTbl58; /* size 0xE8 */
extern FnTbl58 D_8013A4C8;

void func_80132DE0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl58 tbl;

    tbl = D_8013A4C8;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80132E5C(void) {
    func_800AE0A0((void *)(0x80180000 + D_800E6280.unk_1104.w * 0x1400), 0x801C0000 + D_800E6280.unk_1104.w * 0x1400, 0x1400);
    D_800E6280.unk_1104.w += 1;
    if (D_800E6280.unk_1104.w == 0x12) {
        func_8004284C();
    }
}

void func_80132EC8(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    temp_v0 = D_800E6280.unk_1104.w * 0x1400;
    func_800AE0A0(temp_v0 + 0x80180000, temp_v0 + 0x801A0000, 0x1400);
    temp_v0_2 = D_800E6280.unk_1104.w + 1;
    D_800E6280.unk_1104.w = temp_v0_2;
    if (temp_v0_2 == 0x12) {
        D_800E6280.unk_F5F = 0;
        func_80051508(D_800CA17C, 0);
        func_8004284C();
    }
}

void func_80132F48(void) {
    s32 pad; /* FAKE: unused local that moves rect to sp+0x2C like the original (T-3330 frame rule). T-4050 */
    RECT rect;

    rect.x = 0x140;
    rect.y = 0x80;
    rect.w = 0x180;
    rect.h = 0x80;
    func_8009C8E0(&rect, (void *)0x80180000);
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    func_800847B8(D_800E6280.unk_75D);
    func_80133258();
    func_8004284C();
}

void func_80132FB0(void) {
    func_800AE0A0((void *)(0x801C0000 + D_800E6280.unk_1104.w * 0x1400), 0x80180000 + D_800E6280.unk_1104.w * 0x1400, 0x1400);
    D_800E6280.unk_1104.w += 1;
    if (D_800E6280.unk_1104.w == 0x12) {
        func_80083474();
    }
}

void func_8013301C(void) {
    func_800AE0A0((void *)(0x801A0000 + D_800E6280.unk_1104.w * 0x1400), 0x80180000 + D_800E6280.unk_1104.w * 0x1400, 0x1400);
    D_800E6280.unk_1104.w += 1;
    if (D_800E6280.unk_1104.w == 0x12) {
        func_80083474();
    }
}

void func_80133088(void) {
    RECT rect;

    func_80083474();
    rect.x = 0x140;
    rect.y = 0x80;
    rect.w = 0x180;
    rect.h = 0x80;
    func_8009C884(&rect, (void *)0x80180000);
}

void func_801330D4(void) {
    GameState *g;
    s16 sums[10];
    s16 i;
    s16 j;
    s32 best;

    best = 100; /* set before the loops: the original holds 100 in a2 from the entry */
    D_8013A4C0 = 0;
    D_800E6280.unk_F5F = 1;
    g = &D_800E6280;
    for (i = 1; i < 10; i++) {
        sums[i] = 0;
        for (j = 1; j < 0x25; j++) {
            sums[i] += g->unk_1BC[i].unk_14[j]; /* j = 0x24 reads the byte after unk_14 (the original does) */
        }
        sums[i] -= g->unk_1BC[i].unk_14[0x1A];
    }

    for (i = 1; i < 10; i++) {
        if (sums[i] < best && g->unk_1BC[i].unk_0C.f.b1) {
            g->unk_F5F = i; /* through g: the store may alias sums, so sums[i] is reloaded below */
            best = sums[i];
        }
    }
    func_800847B8(D_800E6280.unk_75D = D_800E6280.unk_F5F);
    D_800E6280.unk_F5F = 0;
    func_80085B3C(0xC, 0);
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    func_80085B3C(0xC, 0);
    func_80133258();
    func_801338F8();
    func_8004284C();
}

void func_80133258(void) {
    switch (D_800E6280.unk_F5F) {
    case 0:
        D_8013A410 = D_8013A384;
        D_8013A414 = D_8013A3B0;
        D_8013A418 = D_8013A3DC;
        return;
    case 1:
        D_8013A410 = D_8013A394;
        D_8013A414 = D_8013A3C0;
        D_8013A418 = D_8013A3EC;
        return;
    case 2:
        D_8013A410 = D_8013A388;
        D_8013A414 = D_8013A3B4;
        D_8013A418 = D_8013A3E0;
        return;
    case 3:
        D_8013A410 = D_8013A38C;
        D_8013A414 = D_8013A3B8;
        D_8013A418 = D_8013A3E4;
        return;
    case 4:
        D_8013A410 = D_8013A3A4;
        D_8013A414 = D_8013A3D0;
        D_8013A418 = D_8013A3FC;
        return;
    case 5:
        D_8013A410 = D_8013A398;
        D_8013A414 = D_8013A3C4;
        D_8013A418 = D_8013A3F0;
        return;
    case 6:
        D_8013A410 = D_8013A39C;
        D_8013A414 = D_8013A3C8;
        D_8013A418 = D_8013A3F4;
        return;
    case 7:
        D_8013A410 = D_8013A390;
        D_8013A414 = D_8013A3BC;
        D_8013A418 = D_8013A3E8;
        return;
    case 8:
        D_8013A410 = D_8013A380;
        D_8013A414 = D_8013A3AC;
        D_8013A418 = D_8013A3D8;
        return;
    case 9:
        D_8013A410 = D_8013A3A0;
        D_8013A414 = D_8013A3CC;
        D_8013A418 = D_8013A3F8;
        /* fallthrough */
    default:
        return;
    }
}

void func_80133494(void) {
    s32 temp;
    temp = D_8013A4C0;
    switch (temp) {
    case 0:
        func_8007ED84(0x4350);
        func_800AE0F0(D_800CA1DC, "観覧車");
        break;
    case 1:
        if ((u8) D_800E6280.unk_03E >= 0x61U) {
            func_8007ED84(0x4359);
            func_800AE0F0(D_800CA1DC, "絶叫マシーンビビール");
        } else {
            func_8007ED84(0x433E);
            func_800AE0F0(D_800CA1DC, "ジェットコースター");
        }
        break;
    case 2:
        func_8007ED84(0x4347);
        func_800AE0F0(D_800CA1DC, "ゴーストハウス");
        break;
    }
    func_8004284C();
}

void func_8013357C(void) {
    func_80062CD0(0x6C8C);
    func_8004284C();
}

void func_801335A4(void) {
    D_800E6280.unk_F5F = 0;
    get_p_name(&D_800CA17C, 0);
    func_80062CD0(0x55DF);
    func_8004284C();
}

void func_801335E4(void) {
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    func_800847B8(D_800E6280.unk_75D);
    func_80133258();
    func_80083B24();
}

void func_80133620(void) {
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    func_80133258();
    func_800847B8(D_800E6280.unk_F5F);
    switch (D_800E6280.unk_F5F) {
    case 1:
        func_80062CD0(0x5DC8);
        break;
    case 2:
        func_80062CD0(0x52B5);
        break;
    case 3:
        func_80062CD0(0x5936);
        break;
    case 4:
        func_80062CD0(0x6746);
        break;
    case 5:
        func_80062CD0(0x603E);
        break;
    case 6:
        func_80062CD0(0x62B4);
        break;
    case 7:
        func_80062CD0(0x5B7F);
        break;
    case 8:
        func_80062CD0(0x506C);
        break;
    case 9:
        func_80062CD0(0x652A);
        break;
    }
    func_8004284C();
}

void func_80133720(void) {
    if (D_80122CDC != 0) {
        func_80133620();
        D_8013A4C4 |= 1 << (s8)D_8013A4C0;
    } else {
        func_801335A4();
    }
    func_80133258();
}

void func_80133784(void) {
    s32 temp;
    temp = D_8013A4C0;
    switch (temp) {
    case 0:
        D_8013A430 = 1;
        D_8013A438 = 1;
        break;
    case 1:
        if ((u8) D_800E6280.unk_03E >= 0x61U) {
            D_8013A430 = 5;
            D_8013A438 = 5;
            D_8013A428 += 2;
        } else {
            D_8013A430 = 3;
            D_8013A438 = 3;
        }
        break;
    case 2:
        D_8013A430 = 7;
        D_8013A438 = 7;
        if ((u8) D_800E6280.unk_03E < 0x61U) {
            D_8013A428 += 2;
        }
        break;
    }
    func_801338F8();
    func_8004284C();
}

void func_80133884(void) {
    if (D_8013A4C0 == 2) {
        func_80042808();
        return;
    }
    func_8004284C();
}

void func_801338C4(void) {
    D_8013A4C0 += 1;
    func_80042940(0x1B);
}

/* FAKE: s32 fall-through keeps the original counter/result register. T-10010. */
s32 func_801338F8(void) {
    u8 previous_state;
    u8 first_state;
    first_state = func_80051A68(0U) & 0x7F;
    if (first_state >= 2U) {
        D_8013A430 += 0xA;
    }
    if (first_state >= 3U) {
        D_8013A430 += 0xA;
    }
    if (first_state == 4) {
        D_8013A430 += 0xA;
    }
    previous_state = func_80051A68(D_800E6280.unk_75D) & 0x7F;
    if (previous_state >= 2U) {
        D_8013A438 += 0xA;
    }
    if (previous_state >= 3U) {
        D_8013A438 += 0xA;
    }
    if (previous_state == 4) {
        D_8013A438 += 0xA;
    }
}

void func_801339F8(void) {
    D_8013A5B0 = 0x801C6000;
    D_8013A5B4 = 0x801AE000;
    D_8013A5B8 = 0x801B2000;
    D_8013A5BC = 0x801B6000;
    D_8013A5C0 = 0x801BA000;
    D_8013A5C4 = 0x801BE000;
    D_8013A5C8 = 0x801C2000;
}

void func_80133A6C(void) {
    u8 sel = D_8013A4C0; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        func_801348F0();
        return;
    case 1:
        if ((u8) D_800E6280.unk_03E >= 0x61U) {
            func_80133AF0();
            return;
        }
        func_801350A4();
        return;
    default:
        func_80135E50();
        return;
    }
}

/* s32: the header-error path and the scene steps outside 9..11 return 0, the dot animation falls off the end with the timer in v0 (as func_801348F0). T-11880 */
s32 func_80133AF0(void) {
    GameState *g;
    s16 i;
    s16 v;
    u8 x;
    RECT rect;
    s32 sp50;
    u32 sp4C;
    Rec24 *p;
    DateMovieWork *movie;
    u8 value;
    u32 color;
    u32 timer;

    g = &D_800E6280;
    movie = (DateMovieWork *)D_800B5920;
    D_800B3D60 = 0;
    switch (g->unk_1104.u) {
    case 0:
        D_80122D2C = 1;
        func_800AE120(D_800E6280.unk_10F8);
        func_80044750(0x206);
        movie->unk_06 = 0;
        movie->unk_02 = 0;
        movie->unk_04 = 0;
        movie->unk_00 = 0;
        movie->unk_19 = 0;
        movie->unk_18 = 0;
        movie->unk_28 = -1;
        movie->unk_0A = 0;
        movie->unk_1A = 0;
        func_8006492C(0);
        D_8013A5D0 = 2;
        D_800E6280.unk_110D = 0;
        D_8013A5CC = 0; for (i = 0; i < 2; i++) { /* FAKE: one source line keeps the as1 store order (as in func_801348F0) T-11880 */
            D_801217D0[i + 4].unk_08 = (i << 6) + 0x80;
            D_801217D0[i + 4].unk_00 = 0x82000000;
            D_801217D0[i + 4].unk_0A = 0;
            D_801217D0[i + 4].unk_0E = 0;
            D_801217D0[i + 4].unk_0F = 0xA0;
            D_801217D0[i + 4].unk_10 = 0;
            D_801217D0[i + 4].unk_0C = (i * 2) + 0x15;
            D_801217D0[i + 4].unk_12 = 0x1E0;
            D_801217D0[i + 4].unk_14 = 0;
            D_801217D0[i + 4].unk_15 = 0;
            D_801217D0[i + 4].unk_16 = 0;
            D_801217D0[i + 4].unk_18 = 0;
            D_801217D0[i + 4].unk_1A = 0;
            D_801217D0[i + 4].unk_1C = 0x1000;
            D_801217D0[i + 4].unk_1E = 0x1000;
            D_801217D0[i + 4].unk_20 = 0;
            D_801217D0[i + 4].unk_04 = (i << 7) - 0xA0;
            D_801217D0[i + 4].unk_06 = -0x78;
            D_800E6280.unk_10A5[i + 4] = 0xB;
        }
        for (i = 0; i < 2; i++) {
            D_801217D0[i + 6].unk_08 = (i << 6) + 0x80;
            D_801217D0[i + 6].unk_00 = 0x82000000;
            D_801217D0[i + 6].unk_0A = 0xA0;
            D_801217D0[i + 6].unk_0E = 0;
            D_801217D0[i + 6].unk_0F = 0;
            D_801217D0[i + 6].unk_10 = 0;
            D_801217D0[i + 6].unk_0C = (i * 2) + 0xA;
            D_801217D0[i + 6].unk_12 = 0x1E0;
            D_801217D0[i + 6].unk_14 = 0;
            D_801217D0[i + 6].unk_15 = 0;
            D_801217D0[i + 6].unk_16 = 0;
            D_801217D0[i + 6].unk_18 = 0;
            D_801217D0[i + 6].unk_1A = 0;
            D_801217D0[i + 6].unk_1C = 0x1000;
            D_801217D0[i + 6].unk_1E = 0x1000;
            D_801217D0[i + 6].unk_20 = 0;
            D_801217D0[i + 6].unk_04 = (i << 7) - 0xA0;
            D_801217D0[i + 6].unk_06 = -0x78;
            D_800E6280.unk_10A5[i + 6] = 0xB;
        }
        func_8005742C(0x43CB, 1);
        g->unk_1104.u += 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_800536AC(0x140, 0x100, 0x180, 0xA0);
            rect.x = 0x140;
            rect.y = 0x100;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_8009C8E0(&rect, (void *)0x80183000);
            g->unk_1104.u += 1;
        }
        break;
    case 2:
        func_8005742C(0x43D4, 1);
        g->unk_1104.u += 1;
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_800536AC(0x280, 0, 0x180, 0xA0);
            rect.x = 0x280;
            rect.y = 0;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_8009C8E0(&rect, (void *)0x801A1000);
            g->unk_1104.u += 1;
        }
        break;
    case 4:
        func_8005742C(0x43DE, 1);
        g->unk_1104.u += 1;
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_800536AC(0x140, 0x100, 0x180, 0xA0);
            rect.x = 0x140;
            rect.y = 0x100;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_8009C8E0(&rect, (void *)0x801BF000);
            g->unk_1104.u += 1;
        }
        break;
    case 6:
        func_8005742C(0x43E7, 1);
        g->unk_1104.u += 1;
        break;
    case 7:
        if (cd_read_sync_status() & 1) {
            func_800536AC(0x280, 0, 0x180, 0xA0);
            rect.x = 0x280;
            rect.y = 0;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_8009C8E0(&rect, (void *)0x801DD000);
            g->unk_1104.u += 1;
        }
        break;
    case 8:
        for (i = 0; i < 4; i++) {
            D_801217D0[i + 4].unk_00 = 0x02000000;
        }
        g->unk_1104.u += 1;
        func_80044750(0x500);
        break;
    case 9:
        value = D_801217D0[4].unk_14;
        value += 2;
        color = (value & 0xFF);
        D_801217D0[7].unk_16 = color;
        D_801217D0[7].unk_15 = color;
        D_801217D0[7].unk_14 = color;
        D_801217D0[6].unk_16 = color;
        D_801217D0[6].unk_15 = color;
        D_801217D0[6].unk_14 = color;
        D_801217D0[5].unk_16 = color;
        D_801217D0[5].unk_15 = color;
        D_801217D0[5].unk_14 = color;
        D_801217D0[4].unk_16 = color;
        D_801217D0[4].unk_15 = color;
        D_801217D0[4].unk_14 = color;
        if (value == 0x80) {
            g->unk_1104.u += 1;
        }
        break;
    case 10:
        break;
    case 11:
        value = D_801217D0[4].unk_14;
        value -= 2;
        color = (value & 0xFF);
        D_801217D0[7].unk_16 = color;
        D_801217D0[7].unk_15 = color;
        D_801217D0[7].unk_14 = color;
        D_801217D0[6].unk_16 = color;
        D_801217D0[6].unk_15 = color;
        D_801217D0[6].unk_14 = color;
        D_801217D0[5].unk_16 = color;
        D_801217D0[5].unk_15 = color;
        D_801217D0[5].unk_14 = color;
        D_801217D0[4].unk_16 = color;
        D_801217D0[4].unk_15 = color;
        D_801217D0[4].unk_14 = color;
        if (value == 0) {
            for (i = 0; i < 4; i++) {
                D_801217D0[i + 4].unk_00 = 0x82000000;
            }
            g->unk_1104.u += 1;
        }
        break;
    case 12:
        sp50 = D_800E6280.unk_110A;
        sp4C = g->unk_1104.u;
        func_80083B24();
        g->unk_1104.u = sp4C;
        if (sp50 != D_800E6280.unk_110A) {
            D_800E6280.unk_110A--;
            g->unk_1104.u += 1;
        }
        break;
    case 13:
    case 16:
    case 19:
    case 21:
    case 23:
        if (cd_read_sync_status() & 1) {
            g->unk_1104.u += 1;
        }
        break;
    case 14:
        sp4C = g->unk_1104.u;
        func_80083474();
        D_800E6280.unk_110A -= 1;
        g->unk_1104.u = sp4C + 1;
        break;
    case 15:
        func_800462C8(0xA, 0x80162000, 0x4359);
        g->unk_1104.u += 1;
        break;
    case 17:
        if (((((DateMovieHeader8 *)D_80125C04)->unk_00 & 0xFFFF0000) != 0x38000000) || ((((DateMovieHeader8 *)D_80125C04)->unk_04 & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n");
            movie->unk_04 = 0;
            movie->unk_06 = 0;
            g->unk_1104.u -= 2;
            return 0;
        }
        func_8005751C(1);
        dec_bg_show_switch(1);
        g->unk_1104.u += 1;
        break;
    case 18:
        func_80044750(0x200);
        sp4C = g->unk_1104.u;
        func_80132BB0();
        D_800E6280.unk_110A -= 1;
        g->unk_1104.u = sp4C + 1;
        break;
    default:
        func_8006492C(1);
        D_80122D2C = 0;
        func_8004284C();
        break;
    }
    if (g->unk_1104.u < 9 || g->unk_1104.u >= 12) {
        return 0;
    }
    p = D_801217D0;
    v = p[4].unk_0E;
    v += D_8013A5D0;
    if (v < 0) {
        v = 0;
        D_8013A5D0 = (func_800AE0D0() % 4) + 2;
    }
    if (v >= 0x40) {
        v = 0x3F;
        D_8013A5D0 = -2 - (func_800AE0D0() % 4);
    }
    x = p[4].unk_0F;
    p[7].unk_0E = v;
    p[6].unk_0E = v;
    p[5].unk_0E = v;
    p[4].unk_0E = v;
    if (x != 0) {
        p[4].unk_0A += 0x14;
        p[5].unk_0A += 0x14;
        x -= 0x14;
        p[5].unk_0F -= 0x14;
    } else {
        p[4].unk_0A -= 0x14;
        p[4].unk_06 += 0x14;
        p[5].unk_0A -= 0x14;
        p[5].unk_06 += 0x14;
    }
    if (p[6].unk_0F != 0) {
        p[6].unk_0A += 0x14;
        p[7].unk_0A += 0x14;
        p[6].unk_0F -= 0x14;
        p[7].unk_0F -= 0x14;
    } else {
        p[6].unk_0A -= 0x14;
        p[6].unk_06 += 0x14;
        p[7].unk_0A -= 0x14;
        p[7].unk_06 += 0x14;
    }
    p[4].unk_0F = x;
    if ((x == 0 && (g->unk_110D == 1 || g->unk_110D == 3)) || (p[6].unk_0F == 0 && (g->unk_110D == 0 || g->unk_110D == 2))) {
        switch (D_800E6280.unk_110D) { /* FAKE: g-> in the test, the symbol here; one access path keeps the byte in $v0 and drops the reload T-11880 */
        case 0:
            for (i = 0; i < 2; i++) {
                D_801217D0[i + 4].unk_0A = 0;
                D_801217D0[i + 4].unk_0F = 0xA0;
                D_801217D0[i + 4].unk_06 = -0x78;
                D_801217D0[i + 4].unk_0C = (i * 2) + 0x15;
            }
            func_8004435C(0x140, 0x100, 0x180, 0xA0, 0x80183000);
            break;
        case 1:
            for (i = 0; i < 2; i++) {
                D_801217D0[i + 6].unk_0A = 0;
                D_801217D0[i + 6].unk_0F = 0xA0;
                D_801217D0[i + 6].unk_06 = -0x78;
                D_801217D0[i + 6].unk_0C = (i * 2) + 0xA;
            }
            func_8004435C(0x280, 0, 0x180, 0xA0, 0x801A1000);
            break;
        case 2:
            for (i = 0; i < 2; i++) {
                D_801217D0[i + 4].unk_0A = 0;
                D_801217D0[i + 4].unk_0F = 0xA0;
                D_801217D0[i + 4].unk_06 = -0x78;
                D_801217D0[i + 4].unk_0C = (i * 2) + 0x15;
            }
            func_8004435C(0x140, 0x100, 0x180, 0xA0, 0x801BF000);
            break;
        case 3:
            for (i = 0; i < 2; i++) {
                D_801217D0[i + 6].unk_0A = 0;
                D_801217D0[i + 6].unk_0F = 0xA0;
                D_801217D0[i + 6].unk_06 = -0x78;
                D_801217D0[i + 6].unk_0C = (i * 2) + 0xA;
            }
            func_8004435C(0x280, 0, 0x180, 0xA0, 0x801DD000);
            break;
        }
        if (++D_800E6280.unk_110D >= 4) {
            D_800E6280.unk_110D = 0;
        }
    }
    timer = (D_8013A5CC + 1) & 0xFFFF;
    D_8013A5CC = timer;
    if (timer == 0x96) {
        g->unk_1104.u += 1;
    }
}

/* s32: the step exits return 0 (or v0,zero,zero), the fade steps fall off the end (as in the DATE twin func_8014718C). T-11880 */
s32 func_801348F0(void) {
    GameState *g;
    RECT rect; /* FAKE: unused, reserves the 8 bytes above sp30 in the original 0x40 frame (as in the DATE twin func_8014718C) T-11880 */
    u32 sp30;
    u32 timer;
    s16 i;
    u8 x;
    u32 y; /* FAKE: second copy of the fade step; it holds $v1 so the stored value lands in $a1 T-11880 */
    DateMovieWork *movie;

    g = &D_800E6280;
    movie = (DateMovieWork *)D_800B5920;
    D_800B3D60 = 0;
    switch (g->unk_1104.u) {
    case 0:
        D_80122D2C = 1;
        func_80044750(0x201);
        movie->unk_06 = 0;
        movie->unk_02 = 0;
        movie->unk_04 = 0;
        movie->unk_00 = 0;
        movie->unk_19 = 0;
        movie->unk_18 = 0;
        movie->unk_28 = -1;
        movie->unk_0A = 0;
        movie->unk_1A = 0;
        func_8006492C(0);
        D_8013A5CC = 0; for (i = 0; i < 2; i++) { /* FAKE: one source line keeps the as1 store order T-11880 */
            D_801217D0[i + 10].unk_08 = (i << 6) + 0x80;
            D_801217D0[i + 10].unk_00 = 0x82000000;
            D_801217D0[i + 10].unk_0A = 0xA0;
            D_801217D0[i + 10].unk_0E = 0;
            D_801217D0[i + 10].unk_0F = 0;
            D_801217D0[i + 10].unk_10 = 0;
            D_801217D0[i + 10].unk_0C = (i * 2) + 0x15;
            D_801217D0[i + 10].unk_12 = 0x1E0;
            D_801217D0[i + 10].unk_14 = 0;
            D_801217D0[i + 10].unk_15 = 0;
            D_801217D0[i + 10].unk_16 = 0;
            D_801217D0[i + 10].unk_18 = 0;
            D_801217D0[i + 10].unk_1A = 0;
            D_801217D0[i + 10].unk_1C = 0x1000;
            D_801217D0[i + 10].unk_1E = 0x1000;
            D_801217D0[i + 10].unk_20 = 0;
            D_801217D0[i + 10].unk_04 = (i << 7) - 0xA0;
            D_801217D0[i + 10].unk_06 = -0x96;
            D_800E6280.unk_10A5[i + 10] = 0xC;
        }
        func_801365DC(0x43C2);
        g->unk_1104.u += 1;
        return 0;
    case 1:
        if (cd_read_sync_status() & 1) {
            if (((((DateMovieHeader8 *)D_80125C04)->unk_00 & 0xFFFF0000) != 0x38000000) || ((((DateMovieHeader8 *)D_80125C04)->unk_04 & 0xFF010000) != 0x10000)) {
                func_800AE0B0("DEC_BS header Error \n");
                movie->unk_04 = 0;
                movie->unk_06 = 0;
                g->unk_1104.u -= 1;
                return 0;
            }
            func_800536AC(0x140, 0x100, 0x140, 0xA0);
            g->unk_1104.u += 1;
        }
        return 0;
    case 2:
        cd_read_sectors(0x31, 0x801AE000, 0xBCD1);
        func_801339F8();
        g->unk_1104.u += 1;
        return 0;
    case 3:
    case 11:
    case 14:
    case 16:
    case 18:
        if (cd_read_sync_status() & 1) {
            g->unk_1104.u += 1;
        }
        return 0;
    case 4:
        func_80043914(D_8013A5B0, 0x11, 1, 0, 0);
        func_80084E90(D_8013A5B4, D_8013A5B8, D_8013A5BC, D_8013A5C0, D_8013A5C4, D_8013A5C8);
        g->unk_1104.u += 1;
        return 0;
    case 5:
        for (i = 10; i < 12; i++) {
            D_801217D0[i].unk_00 &= 0x7FFFFFFF;
        }
        for (i = 58; i < 64; i++) {
            D_801217D0[i].unk_00 &= 0x7FFFFFFF;
        }
        g->unk_1104.u += 1;
        return 0;
    case 6:
        x = (D_801217D0[58].unk_14 += 4); y = D_801217D0[58].unk_15 = x; D_801217D0[58].unk_16 = y; /* FAKE: see y; one line keeps the store order T-11880 */
        for (i = 10; i < 12; i++) {
            D_801217D0[i].unk_14 = D_801217D0[i].unk_15 = D_801217D0[i].unk_16 = D_801217D0[58].unk_14;
        }
        for (i = 59; i < 64; i++) {
            D_801217D0[i].unk_14 = D_801217D0[i].unk_15 = D_801217D0[i].unk_16 = D_801217D0[58].unk_14;
        }
        if (D_801217D0[58].unk_14 == 0x40) {
            g->unk_1104.u += 1;
        }
        return 0;
    case 7:
        x = (D_801217D0[58].unk_14 += 4); y = D_801217D0[58].unk_15 = x; D_801217D0[58].unk_16 = y; /* FAKE: see y; one line keeps the store order T-11880 */
        for (i = 10; i < 12; i++) {
            D_801217D0[i].unk_14 = D_801217D0[i].unk_15 = D_801217D0[i].unk_16 = D_801217D0[58].unk_14;
        }
        for (i = 59; i < 64; i++) {
            D_801217D0[i].unk_14 = D_801217D0[i].unk_15 = D_801217D0[i].unk_16 = D_801217D0[58].unk_14;
        }
        if (D_801217D0[58].unk_14 == 0x80) {
            g->unk_1104.u += 1;
        }
        break;
    case 8:
        break;
    case 9:
        x = (D_801217D0[58].unk_14 -= 4); y = D_801217D0[58].unk_15 = x; D_801217D0[58].unk_16 = y; /* FAKE: see y; one line keeps the store order T-11880 */
        for (i = 10; i < 12; i++) {
            D_801217D0[i].unk_14 = D_801217D0[i].unk_15 = D_801217D0[i].unk_16 = D_801217D0[58].unk_14;
        }
        for (i = 59; i < 64; i++) {
            D_801217D0[i].unk_14 = D_801217D0[i].unk_15 = D_801217D0[i].unk_16 = D_801217D0[58].unk_14;
        }
        if (D_801217D0[58].unk_14 == 0) {
            for (i = 10; i < 12; i++) {
                D_801217D0[i].unk_00 |= 0x80000000;
            }
            for (i = 58; i < 64; i++) {
                D_801217D0[i].unk_00 |= 0x80000000;
            }
            g->unk_1104.u += 1;
        }
        break;
    case 10:
        func_800462C8(0xA, 0x80162000, 0x4350);
        g->unk_1104.u += 1;
        return 0;
    case 12:
        if (((((DateMovieHeader8 *)D_80125C04)->unk_00 & 0xFFFF0000) != 0x38000000) || ((((DateMovieHeader8 *)D_80125C04)->unk_04 & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n");
            movie->unk_04 = 0;
            movie->unk_06 = 0;
            g->unk_1104.u -= 2;
            return 0;
        }
        func_8005751C(1);
        dec_bg_show_switch(1);
        g->unk_1104.u += 1;
        return 0;
    case 13:
        sp30 = g->unk_1104.u;
        func_80132BB0();
        D_800E6280.unk_110A -= 1;
        g->unk_1104.u = sp30;
        g->unk_1104.u++;
        return 0;
    default:
        func_8004284C();
        D_80122D2C = 0;
        func_80044750(0x200);
        func_8006492C(1);
        return 0;
    }
    timer = (D_8013A5CC + 1) & 0xFFFF;
    D_8013A5CC = timer;
    if (timer == 0x96) {
        g->unk_1104.u += 1;
    }
}

/* s32: only the header-error path returns 0 (or v0,zero,zero), as in the DATE twin func_80147958. T-11880 */
s32 func_801350A4(void) {
    GameState *g;
    s16 i;
    s32 sp54;
    u32 sp50;
    RECT rect;
    DateMovieWork *movie;
    DateMovieHeader8 *header; /* FAKE: unused, reserves the observed frame slot (as in the DATE twin func_80147958) T-11880 */
    u8 value;
    u32 color;

    g = &D_800E6280;
    movie = (DateMovieWork *)D_800B5920;
    D_800B3D60 = 0;
    switch (g->unk_1104.u) {
    case 0:
        func_80044750(0xCF);
        D_80122D2C = 1;
        movie->unk_06 = 0;
        movie->unk_02 = 0;
        movie->unk_04 = 0;
        movie->unk_00 = 0;
        movie->unk_19 = 0;
        movie->unk_18 = 0;
        movie->unk_28 = -1;
        movie->unk_0A = 0;
        movie->unk_1A = 0;
        func_8006492C(0);
        D_8013A5CC = 0;
        for (i = 0; i < 2; i++) {
            D_801217D0[i + 4].unk_08 = (i << 6) + 0x80;
            D_801217D0[i + 4].unk_00 = 0x82000000;
            D_801217D0[i + 4].unk_0A = 0xA0;
            D_801217D0[i + 4].unk_0E = 0;
            D_801217D0[i + 4].unk_0F = 0;
            D_801217D0[i + 4].unk_10 = 0;
            D_801217D0[i + 4].unk_0C = (i * 2) + 0x15;
            D_801217D0[i + 4].unk_12 = 0x1E0;
            D_801217D0[i + 4].unk_14 = 0;
            D_801217D0[i + 4].unk_15 = 0;
            D_801217D0[i + 4].unk_16 = 0;
            D_801217D0[i + 4].unk_18 = 0;
            D_801217D0[i + 4].unk_1A = 0;
            D_801217D0[i + 4].unk_1C = 0x1000;
            D_801217D0[i + 4].unk_1E = 0x1000;
            D_801217D0[i + 4].unk_20 = 0;
            D_801217D0[i + 4].unk_04 = (i << 7) - 0xA0;
            D_801217D0[i + 4].unk_06 = -0x78;
            D_800E6280.unk_10A5[i + 4] = 0xB;
        }
        for (i = 0; i < 2; i++) {
            D_801217D0[i + 6].unk_08 = (i << 6) + 0x80;
            D_801217D0[i + 6].unk_00 = 0x82000000;
            D_801217D0[i + 6].unk_0A = 0;
            D_801217D0[i + 6].unk_0E = 0;
            D_801217D0[i + 6].unk_0F = 0xA0;
            D_801217D0[i + 6].unk_10 = 0;
            D_801217D0[i + 6].unk_0C = (i * 2) + 0xA;
            D_801217D0[i + 6].unk_12 = 0x1E0;
            D_801217D0[i + 6].unk_14 = 0;
            D_801217D0[i + 6].unk_15 = 0;
            D_801217D0[i + 6].unk_16 = 0;
            D_801217D0[i + 6].unk_18 = 0;
            D_801217D0[i + 6].unk_1A = 0;
            D_801217D0[i + 6].unk_1C = 0x1000;
            D_801217D0[i + 6].unk_1E = 0x1000;
            D_801217D0[i + 6].unk_20 = 0;
            D_801217D0[i + 6].unk_04 = (i << 7) - 0xA0;
            D_801217D0[i + 6].unk_06 = -0x78;
            D_800E6280.unk_10A5[i + 6] = 0xB;
        }
        func_8005742C(0x43B1, 1);
        g->unk_1104.u += 1;
        return;
    case 1:
    case 4:
    case 7:
    case 10:
    case 13:
    case 16:
    case 31:
    case 34:
    case 37:
        if (cd_read_sync_status() & 1) {
            g->unk_1104.u += 1;
            return;
        }
        return;
    case 2:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C8E0(&rect, (void *)0x80183000);
        g->unk_1104.u += 1;
        return;
    case 3:
        func_8005742C(0x43AA, 1);
        g->unk_1104.u += 1;
        return;
    case 5:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C8E0(&rect, (void *)0x8019C000);
        g->unk_1104.u += 1;
        return;
    case 6:
        func_8005742C(0x43A3, 1);
        g->unk_1104.u += 1;
        return;
    case 8:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C8E0(&rect, (void *)0x801B5000);
        g->unk_1104.u += 1;
        return;
    case 9:
        func_8005742C(0x439C, 1);
        g->unk_1104.u += 1;
        return;
    case 11:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C8E0(&rect, (void *)0x801CE000);
        g->unk_1104.u += 1;
        return;
    case 12:
        func_8005742C(0x4395, 1);
        g->unk_1104.u += 1;
        return;
    case 14:
        func_800536AC(0x280, 0, 0x140, 0xA0);
        g->unk_1104.u += 1;
        return;
    case 15:
        func_8005742C(0x438D, 1);
        g->unk_1104.u += 1;
        return;
    case 17:
        func_800536AC(0x140, 0x100, 0x140, 0xA0);
        g->unk_1104.u += 1;
        return;
    case 18:
        for (i = 0; i < 4; i++) {
            D_801217D0[i + 4].unk_00 = (s32) (D_801217D0[i + 4].unk_00 & 0x7FFFFFFF);
        }
        g->unk_1104.u++;
        return;
    case 19:
        for (i = 4; i < 8; i++) {
            func_80084C98(i, 0x80);
        }
        for (i = 0; i < 2; i++) {
            D_801217D0[i + 4].unk_00 = 0x02000000;
            D_801217D0[i + 4].unk_0A = 0xA0;
            D_801217D0[i + 4].unk_0F = 0;
            D_801217D0[i + 4].unk_06 = -0x78;
        }
        for (i = 0; i < 2; i++) {
            D_801217D0[i + 6].unk_00 = 0x02000000;
            D_801217D0[i + 6].unk_0A = 0;
            D_801217D0[i + 6].unk_0F = 0xA0;
            D_801217D0[i + 6].unk_06 = -0x78;
        }
        D_8013A5CC = 0;
        func_80044750(0x500);
        g->unk_1104.u += 1;
        return;
    case 20:
    case 22:
    case 24:
        func_80135DA4(8);
        if (++D_8013A5CC == 0x14) {
            g->unk_1104.u += 1;
            return;
        }
        break;
    case 21:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x140, 0x100);
        func_8004435C(0x280U, 0U, 0x140U, 0xA0U, 0x801CE000);
        for (i = 0; i < 2; i++) {
            D_801217D0[i + 4].unk_0A = 0xA0;
            D_801217D0[i + 4].unk_0F = 0;
            D_801217D0[i + 4].unk_06 = -0x78;
        }
        for (i = 0; i < 2; i++) {
            D_801217D0[i + 6].unk_0A = 0;
            D_801217D0[i + 6].unk_0F = 0xA0;
            D_801217D0[i + 6].unk_06 = -0x78;
        }
        D_8013A5CC = 0;
        g->unk_1104.u += 1;
        return;
    case 23:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x140, 0x100);
        func_8004435C(0x280U, 0U, 0x140U, 0xA0U, 0x801B5000);
        for (i = 0; i < 2; i++) {
            D_801217D0[i + 4].unk_0A = 0xA0;
            D_801217D0[i + 4].unk_0F = 0;
            D_801217D0[i + 4].unk_06 = -0x78;
        }
        for (i = 0; i < 2; i++) {
            D_801217D0[i + 6].unk_0A = 0;
            D_801217D0[i + 6].unk_0F = 0xA0;
            D_801217D0[i + 6].unk_06 = -0x78;
        }
        D_8013A5CC = 0;
        g->unk_1104.u += 1;
        return;
    case 25:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x140, 0x100);
        func_8004435C(0x280U, 0U, 0x140U, 0xA0U, 0x8019C000);
        for (i = 0; i < 2; i++) {
            D_801217D0[i + 4].unk_0A = 0xA0;
            D_801217D0[i + 4].unk_0F = 0;
            D_801217D0[i + 4].unk_06 = -0x78;
        }
        for (i = 0; i < 2; i++) {
            D_801217D0[i + 6].unk_0A = 0;
            D_801217D0[i + 6].unk_0F = 0xA0;
            D_801217D0[i + 6].unk_06 = -0x78;
        }
        D_8013A5CC = 0;
        g->unk_1104.u += 1;
        return;
    case 26:
        func_80135DA4(8);
        if ((u16) D_8013A5CC >= 8U) {
            value = D_801217D0[4].unk_14;
            value -= 4;
            color = (value & 0xFF);
            D_801217D0[7].unk_16 = color;
            D_801217D0[7].unk_15 = color;
            D_801217D0[7].unk_14 = color;
            D_801217D0[6].unk_16 = color;
            D_801217D0[6].unk_15 = color;
            D_801217D0[6].unk_14 = color;
            D_801217D0[5].unk_16 = color;
            D_801217D0[5].unk_15 = color;
            D_801217D0[5].unk_14 = color;
            D_801217D0[4].unk_16 = color;
            D_801217D0[4].unk_15 = color;
            D_801217D0[4].unk_14 = color;
        }
        if (++D_8013A5CC == 0x14) {
            g->unk_1104.u += 1;
            return;
        }
        break;
    case 27:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x140, 0x100);
        func_8004435C(0x280U, 0U, 0x140U, 0xA0U, 0x80183000);
        for (i = 0; i < 2; i++) {
            D_801217D0[i + 4].unk_0A = 0xA0;
            D_801217D0[i + 4].unk_0F = 0;
            D_801217D0[i + 4].unk_06 = -0x78;
        }
        for (i = 0; i < 2; i++) {
            D_801217D0[i + 6].unk_0A = 0;
            D_801217D0[i + 6].unk_0F = 0xA0;
            D_801217D0[i + 6].unk_06 = -0x78;
        }
        D_8013A5CC = 0;
        g->unk_1104.u += 1;
        return;
    case 28:
        func_80135DA4(8);
        value = D_801217D0[4].unk_14;
        value -= 4;
        color = (value & 0xFF);
        D_801217D0[7].unk_16 = color;
        D_801217D0[7].unk_15 = color;
        D_801217D0[7].unk_14 = color;
        D_801217D0[6].unk_16 = color;
        D_801217D0[6].unk_15 = color;
        D_801217D0[6].unk_14 = color;
        D_801217D0[5].unk_16 = color;
        D_801217D0[5].unk_15 = color;
        D_801217D0[5].unk_14 = color;
        D_801217D0[4].unk_16 = color;
        D_801217D0[4].unk_15 = color;
        D_801217D0[4].unk_14 = color;
        if (value < 4U) {
            g->unk_1104.u += 1;
            return;
        }
        break;
    case 29:
        for (i = 0; i < 4; i++) {
            D_801217D0[i + 4].unk_00 = (s32) (D_801217D0[i + 4].unk_00 | 0x80000000);
        }
        g->unk_1104.u++;
        return;
    case 30:
        sp50 = g->unk_1104.u;

        sp54 = D_800E6280.unk_110A;
        func_80083B24();
        g->unk_1104.u = sp50; if (sp54 != D_800E6280.unk_110A) { /* FAKE: one source line keeps the as1 load order T-11880 */
            D_800E6280.unk_110A -= 1;
            g->unk_1104.u++;
        }
        break;
    case 32:
        sp50 = g->unk_1104.u;

        func_80083474();
        D_800E6280.unk_110A -= 1;
        g->unk_1104.u = sp50;
        g->unk_1104.u++;
        return;
    case 33:
        func_800462C8(0xAU, 0x80162000, 0x433E);
        g->unk_1104.u += 1;
        return;
    case 35:
        if (((((DateMovieHeader8 *)D_80125C04)->unk_00 & 0xFFFF0000) != 0x38000000) || ((((DateMovieHeader8 *)D_80125C04)->unk_04 & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n");
            movie->unk_04 = 0;
            movie->unk_06 = 0;
            g->unk_1104.u -= 2;
            return 0;
        }
        func_8005751C(1);
        dec_bg_show_switch(1);
        g->unk_1104.u += 1;
        return;
    case 36:
        sp50 = g->unk_1104.u;

        func_80132BB0();
        D_800E6280.unk_110A -= 1;
        g->unk_1104.u = sp50;
        g->unk_1104.u++;
        return;
    default:
        func_8004284C();
        D_80122D2C = 0;
        func_80044750(0x200);
        func_8006492C(1);
        break;
    }
    return;
}

s32 func_80135DA4(s16 arg0) {
    D_801217D0[4].unk_06 += arg0;
    D_801217D0[5].unk_06 += arg0;
    D_801217D0[4].unk_0A -= arg0;
    D_801217D0[5].unk_0A -= arg0;
    D_801217D0[6].unk_0A += arg0;
    D_801217D0[7].unk_0A += arg0;
    D_801217D0[6].unk_0F -= arg0;
    D_801217D0[7].unk_0F -= arg0;
}

/* Returns int like the DATE twin func_801489D0: only the header-error path of case 2 sets a
 * result (move v0,zero), the other paths leave v0 alone. T-18800 */
s32 func_80135E50(void) {
    s16 *temp_a0;
    s16 *temp_v0;
    s16 temp_a1;
    s16 temp_v0_2;
    s16 var_s3;
    s32 remainder;
    u32 temp_s1;
    u32 temp_s1_3;
    u8 temp_s0;
    u8 temp_s0_2;

    switch (D_800E6280.unk_1104.u) {
    case 0:
        D_80122D2C = 1;
        func_800AE120((*(s32 *)&D_800E6280.unk_10F8));
        D_8013A5CC = 0;
        hizuke_disp_switch(0);
        func_801365DC(0x43BA);
        D_8013A5D4[0] = -0x50;
        D_8013A5D4[1] = 0;
        D_8013A5D4[2] = 0x64;
        D_8013A5DC[0] = -0x28;
        D_8013A5DC[1] = 0;
        D_8013A5DC[2] = -0x3C;
        D_800E6280.unk_1104.u += 1;
        D_8013A5E4[0] = (func_800AE0D0() % 3) - 1;
        D_8013A5E4[1] = (func_800AE0D0() % 3) - 1;
        D_8013A5E4[2] = (func_800AE0D0() % 3) - 1;
        D_8013A5EC[0] = (func_800AE0D0() % 3) - 1;
        D_8013A5EC[1] = (func_800AE0D0() % 3) - 1;
        D_8013A5EC[2] = (func_800AE0D0() % 3) - 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_1104.u += 1;
            break;
        }
        break;
    case 2:
        if (((((u32 *)D_80125C04)[0] & 0xFFFF0000) != 0x38000000) || ((((u32 *)D_80125C04)[1] & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n");
            ((RECT *)D_800B5920)[0].w = 0;
            ((RECT *)D_800B5920)[0].h = 0;
            D_800E6280.unk_1104.u -= 2;
            return 0;
        }
        func_8005751C(1);
        dec_bg_show_switch(1);
        func_80044750(0x203);
        D_800E6280.unk_1104.u += 1;
        break;
    case 3:
        temp_s0 = D_800E6280.unk_110A;
        temp_s1 = D_800E6280.unk_1104.u;
        normal_date_bg_fadein();
        D_800E6280.unk_1104.u = temp_s1;
        if (temp_s0 != D_800E6280.unk_110A) {
            D_800E6280.unk_1104.u = temp_s1 + 1;
        }
        D_800E6280.unk_110A = temp_s0;
        break;
    case 4:
        var_s3 = 0;
        do {
            if (((u32) (*(s32 *)&D_800E6280.unk_10F8) % 30U) == (func_800AE0D0() % 30)) {
                (D_8013A5E4)[var_s3] = (func_800AE0D0() % 3) - 1;
            }

            if (((u32) (*(s32 *)&D_800E6280.unk_10F8) % 30U) == (func_800AE0D0() % 30)) {
                (D_8013A5EC)[var_s3] = (func_800AE0D0() % 3) - 1;
            }

            if ((D_8013A5E4[var_s3] == 0) && (D_8013A5EC[var_s3] == 0)) {
                if ((remainder = func_800AE0D0() % 2)) {
                    D_8013A5E4[var_s3] = ((func_800AE0D0() % 2) * 2) - 1;
                } else {
                    D_8013A5EC[var_s3] = ((func_800AE0D0() % 2) * 2) - 1;
                }
            }
            temp_v0 = &(D_8013A5D4)[var_s3];
            *temp_v0 += D_8013A5E4[var_s3];
            temp_a0 = &(D_8013A5DC)[var_s3];
            *temp_a0 += D_8013A5EC[var_s3];
            temp_a1 = *temp_v0;
            if (temp_a1 < -0x95) {
                D_8013A5E4[var_s3] = 1;
            }
            if (temp_a1 >= 0x96) {
                D_8013A5E4[var_s3] = -1;
            }
            temp_v0_2 = *temp_a0;
            if (temp_v0_2 < -0x6D) {
                D_8013A5EC[var_s3] = 1;
            }
            if (temp_v0_2 >= 0x1E) {
                D_8013A5EC[var_s3] = -1;
            }
            var_s3++;
        } while (var_s3 < 3);
        func_8004A814((s32) D_8013A5D4[0], (s32) D_8013A5DC[0], 0xA, 2, 2, 0x808080, 3, 1);
        func_8004A814((s32) D_8013A5D4[1], (s32) D_8013A5DC[1], 0xA, 2, 2, 0x804040, 3, 1);
        func_8004A814((s32) D_8013A5D4[2], (s32) D_8013A5DC[2], 0xA, 2, 2, 0x800000, 3, 1);
        /* FAKE (T-18800): an empty test (no code) ends the block before the counter test; without it
         * IDO keeps the old count in a1 and branches with bnez instead of the original's v1 and
         * xori/sltiu/beqz (the DATE twin func_801489D0 has real code there). */
        if (var_s3) {
        }
        if (D_8013A5CC++ == 0x96) {
            D_800E6280.unk_1104.u += 1;
            break;
        }
        break;
    case 5:
        temp_s0_2 = D_800E6280.unk_110A;
        temp_s1_3 = D_800E6280.unk_1104.u;
        normal_date_bg_fadeout();
        D_800E6280.unk_1104.u = temp_s1_3;
        if (temp_s0_2 != D_800E6280.unk_110A) {
            D_800E6280.unk_1104.u = temp_s1_3 + 1;
        }
        D_800E6280.unk_110A = temp_s0_2;
        break;
    case 6:
        dec_bg_show_switch(1);
        D_800E6280.unk_1104.u += 1;
        break;
    default:
        func_80044750(0x200);
        D_80122D2C = 0;
        hizuke_disp_switch(1);
        func_8004284C();
        break;
    }
}

void func_801365DC(s32 arg0) {
    s32 v;

    D_800B3D60 = 0;
    v = func_8005742C(arg0, 1);
    if (v == D_800B5939) {
        func_800AE0B0("same\n");
        D_800E6280.unk_1104.w += 3;
        return;
    }
    if (v == 1 - D_800B5939) {
        func_800AE0B0("another\n");
        D_800E6280.unk_1104.w += 2;
    }
}

void func_80136660(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl44 tbl;

    tbl = D_8013A600;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

s32 func_801366E8(void) {
    s32 v;

    v = func_80051A68(0);
    if ((D_800E6280.unk_0F8 & 0xF) == D_800E6280.unk_03F && ((u32)(D_800E6280.unk_0F8 << 0x17) >> 0x1B) == D_800E6280.unk_040) {
        if (((u8)v & 0x7F) < 2U) {
            D_800E6280.unk_71E |= 4;
            D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
            func_80042908(4);
            return 0;
        }
        D_800E6280.unk_71E |= 8;
    }
    func_8004284C();
}

/* The selected girl and the D_800E6280 flag bytes are read through a state
 * pointer: uopt then keeps unk_F5F and unk_71E in registers (a1, v0), which
 * gives the `lui a1; lbu a0,lo(a1)` argument loads. T-11880 */
s32 func_80136794(void) {
    GameState *g;
    s32 v;

    g = &D_800E6280;
    v = func_80051A68(D_800E6280.unk_F5F); /* FAKE: direct symbol here but g-> below; the mix decides which loads uopt keeps in registers T-11880 */
    if ((g->unk_0F8 & 0xF) == g->unk_03F && ((u32)(g->unk_0F8 << 0x17) >> 0x1B) == g->unk_040) {
        if (((u8)v & 0x7F) < 2U) {
            g->unk_71E &= ~8;
            g->unk_71E |= 4;
            g->unk_722 = g->unk_110A + 1;
            g->unk_F5F = g->unk_75D;
            func_800847B8(g->unk_F5F);
            func_80042908(4);
            return 0;
        }
        g->unk_71E |= 8;
    }
    func_8004284C();
}

s32 func_80136860(void) {
    u8 x;
    u8 count;

    if (D_800E6280.unk_110D == 0) {
        func_800674B0();
        D_800E6280.unk_110D++;
    }
    if (D_800E6280.unk_110D == 1) {
        if (D_800E6280.unk_1104.w++ >= 0x400U) {
            func_800452C4();
            func_8004482C();
            D_800E6280.unk_110D = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D++;
        } else {
            return 0;
        }
    }
    x = D_8013A4C4;
    count = ((x & 1) > 0) + ((x & 2) > 0) + ((x & 4) > 0);
    if (count == 0) {
        D_800E6280.unk_1BC[0].unk_06 += 0xC;
        D_800E6280.unk_1BC[D_800E6280.unk_75D].unk_0A += 7;
    } else if (count == 1) {
        D_800E6280.unk_1BC[0].unk_06 += 3;
        D_800E6280.unk_1BC[D_800E6280.unk_75D].unk_0A += 2;
    } else if (count == 2) {
        D_800E6280.unk_1BC[D_800E6280.unk_75D].unk_06 += 3;
        D_800E6280.unk_1BC[0].unk_0A += 2;
    } else {
        D_800E6280.unk_1BC[D_800E6280.unk_75D].unk_06 += 0xC;
        D_800E6280.unk_1BC[0].unk_0A += 7;
    }
    func_80084D3C();
    func_80072B5C(1);
}

void func_80136A90(void) {
    D_8013A430 = 9;
    D_8013A438 = 9;
    func_801338F8();
    if (((u8) func_80051A68(0) & 0x7F) == 4 && (func_800AE0D0() % 2)) {
        D_8013A430 += 1;
    }
    if (((u8) func_80051A68(D_800E6280.unk_75D) & 0x7F) == 4 && (func_800AE0D0() % 2) && (D_800E6280.unk_75D != 6)) {
        D_8013A438 += 1;
    }
    func_8004284C();
}

void func_80136B7C(void) {
    bg_read_sub2(0x432B);
    func_8004284C();
}

void func_80136BA4(void) {
    if (D_800E6280.unk_1BC[0].unk_06 < 0xA) {
        D_8013A428 += 2;
        D_800E6280.unk_F5F = 0;
    } else if (get_h_yuukou(D_800E6280.unk_F5F) < 0xA) {
        D_8013A428 += 2;
    } else if (D_800E6280.unk_1BC[0].unk_0A >= 0x33) {
        D_800E6280.unk_F5F = 0;
    } else if (D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0A < 0x33) {
        if (D_800E6280.unk_1BC[0].unk_02 >= 0x32) {
            D_8013A428 += 1;
            D_800E6280.unk_F5F = 0;
        } else if (get_h_tokimeki(D_800E6280.unk_F5F) >= 0x32) {
            D_8013A428 += 1;
        } else {
            D_8013A428 += 4;
            func_8004284C();
            func_8004284C();
        }
    }
    func_8004284C();
}

void func_80136CEC(void) {
    D_8013A428 = 0x17;
    func_8004284C();
}

void func_80136D14(void) {
    func_800847B8((u8) *(s8 *)&D_800E6280.unk_F5F);
    D_8013A428 = 0x15;
    func_8004284C();
}
