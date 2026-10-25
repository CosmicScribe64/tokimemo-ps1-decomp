/* annual_girl_selection.c: ; src/ovl/GYOZI/80138B50.c:57-78 filters and ranks girl candidates. Original object 0x80138B50-0x80139920 (config/objects/GYOZI.txt). */
#define MAIN_API_OVERRIDE_func_800BCE10 /* matched with implicit int return view (main_api.h: void) */
#define MAIN_API_OVERRIDE_func_800BCDF0 /* T-25120: integer debug-call result (as in 80140C90.c) keeps the rotation offset out of v0 */
#include "common.h"
#include "ovl/GYOZI.h"

s32 func_800BCE10();
s32 func_800BCDF0();

void annual_candidate_scene_audio_setup(void) {
    func_8008A0D4(0x3FED);
    func_80090960(6, 0);
    func_8004DE1C();
}

void annual_candidate_scene_audio_command_play_40c1(void) {
    func_8008A0D4(0x40C1);
    func_8004DE1C();
}

void annual_candidate_scene_selector_update(void) {
    D_80147198 = (D_80147198 + (D_800F53DE * 3)) - 0x120;
    func_8004DE1C();
}

typedef struct {
    void (*f[56])();
} FnTbl56; /* size 0xE0 */
extern FnTbl56 D_80147248;

void annual_candidate_scene_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl56 tbl;

    tbl = D_80147248;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void annual_candidate_target_selector_reset(void) {
    D_800F62CF = 0;
    func_8004DE1C();
}

/* FAKE (T-25120): the gather, print and rotation stages reach the list, its count and the special
 * byte through the CL view of one record. The values store then aliases the count, so uopt keeps the
 * rotation offset out of the rotation loop and computes it before the loop guard, as the original
 * does. The reorder loop keeps the separate symbols, so the address of the last entry stays shared
 * across its store. */
typedef struct {
    /* 0x00 */ s16 values[12]; /* D_80147224 */
    /* 0x18 */ s16 count;      /* D_8014723C */
    /* 0x1A */ s16 pad;
    /* 0x1C */ u8 special;     /* D_80147240 */
} CandList; /* only the part up to special is used */
#define CL (*(CandList *)D_80147224)

void annual_candidate_list_build_and_rotate(void) {
    s16 r;
    s16 sp4C[13];
    s16 i;
    s16 count;

    D_80147220 = 0;
    D_80147244 = 0;
    CL.count = 0;
    CL.special = 0; /* FAKE (T-25120): an entry store by name would let tools/cvt_pass.py drop the
                     * widening of the `== 1` loads and then put the register constant first */
    count = 0;
    if (((GirlFlagBits *)D_800F53A0.girl[9].unk_10)->met == 0 &&
        D_800F53A0.girl[9].unk_06 >= 20) {
        D_80147240 = 1;
    }
    r = 1; /* FAKE (T-25120): `r * 1` is not a plain variable, so uopt puts the widened load first */
    if (i) {} /* FAKE: empty test splits the live range of i (autotweak) T-24707 */
    for (i = 0; i < 11; i++) {
        if (i) {} /* FAKE: empty test splits the live range of i (autotweak) T-24707 */
        if ((((GirlFlagBits *)D_800F53A0.girl[i].unk_10)->met != 0 || (i == 9 && r * 1 == D_80147240)) &&
            (func_8005F220(i) >= 0x32U || (r * 1 == D_80147240 && i == 9)) &&
            (D_80147240 == 0 || i != 0 || func_8005EBE0(0) >= 0x46U)) {
            sp4C[count] = i;
            count += 1;
            CL.count += 1;
            ((GirlFlagBits *)D_800F53A0.girl[i].unk_10)->b8 = 1;
        }
    }
    for (i = 0; i < CL.count; i++) {
        func_800BCDF0("%d ", sp4C[i]);
    }
    func_800BCDF0("\n");
    r = (u32)D_800F6468 % (u32)CL.count;
    for (i = 0; i < CL.count; i++) {
        CL.values[i] = sp4C[(i + r) % CL.count];
    }
    for (i = 0; i < CL.count; i++) {
        func_800BCDF0("%d ", CL.values[i]);
    }
    func_800BCDF0("\n");
    for (i = 0; i < D_8014723C; i++) {
        if (D_80147224[i] == 0 && func_8005EBE0(0) >= 0x46U) {
            D_80147224[i] = D_80147224[D_8014723C - 1];
            D_80147224[D_8014723C - 1] = 0;
            break;
        }
        if (D_80147224[i] == 0) {
            D_80147224[i] = D_80147224[0];
            D_80147224[0] = 0;
            break;
        }
        if (D_80147224[i] == 9 && D_80147240 != 0) {
            D_80147224[i] = D_80147224[0];
            D_80147224[0] = 9;
            break;
        }
    }
    func_8004DE1C();
}

