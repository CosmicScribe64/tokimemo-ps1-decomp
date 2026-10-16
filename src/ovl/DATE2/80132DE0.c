#include "common.h"
#include "ovl/DATE2.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A4C0);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A4C4);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A4C8);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5B0);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5B4);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5B8);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5BC);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5C0);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5C4);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5C8);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5CC);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5D0);
s16 D_8013A5D4[3] = { 0, 0, 0 };
s16 D_8013A5DC[3] = { 0, 0, 0 };
s16 D_8013A5E4[3] = { 0, 0, 0 };
s16 D_8013A5EC[3] = { 0, 0, 0 };

typedef struct {
    void (*f[44])();
} FnTbl44; /* size 0xB0 */
extern FnTbl44 D_8013A600;

typedef struct {
    void (*f[58])();
} FnTbl58; /* size 0xE8 */
extern FnTbl58 D_8013A4C8;

void func_80132DE0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl58 tbl;

    tbl = D_8013A4C8;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80132E5C(void) {
    func_800AE0A0((void *)(0x80180000 + D_800E6280.unk_1104.w * 0x1400), 0x801C0000 + D_800E6280.unk_1104.w * 0x1400, 0x1400);
    D_800E6280.unk_1104.w += 1;
    if (D_800E6280.unk_1104.w == 0x12) {
        func_8004284C();
    }
}

void func_80132EC8(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    temp_v0 = D_800E6280.unk_1104.w * 0x1400;
    func_800AE0A0(temp_v0 + 0x80180000, temp_v0 + 0x801A0000, 0x1400);
    temp_v0_2 = D_800E6280.unk_1104.w + 1;
    D_800E6280.unk_1104.w = temp_v0_2;
    if (temp_v0_2 == 0x12) {
        D_800E6280.unk_F5F = 0;
        func_80051508(D_800CA17C, 0);
        func_8004284C();
    }
}

void func_80132F48(void) {
    s32 pad; /* FAKE: unused local that moves rect to sp+0x2C like the original (T-3330 frame rule). T-4050 */
    RECT rect;

    rect.x = 0x140;
    rect.y = 0x80;
    rect.w = 0x180;
    rect.h = 0x80;
    func_8009C8E0(&rect, (void *)0x80180000);
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    func_800847B8(D_800E6280.unk_75D);
    func_80133258();
    func_8004284C();
}

void func_80132FB0(void) {
    func_800AE0A0((void *)(0x801C0000 + D_800E6280.unk_1104.w * 0x1400), 0x80180000 + D_800E6280.unk_1104.w * 0x1400, 0x1400);
    D_800E6280.unk_1104.w += 1;
    if (D_800E6280.unk_1104.w == 0x12) {
        func_80083474();
    }
}

void func_8013301C(void) {
    func_800AE0A0((void *)(0x801A0000 + D_800E6280.unk_1104.w * 0x1400), 0x80180000 + D_800E6280.unk_1104.w * 0x1400, 0x1400);
    D_800E6280.unk_1104.w += 1;
    if (D_800E6280.unk_1104.w == 0x12) {
        func_80083474();
    }
}

void func_80133088(void) {
    RECT rect;

    func_80083474();
    rect.x = 0x140;
    rect.y = 0x80;
    rect.w = 0x180;
    rect.h = 0x80;
    func_8009C884(&rect, (void *)0x80180000);
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_801330D4);

void func_80133258(void) {
    switch (D_800E6280.unk_F5F) {
    case 0:
        D_8013A410 = D_8013A384;
        D_8013A414 = D_8013A3B0;
        D_8013A418 = D_8013A3DC;
        return;
    case 1:
        D_8013A410 = D_8013A394;
        D_8013A414 = D_8013A3C0;
        D_8013A418 = D_8013A3EC;
        return;
    case 2:
        D_8013A410 = D_8013A388;
        D_8013A414 = D_8013A3B4;
        D_8013A418 = D_8013A3E0;
        return;
    case 3:
        D_8013A410 = D_8013A38C;
        D_8013A414 = D_8013A3B8;
        D_8013A418 = D_8013A3E4;
        return;
    case 4:
        D_8013A410 = D_8013A3A4;
        D_8013A414 = D_8013A3D0;
        D_8013A418 = D_8013A3FC;
        return;
    case 5:
        D_8013A410 = D_8013A398;
        D_8013A414 = D_8013A3C4;
        D_8013A418 = D_8013A3F0;
        return;
    case 6:
        D_8013A410 = D_8013A39C;
        D_8013A414 = D_8013A3C8;
        D_8013A418 = D_8013A3F4;
        return;
    case 7:
        D_8013A410 = D_8013A390;
        D_8013A414 = D_8013A3BC;
        D_8013A418 = D_8013A3E8;
        return;
    case 8:
        D_8013A410 = D_8013A380;
        D_8013A414 = D_8013A3AC;
        D_8013A418 = D_8013A3D8;
        return;
    case 9:
        D_8013A410 = D_8013A3A0;
        D_8013A414 = D_8013A3CC;
        D_8013A418 = D_8013A3F8;
        /* fallthrough */
    default:
        return;
    }
}

