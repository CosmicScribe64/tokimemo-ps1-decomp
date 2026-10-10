#include "common.h"
#include "ovl/EVENT.h"

typedef struct {
    void (*f[15])();
} FnTbl15; /* size 0x3C */

typedef struct {
    void (*f[42])();
} FnTbl42; /* size 0xA8 */

typedef struct {
    void (*f[38])();
} FnTbl38; /* size 0x98 */

void func_800FFBE0(void) {
    D_80121D20 = 0x801CE218;
    D_80121D24 = 0x801CE21C;
    D_80121D28 = 0x801CE25C;
    D_80121D2C = *(s16 *)0x801CE27C;
    D_80121D30 = 0x801B0000;
    D_80121D34 = 0x801B2000;
    D_80121D38 = 0x801B6000;
    D_80121D3C = 0x801BA000;
    D_80121D40 = 0x801BE000;
    D_80121D44 = 0x801C2000;
    D_80121D48 = 0x801C6000;
}

void func_800FFC90(void) {
    D_80121D4C = 0x801CE124;
    D_80121D50 = 0x801CE128;
    D_80121D54 = 0x801CE148;
    D_80121D58 = *(s16 *)0x801CE15C;
    D_80121D5C = 0x801B0000;
    D_80121D60 = 0x801B2000;
    D_80121D64 = 0x801B6000;
    D_80121D68 = 0x801BA000;
    D_80121D6C = 0x801BE000;
    D_80121D70 = 0x801C2000;
    D_80121D74 = 0x801C6000;
}

void func_800FFD40(void) {
    D_80121D78 = 0x801CE0F4;
    D_80121D7C = 0x801CE0F8;
    D_80121D80 = 0x801CE118;
    D_80121D84 = *(s16 *)0x801CE12C;
    D_80121D88 = 0x801B0000;
    D_80121D8C = 0x801B2000;
    D_80121D90 = 0x801B6000;
    D_80121D94 = 0x801BA000;
    D_80121D98 = 0x801BE000;
    D_80121D9C = 0x801C2000;
    D_80121DA0 = 0x801C6000;
}

void func_800FFDF0(void) {
    D_80121DA4 = 0x801CE088;
    D_80121DA8 = 0x801CE08C;
    D_80121DAC = 0x801CE0A0;
    D_80121DB0 = *(s16 *)0x801CE0B4;
    D_80121DB4 = 0x801B0000;
    D_80121DB8 = 0x801B2000;
    D_80121DBC = 0x801B6000;
    D_80121DC0 = 0x801BA000;
    D_80121DC4 = 0x801BE000;
    D_80121DC8 = 0x801C2000;
    D_80121DCC = 0x801C6000;
}

void func_800FFEA0(void) {
    D_80121DD0 = 0x801CE0F4;
    D_80121DD4 = 0x801CE0F8;
    D_80121DD8 = 0x801CE118;
    D_80121DDC = *(s16 *)0x801CE12C;
    D_80121DE0 = 0x801B0000;
    D_80121DE4 = 0x801B2000;
    D_80121DE8 = 0x801B6000;
    D_80121DEC = 0x801BA000;
    D_80121DF0 = 0x801BE000;
    D_80121DF4 = 0x801C2000;
    D_80121DF8 = 0x801C6000;
}

void func_800FFF50(void) {
    switch (D_800EECB0) {
    case 1:
        func_80100010();
        break;
    case 2:
        func_80100CEC();
        break;
    case 3:
        func_80100E30();
        break;
    case 4:
        func_80100F04();
        break;
    case 5:
        func_80100FC0();
        break;
    case 6:
        func_80101104();
        break;
    case 8:
        func_801001F4();
        break;
    default:
        func_80015FE0();
        break;
    }
}

void func_80100010(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_80100070();
        return;
    }
    func_80015FE0();
}

extern FnTbl38 D_80121DFC;

void func_80100070(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    FnTbl38 tbl;

    tbl = D_80121DFC;
    func_80078950("s%d %d\n", D_800B1AF6, D_800E9E63);
    tbl.f[D_800B1AF6](0x80);
}

void func_8010010C(void) {
    func_80015D28(0x3D, 0x801B0000, 0x80A0);
    func_800FFBE0();
    func_80011DFC();
}

void func_80100144(void) {
    func_80078970(D_80094764, "男子１");
    D_800EAFB6 = 4;
    func_80011DFC();
}

void func_80100180(void) {
    func_80078970(D_80094764, "男子２");
    D_800EAFB6 = 5;
    func_80011DFC();
}

void func_801001BC(void) {
    func_8004BC20((s32) D_800B1746);
    D_800EAFB6 = 2;
    func_80011DFC();
}

void func_801001F4(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_80100254();
        return;
    }
    func_80015FE0();
}

typedef struct {
    void (*f[21])();
} FnTbl21; /* size 0x54 */
extern FnTbl21 D_80121E94;

