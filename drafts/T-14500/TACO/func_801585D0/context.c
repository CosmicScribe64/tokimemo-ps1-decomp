#include "common.h"
#include "ovl/TACO.h"

void func_80156A80(void) {
    D_801604C0 += 1;
    D_801604C8 += 1;
    func_8015B59C();
    if (D_801604C4 < 0xC) {
        switch (D_801604CC) {
        case 0:
            if (D_801604C8 == 1) {
                func_80158AB0();
            }
            if (func_8015CCC8(0xE) == 0) {
                func_8015A928();
            }
            break;
        case 1:
            func_80157AE0();
            func_8015A928();
            break;
        case 2:
        case 3:
            if (func_8015CCC8(0xA) == 0) {
                func_80157AE0();
                func_8015A928();
            }
            break;
        case 4:
        case 5:
        case 6:
            func_8015A928();
            break;
        case 7:
        case 8:
            func_8015A928();
            break;
        case 9:
            func_8015A928();
            D_801604CC = 0;
            break;
        }
    }
    switch (D_801604C4) {
    case 0:
    case 6:
        if ((D_801604C0 >= 0x2B) && (func_8015CCC8(8) == 0)) {
            func_8015A900();
        }
        switch (D_801604C0) {
        case 2:
            func_801571E8();
            break;
        case 10:
            func_801571E8();
            break;
        case 18:
            func_801571E8();
            break;
        case 26:
            func_801571E8();
            break;
        case 34:
            func_801571E8();
            break;
        case 42:
            func_801571E8();
            break;
        }
    default:
        break;
    case 1:
    case 7:
        if ((D_801604C0 >= 0x2B) && (func_8015CCC8(8) == 0)) {
            func_8015A900();
        }
        switch (D_801604C0) {
        case 2:
            func_801572D8();
            break;
        case 10:
            func_801572D8();
            break;
        case 18:
            func_801572D8();
            break;
        case 26:
            func_801572D8();
            break;
        case 34:
            func_801572D8();
            break;
        case 42:
            func_801572D8();
            break;
        }
        break;
    case 2:
    case 8:
        if ((D_801604C0 >= 0x15) && (func_8015CCC8(8) == 0)) {
            func_8015A900();
        }
        if (D_801604C0 == 5) {
            func_801573C8();
        }
        break;
    case 3:
    case 9:
        if ((D_801604C0 >= 0x2B) && (func_8015CCC8(8) == 0)) {
            func_8015A900();
        }
        switch (D_801604C0) {
        case 2:
            func_801571E8();
            break;
        case 10:
            func_801572D8();
            break;
        case 18:
            func_801571E8();
            break;
        case 26:
            func_801572D8();
            break;
        case 34:
            func_801571E8();
            break;
        case 42:
            func_801572D8();
            break;
        }
        break;
    case 4:
    case 5:
    case 10:
    case 11:
        if ((D_801604C0 >= 0x42) && (func_8015CCC8(0xC) == 0)) {
            func_8015A900();
        }
        switch (D_801604C0) {
        case 0x5:
            func_8015800C();
            break;
        case 0x23:
            func_8015800C();
            break;
        case 0x41:
            func_8015800C();
            break;
        }
        break;
    case 12:
        if (func_8015CCC8(0xFF) == 0) {
            func_8015A900();
        }
        break;
    case 13:
        if (func_8015CE64() == 0) {
            func_8015ABF0();
            func_8004284C();
        }
        break;
    }
}

void func_80156F50(void) {
    /* the family's position locals and spill (see func_80158AB0); the unused ones keep their stack
     * slots. T-9020 */
    TcPos p0;
    TcPos pos;
    TcPos p2;
    s32 r;

    func_800AE120(D_801604C0);
    r = func_800AE0D0();
    pos.x = (D_8015EDB4[0].unk74 + (func_800AE0D0() % 1500) + (r % 500)) - 0x3E8;
    r = func_800AE0D0();
    pos.y = (func_800AE0D0() % 1000 + (r % 500)) - 0x5DC;
    pos.z = -0x8000;
    func_8015ACCC(0xB, 0x10, D_801604D4, pos, D_801604E4, 6, 0);
}

