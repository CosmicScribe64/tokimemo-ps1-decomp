#include "common.h"
#include "ovl/EN_NICHI.h"

/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139200);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139204);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139208);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_8013920C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139210);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139214);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139218);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_8013921C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139220);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139224);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139228);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139278);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_801392A0);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_801392A4);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_801392A8);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_801392AC);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_8013930C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_801393CC);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_8013984C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139868);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_8013987C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_801398AC);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139A2C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139A34);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139AF4);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139AF8);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139AFC);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139B00);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139B04);
/* Ten status bytes followed by two bytes of end padding. T-10020. */
u8 D_80139B08[12] = { 0 };
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139B14);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139B18);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139B1C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139B20);
s32 D_80139B24 = 0;
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80132000.data", D_80139B28);

void func_80132000(void) {
    if (D_800E6280.unk_03E == 0x5F) {
        func_801320C0();
    } else {
        func_80136B10();
    }
}

void func_80132040(void) {
    D_801391E0 = 0x801E0400;
    D_801391E4 = 0x801E20F8;
    D_801391E8 = 0x801E2120;
    D_801391EC = 0x801E21C4;
    D_801391F0 = *(s16 *)0x801E5A10;
    D_801391F4 = 0x801B0000;
    D_801391F8 = 0x801E21CC;
    D_801391FC = 0x801E0408;
}

void func_801320C0(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        func_8013216C();
        return;
    case 1:
        func_80132834();
        return;
    case 2:
        func_80132450();
        func_8013275C();
        func_80042808();
        return;
    case 3:
        func_80132A0C();
        return;
    case 4:
        func_801362A4();
        return;
    case 5:
        func_80132278();
        /* fallthrough */
    default:
        return;
    }
}

void func_8013216C(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_801321EC();
        return;
    case 1:
        func_80132308();
        return;
    case 2:
        func_80132378();
        return;
    default:
    case 3:
        func_80042808();
        return;
    }
}
void func_801321EC(void) {
    func_80085E30(1, 0);
    func_80044750(0x7F);
    func_8006BC28(0);
    func_8006BD6C(0);
    func_800AE120(D_800E6280.unk_10F4);
    hizuke_disp_switch(0);
    func_80065F34(0);
    message_disp_switch(0);
    func_8006764C(0);
    func_8004284C();
    D_80139B14 = D_800E6280.unk_F6F;
    D_800E6280.unk_F6F = 1;
}

void func_80132278(void) {
    func_80048390();
    func_80048E78();
    func_8006BC28(0);
    D_800E6280.unk_036 = 0;
    D_800E6280.unk_037 = 0;
    D_800E6280.unk_038 = 0;
    func_800438F0(1);
    D_800E6280.unk_F6F = (s8) D_80139B14;
    func_80042878((s32) D_800E6280.unk_720);
    func_80042908((s32) D_800E6280.unk_721);
    func_80042940((s32) D_800E6280.unk_722);
}

void func_80132308(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80044750(0x201);
        func_80046318(0x3F, 0x801B0000, 0x7C8C);
        D_800E6280.unk_110D += 1;
    } else if (func_800460CC() & 1) {
        func_8004284C();
    }
}

void func_80132378(void) {
    load_palette(D_80139210, 0x10, 1, 1, 0);
    load_palette(D_80139210, 0x11, 1, 1, 1);
    load_palette(D_80139210, 0x12, 1, 1, 2);
    load_palette(D_80139210, 0x13, 1, 1, 3);
    load_palette(D_80139210, 0x1F, 1, 3, 0);
    load_palette(D_80139210, 0x1E, 1, 3, 1);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80132450);

void func_8013275C(void) {
    s32 index;
    s32 *word;

    for (index = 0; index < 10; index++) {
        func_80134258(index, 0);
    }
    func_801354E0();
    /* FAKE: reusing index for this reset keeps the first loop bound
     * as an immediate comparison instead of a saved register. T-10020. */
    index = 0;
    D_801392A0 = index;
    /* FAKE: the shared line preserves the pointer setup schedule. T-10020. */
    word = D_80139278; do {
        *word++ = 0;
    } while (word < &D_801392A0);
    D_80139B20 = 0;
    D_80139AF4 = 0;
    D_80139AF8 = 0;
    D_80139AFC = 0;
    D_80139B00 = 0;
    D_80139B04 = 0;
    for (index = 0; index < 10; index++) {
        D_80139B08[index] = 0;
    }
    D_80139B18 = 0;
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80132834);