/* The girl index is reached through the work pointer, as in the VALEN sibling func_80132D0C:
 * the assignment value then stays in $a0 for the store and the first call, and the later reads
 * form one promoted range of unk_F2F that takes $v1 once func_8005EBE0 has its s32 parameter
 * (main_api.h). D_80147240 is volatile in GYOZI.h for the dispatch `sll` slot (T-25120). */
s32 annual_candidate_list_entry_setup(void) {
    GyoziWork *w;
    u8 s;

    w = &D_800F53A0;

    if (D_80147220 == D_8014723C) {
        func_8004DDD8();
        return 0;
    }
    func_8008F618(w->unk_F2F = D_80147224[D_80147220]);
    s = func_8005E0E0(w->unk_F2F) & 0x7F;
    D_80147198 = func_8005EBE0(w->unk_F2F) >= 0x46U;
    if (w->unk_F2F == 9 && D_80147240 == 1) {
        D_80147198 = 3;
    }
    if (w->unk_F2F == 0 && D_80147240 == 1 && s < 2U) {
        D_80147198 = 3;
    }
    switch (w->unk_F2F) {
    case 0:
        D_8014718C = D_801470F8;
        D_80147190 = D_8014712C;
        D_80147194 = D_80147160;
        break;
    case 1:
        D_8014718C = D_8014710C;
        D_80147190 = D_80147140;
        D_80147194 = D_80147174;
        break;
    case 2:
        D_8014718C = D_801470FC;
        D_80147190 = D_80147130;
        D_80147194 = D_80147164;
        break;
    case 3:
        D_8014718C = D_80147104;
        D_80147190 = D_80147138;
        D_80147194 = D_8014716C;
        break;
    case 4:
        D_8014718C = D_8014711C;
        D_80147190 = D_80147150;
        D_80147194 = D_80147184;
        break;
    case 5:
        D_8014718C = D_80147110;
        D_80147190 = D_80147144;
        D_80147194 = D_80147178;
        break;
    case 6:
        D_8014718C = D_80147114;
        D_80147190 = D_80147148;
        D_80147194 = D_8014717C;
        break;
    case 7:
        D_8014718C = D_80147108;
        D_80147190 = D_8014713C;
        D_80147194 = D_80147170;
        break;
    case 8:
        D_8014718C = D_801470F0;
        D_80147190 = D_80147124;
        D_80147194 = D_80147158;
        break;
    case 9:
        D_8014718C = D_80147118;
        D_80147190 = D_8014714C;
        D_80147194 = D_80147180;
        break;
    case 10:
        D_8014718C = D_80147120;
        D_80147190 = D_80147154;
        D_80147194 = D_80147188;
        break;
    }
    func_8004DE1C();
}