void func_801570A4(s32 arg0) {
    TcPos pos;

    if ((arg0 + D_8015EDB4)->unk7A % 60 == 0) {
        pos.x = (arg0 + D_8015EDB4)->unk74;
        pos.y = (arg0 + D_8015EDB4)->unk76;
        pos.z = (arg0 + D_8015EDB4)->unk78;
        func_8013DB80(pos, 0x80, 0);
    }
    (arg0 + D_8015EDB4)->unk76 += func_800A0070((arg0 + D_8015EDB4)->unk7A << 6) * 0x14 >> 12;
    (arg0 + D_8015EDB4)->unk78 += 0x96;
    if ((arg0 + D_8015EDB4)->unk78 > 0x1000) {
        func_8015C208(arg0);
    }
}

void func_801571E8(void) {
    TcPos p0;
    TcPos a;
    TcPos b;

    func_800AE120(D_801604C0 + 0x1E240);
    /* FAKE: the three stores on one source line make as1 order them z, y, x as the original does. T-9160 */
    a.x = 0x3E8; a.y = 0x12C; a.z = -0x8000;
    b.y = 0x800;
    b.x = 0;
    b.z = 0;
    b.unk6 = 3;
    func_8015ACCC(8, 0x12, D_801604D4, a, b, 4, 0);
}

void func_801572D8(void) {
    TcPos p0;
    TcPos a;
    TcPos b;

    func_800AE120(D_801604C0 + 0x1E240);
    /* FAKE: the three stores on one source line make as1 order them z, y, x as the original does. T-9160 */
    a.x = -0x3E8; a.y = 0x12C; a.z = -0x8000;
    b.y = 0x800;
    b.x = 0;
    b.z = 0;
    b.unk6 = 1;
    func_8015ACCC(8, 0x12, D_801604D4, a, b, 4, 0);
}

void func_801573C8(void) {
    TcPos p0;
    TcPos a;
    TcPos b;

    func_800AE120(D_801604C0);
    /* FAKE: the three stores on one source line make as1 order them z, y, x as the original does. T-9160 */
    a.x = 0x1F4; a.y = 0x12C; a.z = -0x8000;
    b.y = 0x800;
    b.x = 0;
    b.z = 0;
    b.unk6 = 0;
    func_8015ACCC(8, 0x12, D_801604D4, a, b, 4, 0x10);
    a.x = 0xFA; a.y = 0x12C; a.z = -0x7830;
    b.unk6 = 1;
    func_8015ACCC(8, 0x12, D_801604D4, a, b, 6, 0);
    a.x = 0; a.y = 0x12C; a.z = -0x7060;
    b.unk6 = 2;
    func_8015ACCC(8, 0x12, D_801604D4, a, b, 2, 0x50);
    a.x = -0xFA; a.y = 0x12C; a.z = -0x7830;
    b.unk6 = 3;
    func_8015ACCC(8, 0x12, D_801604D4, a, b, 6, 0);
    a.x = -0x1F4; a.y = 0x12C; a.z = -0x8000;
    b.unk6 = 4;
    func_8015ACCC(8, 0x12, D_801604D4, a, b, 4, 0x10);
}

