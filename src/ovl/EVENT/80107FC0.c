#include "common.h"
#include "ovl/EVENT.h"

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

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_801083E0);

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

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_801085C0);

void func_80108648(void) {
    func_80015D28(0x45, 0x801B0000, 0x8A1D);
    func_80107FC0();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_80108680);

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

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_801087A8);

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

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_80108D00);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_80108DD4);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_80108F50);

void func_801090D4(void) {
    func_80107FC0();
    func_80012D64(D_80123890, 0x11, 1, 2, 0);
    func_8004C46C(D_80123894, D_80123898, D_8012389C, D_801238A0, D_801238A4, D_801238A8);
    func_8004C6B0(D_80123884, D_80123888, D_80123880, (s32) D_8012388C);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_8010917C);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_801092C8);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_80109404);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_80109704);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_8010981C);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_80109948);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80107FC0", func_80109A4C);

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
