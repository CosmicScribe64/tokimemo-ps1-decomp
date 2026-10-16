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
    *(s32 *)(D_801227A4 - 4) = 0; /* FAKE: D_801227A0 reached through D_801227A4; TAIIKU declares it as TaiikuBig[], so it has no common declaration (CODING_STANDARDS 8a). T-4010 */
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

/* bulk_m2c declarations (T-11190) */
extern s32 D_801500A0;
/* end bulk_m2c declarations */
void func_8013FC64(s32 a, s32 b, s32 c, s32 d) {
    s32 sp64;
    s32 sp5C;
    s32 temp_v0;
    s32 temp_v0_3;
    s32 var_s1;
    s32 var_s2;
    s32 var_s5;
    u8 *temp_v0_2;

    if (a < 0) {
        sp64 = 1;
        var_s1 = -a;
    } else {
        sp64 = 0;
        var_s1 = a;
    }
    var_s5 = 0;
    if (var_s1 >= 0) {
        sp5C = d * 0x10;
        var_s2 = 0;
loop_5:
        temp_v0 = GetWorkBase(0x14, D_8011ECA0);
        func_8009F0A4(temp_v0);
        *(s16 *)((unsigned char *)temp_v0 + (16)) = 8;
        *(s16 *)((unsigned char *)temp_v0 + (18)) = 8;
        SetSemiTrans(temp_v0, 0);
        *(s8 *)((unsigned char *)temp_v0 + (4)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0 + (5)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0 + (6)) = 0x80;
        *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + b) - var_s2);
        *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + c);
        temp_v0_2 = &D_801500A0 + ((var_s1 % 10) * 2);
        *(u8 *)((unsigned char *)temp_v0 + (12)) = (u8) (*(s32 *)temp_v0_2);
        *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) *(u8 *)((unsigned char *)temp_v0_2 + (1));
        *(s16 *)((unsigned char *)temp_v0 + (14)) = func_8009ED74(sp5C, 0x1FD);
        AddPrim((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0);
        if (var_s1 >= 0xA) {
            var_s5 += 1;
            var_s2 += 8;
            var_s1 = var_s1 / 10;
            if (var_s1 >= 0) {
                goto loop_5;
            }
        }
    }
    if (sp64 != 0) {
        sp5C = d * 0x10;
        temp_v0_3 = GetWorkBase(0x14, D_8011ECA0);
        func_8009F0A4(temp_v0_3);
        *(s16 *)((unsigned char *)temp_v0_3 + (16)) = 8;
        *(s16 *)((unsigned char *)temp_v0_3 + (18)) = 8;
        SetSemiTrans(temp_v0_3, 0);
        *(s8 *)((unsigned char *)temp_v0_3 + (4)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0_3 + (5)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0_3 + (6)) = 0x80;
        *(s16 *)((unsigned char *)temp_v0_3 + (8)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + b) - ((var_s5 + 1) * 8));
        *(s16 *)((unsigned char *)temp_v0_3 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + c);
        *(u8 *)((unsigned char *)temp_v0_3 + (12)) = (u8) D_801500B4;
        *(u8 *)((unsigned char *)temp_v0_3 + (13)) = (u8) D_801500B5;
        *(s16 *)((unsigned char *)temp_v0_3 + (14)) = func_8009ED74(sp5C, 0x1FD);
        AddPrim((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0_3);
    }
    dtd_on_tpage(0x3C0, 0, 1, 0, 1);
}