void func_8015776C(s32 arg0) {
    TcDirections28 sp50;
    TcRotations14 sp3C;
    TcPos pos;

    sp50 = D_80160480;
    sp3C = D_801604A8;
    switch ((arg0 + D_8015EDB4)->unk7C) {
    case 0:
        (arg0 + D_8015EDB4)->unk78 += 0x100;
        if ((arg0 + D_8015EDB4)->unk78 >= -0x1D4B) {
            (arg0 + D_8015EDB4)->unk7A = 0;
            (arg0 + D_8015EDB4)->unk7C += 1;
        }
        break;
    case 1:
        (arg0 + D_8015EDB4)->unk74 += (s32) (sp50.pos[(arg0 + D_8015EDB4)->unk6A].x * (arg0 + D_8015EDB4)->unk7A) / 16;
        (arg0 + D_8015EDB4)->unk76 += (s32) (sp50.pos[(arg0 + D_8015EDB4)->unk6A].y * (arg0 + D_8015EDB4)->unk7A) / 16;
        (arg0 + D_8015EDB4)->unk78 += (s32) ((-(arg0 + D_8015EDB4)->unk7A << 8) + 0x1000) / 16;
        (arg0 + D_8015EDB4)->unk64 += 0x40;
        (arg0 + D_8015EDB4)->unk66 += sp3C.rotation[(arg0 + D_8015EDB4)->unk6A];
        if (((s16) (arg0 + D_8015EDB4)->unk7A % 20) == 0) {
            pos.x = (arg0 + D_8015EDB4)->unk74;
            pos.y = (arg0 + D_8015EDB4)->unk76;
            pos.z = (arg0 + D_8015EDB4)->unk78;
            func_8013DB80(pos, 0x80, 0xC0);
        }
        if ((arg0 + D_8015EDB4)->unk64 >= 0x400) {
            (arg0 + D_8015EDB4)->unk7C += 1;
        }
        break;
    case 2:
        (arg0 + D_8015EDB4)->unk74 += sp50.pos[(arg0 + D_8015EDB4)->unk6A].x;
        (arg0 + D_8015EDB4)->unk76 += sp50.pos[(arg0 + D_8015EDB4)->unk6A].y;
        if (((arg0 + D_8015EDB4)->unk76 < -0xFA0) || ((arg0 + D_8015EDB4)->unk74 < -0x1770) || ((arg0 + D_8015EDB4)->unk74 >= 0x1771)) {
            func_8015C208(arg0);
        }
        break;
    }
}

void func_80157AE0(void) {
    TcPos p0;
    TcPos pos;
    TcPos b;
    s32 t;
    s32 r;

    func_800AE120(D_801604C0);
    b.y = 0x800;
    b.x = 0;
    b.z = 0;
    b.unk6 = 0;
    r = func_800AE0D0();
    pos.x = (func_800AE0D0() % 5000 + D_8015EDB4->unk74 + r % 3000) - 0xFA0;
    pos.y = (func_800AE0D0() % 2) * 0x1F40 - 0xBB8;
    pos.z = -0x2742;
    t = func_8015ACCC(9, 0x13, D_801604D4, pos, b, 0x40, 0);
    pos.z = -0x2710;
    b.unk6 = t;
    func_8015ACCC(0xA, 0x14, D_801604D4, pos, b, 0x40, 0x80);
}

void func_80157CD4(s32 arg0) {
    switch ((arg0 + D_8015EDB4)->unk7C) {
    case 0:
        (arg0 + D_8015EDB4)->unk74 += (D_8015EDB4[0].unk74 - (arg0 + D_8015EDB4)->unk74) / 10;
        (arg0 + D_8015EDB4)->unk76 += (D_8015EDB4[0].unk76 - (arg0 + D_8015EDB4)->unk76) / 10;
        D_8015EDB4[arg0].unk78 = -0x2710;
        if ((arg0 + D_8015EDB4)->unk7A >= 0x3D) {
            (arg0 + D_8015EDB4)->unk7C += 1;
            D_8015EDB4[arg0].unk7A = 0;
        }
        break;
    case 1:
        if ((arg0 + D_8015EDB4)->unk7A >= 0x5B) {
            (arg0 + D_8015EDB4)->unk7C += 1;
            D_8015EDB4[arg0].unk7A = 0;
        }
        break;
    case 2:
        (arg0 + D_8015EDB4)->unk66 = 0x800 - ((arg0 + D_8015EDB4)->unk7A << 6);
        if ((arg0 + D_8015EDB4)->unk66 <= 0) {
            (arg0 + D_8015EDB4)->unk7C += 1;
            D_8015EDB4[arg0].unk7A = 0;
        }
        break;
    case 3:
        (arg0 + D_8015EDB4)->unk2 = 0xFD;
        break;
    }
}

