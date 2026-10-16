#include "common.h"
#include "ovl/EVENT.h"

void func_8010B360(void) {
    D_801240F0 = 0x801CE130;
    D_801240F4 = 0x801CE134;
    D_801240F8 = 0x801CE15C;
    D_801240FC = *(s16 *)0x801CE170;
    D_80124100 = 0x801B0000;
    D_80124104 = 0x801B2000;
    D_80124108 = 0x801B6000;
    D_8012410C = 0x801BA000;
    D_80124110 = 0x801BE000;
    D_80124114 = 0x801C2000;
    D_80124118 = 0x801C6000;
}

void func_8010B410(void) {
    func_8010B430();
}

void func_8010B430(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_8010B490();
    } else {
        func_80015FE0();
    }
}

void func_8010B490(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl42 tbl;

    tbl = D_80124120;
    func_80078950("s%d %d\n", D_800B1AF6, D_80094714);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    if (D_8012411C != 0) {
        func_80033E88();
    }
}

void func_8010B538(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8C15);
    func_8010B360();
    func_80011DFC();
}

void func_8010B570(void) {
    func_800F7074();
    D_800EECD0 = 0;
    D_800E9E5F |= 0x80;
    (&D_800E9E5F)[-0x44] |= 0x80;
    *(s32 *)D_800EC190 &= 0x7FFFFFFF;
    *(s32 *)(D_800EC190 + 0x24) &= 0x7FFFFFFF;
    if (D_800B1AE4 % 240U == 0) {
        func_8003424C();
    }
}

/* NON_MATCHING: T-10610, 33 diff rows left (T-17400 score), the original stores the constant 4 straight from $a0 (li a0,4; sb a0 in the jal delay slot); every form tried gets a separate li (u8 store narrows the shared constant) */
#ifdef NON_MATCHING
void func_8010B614(void) {
    u8 *p;
    s32 i;
    D_800B1746 = i = 4;
    func_8004BC20(i);
    p = D_800B0860 + D_800B1746 * 0x34;
    p[0x1B0] &= 0xFFFD;
    func_80011DFC();
}
#else
INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010B360", func_8010B614);
#endif

extern s16 D_800E9E3E;
extern s16 D_800EC194;

/* neighbouring words reached through one base symbol each: as1 keeps the loads after the stores */
void func_8010B678(void) {
    (&D_800E9E3E)[0x22] += 0x40;
    D_800E9E3E += 0x40;
    D_800EC194 += 0x40;
    (&D_800EC194)[0x12] += 0x40;
    func_80011DFC();
}

void func_8010B6E4(void) {
    func_800F7074();
    if (D_80094718 == 2) {
        D_800EECD0 = 0;
        D_800E9E5F |= 0x80;
        (&D_800E9E5F)[-0x44] |= 0x80; /* FAKE: D_800E9E1B, 0x44 below D_800E9E5F (as in func_8010B570); the record table is unknown. T-9320 */
        D_800E9E74 = 0;
        *(s32 *)D_800EC190 &= 0x7FFFFFFF;
        *(s32 *)(D_800EC190 + 0x24) &= 0x7FFFFFFF;
        if (D_800B1AE4 % 240U == 0) {
            func_8003424C();
        }
    } else {
        D_800EECD0 = 1;
        D_800E9E5F |= 0x80;
        (&D_800E9E5F)[-0x44] |= 0x80; /* FAKE: D_800E9E1B, 0x44 below D_800E9E5F (as in func_8010B570); the record table is unknown. T-9320 */
        *(s32 *)D_800EC190 &= 0x7FFFFFFF;
        *(s32 *)(D_800EC190 + 0x24) &= 0x7FFFFFFF;
        D_800EAFA2 = 0;
        D_800EAFB8 = 0;
        if (D_800B1AE4 % 240U == 0x78) {
            func_80038388();
        }
    }
}

void func_8010B844(void) {
    D_80094714 = (u16) D_80094714 + D_800B1746;
    func_80011DFC();
}

void func_8010B87C(void) {
    D_80094714 = 0xC;
    func_80011DFC();
}

/* T-9740: integer definition without an explicit return matches the original. */
s32 func_8010B8A4(void) {
    s32 mask;
    u8 value;

    func_8004C984();
    D_800EECD0 = 1;
    value = D_800EAFA7;
    ((EventFlagRecord44 *)D_800E9620)[31].unk_07 = value;
    ((EventFlagRecord44 *)D_800E9620)[30].unk_07 = value;
    func_8004C060(0, value);
    func_8004C060(1, D_800EAFA7);
    if (D_800EAFA7 < 8U) {
        mask = ~0x80;
        D_800EECD0 = 0;
        ((EventFlagRecord44 *)D_800E9620)[31].unk_03 &= mask;
        ((EventFlagRecord44 *)D_800E9620)[30].unk_03 &= mask;
        ((Rec24 *)D_800EC190)[0].unk_00 |= 0x80000000;
        ((Rec24 *)D_800EC190)[1].unk_00 |= 0x80000000;
    }
}

void func_8010B970(void) {
    D_800B1746 = 0xD;
    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_80017E50();
    func_8001FD50();
    func_80036884("");
    D_800E96EF = 0;
    D_800E9733 = 0;
    D_80094714 = 0;
    D_80094718 = 0;
    func_80045B14();
    func_800F6000();
    func_8003535C();
    func_8003580C();
    func_8004C360();
    D_800F19AB = 0;
    func_8004BC20(D_800B1746);
    func_8004CDDC();
    func_800F6000();
    D_80120678 = D_80120634;
    D_8012067C = *(s32 *)&D_80120654;
    D_80120680 = D_80120674;
    func_80078970(D_80094784, "教室");
    D_8012411C = 0;
    func_80011DFC();
}

void func_8010BA94(void) {
    func_8010B360();
    func_80012D64(D_80124100, 0x11, 1, 2, 0);
    func_8004C46C(D_80124104, D_80124108, D_8012410C, D_80124110, D_80124114, D_80124118);
    func_8004C6B0(D_801240F4, D_801240F8, D_801240F0, (s32) D_801240FC);
    func_80011DFC();
}

void func_8010BB3C(void) {
    func_800469F4(0x3FF0);
    func_80011DFC();
}

void func_8010BB64(void) {
    func_800469F4(0x4600);
    func_80011DFC();
}
