#define MAIN_API_OVERRIDE_D_80122CD0 /* matched as u32 (main_api.h: s32), switch selector in $v1 (T-6010) */
#include "common.h"
#include "ovl/GEKO.h"

extern u32 D_80122CD0;

typedef struct {
    void (*f[22])();
} FnTbl22; /* size 0x58 */
extern FnTbl22 D_80146F34;

typedef struct {
    void (*f[46])();
} FnTbl46; /* size 0xB8 */
extern FnTbl46 D_80146E7C;

void func_8013DF70(void) {
    D_80146E50 = 0x801D22F4;
    D_80146E54 = 0x801D22F8;
    D_80146E58 = 0x801D2318;
    D_80146E5C = *(s16 *)0x801D232C;
    D_80146E60 = 0x801B0000;
    D_80146394 = 0x801D2000;
    D_80146E64 = 0x801B2000;
    D_80146E68 = 0x801B6000;
    D_80146E6C = 0x801BA000;
    D_80146E70 = 0x801BE000;
    D_80146E74 = 0x801C2000;
    D_80146E78 = 0x801C6000;
    D_801463B0 = 0x801CE000;
}

void func_8013E040(void) {
    switch (D_80122CD0) {
    case 1:
        func_8013E0A4();
        return;
    case 2:
        func_8013E0E0();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_8013E0A4(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013E11C();
        return;
    }
    func_80046500();
}

void func_8013E0E0(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013E4F0();
        return;
    }
    func_80046500();
}

void func_8013E11C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl46 tbl;

    tbl = D_80146E7C;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    func_8013E950();
}

void func_8013E1A0(void) {
    if (func_80052C88(9) >= 0x50U) {
        D_800E6280.unk_1BC[9].unk_02 += 1;
        D_800E6280.unk_1BC[9].unk_06 += 2;
    } else {
        D_800E6280.unk_1BC[9].unk_0C.f.b14 = 1;
        D_800E6280.unk_1BC[9].unk_06 += 1;
    }
    func_80084D3C();
    func_8004284C();
}

void func_8013E22C(void) {
    s16 i;
    Rec24 *p;

    D_800E6280.unk_1228[5] = 1;
    func_8004435C(0x140, 0, 0x40, 0x80, D_801463B0);
    func_8013DF70();
    func_80043914(D_80146394, 0x12, 1, 3, 0);
    for (i = 4; i < 0xE; i++) {
        p = &D_801217D0[i];
        p->unk_00 = 0xC1000000;
        p->unk_04 = func_800AE0D0() % 84 - 0x9F;
        p->unk_06 = func_800AE0D0() % 130 - 0x5F;
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
        D_800E6280.unk_10A5[i] = 0x8C;
        D_801463E0[i - 4] = 0;
        D_80146400[i - 4] = 0;
    }
    func_8004284C();
}

void func_8013E410(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x8A88);
    func_8013DF70();
    func_8004284C();
}

void func_8013E448(void) {
    func_8013DF70();
    func_80043914(D_80146E60, 0x11, 1, 2, 0);
    func_80084E90(D_80146E64, D_80146E68, D_80146E6C, D_80146E70, D_80146E74, D_80146E78);
    func_800850D4(D_80146E54, D_80146E58, D_80146E50, D_80146E5C);
    func_8004284C();
}

void func_8013E4F0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl22 tbl;

    tbl = D_80146F34;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

s32 func_8013E56C(void) {
    func_80138AF8();
    if (D_800E6280.unk_1104.w++ == 1) {
        if ((u16) D_800CA154 == 9) {
            func_80083440(0);
        }
    }
}

void func_8013E5C8(void) {
    if (get_h_yuukou(9) < 0x50U) {
        D_800CA150 = (u16) D_800CA150 + 2;
    }
    func_8004284C();
}

void func_8013E60C(void) {
    if (get_h_yuukou(9) < 0x50U) {
        D_800E6280.unk_110A += 0xE;
    }
    func_8004284C();
}

void func_8013E650(void) {
    if (get_h_yuukou(9) >= 0x50U) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
    }
    func_8004284C();
}

void func_8013E69C(void) {
    s16 i;
    Rec24 *records;
    s32 before;
    s32 after;

    before = D_800E6280.unk_110A;
    func_800853FC();
    records = (Rec24 *)D_801217D0;
    for (i = 4; i < 19; i++) {
        if (D_800B593C < 112) {
            records[i].unk_14 = records[i].unk_15 = records[i].unk_16 = D_800B593C;
        }
    }
    after = D_800E6280.unk_110A;
    if (before != after) {
        for (i = 4; i < 19; i++) {
            records[i].unk_00 |= 0x80000000;
        }
    }
}

void func_8013E760(void) {
    s16 i;
    Rec24 *records;

    if (D_800E6280.unk_1104.w == 0) {
        records = (Rec24 *)D_801217D0;
        for (i = 4; i < 19; i++) {
            records[i].unk_00 &= 0x7FFFFFFF;
        }
    }
    func_80085368();
    records = (Rec24 *)D_801217D0;
    for (i = 4; i < 19; i++) {
        if (D_800B593C < 113) {
            records[i].unk_14 = records[i].unk_15 = records[i].unk_16 = D_800B593C;
        }
    }
}

void func_8013E828(void) {
    D_800E6280.unk_F5F = 9;
    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    func_8006612C("");
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    func_8007C740();
    func_80137990();
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    D_800B593C = 0;
    func_800847B8(D_800E6280.unk_F5F);
    func_8008585C();
    func_80137990();
    D_80146274 = D_80145F98;
    D_80146278 = D_801460D4;
    D_8014627C = D_80146210;
    func_800AE0F0(D_800CA19C, "教室");
    D_800CA368 = 0x3C;
    func_8004284C();
}

