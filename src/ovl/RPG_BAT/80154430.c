#define RPG_BAT_OVERRIDE_func_8013E97C /* Matched implicit-int call view keeps counter webs out of v0 (T-24027). */
#include "common.h"
#include "ovl/RPG_BAT.h"
#include "psx_types.h"

s32 func_8013E97C();

void func_80154430(void) {
    s16 *state;

    switch (D_8015EDAC) {
    case 0:
        func_800AE0F0(D_8015E74C, "　　　　　　　色素分解　　　　　　　");
        func_8014B738((s32) &D_8015E814, 1, 0x5A);
        func_80144480();
        func_8013EEAC(D_8015EBAC);
        func_80148F70();
        func_8014EBA8();
        func_8014EBA8();
        return;
    case 2:
        func_8013D418();
        return;
    case 3:
        func_8013E7C0(0x34, 3, 5, 1);
        func_8014EBA8();
        return;
    case 4:
        state = &D_80121438;
        switch (*state) {
        case 1:
            func_8013F0F4(0x509, 0, 2);
            break;
        case 3:
            func_8013F0F4(0x509, 0, 3);
            break;
        case 5:
            func_8013F0F4(0x509, 0, 4);
            break;
        case 7:
            func_8013F0F4(0x509, 0, 5);
            break;
        }
        func_8014EDBC(0x34);
        return;
    case 5:
        func_8013E7C0(0x34, 4, 1, 1);
        func_8014EBA8();
        return;
    case 6:
        func_80154DD8();
        return;
    case 7:
        func_80154760();
        return;
    case 8:
        func_8015461C();
        return;
    case 9:
        func_8014ED44(0x14);
        return;
    case 10:
        func_8013D59C();
        return;
    case 11:
        func_8014EBEC();

    default:
        return;
    }
}

void func_8015461C(void) {
    s32 i;

    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013EA30(255);
        func_8014EBD0();
        return;
    case 1:
        func_8014ED80(5);
        return;
    case 2:
        func_8013EA30(128);
        for (i = 0; i < 6; i++) {
            ((DisplayActor44 *)D_8011ECD0)[i + 136].unk_03 ^= 128;
        }
        if ((D_8015EDC4 & 0x100000) && D_8015EBB0 == 1000) {
            D_8015EBC0 = 0;
        } else {
            ((DisplayActor44 *)D_8011ECD0)[D_8015EC70 + 96].unk_03 ^= 128;
            func_8013D3E4();
        }
        func_8014EBA8();
        return;
    }
}

/* Fall-through s32 preserves the existing caller ABI. T-24027. */
s32 func_80154760(void) {
    s32 i;
    s32 v;

    switch (D_8015EDB0.unk_00) {
    case 0:
        /* FAKE: scalar address view selects the original v1 web (T-17114, T-24027). */
        switch (*(s32 *)&D_8015EBAC) {
        default:
            D_8015EC70 = 0x3B;
            break;
        case 0:
            if (D_8015EBCC[0] == 1) {
                D_8015ED14 = 1;
            }
            D_8015EC70 = 0x3B;
            break;
        case 1:
            if (D_8015EBCC[1] == 1) {
                D_8015ED14 = 1;
            }
            D_8015EC70 = 0x3C;
            break;
        case 2:
            if (D_8015EBCC[2] == 1) {
                D_8015ED14 = 1;
            }
            D_8015EC70 = 0x3D;
            break;
        }
        ((DisplayActor44 *)D_8011ECD0)[D_8015EC70 + 96].unk_03 ^= 0x80;
        /* FAKE: unsigned 1 stays separate from the hoisted signed constant (T-17106, T-24027). */
        load_palette(D_8015EEE4, 0x18, 1U, 3, 0xA);
        load_palette(D_8015EEE8, 0x19, 1, 3, 1);
        load_palette(D_8015EEE8, 0x1A, 1, 3, 0);
        ((DisplayActor44 *)D_8011ECD0)[136].unk_43 = 0x18;
        ((DisplayActor44 *)D_8011ECD0)[139].unk_43 = 0x18;
        ((DisplayActor44 *)D_8011ECD0)[137].unk_43 = 0x19;
        ((DisplayActor44 *)D_8011ECD0)[140].unk_43 = 0x19;
        ((DisplayActor44 *)D_8011ECD0)[138].unk_43 = 0x1A;
        ((DisplayActor44 *)D_8011ECD0)[141].unk_43 = 0x1A;
        for (i = 0; i < 3; i++) {
            ((DisplayActor44 *)D_8011ECD0)[136 + i].unk_38 = 0x51000000;
            ((DisplayActor44 *)D_8011ECD0)[139 + i].unk_38 = 0x41000000;
            ((DisplayActor44 *)D_8011ECD0)[136 + i].unk_07 = 0x80;
            ((DisplayActor44 *)D_8011ECD0)[139 + i].unk_07 = 0xFF;
            func_8013E97C(i + 0x28, D_8015EC28, D_8015EC2C);
            func_8013E97C(i + 0x2B, D_8015EC28, D_8015EC2C);
            switch (D_8015EDC4 & 0xFFFF0000) {
            case 0x100000:
                func_8013E810(i + 0x28, 5, 1, 1);
                func_8013E810(i + 0x2B, 5, 1, 1);
                break;
            case 0x200000:
                func_8013E810(i + 0x28, 6, 1, 1);
                func_8013E810(i + 0x2B, 6, 1, 1);
                break;
            case 0x400000:
                func_8013E810(i + 0x28, 7, 1, 1);
                func_8013E810(i + 0x2B, 7, 1, 1);
                break;
            default:
            case 0x8000000:
                func_8013E810(i + 0x28, 8, 1, 1);
                func_8013E810(i + 0x2B, 8, 1, 1);
                break;
            }
        }
        if (D_8015ED14 == 1) {
            ((DisplayActor44 *)D_8011ECD0)[136].unk_03 |= 1;
            for (i = 1; i < 6; i++) {
                ((DisplayActor44 *)D_8011ECD0)[i + 136].unk_03 |= 1;
            }
        }
        func_8013F0F4(0x50B, 1, 0);
        func_8014EBD0();
        break;
    case 1:
        v = func_800A0070(D_8015EDB8 * 0x800 / 1000) * 8 >> 12;
        if (v >= 8) {
            v = 7;
        }
        func_8013E97C(0x28, D_8015EC28 - v, D_8015EC2C);
        func_8013E97C(0x2B, D_8015EC28 - v, D_8015EC2C);
        func_8013E97C(0x2A, D_8015EC28 + v, D_8015EC2C);
        func_8013E97C(0x2D, D_8015EC28 + v, D_8015EC2C);
        D_8015EDB8 += 0xA;
        if (D_8015EDB8 >= 0x1F5) {
            D_8015EDB8 += 0xA;
        }
        if (D_8015EDB8 >= 0x3E9) {
            func_8013F0F4(0x50B, 1, 0);
            func_8014EBD0();
        }
        break;
    case 2:
        v = func_800A0070(D_8015EDB8 * 0x800 / 1000) * 16 >> 12;
        if (v >= 16) {
            v = 15;
        }
        func_8013E97C(0x28, D_8015EC28 + v, D_8015EC2C);
        func_8013E97C(0x2B, D_8015EC28 + v, D_8015EC2C);
        func_8013E97C(0x2A, D_8015EC28 - v, D_8015EC2C);
        func_8013E97C(0x2D, D_8015EC28 - v, D_8015EC2C);
        D_8015EDB8 += 0xA;
        if (D_8015EDB8 >= 0x1F5) {
            D_8015EDB8 += 0xA;
        }
        if (D_8015EDB8 >= 0x3E9) {
            func_8013F0F4(0x50B, 1, 0);
            func_8014EBD0();
        }
        break;
    case 3:
        v = func_800A0070(D_8015EDB8 * 0x800 / 1000) * 32 >> 12;
        if (v >= 32) {
            v = 31;
        }
        func_8013E97C(0x28, D_8015EC28 - v, D_8015EC2C);
        func_8013E97C(0x2B, D_8015EC28 - v, D_8015EC2C);
        func_8013E97C(0x2A, D_8015EC28 + v, D_8015EC2C);
        func_8013E97C(0x2D, D_8015EC28 + v, D_8015EC2C);
        D_8015EDB8 += 0xA;
        if (D_8015EDB8 >= 0x1F5) {
            D_8015EDB8 += 0xA;
        }
        if (D_8015EDB8 >= 0x3E9) {
            func_8014EBA8();
        }
        break;
    }
}


INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430", func_80154DD8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430", func_8015528C);

void func_80155868(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32 unused; /* FAKE: unused local, only its 4-byte frame slot above p is observed (DATE func_8014EF1C; T-17106) */
    POLY_G4 *p;

    arg0 = 0x2F - arg0;
    arg3 = 0x2FU - arg3; /* FAKE: the unsigned constant keeps this 0x2F out of the constant hoisted for arg0 (the original has two separate li); T-17106 */
    arg1 -= 0x1F;
    arg2 -= 0x1F;
    arg4 -= 0x1F; arg5 -= 0x1F; /* one line: as1 then stores arg5 before arg4 like the original */
    p = (POLY_G4 *)GetWorkBase(0x24, D_8011ECA0);
    func_8009F054(p);
    SetSemiTrans(p, 1);
    p->r0 = ((arg6 >> 16) & 0xFF); p->g0 = ((arg6 >> 8) & 0xFF); p->b0 = (arg6 & 0xFF);
    p->r1 = ((arg6 >> 16) & 0xFF); p->g1 = ((arg6 >> 8) & 0xFF); p->b1 = (arg6 & 0xFF);
    p->r2 = ((arg7 >> 16) & 0xFF); p->g2 = ((arg7 >> 8) & 0xFF); p->b2 = (arg7 & 0xFF);
    p->r3 = ((arg7 >> 16) & 0xFF); p->g3 = ((arg7 >> 8) & 0xFF); p->b3 = (arg7 & 0xFF);
    /* FAKE: each (unused - unused) adds one variable reference and no code. uopt ends a block after 20 references and
     * the original has its boundary after y1 (arg3 is reloaded for x2, arg0 for x0); without them it falls after y2 and
     * arg3 is reloaded early into $a3. T-17106 */
    p->x0 = D_800E6280.unk_014[D_8011ECA0] + arg0 + (unused - unused);
    p->y0 = D_800E6280.unk_018[D_8011ECA0] + arg1 + (unused - unused);
    p->x1 = D_800E6280.unk_014[D_8011ECA0] + arg0 + (unused - unused);
    p->y1 = D_800E6280.unk_018[D_8011ECA0] + arg2 + (unused - unused);
    p->x2 = D_800E6280.unk_014[D_8011ECA0] + arg3;
    p->y2 = D_800E6280.unk_018[D_8011ECA0] + arg4;
    p->x3 = D_800E6280.unk_014[D_8011ECA0] + arg3;
    p->y3 = D_800E6280.unk_018[D_8011ECA0] + arg5;
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + (arg8 * 4)], (s32)p);
}
