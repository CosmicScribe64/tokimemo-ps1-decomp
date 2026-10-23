#include "common.h"
#include "ovl/ENDING.h"
#include "game.h"

void func_80133030(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    EdFnTbl16 tbl;

    tbl = D_8013C370;
    func_80083808();
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    func_8007EDF8();
    func_800846C0();
    func_80066C08(2);
    func_80064F48();
    func_80066334();
    func_80064DEC();
    func_80083A10();
}

void func_801330E4(void) {
    s32 pad[4]; /* FAKE: T-25145, four unused word slots above rank: frame 0xB0, chance at 0x30 */
    s16 best;
    s16 *p;
    u8 rank[32];
    s16 points[16];
    s16 i; /* declared after points: level gets the home 0x54 it is saved to around rand() */
    s16 level;
    EdShorts18 chance;

    chance = D_8013C3B0;
    D_800E6280.unk_03E = 0x62;
    D_800E6280.unk_03F = 3;
    D_800E6280.unk_040 = 1;
    D_800E6280.unk_041 = 6;
    func_80053418(rank, points);
    D_800E6280.unk_F5F = 0xD;
    if (points[0] < 0x1F4 && D_800E6280.unk_1BC[0xC].unk_0C.b[3] >= 0x46 && D_800E6280.unk_56C[0x4F] == 2) {
        D_800E6280.unk_F5F = 0xC;
    } else {
        best = 0x1F3;
        for (i = 0; i < 0xB; i++) {
            p = &points[i];
            if (D_800E6280.unk_1BC[i].unk_0C.f.b1 && best < *p) {
                D_800E6280.unk_F5F = i;
                best = *p;
            }
        }
    }
    if (D_800E6280.unk_F5F == 0xD) {
        /* FAKE: T-25145, a second read of the flag word: IDO 5.3 then keeps the word in a register
         * (a0), as the original compiler did for its one read (wiki/original-compiler.md rule 3),
         * so ugen shifts it into v0 and copies the f9 temporary back after the sum */
        if (D_800E6280.unk_1BC[0xB].unk_0C.u) {
        }
        level = D_800E6280.unk_1BC[0xB].unk_0C.f.f9;
        level += (D_800E6280.unk_56C[0x4C] > 0) * 2;
        level += (D_800E6280.unk_56C[0x4E] > 0) * 2;
        if (level >= 0x12) {
            level = 0x11;
        }
        if (D_800E6280.unk_1BC[0xB].unk_0C.f.f9 != 0 && D_800E6280.unk_56C[0x4D] == 0) {
            if (rand() % 256 < chance.v[level]) {
                D_800E6280.unk_F5F = 0xB;
            }
        }
    }
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    func_800AE0F0(D_800CA19C, "廊下");
    func_800AE0F0(D_800CA1DC, "教室");
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    func_8008585C();
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    func_80084E4C();
    D_800CA148 = 0;
    D_800CA14C = 0;
    func_800847B8(D_800E6280.unk_F5F);
    func_80132000();
    D_800CA160 = D_8013C280[0][3];
    D_800CA164 = D_8013C280[1][3];
    D_800CA168 = D_8013C280[2][3];
    func_8004284C();
}

void func_80133400(void) {
    bg_read_sub2(0x4045);
    func_8004284C();
}

void func_80133428(void) {
    cd_read_sectors(0x11, 0x801F0000, 0xAE07);
    func_80132000();
    func_8004284C();
}

void func_80133460(void) {
    if (((u8) D_800E6280.unk_F5F >= 0xDU) && ((u32) ((u32) (D_800E6280.unk_1BC[14].unk_0C.w << 0x14) >> 0x1D) < 3U)) {
        func_80042908(7);
        D_800E7D34 |= 8;
        return;
    }
    func_8004284C();
    D_800E7D34 |= 4;
}

void func_801334E4(void) {
    if ((((u32)(D_800E6280.unk_1BC[1].unk_0C.w << 0x1E) >> 0x1F)) && (get_h_tokimeki(1) >= 0x46U)) {
        normal_date_speak();
        return;
    }
    func_8004284C();
}
