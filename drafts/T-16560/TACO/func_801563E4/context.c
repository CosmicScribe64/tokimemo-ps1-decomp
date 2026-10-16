#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_80154960);

void func_80154F74(void) {
    /* FAKE: unused leading TcPos local reproduces the frame (see func_80156F50); real locals unknown. T-9180 */
    TcPos p0;
    TcPos pos;
    TcPos vel;
    s32 id;
    s32 r;

    func_800AE120(D_801604C0 + 0x1E240);
    r = func_800AE0D0();
    pos.x = (func_800AE0D0() % 5000 + D_8015EDB4->unk74 + r % 3000) - 0xFA0;
    r = func_800AE0D0();
    pos.y = (func_800AE0D0() % 2500 + D_8015EDB4->unk76 + r % 1500) - 0x7D0;
    pos.z = -0x8000;
    vel.x = func_800AE0D0() % 4096;
    vel.y = func_800AE0D0() % 4096;
    vel.z = func_800AE0D0() % 4096;
    vel.unk6 = func_800AE0D0() % 7;
    vel.x = func_800AE0D0() % 3;
    if (func_800AE0D0() % 3 == 0) {
        id = func_8015ACCC(0, 1, vel, pos, vel, vel.x * 4 + 4, 0x10);
    } else {
        id = func_8015ACCC(0, 1, vel, pos, vel, vel.x * 4 + 4, 0);
    }
    D_8015EDB4[id].unk84[0] = vel.x + 3;
}

