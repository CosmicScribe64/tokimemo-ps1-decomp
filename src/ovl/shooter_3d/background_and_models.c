/* background_and_models.c: src/ovl/TACO/80143E80.c:11: frame hook, background scrolling, floor grid/horizon and model submission; body 12: shooter_background_scroll.a = 0; 13: shooter_background_scroll.b = 0x400;. Original object 0x80143E80-0x801447D0 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/shooter_3d.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
ShooterBackgroundScroll shooter_background_scroll = { 0, 0x400, 0, 0 };
INCLUDE_RODATA("asm/ovl/TACO/data/shooter_3d/background_and_models.data", shooter_floor_tile_u);
INCLUDE_RODATA("asm/ovl/TACO/data/shooter_3d/background_and_models.data", shooter_floor_tile_v);
INCLUDE_RODATA("asm/ovl/TACO/data/shooter_3d/background_and_models.data", shooter_opening_pose_frames);
INCLUDE_RODATA("asm/ovl/TACO/data/shooter_3d/background_and_models.data", shooter_opening_pose_tables);

void shooter_init_background_scroll(void) {
    shooter_background_scroll.x = 0;
    shooter_background_scroll.y = 0x400;
    shooter_background_scroll.scroll_z = 0;
    shooter_background_scroll.tile_phase = 0;
}

void shooter_frame_update(void) {
    if (shooter_background_flags != 0) {
        shooter_update_camera();
        shooter_scroll_background();
        shooter_process_load_request();
        shooter_tick_se_cooldowns();
    }
    if (shooter_background_flags & 1) {
        shooter_draw_floor_grid();
    }
    if (shooter_background_flags & 2) {
        shooter_draw_horizon_bands();
    }
    if (shooter_background_flags & 4) {
        shooter_nop_background_layer();
    }
}

s32 shooter_scroll_background(void) {
    shooter_background_scroll.scroll_z += 0x80;
    if (shooter_background_scroll.scroll_z >= 0x1001) {
        shooter_background_scroll.scroll_z -= 0x1000;
        shooter_background_scroll.tile_phase += 1;
    }
    if (shooter_background_scroll.scroll_z < 0) {
        shooter_background_scroll.scroll_z += 0x1000;
        shooter_background_scroll.tile_phase -= 1;
    }
    if (shooter_background_scroll.x >= 0x801) {
        shooter_background_scroll.x -= 0x800;
    }
    if (shooter_background_scroll.x < 0) {
        shooter_background_scroll.x += 0x800;
    }
}

void shooter_update_camera(void) {
    D_80122740 = shooter_actors->x;
    D_80122744 = shooter_actors->y;
    D_80122748 = shooter_actors->z;
    D_8012274C = shooter_actors->x;
    D_80122750 = shooter_actors->y;
    D_80122754 = 0;
    D_80122758 = shooter_actors->rotation_z * 0x168;
    D_8012275C = 0;
    func_80099540(&D_80122740);
}

void shooter_draw_floor_grid(void) {
    s32 pad[2]; /* FAKE: eight unused bytes above m reproduce the original frame (608); T-24136 */
    MATRIX m;
    SVECTOR v;
    s32 sxy[13][9];
    s32 ang;
    s32 i;
    s16 *q;
    s32 idx;
    POLY_FT4 *poly;
    s32 depth;
    s32 flag;
    s32 j;
    ShooterTextureCoordinates ut;
    ShooterTextureCoordinates vt;
    s32 k;

    ut = shooter_floor_tile_u;
    vt = shooter_floor_tile_v;
    m = D_80129C40;
    func_800A1100(&m);
    SetRotMatrix(&m);
    v.vy = shooter_background_scroll.y;
    for (ang = 0, i = 0; i < 9; i++) {
        if (i == 8) {
            v.vz = ang;
        } else {
            /* FAKE: | 0 flips commutative Uadd operand order to addu t5, s8, t4 (T-24136) */
            v.vz = (ang | 0) + shooter_background_scroll.scroll_z;
        }
        for (j = 0; j < 13; j++) {
            v.vx = 0x3000 - j * 0x800 + shooter_background_scroll.x;
            func_800AB170(&v, &sxy[j][i], &depth, &flag);
        }
        ang -= 0x1000;
    }
    /* FAKE: * 0 occurrences adjust uopt priority of k ($a1) and loop constant 12 ($s6); T-24136 */
    k = draw_buffer + (k * 0);
    for (i = 0; i < 8; i++) {
        k += (i == 12) * 0;
        for (j = 0; j < 12; j++) {
            q = (s16 *)&sxy[j][i];
            if (q[20] >= 0 && q[2] < 0x201) {
                if (q[1] >= game_state.draw_origin_y[k] - 0x200 && game_state.draw_origin_y[k] + 0x400 >= q[3]) {
                    poly = GetWorkBase(0x28, k);
                    func_8009F040(poly);
                    poly->r0 = 0x80;
                    poly->g0 = 0x80;
                    poly->b0 = 0x80;
                    poly->tpage = func_8009ECB0(1, 0, 0x200, 0x100);
                    poly->clut = func_8009ED74(0, 0x1ED);
                    idx = (j + i + shooter_background_scroll.tile_phase) & 7;
                    if (idx >= 4) {
                        idx = 0;
                    }
                    poly->u0 = ut.b[idx];
                    poly->v0 = vt.b[idx];
                    poly->u1 = ut.b[idx] + 0x3F;
                    poly->v1 = vt.b[idx];
                    poly->u2 = ut.b[idx];
                    poly->v2 = vt.b[idx] + 0x3F;
                    poly->u3 = ut.b[idx] + 0x3F;
                    poly->v3 = vt.b[idx] + 0x3F;
                    poly->x0 = q[0];
                    poly->y0 = q[1];
                    poly->x1 = q[2];
                    poly->y1 = q[3];
                    poly->x2 = q[18];
                    poly->y2 = q[19];
                    poly->x3 = q[20];
                    poly->y3 = q[21];
                    AddPrim(draw_ot_tags + ((draw_buffer << 10) + 0x3F8), poly);
                    k = draw_buffer;
                }
            }
        }
    }
}

