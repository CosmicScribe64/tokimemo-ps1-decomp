#include "common.h"
#include "ovl/EVENT.h"

void func_800F7220(void) {
    D_80120690 = 0x801CE0FC;
    D_80120694 = 0x801CE100;
    D_80120698 = 0x801CE120;
    D_8012069C = *(s16 *)0x801CE13C;
    D_801206A0 = 0x801B0000;
    D_801206A4 = 0x801B2000;
    D_801206A8 = 0x801B6000;
    D_801206AC = 0x801BA000;
    D_801206B0 = 0x801BE000;
    D_801206B4 = 0x801C2000;
    D_801206B8 = 0x801C6000;
}

void func_800F72D0(void) {
    D_801206BC = 0x801CE0F4;
    D_801206C0 = 0x801CE0F8;
    D_801206C4 = 0x801CE118;
    D_801206C8 = *(s16 *)0x801CE12C;
    D_801206CC = 0x801B0000;
    D_801206D0 = 0x801B2000;
    D_801206D4 = 0x801B6000;
    D_801206D8 = 0x801BA000;
    D_801206DC = 0x801BE000;
    D_801206E0 = 0x801C2000;
    D_801206E4 = 0x801C6000;
}

void func_800F7380(void) {
    D_801206E8 = 0x801D2184;
    D_801206EC = 0x801D218C;
    D_801206F0 = 0x801D21B4;
    D_801206F4 = *(s16 *)0x801D21C8;
    D_801206F8 = 0x801B0000;
    D_801206FC = 0x801B2000;
    D_80120700 = 0x801B6000;
    D_80120704 = 0x801BA000;
    D_80120708 = 0x801BE000;
    D_8012070C = 0x801C2000;
    D_80120710 = 0x801C6000;
}

void func_800F7430(void) {
    D_80120714 = 0x801CE080;
    D_80120718 = 0x801CE084;
    D_8012071C = 0x801CE094;
    D_80120720 = *(s16 *)0x801CE0A8;
    D_80120724 = 0x801B0000;
    D_80120728 = 0x801B2000;
    D_8012072C = 0x801B6000;
    D_80120730 = 0x801BA000;
    D_80120734 = 0x801BE000;
    D_80120738 = 0x801C2000;
    D_8012073C = 0x801C6000;
}

void func_800F74E0(void) {
    D_80120740 = 0x801CE0B8;
    D_80120744 = 0x801CE0BC;
    D_80120748 = 0x801CE0D4;
    D_8012074C = *(s16 *)0x801CE0E8;
    D_80120750 = 0x801B0000;
    D_80120754 = 0x801B2000;
    D_80120758 = 0x801B6000;
    D_8012075C = 0x801BA000;
    D_80120760 = 0x801BE000;
    D_80120764 = 0x801C2000;
    D_80120768 = 0x801C6000;
}

void func_800F7590(void) {
    D_8012076C = 0x801D2240;
    D_80120770 = 0x801D2248;
    D_80120774 = 0x801D2298;
    D_80120778 = *(s16 *)0x801D22B0;
    D_8012077C = 0x801B0000;
    D_80120780 = 0x801B2000;
    D_80120784 = 0x801B6000;
    D_80120788 = 0x801BA000;
    D_8012078C = 0x801BE000;
    D_80120790 = 0x801C2000;
    D_80120794 = 0x801C6000;
}

void func_800F7640(void) {
    D_80120798 = 0x801D2074;
    D_8012079C = 0x801D2078;
    D_801207A0 = 0x801D2088;
    D_801207A4 = *(s16 *)0x801D6094;
    D_801207A8 = 0x801D0000;
}

void func_800F7690(void) {
    switch (D_800EECB0) {
    case 1:
        func_800F7740();
        break;
    case 2:
        func_800F84E0();
        break;
    case 3:
        func_800F86C4();
        break;
    case 4:
        func_800F8B50();
        break;
    case 5:
        func_800F93FC();
        break;
    case 6:
        func_800F9558();
        break;
    default:
        func_80015FE0();
        break;
    }
}

void func_800F7740(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_800F77A0();
        return;
    }
    func_80015FE0();
}

void func_800F77A0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl52 tbl;

    tbl = D_801207B8;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    D_800EAFA2 = 1;
}

void func_800F7824(void) {
    func_80015D28(0x3D, 0x801B0000, 0x7A26);
    func_800F7220();
    func_80011DFC();
}

