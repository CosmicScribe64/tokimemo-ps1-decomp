#include "common.h"
#include "ovl/EVENT.h"

void func_801050D0(void) {
    D_80122F70 = 0x801D22F4;
    D_80122F74 = 0x801D22F8;
    D_80122F78 = 0x801D2318;
    D_80122F7C = *(s16 *)0x801D232C;
    D_80122F80 = 0x801B0000;
    D_80120C04 = 0x801D2000;
    D_80122F84 = 0x801B2000;
    D_80122F88 = 0x801B6000;
    D_80122F8C = 0x801BA000;
    D_80122F90 = 0x801BE000;
    D_80122F94 = 0x801C2000;
    D_80122F98 = 0x801C6000;
    D_80120C20 = 0x801CE000;
}

void func_801051A0(void) {
    D_80122F9C = 0x801CE090;
    D_80122FA0 = 0x801CE094;
    D_80122FA4 = 0x801CE0AC;
    D_80122FA8 = *(s16 *)0x801CE0C0;
    D_80122FAC = 0x801B0000;
    D_80122FB0 = 0x801B2000;
    D_80122FB4 = 0x801B6000;
    D_80122FB8 = 0x801BA000;
    D_80122FBC = 0x801BE000;
    D_80122FC0 = 0x801C2000;
    D_80122FC4 = 0x801C6000;
}

void func_80105250(void) {
    D_80122FC8 = 0x801D2240;
    D_80122FCC = 0x801D2248;
    D_80122FD0 = 0x801D2288;
    D_80122FD4 = *(s16 *)0x801D22A0;
    D_80122FD8 = 0x801B0000;
    D_80122FDC = 0x801B2000;
    D_80122FE0 = 0x801B6000;
    D_80122FE4 = 0x801BA000;
    D_80122FE8 = 0x801BE000;
    D_80122FEC = 0x801C2000;
    D_80122FF0 = 0x801C6000;
}

void func_80105300(void) {
    D_80122FF4 = 0x801CE078;
    D_80122FF8 = 0x801CE07C;
    D_80122FFC = 0x801CE08C;
    D_80123000 = *(s16 *)0x801CE098;
    D_80123004 = 0x801B0000;
    D_80123008 = 0x801B2000;
    D_8012300C = 0x801B6000;
    D_80123010 = 0x801BA000;
    D_80123014 = 0x801BE000;
    D_80123018 = 0x801C2000;
    D_8012301C = 0x801C6000;
}

void func_801053B0(void) {
    D_80123020 = 0x801CE0F4;
    D_80123024 = 0x801CE0F8;
    D_80123028 = 0x801CE118;
    D_8012302C = *(s16 *)0x801CE12C;
    D_80123030 = 0x801B0000;
    D_80123034 = 0x801B2000;
    D_80123038 = 0x801B6000;
    D_8012303C = 0x801BA000;
    D_80123040 = 0x801BE000;
    D_80123044 = 0x801C2000;
    D_80123048 = 0x801C6000;
}

void func_80105460(void) {
    switch (D_800EECB0) {
    case 1:
        func_80105510();
        return;
    case 2:
        func_80105570();
        return;
    case 3:
        func_80106834();
        return;
    case 4:
        func_80106908();
        return;
    case 5:
        func_80106A8C();
        return;
    case 6:
        func_80106B48();
        return;
    default:
        func_80015FE0();
        return;
    }
}

void func_80105510(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_801055D0();
        return;
    }
    func_80015FE0();
}

void func_80105570(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_80105910();
        return;
    }
    func_80015FE0();
}

void func_801055D0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl45 tbl;

    tbl = D_8012304C;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    func_80105DF8();
}

