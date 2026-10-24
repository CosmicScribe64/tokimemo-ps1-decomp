#define RPG_BAT_OVERRIDE_func_8013E97C /* Matched implicit-int call view keeps counter webs out of v0 (T-24027). */
#include "common.h"
#include "ovl/RPG_BAT.h"
#include "psx_types.h"

s32 func_8013E97C();

void func_80154430(void) {
    s16 *state;

    switch (D_8015EDAC) {
    case 0:
        strcpy(D_8015E74C, "　　　　　　　色素分解　　　　　　　");
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
            ((Actor *)actor_pool)[i + 136].draw_flags ^= 128;
        }
        if ((D_8015EDC4 & 0x100000) && D_8015EBB0 == 1000) {
            D_8015EBC0 = 0;
        } else {
            ((Actor *)actor_pool)[D_8015EC70 + 96].draw_flags ^= 128;
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
        ((Actor *)actor_pool)[D_8015EC70 + 96].draw_flags ^= 0x80;
        /* FAKE: unsigned 1 stays separate from the hoisted signed constant (T-17106, T-24027). */
        load_palette(D_8015EEE4, 0x18, 1U, 3, 0xA);
        load_palette(D_8015EEE8, 0x19, 1, 3, 1);
        load_palette(D_8015EEE8, 0x1A, 1, 3, 0);
        ((Actor *)actor_pool)[136].unk_43 = 0x18;
        ((Actor *)actor_pool)[139].unk_43 = 0x18;
        ((Actor *)actor_pool)[137].unk_43 = 0x19;
        ((Actor *)actor_pool)[140].unk_43 = 0x19;
        ((Actor *)actor_pool)[138].unk_43 = 0x1A;
        ((Actor *)actor_pool)[141].unk_43 = 0x1A;
        for (i = 0; i < 3; i++) {
            ((Actor *)actor_pool)[136 + i].quad_flags = 0x51000000;
            ((Actor *)actor_pool)[139 + i].quad_flags = 0x41000000;
            ((Actor *)actor_pool)[136 + i].brightness = 0x80;
            ((Actor *)actor_pool)[139 + i].brightness = 0xFF;
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
            ((Actor *)actor_pool)[136].draw_flags |= 1;
            for (i = 1; i < 6; i++) {
                ((Actor *)actor_pool)[i + 136].draw_flags |= 1;
            }
        }
        func_8013F0F4(0x50B, 1, 0);
        func_8014EBD0();
        break;
    case 1:
        v = rsin(D_8015EDB8 * 0x800 / 1000) * 8 >> 12;
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
        v = rsin(D_8015EDB8 * 0x800 / 1000) * 16 >> 12;
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
        v = rsin(D_8015EDB8 * 0x800 / 1000) * 32 >> 12;
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


/* The s32 return matches the implicit-int call in func_80154430 (a void definition is a redeclaration
 * error). */
extern s32 D_8015EC9C;
void func_8015528C(s32, s32, s32, s32, s32, s32, s32, s32, s32);

s32 func_80154DD8(void) {
    s32 x;
    s32 y;

    func_8013F0F4(0x50A, 0, 1);
    if (((D_8015EDB0.unk_00 == 1) && (D_8015EC9C >= 0x3D)) || (D_8015EDB0.unk_00 >= 2)) {
        func_8013F0F4(0x50C, 0xA, 7);
    }
    switch (D_8015EDB0.unk_00) {
    case 0:
        /* FAKE: D_8015EC9C written as element 60 of D_8015EBAC. as1 does not move a load above a store
         * through the same symbol, so the selector load stays in front of the stores, as in the original
         * (the counter and the selector are one int array there). Writing all five stores this way
         * matches too. T-25308 */
        (&D_8015EBAC)[60] = 1;
        D_8015EC74 = 0xA;
        D_8015EC78 = 0;
        D_8015EC7C = 0xA;
        D_8015EC80 = 0;
        /* FAKE: scalar-address view keeps the selector in $v1 and the constant 1 in $a3 (T-17114, T-24910;
         * 20 rows without it, T-25308). */
        switch (*(s32 *)&D_8015EBAC) {
        default:
            D_8015EC58.unk_10 = 0xB8;
            D_8015EC58.unk_04 = 0x48;
            D_8015EC58.unk_0C = -0x50;
            break;
        case 0:
            D_8015EC58.unk_10 = 0x70;
            D_8015EC58.unk_04 = 0x10;
            D_8015EC58.unk_0C = -0x30;
            break;
        case 1:
            D_8015EC58.unk_10 = 0xA8;
            D_8015EC58.unk_04 = 0x24;
            D_8015EC58.unk_0C = -0x1C;
            D_801215FD = 4;
            D_80121685 = 4;
            break;
        case 2:
            D_8015EC58.unk_10 = 0xB0;
            D_8015EC58.unk_04 = -0x18;
            D_8015EC58.unk_0C = -0x50;
            break;
        }
        func_8014EBD0();
        break;
    case 1:
        /* FAKE: the address views keep x and y as their own webs ($v0/$v1); with plain reads the
         * quotients are folded and spilled (281 rows). uopt then keeps -x and -y in temporaries that
         * cross the first call. T-25114, T-25308 */
        x = (*(s32 *)&D_8015EC58.unk_04 * D_8015EC9C) / D_8015EC58.unk_10;
        y = (*(s32 *)&D_8015EC58.unk_0C * D_8015EC9C) / D_8015EC58.unk_10;
        func_8015528C(D_8015EC9C, -x, -y, 0x30, 5, 5, 0x34, 0x35, 0xBF);
        func_8015528C(D_8015EC9C, -x, -y, 0xC, 0x14, 4, 0x32, 0x33, 0xDF);
        D_8015EC9C++;
        if (D_8015EC58.unk_10 < D_8015EC9C) {
            func_8014EBD0();
            D_8015EC9C = 1;
        }
        break;
    case 2:
        func_8015528C(D_8015EC58.unk_10, -D_8015EC58.unk_04, -D_8015EC58.unk_0C, 0x30, 5, 5, 0x34, 0x35, 0xBF);
        func_8015528C(D_8015EC58.unk_10, -D_8015EC58.unk_04, -D_8015EC58.unk_0C, 0xC, 0x14, 4, 0x32, 0x33, 0xDF);
        D_8015EC9C++;
        if (D_8015EC9C >= 0x79) {
            func_8014EBD0();
            D_8015EC9C = 1;
        }
        break;
    case 3:
        /* FAKE: same web-keeping view as case 1 (297 rows without it); the view on the counter also keeps
         * it as the first addu operand of y. T-25114, T-25308 */
        x = D_8015EC58.unk_04 - *(s32 *)&D_8015EC9C;
        y = *(s32 *)&D_8015EC9C + D_8015EC58.unk_0C;
        func_8015528C(D_8015EC58.unk_10, -x, -y, 0x30, 5, 5, 0x34, 0x35, 0xBF);
        func_8015528C(D_8015EC58.unk_10, -x, -y, 0xC, 0x14, 4, 0x32, 0x33, 0xDF);
        D_8015EC9C++;
        /* FAKE: D_8015EC5C read through the counter's symbol, so as1 keeps the test loads behind the
         * counter store, as in the original. T-25308 */
        if (((&D_8015EC9C)[-16] - D_8015EC9C) < (D_8015EC9C + D_8015EC58.unk_0C)) {
            func_8014EBA8();
        }
        break;
    }
}

void func_8015528C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32 *phase;
    s32 end_candidate;
    s32 phase_sum;
    s32 start;
    s32 end;
    s32 i;
    s32 color_phase;

    /* FAKE: T-25114, D_8015EBAC is an int array that the header declares as a scalar (as in T-9310);
     * its subscripts give the original's base-first addresses. The two integer address sums keep the
     * arm's base-first and the join's index-first `addu` in one register range ($s7). */
    if (arg3 < ((&D_8015EBAC)[arg6] / 10)) {
        phase = (s32 *)((s32)&D_8015EBAC + arg7 * 4);
        (&D_8015EBAC)[arg6] -= arg3 * 0xA;
        *phase -= 1;
        if (*phase < 0) {
            *phase = 2;
        }
    }
    phase = (s32 *)(arg7 * 4 + (s32)&D_8015EBAC);
    for (i = 0; i < 0x32; i++) {
        end_candidate = (&D_8015EBAC)[arg6] / 10 + (i * arg3);
        phase_sum = *phase + i;
        if (end_candidate < arg0) {
            start = (&D_8015EBAC)[arg6] / 10 + (i - 1) * arg3;
            /* FAKE: T-24522, T-25114: the `* 1` keeps this arm's remainder in its own range, as in the original. */
            color_phase = (phase_sum % 3) * 1;
            end = end_candidate;
            /* FAKE: T-25114, the shared clamp label keeps both `start < 0` tests as in the original (merging
             * them after the if/else costs 18 rows). */
            if (start < 0) {
                goto block_9;
            }
        } else {
            start = (&D_8015EBAC)[arg6] / 10 + (i - 1) * arg3;
            color_phase = phase_sum % 3;
            end = arg0;
            i = 0x64;
            if (start < 0) {
block_9:
                start = 0;
            }
        }
        switch (color_phase) {
        case 0:
            func_80155868(start, arg1 * start / arg0, arg2 * start / arg0, end, arg1 * end / arg0, arg2 * end / arg0, arg8 << 0x10, arg8 << 8, 4);
            break;
        case 1:
            func_80155868(start, arg1 * start / arg0, arg2 * start / arg0, end, arg1 * end / arg0, arg2 * end / arg0, arg8 << 8, arg8, 4);
            break;
        case 2:
            func_80155868(start, arg1 * start / arg0, arg2 * start / arg0, end, arg1 * end / arg0, arg2 * end / arg0, arg8, arg8 << 0x10, 4);
            break;
        }
    }
    dtd_on_tpage(0, 0, 4, arg5 - 4, 0);
    (&D_8015EBAC)[arg6] += arg4;
}

void func_80155868(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32 unused; /* FAKE: unused local, only its 4-byte frame slot above p is observed (DATE func_8014EF1C; T-17106) */
    POLY_G4 *p;

    arg0 = 0x2F - arg0;
    arg3 = 0x2FU - arg3; /* FAKE: the unsigned constant keeps this 0x2F out of the constant hoisted for arg0 (the original has two separate li); T-17106 */
    arg1 -= 0x1F;
    arg2 -= 0x1F;
    arg4 -= 0x1F; arg5 -= 0x1F; /* one line: as1 then stores arg5 before arg4 like the original */
    p = (POLY_G4 *)GetWorkBase(0x24, draw_buffer);
    func_8009F054(p);
    SetSemiTrans(p, 1);
    p->r0 = ((arg6 >> 16) & 0xFF); p->g0 = ((arg6 >> 8) & 0xFF); p->b0 = (arg6 & 0xFF);
    p->r1 = ((arg6 >> 16) & 0xFF); p->g1 = ((arg6 >> 8) & 0xFF); p->b1 = (arg6 & 0xFF);
    p->r2 = ((arg7 >> 16) & 0xFF); p->g2 = ((arg7 >> 8) & 0xFF); p->b2 = (arg7 & 0xFF);
    p->r3 = ((arg7 >> 16) & 0xFF); p->g3 = ((arg7 >> 8) & 0xFF); p->b3 = (arg7 & 0xFF);
    /* FAKE: each (unused - unused) adds one variable reference and no code. uopt ends a block after 20 references and
     * the original has its boundary after y1 (arg3 is reloaded for x2, arg0 for x0); without them it falls after y2 and
     * arg3 is reloaded early into $a3. T-17106 */
    p->x0 = game_state.screen_center_x[draw_buffer] + arg0 + (unused - unused);
    p->y0 = game_state.screen_center_y[draw_buffer] + arg1 + (unused - unused);
    p->x1 = game_state.screen_center_x[draw_buffer] + arg0 + (unused - unused);
    p->y1 = game_state.screen_center_y[draw_buffer] + arg2 + (unused - unused);
    p->x2 = game_state.screen_center_x[draw_buffer] + arg3;
    p->y2 = game_state.screen_center_y[draw_buffer] + arg4;
    p->x3 = game_state.screen_center_x[draw_buffer] + arg3;
    p->y3 = game_state.screen_center_y[draw_buffer] + arg5;
    AddPrim(&D_800E8CA0[(draw_buffer << 0xA) + (arg8 * 4)], (s32)p);
}
