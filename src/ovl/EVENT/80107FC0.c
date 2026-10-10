#include "common.h"
#include "ovl/EVENT.h"

typedef struct {
    void (*f[47])();
} FnTbl47; /* size 0xBC */
extern FnTbl47 D_80123990;

typedef struct {
    void (*f[21])();
} FnTbl21; /* size 0x54 */
extern FnTbl21 D_80123A4C;

typedef struct {
    void (*f[44])();
} FnTbl44; /* size 0xB0 */
extern FnTbl44 D_80123D0C;

typedef struct {
    void (*f[52])();
} FnTbl52; /* size 0xD0 */
extern FnTbl52 D_80123DBC;

typedef struct {
    void (*f[41])();
} FnTbl41; /* size 0xA4 */
extern FnTbl41 D_80123BB8;

void func_80107FC0(void) {
    D_80123880 = 0x801D21BC;
    D_80123884 = 0x801D21C4;
    D_80123888 = 0x801D2204;
    D_8012388C = *(s16 *)0x801D221C;
    D_80123890 = 0x801B0000;
    D_80123894 = 0x801B2000;
    D_80123898 = 0x801B6000;
    D_8012389C = 0x801BA000;
    D_801238A0 = 0x801BE000;
    D_801238A4 = 0x801C2000;
    D_801238A8 = 0x801C6000;
}

void func_80108070(void) {
    D_801238AC = 0x801CE0F4;
    D_801238B0 = 0x801CE0F8;
    D_801238B4 = 0x801CE118;
    D_801238B8 = *(s16 *)0x801CE12C;
    D_801238BC = 0x801B0000;
    D_801238C0 = 0x801B2000;
    D_801238C4 = 0x801B6000;
    D_801238C8 = 0x801BA000;
    D_801238CC = 0x801BE000;
    D_801238D0 = 0x801C2000;
    D_801238D4 = 0x801C6000;
}

void func_80108120(void) {
    D_801238D8 = 0x801CE0F4;
    D_801238DC = 0x801CE0F8;
    D_801238E0 = 0x801CE118;
    D_801238E4 = *(s16 *)0x801CE12C;
    D_801238E8 = 0x801B0000;
    D_801238EC = 0x801B2000;
    D_801238F0 = 0x801B6000;
    D_801238F4 = 0x801BA000;
    D_801238F8 = 0x801BE000;
    D_801238FC = 0x801C2000;
    D_80123900 = 0x801C6000;
}

void func_801081D0(void) {
    D_80123904 = 0x801CE0F4;
    D_80123908 = 0x801CE0F8;
    D_8012390C = 0x801CE118;
    D_80123910 = *(s16 *)0x801CE12C;
    D_80123914 = 0x801B0000;
    D_80123918 = 0x801B2000;
    D_8012391C = 0x801B6000;
    D_80123920 = 0x801BA000;
    D_80123924 = 0x801BE000;
    D_80123928 = 0x801C2000;
    D_8012392C = 0x801C6000;
}

void func_80108280(void) {
    D_80123930 = 0x801D2470;
    D_80123934 = 0x801D2478;
    D_80123938 = 0x801D24B8;
    D_8012393C = *(s16 *)0x801D24D0;
    D_80123940 = 0x801B0000;
    D_80123944 = 0x801B2000;
    D_80123948 = 0x801B6000;
    D_8012394C = 0x801BA000;
    D_80123950 = 0x801BE000;
    D_80123954 = 0x801C2000;
    D_80123958 = 0x801C6000;
}

void func_80108330(void) {
    D_8012395C = 0x801D2600;
    D_80123960 = 0x801D2608;
    D_80123964 = 0x801D2674;
    D_80123968 = *(s16 *)0x801D268C;
    D_8012396C = 0x801B0000;
    D_80123970 = 0x801B2000;
    D_80123974 = 0x801B6000;
    D_80123978 = 0x801BA000;
    D_8012397C = 0x801BE000;
    D_80123980 = 0x801C2000;
    D_80123984 = 0x801C6000;
}

void func_801083E0(void) {
    switch (D_800EECB0) {
    case 1:
        func_801084A0();
        return;
    case 2:
        func_80108500();
        return;
    case 3:
        func_80109E14();
        return;
    case 4:
        func_80109EE8();
        return;
    case 5:
        func_8010A008();
        return;
    case 6:
        func_8010A0E4();
        return;
    case 7:
        func_80108560();
        return;
    default:
        func_80015FE0();
        return;
    }
}

