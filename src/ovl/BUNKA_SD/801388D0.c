#include "common.h"
#include "game.h"
#include "ovl/BUNKA_SD.h"

s32 func_801388D0(void) {
    if (D_800E6280.unk_110D == 0) {
        if (func_800460EC() == 2) {
            D_800E6280.unk_110D += 1;
        }
    } else if (D_800E6280.unk_110D == 1) {
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        D_800E6280.unk_110D += 1;
    } else if (D_800E6280.unk_110D == 2) {
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D += 1;
        }
    } else {
        func_80048390();
        func_80048E78();
        func_8006492C(1);
        func_80042908(0x10);
        func_80044750(0x200);
    }
}


s32 func_801389DC(void) {
    if (D_800E6280.unk_110D == 0) {
        cd_read_sectors(0x56, 0x80180000, 0x4F1D);
        D_800E6280.unk_110D += 1;
    } else if (D_800E6280.unk_110D == 1 && (cd_read_sync_status() & 1)) {
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80046290(0x3F, 0x0500039C, 0xF);
            break;
        case 0x60:
            func_80046290(0x0500005B, 0x05000450, 0xF);
            break;
        default:
        case 0x61:
            func_80046290(0x02000062, 0x080003F6, 0xF);
            break;
        }
        func_80044750(0x300);
        func_8004284C();
    }
}

void func_80138ADC(void) {
    if ((u32)D_800E6280.unk_110A >= 3 && (u32)D_800E6280.unk_110A < 6 && (D_800E6280.unk_F80 & 0x40)) {
        func_80042940(5);
        func_80044750(0xC4);
    }
    func_80138B3C();
}