void func_80157EA8(s32 arg0) {
    switch ((arg0 + D_8015EDB4)->unk7C) {
    case 0:
        (arg0 + D_8015EDB4)->unk74 += (D_8015EDB4[0].unk74 - (arg0 + D_8015EDB4)->unk74) / 10;
        (arg0 + D_8015EDB4)->unk76 += (D_8015EDB4[0].unk76 - (arg0 + D_8015EDB4)->unk76) / 10;
        D_8015EDB4[arg0].unk78 = -0x2742;
        if ((arg0 + D_8015EDB4)->unk7A >= 0x3D) {
            (arg0 + D_8015EDB4)->unk7C += 1;
            D_8015EDB4[arg0].unk7A = 0;
        }
        break;
    case 1:
        if ((arg0 + D_8015EDB4)->unk7A >= 0x3D) {
            (arg0 + D_8015EDB4)->unk7C += 1;
        }
        break;
    case 2:
        (arg0 + D_8015EDB4)->unk2 = 0xFE;
        break;
    }
}

void func_8015800C(void) {
    TcPos p0;
    TcPos p1;
    TcPos b;

    func_800AE120(D_801604C0);
    b.y = 0x800;
    b.x = 0;
    b.z = 0;
    if (D_801604D0 == 0) {
        if ((D_8015EDB4->unk74 < 0 && D_8015EDB4->unk76 < -0x2BC) || (D_8015EDB4->unk74 > 0 && D_8015EDB4->unk76 >= -0x2BB)) {
            b.unk6 = 0x11;
            D_801604D0 = 1;
            func_8015ACCC(0xC, 0x11, D_801604D4, D_801604DC, b, 2, 0x10);
            b.unk6 = 0;
            func_8015ACCC(0xC, 0x11, D_801604D4, D_801604DC, b, 2, 0x10);
            return;
        }
        b.unk6 = 1;
        D_801604D0 = 2;
        func_8015ACCC(0xC, 0x11, D_801604D4, D_801604DC, b, 2, 0x10);
        b.unk6 = 0x10;
        func_8015ACCC(0xC, 0x11, D_801604D4, D_801604DC, b, 2, 0x10);
        return;
    }
    if (D_801604D0 == 1) {
        b.unk6 = 0x11;
        func_8015ACCC(0xC, 0x11, D_801604D4, D_801604DC, b, 2, 0x10);
        b.unk6 = 0;
        func_8015ACCC(0xC, 0x11, D_801604D4, D_801604DC, b, 2, 0x10);
        return;
    }
    b.unk6 = 1;
    func_8015ACCC(0xC, 0x11, D_801604D4, D_801604DC, b, 2, 0x10);
    b.unk6 = 0x10;
    func_8015ACCC(0xC, 0x11, D_801604D4, D_801604DC, b, 2, 0x10);
}

