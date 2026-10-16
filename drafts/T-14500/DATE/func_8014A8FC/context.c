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

/* bulk_m2c declarations (T-11190) */
extern s32 D_80122670;
extern s32 D_801226F0;
extern u8 D_800E62B8;
/* end bulk_m2c declarations */
void func_8014A8FC(void) {
    s32 *var_v0;
    s16 temp_s0;
    s32 *var_s1;
    s32 *var_s1_2;
    s32 *var_s1_3;
    s32 *var_s1_4;
    s32 *var_s1_5;
    s32 *var_s1_6;
    s32 *var_s1_7;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_s0_5;
    s32 var_s0_6;
    s32 var_s0_7;
    s32 var_s0_8;
    s32 var_s1_8;
    u8 temp_t3;
    u8 temp_t4;
    u8 temp_t7;
    u8 temp_t8;
    u8 temp_v0;
    u8 temp_v0_2;

    message_window_show();
    func_80066C08(2);
    func_80066334();
    temp_s0 = D_8015DE4C;
    if (D_8015DE4C < 0x20) {
        if ((u8) D_8012276C < 0xC0U) {
            temp_t7 = D_8012276E + 6;
            D_8012276D = temp_t7;
            D_8012276C = temp_t7;
            temp_t8 = D_8012277E + 1;
            D_8012277D = temp_t8;
            D_8012277C = temp_t8;
            temp_v0 = D_8012278E + 1;
            D_8012278D = temp_v0;
            D_8012278C = temp_v0;
            D_8012276E = temp_t7;
            D_8012277E = temp_t8;
            D_8012278E = temp_v0;
            func_8009AD70(0, (s32) &D_80122760);
            func_8009AD70(1, (s32) &D_80122770);
            func_8009AD70(2, (s32) &D_80122780);
        }
        if (temp_s0 < 0xF) {
            D_800E62B8 += 1;
        }
        goto block_52;
    }
    if ((D_8015DE4C >= 0x20) && (D_8015DE4C < 0x54)) {
        var_s0 = 0;
        var_s1 = D_8015DE34;
        do {
            func_8014AD9C(var_s0, *var_s1, 1);
            var_s0 += 1;
            var_s1 += 4;
        } while (var_s0 != 2);
        goto block_52;
    }
    if ((D_8015DE4C >= 0x54) && (D_8015DE4C < 0x68)) {
        var_s0_2 = 0;
        var_s1_2 = D_8015DE34;
        do {
            func_8014AD9C(var_s0_2, *var_s1_2, 2);
            var_s0_2 += 1;
            var_s1_2 += 4;
        } while (var_s0_2 != 2);
        goto block_52;
    }
    if ((D_8015DE4C >= 0x68) && (D_8015DE4C < 0x72)) {
        var_s0_3 = 0;
        var_s1_3 = D_8015DE34;
        do {
            func_8014AD9C(var_s0_3, *var_s1_3, 3);
            var_s0_3 += 1;
            var_s1_3 += 4;
        } while (var_s0_3 != 2);
        goto block_52;
    }
    if ((D_8015DE4C >= 0x72) && (D_8015DE4C < 0x7C)) {
        var_s0_4 = 0;
        var_s1_4 = D_8015DE34;
        do {
            func_8014AD9C(var_s0_4, *var_s1_4, 4);
            var_s0_4 += 1;
            var_s1_4 += 4;
        } while (var_s0_4 != 2);
        goto block_52;
    }
    if ((D_8015DE4C >= 0x7C) && (D_8015DE4C < 0x9A)) {
        var_s0_5 = 0;
        var_s1_5 = D_8015DE34;
        do {
            func_8014AD9C(var_s0_5, *var_s1_5, 5);
            var_s0_5 += 1;
            var_s1_5 += 4;
        } while (var_s0_5 != 2);
        goto block_52;
    }
    if ((D_8015DE4C >= 0x9A) && (D_8015DE4C < 0xAE)) {
        var_s0_6 = 0;
        var_s1_6 = D_8015DE34;
        do {
            func_8014AD9C(var_s0_6, *var_s1_6, 6);
            var_s0_6 += 1;
            var_s1_6 += 4;
        } while (var_s0_6 != 2);
        goto block_52;
    }
    if ((D_8015DE4C >= 0xAE) && (D_8015DE4C < 0xEA)) {
        var_s0_7 = 0;
        var_s1_7 = D_8015DE34;
        do {
            func_8014AD9C(var_s0_7, *var_s1_7, 7);
            var_s0_7 += 1;
            var_s1_7 += 4;
        } while (var_s0_7 != 2);
        goto block_52;
    }
    if ((D_8015DE4C >= 0x10E) && (D_8015DE4C < 0x12E)) {
        if (D_8012276C != 0) {
            temp_t3 = D_8012276E - 6;
            D_8012276D = temp_t3;
            D_8012276C = temp_t3;
            temp_t4 = D_8012277E - 1;
            D_8012277D = temp_t4;
            D_8012277C = temp_t4;
            temp_v0_2 = D_8012278E - 1;
            D_8012278D = temp_v0_2;
            D_8012278C = temp_v0_2;
            D_8012276E = temp_t3;
            D_8012277E = temp_t4;
            D_8012278E = temp_v0_2;
            func_8009AD70(0, (s32) &D_80122760);
            func_8009AD70(1, (s32) &D_80122770);
            func_8009AD70(2, (s32) &D_80122780);
        }
        if (temp_s0 < 0x11D) {
            D_800E62B8 -= 1;
        }
        if (D_8012276C == 0) {
            var_v0 = &D_80122670;
            *D_80122660 = 0x80000000;
            do {
                var_v0 += 0x40;
                *(s32 *)((unsigned char *)var_v0 + (-48)) = 0x80000000;
                *(s32 *)((unsigned char *)var_v0 + (-32)) = 0x80000000;
                *(s32 *)((unsigned char *)var_v0 + (-16)) = 0x80000000;
                *(s32 *)((unsigned char *)var_v0 + (-64)) = 0x80000000;
            } while (var_v0 != &D_801226F0);
        }
        goto block_52;
    }
    var_s0_8 = 2;
    if (D_8015DE4C >= 0x138) {
        func_8014B1C4();
block_52:
        var_s0_8 = 2;
    }
    var_s1_8 = 9;
    do {
        func_8014AD9C(var_s0_8, D_8015DE3C, var_s1_8);
        var_s0_8 += 1;
        var_s1_8 += 1;
    } while (var_s0_8 != 0xB);
    D_8015DE4C += 1;
}

