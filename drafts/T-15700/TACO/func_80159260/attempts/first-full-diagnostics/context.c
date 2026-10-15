#include "common.h"
#include "ovl/TACO.h"

void func_80159090(void) {
    D_801604C0 += 1;
    func_8015B59C();
    if (((s32) D_801604C0 % 30) == 0) {
        if (D_801604C4 < 4) {
            if (func_8015CCC8(2) < 0xA) {
                func_80159260();
            }
        } else if ((D_801604C4 < 6) && (func_8015CCC8(2) < 3)) {
            func_80159260();
        }
    }
    switch (D_801604C4) {
    case 0:
        func_80159878(D_801604C4);
        func_8015A900();
    default:
        break;
    case 1:
    case 2:
    case 3:
        if (func_8015CCC8(3) == 0) {
            func_80159878(D_801604C4);
            func_8015A900();
        }
        break;
    case 4:
        if (func_8015CCC8(3) == 0) {
            func_8015A900();
        }
        break;
    case 5:
        func_8015A900();
        break;
    case 6:
        func_8015A900();
        break;
    case 7:
    case 8:
    case 9:
    case 10:
        if (func_8015CE64() == 0) {
            func_8015A164(D_801604C4 - 3);
            func_8015A900();
        }
        break;
    case 11:
        if (func_8015CCC8(0xFF) == 0) {
            func_8015A900();
        }
        break;
    case 12:
        if (func_8015CE64() == 0) {
            func_8015ABF0();
            func_8004284C();
        }
        break;
    }
}

/* bulk_m2c declarations (T-11190) */

extern s32 subroutine_arg2;
extern s32 subroutine_arg3;
/* end bulk_m2c declarations */
s32 func_80159260(void) {
    s16 sp5A;
    s16 sp58;
    s16 sp54;
    s16 sp52;
    s16 sp50;
    s32 sp44;
    u32 temp_hi;

    func_800AE120(D_801604C0);
    sp44 = func_800AE0D0();
    sp50 = ((func_800AE0D0() % 2500) + (sp44 % 1500)) - 0x7D0;
    sp44 = func_800AE0D0();
    sp52 = ((func_800AE0D0() % 2000) + (sp44 % 1000)) - 0x7D0;
    sp54 = (func_800AE0D0() % 5000) - 0x61A8;
    sp58 = (func_800AE0D0() % 200) - 0x64;
    sp5A = (func_800AE0D0() % 200) - 0x64;
    temp_hi = func_800AE0D0() % 5U;
    switch (temp_hi) {
    case 0:
        func_8015ACCC(2, 0x20, /* TcPos+0x0 */ subroutine_arg2, /* TcPos+0x4 */ subroutine_arg3, /* TcPos+0x0 */ sp50, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&sp50) + (4)), /* TcPos+0x0 */ D_801604E4.unk0, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&D_801604E4) + (4)), 4, 0x10);
        return;
    case 1:
        func_8015ACCC(2, 0x20, /* TcPos+0x0 */ subroutine_arg2, /* TcPos+0x4 */ subroutine_arg3, /* TcPos+0x0 */ sp50, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&sp50) + (4)), /* TcPos+0x0 */ D_801604E4.unk0, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&D_801604E4) + (4)), 4, 0x10);
        return;
    case 2:
        func_8015ACCC(2, 0x20, /* TcPos+0x0 */ subroutine_arg2, /* TcPos+0x4 */ subroutine_arg3, /* TcPos+0x0 */ sp50, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&sp50) + (4)), /* TcPos+0x0 */ D_801604E4.unk0, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&D_801604E4) + (4)), 4, 0);
        return;
    case 3:
        func_8015ACCC(2, 0x20, /* TcPos+0x0 */ subroutine_arg2, /* TcPos+0x4 */ subroutine_arg3, /* TcPos+0x0 */ sp50, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&sp50) + (4)), /* TcPos+0x0 */ D_801604E4.unk0, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&D_801604E4) + (4)), 4, 0);
        return;
    case 4:
        func_8015ACCC(2, 0x20, /* TcPos+0x0 */ subroutine_arg2, /* TcPos+0x4 */ subroutine_arg3, /* TcPos+0x0 */ sp50, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&sp50) + (4)), /* TcPos+0x0 */ D_801604E4.unk0, /* TcPos+0x4 */ *(s32 *)((unsigned char *)(&D_801604E4) + (4)), 4, 0);
        /* fallthrough */
    default:
        return;
    }
}

