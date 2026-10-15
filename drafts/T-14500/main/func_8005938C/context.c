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

/* NON_MATCHING: T-11610, 22 differing lines. Reading w and h through their
 * addresses (FAKE) stops uopt from propagating the constants, so 0x140/0xF0 are
 * loaded into a0/a1 before the branch as in the original; left: w and h are
 * spilled to the stack before each call (frame 0x38, not 0x30) and the then-arm
 * loses its second `li a1,0xF0`. Plain constants give 40 lines. */
#ifdef NON_MATCHING
void func_800590CC(s32 arg0) {
    s32 w;
    s32 h;
    s32 *pw;
    s32 *ph;

    func_8009C674(0);
    func_8009C210(1);
    func_800AD950(0);
    func_8009C5E0(0);
    w = 0x140;
    h = 0xF0;
    pw = &w;
    ph = &h;
    if (arg0 != 0) {
        D_800E6280.unk_014[0] = 0x80;
        D_800E6280.unk_014[1] = 0x80;
        w = 0x100;
        D_800E6280.unk_000 = *pw;
        h = 0xF0;
        D_800E6280.unk_002 = *ph;
        D_800E6280.unk_004 = 0;
        func_80097F58(*pw, *ph, 0, 0, 0);
        func_8005907C();
        back_clear_switch(1);
    } else {
        D_800E6280.unk_014[0] = 0xA0;
        D_800E6280.unk_014[1] = 0xA0;
        D_800E6280.unk_000 = *pw;
        D_800E6280.unk_002 = *ph;
        D_800E6280.unk_004 = 0;
        func_80097F58(*pw, *ph, 0, 0, 0);
        func_8005907C();
        back_clear_switch(1);
    }
    func_800AD950(0);
    func_8009C5E0(1);
}
#else
INCLUDE_ASM("asm/nonmatchings/main/80058D20", func_800590CC);
#endif

