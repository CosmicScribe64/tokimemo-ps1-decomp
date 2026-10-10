#include "common.h"
#include "ovl/EVENT.h"

void func_80106C20(void) {
    D_80123430 = 0x801CE120;
    D_80123434 = 0x801CE124;
    D_80123438 = 0x801CE144;
    D_8012343C = *(s16 *)0x801CE158;
    D_80123440 = 0x801B0000;
    D_80123444 = 0x801B2000;
    D_80123448 = 0x801B6000;
    D_8012344C = 0x801BA000;
    D_80123450 = 0x801BE000;
    D_80123454 = 0x801C2000;
    D_80123458 = 0x801C6000;
}

void func_80106CD0(void) {
    D_8012345C = 0x801CE124;
    D_80123460 = 0x801CE128;
    D_80123464 = 0x801CE148;
    D_80123468 = *(s16 *)0x801CE15C;
    D_8012346C = 0x801B0000;
    D_80123470 = 0x801B2000;
    D_80123474 = 0x801B6000;
    D_80123478 = 0x801BA000;
    D_8012347C = 0x801BE000;
    D_80123480 = 0x801C2000;
    D_80123484 = 0x801C6000;
}

void func_80106D80(void) {
    D_80123488 = 0x801D2240;
    D_8012348C = 0x801D2248;
    D_80123490 = 0x801D2288;
    D_80123494 = *(s16 *)0x801D22A0;
    D_80123498 = 0x801B0000;
    D_8012349C = 0x801B2000;
    D_801234A0 = 0x801B6000;
    D_801234A4 = 0x801BA000;
    D_801234A8 = 0x801BE000;
    D_801234AC = 0x801C2000;
    D_801234B0 = 0x801C6000;
}

void func_80106E30(void) {
    D_801234B4 = 0x801CE0F4;
    D_801234B8 = 0x801CE0F8;
    D_801234BC = 0x801CE118;
    D_801234C0 = *(s16 *)0x801CE12C;
    D_801234C4 = 0x801B0000;
    D_801234C8 = 0x801B2000;
    D_801234CC = 0x801B6000;
    D_801234D0 = 0x801BA000;
    D_801234D4 = 0x801BE000;
    D_801234D8 = 0x801C2000;
    D_801234DC = 0x801C6000;
}

void func_80106EE0(void) {
    D_801234E0 = 0x801CE1A0;
    D_801234E4 = 0x801CE1A4;
    D_801234E8 = 0x801CE1D4;
    D_801234EC = *(s16 *)0x801CE1F0;
    D_801234F0 = 0x801B0000;
    D_801234F4 = 0x801B2000;
    D_801234F8 = 0x801B6000;
    D_801234FC = 0x801BA000;
    D_80123500 = 0x801BE000;
    D_80123504 = 0x801C2000;
    D_80123508 = 0x801C6000;
}

void func_80106F90(void) {
    switch (D_800EECB0) {
    case 1:
        func_80107030();
        return;
    case 2:
        func_80107A28();
        return;
    case 3:
        func_80107BB0();
        return;
    case 4:
        func_80107CEC();
        return;
    case 5:
        func_80107E20();
        return;
    default:
        func_80015FE0();
        return;
    }
}

void func_80107030(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_80107090();
        return;
    }
    func_80015FE0();
}

typedef struct {
    void (*f[35])();
} FnTbl35; /* size 0x8C */
extern FnTbl35 D_8012350C;

void func_80107090(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl35 tbl;

    tbl = D_8012350C;
    idx = D_800B1AF6;
    tbl.f[idx]();
}

void func_80107118(void) {
    func_80015D28(0x3D, 0x801B0000, 0x886E);
    func_80106C20();
    func_80011DFC();
}

void func_80107150(void) {
    if ((D_800B0E43 == 0) && (D_800B0E42 == 0)) {
        D_80094714 += 4;
        D_80094718 = 0;
    } else if (D_800B0E42 != 0) {
        D_80094714 += 2;
        D_80094718 = 0;
    }
    func_80011DFC();
}