void func_80132A0C(void) {
    s32 v;

    func_80136A2C();
    v = D_80139B1C - 1;
    D_80139B1C = v;
    if (v < 0) {
        D_80139B1C = 0;
    }
    func_80132B40();
    func_8013556C();
    func_80132ADC();
    func_80132D44();
    func_801338EC();
}

/* FAKE: permuter expression/layout spelling preserves target scheduling; original source unknown. T-9710. */
s32 func_80132A74(arg0, arg1, arg2, arg3)
s16 arg0;
s16 arg1;
s16 arg2;
s16 arg3;
{
    s32 dx;
    s32 dy;
    u32 x;
    u32 y;
    x = (dx = arg0 - arg2); dy = (y = arg1 - arg3); if (dx < 0) { x = -dx; } if (dy < 0) {
        y = -dy;
    }
    x *= x;
    y *= y;
    return x + y;
}

void func_80132ADC(void) {
    D_80139B24 += 1;
    if (D_80139B24 >= 0x36) {
        D_80139B24 = 0;
    }
    switch (D_80139B24) {
    case 0:
        D_8012158C = 0x20;
        break;
    case 0x1B:
        D_8012158C = 0x21;
        break;
    }
}

s32 func_80132B40(void) {
    s32 i;

    if (D_8013921C == 0x7C) {
        D_8013921C = 0x80;
        for (i = 0; i < 64; i++) {
            D_8011ECD0[0x1987 + i * 0x44] = 0x80;
        }
        D_800E6280.unk_036 = 0x40;
        D_800E6280.unk_037 = 0x40;
        D_800E6280.unk_038 = 0x80;
        return;
    }
    if (D_8013921C < 0x80) {
        D_8013921C += 4;
        for (i = 0; i < 64; i++) {
            D_8011ECD0[0x1987 + i * 0x44] += 4;
        }
        D_800E6280.unk_036 += 2;
        D_800E6280.unk_037 += 2;
        D_800E6280.unk_038 += 4;
    }
}

void func_80132C4C(void) {
    s32 i;

    if (D_8013921C == 4) {
        D_8013921C = 0;
        for (i = 0; i < 64; i++) {
            D_8011ECD0[0x1987 + i * 0x44] = 0;
        }
        D_800E6280.unk_036 = 0;
        D_800E6280.unk_037 = 0;
        D_800E6280.unk_038 = 0;
        return;
    }
    if (D_8013921C > 0) {
        D_8013921C -= 4;
        for (i = 0; i < 64; i++) {
            D_8011ECD0[0x1987 + i * 0x44] -= 4;
        }
        D_800E6280.unk_036 -= 2;
        D_800E6280.unk_037 -= 2;
        D_800E6280.unk_038 -= 4;
    }
}

void func_80132D44(void) {
    switch (D_80139AF8) {                           /* irregular */
    case 0:
        func_801333B0();
        return;
    case 1:
        func_8013358C();
        return;
    case 2:
        func_80132EE8();
        return;
    case 3:
        func_80132DC4();
        return;
    }
}

void func_80132DC4(void) {
    if (D_80139AFC == 0) {
        func_80044750(0x504);
    }
    D_80139AFC += 1;
    D_80121548 = 0x1E;
    func_80132E3C();
    if (D_80139AFC >= 0xB4) {
        D_80121533 = 0;
        D_80121531 = 3;
        func_80042808();
    }
}

void func_80132E3C(void) {
    func_801330D0();
    if (D_800E6280.unk_F80 & 0x20) {
        if (D_80121531 != 7) {
            func_80135A48(D_8011ECF6, D_8011ECFA);
            func_801369F4(0x501);
        }
        D_80121531 = 7;
        return;
    }
    if (D_80121531 != 3) {
        func_80135A48(D_8011ECF6, D_8011ECFA);
        func_801369F4(0x502);
    }
    D_80121531 = 3;
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80132EE8);

