#include "common.h"
#include "ovl/TEL.h"
#include "game.h"

/* Display record n of the D_8011ECD0 pool. */
#define REC(n) (((DisplayRecord44 *)D_8011ECD0)[n])

/* Frame and position of display record n, one source line (as1 schedules the three stores by line). */
#define SET_REC(n, frame, px, py) (REC(n).unk_18 = (frame), REC(n).unk_26 = (px), REC(n).unk_2A = (py))

void func_8013CCD0(void) {
    DisplayRecord44 *rec;
    s32 i;
    s32 idx;
    s32 x;
    s32 y;
    s32 stars;
    u8 rank[16];
    s16 points[16];

    for (i = 0; i < 0x22; i++) {
        func_80048F64(i + 0x60);
        rec = &REC(i + 0x60);
        rec->unk_01 = 4;
        rec->unk_02 = 0x40;
        rec->unk_38 = 0x40000000;
        rec->unk_03 = 0x84;
        rec->unk_0C = sprite_frame_table;
        rec->unk_10 = D_800C9A14;
        rec->unk_34 = D_800C9730;
        rec->unk_14 = D_800C9A54;
        rec->unk_04 = 8;
        rec->unk_05 = 1;
        rec->unk_26 = 0x200;
    }
    D_80120668 = 0x42;
    D_80120676 = -0x10;
    D_8012067A = -0x1B;
    D_801230D4 = set_kanji_string(-0x80, -0x62, 4, "女の子からの評価", 0);
    idx = 0x61;
    func_80053418(rank, points);
    for (i = 0; i != 11; i++) {
        D_800E6280.unk_1BC[0xF].unk_14[i] = 0;
        if (D_800E6280.unk_1BC[i].unk_0C.f.b1) {
            x = (i / 6) * 0x82;
            y = (i % 6) * 0x10;
            set_kanji_string(x - 0x82, y - 0x48, 3, D_800B3538[i], 0);
            SET_REC(idx, 0x61, x - 0x32, y - 0x44);
            idx++;
            if (points[i] >= 0x1F4) {
                stars = 0;
            } else if (rank[i] < 6) {
                stars = 6;
            } else if (rank[i] < 0xB) {
                stars = 5;
            } else if (rank[i] < 0x15) {
                stars = 4;
            } else if (rank[i] < 0x29) {
                stars = 3;
            } else if (rank[i] < 0x3D) {
                stars = 2;
            } else {
                stars = 1;
            }
            SET_REC(idx, stars + 0x59, x - 0x22, y - 0x44);
            idx++;
            if (D_800E6280.unk_1BC[i].unk_0A >= 0x3C) {
                REC(idx).unk_06 = -(func_800AE0C0(D_800E6280.unk_1BC[i].unk_0A - 0x3C) / 10) * 2 + 8;
                if (REC(idx).unk_06 == 0) {
                    REC(idx).unk_06 = 1;
                }
                SET_REC(idx, 0x60, x - 0x12, y - 0x44);
                idx++;
            }
            D_800E6280.unk_1BC[0xF].unk_14[i] = stars + 1;
        }
    }
    D_801230D4 = set_kanji_string(-0x80, -0x62, 4, "女の子からの評価", 0);
}

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/8013CCD0", func_8013D03C);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/8013CCD0", func_8013D88C);
