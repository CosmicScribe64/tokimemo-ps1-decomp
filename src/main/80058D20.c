/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this file was matched with. */
#define MAIN_API_OVERRIDE_func_8009B430 /* implicit declaration: called with 3 arguments, the original leaves $a3 unset (main_api.h: 4) */

#include "common.h"
#include "game.h"

void initView(void) {
    func_8009AD30(0x3E8);
    D_80122740 = 0;
    D_80122744 = 0;
    D_80122748 = 0x3E8;
    D_8012274C = 0;
    D_80122750 = 0;
    D_80122754 = 0;
    D_80122758 = 0;
    D_8012275C = 0;
    func_80099540(&D_80122740);
    func_8009AD50(0x64);
    func_8009AD60(0x10000);
}

/* FAKE: retain the shared scalar light views until the libgs light aggregate
 * is available in project headers. T-9550 */
void initLight(void) {
    D_80122760 = 0;
    D_80122764 = 0;
    D_80122768 = 0x64;
    D_8012276C = 0xFF;
    D_8012276D = 0xFF;
    D_8012276E = 0xFF;
    func_8009AD70(0, (s32) &D_80122760);
    D_80122770 = 0;
    D_80122774 = 0x64;
    D_80122778 = 0;
    D_8012277C = 0;
    D_8012277D = 0xFF;
    D_8012277E = 0;
    func_8009AD70(1, (s32) &D_80122770);
    D_80122780 = 0x64;
    D_80122784 = 0;
    D_80122788 = 0;
    D_8012278C = 0xFF;
    D_8012278D = 0;
    D_8012278E = 0;
    func_8009AD70(2, (s32) &D_80122780);
    GsSetAmbient(0, 0, 0);
    func_8009B340(0);
}

void initCoordinate(void) {
    TaiikuBig *p;

    /* FAKE: init and loop head on one source line give the original's addiu order (as1 schedules by line, T-7020) */
    p = D_801227A0; do {
        func_80099E30(0, p);
        p->unk20 = -0xFA0;
        p->unk0 = 0;
        p++;
    } while (p != D_80122CA0);
}

/* HYPOTHESIS: the TMD walk of the PsyQ libgs samples (func_8009B3C0 =
 * GsMapModelingData, func_8009B430 = GsLinkObject4): skip the ID word, map the
 * data, skip the flags and object-count words. The separate `dop++` steps give
 * the original's `move s2,s3; addiu s2,s2,8`; the indexed loop gives its
 * address setup order. T-11610 */
void initModelingData_init(s32 *arg0) {
    s32 *dop;
    s32 i;

    dop = arg0;
    dop++;
    func_8009B3C0((s32)dop);
    dop++;
    dop++;
    for (i = 0; i < 16; i++) {
        func_8009B430((s32)dop, (u8 *)&D_80122640[i * 4], 0);
        D_80122640[i * 4 + 1] = (u32)&D_801227A0[i];
        D_80122640[i * 4] = 0x80000000;
    }
}

/* Same TMD walk as initModelingData_init for one object slot (count clamped
 * to 15). FAKE: the slot is reached by byte offset from D_80122640 (u32[], 16
 * bytes per slot) because the record type is not declared yet; the repeated
 * slot address, not a local, gives the original's shared spill slot (as in
 * TAIIKU func_80140738), and `&D_80122640[arg1 * 4]` costs one more ugen
 * temporary. T-11610 */
s32 func_80058F9C(u32 *arg0, s32 arg1, u32 arg2) {
    u32 *dop;
    u32 n;

    dop = arg0;
    dop++;
    func_8009B3C0((s32)dop);
    dop++;
    n = *dop;
    if (n > 16) {
        n = 15;
    }
    dop++;
    if (arg2 < n) {
        func_8009B430((s32)dop, (u8 *)D_80122640 + arg1 * 0x10, arg2);
        *(u32 *)((u8 *)D_80122640 + arg1 * 0x10 + 4) = (u32)&D_801227A0[arg1];
        *(u32 *)((u8 *)D_80122640 + arg1 * 0x10) = 0;
        return 1;
    }
    return -1;
}

void func_80059048(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        D_80122640[i * 4] = 0x80000000;
    }
}

void func_8005907C(void) {
    func_80098380();
    func_8009B560(0, 0, 0, 0xF0);
    func_80098530();
    InitGeom();
    D_8011ECA0 = func_80098370();
}

/* FAKE: the call arguments re-read the three halfwords just stored through a `u16 *` view of
 * D_800E6280; uopt then passes the stored registers (a0/a1) instead of folding the constants,
 * which plain field reads do (18 lines). The else arm stores unk_000/unk_002 before unk_014 so
 * their `li` lead the arm and as1 hoists them above the branch, as in the original. T-24206 */