/* NON_MATCHING: T-10610, switch and prologue match (the two cursor words are D_8011ECF4[0..1], so their loads stay in the original order); the final clamp of the two s16 positions is forwarded through registers here, the original reloads after each store and keeps the base in $v1 (188 against 184 instructions) */
#ifdef NON_MATCHING
void func_801330D0(void) {
    s32 sel;
    s32 v;
    s32 d;
    volatile s16 *px;
    volatile s16 *py;

    sel = D_800E6280.unk_F80;
    if (sel & 0x40) {
        v = 3;
    } else {
        v = 2;
    }
    if (D_800E6280.unk_F74 == 1) {
        switch (sel & ~0x60) {
        case 0x1000:
            (&D_8011ECF4)[1] -= v << 16;
            break;
        case 0x2000:
            D_8011ECF4 += v << 16;
            break;
        case 0x4000:
            (&D_8011ECF4)[1] += v << 16;
            break;
        case 0x8000:
            D_8011ECF4 -= v << 16;
            break;
        case 0x3000:
            d = v * 0xB505;
            D_8011ECF4 += d;
            (&D_8011ECF4)[1] -= d;
            break;
        case 0x6000:
            d = v * 0xB505;
            D_8011ECF4 += d;
            (&D_8011ECF4)[1] += d;
            break;
        case 0xC000:
            d = v * 0xB505;
            D_8011ECF4 -= d;
            (&D_8011ECF4)[1] += d;
            break;
        case 0x9000:
            d = v * 0xB505;
            D_8011ECF4 -= d;
            (&D_8011ECF4)[1] -= d;
            break;
        }
    }
    px = (volatile s16 *)(D_8011ECD0 + 0x26);
    py = (volatile s16 *)(D_8011ECD0 + 0x2A);
    if (*px < -0x8F) {
        *px = -0x90;
    }
    if (*px >= 0x90) {
        *px = 0x90;
    }
    if (*py < -0x67) {
        *py = -0x68;
    }
    if (*py >= 0x68) {
        *py = 0x68;
    }
    D_80121556 = *px;
    D_8012155A = *py;
}
#else
INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_801330D0);
#endif

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_801333B0);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_8013358C);

void func_801337E0(void) {
    s32 distance;
    s32 radius;
    u8 *status;
    EnObj *object;

    radius = ((s32) D_80139B00 / 2) + 5;
    if (radius >= 0xB) {
        radius = 0xA;
    }
    radius *= radius;
    /* FAKE: this shared line preserves the loop-setup load schedule. T-10020. */
    object = (EnObj *)D_8013984C; status = D_80139B08; do {
        distance = func_80132A74((s32) object->unk_16, (s32) object->unk_1A, (s32) D_8011ECF6, (s32) D_8011ECFA);
        if (object->unk_00 != 0) {
            if (distance < radius) {
                *status = 2;
            } else if (distance < 0x100) {
                *status = 1;
            } else {
                *status = 0;
            }
        } else {
            *status = 0;
        }
        status += 1;
        object++;
    } while (status != D_80139B12);
}

void func_801338EC(void) {
    func_801340CC();
    func_80134258(1);
    func_80133924();
    func_80133CA8();
}

void func_80133924(void) {
    s32 i;
    s32 *p;
    u8 n;

    p = (s32 *) D_8013984C;
    for (i = 0, n = 10; i != n; i++) {
        if (!(p[0] & 0x80)) {
            if ((D_80121531 == 7) && (p[1] != 0)) {
                func_80134C1C(i);
                func_801339D4(i);
            } else {
                func_80134800(i);
                func_801339D4(i);
            }
        }
        p += 12;
    }
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_801339D4);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80133CA8);

void func_80133F00(arg0, arg1)
s32 arg0;
s32 arg1;
{
    u8 *rec;
    s32 j;
    u8 *e;
    s16 x;
    s16 y;

    j = arg0 * 2; /* FAKE: the doubled index as its own variable keeps the original's sll-first multiply (T-9330) */
    rec = D_8011ECD0 + j * 0x44;
    rec[0x1983] = 4;
    rec[0x19C7] = 5;
    if (arg1 == 1) {
        e = D_8013984C + arg0 * 0x30;
        if (*(s32 *) e == 4) {
            if (!(*(s32 *) (e + 0xC) & 1)) {
                x = *(s16 *) (e + 0x16);
                y = *(s16 *) (e + 0x1A);
                rec[0x1983] |= 0x80;
                rec[0x19C7] |= 0x80;
                *(s16 *) (rec + 0x19A6) = x - 0x10;
                *(s16 *) (rec + 0x19AA) = y - 7;
                *(s16 *) (rec + 0x19EA) = x + 0x10;
                *(s16 *) (rec + 0x19EE) = y + 7;
            }
        } else if (!(*(s32 *) (e + 0xC) & 1)) {
            x = *(s16 *) (e + 0x16);
            y = *(s16 *) (e + 0x1A);
            rec[0x1983] |= 0x80;
            rec[0x19C7] |= 0x80;
            *(s16 *) (rec + 0x19A6) = x - 0x10;
            *(s16 *) (rec + 0x19AA) = y - 7;
            *(s16 *) (rec + 0x19EA) = x + 0x10;
            *(s16 *) (rec + 0x19EE) = y + 7;
        }
    }
}