void func_800F785C(void) {
    func_8004BC20(D_800B1746);
    func_80011DFC();
}

void func_800F788C(void) {
    D_800B1746 = 8;
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
    func_80078970(D_80094764, "先生");
    func_8004CDDC();
    func_800F6000();
    D_80120678 = D_80120528;
    D_8012067C = D_80120540;
    D_80120680 = D_80120558;
    func_80078970(D_80094784, "廊下");
    func_80078970(D_800947C4, "職員室");
    D_800B0A04[8].unk_06 += 1;
    func_8004C250();
    func_80011DFC();
}

void func_800F79EC(void) {
    func_800F7220();
    func_80012D64(D_801206A0, 0x11, 1, 2, 0);
    func_8004C46C(D_801206A4, D_801206A8, D_801206AC, D_801206B0, D_801206B4, D_801206B8);
    func_8004C6B0(D_80120694, D_80120698, D_80120690, (s32) D_8012069C);
    D_800EAFB6 = 4;
    D_800EECEC = 1;
    func_80011DFC();
}

void func_800F7AA8(void) {
    D_80120678 = D_8012052C;
    D_8012067C = D_80120544;
    D_80120680 = D_8012055C;
    D_800B0A04[8].unk_06 += 3;
    D_800B0A04[8].unk_0A -= 0xA;
    func_8004C250();
    func_800F72D0();
    func_80012D64(D_801206CC, 0x11, 1, 2, 0);
    func_8004C46C(D_801206D0, D_801206D4, D_801206D8, D_801206DC, D_801206E0, D_801206E4);
    func_8004C6B0(D_801206C0, D_801206C4, D_801206BC, D_801206C8);
    func_800F7640();
    func_80012D64(D_801207A8, 0x12, 1, 3, 0);
    func_800189C4(0x62);
    D_800EB029 = 0xC;
    D_800EB02A = 0x40;
    D_800EB060 = 0x41000000;
    D_800EB02B = 4;
    D_800EB034 = D_8012079C;
    D_800EB05C = D_80120798;
    D_800EB038 = D_801207A0;
    D_800EB03C = D_801207A4;
    D_800EB040 = 0;
    D_800EB02E = 1;
    D_800EB06B = 0x12;
    D_800EB04E = -0xA0;
    D_800EB052 = -0x78;
    D_800EB02F = 0;
    D_800EB02D = 0;
    D_801207AC = 0;
    func_80011DFC();
}

typedef struct {
    s32 w[17];
} Words17; /* size 0x44 */

typedef struct {
    s32 w[17];
} Words17; /* size 0x44 */

typedef struct {
    s32 w[17];
} Words17; /* size 0x44 */

void func_800F7C94(void) {
    /* FAKE: one unused word above tmp gives the original's local offset (0x30); the real type of the buffer is unknown. T-9310 */
    s32 pad;
    Words17 tmp;

    D_80120678 = D_80120530;
    D_8012067C = D_80120548;
    D_80120680 = D_80120560;
    D_800B0A04[8].unk_02 += 3;
    D_800B0A04[8].unk_06 += 2;
    D_800B0A04[8].unk_0A -= 0x14;
    func_8004C250();
    D_800B1C70 = -1;
    D_800B1CF0 = -1;
    func_800F7380();
    func_80012D64(D_801206F8, 0x11, 1, 2, 0);
    func_80012D64(D_801206F8, 0x12, 1, 2, 4);
    func_80012D64(D_801206F8, 0x13, 1, 2, 0);
    func_8004C46C(D_801206FC, D_80120700, D_80120704, D_80120708, D_8012070C, D_80120710);
    func_8004C6B0(D_801206EC, D_801206F0, D_801206E8, D_801206F4);
    tmp = *(Words17 *)D_800EAFA0;
    *(Words17 *)&D_800EAFA0[0xCC] = tmp;
    *(Words17 *)&D_800EAFA0[0x88] = tmp;
    D_800EB06E = 0x40;
    D_800EB02A = 0x40;
    D_800EB040 = 8;
    D_800EB029 = 0xC;
    D_800EB06B = 0x13;
    D_800EB084 = 9;
    D_800EB06D = 0xC;
    D_800EB0AF = 0x12;
    D_801207B0 = 0;
    D_801207AC = 0;
    D_801207B4 = 1;
    func_80011DFC();
}

