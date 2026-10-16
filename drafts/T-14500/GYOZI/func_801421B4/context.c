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

/* bulk_m2c declarations (T-11190) */
extern s16 D_800F54A2;
extern s16 D_80129F66;
extern s16 D_80129F6A;
extern s16 D_8012B91C;
extern s16 D_8012B92A;
extern s16 D_8012B92E;
extern s32 D_800F62F8;
extern s8 D_8014882B;
extern u16 D_800F5A10;
extern u8 D_800F6306;
extern u8 D_800F6316;
extern u8 D_800F6326;
extern u8 D_800F6336;
extern u8 D_8012B907;
s32 func_8005BE34();
s32 func_8005C1B8();
/* end bulk_m2c declarations */
void func_801421B4(void) {
    s16 temp_t9;
    s16 var_v0;
    s16 var_v1;
    s16 var_v1_2;
    s32 temp_v0_2;
    s32 var_v0_2;
    s8 temp_t4;
    s8 temp_t7;
    s8 temp_t8;
    u8 *var_a0;
    u8 temp_t6;
    u8 temp_v0;
    u8 temp_v1;

    temp_t6 = D_800F6468 & 3;
    var_v1 = 1;
loop_1:
    temp_t8 = ((D_800F6468 * 2) + var_v1) & 3 & 0xFF;
    var_v0 = var_v1;
    if (temp_t6 == temp_t8) {
        var_v1 += 1;
        goto loop_1;
    }
loop_4:
    temp_t7 = (((u32) D_800F6468 / 10U) + var_v0) & 3 & 0xFF;
    if (temp_t6 == temp_t7) {
block_6:
        temp_t9 = var_v1 + 1;
        var_v1 = temp_t9;
        var_v0 = temp_t9;
        goto loop_4;
    }
    if (temp_t8 == temp_t7) {
        goto block_6;
    }
loop_7:
    temp_t4 = (var_v1 % 4) & 0xFF;
    if (temp_t6 == temp_t4) {
block_11:
        var_v1 += 1;
        goto loop_7;
    }
    if (temp_t8 == temp_t4) {
        goto block_11;
    }
    D_8014882B = temp_t4;
    if (temp_t7 == temp_t4) {
        goto block_11;
    }
    (*(s32 *)D_80148828) = temp_t6;
    *(s8 *)((unsigned char *)D_80148828 + (1)) = temp_t8;
    *(s8 *)((unsigned char *)D_80148828 + (2)) = temp_t7;
    var_v1_2 = 0;
    var_a0 = D_80148828;
    do {
        temp_v0 = *var_a0;
        if (temp_v0 == 1) {
            *var_a0 = 0xB;
        } else if (temp_v0 == 2) {
            *var_a0 = D_80148754;
        } else if (temp_v0 == 3) {
            *var_a0 = D_80148758;
        }
        var_v1_2 += 1;
        var_a0 = &D_80148828[var_v1_2];
    } while (var_v1_2 < 4);
    func_8005BE34(0, D_80129F66, D_80129F6A, D_80148828);
    func_8005C1B8(0);
    if (D_800F62F8 & 0x600060) {
        if (func_8005B32C() != 0) {
            if (D_800F6306 != 0) {
                D_80148818 = 0;
                goto block_31;
            }
            if (D_800F6316 != 0) {
                D_80148818 = 1;
                goto block_31;
            }
            if (D_800F6326 != 0) {
                D_80148818 = 2;
                goto block_31;
            }
            if (D_800F6336 != 0) {
                D_80148818 = 3;
block_31:
                *(u8 *)((unsigned char *)D_80129F40 + (3)) = (u8) (*(u8 *)((unsigned char *)D_80129F40 + (3)) & 0xFF7F);
                D_8012E6C0 = 0;
                func_8005B06C(1);
                D_8012B907 |= 0x80;
                temp_v1 = D_80148828[D_80148818];
                var_v0_2 = temp_v1 == 0xB;
                if (var_v0_2 != 0) {
                    var_v0_2 = D_800F53DE == 0x5F;
                }
                D_8012B91C = ((temp_v1 + ((D_800F53DE == 0x60) * 0xB) + ((D_800F53DE == 0x61) * 0x17)) - var_v0_2) + 1;
                D_8012B92A = (D_80148818 * 0x4D) - 0x75;
                D_8012B92E = -0x3C;
                D_8012B920 = 0;
                D_80148620 = (D_80148620 + (temp_v1 * 3) + D_800F53DE) - 0x5F;
                D_80148820 = 1;
                D_80148818 = D_80148818;
                if (((D_800F53DE == 0x5F) && (temp_v1 == 0) && (D_800F54A2 >= 0x64)) || (((temp_v1 == 0) & (D_800F53DE == 0x5F)) && ((u16) D_800F5A10 >= 0x64U)) || ((temp_v0_2 = temp_v1 == 1, ((temp_v0_2 & (D_800F53DE == 0x5F)) != 0)) && (D_800F549A >= 0x46)) || ((temp_v0_2 & (D_800F53DE == 0x61)) && (D_800F549A >= 0x64)) || (((temp_v1 == 2) & (D_800F53DE == 0x60)) && (D_800F549A >= 0x64))) {
                    D_80148624 = 1;
                }
                func_8004DE1C();
            }
        }
    }
}

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

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/801418B0", func_80142A8C);

void func_80142C14(void) {
    s16 i;

    for (i = 0; i < 3; i++) {
        if (((GyoziInitSprite44 *)D_80129F40)[i + 0x62].unk_18 == 0xE
            && ((GyoziInitSprite44 *)D_80129F40)[i + 0x62].unk_08 == 9) {
            ((GyoziInitSprite44 *)D_80129F40)[i + 0x62].unk_16 = func_800BCE40() % 3;
        }
    }
}
