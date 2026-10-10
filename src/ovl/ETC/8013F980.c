#include "common.h"
#include "ovl/ETC.h"

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
} EtcSVec; /* SVECTOR layout */

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013F980", func_8013F980);

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
    s32 pad;
} EtcVec; /* VECTOR layout */

typedef struct {
    s16 m[3][3];
    s16 pad;
    s32 t[3];
} EtcMat; /* MATRIX layout, size 0x20 */

void func_8013FA7C(void) {
    s32 pad1[2]; /* FAKE: two unused 8-byte locals (above rot and below scale) give the original frame 0x90; real source unknown. T-4010 */
    EtcSVec rot;
    EtcMat m1;
    EtcMat m2;
    EtcVec scale;
    s32 pad2[2];

    rot.vz = 0;
    rot.vx = 0;
    rot.vy = D_801500D4;
    if (D_801500D4 >= 0xC00) {
        rot.vy = 0xC00;
    }
    m1 = *(EtcMat *)D_80125C60;
    m2 = *(EtcMat *)D_80125C60;
    func_800A09D0(&rot, &m1);
    scale.vx = 0x1000;
    scale.vy = 0x1000;
    scale.vz = 0x1000;
    func_800A0F4C(&m2, &scale);
    func_800A0C64(&m1, &m2);
    m1.t[0] = D_801500C8;
    m1.t[1] = D_801500CC;
    m1.t[2] = D_801500D0;
    *(EtcMat *)D_801227A4 = m1;
    *(s32 *)(D_801227A4 - 4) = 0; /* D_801227A0: TAIIKU declares it as TaiikuBig[], so it has no common declaration */
}

void func_8013FC08(void) {
    s32 pad; /* FAKE: unused 4-byte local puts rect at the original offset; real source unknown. T-2040 */
    RECT rect;

    rect.x = 0;
    rect.y = D_8011ECA0 * 0xF0;
    rect.w = 0x140;
    rect.h = 0xF0;
    func_8009C93C(&rect, 0x280, 0);
    func_8009C674(0);
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013F980", func_8013FC64);