void func_801084A0(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_801085C0();
        return;
    }
    func_80015FE0();
}

void func_80108500(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_80108680();
        return;
    }
    func_80015FE0();
}

void func_80108560(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_801087F8();
        return;
    }
    func_80015FE0();
}

void func_801085C0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl47 tbl;

    tbl = D_80123990;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_80108648(void) {
    func_80015D28(0x45, 0x801B0000, 0x8A1D);
    func_80107FC0();
    func_80011DFC();
}

void func_80108680(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl21 tbl;

    tbl = D_80123A4C;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_801086F4(void) {
    if (D_800B0E6A != 0) {
        D_80094714 = (u16) D_80094714 + 5;
    }
    func_80011DFC();
}

void func_80108734(void) {
    if (D_800B0E6A != 0) {
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
    }
    func_80011DFC();
}

void func_801087A8(void) {
    if (((u8)func_8002328C(D_800B1746) & 0x7F) >= 2U) {
        D_80094714 += 2;
    }
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_801087F8);

void func_801088E8(void) {
    func_80015D28(0x45, 0x801B0000, 0x8B5E);
    func_80108330();
    func_80011DFC();
}

void func_80108920(void) {
    func_800433D0(0x500);
    func_80011DFC();
}

void func_80108948(void) {
    func_800143DC(1, 0xB60A, 0xB605, 0xC906, 0xC8B8, 0xC8B3);
    func_80011DFC();
}

void func_8010898C(void) {
    func_800F7074();
    if (D_800EECC0 != 0) {
        D_800EECC0 = 0;
        D_800EECCC = 0;
        D_800E96AB &= 0xFF7F;
        D_80094714 = (u16) D_80094714 + 1;
        D_80094718 = 0;
        func_80011DFC();
    }
}

void func_801089FC(void) {
    D_8008093C = 0xF;
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_80108A24);

void func_80108AD8(void) {
    func_80108B24(-0xA0, -0x78, 0x140, 0xA0, 0xFFFFFF, 0xA, 0);
    func_8001A16C(9);
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_80108B24);

void func_80108D00(void) {
    s16 i;

    D_8008093C = 0xA;
    func_8004C808(1);
    D_800EECE0 = 1;
    D_800EAFA7 = 0x80;
    D_800EAFEB = 0x80;
    for (i = 0x64; i < 0x6A; i++) {
        func_8004C060(i, 0x80);
    }
    D_80123988 = 0;
    for (i = 0; i < 0xA; i++) {
        D_800E9620[i * 0x44 + 0x1A4F] |= 0x80;
    }
    D_8012398C = 1;
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_80108DD4);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_80108F50);

void func_801090D4(void) {
    func_80107FC0();
    func_80012D64(D_80123890, 0x11, 1, 2, 0);
    func_8004C46C(D_80123894, D_80123898, D_8012389C, D_801238A0, D_801238A4, D_801238A8);
    func_8004C6B0(D_80123884, D_80123888, D_80123880, (s32) D_8012388C);
    func_80011DFC();
}

void func_8010917C(void) {
    D_800B1746 = 0xA;
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
    D_80120678 = D_801205C8;
    D_8012067C = D_801205E4;
    D_80120680 = D_80120600;
    if (((u8)func_8002328C(0xA) & 0x7F) < 2U) {
        D_800B0A04[10].unk_02 += 1;
    }
    D_800B0A04[10].unk_06 += 2;
    func_8004C250();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_801092C8);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_80109404);

void func_80109704(void) {
    D_80120678 = D_801205CC;
    D_8012067C = D_801205E8;
    D_80120680 = D_80120604;
    D_800B0A04[10].unk_02 += 1;
    D_800B0A04[10].unk_06 += 1;
    D_800B0A04[10].unk_0A -= 0xA;
    func_8004C250();
    func_80108070();
    func_80012D64(D_801238BC, 0x11, 1, 2, 0);
    func_8004C46C(D_801238C0, D_801238C4, D_801238C8, D_801238CC, D_801238D0, D_801238D4);
    func_8004C6B0(D_801238B0, D_801238B4, D_801238AC, D_801238B8);
    func_80011DFC();
}

