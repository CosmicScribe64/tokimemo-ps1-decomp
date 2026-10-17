#include "common.h"
#include "libgte.h"
#include "ovl/DATE.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
s16 D_8015DDD0[0x2C] = { 0 };
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/8014A1B0.data", D_8015DE28);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/8014A1B0.data", D_8015DE2C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/8014A1B0.data", D_8015DE30);
s32 D_8015DE34[3] = { 0, 0, 0 };

void func_8014A1B0(void) {
    D_8015DE28 = 0x80195000;
    D_8015DE2C = 0x80195060;
    D_8015DE30 = 0x801950C0;
}

void func_8014A1E4(void) {
    /* The dispatcher reads the selector as an unsigned halfword. */
    switch (*(u16 *)&D_8015DE40) {
    case 0:
        func_8014A280();
        return;
    case 1:
        func_8014A2AC();
        return;
    case 2:
        func_8014A314();
        return;
    case 3:
        func_8014A360();
        return;
    case 4:
        func_8014A39C();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_8014A280(void) {
    if (D_8015DE44 == 0) {
        func_8014A4DC();
    }
}

void func_8014A2AC(void) {
    switch (D_8015DE44) {
    case 0:
        func_8014A810();
        return;
    case 1:
        func_8014A8FC();
        return;
    case 2:
        func_8014AD70();
        return;
    }
}

void func_8014A314(void) {
    func_80044750(0x200);
    read_bustup();
    D_800E6280.unk_110A -= 1;
    D_8015DE40 += 1;
}

void func_8014A360(void) {
    if (cd_read_sync_status() & 1) {
        D_8015DE40 += 1;
    }
}

s32 func_8014A39C(void) {
    s32 var_v1;
    u8 r;

    if (*(s32 *)D_800CA120 != 0x801820AC || *(s32 *)(D_800CA120 + 4) != 0x801860AC || *(s32 *)(D_800CA120 + 8) != 0x8018A0AC
        || *(s32 *)(D_800CA120 + 0xC) != 0x8018E0AC || *(s32 *)(D_800CA120 + 0x10) != 0x801920AC || *(s32 *)D_800CA128 != 0x8018005C
        || *(s32 *)(D_800CA128 + 4) != 0x80180070 || *(s32 *)D_800CA124 != 0x80180000) {
        D_8015DE40 -= 2;
        return 0;
    }
    r = func_80051B48(D_800E6280.unk_F5F);
    var_v1 = r & 0x7F;
    if (D_800E6280.unk_F5F >= 0xCU) {
        var_v1 = 0;
    }
    func_80062DBC(D_800CA130, D_800CA120, D_800CA124, D_800CA128, var_v1);
    func_8006492C(1);
    func_8004284C();
}

void func_8014A4DC(void) {
    u8 *var_s1;
    s32 var_s0;

    switch (*(u16 *)&D_8015DE48) {
    case 0:
        func_8014A1B0();
        D_8015DE34[0] = D_8015DE28;
        D_8015DE34[1] = D_8015DE2C;
        D_8015DE34[2] = D_8015DE30;
        func_800462C8(1U, 0x80195000, 0xBD9E);
        D_8015DE48 += 1;
        return;
    case 1:
        if (cd_read_sync_status() & 1) {
            D_8015DE48 += 1;
            return;
        }
    default:
        return;
    case 2:
        func_80044750(0xCF);
        func_80044750(0x202);
        func_80048390();
        func_800438F0(1);
        func_80048DAC(1);
        D_800E6280.unk_037 = 0;
        D_800E6280.unk_036 = 0;
        D_800E6280.unk_038 = 0;
        func_8006492C(0);
        func_800462C8(2U, 0x801F8000, 0xB471);
        D_8015DE48 += 1;
        return;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_800462C8(1U, 0x801F9000, 0xB470);
            D_8015DE48 += 1;
            return;
        }
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_800438DC(1, 1);
            func_80058D20();
            func_8014B080();
            func_80058EB0();
            func_80058F0C((s32 *)0x801F8000);
            func_8014AF88();
            func_800462C8(0x21U, 0x80180000, 0xB57F);
            D_8015DE48 += 1;
            return;
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_800462C8(1U, 0x80190800, 0xB456);
            D_8015DE48 += 1;
            return;
        }
        break;
    case 6:
        if (cd_read_sync_status() & 1) {
            func_80043914(0x80190800, 0x10, 1, 2, 0);
            var_s0 = 0;
            var_s1 = (u8 *)0x80180000;
            do {
                func_80048514(var_s0 + 5, var_s1, 1);
                var_s0 += 1;
                var_s1 += 0x4000;
            } while (var_s0 < 2);
            var_s1 = (u8 *)0x80188000;
            var_s0 = 0;
            do {
                func_80048514(var_s0 + 0x25, var_s1, 1);
                var_s0 += 1;
                var_s1 += 0x4000;
            } while (var_s0 != 2);
            func_800462C8(9U, 0x80180000, 0xB467);
            D_8015DE48 += 1;
            return;
        }
        break;
    case 7:
        if (cd_read_sync_status() & 1) {
            func_80043914(0x80180000, 0x11, 1, 2, 0);
            func_80043A00(0x80180200, 7, 1, 0, 0, 0x80, 0x80);
            func_8014B194();
        }
        break;
    }
}