s32 func_80134008(s32 arg0) {
    s32 v;

    v = D_80139868[arg0].val;
    if (v < 0x100) {
        return 0;
    }
    if (v < 0x300) {
        return 1;
    }
    if (v < 0x500) {
        return 2;
    }
    if (v < 0x700) {
        return 3;
    }
    if (v < 0x900) {
        return 4;
    }
    if (v < 0xB00) {
        return 5;
    }
    if (v < 0xD00) {
        return 6;
    }
    if (v < 0xF00) {
        return 7;
    }
    return 0;
}

void func_801340CC(void) {
    s32 i;
    s32 x;
    s32 y;

    /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
    for (i = 0; i < 10; i++) if (*(s16 *) &D_8013984C[i * 0x30 + 0x16] < -0xC0 || *(s16 *) &D_8013984C[i * 0x30 + 0x16] >= 0xC1 || *(s16 *) &D_8013984C[i * 0x30 + 0x1A] < -0x98 || *(s16 *) &D_8013984C[i * 0x30 + 0x1A] >= 0x99) *(s32 *) &D_8013984C[i * 0x30] = 0;
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80134258);

void func_801344F0(s32 arg0, s32 arg1) {
    s32 temp_hi;
    s32 temp_hi_2;
    s32 temp_v1;
    EnObj *var_s0;

    if (arg1 == 1) {
        temp_v1 = func_800AE0D0() % 4;
        switch (temp_v1) {
        default:
            var_s0 = (EnObj *)(D_8013984C + arg0 * 0x30);
            break;
        case 0:
            temp_hi = func_800AE0D0() % 320 - 0xA0;
            var_s0 = (EnObj *)(D_8013984C + arg0 * 0x30);
            var_s0->unk_16 = temp_hi;
            var_s0->unk_1A = -0x96;
            var_s0->unk_1C = (s32) ((func_800AE0D0() % 1024) + 0x200);
            break;
        case 1:
            var_s0 = (EnObj *)(D_8013984C + arg0 * 0x30);
            var_s0->unk_16 = 0xBE;
            var_s0->unk_1A = (s16) ((func_800AE0D0() % 240) - 0x78);
            var_s0->unk_1C = (s32) ((func_800AE0D0() % 1024) + 0x600);
            break;
        case 2:
            temp_hi_2 = func_800AE0D0() % 320 - 0xA0;
            var_s0 = (EnObj *)(D_8013984C + arg0 * 0x30);
            var_s0->unk_16 = temp_hi_2;
            var_s0->unk_1A = 0x96;
            var_s0->unk_1C = (s32) ((func_800AE0D0() % 1024) + 0xA00);
            break;
        case 3:
            var_s0 = (EnObj *)(D_8013984C + arg0 * 0x30);
            var_s0->unk_16 = -0xBE;
            var_s0->unk_1A = (s16) ((func_800AE0D0() % 240) - 0x78);
            var_s0->unk_1C = (s32) ((func_800AE0D0() % 1024) + 0xE00);
            break;
        }
    } else {
        var_s0 = (EnObj *)(D_8013984C + arg0 * 0x30);
        var_s0->unk_16 = (s16) ((func_800AE0D0() % 320) - 0xA0);
        var_s0->unk_1A = (s16) ((func_800AE0D0() % 240) - 0x78);
        var_s0->unk_1C = (s32) (func_800AE0D0() % 4096);
    }
    var_s0->unk_24 = 1;
    if (var_s0->unk_1C >= 0x1000) {
        var_s0->unk_1C -= 0x1000;
    }
    if (var_s0->unk_1C < 0) {
        var_s0->unk_1C += 0x1000;
    }
}

void func_80134784(s32 *arg0, s32 *arg1) {
    s32 i;

    *arg0 = 0;
    *arg1 = 0;
    for (i = 0; i < 10; i++) {
        switch (*(s32 *)(D_8013984C + i * 0x30)) {
        case 0:
            break;
        case 1:
        case 2:
        case 3:
            (*arg0)++;
            break;
        case 4:
            (*arg1)++;
            break;
        }
    }
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80134800);