void func_80159678(s32 arg0) {
    (arg0 + D_8015EDB4)->unk74 -= (((((arg0 + D_8015EDB4)->unk78 << 5) / 25000) * func_800A0140((arg0 + D_8015EDB4)->unk7A << 5)) >> 0xC);
    (arg0 + D_8015EDB4)->unk76 -= (((((arg0 + D_8015EDB4)->unk78 << 4) / 25000) * func_800A0070((arg0 + D_8015EDB4)->unk7A << 7)) >> 0xC);
    if ((arg0 + D_8015EDB4)->unk7A >= 0x12D) {
        (arg0 + D_8015EDB4)->unk78 += 0x100;
        (arg0 + D_8015EDB4)->unk74 += ((D_8015EDB4->unk74 + (arg0 + D_8015EDB4)->unk7C) - (arg0 + D_8015EDB4)->unk74) / 10;
        (arg0 + D_8015EDB4)->unk76 += ((D_8015EDB4->unk76 + (arg0 + D_8015EDB4)->unk7E) - (arg0 + D_8015EDB4)->unk76) / 10;
    }
    if ((arg0 + D_8015EDB4)->unk78 >= 0x1001) {
        func_8015C208(arg0);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80159090", func_80159878);

void func_80159F60(s32 arg0) {
    TcPos pos;

    (arg0 + D_8015EDB4)->unk74 += -(arg0 + D_8015EDB4)->unk74 / 50;
    if ((arg0 + D_8015EDB4)->unk7A >= 0xB5) {
        (arg0 + D_8015EDB4)->unk7A = 0;
    }
    if (((arg0 + D_8015EDB4)->unk7A % 10) == 0) {
        if (func_800AE0C0((arg0 + D_8015EDB4)->unk74) < 0x3E8) {
            pos.x = (arg0 + D_8015EDB4)->unk74;
            pos.y = (arg0 + D_8015EDB4)->unk76;
            pos.z = (arg0 + D_8015EDB4)->unk78;
            switch ((arg0 + D_8015EDB4)->unk76) {
            case -1200:
                if ((arg0 + D_8015EDB4)->unk7A < 0x3C) {
                    func_8013DB80(pos, 0x180, 0xC0);
                }
                break;
            case -700:
                if ((arg0 + D_8015EDB4)->unk7A >= 0x3D && (arg0 + D_8015EDB4)->unk7A < 0x78) {
                    func_8013DB80(pos, 0x180, 0x82);
                }
                break;
            case -200:
                if ((arg0 + D_8015EDB4)->unk7A >= 0x79) {
                    func_8013DB80(pos, 0x180, 0xC0);
                }
                break;
            }
        }
    }
}

void func_8015A164(s32 arg0) {
    /* FAKE: unused leading TcPos local reproduces the frame (see func_80156F50); real locals unknown. T-9180 */
    TcPos p0;
    TcPos pos;
    TcPos vel;
    s32 i;
    s32 j;

    func_800AE120(D_801604C0);
    pos.z = 0x4E20;
    func_8015ACCC(0xD, 0x23, D_801604D4, pos, D_801604E4, 0x28, 0xD0);
    i = 0;
    if (arg0 > 0) {
        j = 0;
        do {
            vel.y = 0x800;
            vel.x = 0;
            vel.z = 0;
            vel.unk6 = j / arg0;
            func_8015ACCC(0xD, 0x22, D_801604D4, pos, vel, 0x14, (func_800AE0D0() % 9) * 0x10);
            i += 1;
            j += 0x1000;
        } while (i != arg0);
    }
}

void func_8015A378(s32 arg0) {
    switch ((arg0 + D_8015EDB4)->unk7C) {
    case 0:
        D_8015EDB4[arg0].unk74 = (func_800A0070((arg0 + D_8015EDB4)->unk6A) * 0x3E8) >> 0xC;
        D_8015EDB4[arg0].unk76 = ((func_800A0140((arg0 + D_8015EDB4)->unk6A) * 0x3E8) >> 0xC) - 0x2BC;
        D_8015EDB4[arg0].unk78 = -0x4E20;
        D_8015EDB4[arg0].unk6E = 1;
        (arg0 + D_8015EDB4)->unk7C += 1;
        break;
    case 1:
        (arg0 + D_8015EDB4)->unk6E = (arg0 + D_8015EDB4)->unk7A << 9;
        (arg0 + D_8015EDB4)->unk84[0] = ((arg0 + D_8015EDB4)->unk7A * 0x16) / 32;
        if ((arg0 + D_8015EDB4)->unk6E >= (arg0 + D_8015EDB4)->unk6C) {
            (arg0 + D_8015EDB4)->unk7C += 1;
            D_8015EDB4[arg0].unk7A = 0;
        }
        break;
    case 2:
        D_8015EDB4[arg0].unk74 = (func_800A0070((arg0 + D_8015EDB4)->unk6A) * 0x3E8) >> 0xC;
        D_8015EDB4[arg0].unk76 = ((func_800A0140((arg0 + D_8015EDB4)->unk6A) * 0x3E8) >> 0xC) - 0x2BC;
        (arg0 + D_8015EDB4)->unk6A += 0x20;
        if ((arg0 + D_8015EDB4)->unk7A >= 0x259) {
            (arg0 + D_8015EDB4)->unk7C += 1;
            D_8015EDB4[arg0].unk7A = 0;
        }
        break;
    case 3:
        (arg0 + D_8015EDB4)->unk6E = (0x10 - (arg0 + D_8015EDB4)->unk7A) << 9;
        (arg0 + D_8015EDB4)->unk84[0] = ((0x10 - (arg0 + D_8015EDB4)->unk7A) << 5) / 32;
        if ((arg0 + D_8015EDB4)->unk6E <= 0) {
            func_8015C208(arg0);
        }
        break;
    }
}

void func_8015A680(s32 arg0) {
    TcPos pos;

    pos.x = (arg0 + D_8015EDB4)->unk74;
    pos.y = (arg0 + D_8015EDB4)->unk76;
    pos.z = (arg0 + D_8015EDB4)->unk78;
    switch ((arg0 + D_8015EDB4)->unk7C) {
    case 0:
        (arg0 + D_8015EDB4)->unk74 = 0;
        D_8015EDB4[arg0].unk76 = -0x2BC;
        D_8015EDB4[arg0].unk78 = -0x4E20;
        D_8015EDB4[arg0].unk6E = 1;
        (arg0 + D_8015EDB4)->unk7C += 1;
        break;
    case 1:
        (arg0 + D_8015EDB4)->unk6E = (arg0 + D_8015EDB4)->unk7A << 9;
        (arg0 + D_8015EDB4)->unk84[0] = ((arg0 + D_8015EDB4)->unk7A * 0x16) / 32;
        if ((arg0 + D_8015EDB4)->unk6E >= (arg0 + D_8015EDB4)->unk6C) {
            (arg0 + D_8015EDB4)->unk7C += 1;
            D_8015EDB4[arg0].unk7A = 0;
        }
        break;
    case 2:
        if (((arg0 + D_8015EDB4)->unk7A % 15) == 0) {
            func_8013DB80(pos, 0x100, 0xC2);
        }
        if ((arg0 + D_8015EDB4)->unk7A >= 0x259) {
            (arg0 + D_8015EDB4)->unk7C += 1;
            D_8015EDB4[arg0].unk7A = 0;
        }
        break;
    case 3:
        (arg0 + D_8015EDB4)->unk6E = (0x10 - (arg0 + D_8015EDB4)->unk7A) << 9;
        (arg0 + D_8015EDB4)->unk84[0] = ((0x10 - (arg0 + D_8015EDB4)->unk7A) << 5) / 32;
        if ((arg0 + D_8015EDB4)->unk6E <= 0) {
            func_8015C208(arg0);
        }
        break;
    }
}