/* bulk_m2c declarations (T-11190) */
void func_801589B0(void);
extern s32 subroutine_arg0;
extern s32 subroutine_arg1;
/* end bulk_m2c declarations */
void func_801585D0(s32 arg0) {
    s32 sp44;
    s32 sp40;
    s16 sp3C;
    s16 sp3A;
    s16 sp38;
    s32 sp30;
    TcActor *temp_v1;
    TcActor *temp_v1_2;
    TcActor *temp_v1_3;
    TcActor *temp_v1_4;
    TcActor *temp_v1_5;
    TcActor *temp_v1_6;
    TcActor *temp_v1_7;
    TcActor *var_v1;
    s16 temp_a0;
    s16 temp_v0;
    s16 temp_v0_2;
    s32 temp_t0;
    s32 var_at;

    temp_t0 = arg0 * 0x88;
    temp_v1 = &D_8015EDB4[arg0];
    temp_v0 = temp_v1->unk7C;
    switch (temp_v0) {
    case 0:
        if (temp_v1->unk6A & 1) {
            temp_v1->unk74 = -0x1770;
        } else {
            temp_v1->unk74 = 0x1770;
        }
        temp_v1_2 = &D_8015EDB4[arg0];
        if (temp_v1_2->unk6A & 0x10) {
            temp_v1_2->unk76 = -0x898;
        } else {
            temp_v1_2->unk76 = 0x320;
        }
        D_8015EDB4[arg0].unk78 = 0;
        temp_v1_3 = &D_8015EDB4[arg0];
        temp_v1_3->unk7C += 1;
        break;
    case 1:
        if (temp_v1->unk6A & 1) {
            temp_v1->unk74 = (temp_v1->unk7A * 0x1E) - 0x1770;
        } else {
            temp_v1->unk74 = 0x1770 - (temp_v1->unk7A * 0x1E);
        }
        temp_v1_4 = &D_8015EDB4[arg0];
        if (temp_v1_4->unk6A & 0x10) {
            temp_v1_4->unk76 = (temp_v1_4->unk7A * 5) - 0x898;
        } else {
            temp_v1_4->unk76 = 0x320 - (temp_v1_4->unk7A * 5);
        }
        temp_v1_5 = &D_8015EDB4[arg0];
        temp_v1_5->unk78 = -temp_v1_5->unk7A * 0x1E;
        temp_v1_6 = &D_8015EDB4[arg0];
        if (temp_v1_6->unk7A >= 0xC9) {
            temp_v1_6->unk7A = 0;
            temp_v1_7 = &D_8015EDB4[arg0];
            temp_v1_7->unk7C += 1;
        }
        break;
    case 2:
        temp_v0_2 = temp_v1->unk6A;
        switch (temp_v0_2) {
        case 0:
            sp40 = -0x1F4;
            sp44 = temp_v1->unk7A * 0x14;
            break;
        case 1:
            sp40 = -0x1F4;
            sp44 = temp_v1->unk7A * -0x14;
            break;
        case 16:
            sp40 = 0x1F4;
            sp44 = temp_v1->unk7A * -0x14;
            break;
        case 17:
            sp40 = 0x1F4;
            sp44 = temp_v1->unk7A * 0x14;
            break;
        }
        sp30 = temp_t0;
        D_8015EDB4[arg0].unk74 = (s16) ((s32) (func_800A0070(sp44) * sp40) >> 0xC);
        D_8015EDB4[arg0].unk76 = ((s32) (func_800A0140(sp44) * -sp40) >> 0xC) - 0x2BC;
        var_v1 = &D_8015EDB4[arg0];
        temp_a0 = var_v1->unk7A;
        var_at = temp_a0 < 0x19A;
        if ((temp_a0 % 20) == 0) {
            sp38 = var_v1->unk74;
            sp3A = var_v1->unk76;
            sp3C = var_v1->unk78;
            sp30 = temp_t0;
            func_8013DB80(subroutine_arg0, subroutine_arg1, 0x100, 0xC0);
            var_v1 = &D_8015EDB4[arg0];
            var_at = var_v1->unk7A < 0x19A;
        }
        if (var_at == 0) {
            var_v1->unk7C += 1;
        }
        break;
    case 3:
        sp30 = temp_t0;
        func_801589B0();
        D_8015EDB4[arg0].unk2 = 0xFE;
        break;
    }
}

void func_801589B0(void) {
    s32 i;

    for (i = 16; i < 63; i++) {
        if ((i + D_8015EDB4)->unk84[2] == 0xC) {
            (i + D_8015EDB4)->unk2 = 0xFE;
        }
    }
}