void func_80100254(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl21 tbl;

    tbl = D_80121E94;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FFBE0", func_801002C8);

void func_80100448(void) {
    func_800FFBE0();
    func_80012D64(D_80121D30, 0x11, 1, 2, 0);
    func_8004C46C(D_80121D34, D_80121D38, D_80121D3C, D_80121D40, D_80121D44, D_80121D48);
    func_8004C6B0(D_80121D24, D_80121D28, D_80121D20, (s32) D_80121D2C);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FFBE0", func_801004F0);

void func_80100614(void) {
    D_80120678 = D_801204D8;
    D_8012067C = D_801204F4;
    D_80120680 = D_80120510;
    D_8012438C = 1;
    D_8012438E = 2;
    D_80124390 = 4;
    D_80124394 = 1;
    D_80124396 = 2;
    D_80124398 = 3;
    D_8012439C = 0;
    D_8012439E = 2;
    D_801243A0 = 4;
    D_801243A4 = 0;
    D_801243A6 = 2;
    D_801243A8 = 4;
    func_800FFC90();
    func_80012D64(D_80121D5C, 0x11, 1, 2, 0);
    func_8004C46C(D_80121D60, D_80121D64, D_80121D68, D_80121D6C, D_80121D70, D_80121D74);
    func_8004C6B0(D_80121D50, D_80121D54, D_80121D4C, (s32) D_80121D58);
    func_80011DFC();
}

void func_80100770(void) {
    D_80120678 = D_801204DC;
    D_8012067C = D_801204F8;
    D_80120680 = D_80120514;
    D_800B0A04[7].unk_02 += 1;
    D_800B0A04[7].unk_06 += 1;
    D_800B0A04[7].unk_0A -= 0x14;
    func_8004C250();
    func_800FFD40();
    func_80012D64(D_80121D88, 0x11, 1, 2, 0);
    func_8004C46C(D_80121D8C, D_80121D90, D_80121D94, D_80121D98, D_80121D9C, D_80121DA0);
    func_8004C6B0(D_80121D7C, D_80121D80, D_80121D78, D_80121D84);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FFBE0", func_80100888);

void func_801009A0(void) {
    D_80120678 = D_801204E4;
    D_8012067C = D_80120500;
    D_80120680 = D_8012051C;
    D_800B0A04[7].unk_02 += 3;
    D_800B0A04[7].unk_06 += 2;
    D_800B0A04[7].unk_0A -= 0x14;
    func_8004C250();
    func_800FFEA0();
    func_80012D64(D_80121DE0, 0x11, 1, 2, 0);
    func_8004C46C(D_80121DE4, D_80121DE8, D_80121DEC, D_80121DF0, D_80121DF4, D_80121DF8);
    func_8004C6B0(D_80121DD4, D_80121DD8, D_80121DD0, D_80121DDC);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FFBE0", func_80100AB8);

void func_80100B34(void) {
    func_800469F4(0x3FDE);
    func_80011DFC();
}

void func_80100B5C(void) {
    func_800469F4(0x4667);
    func_80011DFC();
}

void func_80100B84(void) {
    func_800469F4(0x47B5);
    func_80011DFC();
}

void func_80100BAC(void) {
    func_800469F4(0x4499);
    func_80011DFC();
}

void func_80100BD4(void) {
    func_800469F4(0x481D);
    func_80011DFC();
}

void func_80100BFC(void) {
    func_800469F4(0x451C);
    func_80011DFC();
}

void func_80100C24(void) {
    func_800469F4(0x4770);
    func_80011DFC();
}

void func_80100C4C(void) {
    func_800469F4(0x42A8);
    func_80011DFC();
}

void func_80100C74(void) {
    func_800469F4(0x47D8);
    func_80011DFC();
}

void func_80100C9C(void) {
    func_800469F4(0x44D1);
    func_80011DFC();
}

void func_80100CC4(void) {
    func_800469F4(0x3FDE);
    func_80011DFC();
}

typedef struct {
    void (*f[40])();
} FnTbl40; /* size 0xA0 */
extern FnTbl40 D_80121EE8;

void func_80100CEC(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    FnTbl40 tbl;

    tbl = D_80121EE8;
    func_80078950("%d\n", D_800B1AF6);
    tbl.f[D_800B1AF6](0x80);
}

void func_80100D78(void) {
    func_800433D0(0x500);
    func_80011DFC();
}

void func_80100DA0(void) {
    func_80015D28(0x3D, 0x801B0000, 0x80DD);
    func_800FFC90();
    func_80011DFC();
}

void func_80100DD8(void) {
    u16 t;

    /* FAKE: one-line do-while around both statements fixes the as1 load order (lhu before lw); T-8040 */
    do { t = D_80094714; D_80094714 = t + D_800EECBC; } while (0);
    func_80012D64(D_80121D5C, 0x11, 1, 2, D_800EECBC);
    func_80011DFC();
}

extern FnTbl38 D_80121F88;

void func_80100E30(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    FnTbl38 tbl;

    tbl = D_80121F88;
    func_80078950("%d %d\n", D_800B1AF6, D_800EECD0);
    tbl.f[D_800B1AF6](0x80);
}

void func_80100ECC(void) {
    func_80015D28(0x3D, 0x801B0000, 0x811A);
    func_800FFD40();
    func_80011DFC();
}

typedef struct {
    void (*f[51])();
} FnTbl51; /* size 0xCC */
extern FnTbl51 D_80122020;

void func_80100F04(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    FnTbl51 tbl;

    tbl = D_80122020;
    func_80078950("%d\n", D_800B1AF6);
    tbl.f[D_800B1AF6](0x80);
}

void func_80100F88(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8157);
    func_800FFDF0();
    func_80011DFC();
}

void func_80100FC0(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    FnTbl42 tbl;

    tbl = *(FnTbl42 *)&D_801220EC;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    tbl.f[D_800B1AF6](0x80);
}

void func_80101054(void) {
    func_800433D0(0x502);
    func_80011DFC();
}

void func_8010107C(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8194);
    func_800FFEA0();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FFBE0", func_801010B4);

extern FnTbl15 D_80122194;

void func_80101104(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    FnTbl15 tbl;

    tbl = D_80122194;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    tbl.f[D_800B1AF6](0x80);
}
