#include "common.h"
#include "ovl/RPG_BAT.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
s32 D_8015ED90 = 1;
s32 D_8015ED94 = 0;
s32 D_8015ED98 = 1;
s32 D_8015ED9C = 2;
s32 D_8015EDA0 = 3;
s32 D_8015EDA4 = 4;
s32 D_8015EDA8[2] = { 5, 6 };
RpgBatWords3 D_8015EDB0 = { 7, 8, 9 };
s32 D_8015EDBC = 0;
s32 D_8015EDC0 = 0;
s32 D_8015EDC4 = 1;
s32 D_8015EDC8 = 2;
s32 D_8015EDCC = 3;
s32 D_8015EDD0 = 4;
s32 D_8015EDD4 = 5;
s32 D_8015EDD8 = 6;
s32 D_8015EDDC = 7;
s32 D_8015EDE0 = 8;
s32 D_8015EDE4 = 9;
s32 D_8015EDE8 = 0;
s32 D_8015EDEC = 0;
s32 D_8015EDF0 = 1;
s32 D_8015EDF4 = 2;
s32 D_8015EDF8 = 3;
s32 D_8015EDFC = 4;
RpgBatWords4 D_8015EE00 = { 5, 6, 7, 8 };
s32 D_8015EE10 = 9;
s32 D_8015EE14 = 0;
s32 D_8015EE18 = 0;
s32 D_8015EE1C = 1;
s32 D_8015EE20 = 2;
s32 D_8015EE24 = 3;
s32 D_8015EE28 = 4;
s32 D_8015EE2C = 5;
RpgBatWords4 D_8015EE30 = { 6, 7, 8, 9 };
s32 D_8015EE40 = 0;
RpgBatWords3 D_8015EE44 = { 0, 1, 2 };
RpgBatWords3 D_8015EE50 = { 0, 1, 2 };
s32 D_8015EE5C = 0;
s32 D_8015EE60 = 1;
s32 D_8015EE64 = 2;
s32 D_8015EE68 = 0;
s32 D_8015EE6C = 1;
s32 D_8015EE70 = 2;
s32 D_8015EE74 = 0;
s32 D_8015EE78 = 1;
s32 D_8015EE7C = 2;
s32 D_8015EE80 = 0;
s32 D_8015EE84 = 1;
s32 D_8015EE88 = 2;
s32 D_8015EE8C = 0;

void func_8014E780(void) {
    func_800AE120(D_800E6280.unk_10F4);
    func_80048EB8(0);
    func_8006BC28(0);
    func_8004E58C();
    tpage_buf_clear();
    D_800E6280.unk_036 = 0;
    (*(s8 *)&D_800E6280.unk_038) = 0;
    (*(s8 *)&D_800E6280.unk_037) = 0;
    back_clear_switch(1);
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    hizuke_disp_switch(0);
    func_80065F34(0);
    message_disp_switch(0);
    func_80044750(0x7F);
    func_8014E994();
    D_8015EC18 = (s32) D_800E6280.unk_F5F;
    D_8015EB9C = (s32) D_800E6280.unk_F5F;
    switch (D_80122CE0) {
    case 1:
        D_8015EB98 = 0x10000;
        break;
    case 2:
        D_8015EB98 = 0x20000;
        break;
    case 3:
        D_8015EB98 = 0x40000;
        break;
    case 4:
        D_8015EB98 = 0x100000;
        break;
    case 5:
        D_8015EC1C = 1;
        D_8015EB98 = 0x100000;
        break;
    case 6:
        D_8015EB98 = 0x08000000;
        break;
    case 7:
    default:
        D_8015EB98 = 0x80000000;
        break;
    }
    func_80042808();
}

void func_8014E8F4(void) {
    func_80048390();
    func_800573AC();
    func_8004E58C();
    func_80048EB8(0);
    func_8006BC28(0);
    func_8006BD6C(0);
    func_800438F0(1);
    D_80122CE0 = D_8015EC14;
    D_800E6280.unk_F5F = (s8) D_8015EC18;
    main_state_set((s32) D_800E6280.unk_720);
    func_80042908((s32) D_800E6280.unk_721);
    func_80042940((s32) D_800E6280.unk_722);
}