void func_800F7F44(void) {
    D_801207AC = 0;
    *(s16 *)&D_801207B0 = 3;
    D_80120678 = D_80120534;
    D_8012067C = D_8012054C;
    D_80120680 = D_80120564;
    D_800B0A04[8].unk_02 += 1;
    D_800B0A04[8].unk_06 += 1;
    D_800B0A04[8].unk_0A -= 0x14;
    func_8004C250();
    func_800F7430();
    func_80012D64(D_80120724, 0x11, 1, 2, 0);
    func_8004C46C(D_80120728, D_8012072C, D_80120730, D_80120734, D_80120738, D_8012073C);
    func_8004C6B0(D_80120718, D_8012071C, D_80120714, D_80120720);
    func_80011DFC();
}

void func_800F8070(void) {
    D_80120678 = D_80120538;
    D_8012067C = D_80120550;
    D_80120680 = D_80120568;
    D_800B0A04[8].unk_02 += 1;
    D_800B0A04[8].unk_06 += 1;
    D_800B0A04[8].unk_0A -= 0x14;
    func_8004C250();
    func_800F74E0();
    func_80012D64(D_80120750, 0x11, 1, 2, 0);
    func_8004C46C(D_80120754, D_80120758, D_8012075C, D_80120760, D_80120764, D_80120768);
    func_8004C6B0(D_80120744, D_80120748, D_80120740, D_8012074C);
    func_80011DFC();
}

void func_800F8188(void) {
    D_80120678 = D_8012053C;
    D_8012067C = D_80120554;
    D_80120680 = D_8012056C;
    func_800F7590();
    func_80012D64(D_8012077C, 0x11, 1, 2, 0);
    func_8004C46C(D_80120780, D_80120784, D_80120788, D_8012078C, D_80120790, D_80120794);
    func_8004C6B0(D_80120770, D_80120774, D_8012076C, D_80120778);
    D_800EAFA0[4] = D_800EAFA0[0x48] = 8;
    D_800B0A04[8].unk_02 += 1;
    D_800B0A04[8].unk_06 += 1;
    D_800B0A04[8].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void func_800F82B0(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}

void func_800F82D8(void) {
    func_800469F4(0x4028);
    func_80011DFC();
}

void func_800F8300(void) {
    func_800469F4(0x4646);
    func_80011DFC();
}

void func_800F8328(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}

void func_800F8350(void) {
    func_800469F4(0x4795);
    func_80011DFC();
}

void func_800F8378(void) {
    func_800469F4(0x444F);
    func_80011DFC();
}

void func_800F83A0(void) {
    func_800469F4(0x4755);
    func_80011DFC();
}

void func_800F83C8(void) {
    func_800469F4(0x42E8);
    func_80011DFC();
}

void func_800F83F0(void) {
    func_800469F4(0x4782);
    func_80011DFC();
}

void func_800F8418(void) {
    func_800469F4(0x43ED);
    func_80011DFC();
}

void func_800F8440(void) {
    func_800469F4(0x43E3);
    func_80011DFC();
}

void func_800F8468(void) {
    func_800469F4(0x47EA);
    func_80011DFC();
}

void func_800F8490(void) {
    func_800469F4(0x44E5);
    func_80011DFC();
}

void func_800F84B8(void) {
    func_800469F4(0x47C7);
    func_80011DFC();
}

void func_800F84E0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl41 tbl;

    tbl = D_80120888;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    if (D_800EECE0 != 0) {
        func_800F8590();
    }
}

s32 func_800F8590(void) {
    D_800EB02F = D_800EAFA7;
    if (D_800EAFA7 < 8) {
        D_800EB02B &= 0x7F;
    } else {
        D_800EB02B |= 0x80;
    }
    if (D_800B1AE4 % 13 == 0) {
        D_801207AC += 1;
        if (D_801207AC >= 4) {
            D_801207AC = 0;
        }
        func_80012D64(D_801207A8, 0x12, 1, 3, D_801207AC);
    }
}

void func_800F8654(void) {
    func_80015D28(0x3D, 0x801B0000, 0x7A63);
    func_800F72D0();
    func_80011DFC();
}

void func_800F868C(void) {
    D_80094714 = (u16) D_80094714 + D_800EECBC;
    func_80011DFC();
}

void func_800F86C4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl35 tbl;

    tbl = D_8012092C;
    func_80078950("%d %d\n", D_800B1AF6, D_800EECD0);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    func_800F8878();
    if (D_800B1AF6 == 0xE) {
        func_800F8970();
    }
}

