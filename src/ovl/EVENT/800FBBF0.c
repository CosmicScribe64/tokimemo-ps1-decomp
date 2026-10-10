#include "common.h"
#include "ovl/EVENT.h"

typedef struct {
    void (*f[38])();
} FnTbl38; /* size 0x98 */
extern FnTbl38 D_8012153C;

typedef struct {
    void (*f[41])();
} FnTbl41; /* size 0xA4 */
extern FnTbl41 D_80121360;

void func_800FBBF0(void) {
    D_801211E0 = 0x801D21D8;
    D_801211E4 = 0x801D21E0;
    D_801211E8 = 0x801D2218;
    D_801211EC = *(s16 *)0x801D2238;
    D_801211F0 = 0x801B0000;
    D_801211F4 = 0x801B2000;
    D_801211F8 = 0x801B6000;
    D_801211FC = 0x801BA000;
    D_80121200 = 0x801BE000;
    D_80121204 = 0x801C2000;
    D_80121208 = 0x801C6000;
}

void func_800FBCA0(void) {
    D_8012120C = 0x801B0000;
    D_80121210 = 0x801B2000;
    D_80121214 = 0x801B6000;
    D_80121218 = 0x801BA000;
    D_8012121C = 0x801BE000;
    D_80121220 = 0x801C2000;
    D_80121224 = 0x801C6000;
}

void func_800FBD10(void) {
    D_80121228 = 0x801CE19C;
    D_8012122C = 0x801CE1A0;
    D_80121230 = 0x801CE1D8;
    D_80121234 = *(s16 *)0x801CE1F4;
    D_80121238 = 0x801B0000;
    D_8012123C = 0x801B2000;
    D_80121240 = 0x801B6000;
    D_80121244 = 0x801BA000;
    D_80121248 = 0x801BE000;
    D_8012124C = 0x801C2000;
    D_80121250 = 0x801C6000;
}

void func_800FBDC0(void) {
    D_80121254 = 0x801D2368;
    D_80121258 = 0x801D236C;
    D_8012125C = 0x801D239C;
    D_80121260 = *(s16 *)0x801D23B8;
    D_80121264 = 0x801B0000;
    D_80121268 = 0x801D2000;
    D_8012126C = 0x801B2000;
    D_80121270 = 0x801B6000;
    D_80121274 = 0x801BA000;
    D_80121278 = 0x801BE000;
    D_8012127C = 0x801C2000;
    D_80121280 = 0x801C6000;
    D_80121284 = 0x801CE000;
}

void func_800FBE90(void) {
    D_80121288 = 0x801D2170;
    D_8012128C = 0x801D2178;
    D_80121290 = 0x801D21A4;
    D_80121294 = *(s16 *)0x801D21C0;
    D_80121298 = 0x801B0000;
    D_8012129C = 0x801B2000;
    D_801212A0 = 0x801B6000;
    D_801212A4 = 0x801BA000;
    D_801212A8 = 0x801BE000;
    D_801212AC = 0x801C2000;
    D_801212B0 = 0x801C6000;
}

void func_800FBF40(void) {
    switch (D_800EECB0) {
    case 1:
        func_800FBFE0();
        return;
    case 2:
        func_800FCD0C();
        return;
    case 3:
        func_800FCDD8();
        return;
    case 4:
        func_800FD00C();
        return;
    case 5:
        func_800FD1C0();
        return;
    default:
        func_80015FE0();
        return;
    }
}

void func_800FBFE0(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_800FC040();
        return;
    }
    func_80015FE0();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FBBF0", func_800FC040);