void func_80158AB0(void) {
    /* p0..p2 and r are the locals of this family (func_801563E4 passes all three positions and spills r
     * at the same offsets); the unused ones keep their stack slots. T-9020 */
    TcPos p0;
    TcPos p1;
    TcPos pos;
    s32 r;

    func_800AE120(D_801604C0);
    pos.y = 0x400;
    pos.x = 0;
    pos.z = 0;
    pos.unk6 = 0;
    func_8015ACCC(0xE, 0x15, D_801604D4, D_801604DC, pos, 0x10, 0xE0);
}

void func_80158B84(s32 arg0) {
    (arg0 + D_8015EDB4)->unk74 = 0x1B58 - (arg0 + D_8015EDB4)->unk7A * 0x14;
    D_8015EDB4[arg0].unk76 = -0x6A4;
    D_8015EDB4[arg0].unk78 = -0x4E20;
    if ((arg0 + D_8015EDB4)->unk74 < -0x1B58) {
        func_8015C208(arg0);
    }
    if ((arg0 + D_8015EDB4)->unk74 == -0x320) {
        func_80158CDC((arg0 + D_8015EDB4)->unk74, (arg0 + D_8015EDB4)->unk76, (arg0 + D_8015EDB4)->unk78);
    }
    if ((arg0 + D_8015EDB4)->unk74 == 0) {
        func_80158DBC((arg0 + D_8015EDB4)->unk74, (arg0 + D_8015EDB4)->unk76, (arg0 + D_8015EDB4)->unk78);
    }
    if ((arg0 + D_8015EDB4)->unk74 == 0x320) {
        func_80158CDC((arg0 + D_8015EDB4)->unk74, (arg0 + D_8015EDB4)->unk76, (arg0 + D_8015EDB4)->unk78);
    }
}

void func_80158CDC(s16 arg0, s16 arg1, s16 arg2) {
    /* the family's position locals and spill (see func_80158AB0); the unused ones keep their stack
     * slots. T-9020 */
    TcPos p1;
    TcPos pos;
    s32 r;

    func_800AE120(D_801604C0);
    pos.x = arg0;
    pos.y = arg1;
    pos.z = arg2;
    func_8015ACCC(0xF, 0x16, D_801604D4, pos, D_801604E4, 4, 0xD0);
}

void func_80158DBC(s16 arg0, s16 arg1, s16 arg2) {
    /* the family's position locals and spill (see func_80158AB0); the unused ones keep their stack
     * slots. T-9020 */
    TcPos p1;
    TcPos pos;
    s32 r;

    func_800AE120(D_801604C0);
    pos.x = arg0;
    pos.y = arg1;
    pos.z = arg2;
    if (!(func_800AE0D0() & 1)) {
        func_8015ACCC(0x10, 0x16, D_801604D4, pos, D_801604E4, 8, 0x60);
        return;
    }
    func_8015ACCC(0x10, 0x16, D_801604D4, pos, D_801604E4, 8, 0x70);
}

void func_80158F48(s32 arg0) {
    TcPos pos;

    if ((arg0 + D_8015EDB4)->unk7A % 20 == 0) {
        pos.x = (arg0 + D_8015EDB4)->unk74;
        pos.y = (arg0 + D_8015EDB4)->unk76;
        pos.z = (arg0 + D_8015EDB4)->unk78;
        func_8013DB80(pos, 0x80, 0x80);
    }
    (arg0 + D_8015EDB4)->unk76 += 0x20;
    if ((arg0 + D_8015EDB4)->unk76 > 0x400) {
        (arg0 + D_8015EDB4)->unk2 = 0xFE;
    }
    if ((arg0 + D_8015EDB4)->unk7C == 0) {
        /* FAKE: call and increment on one source line make as1 place the reload of the spilled index as the original does. T-9310 */
        func_8015C0A0(0, arg0); (arg0 + D_8015EDB4)->unk7C += 1;
    } else {
        func_8015C0A0(1, arg0);
    }
}