void func_800F8784(void) {
    func_80015D28(0x45, 0x801B0000, 0x7AA0);
    func_800F7380();
    func_80011DFC();
}

void func_800F87BC(void) {
    func_8004C90C();
    D_800EAFA0[0x8B] |= 0x80;
    D_800EAFA0[0xCF] |= 0x80;
    D_800EAFA0[0x8F] = D_800EAFA0[7];
    D_800EAFA0[0xD3] = D_800EAFA0[7];
}

void func_800F8818(void) {
    func_8004C984();
    if (D_800EAFA0[7] == 0) {
        D_800EAFA0[0x8B] &= 0xFF7F;
        D_800EAFA0[0xCF] &= 0xFF7F;
    }
    D_800EAFA0[0x8F] = D_800EAFA0[7];
    D_800EAFA0[0xD3] = D_800EAFA0[7];
}

s32 func_800F8878(void) {
    if (D_800B1AE4 % 10 == 0) {
        if (D_800B1AF6 >= 0xD) {
            D_801207AC += 1;
            if (D_801207AC >= 4) {
                D_801207AC = 0;
            }
            D_801207B0 += D_801207B4;
            if (D_801207B0 == 0) {
                D_801207B4 = 1;
            }
            if (D_801207B0 == 3) {
                D_801207B4 = -1;
            }
            func_80012D64(D_801206F8, 0x13, 1, 2, D_801207AC);
            func_80012D64(D_801206F8, 0x12, 1, 2, D_801207B0 + 4);
        }
    }
}

void func_800F8970(void) {
    func_800F9730(-0x96, -0x78, -0x9B, -0x78, -0x50, 0x28, -0x69, 0x28, 0xFFFF00, 0xFFFF00, 9, 1);
    func_800F9730(-0xA0, -0x78, -0xA5, -0x78, -0x64, 0x28, -0x73, 0x28, 0xFFFFFF, 0xFFFFFF, 9, 1);
    func_800F9730(-0xA2, -0x78, -0xA7, -0x78, -0x6E, 0x28, -0x87, 0x28, 0xFFFF00, 0xFFFF00, 9, 1);
    func_8001A0CC(0, 0, 9, 3, 0);
    func_800F9730(-0x7D, -0x78, -0x82, -0x78, -0x28, 0x28, -0x32, 0x28, 0xFFFF00, 0xFFFF00, 0xD, 1);
    func_800F9730(-0x84, -0x78, -0x89, -0x78, -0x3C, 0x28, -0x46, 0x28, 0xFFFFFF, 0xFFFFFF, 0xD, 1);
    func_8001A0CC(0, 0, 0xD, 3, 0);
}

void func_800F8B50(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl47 tbl;

    tbl = D_801209B8;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    if (D_800B1AF6 == 0xD) {
        func_800F8C2C();
    }
}

void func_800F8BF4(void) {
    func_80015D28(0x3D, 0x801B0000, 0x7AE5);
    func_800F7430();
    func_80011DFC();
}

