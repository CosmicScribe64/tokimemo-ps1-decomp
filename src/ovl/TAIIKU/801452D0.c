#include "common.h"
#include "ovl/TAIIKU.h"

/* Display record n of the runner sprites (D_8011ECD0 is an array of 0x44-byte records). */
#define DISP(n) (((DisplayActor44 *)D_8011ECD0)[n])
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A3B0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A3B4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A3B8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A3C4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A3D0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A3D4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A3D8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A3DC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A3E0);
TaiikuController D_8014A3E4 = { 0 };
TaiikuController D_8014A400[3] = { 0 };
TaiikuSlot1C D_8014A454[4] = { 0 };

INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A4C4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A4D4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A4D8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A4E4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A4E8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A4F4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A4F8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A528);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A52C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A53C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A54C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A56C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A570);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A5A0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A5C4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A5DC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A5F4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A5FC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A608);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A614);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A620);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A634);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A648);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A658);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/801452D0.data", D_8014A668);

typedef struct {
    void (*f[8])();
} FnTbl8; /* size 0x20 */
extern FnTbl8 D_8014A54C;

typedef struct {
    s32 v[3];
} Lim3; /* size 0xC */

typedef struct TkPl {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ u8 pad4[2];
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ u8 unk_0C;
    /* 0x0D */ u8 unk_0D;
    /* 0x0E */ u8 pad0E[6];
    /* 0x14 */ u16 unk_14;
    /* 0x16 */ u16 unk_16;
    /* 0x18 */ u8 pad18[4];
} TkPl; /* size 0x1C */
extern Lim3 D_8014A5FC;
extern Lim3 D_8014A608;

void func_801452D0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl8 tbl;

    tbl = D_8014A54C;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

/* NON_MATCHING: T-16553, 22 differing lines; in the two loops that store 8 the original keeps the constant 8 in v0 and the loop pointer in v1, the build has them the other way round (registers only) */
#ifdef NON_MATCHING
void func_80145370(void) {
    s32 i;
    s32 j;
    s32 a[1];
    TaiWords12 b;
    TaiWords9 c;

    a[0] = D_8014A56C;
    b = D_8014A570;
    c = D_8014A5A0;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80085E30(1, 1);
        for (i = 0; i < 11; i++) {
            D_800E6280.unk_1228[i + 5] = 8;
        }
        for (i = 0; i < 3; i++) {
            D_800E6280.unk_1228[i + 0x25] = 8;
        }
        D_8014A4F4 = a[0];
        for (i = 0; i < 12; i++) {
            D_8014A4F8[i] = b.v[i];
        }
        D_8014A528[0] = c.v[0];
        for (i = 1; i < 9; i++) {
            D_8014A528[i] = c.v[i];
        }
        func_80048EB8(0);
        func_8006BD6C(0);
        func_8004284C();
        break;
    case 1:
        cd_read_sectors(0x1E, 0x80180000, 0xBAD0);
        func_800862D0();
        func_8004284C();
        break;
    case 2:
        func_800862F0();
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x26, 0x8018F000, 0xB969);
            func_8004284C();
        }
        break;
    case 3:
        if ((func_800862F0() != 0) && (cd_read_sync_status() & 1)) {
            func_80140810();
            func_80132B40();
            search_load_tpage_buf_lock(D_8014A4F4, 1);
            for (j = 0; j < 0x10; j++) {
                load_palette(D_80148EF4, (j + 0x10) & 0xFF, 1, 1, j);
            }
            func_8014607C();
            func_80145EF4();
            func_80145F8C();
            cd_read_sectors(0xB9, 0x8018F000, 0xB8B0);
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}
#else
INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80145370);
#endif

