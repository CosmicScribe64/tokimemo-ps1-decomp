/* album_model_setup.c: ; src/ovl/ETC/8013F980.c:11-20 initializes 3D model and object state. Original object 0x8013F980-0x8013FF80 (config/objects/ETC.txt). */
#include "common.h"
#include "ovl/ETC.h"

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
} AlbumRotation; /* SVECTOR layout */

void album_model_init(void) {
    s32 model;
    TaiikuBig *record;
    u32 *object;

    model = 0x801FA804;
    func_8009B3C0(model);
    model += 4;
    printf("NOBJ %x\n", *(u32 *)model);
    model += 4;
    /* FAKE: initializer/do line grouping reproduces setup order; original line layout unknown. T-24064. */
    object = D_80122640; record = D_801227A0; do {
        func_8009B430(model, (u8 *)object, 0);
        object[1] = (u32)record;
        record++;
        object += 4;
        object[-4] = 0x80000000;
    } while (record != D_80122CA0);
    func_8009B430(model, (u8 *)D_80122640, 0);
    D_80122640[1] = (u32)D_801227A0;
    D_80122640[0] = 0;
    album_model_x = 0x9F;
    album_model_y = -0x78;
    album_model_z = 0;
    album_model_angle = 0xC00;
}



void album_model_matrix_update(void) {
    s32 pad1[2]; /* FAKE: two unused 8-byte locals (above rot and below scale) give the original frame 0x90; real source unknown. T-4010 */
    AlbumRotation rot;
    MATRIX m1;
    MATRIX m2;
    VECTOR scale;
    s32 pad2[2];

    rot.vz = 0;
    rot.vx = 0;
    rot.vy = album_model_angle;
    if (album_model_angle >= 0xC00) {
        rot.vy = 0xC00;
    }
    m1 = *(MATRIX *)D_80125C60;
    m2 = *(MATRIX *)D_80125C60;
    RotMatrix(&rot, &m1);
    scale.vx = 0x1000;
    scale.vy = 0x1000;
    scale.vz = 0x1000;
    ScaleMatrix(&m2, &scale);
    MulMatrix(&m1, &m2);
    m1.t[0] = album_model_x;
    m1.t[1] = album_model_y;
    m1.t[2] = album_model_z;
    *(MATRIX *)D_801227A4 = m1;
    *(s32 *)(D_801227A4 - 4) = 0; /* FAKE: D_801227A0 reached through D_801227A4; TAIIKU declares it as TaiikuBig[], so it has no common declaration (CODING_STANDARDS 8a). T-4010 */
}

void album_clear_buffer_area(void) {
    s32 pad; /* FAKE: unused 4-byte local puts rect at the original offset; real source unknown. T-2040 */
    RECT rect;

    rect.x = 0;
    rect.y = draw_buffer * 0xF0;
    rect.w = 0x140;
    rect.h = 0xF0;
    func_8009C93C(&rect, 0x280, 0);
    func_8009C674(0);
}

void album_number_sprite_draw(s32 a, s32 b, s32 c, s32 d) {
    SPRT *p;
    s32 v;
    s32 xoff;
    s32 n;
    s32 negative;
    s32 tpx;

    if (a < 0) {
        negative = 1;
        v = -a;
    } else {
        negative = 0;
        v = a;
    }
    n = 0;
    if (v >= 0) {
        tpx = d * 16;
        do {
            p = (SPRT *)GetWorkBase(0x14, draw_buffer);
            func_8009F0A4(p);
            p->w = 8;
            p->h = 8;
            SetSemiTrans(p, 0);
            p->r0 = 0x80;
            p->g0 = 0x80;
            p->b0 = 0x80;
            p->x0 = game_state.screen_center_x[draw_buffer] + b - n * 8;
            p->y0 = game_state.screen_center_y[draw_buffer] + c;
            p->u0 = album_digit_uv[(v % 10) * 2];
            p->v0 = album_digit_uv[(v % 10) * 2 + 1];
            p->clut = func_8009ED74(tpx, 0x1FD);
            AddPrim((draw_buffer << 10) + 4 + D_800E8CA0, p);
            if (v < 10) {
                break;
            }
            v = v / 10;
            n++;
        } while (v >= 0);
    }
    if (negative != 0) {
        tpx = d * 16;
        n++;
        xoff = n * 8;
        p = (SPRT *)GetWorkBase(0x14, draw_buffer);
        func_8009F0A4(p);
        p->w = 8;
        p->h = 8;
        SetSemiTrans(p, 0);
        p->r0 = 0x80;
        p->g0 = 0x80;
        p->b0 = 0x80;
        p->x0 = game_state.screen_center_x[draw_buffer] + b - xoff;
        p->y0 = game_state.screen_center_y[draw_buffer] + c;
        p->u0 = album_minus_u;
        p->v0 = album_minus_v;
        p->clut = func_8009ED74(tpx, 0x1FD);
        AddPrim((draw_buffer << 10) + 4 + D_800E8CA0, p);
    }
    dtd_on_tpage(0x3C0, 0, 1, 0, 1);
}