void func_8014AD70(void) {
    draw2d3d(1, 0);
    func_8014B194();
}

/* NON_MATCHING: T-12100, five saved-pointer stack-offset rows remain.
 * MATRIX and SVECTOR recover every instruction and register. */
#ifdef NON_MATCHING
void func_8014AD9C(s32 arg0, s16 *arg1, s32 arg2) {
    MATRIX matrix;
    TaiikuBig *coord;
    SVECTOR *rotation;

    arg1 += arg2 * 6;
    matrix = *(MATRIX *)D_80125C60;
    coord = &D_801227A0[arg0];
    matrix.t[0] = ((MATRIX *)coord->unk4)->t[0] + arg1[0];
    matrix.t[1] = ((MATRIX *)coord->unk4)->t[1] + arg1[1];
    matrix.t[2] = ((MATRIX *)coord->unk4)->t[2] + arg1[2];
    rotation = &((SVECTOR *)D_8015DDD0)[arg0];
    rotation->vx += arg1[3];
    arg1 += 4;
    func_8015A640((rotation->vx << 12) / 360, &matrix);
    rotation->vz += *arg1;
    arg1++;
    func_8015A7E0((rotation->vz << 12) / 360, &matrix);
    rotation->vy += *arg1;
    func_8015A980((rotation->vy << 12) / 360, &matrix);
    *(MATRIX *)coord->unk4 = matrix;
    coord->unk0 = 0;
}
#else
INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014A1B0", func_8014AD9C);
#endif

#ifdef NON_MATCHING
/* NON_MATCHING: T-10320, seventeen instruction rows differ in saved constants and end-pointer allocation; T-12100 recheck retains the existing 3-argument func_8009B430 view */
void func_8014AF88(void) {
    s32 i;

    func_8009B3C0(0x801F8004);
    for (i = 0; i < 2; i++) {
        func_8009B430(0x801F800C, (u8 *)&D_80122640[i * 4], i);
        D_80122640[i * 4 + 1] = (u32)&D_801227A0[i];
        D_80122640[i * 4] = 0x80000000;
    }
    func_8009B3C0(0x801F9004);
    for (i = 0; i < 9; i++) {
        func_8009B430(0x801F900C, D_80122660 + i * 0x10, 0);
        D_80122640[i * 4 + 9] = (u32)&D_80122840[i];
        D_80122640[i * 4 + 8] = 0x80000000;
    }
}
#else
INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014A1B0", func_8014AF88);
#endif

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
