#include "common.h"
#include "ovl/RPG_BAT.h"
#include "psx_types.h"

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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430", func_80154760);

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