void func_8010981C(void) {
    D_80120678 = D_801205D0;
    D_8012067C = D_801205EC;
    D_80120680 = D_80120608;
    func_80078970(D_800947C4, "池");
    D_800B0A04[10].unk_02 += 3;
    D_800B0A04[10].unk_06 += 2;
    D_800B0A04[10].unk_0A -= 0x14;
    func_8004C250();
    func_80108120();
    func_80012D64(D_801238E8, 0x11, 1, 2, 0);
    func_8004C46C(D_801238EC, D_801238F0, D_801238F4, D_801238F8, D_801238FC, D_80123900);
    func_8004C6B0(D_801238DC, D_801238E0, D_801238D8, D_801238E4);
    func_80011DFC();
}

void func_80109948(void) {
    D_80120678 = D_801205D4;
    D_8012067C = D_801205F0;
    D_80120680 = D_8012060C;
    D_800B0A04[10].unk_06 += 2;
    D_800B0A04[10].unk_0A -= 0xA;
    func_8004C250();
    func_801081D0();
    func_80012D64(D_80123914, 0x11, 1, 2, 0);
    func_8004C46C(D_80123918, D_8012391C, D_80123920, D_80123924, D_80123928, D_8012392C);
    func_8004C6B0(D_80123908, D_8012390C, D_80123904, D_80123910);
    func_80011DFC();
}

void func_80109A4C(void) {
    D_80120678 = D_801205D8;
    D_8012067C = D_801205F4;
    D_80120680 = D_80120610;
    D_800B0A04[10].unk_02 += 2;
    D_800B0A04[10].unk_06 += 1;
    D_800B0A04[10].unk_0A -= 0x14;
    func_8004C250();
    func_80108280();
    func_80012D64(D_80123940, 0x11, 1, 2, 0);
    func_8004C46C(D_80123944, D_80123948, D_8012394C, D_80123950, D_80123954, D_80123958);
    func_8004C6B0(D_80123934, D_80123938, D_80123930, D_8012393C);
    func_80011DFC();
}

void func_80109B64(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}

void func_80109B8C(void) {
    func_800469F4(0x4609);
    func_80011DFC();
}

void func_80109BB4(void) {
    func_800469F4(0x401E);
    func_80011DFC();
}

void func_80109BDC(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}

void func_80109C04(void) {
    func_800469F4(0x4767);
    func_80011DFC();
}

void func_80109C2C(void) {
    func_800469F4(0x42A8);
    func_80011DFC();
}

void func_80109C54(void) {
    func_800469F4(0x428D);
    func_80011DFC();
}

void func_80109C7C(void) {
    func_800469F4(0x472E);
    func_80011DFC();
}

void func_80109CA4(void) {
    func_800469F4(0x4284);
    func_80011DFC();
}

void func_80109CCC(void) {
    func_800469F4(0x485A);
    func_80011DFC();
}

void func_80109CF4(void) {
    func_800469F4(0x4571);
    func_80011DFC();
}

void func_80109D1C(void) {
    func_800469F4(0x47E1);
    func_80011DFC();
}

void func_80109D44(void) {
    func_800469F4(0x44DB);
    func_80011DFC();
}

void func_80109D6C(void) {
    func_800469F4(0x44D1);
    func_80011DFC();
}

void func_80109D94(void) {
    func_800469F4(0x40E0);
    func_80011DFC();
}

void func_80109DBC(void) {
    func_800433D0(0x200);
    func_800469F4(0x40F9);
    func_80011DFC();
}

void func_80109DEC(void) {
    func_800469F4(0x469A);
    func_80011DFC();
}

void func_80109E14(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl41 tbl;

    tbl = D_80123BB8;
    func_80078950("%d %d\n", D_800B1AF6, D_800EECD0);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_80109EB0(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8A62);
    func_80108070();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_80109EE8);

void func_80109FB0(void) {
    func_800433D0(0x500);
}

void func_80109FD0(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8A9F);
    func_80108120();
    func_80011DFC();
}

void func_8010A008(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl44 tbl;

    tbl = D_80123D0C;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_8010A0AC(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8ADC);
    func_801081D0();
    func_80011DFC();
}

void func_8010A0E4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl52 tbl;

    tbl = D_80123DBC;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    idx = D_800B1AF6;
    tbl.f[idx](0x3C);
}

void func_8010A180(void) {
    func_800433D0(0x501);
    func_80011DFC();
}

void func_8010A1A8(void) {
    func_800433D0(0x500);
    func_80011DFC();
}

void func_8010A1D0(void) {
    func_80015D28(0x45, 0x801B0000, 0x8B19);
    func_80108280();
    func_80011DFC();
}