void func_801071D8(void) {
    D_800B1746 = 4;
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
    D_80120678 = D_801203FC;
    D_8012067C = D_80120414;
    D_80120680 = D_8012042C;
    func_80078970(D_80094784, "廊下");
    if (D_800B0E43 == 0 && D_800B0E42 == 0) {
        D_800B0A04[4].unk_02 += 1;
    }
    D_800B0A04[4].unk_06 += 1;
    *(s16 *)((u8 *)D_800B0A04 - 0xB6) += 0xA; /* FAKE: this is D_800B094E; addressing it from the table keeps IDO from hoisting its load */
    func_8004C250();
    func_80011DFC();
}

void func_80107354(void) {
    func_80106C20();
    func_80012D64(D_80123440, 0x11, 1, 2, 0);
    func_8004C46C(D_80123444, D_80123448, D_8012344C, D_80123450, D_80123454, D_80123458);
    func_8004C6B0(D_80123434, D_80123438, D_80123430, (s32) D_8012343C);
    func_80011DFC();
}

void func_801073FC(void) {
    D_80120678 = D_80120400;
    D_8012067C = D_80120418;
    D_80120680 = D_80120430;
    func_80106CD0();
    func_80012D64(D_8012346C, 0x11, 1, 2, 0);
    func_8004C46C(D_80123470, D_80123474, D_80123478, D_8012347C, D_80123480, D_80123484);
    func_8004C6B0(D_80123460, D_80123464, D_8012345C, (s32)D_80123468);
    D_800B0A04[4].unk_02 += 2;
    D_800B0A04[4].unk_06 += 1;
    D_800B0A04[4].unk_0A -= 0x14;
    func_8004C250();
    D_800B0A04[4].unk_02 += 3;
    D_800B0A04[4].unk_06 += 2;
    D_800B0A04[4].unk_0A -= 0x14;
    *(s16 *)((u8 *)D_800B0A04 - 0xB6) += 0xA; /* FAKE: this is D_800B094E; addressing it from the table keeps IDO from hoisting its load */
    func_8004C250();
    func_80011DFC();
}

