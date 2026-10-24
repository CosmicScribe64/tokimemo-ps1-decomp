/* girl_action_yuko.c: src/ovl/RPG_BAT/801563A0.c:86: girl 8 action; body 87: switch (D_8015EE04) {; 89: func_8013E7C0(0x1D, 5, 5, 1); partner-id dispatch src/ovl/RPG_BAT/801439E0.c:24. Original object 0x801563A0-0x80156870 (config/objects/RPG_BAT.txt). */
#include "common.h"
#include "ovl/RPG_BAT.h"

void func_801563A0(void) {
    switch (*D_8015ED8C) {
    case 0:
        strcpy(D_8015E904, "朝日奈「ねえ、ねえっ。");
        strcpy(D_8015E92C, "聞いて、聞いてっ！");
        func_8014B738((s32) D_8015E9CC, 2, 0xA0);
        *D_8015ED8C += 1;
        return;
    case 1:
        func_8013F1C4(0x1F000015, 0x05000017, 0);
        func_8013E7C0(0x1D, 7, 1, 1);
        *D_8015ED8C += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x1F000018, 0x0600001D, 1);
        func_8013E7C0(0x1D, 8, 1, 1);
        *D_8015ED8C += 1;
        return;
    case 4:
        func_8013F250(2);
        return;
    case 5:
        func_8013F220();
        func_8014F230();
        /* fallthrough */
    default:
        return;
    }
}

/* FAKE: typed 0x44-byte record view coexists with unresolved shared byte-table/scalar declarations. T-9550 */
void func_801564F0(void) {
    func_8013E7C0(0x1D, 8, 1, 1);
    switch (*D_8015ED8C) {
    case 0:
        if (D_8015EDC4 & 0x100000) {
            strcpy(D_8015E904, "不良達「なんだ、なんだ？");
        } else {
            strcpy(D_8015E904, "番長「なんだ、なんだ？");
        }
        func_8014B738((s32) D_8015E9CC, 1, 0x82);
        *D_8015ED8C += 1;
        return;
    case 1:
        if (D_8015EDC4 & 0x100000) {
            func_8013F1C4(0x1E00006F, 0x17000072, 0);
        } else {
            func_8013F1C4(0x10000093, 0x06000098, 0);
        }
        *D_8015ED8C += 1;
        return;
    case 2:
        ((Actor *)actor_pool)[155].draw_flags |= func_8013E9A8(2);
        ((Actor *)actor_pool)[156].draw_flags |= func_8013E9A8(2);
        ((Actor *)actor_pool)[157].draw_flags |= func_8013E9A8(2);
        *D_8015ED8C += 1;
        return;
    case 3:
        if (((s32) D_8015EE0C % 15) == 0xE) {
            ((Actor *)actor_pool)[155].draw_flags ^= 1;
            ((Actor *)actor_pool)[156].draw_flags ^= 1;
            ((Actor *)actor_pool)[157].draw_flags ^= 1;
        }
        D_8015EE0C += 1;
        func_8013F250(0);
        return;
    case 4:
        ((Actor *)actor_pool)[155].draw_flags &= 0xFE;
        ((Actor *)actor_pool)[156].draw_flags &= 0xFE;
        ((Actor *)actor_pool)[157].draw_flags &= 0xFE;
        func_8013E7C0(0x1D, 0, 1, 1);
        func_8013F220();
        func_8014F230();
        /* fallthrough */
    default:
        return;
    }
}

void func_8015675C(void) {
    switch (D_8015EE04) {
    case 0:
        func_8013E7C0(0x1D, 5, 5, 1);
        func_8014F230();
        break;
    case 1:
        if (D_80120E1C == 2) {
            func_8013F15C(0x513, 0, 1);
        }
        func_8014F3C8(0x1D);
        break;
    case 2:
        func_80143C28(6, 0);
        break;
    case 3:
        func_801563A0();
        break;
    case 4:
        func_801564F0();
        break;
    case 5:
        func_8014F350(0x14);
        break;
    case 6:
        func_80143CE8(3, 0);
        break;
    case 7:
        func_8014F350(0x14);
        break;
    case 8:
        func_8014F278();
        break;
    }
}