void func_800F8C2C(void) {
    s32 pad[1]; /* FAKE: one unused word above tbl gives the original local offset (0x54); real source unknown. T-9310 */
    EvTbl tbl;

    tbl = D_80120A74;
    if (D_800B1AE4 % 120 == 0) {
        D_801207AC += 1;
    }
    if (D_801207AC >= 3) {
        D_801207AC = 0;
    }
    if (D_800B1AE4 % 100 == 0) {
        D_801207B0 += 1;
    }
    if (D_801207B0 >= 4) {
        D_801207B0 = 0;
    }
    if (D_800B1AE4 % 36 < 0xC) {
        func_800F9730(0xA0, -0xA0, 0xB4, -0xA0, 0x6E, 0x28, 0x82, 0x28, tbl.e[D_801207B0].a, tbl.e[D_801207B0].b, 9, 1);
        func_800F9730(0xA5, -0xA0, 0xC3, -0xA0, 0x64, 0x28, 0x46, 0x28, tbl.e[D_801207B0].a, tbl.e[D_801207B0].b, 9, 1);
        func_800F9730(0xAA, -0xA0, 0xB4, -0xA0, 0x41, 0x28, 0x32, 0x28, tbl.e[D_801207B0].a, tbl.e[D_801207B0].b, 9, 1);
        func_800F9730(0xA0, -0xA0, 0xAF, -0xA0, 0x69, 0x28, 0x7D, 0x28, tbl.e[D_801207B0].a, tbl.e[D_801207B0].b, 9, 1);
    } else if (D_800B1AE4 % 36 < 0x18) {
        func_800F9730(0x9B, -0xA0, 0xAF, -0xA0, 0x69, 0x28, 0x82, 0x28, tbl.e[D_801207B0].a, tbl.e[D_801207B0].b, 9, 1);
        func_800F9730(0x9B, -0xA0, 0xC3, -0x96, 0x5A, 0x28, 0x3C, 0x28, tbl.e[D_801207B0].a, tbl.e[D_801207B0].b, 9, 1);
        func_800F9730(0xA5, -0xA0, 0xAF, -0xA0, 0x3C, 0x28, 0x2D, 0x28, tbl.e[D_801207B0].a, tbl.e[D_801207B0].b, 9, 1);
        func_800F9730(0x96, -0xA0, 0xA0, -0xA0, 0x64, 0x28, 0x73, 0x28, tbl.e[D_801207B0].a, tbl.e[D_801207B0].b, 9, 1);
    }
    func_8001A0CC(0, 0, 9, 3, 0);
    if (D_800B1AE4 % 30 < 0xA) {
        func_800F9730(0x1E, -0xA0, 0x64, -0xA0, -0x140, 0x28, -0xFA, 0x28, tbl.e[D_801207AC].a, tbl.e[D_801207AC].b, 0xD, 1);
        func_800F9730(0xA, -0xA0, 0x19, -0xA0, -0xDC, 0x28, -0xBE, 0x28, tbl.e[D_801207AC].a, tbl.e[D_801207AC].b, 0xD, 1);
        func_800F9730(0, -0xA0, 0x14, -0xA0, -0xB4, 0x28, -0x82, 0x28, tbl.e[D_801207AC].a, tbl.e[D_801207AC].b, 0xD, 1);
        func_800F9730(0x19, -0xA0, 0x28, -0xA0, -0x12C, 0x28, -0x118, 0x28, tbl.e[D_801207AC].a, tbl.e[D_801207AC].b, 0xD, 1);
    } else if (D_800B1AE4 % 30 < 0x14) {
        func_800F9730(0x14, -0xA0, 0x5A, -0xA0, -0x154, 0x28, -0x10E, 0x28, tbl.e[D_801207AC].a, tbl.e[D_801207AC].b, 0xD, 1);
        func_800F9730(0xA, -0xA0, 0x1E, -0xA0, -0xF0, 0x28, -0xD2, 0x28, tbl.e[D_801207AC].a, tbl.e[D_801207AC].b, 0xD, 1);
        func_800F9730(0, -0xA0, 0xF, -0xA0, -0xC8, 0x28, -0x96, 0x28, tbl.e[D_801207AC].a, tbl.e[D_801207AC].b, 0xD, 1);
        func_800F9730(0xF, -0xA0, 0x19, -0xA0, -0x136, 0x28, -0x122, 0x28, tbl.e[D_801207AC].a, tbl.e[D_801207AC].b, 0xD, 1);
    }
    func_8001A0CC(0, 0, 0xD, 3, 0);
}

void func_800F93FC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl42 tbl;

    tbl = D_80120A94;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_800F9470(void) {
    func_800433D0(0x503);
    func_80011DFC();
}

void func_800F9498(void) {
    func_80015D28(0xD, 0x801D0000, 0x8D5B);
    func_800F74E0();
    func_80011DFC();
}

void func_800F94D0(void) {
    func_80015D28(0x3D, 0x801B0000, 0x7B22);
    func_800F74E0();
    func_80011DFC();
}

void func_800F9508(void) {
    /* the u8 cast reproduces the conversion of the callee's u8 result (andi) that shifts the temporary registers */
    if (((u8)func_8002328C(D_800B1746) & 0x7F) >= 2U) {
        D_80094714 += 3;
    }
    func_80011DFC();
}

void func_800F9558(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl42 tbl;

    tbl = D_80120B3C;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    if ((D_800EECE0 != 0) && (D_800B1AE4 % 180 == 0)) {
        func_800433D0(0x501);
    }
    if ((D_800EECE0 != 0) && (D_800B1AE4 % 210 == 10)) {
        func_800433D0(0x502);
    }
    if ((D_800EECE0 != 0) && (D_800B1AE4 % 450 == 20)) {
        func_800433D0(0x500);
    }
}