void func_8014A810(void) {
    s32 i;
    s32 j;

    func_8014AD9C(0, D_8015DE34[0], 0);
    D_80122640[0] = 0;
    D_8015DDD0[0] = 0x32;
    D_8015DDD0[2] = 0x32;
    D_8015DDD0[1] = 0xD2;
    func_8014AD9C(1, D_8015DE38, 0);
    D_80122640[4] = 0;
    D_8015DDD0[4] = 0x4B;
    D_8015DDD0[6] = -0x32;
    D_8015DDD0[5] = 0xC3;
    for (i = 2, j = 0; i != 0xB; i++) {
        func_8014AD9C(i, D_8015DE3C, j);
        j++;
        D_80122640[i * 4] = 0;
    }
    D_8015DE4C = 0;
    func_8014B1C4();
}

#ifdef NON_MATCHING
/* NON_MATCHING: T-22006, 20 rows (10 per fade block); registers and the order of the sums now match, only the schedule of the byte loads differs: the original loads 6E, 8E and 7E in a row before the first sum, IDO loads each byte just before its group. */
/* FAKE: s32 without a return keeps the s16 counter in $v0 at the exit (sll/sra before the sh), the real return type is unknown. T-17208 */
s32 func_8014A8FC(void) {
    s32 frame;
    s32 i;
    s32 j;

    message_window_show();
    func_80066C08(2);
    func_80066334();
    frame = D_8015DE4C;
    if (frame < 0x20) {
        if (D_8012276C < 0xC0U) {
            D_8012276C = D_8012276D = (D_8012276E += 6);
            D_8012277C = D_8012277D = (D_8012277E += 1);
            /* FAKE: the two equal masked sums make the third group a uopt range ($v0, the bytes get $v1/$a2/$a3) and keep its ugen slot, as in the original. T-22006 */
            D_8012278D = (D_8012278E + 1) & 0xFF;
            D_8012278C = D_8012278E = (D_8012278E + 1) & 0xFF;
            func_8009AD70(0, &D_80122760);
            func_8009AD70(1, &D_80122770);
            func_8009AD70(2, &D_80122780);
        }
        if (frame < 0xF) {
            D_800E6280.unk_038 += 1;
        }
    } else if (frame >= 0x20 && frame < 0x54) {
        for (i = 0; i != 2; i++) {
            func_8014AD9C(i, D_8015DE34[i], 1);
        }
    } else if (frame >= 0x54 && frame < 0x68) {
        for (i = 0; i != 2; i++) {
            func_8014AD9C(i, D_8015DE34[i], 2);
        }
    } else if (frame >= 0x68 && frame < 0x72) {
        for (i = 0; i != 2; i++) {
            func_8014AD9C(i, D_8015DE34[i], 3);
        }
    } else if (frame >= 0x72 && frame < 0x7C) {
        for (i = 0; i != 2; i++) {
            func_8014AD9C(i, D_8015DE34[i], 4);
        }
    } else if (frame >= 0x7C && frame < 0x9A) {
        for (i = 0; i != 2; i++) {
            func_8014AD9C(i, D_8015DE34[i], 5);
        }
    } else if (frame >= 0x9A && frame < 0xAE) {
        for (i = 0; i != 2; i++) {
            func_8014AD9C(i, D_8015DE34[i], 6);
        }
    } else if (frame >= 0xAE && frame < 0xEA) {
        for (i = 0; i != 2; i++) {
            func_8014AD9C(i, D_8015DE34[i], 7);
        }
    } else if (frame >= 0x10E && frame < 0x12E) {
        if (D_8012276C != 0) {
            D_8012276C = D_8012276D = (D_8012276E -= 6);
            D_8012277C = D_8012277D = (D_8012277E -= 1);
            /* FAKE: the two equal masked sums make the third group a uopt range ($v0, the bytes get $v1/$a2/$a3) and keep its ugen slot, as in the original. T-22006 */
            D_8012278D = (D_8012278E - 1) & 0xFF;
            D_8012278C = D_8012278E = (D_8012278E - 1) & 0xFF;
            func_8009AD70(0, &D_80122760);
            func_8009AD70(1, &D_80122770);
            func_8009AD70(2, &D_80122780);
        }
        if (frame < 0x11D) {
            D_800E6280.unk_038 -= 1;
        }
        /* FAKE: the byte view makes this second read of D_8012276C a reload; with the plain global uopt promotes it into $v0 and the first test is no longer a $t2 load. T-17208 */
        if (*(u8 *)&D_8012276C == 0) {
            /* The nine ordering-table heads are addressed from D_80122660 (stride 0x10), not from D_80122640. */
            for (i = 0; i < 9; i++) {
                ((u32 *)D_80122660)[i * 4] = 0x80000000;
            }
        }
    } else if (frame >= 0x138) {
        func_8014B1C4();
    }
    for (i = 2, j = 9; i != 0xB; i++, j++) {
        func_8014AD9C(i, D_8015DE3C, j);
    }
    D_8015DE4C = D_8015DE4C + 1;
}
#else
INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014A1B0", func_8014A8FC);
#endif