/* FAKE: the record pointer stays an expression; as a local it is spilled to another stack slot than the original's (T-3330, T-9330) */
#define EN_OBJ(i) ((EnObj *) (D_8013984C + (i) * 0x30))
void func_80134C1C(arg0)
s32 arg0;
{
    s32 d;

    if (((EN_OBJ(arg0)->unk_2C & 0x40) || (EN_OBJ(arg0)->unk_2C & 0x80)) && EN_OBJ(arg0)->unk_24 <= 0) {
        EN_OBJ(arg0)->unk_28 = EN_OBJ(arg0)->unk_08 * 2 + 4;
        EN_OBJ(arg0)->unk_24 = func_800AE0D0() % 15 + 0x1E;
        if (!(func_800AE0D0() & 1)) {
            EN_OBJ(arg0)->unk_2C = 0;
        } else {
            EN_OBJ(arg0)->unk_2C = 1;
        }
    }
    d = func_80132A74(EN_OBJ(arg0)->unk_16, EN_OBJ(arg0)->unk_1A, D_8011ECF6, D_8011ECFA);
    switch (func_80135274(arg0)) {
    case 0:
        func_80134D74(arg0, d);
        break;
    case 1:
        func_80134F2C(arg0, d);
        break;
    case 2:
        func_80135044(arg0, d);
        break;
    case 3:
        func_8013515C(arg0, d);
        break;
    }
    func_80134800(arg0);
}

void func_80134D74(s32 arg0, s32 arg1) {
    s32 temp_v0;
    EnObj *temp_s0;

    temp_s0 = EN_OBJ(arg0);
    if (temp_s0->unk_28 < 3) {
        temp_v0 = temp_s0->unk_04;
        switch (temp_v0) {
        case 1:
            if (arg1 < 0x844) {
                temp_s0->unk_28 = (s32) ((temp_s0->unk_08 * 2) + 4);
                temp_s0->unk_24 = (s32) ((func_800AE0D0() % 15) + 0x1E);
                if (!(func_800AE0D0() & 1)) {
                    temp_s0->unk_2C = 0;
                } else {
                    temp_s0->unk_2C = 1;
                }
                if (arg1 >= 0x191) {
                    temp_s0->unk_24 = 8;
                    if (!(func_800AE0D0() & 1)) {
                        temp_s0->unk_2C = 0x80;
                    } else {
                        temp_s0->unk_2C = 0x40;
                    }
                }
            }
            break;
        case 2:
            if (arg1 < 0xF04) {
                temp_s0->unk_28 = (s32) ((temp_s0->unk_08 * 2) + 4);
                temp_s0->unk_24 = (s32) ((func_800AE0D0() % 15) + 0x1E);
                if (!(func_800AE0D0() & 1)) {
                    temp_s0->unk_2C = 0;
                } else {
                    temp_s0->unk_2C = 1;
                }
                if (arg1 >= 0x191) {
                    temp_s0->unk_24 = 4;
                    if (!(func_800AE0D0() & 1)) {
                        temp_s0->unk_2C = 0x81;
                    } else {
                        temp_s0->unk_2C = 0x41;
                    }
                }
            }
            break;
        }
        if (temp_s0->unk_1C >= 0x1000) {
            temp_s0->unk_1C -= 0x1000;
        }
        if (temp_s0->unk_1C < 0) {
            temp_s0->unk_1C += 0x1000;
        }
    }
}

void func_80134F2C(s32 arg0, s32 arg1) {

    if (EN_OBJ(arg0)->unk_28 < 3) {
        switch (EN_OBJ(arg0)->unk_04) {
        case 1:
            if (arg1 < 0x384) {
                EN_OBJ(arg0)->unk_28 = EN_OBJ(arg0)->unk_08 * 2 + 4;
                EN_OBJ(arg0)->unk_2C = 0x80;
                if (!(func_800AE0D0() & 1)) {
                    EN_OBJ(arg0)->unk_24 = 1;
                } else {
                    EN_OBJ(arg0)->unk_24 = 2;
                }
            }
            break;
        case 2:
            if (arg1 < 0x844) {
                EN_OBJ(arg0)->unk_28 = EN_OBJ(arg0)->unk_08 * 2 + 4;
                EN_OBJ(arg0)->unk_2C = 0x81;
                if (!(func_800AE0D0() & 1)) {
                    EN_OBJ(arg0)->unk_24 = 1;
                } else {
                    EN_OBJ(arg0)->unk_24 = 2;
                }
            }
            break;
        }
        if (EN_OBJ(arg0)->unk_1C >= 0x1000) {
            EN_OBJ(arg0)->unk_1C -= 0x1000;
        }
        if (EN_OBJ(arg0)->unk_1C < 0) {
            EN_OBJ(arg0)->unk_1C += 0x1000;
        }
    }
}