void func_8010564C(void) {
    s16 i;
    Rec24 *p;

    D_800B1C1C = 1;
    func_800137AC(0x140, 0, 0x40, 0x80, D_80120C20);
    func_801050D0();
    func_80012D64(D_80120C04, 0x12, 1, 3, 0);
    for (i = 4; i < 0xE; i++) {
        p = (Rec24 *)(D_800EC190 + i * 0x24);
        p->unk_00 = 0xC1000000;
        p->unk_04 = func_800789A0() % 84 - 0x9F;
        p->unk_06 = func_800789A0() % 130 - 0x5F;
        p->unk_08 = 8;
        p->unk_0A = 0x18;
        p->unk_0C = 5;
        p->unk_0E = 0;
        p->unk_0F = 0;
        p->unk_10 = 0;
        p->unk_12 = 0x1F2;
        p->unk_14 = 0;
        p->unk_15 = 0;
        p->unk_16 = 0;
        p->unk_18 = 0;
        p->unk_1A = 0;
        p->unk_1C = 0x1000;
        p->unk_1E = 0x1000;
        p->unk_20 = 0;
        D_800B188D[i] = 0x8C;
        ((s16 *)&D_80120CE8)[i] = 0;
        D_80120D08[i] = 0;
    }
    func_80011DFC();
}

void func_80105830(void) {
    func_80015D28(0x45, 0x801B0000, 0x86E8);
    func_801050D0();
    func_80011DFC();
}

void func_80105868(void) {
    func_801050D0();
    func_80012D64(D_80122F80, 0x11, 1, 2, 0);
    func_8004C46C(D_80122F84, D_80122F88, D_80122F8C, D_80122F90, D_80122F94, D_80122F98);
    func_8004C6B0(D_80122F74, D_80122F78, D_80122F70, (s32) D_80122F7C);
    func_80011DFC();
}