void func_80133494(void) {
    s32 temp;
    temp = D_8013A4C0;
    switch (temp) {
    case 0:
        func_8007ED84(0x4350);
        func_800AE0F0(D_800CA1DC, "観覧車");
        break;
    case 1:
        if ((u8) D_800E6280.unk_03E >= 0x61U) {
            func_8007ED84(0x4359);
            func_800AE0F0(D_800CA1DC, "絶叫マシーンビビール");
        } else {
            func_8007ED84(0x433E);
            func_800AE0F0(D_800CA1DC, "ジェットコースター");
        }
        break;
    case 2:
        func_8007ED84(0x4347);
        func_800AE0F0(D_800CA1DC, "ゴーストハウス");
        break;
    }
    func_8004284C();
}

void func_8013357C(void) {
    func_80062CD0(0x6C8C);
    func_8004284C();
}

void func_801335A4(void) {
    D_800E6280.unk_F5F = 0;
    get_p_name(&D_800CA17C, 0);
    func_80062CD0(0x55DF);
    func_8004284C();
}

void func_801335E4(void) {
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    func_800847B8(D_800E6280.unk_75D);
    func_80133258();
    func_80083B24();
}

void func_80133620(void) {
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    func_80133258();
    func_800847B8(D_800E6280.unk_F5F);
    switch (D_800E6280.unk_F5F) {
    case 1:
        func_80062CD0(0x5DC8);
        break;
    case 2:
        func_80062CD0(0x52B5);
        break;
    case 3:
        func_80062CD0(0x5936);
        break;
    case 4:
        func_80062CD0(0x6746);
        break;
    case 5:
        func_80062CD0(0x603E);
        break;
    case 6:
        func_80062CD0(0x62B4);
        break;
    case 7:
        func_80062CD0(0x5B7F);
        break;
    case 8:
        func_80062CD0(0x506C);
        break;
    case 9:
        func_80062CD0(0x652A);
        break;
    }
    func_8004284C();
}

void func_80133720(void) {
    if (D_80122CDC != 0) {
        func_80133620();
        D_8013A4C4 |= 1 << (s8)D_8013A4C0;
    } else {
        func_801335A4();
    }
    func_80133258();
}

void func_80133784(void) {
    s32 temp;
    temp = D_8013A4C0;
    switch (temp) {
    case 0:
        D_8013A430 = 1;
        D_8013A438 = 1;
        break;
    case 1:
        if ((u8) D_800E6280.unk_03E >= 0x61U) {
            D_8013A430 = 5;
            D_8013A438 = 5;
            D_8013A428 += 2;
        } else {
            D_8013A430 = 3;
            D_8013A438 = 3;
        }
        break;
    case 2:
        D_8013A430 = 7;
        D_8013A438 = 7;
        if ((u8) D_800E6280.unk_03E < 0x61U) {
            D_8013A428 += 2;
        }
        break;
    }
    func_801338F8();
    func_8004284C();
}

void func_80133884(void) {
    if (D_8013A4C0 == 2) {
        func_80042808();
        return;
    }
    func_8004284C();
}

void func_801338C4(void) {
    D_8013A4C0 += 1;
    func_80042940(0x1B);
}

/* FAKE: s32 fall-through keeps the original counter/result register. T-10010. */
s32 func_801338F8(void) {
    u8 previous_state;
    u8 first_state;
    first_state = func_80051A68(0U) & 0x7F;
    if (first_state >= 2U) {
        D_8013A430 += 0xA;
    }
    if (first_state >= 3U) {
        D_8013A430 += 0xA;
    }
    if (first_state == 4) {
        D_8013A430 += 0xA;
    }
    previous_state = func_80051A68(D_800E6280.unk_75D) & 0x7F;
    if (previous_state >= 2U) {
        D_8013A438 += 0xA;
    }
    if (previous_state >= 3U) {
        D_8013A438 += 0xA;
    }
    if (previous_state == 4) {
        D_8013A438 += 0xA;
    }
}

void func_801339F8(void) {
    D_8013A5B0 = 0x801C6000;
    D_8013A5B4 = 0x801AE000;
    D_8013A5B8 = 0x801B2000;
    D_8013A5BC = 0x801B6000;
    D_8013A5C0 = 0x801BA000;
    D_8013A5C4 = 0x801BE000;
    D_8013A5C8 = 0x801C2000;
}