void func_80155230(s32 arg0) {
    s16 t;

    (arg0 + D_8015EDB4)->unk78 += 0x100;
    t = ((arg0 + D_8015EDB4)->unk7C << 9) + 0x200;
    (arg0 + D_8015EDB4)->unk70 = t;
    D_8015EDB4[arg0].unk6E = t;
    D_8015EDB4[arg0].unk6C = t;
    switch ((arg0 + D_8015EDB4)->unk6A) {
    case 0:
        (arg0 + D_8015EDB4)->unk64 += 0x32;
        break;
    case 1:
        (arg0 + D_8015EDB4)->unk66 += 0x32;
        break;
    case 2:
        (arg0 + D_8015EDB4)->unk68 += 0x32;
        break;
    case 3:
        (arg0 + D_8015EDB4)->unk64 += 0x63;
        break;
    case 4:
        (arg0 + D_8015EDB4)->unk66 += 0x63;
        break;
    case 5:
        (arg0 + D_8015EDB4)->unk68 += 0x63;
        break;
    }
    if ((arg0 + D_8015EDB4)->unk78 >= 0x1001) {
        func_8015C208(arg0);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_801553AC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_801557E8);

void func_8015595C(void) {
    /* FAKE: unused TcPos local reproduces the frame and the offsets of pos and vel (see func_80156F50); real locals unknown. T-9180 */
    TcPos pos;
    TcPos p1;
    TcPos vel;
    s32 id;
    s32 r;

    func_800AE120(D_801604C0);
    r = func_800AE0D0();
    pos.x = (func_800AE0D0() % 250 + D_8015EDB4->unk74 + r % 150) - 0xC8;
    r = func_800AE0D0();
    pos.y = (func_800AE0D0() % 135 + D_8015EDB4->unk76 + r % 75) - 0x64;
    pos.z = -0x1D4C;
    vel.y = 0x800;
    vel.z = 0;
    vel.x = 0;
    if (D_801604CC % 3 == 0) {
        id = func_8015ACCC(5, 3, pos, D_801604D4, vel, 4, 0x80);
    } else {
        id = func_8015ACCC(5, 3, pos, D_801604D4, vel, 4, 0x10);
    }
    if (id != -1) {
        D_8015EDB4[id].unk84[0] = 0;
    }
    id = func_8015ACCC(5, 4, pos, D_801604D4, vel, 2, 0);
    if (id != -1) {
        D_8015EDB4[id].unk84[0] = 0;
    }
    D_801604CC += 1;
}

void func_80155C64(s32 arg0) {
    TcActor *actor;
    s32 trig;

    /* FAKE: byte-stride pointer calculations and xor-zero keep the original
     * addition and multiplication operand order. T-10010. */
    trig = func_800A0070((arg0 + D_8015EDB4)->unk7A * 0x3C);
    actor = (TcActor *)((u8 *)D_8015EDB4 + arg0 * sizeof(TcActor));
    actor->unk74 = ((s32) ((trig ^ 0) * (0x3E8 - (actor->unk7A * 0xA))) >> 0xD) + actor->unk7C;
    trig = func_800A0140((arg0 + D_8015EDB4)->unk7A * 0x3C);
    actor = (TcActor *)((u8 *)D_8015EDB4 + arg0 * sizeof(TcActor));
    actor->unk76 = ((s32) ((trig ^ 0) * (0x3E8 - (actor->unk7A * 0xA))) >> 0xD) + actor->unk7E;
    actor = arg0 + D_8015EDB4;
    actor->unk78 = actor->unk80;
    if (!((arg0 + D_8015EDB4)->unk7A & 1)) {
        func_8015C0A0(0, arg0);
    } else {
        func_8015C0A0(1, arg0);
    }
    actor = arg0 + D_8015EDB4;
    if (actor->unk7A >= 0x65) {
        actor->unk84[0] = D_80161ECD;
        D_8015EDB4[arg0].unk2 = 5;
        D_8015EDB4[arg0].unk7A = 0;
        D_8015EDB4[arg0].unk7E = 0;
    }
}

void func_80155E10(s32 arg0) {
    (arg0 + D_8015EDB4)->unk74 = ((func_800A0070((arg0 + D_8015EDB4)->unk7A * 0x3C + 0x800) * (0x3E8 - (D_8015EDB4 + arg0)->unk7A * 0xA)) >> 0xD) + (D_8015EDB4 + arg0)->unk7C;
    (arg0 + D_8015EDB4)->unk76 = ((func_800A0140((arg0 + D_8015EDB4)->unk7A * 0x3C + 0x800) * (0x3E8 - (D_8015EDB4 + arg0)->unk7A * 0xA)) >> 0xD) + (D_8015EDB4 + arg0)->unk7E;
    (arg0 + D_8015EDB4)->unk78 = (arg0 + D_8015EDB4)->unk80;
    if ((arg0 + D_8015EDB4)->unk7A >= 0x65) {
        func_8015C208(arg0);
    }
    if (((arg0 + D_8015EDB4)->unk7A & 1) == 0) {
        func_8015C0A0(1, arg0);
    } else {
        func_8015C0A0(0, arg0);
    }
}

void func_80155F98(s32 arg0) {
    TcPos pos;

    switch ((arg0 + D_8015EDB4)->unk7E) {
    case 0:
        if (((arg0 + D_8015EDB4)->unk7A % 7) == 0) {
            pos.x = (arg0 + D_8015EDB4)->unk74;
            pos.y = (arg0 + D_8015EDB4)->unk76;
            pos.z = (arg0 + D_8015EDB4)->unk78;
            func_8013DB80(pos, 0x100, 0xC0);
        }
        if ((arg0 + D_8015EDB4)->unk7A == 0x3C) {
            if (D_8015EDB4->unk74 < (arg0 + D_8015EDB4)->unk7C) {
                (arg0 + D_8015EDB4)->unk7C = 1;
            } else {
                (arg0 + D_8015EDB4)->unk7C = -1;
            }
            D_8015EDB4[arg0].unk7A = 0;
            (arg0 + D_8015EDB4)->unk7E += 1;
        }
        break;
    case 1:
        (arg0 + D_8015EDB4)->unk66 += (arg0 + D_8015EDB4)->unk7C * 0x32;
        if ((arg0 + D_8015EDB4)->unk7A >= 0x29) {
            (arg0 + D_8015EDB4)->unk7E += 1;
        }
        break;
    case 2:
        (arg0 + D_8015EDB4)->unk78 -= 0x320;
        if ((arg0 + D_8015EDB4)->unk78 < -0x7530) {
            func_8015C208(arg0);
        }
        break;
    }
}

void func_80156178(void) {
    TcPos p0;
    TcPos p2;
    TcPos pos;

    func_800AE120(D_801604C0);
    /* FAKE: x and y on one line so as1 orders their stores y, x like the original (decomp-permuter); real source unknown. T-9180 */
    pos.x = -0x100; pos.y = 0x800;
    pos.z = 0;
    func_8015ACCC(6, 6, D_801604D4, D_801604D4, pos, 0xFF, 0xF0);
}

s32 func_80156244(s32 arg0) {
    /* FAKE: mixed pointer-add spelling and byte-stride view retain the target's addu operand order. T-9750. */
    s32 state;
    s16 value;

    state = (D_8015EDB4 + arg0)->unk7C;
    switch (state) {
    case 0:
        (D_8015EDB4 + arg0)->unk78 = -0x8000;
        (D_8015EDB4 + arg0)->unk74 = 0;
        (D_8015EDB4 + arg0)->unk76 = -0x1F4;
        (D_8015EDB4 + arg0)->unk70 = 1;
        value = ((TcActor *)((u8 *)D_8015EDB4 + arg0 * 0x88))->unk70;
        (D_8015EDB4 + arg0)->unk6E = value;
        (D_8015EDB4 + arg0)->unk6C = value;
        (D_8015EDB4 + arg0)->unk84[0] = 0;
        (arg0 + D_8015EDB4)->unk7C++;
        break;
    case 1:
        value = (D_8015EDB4 + arg0)->unk7A * 0x14;
        (D_8015EDB4 + arg0)->unk70 = value;
        (D_8015EDB4 + arg0)->unk6E = value;
        (D_8015EDB4 + arg0)->unk6C = value;
        if ((D_8015EDB4 + arg0)->unk7A >= 0x1F5) {
            (D_8015EDB4 + arg0)->unk7C++;
        }
        break;
    case 2:
        (D_8015EDB4 + arg0)->unk78 += 0x32;
        if ((D_8015EDB4 + arg0)->unk78 >= -0x61A7) {
            (D_8015EDB4 + arg0)->unk7C++;
        }
        break;
    case 3:
        (D_8015EDB4 + arg0)->unk84[0] = 0x20;
        break;
    }
}

void func_801563E4(s32 arg0) {
    TcPos sp58;
    TcPos sp50;
    TcPos sp48;
    s32 sp40;
    s32 temp_hi;
    s32 temp_hi_2;
    s32 temp_hi_3;
    s32 temp_hi_4;

    func_800AE120(D_801604C0);
    if (arg0 == 0) {
        sp40 = func_800AE0D0();
        sp50.x = ((func_800AE0D0() % 4000) + D_8015EDB4->unk74 + (sp40 % 2000)) - 0xBB8;
        sp40 = func_800AE0D0();
        sp50.y = ((func_800AE0D0() % 2500) + D_8015EDB4->unk76 + (sp40 % 1500)) - 0x7D0;
        sp50.z = (func_800AE0D0() % 20000) + 0x8000;
        temp_hi = func_800AE0D0() % 30;
        sp48.y = 0x800;
        sp48.x = 0;
        sp48.z = 0;
        sp58.x = temp_hi + 0x28;
        sp58.y = (s16) (func_800AE0D0() % 2);
        if (D_801604C0 < 0xF0) {
            func_8015ACCC(7, 7, sp58, sp50, sp48, 5, 0x50);
            return;
        }
        func_8015ACCC(7, 7, sp58, sp50, sp48, 5, 0);
        return;
    }
    sp50.y = -0x384;
    sp50.z = -0x6590;
    sp48.y = 0x800;
    sp48.x = 0;
    sp48.z = 0;
    temp_hi_2 = func_800AE0D0() % 20;
    sp50.x = 0;
    sp58.x = temp_hi_2 + 0x1E;
    sp58.y = (s16) (func_800AE0D0() % 2);
    D_8015EDB4[func_8015ACCC(7, 7, sp58, sp50, sp48, 0x14, 0)].unk84[0] = 4;
    temp_hi_3 = func_800AE0D0() % 20;
    sp50.x = -0x3E8;
    sp58.x = temp_hi_3 + 0x1E;
    sp58.y = (s16) (func_800AE0D0() % 2);
    D_8015EDB4[func_8015ACCC(7, 7, sp58, sp50, sp48, 0x14, 0)].unk84[0] = 4;
    temp_hi_4 = func_800AE0D0() % 20;
    sp50.x = 0x3E8;
    sp58.x = temp_hi_4 + 0x1E;
    sp58.y = (s16) (func_800AE0D0() % 2);
    D_8015EDB4[func_8015ACCC(7, 7, sp58, sp50, sp48, 0x14, 0)].unk84[0] = 4;
}

void func_80156924(s32 arg0) {
    (arg0 + D_8015EDB4)->unk78 += 0xFA;
    (arg0 + D_8015EDB4)->unk74 += (D_8015EDB4->unk74 - (arg0 + D_8015EDB4)->unk74) / (arg0 + D_8015EDB4)->unk7C;
    (arg0 + D_8015EDB4)->unk76 += (D_8015EDB4->unk76 - (arg0 + D_8015EDB4)->unk76) / (arg0 + D_8015EDB4)->unk7C;
    if ((arg0 + D_8015EDB4)->unk7E == 0) {
        (arg0 + D_8015EDB4)->unk68 += 0xC8;
    } else {
        (arg0 + D_8015EDB4)->unk68 -= 0xC8;
    }
    if ((arg0 + D_8015EDB4)->unk78 >= 0x7D1) {
        func_8015C208(arg0);
    }
}