void shooter_draw_horizon_bands(void) {
    s32 pad[2]; /* FAKE: eight unused bytes above m reproduce the original frame (216); T-19009 */
    MATRIX m;
    SVECTOR v;
    s32 pt[10];
    /* The scalars are declared in this order because IDO gives each a stack slot (register
     * resident or not) and the original's slots for rot, flag and depth are 0x60, 0x68, 0x6C. T-19009 */
    s32 o;
    s16 *q;
    POLY_FT4 *poly;
    s32 t;
    s32 depth;
    s32 flag;
    s32 j;
    s32 rot;

    m = D_80129C40;
    func_800A1100(&m);
    SetRotMatrix(&m);
    v.vz = -0x7FFF;
    rot = 0;
    for (o = 0; o < 2; o++) {
        v.vy = shooter_background_scroll.y + rot / 4 + 0x80;
        for (j = 0; j < 5; j++) {
            v.vx = 0x4000 - j * 0x2000 + D_8012274C;
            func_800AB170(&v, &pt[j * 2 + o], &depth, &flag);
        }
        rot -= 0x1000;
    }
    for (o = 0; o < 1; o++) {
        for (j = 0; j < 4; j++) {
            q = (s16 *)&pt[j * 2 + o];
            poly = GetWorkBase(0x28, draw_buffer);
            func_8009F040(poly);
            poly->r0 = 0x80;
            poly->g0 = 0x80;
            poly->b0 = 0x80;
            poly->tpage = func_8009ECB0(1, 0, 0x200, 0x100);
            t = (j & 1) << 6;
            poly->clut = func_8009ED74(0, 0x1EE);
            poly->v0 = t + 0x3F;
            poly->v2 = t + 0x3F;
            poly->v1 = t;
            poly->v3 = t;
            poly->u0 = 0;
            poly->u1 = 0;
            poly->u2 = 0xFF;
            poly->u3 = 0xFF;
            poly->x0 = q[0];
            poly->y0 = q[1];
            poly->x1 = q[2];
            poly->y1 = q[3];
            poly->x2 = q[4];
            poly->y2 = q[5];
            poly->x3 = q[6];
            poly->y3 = q[7];
            AddPrim(draw_ot_tags + (draw_buffer << 10) + 0x3F4, poly);
        }
    }
}

void shooter_nop_background_layer(void) {
}

void shooter_draw_models(void) {
    /* FAKE: two spare workspace words retain the observed frame size;
     * keeping the pointer init on the do line permits the original schedule.
     * T-9510: the transform helpers themselves use 32-byte workspaces. */
    u32 sp70[10];
    u32 sp50[8];
    ShooterModelHandle *var_s0;

    if (game_state.draw_3d_enabled != 0) {
        func_80099540(&D_80122740);
        var_s0 = D_80127080; do {
            func_80099B10(var_s0->links[0], &sp50, &sp70);
            func_80099F04(&sp50);
            func_8009A200(var_s0->links[0], &sp70);
            func_80099ED8(&sp70);
            func_8009A4D0((s32) var_s0, (s32) ((u8 *)&draw_ot_table + (draw_buffer * 0x14)), 6, 0x1F800000);
            var_s0++;
        } while (var_s0 != (ShooterModelHandle *)D_80127480);
    }
}