void func_80105910(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80123100;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_80105998(void) {
    if (D_800B0A04[D_800B1746].unk_06 < 0x50) {
        D_80094714 += 2;
    }
    func_80011DFC();
}

void func_801059FC(void) {
    if (D_800B0A04[D_800B1746].unk_06 < 0x50) {
        D_800B1AF6 += 0xE;
    }
    func_80011DFC();
}

void func_80105A60(void) {
    if (D_800B0A04[D_800B1746].unk_06 >= 0x50) {
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
    }
    func_80011DFC();
}

void func_80105AC8(void) {
    D_80094714 = (u16) D_80094714 + D_800EECBC;
    func_80011DFC();
}

void func_80105B00(void) {
    s16 i;
    Rec24 *records;
    s32 before;
    s32 after;

    before = D_800B1AF6;
    func_8004C984();
    records = (Rec24 *)D_800EC190;
    for (i = 4; i < 19; i++) {
        if (D_800F19AB < 112) {
            records[i].unk_14 = records[i].unk_15 = records[i].unk_16 = D_800F19AB;
        }
    }
    after = D_800B1AF6;
    if (before != after) {
        for (i = 4; i < 19; i++) {
            records[i].unk_00 |= 0x80000000;
        }
    }
}

void func_80105BC4(void) {
    s16 i;
    Rec24 *records;

    if (D_800B1AF0 == 0) {
        records = (Rec24 *)D_800EC190;
        for (i = 4; i < 19; i++) {
            records[i].unk_00 &= 0x7FFFFFFF;
        }
    }
    func_8004C90C();
    records = (Rec24 *)D_800EC190;
    for (i = 4; i < 19; i++) {
        if (D_800F19AB < 113) {
            records[i].unk_14 = records[i].unk_15 = records[i].unk_16 = D_800F19AB;
        }
    }
}

void func_80105C8C(void) {
    D_800B1746 = 9;
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
    D_80120678 = D_80120570;
    D_8012067C = D_8012058C;
    D_80120680 = D_801205A8;
    func_80078970(D_80094784, "廊下");
    if (D_800EECB0 == 1) {
        if (D_800B0A04[9].unk_06 >= 0x50) {
            D_800B0A04[9].unk_02 += 1;
        } else {
            D_800B0A04[9].unk_06 += 1;
        }
        func_8004C250();
    }
    func_80011DFC();
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_80120CF0;
extern s32 D_80120D10;
/* end bulk_m2c declarations */
void func_80105DF8(void) {
    void *sp54;
    void *sp4C;
    s16 temp_a1;
    s16 temp_a1_2;
    s16 temp_v0;
    s16 temp_v0_2;
    s16 temp_v0_4;
    s16 temp_v0_5;
    s16 temp_v0_6;
    s16 var_s1;
    s16 var_t5;
    s16 var_v1;
    s32 temp_hi;
    s32 temp_hi_2;
    s32 temp_v0_3;
    s32 var_at;
    s32 var_at_2;
    s32 var_at_3;
    s32 var_t6;
    s32 var_v0;
    u8 *temp_s0;
    u8 *var_a0;
    u8 *var_a2;

    var_s1 = 4;
    var_t6 = 4 * 8;
    do {
        temp_s0 = ((var_t6 + var_s1) * 4) + D_800EC190;
        *(s16 *)((unsigned char *)temp_s0 + (6)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (6)) + 0xA);
        var_v0 = var_s1 * 2;
        if (!(func_800789A0() & 3)) {
            temp_a1 = *(s16 *)((unsigned char *)temp_s0 + (6));
            if (temp_a1 >= -0x52) {
                var_at = temp_a1 < -0x39;
                if (temp_a1 < -0x39) {
                    if (*(s16 *)((unsigned char *)temp_s0 + (4)) >= -0x57) {
                        goto block_5;
                    }
                    goto block_18;
                }
                goto block_6;
            }
block_5:
            var_at = temp_a1 < -0x39;
block_6:
            if (var_at == 0) {
                var_at_2 = temp_a1 < -0xB;
                if (temp_a1 < -0x1A) {
                    if (*(s16 *)((unsigned char *)temp_s0 + (4)) >= -0x77) {
                        goto block_9;
                    }
                    goto block_18;
                }
                goto block_10;
            }
block_9:
            var_at_2 = temp_a1 < -0xB;
block_10:
            if (var_at_2 == 0) {
                var_at_3 = temp_a1 < 5;
                if (temp_a1 < 3) {
                    temp_v0 = *(s16 *)((unsigned char *)temp_s0 + (4));
                    if ((temp_v0 < -0x8D) || (temp_v0 >= -0x6F)) {
                        goto block_14;
                    }
                    goto block_18;
                }
                goto block_15;
            }
block_14:
            var_at_3 = temp_a1 < 5;
block_15:
            var_v0 = var_s1 * 2;
            if (var_at_3 == 0) {
                temp_v0_2 = *(s16 *)((unsigned char *)temp_s0 + (4));
                if ((temp_v0_2 >= -0x7F) && (temp_v0_2 < -0x5C)) {
block_18:
                    temp_v0_3 = var_s1 * 2;
                    var_a2 = &D_80120CF0 + temp_v0_3;
                    var_v1 = *(s16 *)((unsigned char *)var_a2 + (-8));
                    if ((var_v1 != 0) || (var_a0 = temp_v0_3 + &D_80120D10, (*(s16 *)((unsigned char *)var_a0 + (-8)) != 0))) {
                        goto block_20;
                    }
                    goto block_23;
                }
block_20:
                var_v0 = var_s1 * 2;
                goto block_21;
            }
            goto block_21;
        }
block_21:
        var_a2 = &D_80120CF0 + var_v0;
        var_v1 = *(s16 *)((unsigned char *)var_a2 + (-8));
        temp_a1_2 = *(s16 *)((unsigned char *)temp_s0 + (6));
        var_a0 = var_v0 + &D_80120D10;
        if ((var_v1 != 0) || (temp_v0_4 = *(s16 *)((unsigned char *)var_a0 + (-8)), (temp_v0_4 != 0))) {
block_23:
            *(s16 *)((unsigned char *)temp_s0 + (6)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (6)) - 0xA);
            if (var_v1 == 0) {
                temp_v0_5 = *(s16 *)((unsigned char *)var_a0 + (-8));
                if (temp_v0_5 == 0) {
                    if (*(s16 *)((unsigned char *)temp_s0 + (6)) < -0x44) {
                        *(u8 *)((unsigned char *)temp_s0 + (15)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (15)) + 8);
                    }
                    *(s16 *)((unsigned char *)temp_s0 + (4)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (4)) - 0xC);
                    *(s16 *)((unsigned char *)temp_s0 + (6)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (6)) + 0x10);
                    *(u8 *)((unsigned char *)temp_s0 + (14)) = 8U;
                    *(s16 *)((unsigned char *)temp_s0 + (8)) = 0x18;
                    *(s16 *)((unsigned char *)temp_s0 + (10)) = 8;
                    *(s16 *)((unsigned char *)var_a0 + (-8)) = (s16) (temp_v0_5 + 1);
                } else {
                    *(s16 *)((unsigned char *)var_a0 + (-8)) = 0;
                    *(s16 *)((unsigned char *)var_a2 + (-8)) = (s16) (var_v1 + 1);
                }
            } else {
                if (var_v1 == 4) {
                    *(u8 *)((unsigned char *)temp_s0 + (14)) = 0U;
                    *(u8 *)((unsigned char *)temp_s0 + (15)) = 0U;
                    *(s16 *)((unsigned char *)temp_s0 + (8)) = 8;
                    *(s16 *)((unsigned char *)temp_s0 + (10)) = 0x18;
                    sp4C = var_a0;
                    sp54 = var_a2;
                    temp_hi = func_800789A0(var_a0, *(s16 *)((unsigned char *)temp_s0 + (6)), var_a2) % 84;
                    *(s16 *)((unsigned char *)temp_s0 + (6)) = -0x78;
                    *(s16 *)((unsigned char *)var_a0 + (-8)) = 0;
                    *(s16 *)((unsigned char *)var_a2 + (-8)) = 0;
                    var_t5 = temp_hi - 0x9F;
                    goto block_42;
                }
                temp_v0_6 = *(s16 *)((unsigned char *)var_a0 + (-8));
                if (var_v1 == 5) {
                    if (temp_v0_6 == 0) {
                        *(s16 *)((unsigned char *)var_a0 + (-8)) = (s16) (temp_v0_6 + 1);
                        *(u8 *)((unsigned char *)temp_s0 + (14)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (14)) - 0x18);
                    } else {
                        *(s16 *)((unsigned char *)var_a0 + (-8)) = 0;
                        *(s16 *)((unsigned char *)var_a2 + (-8)) = (s16) (var_v1 + 1);
                    }
                } else if (temp_v0_6 == 0) {
                    *(s16 *)((unsigned char *)var_a0 + (-8)) = (s16) (temp_v0_6 + 1);
                    *(u8 *)((unsigned char *)temp_s0 + (14)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (14)) + 0x18);
                } else {
                    *(s16 *)((unsigned char *)var_a0 + (-8)) = 0;
                    *(s16 *)((unsigned char *)var_a2 + (-8)) = (s16) (var_v1 + 1);
                }
            }
        } else if ((temp_a1_2 >= 0x19) && (var_v1 == 0) && (temp_v0_4 == 0)) {
            temp_hi_2 = func_800789A0(var_a0, temp_a1_2, var_a2) % 84;
            *(s16 *)((unsigned char *)temp_s0 + (6)) = -0x78;
            var_t5 = temp_hi_2 - 0x9F;
block_42:
            *(s16 *)((unsigned char *)temp_s0 + (4)) = var_t5;
        }
        var_s1 += 1;
        var_t6 = var_s1 * 8;
    } while (var_s1 < 0xE);
}

