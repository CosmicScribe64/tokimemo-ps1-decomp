#include "common.h"
#include "game.h"
#include "ovl/GYOZI.h"

void func_801418B0(void) {
    D_801487F0 = (u8 *)0x801D6360;
    D_801487F4 = (u8 *)0x801D6384;
    D_801487F8 = (u8 *)0x801D6414;
    D_801487FC = *(s16 *)0x801D641C;
    D_80148800 = (u8 *)0x801B0000;
}

void func_80141900(void) {
    D_80148804 = 0x801DDBB4;
    D_80148808 = 0x801DDBB8;
    D_8014880C = 0x801DDC70;
    D_80148814 = 0x801DB000;
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801418B0", func_80141944);

void func_80141A94(void) {
    func_8004C6A0(0, 0x40);
    func_80050D60(0, 0);
    func_80081D30(1);
}

void func_80141ACC(void) {
    s32 unused; /* FAKE: restore observed implicit-copy offsets. T-13202. */
    s16 i;

    D_8014881C = 0;
    D_80148820 = 0;
    D_80148824 = 0;
    func_801418B0();
    i = 0;
    do {
        func_8004EE18(D_80148800, (i + 0x10) & 0xFF, 1, 2, i);
        i++;
    } while (i < 9);
    func_800549E8(0x60);
    ((DisplayActor44 *)D_80129F40)[96].unk_01 = 8;
    ((DisplayActor44 *)D_80129F40)[96].unk_02 = 0x40;
    ((DisplayActor44 *)D_80129F40)[96].unk_38 = 0x01000000;
    ((DisplayActor44 *)D_80129F40)[96].unk_03 = 4;
    ((DisplayActor44 *)D_80129F40)[96].unk_0C = D_801487F4;
    ((DisplayActor44 *)D_80129F40)[96].unk_10 = D_801487F8;
    ((DisplayActor44 *)D_80129F40)[96].unk_34 = D_801487F0;
    ((DisplayActor44 *)D_80129F40)[96].unk_14 = D_801487FC;
    ((DisplayActor44 *)D_80129F40)[96].unk_16 = 0;
    ((DisplayActor44 *)D_80129F40)[96].unk_18 = 0;
    ((DisplayActor44 *)D_80129F40)[96].unk_06 = 1;
    ((DisplayActor44 *)D_80129F40)[96].unk_43 = 0;
    ((DisplayActor44 *)D_80129F40)[96].unk_26 = 0;
    ((DisplayActor44 *)D_80129F40)[96].unk_2A = 8;
    ((DisplayActor44 *)D_80129F40)[96].unk_07 = 0x80;
    ((DisplayActor44 *)D_80129F40)[96].unk_05 = 0x80;
    ((DisplayActor44 *)D_80129F40)[96].unk_04 = 0;
    ((DisplayActor44 *)D_80129F40)[97] = ((DisplayActor44 *)D_80129F40)[96];
    func_80141900();
    func_8004EE18((u8 *) D_80148814, 0x1F, 1, 2, 0);
    ((DisplayActor44 *)D_80129F40)[98] = ((DisplayActor44 *)D_80129F40)[96];
    ((DisplayActor44 *)D_80129F40)[98].unk_01 = 0xB;
    ((DisplayActor44 *)D_80129F40)[98].unk_04 = 8;
    ((DisplayActor44 *)D_80129F40)[98].unk_02 = 1;
    ((DisplayActor44 *)D_80129F40)[98].unk_0C = D_80148808;
    ((DisplayActor44 *)D_80129F40)[98].unk_10 = D_8014880C;
    ((DisplayActor44 *)D_80129F40)[98].unk_34 = D_80148804;
    ((DisplayActor44 *)D_80129F40)[98].unk_14 = D_80148810;
    ((DisplayActor44 *)D_80129F40)[98].unk_16 = (s16) (func_800BCE40() % 3);
    ((DisplayActor44 *)D_80129F40)[98].unk_18 = 0;
    ((DisplayActor44 *)D_80129F40)[98].unk_26 = -0xA0;
    ((DisplayActor44 *)D_80129F40)[98].unk_2A = -0x78;
    ((DisplayActor44 *)D_80129F40)[98].unk_43 = 0x1F;
    ((DisplayActor44 *)D_80129F40)[101] = ((DisplayActor44 *)D_80129F40)[99] = ((DisplayActor44 *)D_80129F40)[100] = ((DisplayActor44 *)D_80129F40)[98];
    ((DisplayActor44 *)D_80129F40)[99].unk_26 += 0x48;
    ((DisplayActor44 *)D_80129F40)[99].unk_16 = (s16) (func_800BCE40() % 3);
    ((DisplayActor44 *)D_80129F40)[99].unk_18 = 4;
    ((DisplayActor44 *)D_80129F40)[100].unk_26 += 0x90;
    ((DisplayActor44 *)D_80129F40)[100].unk_16 = (s16) (func_800BCE40() % 3);
    ((DisplayActor44 *)D_80129F40)[100].unk_18 = 0xA;
    ((DisplayActor44 *)D_80129F40)[101].unk_01 = 0xA;
    ((DisplayActor44 *)D_80129F40)[101].unk_02 = 0x40;
    ((DisplayActor44 *)D_80129F40)[101].unk_16 = 0;
    D_80148620 = 0;
    D_80148614 = D_801481F8;
    D_80148618 = D_8014822C;
    D_8014861C = D_80148260;
    D_800F53A0.girl[0].unk_0A += 1;
    D_800F53A0.girl[D_80148754].unk_0A += 1;
    D_800F53A0.girl[D_80148758].unk_0A += 1;
    func_8008FB00();
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801418B0", func_8014206C);

void func_8014210C(void) {
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s16 d[4];

    a[0] = -0x96;
    a[1] = -0x48;
    a[2] = 6;
    a[3] = 0x54;
    b[0] = b[1] = b[2] = b[3] = -0x40;
    c[0] = c[1] = c[2] = c[3] = 0x40;
    d[0] = d[1] = d[2] = d[3] = 0x40;
    func_8005BD20(0, 4, a, b, c, d);
    D_8012E6C0 = 1;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801418B0", func_801421B4);

void func_801425F4(void) {
    func_8008A0D4(0x4183);
    func_8004DE1C();
}

void func_8014261C(void) {
    s16 i;

    for (i = 0; i < 6; i++) {
        D_80129F40[i * 0x44 + 0x1983] = 0;
    }
    D_80148820 = 0;
    D_80148824 = 0;
    func_8004DE1C();
}

s32 func_8014267C(void) {
    D_8012E6BC = 0;
    if (D_801486E4 == 0) {
        D_800F62CF = 0;
        D_8012E664 = 0;
        D_8012E660 = 7;
        func_8004DEAC(6);
        D_800F5902 += 1;
        return 0;
    }
    if (D_801486E4 == 7) {
        D_800F62CF = 7;
        D_8012E664 = 7;
        D_8012E660 = 7;
        func_8004DEAC(7);
        D_800F592F += 1;
        return 0;
    }
    if (D_801486E4 == 9) {
        D_800F62CF = 9;
        D_8012E664 = 9;
        D_8012E660 = 7;
        func_8004DEAC(8);
        D_800F593D += 1;
        return 0;
    }
    func_8004DE1C();
}

void func_80142790(void) {
    if (D_80148828[D_80148818] == 0xB) {
        func_80086AB0(0x200);
    } else {
        func_80086AB0(0x201);
    }
    func_8004DE1C();
}

void func_801427E4(void) {
    s16 v;

    v = D_8012B920 + 0x200;
    D_8012B920 = v;
    if (v >= 0x1000) {
        func_8004DE1C();
    }
}

void func_80142828(void) {
    func_80051DD8(0x4D, 0x801B0000, 0xB0C5);
    func_8004DE1C();
}

void func_80142858(void) {
    func_80051DD8(0xE, 0x801D7000, 0xB112);
    func_8004DE1C();
}

void func_8014288C(void) {
    D_80148624 = 0;
    if (D_80148828[D_80148818] == 0xB) {
        D_80148620 = 0x27;
    } else {
        D_80148620 = 0x26;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801418B0", func_801428EC);

void func_80142A8C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u32 arg6, u32 arg7, s32 arg8, s32 arg9) {
    u8 *sp28;
    u32 temp_t2;
    u32 temp_t3;
    POLY_G3 *temp_v0;

    temp_v0 = (POLY_G3 *)func_80054B60(0x1C, D_80129F10);
    sp28 = temp_v0;
    func_800ABAB8(temp_v0);
    func_800ABA38(sp28, arg9);
    temp_t2 = arg7 >> 0x10;
    temp_t3 = arg7 >> 8;
    *(s8 *)((unsigned char *)sp28 + (5)) = (s8) (arg6 >> 8);
    *(s8 *)((unsigned char *)sp28 + (4)) = (s8) (arg6 >> 0x10);
    *(s8 *)((unsigned char *)sp28 + (6)) = (s8) arg6;
    *(s8 *)((unsigned char *)sp28 + (13)) = (s8) temp_t3;
    *(s8 *)((unsigned char *)sp28 + (12)) = (s8) temp_t2;
    *(s8 *)((unsigned char *)sp28 + (14)) = (s8) arg7;
    *(s8 *)((unsigned char *)sp28 + (21)) = (s8) temp_t3;
    *(s8 *)((unsigned char *)sp28 + (20)) = (s8) temp_t2;
    *(s8 *)((unsigned char *)sp28 + (22)) = (s8) arg7;
    *(s16 *)((unsigned char *)sp28 + (8)) = (s16) (D_800F53A0.unk_014[D_80129F10] + arg0);
    *(s16 *)((unsigned char *)sp28 + (10)) = (s16) (D_800F53A0.unk_018[D_80129F10] + arg1);
    *(s16 *)((unsigned char *)sp28 + (16)) = (s16) (D_800F53A0.unk_014[D_80129F10] + arg2);
    *(s16 *)((unsigned char *)sp28 + (18)) = (s16) (D_800F53A0.unk_018[D_80129F10] + arg3);
    *(s16 *)((unsigned char *)sp28 + (24)) = (s16) (D_800F53A0.unk_014[D_80129F10] + arg4);
    *(s16 *)((unsigned char *)sp28 + (26)) = (s16) (D_800F53A0.unk_018[D_80129F10] + arg5);
    func_800AB984(&D_800F7710[(D_80129F10 << 0xA) + (arg8 * 4)], sp28, sp28);
}

void func_80142C14(void) {
    s16 i;

    for (i = 0; i < 3; i++) {
        if (((GyoziInitSprite44 *)D_80129F40)[i + 0x62].unk_18 == 0xE
            && ((GyoziInitSprite44 *)D_80129F40)[i + 0x62].unk_08 == 9) {
            ((GyoziInitSprite44 *)D_80129F40)[i + 0x62].unk_16 = func_800BCE40() % 3;
        }
    }
}
