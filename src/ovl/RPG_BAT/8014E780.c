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

/* FAKE: the three clear loops share one counter (it keeps the first two rolled
 * and lets the last unroll by four, as in func_8014F540) and walk the legacy
 * scalar D_8015EBAC and the 2-D D_8015ED64 as flat word arrays; the third loop
 * body shares its for line to keep the store scheduling. The `(&X)[k]` stores
 * reach the later words of the shared-$at groups through their first symbol
 * (the aggregates are not recovered). T-17108. */
s32 func_8014E994(void) {
    s32 i;

    for (i = 0; i < 100; i++) {
        (&D_8015EBAC)[i] = 0;
    }
    for (i = 0; i < 10; i++) {
        D_8015ED3C[i] = 0;
    }
    for (i = 0; i < 12; i++) (&D_8015ED64[0][0])[i] = 0;
    D_8015EE44.unk_00 = 0;
    D_8015EE44.unk_04 = 0;
    D_8015EE44.unk_08 = 0;
    D_8015EE50.unk_00 = 0;
    D_8015EE50.unk_04 = 0;
    D_8015EE50.unk_08 = 0;
    D_8015EE5C = 0;
    D_8015EE60 = 0;
    (&D_8015EE60)[1] = 0;
    D_8015EE68 = 0;
    (&D_8015EE68)[1] = 0;
    D_8015EE70 = 0;
    D_8015EE74 = 0;
    (&D_8015EE74)[1] = 0;
    (&D_8015EE74)[2] = 0;
    D_8015EE80 = 0;
    (&D_8015EE80)[1] = 0;
    (&D_8015EE80)[2] = 0;
    D_8015ED94 = 0;
    (&D_8015ED94)[1] = 0;
    (&D_8015ED94)[2] = 0;
    D_8015EDA0 = 0;
    (&D_8015EDA0)[1] = 0;
    D_8015EDEC = 0;
    D_8015EDF0 = 0;
    (&D_8015EDF0)[1] = 0;
    (&D_8015EDF0)[2] = 0;
    (&D_8015EDF0)[3] = 0;
    D_8015EDC0 = 0;
    (&D_8015EDC0)[1] = 0;
    (&D_8015EDC0)[2] = 0;
    (&D_8015EDC0)[3] = 0;
    D_8015EDD0 = 0;
    D_8015EE18 = 0;
    (&D_8015EE18)[1] = 0;
    D_8015EE20 = 0;
    (&D_8015EE20)[1] = 0;
    (&D_8015EE20)[2] = 0;
    D_8015EDA8[0] = 0;
    D_8015EDA8[1] = 0;
    D_8015EDB0.unk_04 = 0;
    D_8015EDB0.unk_08 = 0;
    D_8015EE00.unk_00 = 0;
    D_8015EE00.unk_04 = 0;
    D_8015EE00.unk_0C = 0;
    D_8015EE10 = 0;
    D_8015EDD4 = 0;
    (&D_8015EDD4)[1] = 0;
    D_8015EDE0 = 0;
    (&D_8015EDE0)[1] = 0;
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

/* FAKE: the three clear loops and the record loop reuse one counter, the clear
 * loops walk the legacy scalar D_8015EC74 and the 2-D D_8015ED64 as flat word
 * arrays (func_8014E994 has the same loops), the record loop shares its for line
 * to keep the store order, and D_8015ED9C is reached through D_8015ED98 to share
 * its $at (aggregate not recovered). T-17108. */
void func_8014EBEC(void) {
    s32 i;

    D_8015EDA4 = 0;
    D_8015ED98 = 0;
    (&D_8015ED98)[1] = 0;
    D_8015EDA8[1] = 0;
    D_8015EDB0.unk_04 = 0;
    D_8015EDB0.unk_00 = 0;
    D_8015EDB0.unk_08 = 0;
    for (i = 0; i < 50; i++) {
        (&D_8015EC74)[i] = 0;
    }
    for (i = 0; i < 10; i++) {
        D_8015ED3C[i] = 0;
    }
    for (i = 0; i < 12; i++) {
        (&D_8015ED64[0][0])[i] = 0;
    }
    if (D_8015EB98 != 0x80000000) {
        for (i = 0x28; i < 0x30; i++) {
            func_8013E7C0(i, 0xFF, 1, 0);
        }
    }
    i = 0x32;
    do {
        func_8013E7C0(i, 0xFF, 1, 0);
        i++;
    } while (i < 0x38);
    for (i = 40; i < 64; i++) ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_01 = 5;
    if (D_8015EB98 != 0x80000000) {
        ((DisplayActor44 *)D_8011ECD0)[125].unk_01 = 5;
    }
}

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

/* FAKE: same loops as func_8014EBEC (shared counter, flat word views of the
 * legacy scalar D_8015EC74 and the 2-D D_8015ED64, record loop on one line), and
 * D_8015EDD0, D_8015EDC8 are reached through D_8015EDC4 (an unrecovered
 * four-word table; the view also keeps the first store ahead of the load in the
 * schedule). T-17108. */
void func_8014EF8C(void) {
    s32 i;

    (&D_8015EDC4)[3] = 0;
    D_8015EDC4 &= 0xFFFF0000;
    (&D_8015EDC4)[1] = 0;
    D_8015EDD8 = 0;
    D_8015EDE0 = 0;
    D_8015EDDC = 0;
    D_8015EDE4 = 0;
    for (i = 0; i < 50; i++) {
        (&D_8015EC74)[i] = 0;
    }
    for (i = 0; i < 10; i++) {
        D_8015ED3C[i] = 0;
    }
    for (i = 0; i < 12; i++) {
        (&D_8015ED64[0][0])[i] = 0;
    }
    for (i = 40; i < 64; i++) ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_01 = 5;
    if (D_8015EB98 != 0x80000000) {
        ((DisplayActor44 *)D_8011ECD0)[125].unk_01 = 5;
    }
}

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

/* FAKE: same loops as func_8014EBEC (shared counter, flat word views of the
 * legacy scalar D_8015EC74 and the 2-D D_8015ED64, record loop on one line), and
 * D_8015EDFC, D_8015EDF4 are reached through D_8015EDF0 to share its $at (the
 * four-word table is not recovered). T-17108. */
void func_8014F278(void) {
    s32 i;

    (&D_8015EDF0)[3] = 0;
    D_8015EDF0 = 0;
    (&D_8015EDF0)[1] = 0;
    D_8015EE00.unk_04 = 0;
    D_8015EE00.unk_0C = 0;
    D_8015EE00.unk_08 = 0;
    D_8015EE10 = 0;
    for (i = 0; i < 50; i++) {
        (&D_8015EC74)[i] = 0;
    }
    for (i = 0; i < 10; i++) {
        D_8015ED3C[i] = 0;
    }
    for (i = 0; i < 12; i++) {
        (&D_8015ED64[0][0])[i] = 0;
    }
    for (i = 40; i < 64; i++) ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_01 = 5;
    if (D_8015EB98 != 0x80000000) {
        ((DisplayActor44 *)D_8011ECD0)[125].unk_01 = 5;
    }
}

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