void func_80107564(void) {
    D_80120678 = D_80120404;
    D_8012067C = D_8012041C;
    D_80120680 = D_80120434;
    func_80106D80();
    func_80012D64(D_80123498, 0x11, 1, 2, 0);
    func_8004C46C(D_8012349C, D_801234A0, D_801234A4, D_801234A8, D_801234AC, D_801234B0);
    func_8004C6B0(D_8012348C, D_80123490, D_80123488, (s32)D_80123494);
    D_800EECE8 = 1;
    D_800B0A04[4].unk_02 += 3;
    D_800B0A04[4].unk_06 += 2;
    D_800B0A04[4].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void func_80107684(void) {
    D_80120678 = D_80120408;
    D_8012067C = D_80120420;
    D_80120680 = D_80120438;
    func_80106E30();
    func_80012D64(D_801234C4, 0x11, 1, 2, 0);
    func_8004C46C(D_801234C8, D_801234CC, D_801234D0, D_801234D4, D_801234D8, D_801234DC);
    func_8004C6B0(D_801234B8, D_801234BC, D_801234B4, (s32)D_801234C0);
    D_800B0A04[4].unk_06 += 2;
    D_800B0A04[4].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void func_80107784(void) {
    D_80120678 = D_8012040C;
    D_8012067C = D_80120424;
    D_80120680 = D_8012043C;
    func_80106EE0();
    func_80012D64(D_801234F0, 0x11, 1, 2, 0);
    func_8004C46C(D_801234F4, D_801234F8, D_801234FC, D_80123500, D_80123504, D_80123508);
    func_8004C6B0(D_801234E4, D_801234E8, D_801234E0, D_801234EC);
    D_800B0A04[4].unk_02 += 1;
    D_800B0A04[4].unk_06 += 1;
    D_800B0A04[4].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void func_80107898(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}

void func_801078C0(void) {
    func_800469F4(0x4650);
    func_80011DFC();
}

void func_801078E8(void) {
    func_800469F4(0x471C);
    func_80011DFC();
}

void func_80107910(void) {
    func_800469F4(0x4284);
    func_80011DFC();
}

void func_80107938(void) {
    func_800469F4(0x4827);
    func_80011DFC();
}

void func_80107960(void) {
    func_800469F4(0x4538);
    func_80011DFC();
}

void func_80107988(void) {
    func_800469F4(0x4851);
    func_80011DFC();
}

void func_801079B0(void) {
    func_800469F4(0x4571);
    func_80011DFC();
}

void func_801079D8(void) {
    func_800469F4(0x47AF);
    func_80011DFC();
}

void func_80107A00(void) {
    func_800469F4(0x4499);
    func_80011DFC();
}

typedef struct {
    void (*f[37])();
} FnTbl37; /* size 0x94 */
extern FnTbl37 D_80123598;

void func_80107A28(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl37 tbl;

    tbl = D_80123598;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    tbl.f[idx]();
    if (D_800B1764 & 0x100) {
        func_80012D64(D_8012346C, 0x11, 1, 0, 0);
        func_80078950("S\n");
    }
    if (D_800B1764 & 0x800) {
        func_80012D64(D_8012346C, 0x11, 1, 2, 0);
        func_80078950("T\n");
    }
}

void func_80107B24(void) {
    func_80015D28(0x3D, 0x801B0000, 0x88AB);
    func_80106CD0();
    func_80011DFC();
}

void func_80107B5C(void) {
    if (D_800B0E43 != 0) {
        D_80094714 = (u16) D_80094714 + 1;
        D_80094718 = (u16) D_80094718 + 1;
    }
    func_80011DFC();
}

typedef struct {
    void (*f[55])();
} FnTbl55; /* size 0xDC */
extern FnTbl55 D_8012362C;

void func_80107BB0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl55 tbl;

    tbl = D_8012362C;
    func_80078950("%d %d\n", D_800B1AF6, D_800EECD0);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    if (D_800B1764 & 0x100) {
        func_80012D64(D_80123498, 0x11, 1, 0, 0);
        func_80078950("S\n");
    }
    if (D_800B1764 & 0x800) {
        func_80012D64(D_80123498, 0x11, 1, 2, 0);
        func_80078950("T\n");
    }
}

void func_80107CB4(void) {
    func_80015D28(0x45, 0x801B0000, 0x88E8);
    func_80106D80();
    func_80011DFC();
}

typedef struct {
    void (*f[40])();
} FnTbl40; /* size 0xA0 */
extern FnTbl40 D_80123708;

void func_80107CEC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl40 tbl;

    tbl = D_80123708;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    if (D_800B1764 & 0x100) {
        func_80012D64(D_801234C4, 0x11, 1, 0, 0);
        func_80078950("S\n");
    }
    if (D_800B1764 & 0x800) {
        func_80012D64(D_801234C4, 0x11, 1, 2, 0);
        func_80078950("T\n");
    }
}

void func_80107DE8(void) {
    func_80015D28(0x3D, 0x801B0000, 0x892D);
    func_80106E30();
    func_80011DFC();
}

typedef struct {
    void (*f[51])();
} FnTbl51; /* size 0xCC */
extern FnTbl51 D_801237A8;

void func_80107E20(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl51 tbl;

    tbl = D_801237A8;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    if (D_800B1764 & 0x100) {
        func_80012D64(D_801234F0, 0x11, 1, 0, 0);
        func_80078950("S\n");
    }
    if (D_800B1764 & 0x800) {
        func_80012D64(D_801234F0, 0x11, 1, 2, 0);
        func_80078950("T\n");
    }
}

void func_80107F24(void) {
    func_80015D28(0x3D, 0x801B0000, 0x896A);
    func_80106EE0();
    func_80011DFC();
}

void func_80107F5C(void) {
    D_800B1746 = 0xF;
    D_800EAFB6 = 4;
    func_80011DFC();
}

void func_80107F90(void) {
    D_800B1746 = 4;
    func_80011DFC();
}