void func_80106150(void) {
    D_800B1746 = 9;
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
    D_80120678 = D_80120574;
    D_8012067C = D_80120590;
    D_80120680 = D_801205AC;
    func_80011DFC();
}

void func_80106258(void) {
    D_80120678 = D_80120578;
    D_8012067C = D_80120594;
    D_80120680 = D_801205B0;
    func_80078970(D_80094784, "遊園地");
    D_800B0A04[9].unk_02 += 1;
    D_800B0A04[9].unk_06 += 1;
    D_800B0A04[9].unk_0A -= 0x14;
    func_8004C250();
    func_801051A0();
    func_80012D64(D_80122FAC, 0x11, 1, 2, 0);
    func_8004C46C(D_80122FB0, D_80122FB4, D_80122FB8, D_80122FBC, D_80122FC0, D_80122FC4);
    func_8004C6B0(D_80122FA0, D_80122FA4, D_80122F9C, D_80122FA8);
    func_80011DFC();
}

void func_80106384(void) {
    D_80120678 = D_8012057C;
    D_8012067C = D_80120598;
    D_80120680 = D_801205B4;
    D_800B0A04[9].unk_02 += 2;
    D_800B0A04[9].unk_06 += 1;
    D_800B0A04[9].unk_0A -= 0x14;
    func_8004C250();
    func_80105250();
    func_80012D64(D_80122FD8, 0x11, 1, 2, 0);
    func_8004C46C(D_80122FDC, D_80122FE0, D_80122FE4, D_80122FE8, D_80122FEC, D_80122FF0);
    func_8004C6B0(D_80122FCC, D_80122FD0, D_80122FC8, D_80122FD4);
    func_80011DFC();
}