/* NON_MATCHING: T-16553, 8 differing lines; the loop counter i is spilled at sp+0x3C, the original spills it at sp+0x38 (everything else matches) */
#ifdef NON_MATCHING
void func_80145640(void) {
    s32 i;

    switch (D_800E6280.unk_110A) {
    case 0:
        if (cd_read_sync_status() & 1) {
            for (i = 0; i < 0xC; i++) {
                D_8014A4F8[i] = D_800E6280.unk_1228[search_load_tpage_buf_lock(D_8014A4F8[i], 1)] = i + 0x80000100;
            }
            for (i = 5; i < 9; i++) {
                D_8014A528[i] = D_800E6280.unk_1228[search_load_tpage_buf_lock(D_8014A528[i], 1)] = i + 0x80000200;
            }
            func_8004284C();
        }
        break;
    case 1:
        func_80044750(0x201);
        Fade_In_Init();
        func_8004284C();
        break;
    case 2:
        if (Fade_In_Main() != 0) {
            func_8004284C();
        }
        if (D_800E6280.unk_F88 & 0xF0) {
            Fade_Out_Init();
            func_80042940(4);
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        break;
    case 3:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        switch (D_800E6280.unk_110D) {
        case 0:
            func_80046290(0x1F000330, 0x3F000356, 0);
            func_80044750(0x300);
            D_800E6280.unk_110D += 1;
            break;
        case 1:
        case 3:
            if (func_800460EC() == 4) {
                D_800E6280.unk_110D += 1;
            }
            break;
        case 2:
            if (func_800460EC() == 2) {
                func_80046290(0x1F000357, 0x40000360, 0);
                func_80044750(0x300);
                D_800E6280.unk_110D += 1;
            }
            break;
        case 4:
            if (func_800460EC() == 2) {
                Vblnk_Timer_Init();
                D_800E6280.unk_110D += 1;
            }
            break;
        case 5:
            if (Vblnk_Timer() >= 0xB5) {
                D_800E6280.unk_110D = 0;
            }
            break;
        }
        if (D_800E6280.unk_F88 & 0xF0) {
            Fade_Out_Init();
            func_80044750(0x74);
            func_8004284C();
        }
        break;
    case 4:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        if (func_800460EC() == 2) {
            cd_read_sectors(0x87U, 0x8018F000, 0xB98F);
            func_8004284C();
        }
        break;
    case 5:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        if ((Fade_Out_Main() != 0) && (cd_read_sync_status() & 1)) {
            func_80133C30();
            i = 0;
            do {
                search_load_tpage_buf_lock(D_8014A070[i], 1);
                i++;
            } while ((s32 *)&D_8014A088 != &D_8014A070[i]);
            func_8004284C();
        }
        break;
    case 6:
        func_80048F64(0x60);
        func_801330D4();
        D_800E6280.unk_1228[21] = -1;
        func_80132BD0();
        func_80146338();
        func_80044750(0x505);
        Fade_In_Init();
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
}
#else
INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80145640);
#endif

void func_80145A78(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        if (func_80086370() != 0) {
            func_80085ED0();
            func_8004284C();
        }
        func_80133438();
        break;
    case 1:
        if (func_80085EEC() >= 0x5B) {
            func_80044750(0x502);
            func_8004284C();
        }
        func_80133438();
        break;
    case 2:
        func_8014666C();
        func_8014684C();
        func_80146C20();
        func_80132E20(D_8014A3DE, 4);
        func_80147068();
        func_80147520();
        func_80147F10();
        if (func_80146D18() != 0) {
            func_80044750(0x504);
            func_800862D0();
            func_8004284C();
        }
        break;
    case 3:
        func_80085F0C(D_8014A3DE, 0xF, 0, 0x70, 0x280);
        func_80085F0C(D_8014A3DE % 128, 0xF, 0x70, 0x80, 0x80);
        if (func_800862F0() != 0) {
            func_80044750(0xC2);
            func_80148370();
            func_80133258();
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80145C28(void) {
    s32 i;
    s32 j; /* declared before p: the spill slot of p and the 0x48 frame depend on it (T-16553) */
    u8 *p;

    Scroll(0, 0xF, 0, 0xF0, 0x280);
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80044750(0x50B);
        Fade_In_Init();
        func_8004284C();
        break;
    case 1:
        if (Fade_In_Main() != 0) {
            if ((D_8014A3D8[0] == 1) || (D_8014A3D8[1] == 1)) {
                func_80044750(0x205);
            } else {
                func_80044750(0x206);
            }
            func_8004284C();
        }
        break;
    case 2:
        if (func_801485C4() != 0) {
            func_8004284C();
        }
        break;
    case 3:
        if (func_80148678() != 0) {
            func_8004284C();
        }
        break;
    case 4:
        func_801487B4();
        p = D_8014A3D8;
        for (i = 0; i < 4; i++) {
            if (*p == 1) {
                func_8013384C(0xB, i & 0xFF);
            }
            p++;
        }
        func_8004284C();
        break;
    case 5:
        func_80148820();
        break;
    case 6:
        Vblnk_Timer_Init();
        func_8004284C();
        break;
    case 7:
        if (Vblnk_Timer() >= 0xF1) {
            Fade_Out_Init();
            func_8004284C();
        }
        break;
    case 8:
        if (Fade_Out_Main() != 0) {
            if (D_8014A3D8[0] == 1) {
                for (j = 0; j < 13; j++) {
                    D_800E6280.unk_1BC[j].unk_02 += 5;
                }
            } else if (D_8014A3D8[1] == 1) {
                for (j = 0; j < 13; j++) {
                    D_800E6280.unk_1BC[j].unk_02 += 3;
                }
            }
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80145EF4(void) {
    s32 i;

    func_8004E44C(0, D_80148EF8, D_80148EF0);
    srn_init(0, D_80148EFC, 0);
    srn_vram_set(0, 5, 0x580, 0xF0);
    for (i = 0; i < 3; i++) {
        D_800E6280.unk_1228[i + 5] = 8;
    }
    for (i = 0; i < 3; i++) {
        D_800E6280.unk_1228[i + 0x25] = 8;
    }
}

void func_80145F8C(void) {
    func_80048F64(0x60);
    D_80120651 = 3;
    D_80120688 = 0x01000000;
    D_80120652 = 1;
    D_80120653 = 0xA4;
    D_80120657 = 0x80;
    D_8012065C = (u8 *) D_80149FD0;
    D_80120660 = (u8 *) D_8014A008;
    D_80120684 = (u8 *)&D_8014A4F4;
    D_80120664 = D_8014A040;
    D_80120654 = 0;
    D_80120655 = 0x80;
    D_80120666 = 0;
    D_80120676 = 0x20;
    D_8012067A = 0x10;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
}

/* NON_MATCHING: T-16553, 22 differing lines; the tail loads D_800E638E into a1 and the masked D_800E6375 into t5, the original loads it into v0 (the variable itself) and keeps the mask in v1 */
#ifdef NON_MATCHING
void func_8014607C(void) {
    s32 i;
    s32 var_v0_2;
    s32 temp_v1;

    D_8014A3E4.unk_00 = 0;
    D_8014A3E4.unk_08 = 0;
    D_8014A3E4.unk_0C = 0;
    D_8014A3E4.unk_04 = 0;
    D_8014A3E4.position.h[0] = 0;
    D_8014A3E4.unk_14 = 0;
    D_8014A3E4.unk_01 = 0;
    D_8014A3E4.unk_02 = 0;
    for (i = 0; i < 3; i++) {
        D_8014A400[i].position.h[1] = (i + 1) * 8;
        D_8014A400[i].unk_0C = 0;
        D_8014A400[i].unk_04 = 0;
        D_8014A400[i].unk_00 = 0;
        D_8014A400[i].unk_14 = 0;
        D_8014A400[i].position.h[0] = 0;
        D_8014A400[i].unk_01 = 0;
    }
    for (i = 0; i < 4; i++) {
        D_8014A3D8[i] = 0;
        D_8014A454[i].unk_18 = 0;
        D_8014A454[i].unk_02 = 0x37;
        D_8014A454[i].unk_06 = 0x19;
        D_8014A454[i].unk_08 = 0;
        D_8014A454[i].unk_14 = 0;
        D_8014A454[i].unk_16 = 0;
        D_8014A454[i].unk_0C = 0;
        D_8014A454[i].unk_0D = 0;
        D_8014A454[i].unk_0E = 0;
        D_8014A454[i].unk_0F = 0;
    }
    D_8014A3E4.position.h[1] = -0xA0;
    D_8014A3E0 = 0;
    D_8014A3DC = 0;
    var_v0_2 = D_800E6280.unk_10C.unk_02;
    temp_v1 = D_800E6280.unk_0F4.b[1] & 0xF;
    D_8014A3C4 = 0;
    if ((temp_v1 & 1) || (temp_v1 & 2)) {
        var_v0_2 = (s16) (var_v0_2 >> 1);
    }
    if (var_v0_2 >= 0xA0) {
        D_8014A3D0 = 0x280;
        D_8014A3D4 = 0x20000;
    } else if (var_v0_2 >= 0x7D) {
        D_8014A3D0 = 0x220;
        D_8014A3D4 = 0x1E000;
    } else if (var_v0_2 >= 0x64) {
        D_8014A3D0 = 0x1C0;
        D_8014A3D4 = 0x1C000;
    } else if (var_v0_2 >= 0x4B) {
        D_8014A3D0 = 0x160;
        D_8014A3D4 = 0x1A000;
    } else {
        D_8014A3D0 = 0x100;
        D_8014A3D4 = 0x18000;
    }
    func_800AE120(D_800E6280.unk_10F4);
}
#else
INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_8014607C);
#endif

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80146338);

s32 func_8014666C(void) {
    TaiikuController *controller;
    s32 i;

    controller = &D_8014A3E4;
    controller->unk_0C++;
    controller->unk_08++;
    controller->unk_04++;
    controller->unk_02++;
    i = 0;
    /* the pointer bound keeps the original's sltu and lui order; `i < 3` gives bne (T-16553) */
    do {
        D_8014A400[i].unk_0C++;
        D_8014A400[i].unk_04++;
        i++;
    } while (&D_8014A400[i] < (TaiikuController *)D_8014A454);
    D_8014A454[0].unk_18++;
    D_8014A454[0].unk_14++;
    D_8014A454[0].unk_16++;
    D_8014A454[0].unk_0D++;
    D_8014A454[0].unk_0F++;
    D_8014A454[1].unk_18++;
    D_8014A454[1].unk_14++;
    D_8014A454[1].unk_16++;
    D_8014A454[1].unk_0D++;
    D_8014A454[1].unk_0F++;
    D_8014A454[2].unk_18++;
    D_8014A454[2].unk_14++;
    D_8014A454[2].unk_16++;
    D_8014A454[2].unk_0D++;
    D_8014A454[2].unk_0F++;
    D_8014A454[3].unk_18++;
    D_8014A454[3].unk_14++;
    D_8014A454[3].unk_16++;
    D_8014A454[3].unk_0D++;
    D_8014A454[3].unk_0F++;
}

void func_8014684C(void) {
    func_80146884();
    func_8014692C();
    func_80146994();
    func_80146A60();
}

void func_80146884(void) {
    if (D_8014A454[0].unk_0C != 0) {
        D_8014A3E4.unk_10 = -0x300;
        return;
    }
    if (D_800E6280.unk_F88 & 0x20) {
        D_8014A3C4 = 1;
        return;
    }
    if (D_8014A3E4.unk_08 >= 0x10) {
        if (D_8014A3C4 != 0) {
            if (D_8014A3E4.unk_14 < 0) {
                D_8014A3E4.unk_14 = 0;
            }
            D_8014A3E4.unk_10 = D_8014A3D0;
        } else {
            D_8014A3E4.unk_10 = -0x300;
        }
        D_8014A3E4.unk_08 = 0;
        D_8014A3C4 = 0;
    }
}

void func_8014692C(void) {
    if ((u32)D_8014A3E4.unk_0C >= 0xB) {
        D_8014A3E4.unk_14 += D_8014A3E4.unk_10;
        if (D_8014A3E4.unk_14 < -0x10000) {
            D_8014A3E4.unk_14 = -0x10000;
        }
        if (D_8014A3D4 < D_8014A3E4.unk_14) {
            D_8014A3E4.unk_14 = D_8014A3D4;
        }
        D_8014A3E4.unk_0C = 0;
    }
}

void func_80146994(void) {
    if ((D_80120668 != 3) && ((D_8014A454[0].unk_0C == 0) || (D_8014A3E0 != 0))) {
        if ((D_8014A3E4.unk_10 < 0) || (D_8014A3DE == 0)) {
            D_8014A3E4.position.w -= D_8014A3E0;
        } else {
            D_8014A3E4.position.w += 0x8000;
        }
    }
    if (D_8014A3E4.position.h[1] >= -0x1F) {
        D_8014A3E4.position.h[1] = -0x20;
    }
    if (D_8014A3E4.position.h[1] < -0x9F) {
        D_8014A3E4.position.h[1] = -0xA0;
    }
    D_80120676 = D_8014A3E4.position.h[1];
}

void func_80146A60(void) {
    s32 flags;

    if (D_8014A454[0].unk_0C == 0) {
        func_80146B8C();
        flags = D_800E6280.unk_F80;
        if ((flags & 0xA) && !(flags & 5)) {
            if (D_8014A3E4.unk_02 >= 0x15 && D_8014A3E0 > 0) {
                func_80044750(0x507);
                D_8014A3E4.unk_02 = 0;
            }
            D_80120668 = D_8014A4D4[D_8014A3E4.unk_00];
            return;
        }
        if ((flags & 5) && (flags & 0xA) == 0) {
            if (D_8014A3E4.unk_02 >= 0x15 && D_8014A3E0 > 0) {
                func_80044750(0x508);
                D_8014A3E4.unk_02 = 0;
            }
            D_80120668 = D_8014A4E4[D_8014A3E4.unk_00];
            return;
        }
        D_80120668 = D_8014A4C4[D_8014A3E4.unk_00];
    }
}

void func_80146B8C(void) {
    if (D_8014A3E0 == 0) {
        D_8014A3E4.unk_00 = 0;
        return;
    }
    if (((u32)D_8014A3E4.unk_04 >= 6 && D_8014A3E4.unk_10 > 0) || ((u32)D_8014A3E4.unk_04 >= 0xB && D_8014A3E4.unk_14 != 0)) {
        if (D_8014A3E4.unk_00 >= 7) {
            D_8014A3E4.unk_00 = 2;
            D_8014A3E4.unk_04 = 0;
        } else {
            D_8014A3E4.unk_00++;
            D_8014A3E4.unk_04 = 0;
        }
    }
}

void func_80146C20(void) {
    D_8014A3E0 += D_8014A3E4.unk_10;
    if (D_8014A3D4 < D_8014A3E0) {
        D_8014A3E0 = D_8014A3D4;
    }
    if (D_8014A3E0 < 0 || (D_8014A3E4.unk_10 < 0 && D_8014A3E4.position.h[1] < -0x9F)) {
        D_8014A3E0 = 0;
    }
    D_8014A3DC += D_8014A3E0; /* the fixed-point high half (+2) is read as an s16 below */
    func_80085F0C(((s16 *)&D_8014A3DC)[1] / 2 % 640, 0xF, 0, 0x70, 0x280);
    func_80085F0C(((s16 *)&D_8014A3DC)[1] % 128, 0xF, 0x70, 0x80, 0x80);
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80146D18);

/* FAKE: v[6] as in func_8013815C. T-0016 */
s32 func_80146FA0(void) {
    s16 v[6];
    s32 i;

    for (i = 0; i < 3; i++) {
        if (D_8014A400[i].unk_01 == 0) {
            v[i] = *(s16 *)(D_8011ECD0 + i * 0x44 + 0x19EA);
        } else {
            v[i] = -0xF00;
        }
    }
    if (v[0] >= v[1] && v[0] >= v[2]) {
        D_8014A400[0].unk_01 = 1;
        return 2;
    }
    if (v[1] >= v[2]) {
        D_8014A400[1].unk_01 = 1;
        return 3;
    }
    D_8014A400[2].unk_01 = 1;
    return 4;
}

void func_80147068(void) {
    func_801470A0();
    func_80147120();
    func_80147210();
    func_80147318();
}

void func_801470A0(void) {
    s32 i;
    Lim3 lim;

    lim = D_8014A5FC;
    for (i = 0; i < 3; i++) {
        if (D_8014A454[i + 1].unk_0C != 0) {
            D_8014A400[i].unk_10 = -0x4000;
        } else {
            D_8014A400[i].unk_10 = lim.v[i];
        }
    }
}

void func_80147120(void) {
    s32 i;
    Lim3 lim;

    lim = D_8014A608;
    for (i = 0; i < 3; i++) {
        if (D_8014A400[i].unk_01 == 1 && D_80120896 + 0x140 < *(s16 *)(D_8011ECD0 + i * 0x44 + 0x19EA)) {
            D_8014A400[i].unk_14 = 0;
        } else if (D_8014A400[i].unk_0C >= 0xB) {
            D_8014A400[i].unk_14 += D_8014A400[i].unk_10;
            if (D_8014A400[i].unk_14 < 0) {
                D_8014A400[i].unk_14 = 0;
            }
            if (lim.v[i] < D_8014A400[i].unk_14) {
                D_8014A400[i].unk_14 = lim.v[i];
            }
            D_8014A400[i].unk_0C = 0;
        }
    }
}


void func_80147210(void) {
    s32 i;
    s32 k;
    TaiikuController *v;
    u8 *a;
    s32 d;
    s32 x;

    for (i = 0, v = D_8014A400, a = D_8011ECD0, d = D_8014A3DE; i < 3; i++, v++, a += 0x44) {
        v->position.w += v->unk_14;
        x = v->position.h[1] - d;
        *(s16 *)(a + 0x19EA) = x - 0xA0;
        if (D_80120896 + (i + 1) * 8 < *(s16 *)(a + 0x19EA) + 0x40 && v->unk_01 == 0) {
            D_801208CC = 0x4F;
            k = 0;
            if (D_8014A3D8[0] != 0) {
                do {
                    k += 1;
                } while (D_8014A3D8[k] != 0);
            }
            D_8014A3D8[k] = i + 2;
            v->unk_01 = 1;
            x = v->position.h[1] - d;
        }
        if (x >= 0x1E1 || x < -0xA0) {
            a[0x19C7] = 0;
        } else {
            a[0x19C7] = 0xA4;
        }
    }
}

void func_80147318(void) {
    s32 i;
    s32 off;
    DisplayActor44 *rec;

    for (i = 0; i < 3; i++) {
        switch (D_8014A454[i + 1].unk_0C) {
        case 0:
            if (D_8014A400[i].unk_04 >= 6) {
                rec = &((DisplayActor44 *)D_8011ECD0)[i];
                off = i * 0x13 + 0x13;
                if (D_8014A400[i].unk_00 >= 7) {
                    D_8014A400[i].unk_00 = 2;
                    D_8014A400[i].unk_04 = 0;
                } else {
                    D_8014A400[i].unk_00 += 1;
                    D_8014A400[i].unk_04 = 0;
                }
                if (D_8014A454[i + 1].unk_08 > 0) {
                    rec[97].unk_18 = D_8014A4D4[D_8014A400[i].unk_00] + off;
                } else if (D_8014A454[i + 1].unk_08 < 0) {
                    rec[97].unk_18 = D_8014A4E4[D_8014A400[i].unk_00] + off;
                } else {
                    rec[97].unk_18 = D_8014A4C4[D_8014A400[i].unk_00] + off;
                }
            }
            break;
        case 1:
            if (D_8014A454[i + 1].unk_0D < 0x32) {
                ((DisplayActor44 *)D_8011ECD0)[97 + i].unk_18 = i * 2 + 0x53;
            }
            break;
        case 2:
            if (D_8014A454[i + 1].unk_0D < 0x32) {
                ((DisplayActor44 *)D_8011ECD0)[97 + i].unk_18 = i * 2 + 0x54;
            }
            break;
        default:
            func_80046500();
            break;
        }
    }
}

void func_80147520(void) {
    s32 var_s0;

    func_8014756C();
    var_s0 = 0;
    do {
        func_80147A6C(var_s0);
        func_80147B14(var_s0);
        var_s0 += 1;
    } while (var_s0 != 4);
}

void func_8014756C(void) {
    func_80147594();
    func_80147928();
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80147594);

void func_80147928(void) {
    s32 r;
    s32 bonus;
    u32 chance;

    if ((D_8014A3E4.unk_14 > 0) || (D_8014A3E0 != 0)) {
        if (D_8014A454[0].unk_14 >= 0x65 && D_8014A454[0].unk_08 == 0) {
            r = func_800AE0D0();
            bonus = (D_8014A3E4.unk_14 == D_8014A3D4) ? 0xF : 0;
            chance = (r % 100) & 0xFFFF;
            if (chance < bonus + 0x19) {
                D_8014A454[0].unk_08 = -0x64;
                D_8014A454[0].unk_14 = 0;
            } else if (chance < bonus + 0x32) {
                D_8014A454[0].unk_08 = 0x64;
                D_8014A454[0].unk_14 = 0;
            }
        }
        if ((D_800E6280.unk_F80 & 0xA) && !(D_800E6280.unk_F80 & 5)) {
            D_8014A454[0].unk_08 -= 0xB;
            if (D_8014A454[0].unk_08 == 0) {
                D_8014A454[0].unk_14 = 0;
            }
        }
        if (D_800E6280.unk_F80 & 5) {
            if ((D_800E6280.unk_F80 & 0xA) == 0) {
                D_8014A454[0].unk_08 += 0xB;
                if (D_8014A454[0].unk_08 == 0) {
                    D_8014A454[0].unk_14 = 0;
                }
            }
        }
    }
}

void func_80147A6C(s32 index) {
    TaiikuSlot1C *slot;

    slot = &D_8014A454[index];
    /* FAKE: this empty test preserves the slot pointer's register range. T-10020. */
    if (!slot) {
    }
    if (slot->unk_0C == 0 && (index != 0 || D_8014A3E4.unk_10 > 0 || D_8014A3E0 != 0) && slot->unk_16 >= 6) {
        if (slot->unk_08 > 0) {
            slot->unk_02++;
            slot->unk_16 = 0;
        }
        if (slot->unk_08 < 0) {
            slot->unk_16 = 0;
            slot->unk_02--;
        }
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/801452D0", func_80147B14);

void func_80147F10(void) {
    s32 var_s0;

    var_s0 = 0;
    do {
        func_80147F58(var_s0);
        func_801482BC(var_s0);
        var_s0 += 1;
    } while (var_s0 != 4);
}

void func_80147F58(s32 arg0) {
    s32 i; /* declared before tbl: the stack table sits 4 bytes below the locals area top (T-3330, T-16553) */
    TaiikuHalf8 tbl;

    tbl = D_8014A668;
    if (D_8014A454[arg0].unk_0C == 0) {
        i = 0;
        do {
            if (DISP(arg0 + 96).unk_18 == D_8014A4C4[i] + arg0 * 0x13) {
                if (tbl.h[i] + DISP(arg0 + 96).unk_26 + 0x46 < DISP(arg0 + 100).unk_26) {
                    func_80148194(arg0);
                }
                if (DISP(arg0 + 100).unk_26 < tbl.h[i] + DISP(arg0 + 96).unk_26 + 0x23) {
                    func_80148228(arg0);
                }
            }
            i++;
        } while (&D_8014A4C4[i] < D_8014A4D4); /* pointer bounds: the next table symbol, as in the original (sltu) */
        i = 0;
        do {
            if (DISP(arg0 + 96).unk_18 == D_8014A4D8[i] + arg0 * 0x13) {
                if (DISP(arg0 + 96).unk_26 + 0x5E < DISP(arg0 + 100).unk_26) {
                    func_80148194(arg0);
                }
                if (DISP(arg0 + 100).unk_26 < DISP(arg0 + 96).unk_26 + 0x3B) {
                    func_80148228(arg0);
                }
            }
            i++;
        } while (&D_8014A4D8[i] < D_8014A4E4);
        i = 0;
        do {
            if (DISP(arg0 + 96).unk_18 == D_8014A4E8[i] + arg0 * 0x13) {
                if (DISP(arg0 + 96).unk_26 + 0x3C < DISP(arg0 + 100).unk_26) {
                    func_80148194(arg0);
                }
                if (DISP(arg0 + 100).unk_26 < DISP(arg0 + 96).unk_26 + 0x19) {
                    func_80148228(arg0);
                }
            }
            i++;
        } while ((u16 *)&D_8014A4F4 != &D_8014A4E8[i]);
    }
}

void func_80148194(s32 arg0) {
    u8 *temp_v0;
    TaiikuSlot1C *temp_v1;

    temp_v1 = &D_8014A454[arg0];
    temp_v0 = (arg0 * 0x44) + D_8011ECD0;
    temp_v1->unk_0C = 1;
    temp_v1->unk_0D = 0;
    *(s16 *)(temp_v0 + 0x1998) = (s16) ((arg0 * 2) + 0x51);
    *(s16 *)(temp_v0 + 0x1AB6) = (s16) (*(s16 *)(temp_v0 + 0x19A6) + 0x74);
    *(s16 *)(temp_v0 + 0x1ABA) = (s16) (*(s16 *)(temp_v0 + 0x19AA) + 0x32);
    temp_v1->unk_02 = 0x6A;
    temp_v1->unk_06 = 0x32;
    if (arg0 == 0) {
        func_80044750(0x503);
    }
}

void func_80148228(s32 arg0) {
    u8 *temp_v0;
    TaiikuSlot1C *temp_v1;

    temp_v1 = &D_8014A454[arg0];
    temp_v0 = (arg0 * 0x44) + D_8011ECD0;
    temp_v1->unk_0C = 2;
    temp_v1->unk_0D = 0;
    *(s16 *)(temp_v0 + 0x1998) = (s16) ((arg0 * 2) + 0x52);
    *(s16 *)(temp_v0 + 0x1AB6) = (s16) (*(s16 *)(temp_v0 + 0x19A6) - 0x17);
    *(s16 *)(temp_v0 + 0x1ABA) = (s16) (*(s16 *)(temp_v0 + 0x19AA) + 0x32);
    temp_v1->unk_02 = -0x17;
    temp_v1->unk_06 = 0x32;
    if (arg0 == 0) {
        func_80044750(0x503);
    }
}

void func_801482BC(s32 index) {
    TaiikuController *controller;
    TaiikuSlot1C *slot;

    slot = &D_8014A454[index];
    if (slot->unk_0C != 0 && slot->unk_0D >= 0x65U) {
        slot->unk_0C = 0;
        if (index == 0) {
            D_8014A3E4.unk_00 = 0;
            D_8014A3E4.unk_08 = 0;
            D_8014A3E4.unk_0C = 0;
            D_8014A3E4.unk_04 = 0;
            D_8014A3E4.unk_14 = 0;
        } else {
            controller = &D_8014A400[index - 1];
            controller->unk_00 = 0;
            controller->unk_08 = 0;
            controller->unk_0C = 0;
            controller->unk_04 = 0;
            controller->unk_14 = 0;
        }
        slot->unk_02 = 0x37;
        slot->unk_06 = 0x19;
        slot->unk_08 = 0;
        slot->unk_14 = 0;
        slot->unk_16 = 0;
        slot->unk_0D = 0;
    }
}

s32 func_80148370(void) {
    s32 i;

    func_80048E78();
    for (i = 0; i < 9; i++) {
        DISP(96 + i).unk_38 = 0x01000000;
        DISP(96 + i).unk_07 = 0x80;
        DISP(96 + i).unk_02 = 0x40;
        DISP(96 + i).unk_03 = 0xA4;
        DISP(96 + i).unk_0C = (u8 *)D_80149FD8;
        DISP(96 + i).unk_10 = (u8 *)D_8014A010;
        DISP(96 + i).unk_34 = (u8 *)&D_8014A528;
        DISP(96 + i).unk_14 = D_8014A048;
        DISP(96 + i).unk_04 = 0;
        DISP(96 + i).unk_05 = 0x80;
        DISP(96 + i).unk_1A = 0x1000;
        *(s16 *)DISP(96 + i).pad_1C = 0x1000;
        DISP(96 + i).unk_06 = 2;
    }
    for (i = 0; i < 4; i++) {
        DISP(96 + i).unk_01 = 3;
        DISP(96 + i).unk_18 = D_8014A3D8[i] * 5 + 0x1D;
        DISP(96 + i).unk_2A = -8;
        DISP(96 + i).unk_26 = i * 0x30 - 0x70;
    }
    for (i = 0; i < 4; i++) {
        DISP(100 + i).unk_01 = 3;
        DISP(100 + i).unk_18 = D_8014A3D8[i] * 5 + 0xD;
        DISP(100 + i).unk_26 = DISP(96 + i).unk_26;
        DISP(100 + i).unk_2A = DISP(96 + i).unk_2A;
    }
    DISP(104).unk_01 = 3;
    DISP(104).unk_16 = 9;
    DISP(104).unk_26 = 0xC0;
    DISP(104).unk_2A = -0x20;
    DISP(104).unk_02 = 1;
}

s32 func_801485C4(void) {
    s32 index;
    s32 chosen;
    u8 *p;

    p = D_8014A3D8;
    index = 0;
    do {
        if (*p == 1) {
            chosen = index;
        }
        index++;
        p++;
    } while (index < 4);
    if (chosen * 48 - 64 < ((TaiikuScene44 *)D_8011ECD0)[104].position.h[1]) {
        ((TaiikuScene44 *)D_8011ECD0)[104].position.w += 0xFFFE8000;
        return 0;
    }
    ((TaiikuScene44 *)D_8011ECD0)[104].unk_02 = 64;
    ((TaiikuScene44 *)D_8011ECD0)[104].unk_18 = 61;
    func_80085ED0();
    return 1;
}

s32 func_80148678(void) {
    s32 i;
    s32 n;

    if (func_80085EEC() >= 0x2E) {
        *(s16 *)&D_8011ECD0[0x1BB8] = 0x3C;
        *(s16 *)&D_8011ECD0[0x1BCA] = -8;
        for (i = 0; i < 4; i++) {
            if (D_8014A3D8[i] == 1) {
                n = i;
            }
        }
        for (i = 0; i < 4; i++) {
            if (n < i) {
                *(s16 *)&D_8011ECD0[0x19A6 + i * 0x44] += 0x30;
                *(s16 *)&D_8011ECD0[0x19A6 + 0x110 + i * 0x44] += 0x30;
            }
        }
        return 1;
    }
    return 0;
}

void func_801487B4(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        *(s16 *)&D_8011ECD0[0x1AA8 + i * 0x44] -= 4 - i;
    }
    func_80085ED0();
}

void func_80148820(void) {
    s32 i;
    s32 n;

    for (i = 0; i < 4; i++) {
        if (D_8014A3D8[i] == 1) {
            n = i;
        }
    }
    if (func_80085EEC() >= 0x33) {
        for (i = 0; i < 4; i++) {
            *(s16 *)&D_8011ECD0[0x1998 + i * 0x44] += i + 1;
        }
        *(s16 *)&D_8011ECD0[0x1BB8] = *(s16 *)&D_8011ECD0[0x1BB8] + n + 2;
        func_8004284C();
    }
}