void func_80135044(s32 arg0, s32 arg1) {

    if (EN_OBJ(arg0)->unk_28 < 3) {
        switch (EN_OBJ(arg0)->unk_04) {
        case 1:
            if (arg1 < 0x384) {
                EN_OBJ(arg0)->unk_28 = EN_OBJ(arg0)->unk_08 * 2 + 4;
                EN_OBJ(arg0)->unk_2C = 0x40;
                if (!(func_800AE0D0() & 1)) {
                    EN_OBJ(arg0)->unk_24 = 1;
                } else {
                    EN_OBJ(arg0)->unk_24 = 2;
                }
            }
            break;
        case 2:
            if (arg1 < 0x844) {
                EN_OBJ(arg0)->unk_28 = EN_OBJ(arg0)->unk_08 * 2 + 4;
                EN_OBJ(arg0)->unk_2C = 0x41;
                if (!(func_800AE0D0() & 1)) {
                    EN_OBJ(arg0)->unk_24 = 1;
                } else {
                    EN_OBJ(arg0)->unk_24 = 2;
                }
            }
            break;
        }
        if (EN_OBJ(arg0)->unk_1C >= 0x1000) {
            EN_OBJ(arg0)->unk_1C -= 0x1000;
        }
        if (EN_OBJ(arg0)->unk_1C < 0) {
            EN_OBJ(arg0)->unk_1C += 0x1000;
        }
    }
}

void func_8013515C(s32 arg0, s32 arg1) {
    u8 *p;

    p = D_8013984C + arg0 * 0x30;
    if (*(s32 *) (p + 0x28) < 3) {
        switch (*(s32 *) (p + 4)) {
        case 1:
            if (arg1 < 0xC4) {
                if (!(func_800AE0D0() & 1)) {
                    *(s32 *) (p + 0x2C) = 0;
                } else {
                    *(s32 *) (p + 0x2C) = 1;
                }
                *(s32 *) (p + 0x28) = *(s32 *) (p + 8) * 2 + 4;
                *(s32 *) (p + 0x24) = func_800AE0D0() % 15 + 0x1E;
            }
            break;
        case 2:
            if (arg1 < 0x384) {
                if (!(func_800AE0D0() & 1)) {
                    *(s32 *) (p + 0x2C) = 0;
                } else {
                    *(s32 *) (p + 0x2C) = 1;
                }
                *(s32 *) (p + 0x28) = *(s32 *) (p + 8) * 2 + 4;
                *(s32 *) (p + 0x24) = func_800AE0D0() % 15 + 0x1E;
            }
            break;
        }
    }
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80135274);

void func_801354E0(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 12; i++) {
        for (j = 0; j < 15; j++) {
            D_801392AC[j][i].unk_02 = 0;
            D_801392AC[j][i].unk_06 = 0;
        }
    }
    D_801392A4 = 0;
    D_801392A8 = 0;
}

void func_8013556C(void) {
    D_80139220 += 1;
    func_80135600();
    D_80139224 += 3;
    if (D_80139224 >= 0x1000) {
        D_80139224 = 0;
    }
    D_801392A4 = (func_800A0070(D_80139224) * 8) >> 12;
    D_801392A8 = (func_800A0140(D_80139224) * 0x10) >> 12;
    func_801359D0();
    func_80135F54();
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80135600);

void func_801359D0(void) {
    s32 i;

    for (i = 0; i != 10; i++) {
        if (D_80139278[i] != 0) {
            D_80139278[i] += 1;
            func_80135ACC(i);
            if (D_80139278[i] >= 0x51) {
                D_80139278[i] = 0;
            }
        }
    }
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80135A48);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80135ACC);

void func_80135F54(void) {
    s32 i;
    s32 j;

    for (j = 0; j < 11; j++) {
        for (i = 0; i < 14; i++) {
            func_80135FBC(i, j);
        }
    }
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80135FBC);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_801362A4);