void func_80138B3C(void) {
    func_80066C08(2);
    func_80064F48();
    func_80066334();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80134260();
        func_80044750(0x7F);
        return;
    case 1:
        func_801389DC();
        return;
    case 2:
        func_80138C34();
        return;
    case 3:
        func_80134418();
        return;
    case 4:
        func_80138CAC();
        return;
    case 5:
        func_8004E9F4(1);
        func_8004284C();
        return;
    case 6:
        func_8013987C();
        return;
    case 7:
        func_801388D0();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_80138C34(void) {
    if (func_800460EC() & 4) {
        if (D_800E6280.unk_1100++ >= 1U) {
            func_80138D48();
            func_8004284C();
            func_80045414(9, 0, 0);
        }
    } else {
        D_800E6280.unk_1100 = 0;
    }
}

void func_80138CAC(void) {
    GameState *g;

    func_80139100();
    func_80139290();
    g = &D_800E6280;
    g->unk_1104.u += 1;
    if ((g->unk_1104.u + 0x90000000U) == 0xFA && g->unk_03E != 0x60) {
        func_8004284C();
    }
    if ((g->unk_1104.u + 0x90000000U) == 0xD0 && g->unk_03E == 0x60) {
        func_8004284C();
    }
}

void func_80138D48(void) {
    s32 frame;
    s32 i;
    DisplayActor44 *record;

    i = 0;
    do {
        load_palette(D_8013D9CC, (i + 0x10) & 0xFF, 1, 1, i);
        i += 1;
    } while (i < 0x10);
    func_80048F64(0x60);
    ((DisplayActor44 *)D_80120650)[0].unk_01 = 5;
    ((DisplayActor44 *)D_80120650)[0].unk_38 = 0x01000000;
    ((DisplayActor44 *)D_80120650)[0].unk_03 = 0x80;
    ((DisplayActor44 *)D_80120650)[0].unk_02 = 0x40;
    ((DisplayActor44 *)D_80120650)[0].unk_0C = D_8013D9C4;
    ((DisplayActor44 *)D_80120650)[0].unk_10 = D_8013D9C8;
    ((DisplayActor44 *)D_80120650)[0].unk_34 = D_8013D9C0;
    ((DisplayActor44 *)D_80120650)[0].unk_18 = 0x2B;
    ((DisplayActor44 *)D_80120650)[0].unk_07 = 0;
    ((DisplayActor44 *)D_80120650)[0].unk_26 = -0xA0;
    ((DisplayActor44 *)D_80120650)[0].unk_2A = -0x78;
    ((DisplayActor44 *)D_80120650)[0].unk_05 = 0x80;
    ((DisplayActor44 *)D_80120650)[0].unk_43 = 0;
    ((DisplayActor44 *)D_80120650)[0].unk_04 = 0;
    func_80048F64(0x61);
    ((DisplayActor44 *)D_80120650)[1].unk_01 = 4;
    ((DisplayActor44 *)D_80120650)[1].unk_38 = 0x01000000;
    ((DisplayActor44 *)D_80120650)[1].unk_03 = 0x80;
    ((DisplayActor44 *)D_80120650)[1].unk_02 = 0x40;
    ((DisplayActor44 *)D_80120650)[1].unk_0C = D_8013D9C4;
    ((DisplayActor44 *)D_80120650)[1].unk_10 = D_8013D9C8;
    ((DisplayActor44 *)D_80120650)[1].unk_34 = D_8013D9C0;
    ((DisplayActor44 *)D_80120650)[1].unk_18 = 0;
    ((DisplayActor44 *)D_80120650)[1].unk_07 = 0;
    ((DisplayActor44 *)D_80120650)[1].unk_26 = -0xA0;
    ((DisplayActor44 *)D_80120650)[1].unk_2A = -0x78;
    ((DisplayActor44 *)D_80120650)[1].unk_05 = 0x80;
    ((DisplayActor44 *)D_80120650)[1].unk_43 = 0;
    ((DisplayActor44 *)D_80120650)[1].unk_04 = 0;
    record = (DisplayActor44 *)D_8011ED58;
    i = 2;
    frame = 0xF;
    for (; i != 6; i++) {
        func_80048F64(i + 0x60);
        record[96].unk_01 = 4;
        record[96].unk_38 = 0x01000000;
        record[96].unk_03 = 0x80;
        record[96].unk_02 = 0x40;
        record[96].unk_0C = D_8013D9C4;
        record[96].unk_10 = D_8013D9C8;
        record[96].unk_34 = D_8013D9C0;
        record[96].unk_18 = frame;
        frame += 4;
        record++;
        record[95].unk_06 = 1;
        record[95].unk_07 = 0;
        record[95].unk_26 = -0xA0;
        record[95].unk_2A = -0x78;
        record[95].unk_05 = 0x80;
        record[95].unk_43 = 0;
        record[95].unk_04 = 0;
    }
    if (D_80122EAC & 1) {
        ((DisplayActor44 *)D_80120650)[3].unk_18 = 0x1F;
    }
    if (D_80122EAC & 2) {
        ((DisplayActor44 *)D_80120650)[4].unk_18 = 0x23;
    }
    if (D_80122EAC & 4) {
        ((DisplayActor44 *)D_80120650)[5].unk_18 = 0x27;
    }
    load_tpage_buf_lock(5, ((s32 *)D_8013D9C0)[0], 1);
    load_tpage_buf_lock(6, ((s32 *)D_8013D9C0)[1], 1);
    load_tpage_buf_lock(7, ((s32 *)D_8013D9C0)[2], 1);
    load_tpage_buf_lock(8, ((s32 *)D_8013D9C0)[3], 1);
    load_tpage_buf_lock(9, ((s32 *)D_8013D9C0)[4], 1);
    load_tpage_buf_lock(0xA, ((s32 *)D_8013D9C0)[5], 1);
    load_tpage_buf_lock(0x25, ((s32 *)D_8013D9C0)[6], 1);
    load_tpage_buf_lock(0x26, ((s32 *)D_8013D9C0)[7], 1);
    load_tpage_buf_lock(0x27, ((s32 *)D_8013D9C0)[8], 1);
    load_tpage_buf_lock(0x28, ((s32 *)D_8013D9C0)[9], 1);
}


s32 func_80139100(void) {
    if (D_800E6280.unk_1104.u == 0xBE) {
        D_800E6280.unk_1100 = func_80046274() - 0x1000;
    }
    if (D_800E6280.unk_1104.u == 0xBC) {
        func_80044750(0x300);
    }
    func_8004B19C(func_80046274(), 0, 0);
    func_8004B19C(D_800E6280.unk_1100, 0, 0x10);
    func_8004B19C(func_80046274() - D_800E6280.unk_1100, 0, 0x20);
    if (D_800E6280.unk_1104.u >= 0xC0U) {
        if (D_800E6280.unk_1104.u < 0x70000000U) {
            switch (D_800E6280.unk_03E) {
            case 0x5F:
                if ((u32)(func_80046274() - D_800E6280.unk_1100) >= 0x2609U) {
                    D_800E6280.unk_1104.u = 0x70000000;
                }
                break;
            case 0x60:
                if ((u32)(func_80046274() - D_800E6280.unk_1100) >= 0x2AF1U) {
                    D_800E6280.unk_1104.u = 0x70000000;
                }
                break;
            case 0x61:
            default:
                if ((u32)(func_80046274() - D_800E6280.unk_1100) >= 0x27E9U) {
                    D_800E6280.unk_1104.u = 0x70000000;
                }
                break;
            }
        }
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_80139290);

/* FAKE: T-22021. The byte D_800E6280.unk_110B is read and written only as a u8 view, so uopt promotes it to one register for the whole function (a local copy splits the range and adds a `move`). `*= 0` is the clear that survives constant folding: it keeps the `move v1,zero` and the loop stores of that register (a plain `= 0` folds to `sb zero`); the original source is unknown. */
void func_8013987C(void) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        ((DisplayActor44 *)D_80120650)[i].unk_07 = *(u8 *)&D_800E6280.unk_110B;
    }
    if (*(u8 *)&D_800E6280.unk_110B == 0) {
        *(u8 *)&D_800E6280.unk_110B = 2;
    }
    *(u8 *)&D_800E6280.unk_110B -= 1;
    if (*(u8 *)&D_800E6280.unk_110B == 0) {
        *(u8 *)&D_800E6280.unk_110B *= 0;
        for (i = 0; i < 0x40; i++) {
            ((DisplayActor44 *)D_80120650)[i].unk_07 = *(u8 *)&D_800E6280.unk_110B;
        }
        func_8004284C();
    }
}