void func_800FC184(void) {
    func_80015D28(0x45, 0x801B0000, 0x7D8F);
    func_800FBBF0();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FBBF0", func_800FC1BC);

void func_800FC300(void) {
    func_800FBBF0();
    func_80012D64(D_801211F0, 0x11, 1, 2, 0);
    func_8004C46C(D_801211F4, D_801211F8, D_801211FC, D_80121200, D_80121204, D_80121208);
    func_8004C6B0(0, 0, 0, 0);
    func_800189C4(0x63);
    D_800EB06D = 0xA;
    D_800EB06E = 0;
    D_800EB0A4 = 0x41000000;
    D_800EB06F = 4;
    D_800EB078 = D_801211E4;
    D_800EB07C = D_801211E8;
    D_800EB0A0 = D_801211E0;
    D_800EB080 = D_801211EC;
    D_800EB082 = 4;
    D_800EB084 = 0;
    D_800EB072 = 1;
    D_800EB0AF = 0x11;
    D_800EB092 = -0xA0;
    D_800EB096 = -0x78;
    D_800EB073 = 0;
    D_800EB071 = 0;
    D_800EB070 = 0;
    D_800B1C70 = -1;
    func_80011DFC();
}

void func_800FC46C(void) {
    D_80120678 = D_80120370;
    D_8012067C = D_80120388;
    D_80120680 = D_801203A0;
    func_800FBCA0();
    func_80012D64(D_8012120C, 0x11, 1, 2, 0);
    func_8004C46C(D_80121210, D_80121214, D_80121218, D_8012121C, D_80121220, D_80121224);
    func_8004C6B0(0, 0, 0, 0);
    D_800B0A04[2].unk_02 += 3;
    D_800B0A04[2].unk_06 += 2;
    D_800B0A04[2].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

typedef struct {
    s32 w[0x11];
} Ev44; /* size 0x44: record of D_800EAFA0 */

void func_800FC56C(void) {
    D_80120678 = D_80120374;
    D_8012067C = D_8012038C;
    D_80120680 = D_801203A4;
    D_800B0A04[2].unk_02 += 2;
    D_800B0A04[2].unk_0A -= 0xA;
    func_8004C250();
    func_800FBD10();
    func_80012D64(D_80121238, 0x11, 1, 2, 0);
    func_8004C46C(D_8012123C, D_80121240, D_80121244, D_80121248, D_8012124C, D_80121250);
    func_8004C6B0(D_8012122C, D_80121230, D_80121228, D_80121234);
    *(Ev44 *)&D_800EAFA0[0x88] = *(Ev44 *)D_800EAFA0;
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FBBF0", func_800FC6B8);

void func_800FC994(void) {
    D_80120678 = D_8012037C;
    D_8012067C = D_80120394;
    D_80120680 = D_801203AC;
    func_800FBE90();
    func_80012D64(D_80121298, 0x11, 1, 2, 0);
    func_8004C46C(D_8012129C, D_801212A0, D_801212A4, D_801212A8, D_801212AC, D_801212B0);
    func_8004C6B0(D_8012128C, D_80121290, D_80121288, D_80121294);
    func_800189C4(0x62);
    D_800EB029 = 9;
    D_800EB02A = 1;
    D_800EB060 = 0x41000000;
    D_800EB02B = 4;
    D_800EB034 = D_8012128C;
    D_800EB038 = D_80121290;
    D_800EB05C = D_80121288;
    D_800EB03C = D_80121294;
    D_800EB03E = 4;
    D_800EB040 = 0;
    D_800EB02E = 1;
    D_800EB06B = 0x11;
    D_800EB04E = -0xA0;
    D_800EB052 = -0x78;
    D_800EB02F = 0;
    D_800EB02D = 0;
    D_800EB02C = 0;
    D_800B0A04[2].unk_02 += 3;
    D_800B0A04[2].unk_06 += 2;
    D_800B0A04[2].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void func_800FCB7C(void) {
    func_800469F4(0x40C6);
    func_80011DFC();
}

void func_800FCBA4(void) {
    func_800469F4(0x4613);
    func_80011DFC();
}

void func_800FCBCC(void) {
    func_800469F4(0x4500);
    func_80011DFC();
}

void func_800FCBF4(void) {
    func_800469F4(0x44F6);
    func_80011DFC();
}

void func_800FCC1C(void) {
    func_800469F4(0x486C);
    func_80011DFC();
}

void func_800FCC44(void) {
    func_800469F4(0x45AA);
    func_80011DFC();
}

void func_800FCC6C(void) {
    func_800469F4(0x478B);
    func_80011DFC();
}

void func_800FCC94(void) {
    func_800469F4(0x444F);
    func_80011DFC();
}

void func_800FCCBC(void) {
    func_800469F4(0x44B4);
    func_80011DFC();
}

void func_800FCCE4(void) {
    func_800469F4(0x4499);
    func_80011DFC();
}

void func_800FCD0C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl41 tbl;

    tbl = D_80121360;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    tbl.f[idx]();
}

void func_800FCDA0(void) {
    func_80015D28(0x35, 0x801B0000, 0x7DD4);
    func_800FBCA0();
    func_80011DFC();
}

typedef struct {
    void (*f[39])();
} FnTbl39; /* size 0x9C */
extern FnTbl39 D_80121404;

void func_800FCDD8(void) {
    s32 idx; /* FAKE: unused, declared before tbl for its stack slot (T-3330) */
    FnTbl39 tbl;
    u32 r;

    tbl = D_80121404;
    tbl.f[D_800B1AF6](0x80);
    if (D_800B1AF6 >= 0xD && D_800B1AF6 < 0x10) {
        r = D_800B1AE4 % 180;
        if (r == (D_800B1AE4 * 0)) { /* FAKE: permuter find, the no-op multiply moves the load into $v1 */
            func_800FCFA4();
            r = D_800B1AE4 % 180;
        }
        if (r == 0x5A) {
            func_800FCFC4();
        }
    }
}

void func_800FCEBC(void) {
    func_80015D28(0x3D, 0x801B0000, 0x7E09);
    func_800FBD10();
    func_80011DFC();
}

void func_800FCEF4(void) {
    func_8004C984();
    D_800EB02F = D_800F19AB;
    if ((u8) D_800F19AB < 8U) {
        D_800EB02B &= 0xFF7F;
    }
}

void func_800FCF40(void) {
    D_800EAFA0[0x8F] = 0x80;
    D_800EAFA0[0x8B] |= 0x80;
    *(s16 *)&D_800EAFA0[0x90] = 0;
    *(s16 *)&D_800EAFA0[0x9E] = 4;
    *(s16 *)&D_800EAFA0[0xA0] = 0;
    D_800EAFA0[0x8A] = 1;
    func_80011DFC();
}

void func_800FCFA4(void) {
    func_800433D0(0x500);
}

void func_800FCFC4(void) {
    func_800433D0(0x503);
}

void func_800FCFE4(void) {
    func_800433D0(0x504);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FBBF0", func_800FD00C);

void func_800FD160(void) {
    func_800433D0(0x502);
    func_80011DFC();
}

void func_800FD188(void) {
    func_80015D28(0x45, 0x801B0000, 0x7E46);
    func_800FBDC0();
    func_80011DFC();
}

void func_800FD1C0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl38 tbl;

    tbl = D_8012153C;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    if (D_800EECE0 != 0) {
        D_800EB02B |= 0x80;
        D_800EB02F = D_800F19AB;
        return;
    }
    D_800EB02B &= 0xFF7F;
}

void func_800FD2AC(void) {
    func_80015D28(0x45, 0x801B0000, 0x7E8B);
    func_800FBE90();
    func_80011DFC();
}