void func_8014AD70(void) {
    draw2d3d(1, 0);
    func_8014B194();
}

void func_8014AD9C(s32 arg0, s16 *arg1, s32 arg2) {
    MATRIX matrix;
    SVECTOR *rotation;

    arg1 += arg2 * 6;
    matrix = *(MATRIX *)D_80125C60;
    matrix.t[0] = ((MATRIX *)D_801227A0[arg0].unk4)->t[0] + arg1[0];
    matrix.t[1] = ((MATRIX *)D_801227A0[arg0].unk4)->t[1] + arg1[1];
    matrix.t[2] = ((MATRIX *)D_801227A0[arg0].unk4)->t[2] + arg1[2];
    rotation = &((SVECTOR *)D_8015DDD0)[arg0];
    rotation->vx += arg1[3];
    arg1 += 4;
    func_8015A640((rotation->vx << 12) / 360, &matrix);
    rotation->vz += *arg1;
    arg1++;
    func_8015A7E0((rotation->vz << 12) / 360, &matrix);
    rotation->vy += *arg1;
    func_8015A980((rotation->vy << 12) / 360, &matrix);
    *(MATRIX *)D_801227A0[arg0].unk4 = matrix;
    D_801227A0[arg0].unk0 = 0;
}

/* The display-list argument is the pointer variable dop (base plus steps), as in TACO func_801347F4;
 * with immediates the hoisted constants take other saved registers. T-17208 */
void func_8014AF88(void) {
    u32 *dop;
    s32 i;

    dop = (u32 *)0x801F8000;
    dop++;
    func_8009B3C0((s32)dop);
    dop += 2;
    for (i = 0; i < 2; i++) {
        func_8009B430((s32)dop, (u8 *)&D_80122640[i * 4], i);
        D_80122640[i * 4 + 1] = (u32)&D_801227A0[i];
        D_80122640[i * 4] = 0x80000000;
    }
    dop = (u32 *)0x801F9000;
    dop++;
    func_8009B3C0((s32)dop);
    dop += 2;
    for (i = 0; i < 9; i++) {
        func_8009B430((s32)dop, D_80122660 + i * 0x10, 0);
        D_80122640[i * 4 + 9] = (u32)&D_80122840[i];
        D_80122640[i * 4 + 8] = 0x80000000;
    }
}

void func_8014B080(void) {
    D_80122760 = 0x64;
    D_80122764 = 0x1E;
    D_80122768 = -0x32;
    D_8012276C = 0;
    D_8012276D = 0;
    D_8012276E = 0;
    func_8009AD70(0, &D_80122760);
    D_80122770 = 0x14;
    D_80122774 = 0xA;
    D_80122778 = -0x64;
    D_8012277C = 0;
    D_8012277D = 0;
    D_8012277E = 0;
    func_8009AD70(1, &D_80122770);
    D_80122780 = -0x14;
    D_80122784 = -0x14;
    D_80122788 = -0x64;
    D_8012278C = 0;
    D_8012278D = 0;
    D_8012278E = 0;
    func_8009AD70(2, &D_80122780);
    func_8009B310(0, 0, 0);
    func_8009B340(0);
}

void func_8014B194(void) {
    D_8015DE40 += 1;
    D_8015DE44 = 0;
    D_8015DE4C = 0;
    D_8015DE48 = 0;
}

void func_8014B1C4(void) {
    D_8015DE44 += 1;
    D_8015DE4C = 0;
    D_8015DE48 = 0;
}