void func_8013E950(void) {
    s16 *sp54;
    s16 *sp4C;
    s16 *var_a0;
    s16 *var_a2;
    s16 temp_a1;
    s16 temp_v0;
    s16 temp_v0_2;
    s16 temp_v0_3;
    s16 temp_v0_4;
    s16 temp_v0_5;
    s16 var_s1;
    s16 var_t5;
    s16 var_v1;
    s32 temp_hi;
    s32 temp_hi_2;
    s32 var_at;
    s32 var_at_2;
    s32 var_at_3;
    s32 var_t6;
    s32 var_v0;
    u8 *temp_s0;

    var_s1 = 4;
    var_t6 = 4 * 8;
    do {
        temp_s0 = ((var_t6 + var_s1) * 4) + D_801217D0;
        *(s16 *)((unsigned char *)temp_s0 + (6)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (6)) + 0xA);
        var_v0 = var_s1 * 2;
        if (!(func_800AE0D0() & 3)) {
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
                    var_a2 = &D_801463E0[var_s1];
                    var_v1 = *(s16 *)((unsigned char *)var_a2 + (-8));
                    if ((var_v1 != 0) || (var_a0 = &D_80146400[var_s1], (*(s16 *)((unsigned char *)var_a0 + (-8)) != 0))) {
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
        var_a2 = D_801463E0 + var_v0;
        var_v1 = *(s16 *)((unsigned char *)var_a2 + (-8));
        var_a0 = var_v0 + D_80146400;
        if ((var_v1 != 0) || (temp_v0_3 = *(s16 *)((unsigned char *)var_a0 + (-8)), (temp_v0_3 != 0))) {
block_23:
            *(s16 *)((unsigned char *)temp_s0 + (6)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (6)) - 0xA);
            if (var_v1 == 0) {
                temp_v0_4 = *(s16 *)((unsigned char *)var_a0 + (-8));
                if (temp_v0_4 == 0) {
                    if (*(s16 *)((unsigned char *)temp_s0 + (6)) < -0x44) {
                        *(u8 *)((unsigned char *)temp_s0 + (15)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (15)) + 8);
                    }
                    *(s16 *)((unsigned char *)temp_s0 + (4)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (4)) - 0xC);
                    *(s16 *)((unsigned char *)temp_s0 + (6)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (6)) + 0x10);
                    *(u8 *)((unsigned char *)temp_s0 + (14)) = 8U;
                    *(s16 *)((unsigned char *)temp_s0 + (8)) = 0x18;
                    *(s16 *)((unsigned char *)temp_s0 + (10)) = 8;
                    *(s16 *)((unsigned char *)var_a0 + (-8)) = (s16) (temp_v0_4 + 1);
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
                    temp_hi = func_800AE0D0() % 84;
                    *(s16 *)((unsigned char *)temp_s0 + (6)) = -0x78;
                    *(s16 *)((unsigned char *)var_a0 + (-8)) = 0;
                    *(s16 *)((unsigned char *)var_a2 + (-8)) = 0;
                    var_t5 = temp_hi - 0x9F;
                    goto block_42;
                }
                temp_v0_5 = *(s16 *)((unsigned char *)var_a0 + (-8));
                if (var_v1 == 5) {
                    if (temp_v0_5 == 0) {
                        *(s16 *)((unsigned char *)var_a0 + (-8)) = (s16) (temp_v0_5 + 1);
                        *(u8 *)((unsigned char *)temp_s0 + (14)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (14)) - 0x18);
                    } else {
                        *(s16 *)((unsigned char *)var_a0 + (-8)) = 0;
                        *(s16 *)((unsigned char *)var_a2 + (-8)) = (s16) (var_v1 + 1);
                    }
                } else if (temp_v0_5 == 0) {
                    *(s16 *)((unsigned char *)var_a0 + (-8)) = (s16) (temp_v0_5 + 1);
                    *(u8 *)((unsigned char *)temp_s0 + (14)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (14)) + 0x18);
                } else {
                    *(s16 *)((unsigned char *)var_a0 + (-8)) = 0;
                    *(s16 *)((unsigned char *)var_a2 + (-8)) = (s16) (var_v1 + 1);
                }
            }
        } else if ((*(s16 *)((unsigned char *)temp_s0 + (6)) >= 0x19) && (var_v1 == 0) && (temp_v0_3 == 0)) {
            temp_hi_2 = func_800AE0D0() % 84;
            *(s16 *)((unsigned char *)temp_s0 + (6)) = -0x78;
            var_t5 = temp_hi_2 - 0x9F;
block_42:
            *(s16 *)((unsigned char *)temp_s0 + (4)) = var_t5;
        }
        var_s1 += 1;
        var_t6 = var_s1 * 8;
    } while (var_s1 < 0xE);
}

void func_8013ECA8(void) {
    D_800E6280.unk_F5F = 9;
    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    func_800AE0F0(D_800CA19C, "廊下");
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    func_8007C740();
    func_80137990();
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    D_800B593C = 0;
    func_800847B8(D_800E6280.unk_F5F);
    func_8008585C();
    func_80137990();
    D_80146274 = D_80145F9C;
    D_80146278 = D_801460D8;
    D_8014627C = D_80146214;
    D_800CA368 = 0x3D;
    func_8004284C();
}

void func_8013EDC4(void) {
    bg_read_sub2(0x4068);
    func_8004284C();
}

void func_8013EDEC(void) {
    bg_read_sub2(0x46B2);
    func_8004284C();
}

void func_8013EE14(void) {
    func_8007ED84(0x403B);
    func_8004284C();
}