void func_80136700(void) {
    s32 pad; /* FAKE: unused word above poly, puts poly at sp+0x30 as in the original (T-3330 slot rule) */
    POLY_F4 *poly;

    poly = func_800490F0(0x18, D_8011ECA0);
    func_8009F02C(poly);
    func_8009EF84(poly, 1);
    poly->r0 = D_80139B20;
    poly->g0 = D_80139B20;
    poly->b0 = D_80139B20;
    poly->x0 = D_800E6280.unk_014[D_8011ECA0] - 0x58;
    poly->y0 = D_800E6280.unk_018[D_8011ECA0] - 0x40;
    poly->x1 = D_800E6280.unk_014[D_8011ECA0] + 0x58;
    poly->y1 = D_800E6280.unk_018[D_8011ECA0] - 0x40;
    poly->x2 = D_800E6280.unk_014[D_8011ECA0] - 0x58;
    poly->y2 = D_800E6280.unk_018[D_8011ECA0] + 0x30;
    poly->x3 = D_800E6280.unk_014[D_8011ECA0] + 0x58;
    poly->y3 = D_800E6280.unk_018[D_8011ECA0] + 0x30;
    func_8009EED0(D_8011ECA0 * 0x400 + 8 + D_800E8CA0, poly);
    func_800494BC(0, 0, 2, 2, 0);
    if (D_80139B20 == 0) {
        D_801215FF = 0x80;
    }
    D_80139B20 += 2;
    D_80121603 = D_80139B20;
    if (D_80139B20 >= 0x80) {
        *(u8 *) &D_80121603 = 0x80; /* FAKE: unsigned view keeps the constant 0x80 (an s8 store would load -128), T-9330 */
    }
    if (D_80139B20 == 0xA0) {
        D_80121634 = 0x01000000;
        D_8012163F = 0x11;
        func_8004284C();
    }
}

void func_80136904(s32 arg0, s32 arg1) {
    D_80121313 = 0x84;
    D_80121357 = 0x84;
    D_8012139B = 0x84;
    D_801213DF = 0x84;
    D_80121328 = (arg0 % 10) + 0x23;
    D_8012136C = (arg0 / 10) + 0x23;
    if (arg0 < 0xA) {
        D_80121357 = 0;
    }
    D_801213B0 = (arg1 % 10) + 0x23;
    D_801213F4 = (arg1 / 10) + 0x23;
    if (arg1 < 0xA) {
        D_801213DF = 0;
    }
}

void func_801369F4(s32 arg0) {
    if (D_80139B1C <= 0) {
        D_80139B1C = 1;
        func_80044750(arg0 & 0xFFFF);
    }
}

/* NON_MATCHING: T-10810, rolled 5-trip loop matches; the end-pointer constant (a1) and the constant 4 (a2) swap registers (regdiff: numbering order of the two new temporaries) */
#ifdef NON_MATCHING
void func_80136A2C(void) {
    s32 i;

    if (D_800B3CFC != 0) {
        D_800B3CF8 = 0;
        D_800B3CFC = 0;
        D_80139B18 += 1;
    }
    if (D_80139B18 > 0) {
        for (i = 0; i < 5; i++) {
            if (((EnObj *)D_8013984C)[i * 2].unk_00 > 0 && ((EnObj *)D_8013984C)[i * 2].unk_00 < 4) {
                if (D_80139B18 < 3) {
                    ((EnObj *)D_8013984C)[i * 2].unk_00 = ((EnObj *)D_8013984C)[i * 2].unk_04 + 1;
                } else {
                    ((EnObj *)D_8013984C)[i * 2].unk_00 = 4;
                    ((EnObj *)D_8013984C)[i * 2].unk_04 = 0;
                }
            }
            if (((EnObj *)D_8013984C)[i * 2 + 1].unk_00 > 0 && ((EnObj *)D_8013984C)[i * 2 + 1].unk_00 < 4) {
                if (D_80139B18 < 3) {
                    ((EnObj *)D_8013984C)[i * 2 + 1].unk_00 = ((EnObj *)D_8013984C)[i * 2 + 1].unk_04 + 1;
                } else {
                    ((EnObj *)D_8013984C)[i * 2 + 1].unk_00 = 4;
                    ((EnObj *)D_8013984C)[i * 2 + 1].unk_04 = 0;
                }
            }
        }
    }
}
#else
INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI/80132000", func_80136A2C);
#endif