void func_80133A6C(void) {
    u8 sel = D_8013A4C0; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        func_801348F0();
        return;
    case 1:
        if ((u8) D_800E6280.unk_03E >= 0x61U) {
            func_80133AF0();
            return;
        }
        func_801350A4();
        return;
    default:
        func_80135E50();
        return;
    }
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_80133AF0);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_801348F0);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_801350A4);

s32 func_80135DA4(s16 arg0) {
    D_801217D0[4].unk_06 += arg0;
    D_801217D0[5].unk_06 += arg0;
    D_801217D0[4].unk_0A -= arg0;
    D_801217D0[5].unk_0A -= arg0;
    D_801217D0[6].unk_0A += arg0;
    D_801217D0[7].unk_0A += arg0;
    D_801217D0[6].unk_0F -= arg0;
    D_801217D0[7].unk_0F -= arg0;
}

s32 func_80135E50(void) {
    s16 *temp_a0;
    s16 *temp_v0;
    s16 temp_a1;
    s16 temp_v0_2;
    s16 var_s3;
    s32 remainder;
    u32 temp_s1;
    u32 temp_s1_3;
    u8 temp_s0;
    u8 temp_s0_2;

    switch (D_800E6280.unk_1104.u) {
    case 0:
        D_80122D2C = 1;
        func_800AE120((*(s32 *)&D_800E6280.unk_10F8));
        D_8013A5CC = 0;
        hizuke_disp_switch(0);
        func_801365DC(0x43BA);
        D_8013A5D4[0] = -0x50;
        D_8013A5D4[1] = 0;
        D_8013A5D4[2] = 0x64;
        D_8013A5DC[0] = -0x28;
        D_8013A5DC[1] = 0;
        D_8013A5DC[2] = -0x3C;
        D_800E6280.unk_1104.u += 1;
        D_8013A5E4[0] = (func_800AE0D0() % 3) - 1;
        D_8013A5E4[1] = (func_800AE0D0() % 3) - 1;
        D_8013A5E4[2] = (func_800AE0D0() % 3) - 1;
        D_8013A5EC[0] = (func_800AE0D0() % 3) - 1;
        D_8013A5EC[1] = (func_800AE0D0() % 3) - 1;
        D_8013A5EC[2] = (func_800AE0D0() % 3) - 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_1104.u += 1;
            break;
        }
        break;
    case 2:
        if (((((u32 *)D_80125C04)[0] & 0xFFFF0000) != 0x38000000) || ((((u32 *)D_80125C04)[1] & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n");
            ((RECT *)D_800B5920)[0].w = 0;
            ((RECT *)D_800B5920)[0].h = 0;
            D_800E6280.unk_1104.u -= 2;
            return 0;
        }
        func_8005751C(1);
        dec_bg_show_switch(1);
        func_80044750(0x203);
        D_800E6280.unk_1104.u += 1;
        break;
    case 3:
        temp_s0 = D_800E6280.unk_110A;
        temp_s1 = D_800E6280.unk_1104.u;
        normal_date_bg_fadein();
        D_800E6280.unk_1104.u = temp_s1;
        if (temp_s0 != D_800E6280.unk_110A) {
            D_800E6280.unk_1104.u = temp_s1 + 1;
        }
        D_800E6280.unk_110A = temp_s0;
        break;
    case 4:
        var_s3 = 0;
        do {
            if (((u32) (*(s32 *)&D_800E6280.unk_10F8) % 30U) == (func_800AE0D0() % 30)) {
                (D_8013A5E4)[var_s3] = (func_800AE0D0() % 3) - 1;
            }

            if (((u32) (*(s32 *)&D_800E6280.unk_10F8) % 30U) == (func_800AE0D0() % 30)) {
                (D_8013A5EC)[var_s3] = (func_800AE0D0() % 3) - 1;
            }

            if ((D_8013A5E4[var_s3] == 0) && (D_8013A5EC[var_s3] == 0)) {
                if ((remainder = func_800AE0D0() % 2)) {
                    D_8013A5E4[var_s3] = ((func_800AE0D0() % 2) * 2) - 1;
                } else {
                    D_8013A5EC[var_s3] = ((func_800AE0D0() % 2) * 2) - 1;
                }
            }
            temp_v0 = &(D_8013A5D4)[var_s3];
            *temp_v0 += D_8013A5E4[var_s3];
            temp_a0 = &(D_8013A5DC)[var_s3];
            *temp_a0 += D_8013A5EC[var_s3];
            temp_a1 = *temp_v0;
            if (temp_a1 < -0x95) {
                D_8013A5E4[var_s3] = 1;
            }
            if (temp_a1 >= 0x96) {
                D_8013A5E4[var_s3] = -1;
            }
            temp_v0_2 = *temp_a0;
            if (temp_v0_2 < -0x6D) {
                D_8013A5EC[var_s3] = 1;
            }
            if (temp_v0_2 >= 0x1E) {
                D_8013A5EC[var_s3] = -1;
            }
            var_s3++;
        } while (var_s3 < 3);
        func_8004A814((s32) D_8013A5D4[0], (s32) D_8013A5DC[0], 0xA, 2, 2, 0x808080, 3, 1);
        func_8004A814((s32) D_8013A5D4[1], (s32) D_8013A5DC[1], 0xA, 2, 2, 0x804040, 3, 1);
        func_8004A814((s32) D_8013A5D4[2], (s32) D_8013A5DC[2], 0xA, 2, 2, 0x800000, 3, 1);
        /* FAKE (T-18800): an empty test (no code) ends the block before the counter test; without it
         * IDO keeps the old count in a1 and branches with bnez instead of the original's v1 and
         * xori/sltiu/beqz (the DATE twin func_801489D0 has real code there). */
        if (var_s3) {
        }
        if (D_8013A5CC++ == 0x96) {
            D_800E6280.unk_1104.u += 1;
            break;
        }
        break;
    case 5:
        temp_s0_2 = D_800E6280.unk_110A;
        temp_s1_3 = D_800E6280.unk_1104.u;
        normal_date_bg_fadeout();
        D_800E6280.unk_1104.u = temp_s1_3;
        if (temp_s0_2 != D_800E6280.unk_110A) {
            D_800E6280.unk_1104.u = temp_s1_3 + 1;
        }
        D_800E6280.unk_110A = temp_s0_2;
        break;
    case 6:
        dec_bg_show_switch(1);
        D_800E6280.unk_1104.u += 1;
        break;
    default:
        func_80044750(0x200);
        D_80122D2C = 0;
        hizuke_disp_switch(1);
        func_8004284C();
        break;
    }
}

void func_801365DC(s32 arg0) {
    s32 v;

    D_800B3D60 = 0;
    v = func_8005742C(arg0, 1);
    if (v == D_800B5939) {
        func_800AE0B0("same\n");
        D_800E6280.unk_1104.w += 3;
        return;
    }
    if (v == 1 - D_800B5939) {
        func_800AE0B0("another\n");
        D_800E6280.unk_1104.w += 2;
    }
}

void func_80136660(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl44 tbl;

    tbl = D_8013A600;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

s32 func_801366E8(void) {
    s32 v;

    v = func_80051A68(0);
    if ((D_800E6280.unk_0F8 & 0xF) == D_800E6280.unk_03F && ((u32)(D_800E6280.unk_0F8 << 0x17) >> 0x1B) == D_800E6280.unk_040) {
        if (((u8)v & 0x7F) < 2U) {
            D_800E6280.unk_71E |= 4;
            D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
            func_80042908(4);
            return 0;
        }
        D_800E6280.unk_71E |= 8;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_80136794);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_80136860);

void func_80136A90(void) {
    D_8013A430 = 9;
    D_8013A438 = 9;
    func_801338F8();
    if (((u8) func_80051A68(0) & 0x7F) == 4 && (func_800AE0D0() % 2)) {
        D_8013A430 += 1;
    }
    if (((u8) func_80051A68(D_800E6280.unk_75D) & 0x7F) == 4 && (func_800AE0D0() % 2) && (D_800E6280.unk_75D != 6)) {
        D_8013A438 += 1;
    }
    func_8004284C();
}

void func_80136B7C(void) {
    bg_read_sub2(0x432B);
    func_8004284C();
}

void func_80136BA4(void) {
    if (D_800E6280.unk_1BC[0].unk_06 < 0xA) {
        D_8013A428 += 2;
        D_800E6280.unk_F5F = 0;
    } else if (get_h_yuukou(D_800E6280.unk_F5F) < 0xA) {
        D_8013A428 += 2;
    } else if (D_800E6280.unk_1BC[0].unk_0A >= 0x33) {
        D_800E6280.unk_F5F = 0;
    } else if (D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0A < 0x33) {
        if (D_800E6280.unk_1BC[0].unk_02 >= 0x32) {
            D_8013A428 += 1;
            D_800E6280.unk_F5F = 0;
        } else if (get_h_tokimeki(D_800E6280.unk_F5F) >= 0x32) {
            D_8013A428 += 1;
        } else {
            D_8013A428 += 4;
            func_8004284C();
            func_8004284C();
        }
    }
    func_8004284C();
}

void func_80136CEC(void) {
    D_8013A428 = 0x17;
    func_8004284C();
}

void func_80136D14(void) {
    func_800847B8((u8) *(s8 *)&D_800E6280.unk_F5F);
    D_8013A428 = 0x15;
    func_8004284C();
}
