#include "common.h"
#include "ovl/DATE.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
s16 D_8015DDD0[0x2C] = { 0 };
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/8014A1B0.data", D_8015DE28);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/8014A1B0.data", D_8015DE2C);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/8014A1B0.data", D_8015DE30);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/8014A1B0.data", D_8015DE34);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/8014A1B0.data", D_8015DE38);
INCLUDE_RODATA("asm/ovl/DATE/data/DATE/8014A1B0.data", D_8015DE3C);

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

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014A1B0", func_8014A4DC);

void func_8014A810(void) {
    s32 i;
    s32 j;

    func_8014AD9C(0, D_8015DE34, 0);
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

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014A1B0", func_8014A8FC);

void func_8014AD70(void) {
    draw2d3d(1, 0);
    func_8014B194();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014A1B0", func_8014AD9C);

#ifdef NON_MATCHING
/* NON_MATCHING: T-10320, only the saved registers of the two hoisted constants and the end pointer differ (22 lines); needs the 3-argument func_8009B430 view */
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

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014A1B0", func_8014B1C4);