void func_8010649C(void) {
    D_80120678 = D_80120580;
    D_8012067C = D_8012059C;
    D_80120680 = D_801205B8;
    D_800B0A04[9].unk_02 += 1;
    D_800B0A04[9].unk_06 += 1;
    D_800B0A04[9].unk_0A -= 0x14;
    func_8004C250();
    func_80105300();
    func_80012D64(D_80123004, 0x11, 1, 2, 0);
    func_8004C46C(D_80123008, D_8012300C, D_80123010, D_80123014, D_80123018, D_8012301C);
    func_8004C6B0(D_80122FF8, D_80122FFC, D_80122FF4, D_80123000);
    func_80011DFC();
}

void func_801065B4(void) {
    D_80120678 = D_80120584;
    D_8012067C = D_801205A0;
    D_80120680 = D_801205BC;
    D_800B0A04[9].unk_02 += 3;
    D_800B0A04[9].unk_06 += 2;
    D_800B0A04[9].unk_0A -= 0x14;
    func_8004C250();
    func_801053B0();
    func_80012D64(D_80123030, 0x11, 1, 2, 0);
    func_8004C46C(D_80123034, D_80123038, D_8012303C, D_80123040, D_80123044, D_80123048);
    func_8004C6B0(D_80123024, D_80123028, D_80123020, D_8012302C);
    func_80011DFC();
}

void func_801066CC(void) {
    func_800469F4(0x3FDE);
    func_80011DFC();
}

void func_801066F4(void) {
    func_800469F4(0x4622);
    func_80011DFC();
}

void func_8010671C(void) {
    func_800469F4(0x3FDE);
    func_80011DFC();
}

void func_80106744(void) {
    func_800469F4(0x475E);
    func_80011DFC();
}

void func_8010676C(void) {
    func_800469F4(0x42A8);
    func_80011DFC();
}

void func_80106794(void) {
    func_800469F4(0x4737);
    func_80011DFC();
}

void func_801067BC(void) {
    func_800469F4(0x47F3);
    func_80011DFC();
}

void func_801067E4(void) {
    func_800469F4(0x44F6);
    func_80011DFC();
}

void func_8010680C(void) {
    func_800469F4(0x44E5);
    func_80011DFC();
}

void func_80106834(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl47 tbl;

    tbl = D_80123150;
    func_80078950("%d %d\n", D_800B1AF6, D_800EECD0);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_801068D0(void) {
    func_80015D28(0x3D, 0x801B0000, 0x872D);
    func_801051A0();
    func_80011DFC();
}

void func_80106908(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl63 tbl;

    tbl = D_8012320C;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_8010698C(void) {
    func_80015D28(0x45, 0x801B0000, 0x876A);
    func_80105250();
    func_80011DFC();
}

void func_801069C4(void) {
    func_800433D0(0x501);
    func_80011DFC();
}

void func_801069EC(void) {
    func_800433D0(0x503);
    func_80011DFC();
}

void func_80106A14(void) {
    func_800433D0(0x504);
    func_80011DFC();
}

void func_80106A3C(void) {
    func_800433D0(0x505);
    func_80011DFC();
}

void func_80106A64(void) {
    func_800433D0(0x506);
    func_80011DFC();
}

void func_80106A8C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl36 tbl;

    tbl = D_80123308;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_80106B10(void) {
    func_80015D28(0x3D, 0x801B0000, 0x87AF);
    func_80105300();
    func_80011DFC();
}

void func_80106B48(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl36 tbl;

    tbl = D_80123398;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_80106BDC(void) {
    func_80015D28(0x3D, 0x801B0000, 0x87EC);
    func_801053B0();
    func_80011DFC();
}