void annual_candidate_scene_background_select(void) {
    s32 pad;
    s32 bg;

    /* FAKE: dead stores touch D_800F62CF at entry for cvt_pass.py (T-20610) and promote D_80147244 to $v0; capturing func_800BCE10 into pad reserves $v0 across the switch and post-switch check. T-24130 */
    D_80147244 = D_800F62CF;
    D_80147244 = D_800F6468;
    if ((D_800F6468 & 7) == 0) {
        D_80147244 = 3;
    } else if ((D_800F6468 & 7) < 4U) {
        D_80147244 = 0;
    } else {
        D_80147244 = 1;
    }
    if (D_800F62CF == 0) {
        D_80147244 = D_800F6468 & 1;
    }
    switch (D_80147244) {
    case 0:
        pad = func_800BCE10(&D_800D92E0, "廊下");
        bg = 0x3FED;
        break;
    case 1:
        pad = func_800BCE10(&D_800D92E0, "教室");
        bg = 0x3FFF;
        break;
    case 2:
        pad = func_800BCE10(&D_800D92E0, "校舎裏");
        bg = 0x401A;
        break;
    default:
        pad = func_800BCE10(&D_800D92E0, "屋上");
        bg = 0x4023;
        break;
    }
    if (pad) {}
    if (D_80147220 == D_8014723C - 1) {
        pad = func_800BCE10(&D_800D92E0, "校門前");
        bg = 0x3FDA;
    }
    if (pad) {}
    if (D_80147220 == D_8014723C - 1 && D_800F62CF == 0 &&
        func_8005EBE0(D_800F62CF) >= 0x46U) {
        func_800BCE10(&D_800D92E0, "自宅前");
        bg = 0x40ED;
    }
    if (D_800F62CF == 9 && D_80147240 == 1 && D_80147244 >= 2U) {
        func_800BCE10(&D_800D92E0, "教室");
        bg = 0x3FF5;
    }
    /* FAKE: non-volatile view of D_80147240 (volatile in GYOZI.h for annual_candidate_list_entry_setup) keeps the original local order of its `lui`. T-25120 */
    if (D_800F62CF == 9 && *(u8 *)&D_80147240 == 1 && D_80147244 < 2U) {
        func_800BCE10(&D_800D92E0, "廊下");
        bg = 0x3FED;
    }
    switch (bg) {
    case 0x3FED:
        func_80090960(6, 0);
        break;
    case 0x3FF5:
        func_80090960(6, 1);
        break;
    case 0x40ED:
        func_80090960(6, 5);
        break;
    case 0x3FDA:
        func_80090960(6, 4);
        break;
    case 0x401A:
        func_80090960(6, 3);
        break;
    default:
        func_80090960(6, 2);
        break;
    }
    func_8008A0D4(bg);
    func_8004DE1C();
}

void annual_candidate_list_index_advance(void) {
    D_80147220 += 1;
    func_8004DEE4(1);
}

void annual_candidate_dialogue_copy_and_advance(void) {
    if (func_800BDC20(&D_800D92A0, &D_800D92E0) == 0) {
        func_8004DE1C();
    }
    func_800BCE10(&D_800D92A0, &D_800D92E0);
    func_8004DE1C();
}

void annual_special_candidate_skip_route(void) {
    if ((D_80147240 == 1) && (D_800F62CF == 9)) {
        D_800F647A += 6;
        return;
    }
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/annual_girl_selection.rodata", D_80145A40);

void annual_candidate_scene_fixed_text_setup(void) {
    func_800BCE10(&D_800D92E0, &D_80145A40);
    func_8008A0D4(0x401A);
    func_8004DE1C();
}

void annual_candidate_voice_resource_play_53a1(void) {
    func_8008F618(0);
    func_80072734(0x53A1);
    func_8004DE1C();
}

void annual_candidate_voice_resource_play_63cd(void) {
    func_8008F618(9);
    func_80072734(0x63CD);
    func_8004DE1C();
}

s32 annual_candidate_date_rank_route(void) {
    s32 v;

    v = D_800F5490;
    if (D_800F53E0 == ((u32)(v << 0x17) >> 0x1B) && D_800F53DF == (v & 0xF)
        && func_8005EBE0(D_800F62CF) >= 0x46U) {
        D_800F5AB1 = D_800F6479;
        /* FAKE: D_800F647A read through the D_800F5AB1 base so as1 keeps the load after the store. T-11430 */
        D_800F5AB2 = (&D_800F5AB1)[0x9C9] + 1;
        func_8004DEAC(3);
        return 0;
    }
    if (func_8005EBE0(D_800F62CF) >= 0x46U) {
        annual_scene_group_load_setup();
        return;
    }
    func_8004DE1C();
}