/* FAKE: first-symbol views bridge unresolved aggregate declarations; the
 * offset bound prevents address CSE and the same-line loop preserves scheduling.
 * T-10850. */
s32 func_8014E994(void) {
    s32 *end;
    s32 i;
    s32 *var_v1;
    s32 *var_v1_2;

    end = (s32 *)((u8 *)D_8015ED64 - 0x28);
    var_v1 = &D_8015EBAC;
    do {
        var_v1++;
        var_v1[-1] = 0;
    } while ((u32) var_v1 < (u32) end);
    end = (s32 *)D_8015ED64;
    var_v1_2 = D_8015ED3C;
    do {
        var_v1_2++;
        var_v1_2[-1] = 0;
    } while ((u32) var_v1_2 < (u32) end);
    for (i = 0; i < 12; i++) { (&D_8015ED64[0][0])[i] = 0;
    }
    D_8015EE44.unk_00 = 0;
    D_8015EE44.unk_04 = 0;
    D_8015EE44.unk_08 = 0;
    D_8015EE50.unk_00 = 0;
    D_8015EE50.unk_04 = 0;
    D_8015EE50.unk_08 = 0;
    D_8015EE5C = 0;
    D_8015EE60 = 0;
    *(s32 *)((u8 *)(&D_8015EE60) + (4)) = 0;
    D_8015EE68 = 0;
    *(s32 *)((u8 *)(&D_8015EE68) + (4)) = 0;
    D_8015EE70 = 0;
    D_8015EE74 = 0;
    *(s32 *)((u8 *)(&D_8015EE74) + (4)) = 0;
    *(s32 *)((u8 *)(&D_8015EE74) + (8)) = 0;
    D_8015EE80 = 0;
    *(s32 *)((u8 *)(&D_8015EE80) + (4)) = 0;
    *(s32 *)((u8 *)(&D_8015EE80) + (8)) = 0;
    D_8015ED94 = 0;
    *(s32 *)((u8 *)(&D_8015ED94) + (4)) = 0;
    *(s32 *)((u8 *)(&D_8015ED94) + (8)) = 0;
    D_8015EDA0 = 0;
    *(s32 *)((u8 *)(&D_8015EDA0) + (4)) = 0;
    D_8015EDEC = 0;
    D_8015EDF0 = 0;
    *(s32 *)((u8 *)(&D_8015EDF0) + (4)) = 0;
    *(s32 *)((u8 *)(&D_8015EDF0) + (8)) = 0;
    *(s32 *)((u8 *)(&D_8015EDF0) + (12)) = 0;
    D_8015EDC0 = 0;
    *(s32 *)((u8 *)(&D_8015EDC0) + (4)) = 0;
    *(s32 *)((u8 *)(&D_8015EDC0) + (8)) = 0;
    *(s32 *)((u8 *)(&D_8015EDC0) + (12)) = 0;
    D_8015EDD0 = 0;
    D_8015EE18 = 0;
    *(s32 *)((u8 *)(&D_8015EE18) + (4)) = 0;
    D_8015EE20 = 0;
    *(s32 *)((u8 *)(&D_8015EE20) + (4)) = 0;
    *(s32 *)((u8 *)(&D_8015EE20) + (8)) = 0;
    D_8015EDA8[0] = 0;
    D_8015EDA8[1] = 0;
    D_8015EDB0.unk_04 = 0;
    D_8015EDB0.unk_08 = 0;
    D_8015EE00.unk_00 = 0;
    D_8015EE00.unk_04 = 0;
    D_8015EE00.unk_0C = 0;
    D_8015EE10 = 0;
    D_8015EDD4 = 0;
    *(s32 *)((u8 *)(&D_8015EDD4) + (4)) = 0;
    D_8015EDE0 = 0;
    *(s32 *)((u8 *)(&D_8015EDE0) + (4)) = 0;
    D_8015EE2C = 0;
    D_8015EE30.unk_00 = 0;
    D_8015EE30.unk_08 = 0;
    D_8015EE30.unk_0C = 0;
    D_8015EDB0.unk_00 = 0;
    D_8015EE00.unk_08 = 0;
    D_8015EDDC = 0;
    D_8015EE30.unk_04 = 0;
}