INCLUDE_ASM("asm/nonmatchings/main/80058D20", func_800591D8);

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

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E731C;
/* end bulk_m2c declarations */
void func_8005938C(void) {
    s16 spF8;
    s16 spF6;
    s16 spF4;
    s32 spEC;
    s32 spE8;
    s32 spE4;
    s32 spE0;
    s32 spDC;
    s32 spD8;
    s32 spC4[8];
    s32 spA4[8];
    s32 sp84[1];
    s32 sp64[1];
    s16 temp_v0;
    s16 temp_v0_2;
    s16 temp_v0_3;
    u8 *temp_s4;
    u8 *var_s0;

    if (D_800E731C < 0xA0) {
        var_s0 = (D_800E731C * 0x44) + D_8011ECD0;
        do {
            if (((*(s32 *)var_s0) != 0) && (*(u8 *)((unsigned char *)var_s0 + (1)) & 0x80)) {
                spF8 = *(s16 *)((unsigned char *)var_s0 + (104));
                spF4 = *(s16 *)((unsigned char *)var_s0 + (100));
                spF6 = *(s16 *)((unsigned char *)var_s0 + (102));
                (*(s32 *)&spC4) = (s32) (*(s32 *)D_80125C60);
                *(s32 *)((unsigned char *)(&spC4) + (8)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (8));
                *(s32 *)((unsigned char *)(&spC4) + (16)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (16));
                *(s32 *)((unsigned char *)(&spC4) + (4)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (4));
                *(s32 *)((unsigned char *)(&spC4) + (24)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (24));
                *(s32 *)((unsigned char *)(&spA4) + (4)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (4));
                (*(s32 *)&spA4) = (s32) (*(s32 *)D_80125C60);
                *(s32 *)((unsigned char *)(&spC4) + (12)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (12));
                *(s32 *)((unsigned char *)(&spA4) + (8)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (8));
                *(s32 *)((unsigned char *)(&spA4) + (12)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (12));
                *(s32 *)((unsigned char *)(&spC4) + (20)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (20));
                *(s32 *)((unsigned char *)(&spA4) + (20)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (20));
                *(s32 *)((unsigned char *)(&spA4) + (16)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (16));
                temp_s4 = var_s0 + 0x18;
                *(s32 *)((unsigned char *)(&spC4) + (28)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (28));
                *(s32 *)((unsigned char *)(&spA4) + (24)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (24));
                *(s32 *)((unsigned char *)(&spA4) + (28)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (28));
                RotMatrix((s32) &spF4, (s32) &spC4);
                temp_v0 = *(s16 *)((unsigned char *)var_s0 + (108));
                if (temp_v0 != 0) {
                    spE4 = (s32) temp_v0;
                } else {
                    spE4 = 0x1000;
                }
                temp_v0_2 = *(s16 *)((unsigned char *)var_s0 + (110));
                if (temp_v0_2 != 0) {
                    spE8 = (s32) temp_v0_2;
                } else {
                    spE8 = 0x1000;
                }
                temp_v0_3 = *(s16 *)((unsigned char *)var_s0 + (112));
                if (temp_v0_3 != 0) {
                    spEC = (s32) temp_v0_3;
                } else {
                    spEC = 0x1000;
                }
                ScaleMatrix((s32) &spA4, (s32) &spE4);
                MulMatrix((s32) &spC4, (s32) &spA4);
                if (*(u8 *)((unsigned char *)var_s0 + (1)) & 0x10) {
                    *(s16 *)((unsigned char *)var_s0 + (116)) = (s16) (*(s16 *)((unsigned char *)var_s0 + (116)) + *(s16 *)((unsigned char *)var_s0 + (124)));
                    *(s16 *)((unsigned char *)var_s0 + (118)) = (s16) (*(s16 *)((unsigned char *)var_s0 + (118)) + *(s16 *)((unsigned char *)var_s0 + (126)));
                    *(s16 *)((unsigned char *)var_s0 + (120)) = (s16) (*(s16 *)((unsigned char *)var_s0 + (120)) + *(s16 *)((unsigned char *)var_s0 + (128)));
                }
                spD8 = (s32) *(s16 *)((unsigned char *)var_s0 + (116));
                spDC = (s32) *(s16 *)((unsigned char *)var_s0 + (118));
                spE0 = (s32) *(s16 *)((unsigned char *)var_s0 + (120));
                *(s32 *)((unsigned char *)var_s0 + (24)) = (s32) (*(s32 *)&spC4);
                *(s32 *)((unsigned char *)temp_s4 + (4)) = (s32) *(s32 *)((unsigned char *)(&spC4) + (4));
                *(s32 *)((unsigned char *)temp_s4 + (8)) = (s32) *(s32 *)((unsigned char *)(&spC4) + (8));
                *(s32 *)((unsigned char *)temp_s4 + (12)) = (s32) *(s32 *)((unsigned char *)(&spC4) + (12));
                *(s32 *)((unsigned char *)temp_s4 + (16)) = (s32) *(s32 *)((unsigned char *)(&spC4) + (16));
                *(s32 *)((unsigned char *)temp_s4 + (20)) = (s32) *(s32 *)((unsigned char *)(&spC4) + (20));
                *(s32 *)((unsigned char *)temp_s4 + (24)) = (s32) *(s32 *)((unsigned char *)(&spC4) + (24));
                *(s32 *)((unsigned char *)temp_s4 + (28)) = (s32) *(s32 *)((unsigned char *)(&spC4) + (28));
                *(s32 *)((unsigned char *)var_s0 + (20)) = 0;
                func_80099B10(*(s32 *)((unsigned char *)var_s0 + (8)), (s32) &sp64, (s32) &sp84);
                func_80099F04((s32) &sp64);
                func_8009A200(*(s32 *)((unsigned char *)var_s0 + (8)), (s32) &sp84);
                func_80099ED8((s32) &sp84);
                func_8009A4D0((s32) (var_s0 + 4), (s32) ((D_8011ECA0 * 0x14) + &D_800E8C70), 6, 0x1F800000);
            }
            var_s0 += 0x88;
        } while ((u32) var_s0 < (u32) D_80121750);
    }
}

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
