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
    main_state_set((s32) D_800E6280.unk_720);
    func_80042908((s32) D_800E6280.unk_721);
    func_80042940((s32) D_800E6280.unk_722);
}

void func_80132308(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80044750(0x201);
        cd_read_sectors(0x3F, 0x801B0000, 0x7C8C);
        D_800E6280.unk_110D += 1;
    } else if (cd_read_sync_status() & 1) {
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

s32 func_80132834(void) {
    s32 i;

    switch (D_800E6280.unk_110A) {
    case 0:
        D_800E6280.unk_03A = 0;
        func_8004E44C(0, (u32 *) D_80139218, D_80139200);
        srn_init(0, (u32 *) D_80139214, 0);
        srn_vram_set(0, 5, 0x580, 0xF0);
        Fade_In_Init();
        func_8004284C();
        break;
    case 1:
        if (func_800460EC() == 2) {
            func_8004284C();
        }
        break;
    case 2:
        Fade_In_Main();
        if (D_800E6280.unk_F88 & 0x60) {
            func_80044750(0x74);
            Fade_Out_Init();
            func_8004284C();
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        break;
    case 3:
        if (Fade_Out_Main() != 0) {
            /* FAKE: the first two of six iterations written out: the original stores them through absolute
             * addresses with one constant each, then runs the loop from 2; one 6-trip loop unrolls the other
             * way in this IDO. T-18200 */
            D_800E6280.unk_1228[5] = -1;
            D_800E6280.unk_1228[37] = -1;
            D_800E6280.unk_1228[6] = -1;
            D_800E6280.unk_1228[38] = -1;
            for (i = 2; i < 6; i++) {
                D_800E6280.unk_1228[i + 5] = -1;
                D_800E6280.unk_1228[i + 37] = -1;
            }
            func_80044750(0x201);
            func_80042808();
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        break;
    default:
        func_80046500();
        break;
    }
}

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
            ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_07 = 0x80;
        }
        D_800E6280.unk_036 = 0x40;
        D_800E6280.unk_037 = 0x40;
        D_800E6280.unk_038 = 0x80;
        return;
    }
    if (D_8013921C < 0x80) {
        D_8013921C += 4;
        for (i = 0; i < 64; i++) {
            ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_07 += 4;
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
            ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_07 = 0;
        }
        D_800E6280.unk_036 = 0;
        D_800E6280.unk_037 = 0;
        D_800E6280.unk_038 = 0;
        return;
    }
    if (D_8013921C > 0) {
        D_8013921C -= 4;
        for (i = 0; i < 64; i++) {
            ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_07 -= 4;
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

/* FAKE: s32 without return keeps the clamp accumulator in v0 and the original
 * split epilogue. Original return type is unconfirmed. T-14000. */
s32 func_801333B0(void) {
    s32 i;

    func_801330D0();
    D_80139AFC += 1;
    if ((D_800E6280.unk_F80 & 0x20) && D_80139AFC >= 4) {
        func_80135A48(D_8011ECF6, D_8011ECFA);
        func_801369F4(0x501);
        D_80139AF8 = 1;
        D_80139AFC = 0;
        D_80121531 = 7;
        D_80139AF4 += 2;
        func_801337E0();
        for (i = 0; i < 10; i++) {
            if (D_80139B08[i] == 2) {
                D_80139AF4 += 0x20;
            } else if (D_80139B08[i] == 1) {
                D_80139AF4 += 0x10;
            }
        }
        if (D_80139AF4 >= 0x50) {
            D_80139AF4 = 0x64;
        }
    }
}


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
    object = D_8013984C; status = D_80139B08; do {
        distance = func_80132A74((s32) object->unk_14.h[1], (s32) object->unk_18.h[1], (s32) D_8011ECF6, (s32) D_8011ECFA);
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

/* Moves object arg0: kind 4 drifts at a fixed step, the others steer by unk_2C and move at the
 * speed of class unk_28; unk_14/unk_18 are 16.16 positions, unk_1C the angle (4096 = 360 degrees).
 * Index form: a pointer local changes the spill slot and the frame. T-20920 */
void func_801339D4(s32 arg0) {

    D_8013984C[arg0].unk_24--;
    if (D_8013984C[arg0].unk_00 == 4) {
        D_8013984C[arg0].unk_14.w += (func_800A0140(D_8013984C[arg0].unk_1C) * 0x8000) >> 12;
        D_8013984C[arg0].unk_18.w += (func_800A0070(D_8013984C[arg0].unk_1C) * 0x8000) >> 12;
    } else {
        switch (D_8013984C[arg0].unk_28) {
        case 0:
            D_8013984C[arg0].unk_20 = 0xC000;
            break;
        case 1:
            D_8013984C[arg0].unk_20 = 0xF000;
            break;
        case 2:
            D_8013984C[arg0].unk_20 = 0x12000;
            break;
        case 3:
            D_8013984C[arg0].unk_20 = 0x15000;
            break;
        case 4:
            D_8013984C[arg0].unk_20 = 0x18000;
            break;
        case 5:
            D_8013984C[arg0].unk_20 = 0x1B000;
            break;
        case 6:
            D_8013984C[arg0].unk_20 = 0x1E000;
            break;
        case 7:
            D_8013984C[arg0].unk_20 = 0x21000;
            break;
        case 8:
            D_8013984C[arg0].unk_20 = 0x24000;
            break;
        }
        if (D_8013984C[arg0].unk_2C == 1) {
            if (D_8013984C[arg0].unk_24 % 40 < 10) {
                D_8013984C[arg0].unk_1C += 30;
            } else if (D_8013984C[arg0].unk_24 % 40 >= 20 && D_8013984C[arg0].unk_24 % 40 < 30) {
                D_8013984C[arg0].unk_1C -= 30;
            }
        }
        if (D_8013984C[arg0].unk_2C & 0x10) {
            D_8013984C[arg0].unk_1C += 20;
        }
        if (D_8013984C[arg0].unk_2C & 0x20) {
            D_8013984C[arg0].unk_1C -= 20;
        }
        switch (D_8013984C[arg0].unk_2C) {
        case 0x40:
            D_8013984C[arg0].unk_20 = 0xC000;
            D_8013984C[arg0].unk_1C += 0x100;
            break;
        case 0x41:
            D_8013984C[arg0].unk_20 = 0xC000;
            D_8013984C[arg0].unk_1C += 0x200;
            break;
        case 0x80:
            D_8013984C[arg0].unk_20 = 0xC000;
            D_8013984C[arg0].unk_1C -= 0x100;
            break;
        case 0x81:
            D_8013984C[arg0].unk_20 = 0xC000;
            D_8013984C[arg0].unk_1C -= 0x200;
            break;
        }
        if (D_8013984C[arg0].unk_1C >= 0x1000) {
            D_8013984C[arg0].unk_1C -= 0x1000;
        }
        if (D_8013984C[arg0].unk_1C < 0) {
            D_8013984C[arg0].unk_1C += 0x1000;
        }
        D_8013984C[arg0].unk_14.w += (D_8013984C[arg0].unk_20 * func_800A0140(D_8013984C[arg0].unk_1C)) >> 12;
        D_8013984C[arg0].unk_18.w += (D_8013984C[arg0].unk_20 * func_800A0070(D_8013984C[arg0].unk_1C)) >> 12;
    }
}

void func_80133CA8(void) {
    s32 i;
    s16 v;

    for (i = 0; i != 10; i++) {
        if (D_8013984C[i].unk_00 & 0x80) {
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_01 = 3;
            D_8013984C[i].unk_10 += 4;
            func_80133F00(i, 1);
        } else {
            D_8013984C[i].unk_10 += 1;
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_01 = 7;
            func_80133F00(i, 0);
        }
        if (D_8013984C[i].unk_10 >= 15) {
            D_8013984C[i].unk_10 = 0;
            D_8013984C[i].unk_0C++;
        }
        switch (D_8013984C[i].unk_00 & 0x7F) {
        case 0:
        default:
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_03 = 0;
            break;
        case 1:
        case 2:
        case 3:
            if (D_8013984C[i].unk_0C >= 6) {
                D_8013984C[i].unk_0C = 0;
            }
            v = D_80139A34[func_80134008(i)][D_8013984C[i].unk_0C];
            if (v & 0x80) {
                ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_03 = 0x85;
            } else {
                ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_03 = 0x84;
            }
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_18 = v & 0x7F;
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_43 = D_80139A2C[D_8013984C[i].unk_00 & 0x7F];
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_26 = D_8013984C[i].unk_14.h[1];
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_2A = D_8013984C[i].unk_18.h[1];
            break;
        case 4:
            if (D_8013984C[i].unk_0C >= 4) {
                D_8013984C[i].unk_0C = 0;
            }
            v = D_80139A34[func_80134008(i) + 8][D_8013984C[i].unk_0C];
            if (v & 0x80) {
                ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_03 = 0x85;
            } else {
                ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_03 = 0x84;
            }
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_18 = v & 0x7F;
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_43 = D_80139A2C[D_8013984C[i].unk_00 & 0x7F];
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_26 = D_8013984C[i].unk_14.h[1];
            ((DisplayActor44 *)D_8011ECD0)[128 + i].unk_2A = D_8013984C[i].unk_18.h[1];
            break;
        }
    }
}

void func_80133F00(arg0, arg1)
s32 arg0;
s32 arg1;
{
    u8 *rec;
    s32 j;
    EnObj *e;
    s16 x;
    s16 y;

    j = arg0 * 2; /* FAKE: the doubled index as its own variable keeps the original's sll-first multiply (T-9330) */
    rec = D_8011ECD0 + j * 0x44;
    rec[0x1983] = 4;
    rec[0x19C7] = 5;
    if (arg1 == 1) {
        e = &D_8013984C[arg0];
        if (e->unk_00 == 4) {
            if (!(e->unk_0C & 1)) {
                x = e->unk_14.h[1];
                y = e->unk_18.h[1];
                rec[0x1983] |= 0x80;
                rec[0x19C7] |= 0x80;
                *(s16 *) (rec + 0x19A6) = x - 0x10;
                *(s16 *) (rec + 0x19AA) = y - 7;
                *(s16 *) (rec + 0x19EA) = x + 0x10;
                *(s16 *) (rec + 0x19EE) = y + 7;
            }
        } else if (!(e->unk_0C & 1)) {
            x = e->unk_14.h[1];
            y = e->unk_18.h[1];
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

    v = D_8013984C[arg0].unk_1C;
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
    for (i = 0; i < 10; i++) if (D_8013984C[i].unk_14.h[1] < -0xC0 || D_8013984C[i].unk_14.h[1] >= 0xC1 || D_8013984C[i].unk_18.h[1] < -0x98 || D_8013984C[i].unk_18.h[1] >= 0x99) D_8013984C[i].unk_00 = 0;
}

void func_80134258(s32 arg0) {
    s32 n;
    s32 m;
    s32 r;
    s32 i;

    func_80134784(&n, &m);
    for (i = 0; i != 10; i++) {
        if (D_8013984C[i].unk_00 == 0 && n * 10 < func_800AE0D0() % 100) {
            r = func_800AE0D0();
            if (m == 0 && r % 100 >= 61) {
                D_8013984C[i].unk_00 = 4;
                m++;
            } else {
                D_8013984C[i].unk_00 = func_800AE0D0() % 3 + 1;
                n++;
            }
            r = func_800AE0D0() % 100;
            if (r < 20) {
                D_8013984C[i].unk_04 = 0;
            } else if (r < 60) {
                D_8013984C[i].unk_04 = 1;
            } else {
                D_8013984C[i].unk_04 = 2;
            }
            r = func_800AE0D0() % 100;
            if (r < 20) {
                D_8013984C[i].unk_08 = 0;
            } else if (r < 60) {
                D_8013984C[i].unk_08 = 1;
            } else {
                D_8013984C[i].unk_08 = 2;
            }
            if (D_8013984C[i].unk_00 == 4) {
                D_8013984C[i].unk_04 = 0;
            }
            D_8013984C[i].unk_0C = 0;
            D_8013984C[i].unk_10 = 0;
            func_801344F0(i, arg0);
        }
    }
}

void func_801344F0(s32 arg0, s32 arg1) {
    s32 temp_hi;
    s32 temp_hi_2;
    s32 temp_v1;
    EnObj *var_s0;

    if (arg1 == 1) {
        temp_v1 = func_800AE0D0() % 4;
        switch (temp_v1) {
        default:
            var_s0 = &D_8013984C[arg0];
            break;
        case 0:
            temp_hi = func_800AE0D0() % 320 - 0xA0;
            var_s0 = &D_8013984C[arg0];
            var_s0->unk_14.h[1] = temp_hi;
            var_s0->unk_18.h[1] = -0x96;
            var_s0->unk_1C = (s32) ((func_800AE0D0() % 1024) + 0x200);
            break;
        case 1:
            var_s0 = &D_8013984C[arg0];
            var_s0->unk_14.h[1] = 0xBE;
            var_s0->unk_18.h[1] = (s16) ((func_800AE0D0() % 240) - 0x78);
            var_s0->unk_1C = (s32) ((func_800AE0D0() % 1024) + 0x600);
            break;
        case 2:
            temp_hi_2 = func_800AE0D0() % 320 - 0xA0;
            var_s0 = &D_8013984C[arg0];
            var_s0->unk_14.h[1] = temp_hi_2;
            var_s0->unk_18.h[1] = 0x96;
            var_s0->unk_1C = (s32) ((func_800AE0D0() % 1024) + 0xA00);
            break;
        case 3:
            var_s0 = &D_8013984C[arg0];
            var_s0->unk_14.h[1] = -0xBE;
            var_s0->unk_18.h[1] = (s16) ((func_800AE0D0() % 240) - 0x78);
            var_s0->unk_1C = (s32) ((func_800AE0D0() % 1024) + 0xE00);
            break;
        }
    } else {
        var_s0 = &D_8013984C[arg0];
        var_s0->unk_14.h[1] = (s16) ((func_800AE0D0() % 320) - 0xA0);
        var_s0->unk_18.h[1] = (s16) ((func_800AE0D0() % 240) - 0x78);
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
        switch (D_8013984C[i].unk_00) {
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

/* When the countdown unk_24 of object arg0 has run out, picks its next unk_28 (the speed class of
 * func_801339D4) and unk_2C (the steering mode) at random. Index form as in T-18200. T-20920 */
void func_80134800(s32 arg0) {
    s32 r;

    if (D_8013984C[arg0].unk_24 <= 0) {
        r = func_800AE0D0();
        switch (D_8013984C[arg0].unk_08) {
        case 0:
            if (r % 100 < 80) {
                D_8013984C[arg0].unk_28 = 0;
            } else {
                D_8013984C[arg0].unk_28 = 1;
            }
            break;
        case 1:
            if (r % 100 < 10) {
                D_8013984C[arg0].unk_28 = 0;
            } else if (r % 100 < 90) {
                D_8013984C[arg0].unk_28 = 1;
            } else {
                D_8013984C[arg0].unk_28 = 2;
            }
            break;
        case 2:
            if (r % 100 < 20) {
                D_8013984C[arg0].unk_28 = 1;
            } else {
                D_8013984C[arg0].unk_28 = 2;
            }
            break;
        }
        r = func_800AE0D0();
        if (D_8013984C[arg0].unk_2C == 0 || D_8013984C[arg0].unk_2C == 1) {
            if (r % 100 < 25) {
                D_8013984C[arg0].unk_2C = 0;
            } else if (r % 100 < 40) {
                D_8013984C[arg0].unk_2C = 1;
            } else if (r % 100 < 70) {
                D_8013984C[arg0].unk_2C = 0x10;
            } else {
                D_8013984C[arg0].unk_2C = 0x20;
            }
        } else if (D_8013984C[arg0].unk_2C & 0x50) {
            if (r % 100 < 40) {
                D_8013984C[arg0].unk_2C = 0;
            } else if (r % 100 < 80) {
                D_8013984C[arg0].unk_2C = 1;
            } else {
                D_8013984C[arg0].unk_2C = 0x20;
            }
        } else if (D_8013984C[arg0].unk_2C & 0xA0) {
            if (r % 100 < 40) {
                D_8013984C[arg0].unk_2C = 0;
            } else if (r % 100 < 80) {
                D_8013984C[arg0].unk_2C = 1;
            } else {
                D_8013984C[arg0].unk_2C = 0x10;
            }
        }
        if ((D_8013984C[arg0].unk_2C & 0x10) || (D_8013984C[arg0].unk_2C & 0x20)) {
            r = func_800AE0D0() % 100;
            if (r < 33) {
                D_8013984C[arg0].unk_2C |= 1;
            } else if (r < 66) {
                D_8013984C[arg0].unk_2C |= 2;
            } else {
                D_8013984C[arg0].unk_2C |= 4;
            }
        }
        if (D_8013984C[arg0].unk_2C == 0 || D_8013984C[arg0].unk_2C == 1) {
            D_8013984C[arg0].unk_24 = func_800AE0D0() % 25 + 40;
            if (D_8013984C[arg0].unk_28 >= 4) {
                D_8013984C[arg0].unk_24 /= 4;
            }
        } else if (D_8013984C[arg0].unk_2C & 1) {
            D_8013984C[arg0].unk_24 = 25;
        } else if (D_8013984C[arg0].unk_2C & 2) {
            D_8013984C[arg0].unk_24 = 50;
        } else if (D_8013984C[arg0].unk_2C & 4) {
            D_8013984C[arg0].unk_24 = 100;
        }
    }
}

/* FAKE: the record pointer stays an expression; as a local it is spilled to another stack slot than the original's (T-3330, T-9330) */
#define EN_OBJ(i) (&D_8013984C[i])
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
    d = func_80132A74(EN_OBJ(arg0)->unk_14.h[1], EN_OBJ(arg0)->unk_18.h[1], D_8011ECF6, D_8011ECFA);
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
    EnObj *p;

    p = &D_8013984C[arg0];
    if (p->unk_28 < 3) {
        switch (p->unk_04) {
        case 1:
            if (arg1 < 0xC4) {
                if (!(func_800AE0D0() & 1)) {
                    p->unk_2C = 0;
                } else {
                    p->unk_2C = 1;
                }
                p->unk_28 = p->unk_08 * 2 + 4;
                p->unk_24 = func_800AE0D0() % 15 + 0x1E;
            }
            break;
        case 2:
            if (arg1 < 0x384) {
                if (!(func_800AE0D0() & 1)) {
                    p->unk_2C = 0;
                } else {
                    p->unk_2C = 1;
                }
                p->unk_28 = p->unk_08 * 2 + 4;
                p->unk_24 = func_800AE0D0() % 15 + 0x1E;
            }
            break;
        }
    }
}

/* The quadrant (0..3) of display record 0 as seen from object arg0, turned by the object's
 * direction func_80134008(arg0) (0..7). dir is unset for other values, so IDO keeps it in memory
 * (sp+0x2C); dx, dy, dir declared in this order give the original's slots. T-20920 */
s32 func_80135274(s32 arg0) {
    s32 dx;
    s32 dy;
    s32 dir;

    dx = D_8011ECF6 - D_8013984C[arg0].unk_14.h[1];
    dy = D_8011ECFA - D_8013984C[arg0].unk_18.h[1];
    switch (func_80134008(arg0)) {
    case 0:
        if (dy < dx) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (dy <= -dx) {
            dir += 2;
        }
        break;
    case 1:
        if (D_8013984C[arg0].unk_14.h[1] < D_8011ECF6) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (D_8011ECFA <= D_8013984C[arg0].unk_18.h[1]) {
            dir += 2;
        }
        break;
    case 2:
        if (-dx < dy) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (dy <= dx) {
            dir += 2;
        }
        break;
    case 3:
        if (D_8013984C[arg0].unk_18.h[1] < D_8011ECFA) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (D_8013984C[arg0].unk_14.h[1] <= D_8011ECF6) {
            dir += 2;
        }
        break;
    case 4:
        if (dx < dy) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (-dx <= dy) {
            dir += 2;
        }
        break;
    case 5:
        if (D_8011ECF6 < D_8013984C[arg0].unk_14.h[1]) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (D_8013984C[arg0].unk_18.h[1] <= D_8011ECFA) {
            dir += 2;
        }
        break;
    case 6:
        if (dy < -dx) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (dx <= dy) {
            dir += 2;
        }
        break;
    case 7:
        if (D_8011ECFA < D_8013984C[arg0].unk_18.h[1]) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (D_8011ECF6 <= D_8013984C[arg0].unk_14.h[1]) {
            dir += 2;
        }
        break;
    }
    return dir;
}

void func_801354E0(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 12; i++) {
        for (j = 0; j < 15; j++) {
            D_801392AC[j][i].unk_00.h[1] = 0;
            D_801392AC[j][i].unk_04.h[1] = 0;
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

void func_80135600(void) {
    s32 x;
    s32 y;
    s32 b;
    EnWords64 table;

    table = D_80139B28;
    b = (D_80139220 / 7) % 8;
    for (x = 0; x < 12; x++) {
        for (y = 0; y < 15; y++) {
            D_801392AC[y][x].unk_00.h[1] = table.value[(x + y * 3) % 8][(D_80139220 / 5) % 8];
            D_801392AC[y][x].unk_04.h[1] = table.value[(x * 3 + y) % 8][b];
        }
    }
}


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

/* Implicit int in the original (no return value): as1 then leaves the nop after the second test. T-18200 */
s32 func_80135A48(s32 arg0, s32 arg1) {
    if (D_80139278[D_801392A0] <= 0 || D_80139278[D_801392A0] >= 10) {
        D_801392A0++;
        if (D_801392A0 >= 10) {
            D_801392A0 = 0;
        }
        D_80139228[D_801392A0].unk_00.h[1] = arg0 + 0xA0;
        D_80139228[D_801392A0].unk_04.h[1] = arg1 + 0x78;
        D_80139278[D_801392A0] = 1;
    }
}

/* Pushes the grid points of ring arg0 (radius 5 * age) outwards or inwards in four bands. x and y are
 * 16.16 locals of which only the integer half is set; r0..r7 are the band bounds, and with x and y in
 * memory each declared local takes a stack slot (T-3330), which gives the original frame. T-20920 */
void func_80135ACC(s32 arg0) {
    s32 i;
    s32 j;
    s32 d;
    s32 r0;
    s32 r1;
    s32 r2;
    s32 r3;
    s32 r4;
    s32 r5;
    s32 r6;
    s32 r7;
    Fix32 x;
    Fix32 y;
    s32 px;
    s32 py;

    for (j = 0, py = -25; j < 12; j++, py += 25) {
        for (i = 0, px = -25; i < 15; i++, px += 25) {
            x.h[1] = px;
            y.h[1] = py;
            d = func_80132A74(D_80139228[arg0].unk_00.h[1], D_80139228[arg0].unk_04.h[1], x.h[1], y.h[1]);
            r0 = D_80139278[arg0] * 5;
            r1 = r0 + 5;
            if (r0 * r0 <= d) {
                if (d <= r1 * r1) {
                    D_801392AC[i][j].unk_00.w += (x.w - D_80139228[arg0].unk_00.w) * 2 / r0;
                    D_801392AC[i][j].unk_04.w += (y.w - D_80139228[arg0].unk_04.w) * 2 / r0;
                } else {
                    r2 = r0 + 10;
                    r3 = r0 + 15;
                    if (r2 * r2 < d) {
                        if (d <= r3 * r3) {
                            D_801392AC[i][j].unk_00.w -= (x.w - D_80139228[arg0].unk_00.w) * 3 / r2;
                            D_801392AC[i][j].unk_04.w -= (y.w - D_80139228[arg0].unk_04.w) * 3 / r2;
                        } else {
                            r4 = r0 + 20;
                            r5 = r0 + 25;
                            if (r4 * r4 < d) {
                                if (d <= r5 * r5) {
                                    D_801392AC[i][j].unk_00.w += (x.w - D_80139228[arg0].unk_00.w) * 3 / r4;
                                    D_801392AC[i][j].unk_04.w += (y.w - D_80139228[arg0].unk_04.w) * 3 / r4;
                                } else {
                                    r6 = r0 + 30;
                                    r7 = r0 + 35;
                                    if (r6 * r6 < d && d <= r7 * r7) {
                                        D_801392AC[i][j].unk_00.w -= (x.w - D_80139228[arg0].unk_00.w) * 2 / r6;
                                        D_801392AC[i][j].unk_04.w -= (y.w - D_80139228[arg0].unk_04.w) * 2 / r6;
                                    }
                                }
                            }; /* FAKE: empty statement (found with the permuter) gives the original's register order, j in s2 before the grid base and the constant 3; T-20920 */
                        }
                    }
                }
            }
        }
    }
}

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

/* A plain index loop: IDO unrolls it by two into the original's pointer walk (T-10850 had written the
 * unrolled form by hand, with a FAKE). T-20920 */
void func_80136A2C(void) {
    s32 i;

    if (D_800B3CFC != 0) {
        D_800B3CF8 = 0;
        D_800B3CFC = 0;
        D_80139B18++;
    }
    if (D_80139B18 > 0) {
        for (i = 0; i < 10; i++) {
            if (D_8013984C[i].unk_00 > 0 && D_8013984C[i].unk_00 < 4) {
                if (D_80139B18 < 3) {
                    D_8013984C[i].unk_00 = D_8013984C[i].unk_04 + 1;
                } else {
                    D_8013984C[i].unk_00 = 4;
                    D_8013984C[i].unk_04 = 0;
                }
            }
        }
    }
}