void func_8014EB58(s32 arg0) {
    switch (arg0) {
    case 0:
        D_8015EE44.unk_00++;
        D_8015EE44.unk_08 = 0;
        break;
    case 1:
        D_8015EE50.unk_00++;
        D_8015EE50.unk_08 = 0;
        break;
    }
}

void func_8014EBA8(void) {
    D_8015EDA8[1]++;
    D_8015EDB0.unk_04 = 0;
    D_8015EDB0.unk_00 = 0;
    D_8015EDB0.unk_08 = 0;
}

void func_8014EBD0(void) {
    D_8015EDB0.unk_00++;
    D_8015EDB0.unk_08 = 0;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014E780", func_8014EBEC);

void func_8014ED44(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_8015EDB0.unk_08 + 1;
    D_8015EDB0.unk_08 = temp_v0;
    if (temp_v0 >= arg0) {
        func_8014EBA8();
    }
}

void func_8014ED80(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_8015EDB0.unk_08 + 1;
    D_8015EDB0.unk_08 = temp_v0;
    if (temp_v0 >= arg0) {
        func_8014EBD0();
    }
}

void func_8014EDBC(s32 arg0) {
    if (!(*(&D_80120652 + (arg0 * 0x44)) & 1)) {
        func_8014EBA8();
    }
}

void func_8014EDFC(s32 arg0) {
    if (!(*(&D_80120652 + (arg0 * 0x44)) & 1)) {
        func_8014EBD0();
    }
}

void func_8014EE3C(s32 arg0) {
    if (D_8015EE44.unk_04 == 0) {
        func_8014EBA8();
    }
    if (arg0 != 0) {
        if ((D_8015EE44.unk_00 - 3) >= arg0) {
            func_8014EBA8();
        }
    }
}

void func_8014EE98(s32 arg0) {
    if (D_8015EE44.unk_04 == 0) {
        func_8014EBD0();
    }
    if (arg0 != 0) {
        if ((D_8015EE44.unk_00 - 3) >= arg0) {
            func_8014EBD0();
        }
    }
}

void func_8014EEF4(void) {
    s32 i;

    for (i = 0x28; i < 0x30; i++) {
        func_8013E97C(i, 0, 0);
    }
}

void func_8014EF3C(void) {
    D_8015EDD8++;
    D_8015EDE0 = 0;
    D_8015EDDC = 0;
    D_8015EDE4 = 0;
}

/* no prototype: func_8014F1D4 calls it with an argument */
void func_8014EF6C() {
    D_8015EDDC++;
    D_8015EDE4 = 0;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014E780", func_8014EF8C);

void func_8014F080(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_8015EDE4 + 1;
    D_8015EDE4 = temp_v0;
    if (temp_v0 >= arg0) {
        func_8014EF3C();
    }
}

void func_8014F0BC(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_8015EDE4 + 1;
    D_8015EDE4 = temp_v0;
    if (temp_v0 >= arg0) {
        func_8014EF6C();
    }
}

void func_8014F0F8(s32 arg0) {
    if (!(*(&D_80120652 + (arg0 * 0x44)) & 1)) {
        func_8014EF3C();
    }
}

void func_8014F138(s32 arg0) {
    if (!(*(&D_80120652 + (arg0 * 0x44)) & 1)) {
        func_8014EF6C();
    }
}

void func_8014F178(s32 arg0) {
    if (D_8015EE44.unk_04 == 0) {
        func_8014EF3C();
    }
    if (arg0 != 0) {
        if ((D_8015EE44.unk_00 - 3) >= arg0) {
            func_8014EF3C();
        }
    }
}

void func_8014F1D4(s32 arg0) {
    if (D_8015EE44.unk_04 == 0) {
        func_8014EF6C();
    }
    if (arg0 != 0) {
        if ((D_8015EE44.unk_00 - 3) >= arg0) {
            func_8014EF6C();
        }
    }
}

void func_8014F230(void) {
    D_8015EE00.unk_04++;
    D_8015EE00.unk_0C = 0;
    D_8015EE00.unk_08 = 0;
    D_8015EE10 = 0;
}

void func_8014F258(void) {
    D_8015EE00.unk_08++;
    D_8015EE10 = 0;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014E780", func_8014F278);

void func_8014F350(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_8015EE10 + 1;
    D_8015EE10 = temp_v0;
    if (temp_v0 >= arg0) {
        func_8014F230();
    }
}

void func_8014F38C(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_8015EE10 + 1;
    D_8015EE10 = temp_v0;
    if (temp_v0 >= arg0) {
        func_8014F258();
    }
}

void func_8014F3C8(s32 arg0) {
    if (!(*(&D_80120652 + (arg0 * 0x44)) & 1)) {
        func_8014F230();
    }
}

void func_8014F408(s32 arg0) {
    if (!(*(&D_80120652 + (arg0 * 0x44)) & 1)) {
        func_8014F258();
    }
}

void func_8014F448(s32 arg0) {
    if (D_8015EE44.unk_04 == 0) {
        func_8014F230();
    }
    if (arg0 != 0) {
        if ((D_8015EE44.unk_00 - 3) >= arg0) {
            func_8014F230();
        }
    }
}

void func_8014F4A4(s32 arg0) {
    if (D_8015EE44.unk_04 == 0) {
        func_8014F258();
    }
    if (arg0 != 0) {
        if ((D_8015EE44.unk_00 - 3) >= arg0) {
            func_8014F258();
        }
    }
}

void func_8014F500(void) {
    D_8015EE30.unk_00++;
    D_8015EE30.unk_08 = 0;
    D_8015EE30.unk_04 = 0;
    D_8015EE30.unk_0C = 0;
}

void func_8014F524(void) {
    D_8015EE30.unk_04++;
    D_8015EE30.unk_0C = 0;
}

/* FAKE: EC74 retains its legacy scalar declaration; the observed 50-word
 * range uses an aggregate view. The last loop body shares its for line to
 * preserve store scheduling. T-9560; original source layout unknown. */
void func_8014F540(void) {
    s32 index;

    D_8015EE28 = 0;
    D_8015EE1C = 0;
    D_8015EE20 = 0;
    D_8015EE30.unk_00 = 0;
    D_8015EE30.unk_08 = 0;
    D_8015EE30.unk_04 = 0;
    D_8015EE30.unk_0C = 0;
    for (index = 0; index < 50; index++) {
        ((RpgClearWords50 *)&D_8015EC74)->values[index] = 0;
    }
    for (index = 0; index < 10; index++) {
        D_8015ED3C[index] = 0;
    }
    for (index = 0; index < 12; index++) ((s32 *)D_8015ED64)[index] = 0;
}

void func_8014F5DC(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_8015EE30.unk_0C + 1;
    D_8015EE30.unk_0C = temp_v0;
    if (temp_v0 >= arg0) {
        func_8014F500();
    }
}

void func_8014F618(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_8015EE30.unk_0C + 1;
    D_8015EE30.unk_0C = temp_v0;
    if (temp_v0 >= arg0) {
        func_8014F524();
    }
}

void func_8014F654(s32 arg0) {
    if (!(*(&D_80120652 + (arg0 * 0x44)) & 1)) {
        func_8014F500();
    }
}

void func_8014F694(s32 arg0) {
    if (!(*(&D_80120652 + (arg0 * 0x44)) & 1)) {
        func_8014F524();
    }
}

void func_8014F6D4(s32 arg0) {
    if (D_8015EE44.unk_04 == 0) {
        func_8014F500();
    }
    if (arg0 != 0) {
        if ((D_8015EE44.unk_00 - 3) >= arg0) {
            func_8014F500();
        }
    }
}

void func_8014F730(s32 arg0) {
    if (D_8015EE48 == 0) {
        func_8014F524();
    }
    if (arg0 != 0) {
        if ((D_8015EE44.unk_00 - 3) >= arg0) {
            func_8014F524();
        }
    }
}