void func_800590CC(s32 arg0) {
    func_8009C674(0);
    func_8009C210(1);
    func_800AD950(0);
    func_8009C5E0(0);
    if (arg0 != 0) {
        D_800E6280.unk_014[0] = 0x80;
        D_800E6280.unk_014[1] = 0x80;
        D_800E6280.unk_000 = 0x100;
        D_800E6280.unk_002 = 0xF0;
        D_800E6280.unk_004 = 0;
        func_80097F58(((u16 *)&D_800E6280)[0], ((u16 *)&D_800E6280)[1], ((u16 *)&D_800E6280)[2], 0, 0);
        func_8005907C();
        back_clear_switch(1);
    } else {
        D_800E6280.unk_000 = 0x140;
        D_800E6280.unk_002 = 0xF0;
        D_800E6280.unk_014[0] = 0xA0;
        D_800E6280.unk_014[1] = 0xA0;
        D_800E6280.unk_004 = 0;
        func_80097F58(((u16 *)&D_800E6280)[0], ((u16 *)&D_800E6280)[1], ((u16 *)&D_800E6280)[2], 0, 0);
        func_8005907C();
        back_clear_switch(1);
    }
    func_800AD950(0);
    func_8009C5E0(1);
}

/* FAKE: the call re-reads the stored halfwords through a `u16 *` view of D_800E6280 so uopt
 * forwards the stored registers into a0-a2; each arm writes unk_002 (and the else arm unk_000)
 * before the unk_014 stores so as1 hoists their li into the branch delay slots, as in
 * func_800590CC (T-24206). T-24210 */
void func_800591D8(s32 arg0) {
    func_80059308();
    func_8009C674(0);
    func_8009C210(1);
    func_8009C5E0(0);
    if ((arg0 != 0) && (D_800E6280.unk_000 != 0x200)) {
        D_800E6280.unk_002 = 0xF0;
        D_800E6280.unk_014[0] = 0x100;
        D_800E6280.unk_014[1] = 0x100;
        D_800E6280.unk_000 = 0x200;
        D_800E6280.unk_004 = 0;
        func_80097F58(((u16 *)&D_800E6280)[0], ((u16 *)&D_800E6280)[1], ((u16 *)&D_800E6280)[2], 0, 0);
        func_8005907C();
        back_clear_switch(1);
    } else if ((arg0 == 0) && (D_800E6280.unk_000 != 0x140)) {
        D_800E6280.unk_000 = 0x140;
        D_800E6280.unk_002 = 0xF0;
        D_800E6280.unk_014[0] = 0xA0;
        D_800E6280.unk_014[1] = 0xA0;
        D_800E6280.unk_004 = 0;
        func_80097F58(((u16 *)&D_800E6280)[0], ((u16 *)&D_800E6280)[1], ((u16 *)&D_800E6280)[2], 0, 0);
        func_8005907C();
        back_clear_switch(1);
    }
    func_800AD950(0);
    func_8009C5E0(1);
}

/* FAKE: the RECT field stores sharing a source line are scheduled as a unit by as1, which gives the original's store order (h before w). T-8010 */
void func_80059308(void) {
    RECT rect;

    rect.w = 0x100; rect.h = 0x1E0;
    rect.x = 0;
    rect.y = 0;
    func_8009C7F8(&rect, 0, 0, 0);
    func_8009C674(0);
    rect.x = 0x100; rect.w = 0x100; rect.h = 0x1E0;
    rect.y = 0;
    func_8009C7F8(&rect, 0, 0, 0);
    func_8009C674(0);
}

INCLUDE_ASM("asm/nonmatchings/main/80058D20", func_8005938C);

void func_80059688(s32 arg0) {
    u8 *p;

    p = D_8011ECD0 + arg0 * 0x44;
    p[0] = 1;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    *(s16 *)(p + 0x64) = 0;
    *(s16 *)(p + 0x66) = 0;
    *(s16 *)(p + 0x68) = 0;
    *(s16 *)(p + 0x6A) = 0;
    *(s16 *)(p + 0x6C) = 0x1000;
    *(s16 *)(p + 0x6E) = 0x1000;
    *(s16 *)(p + 0x70) = 0x1000;
    *(s16 *)(p + 0x72) = 0;
    *(s16 *)(p + 0x74) = 0;
    *(s16 *)(p + 0x76) = 0;
    *(s16 *)(p + 0x78) = 0;
    *(s16 *)(p + 0x7A) = 0;
    *(s16 *)(p + 0x7C) = 0;
    *(s16 *)(p + 0x7E) = 0;
    *(s16 *)(p + 0x80) = 0;
    *(s16 *)(p + 0x82) = 0;
    p[0x84] = 0;
    p[0x85] = 1;
    p[0x86] = 0;
}
