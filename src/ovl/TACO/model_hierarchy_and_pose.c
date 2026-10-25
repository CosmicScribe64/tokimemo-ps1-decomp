/* model_hierarchy_and_pose.c: src/ovl/TACO/80146D60.c:7: coordinate hierarchy, model linking, pose tables, keyframe tweening and boss joint helpers; body 11: q = D_80127480; 12: func_80099E30(0, q);. Original object 0x80146D60-0x801480B0 (config/objects/TACO.txt). */
#define TACO_OVERRIDE_shooter_link_model_objects /* pointer definition, s32 view for the callers: with u32 * in the header the caller in 80148FE0.c loses its shared constant register (T-19015) */
#include "common.h"
#include "ovl/TACO.h"

void shooter_link_model_objects(u32 *arg0);

void shooter_init_object_hierarchy(s32 arg0) {
    ShooterCoordinate *q;
    s32 i;

    q = D_80127480;
    func_80099E30(0, q);
    for (i = 1; i < 64; i++) {
        func_80099E30(q, &D_80127480[i]);
        D_80127480[i].y = 0;
        D_80127480[i].x = 0;
        D_80127480[i].z = arg0;
        *(s32 *)&D_80127480[i] = 0;
    }
    for (i = 0; i < 0x24; i++) {
        D_80128880[i].x = D_80128880[i].y = D_80128880[i].z = 0;
    }
}

void shooter_set_dark_lights(void) {
    view_light_0 = 0x14;
    D_80122764 = 0x64;
    D_80122768 = -0x64;
    D_8012276C = 0xB0;
    D_8012276D = 0xB0;
    D_8012276E = 0xB0;
    func_8009AD70(0, &view_light_0);
    view_light_1 = 0x14;
    D_80122774 = -0x64;
    D_80122778 = 0x64;
    D_8012277C = 0x80;
    D_8012277D = 0x80;
    D_8012277E = 0x80;
    func_8009AD70(1, &view_light_1);
    view_light_2 = -0x14;
    D_80122784 = 0x14;
    D_80122788 = -0x64;
    D_8012278C = 0x60;
    D_8012278D = 0x60;
    D_8012278E = 0x60;
    func_8009AD70(2, &view_light_2);
    GsSetAmbient(0, 0, 0);
    func_8009B340(0);
}

/* Same shape as shooter_apply_keyframe (T-16561): a local for the coordinate object would add a stack home and move the spill slots. */
void shooter_move_model(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    MATRIX matrix;
    ShooterPosition *pos;

    matrix = *(MATRIX *)view_base_matrix;
    matrix.t[0] = ((ShooterCoordinateView *)&D_80127480[arg0])->local.t[0] + arg1;
    matrix.t[1] = ((ShooterCoordinateView *)&D_80127480[arg0])->local.t[1] + arg2;
    matrix.t[2] = ((ShooterCoordinateView *)&D_80127480[arg0])->local.t[2] + arg3;
    if (arg7 != 0 && arg8 != 0) {
        matrix.t[0] += (shooter_actors->x - matrix.t[0]) / arg7;
        matrix.t[1] += (shooter_actors->y - matrix.t[1]) / arg8;
    }
    pos = &D_80128880[arg0];
    pos->x += arg4;
    shooter_rotation_matrix_x((pos->x << 12) / 360, &matrix);
    pos->z += arg6;
    shooter_rotation_matrix_z((pos->z << 12) / 360, &matrix);
    pos->y += arg5;
    shooter_rotation_matrix_y((pos->y << 12) / 360, &matrix);
    ((ShooterCoordinateView *)&D_80127480[arg0])->local = matrix;
    ((ShooterCoordinateView *)&D_80127480[arg0])->flag = 0;
}

/* libgs model setup (GsMapModelingData, GsLinkObject4) walking the TMD header with dop. The second pass
 * restarts dop from arg0, which keeps the copy and the two in-place steps of the original
 * (`move s4,s3; lw s6,4(s4); addiu s4,s4,8`); a walker that is not restarted is folded into arg0 + 8.
 * arg0 must be a pointer: with an s32 parameter uopt keeps the first-pass copy as well (T-19015). */
void shooter_link_model_objects(u32 *arg0) {
    u32 *dop;
    s32 i;
    u32 n;

    dop = arg0;
    dop++;
    func_8009B3C0((s32)dop);
    dop += 2;
    for (i = 0; i < 64; i++) {
        func_8009B430((s32)dop, (u8 *)&D_80127080[i], 0);
        D_80127080[i].links[0] = (s32)&D_80127480[i];
        D_80127080[i].attribute = 0x80000000;
    }
    dop = arg0;
    dop++;
    func_8009B3C0((s32)dop);
    dop++;
    n = *dop;
    dop++;
    for (i = 0; i < n; i++) {
        func_8009B430((s32)dop, (u8 *)&D_80127080[i + 1], i);
        D_80127080[i + 1].links[0] = (s32)&D_80127480[i + 1];
        D_80127080[i + 1].attribute = 0x80000000;
    }
}

s32 shooter_set_pose_from_tables(arg0, arg1, arg2, arg3, arg4, arg5)
s32 arg0;
s32 arg1;
s16 *arg2;
s16 *arg3;
u8 arg4;
u8 arg5;
{
    ShooterModelHandle *rec;
    MATRIX m;
    s32 i;
    s32 j;
    s32 pad; /* FAKE: unused 4-byte local places m at sp+0x2C; real source unknown. T-16561 */

    if (((s32)arg2 & 3) || ((s32)arg3 & 3)) {
        return -1;
    }
    if (arg4 == 0) {
        arg2 += arg1 * 3;
    }
    if (arg5 == 0) {
        arg3 += arg1 * 9;
    }
    for (i = 0; i != 3; i++) {
        for (j = 0; j != 3; j++) m.m[i][j] = *arg3++;
        m.t[i] = *arg2++;
    }
    rec = &D_80127080[arg0];
    ((ShooterCoordinateView *)rec->links[0])->local = m;
    ((ShooterCoordinateView *)rec->links[0])->flag = 0;
    return (s32)rec;
}

/* s32 counter against a u32 bound: IDO cannot count the trips and keeps the loop rolled, like the
 * original (T-5020). */
void shooter_read_model_polygon_ranges(u8 *arg0) {
    s32 i;
    u32 n;
    u8 *p;

    n = *(u32 *)(arg0 + 8);
    p = arg0 + 0x1C;
    for (i = 1; i < n + 1; i++) {
        shooter_model_polygon_ranges[i] = *(s32 *)p;
        p += 0x1C;
    }
}

void shooter_upload_rect_256(void *arg0, s32 arg1, s32 arg2, s32 arg3)
{
    RECT rect;
    /* FAKE: preserve the original frame extent (T-9510). */
    s32 pad[2];

    rect.x = arg1;
    rect.y = arg2;
    rect.w = 0x100;
    rect.h = arg3;
    func_8009C884(&rect, arg0);
}

/* Same code as shooter_apply_pose_key (T-16561). No pointer local for the coordinate object: a second pointer local adds a stack home and
   moves the spill slots 4 bytes down. */
void shooter_apply_keyframe(s32 arg0, s16 *arg1, s32 arg2) {
    MATRIX matrix;
    ShooterPosition *pos;

    arg1 += arg2 * 6;
    matrix = *(MATRIX *)view_base_matrix;
    matrix.t[0] = ((ShooterCoordinateView *)&D_80127480[arg0])->local.t[0] + arg1[0];
    matrix.t[1] = ((ShooterCoordinateView *)&D_80127480[arg0])->local.t[1] + arg1[1];
    matrix.t[2] = ((ShooterCoordinateView *)&D_80127480[arg0])->local.t[2] + arg1[2];
    pos = &D_80128880[arg0];
    pos->x += arg1[3];
    arg1 += 4;
    shooter_rotation_matrix_x((pos->x << 12) / 360, &matrix);
    pos->z += *arg1;
    arg1++;
    shooter_rotation_matrix_z((pos->z << 12) / 360, &matrix);
    pos->y += *arg1;
    shooter_rotation_matrix_y((pos->y << 12) / 360, &matrix);
    ((ShooterCoordinateView *)&D_80127480[arg0])->local = matrix;
    ((ShooterCoordinateView *)&D_80127480[arg0])->flag = 0;
}

/* Same shape as shooter_apply_keyframe with an interpolation step. The record pointer is the parameter itself (a separate local is not kept in $s0). T-16561 */
void shooter_apply_keyframe_tween(s32 arg0, s16 *arg1, s32 arg2, s32 arg3, s32 arg4) {
    MATRIX matrix;
    ShooterPosition *pos;

    arg1 += arg2 * 6;
    matrix = *(MATRIX *)view_base_matrix;
    matrix.t[0] = ((ShooterCoordinateView *)&D_80127480[arg0])->local.t[0] + arg1[0];
    matrix.t[1] = ((ShooterCoordinateView *)&D_80127480[arg0])->local.t[1] + arg1[1];
    matrix.t[2] = ((ShooterCoordinateView *)&D_80127480[arg0])->local.t[2] + arg1[2];
    arg1 += 3;
    if (arg3 != 0 && arg4 != 0) {
        matrix.t[0] += (shooter_actors->x - matrix.t[0]) / arg3;
        matrix.t[1] += (shooter_actors->y - matrix.t[1]) / arg4;
    }
    pos = &D_80128880[arg0];
    pos->x += *arg1;
    arg1++;
    shooter_rotation_matrix_x((pos->x << 12) / 360, &matrix);
    pos->z += *arg1;
    arg1++;
    shooter_rotation_matrix_z((pos->z << 12) / 360, &matrix);
    pos->y += *arg1;
    shooter_rotation_matrix_y((pos->y << 12) / 360, &matrix);
    ((ShooterCoordinateView *)&D_80127480[arg0])->local = matrix;
    ((ShooterCoordinateView *)&D_80127480[arg0])->flag = 0;
}

void shooter_set_rotation_from_pose(s32 arg0, s16 *arg1) {
    ShooterPosition *p;

    p = &D_80128880[arg0];
    p->x = arg1[3];
    p->z = arg1[4];
    arg1 += 5;
    p->y = arg1[0];
}

/* FAKE: the empty do-while adds a basic block after `base = arg2 + 4`, so arg2 gets an allocated range ($a3: `lw a3,72(sp); addiu a0,a3,4`) as in the original;
   found by the permuter, real source unknown (probably an empty macro). T-16561 */
void shooter_attach_boss_part(s32 arg0, s32 arg1, s32 arg2, void *arg3, void *arg4, s32 arg5) {
    u8 *p;
    ShooterPosition *pos;
    s32 slot;
    s32 base;

    slot = arg1 + 0x10;
    shooter_release_actor_model(slot);
    base = arg2 + 4;
    do { } while (0);
    func_8009B3C0(base);
    func_8009B430(base + 8, (u8 *)&shooter_actors[slot].model, arg0 - 1, base);
    shooter_actors[slot].model.attribute = 0x80000000;
    shooter_actors[slot].flag_bytes[1] = 0x80;
    shooter_actors[slot].x = D_80127480[arg0].x;
    shooter_actors[slot].y = D_80127480[arg0].y;
    shooter_actors[slot].z = D_80127480[arg0].z;
    pos = &D_80128880[arg0];
    shooter_actors[slot].rotation_x = pos->x;
    shooter_actors[slot].rotation_y = pos->y;
    shooter_actors[slot].rotation_z = pos->z;
    p = (u8 *)(arg1 * 2 + (s32)arg4);
    shooter_actors[slot].stats[1] = p[0];
    shooter_actors[slot].counter = p[1];
    shooter_actors[slot].auxiliary = 1;
    shooter_actors[slot].stats[0] = *(u8 *)(arg1 * arg5 + (s32)arg3);
    shooter_actors[slot].stats[3] = 1;
    shooter_actors[slot].model.links[0] = (s32)&D_80127480[arg0];
}

s32 shooter_boss_damage_percent(s32 arg0, void *arg1) {
    s32 i;
    s32 num;
    s32 den;

    den = 0;
    num = 0;
    for (i = 0; i < arg0; i++) {
        /* FAKE: byte-pointer Uadd instead of Uixa keeps uopt from replacing the loop exit test i != arg0 with a pointer bound (T-24120). */
        num += ((ShooterActor *)((u8 *)shooter_actors + (i + 16) * 0x88))->stats[1];
        den += ((u8 *)arg1)[i * 2] * (i + 16 + shooter_actors)->counter;
    }
    return 100 - (num * 100) / den;
}

/* No pointer locals for D_80128880[arg0] / D_80127480[...]: each would add a stack home and move the locals 4 bytes. T-16561 */
void shooter_fire_from_joint(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6) {
    MATRIX m;
    SVECTOR vec;
    SVECTOR rot;
    ShooterPosition out;

    m = *(MATRIX *)view_base_matrix;
    shooter_rotation_matrix_x((D_80128880[arg0].x << 12) / 360, &m);
    shooter_rotation_matrix_z((D_80128880[arg0].z << 12) / 360, &m);
    shooter_rotation_matrix_y((D_80128880[arg0].y << 12) / 360, &m);
    vec.vx = D_80127480[arg1].x;
    vec.vy = D_80127480[arg1].y;
    vec.vz = D_80127480[arg1].z;
    ApplyMatrixSV(&m, &vec, &rot);
    out.x = D_80127480[arg0].x + rot.vx + arg2;
    out.y = D_80127480[arg0].y + rot.vy + arg3;
    out.z = D_80127480[arg0].z + rot.vz + (s16)arg4;
    shooter_spawn_enemy_bullet(out, arg5, arg6);
}

void shooter_boss_death_explosions(s32 arg0) {
    s16 var_s2;
    s32 var_s0;
    s32 var_s1;
    s32 var_s3;
    s32 var_s4;
    s32 var_s5;
    u8 temp_v0;

    var_s3 = arg0;
    var_s1 = 0;
    var_s4 = 3;
    var_s5 = 0x880;
    do {
        var_s0 = var_s3 * 0x10;
        temp_v0 = *(u8 *)((unsigned char *)(&shooter_actors->flag_bytes[var_s1]) + (3539));
        var_s0 = var_s3 * 0x11;
        if ((temp_v0 != 0) && (temp_v0 < 9U)) {
            var_s2 = rand() & 0xFF;
            var_s0 = (s32) &shooter_actors->flag_bytes[var_s1];
            var_s2 = var_s2;
            shooter_draw_explosion_sprite((s32) *(s16 *)((unsigned char *)var_s0 + (3652)), (s32) *(s16 *)((unsigned char *)var_s0 + (3654)), (s32) *(s16 *)((unsigned char *)var_s0 + (3656)), *(u8 *)((unsigned char *)var_s0 + (3539)) - 1);
            var_s0 = (s32) &shooter_actors->flag_bytes[var_s1];
            shooter_draw_explosion_sprite((s32) (s16) (*(s16 *)((unsigned char *)var_s0 + (3652)) + var_s2), (s32) *(s16 *)((unsigned char *)var_s0 + (3654)), (s32) *(s16 *)((unsigned char *)var_s0 + (3656)), *(u8 *)((unsigned char *)var_s0 + (3539)) - 2);
            var_s0 = (s32) &shooter_actors->flag_bytes[var_s1];
            shooter_draw_explosion_sprite((s32) (s16) (*(s16 *)((unsigned char *)var_s0 + (3652)) - var_s2), (s32) (s16) (*(s16 *)((unsigned char *)var_s0 + (3654)) + var_s2), (s32) *(s16 *)((unsigned char *)var_s0 + (3656)), *(u8 *)((unsigned char *)var_s0 + (3539)) - 3);
            var_s0 = (s32) &shooter_actors->flag_bytes[var_s1];
            shooter_draw_explosion_sprite((s32) *(s16 *)((unsigned char *)var_s0 + (3652)), (s32) (s16) (*(s16 *)((unsigned char *)var_s0 + (3654)) - var_s2), (s32) *(s16 *)((unsigned char *)var_s0 + (3656)), *(u8 *)((unsigned char *)var_s0 + (3539)) - 4);
        } else {
            var_s0 = var_s3 * 0x88;
            *(s16 *)((unsigned char *)(&shooter_actors->flag_bytes[var_s1]) + (3652)) = (s16) ((shooter_actors[var_s3].x + (rand() & 0x3FF)) - 0x200);
            *(s16 *)((unsigned char *)(&shooter_actors->flag_bytes[var_s1]) + (3654)) = (s16) ((shooter_actors[var_s3].y + (rand() & 0x3FF)) - 0x200);
            *(s16 *)((unsigned char *)(&shooter_actors->flag_bytes[var_s1]) + (3656)) = (s16) ((rand() & 0xFFF) - 0x1770);
            *(s8 *)((unsigned char *)(&shooter_actors->flag_bytes[var_s1]) + (3539)) = 0;
        }
        if ((game_state.frame_count & 3) == 3) {
            var_s0 = (s32) &shooter_actors->flag_bytes[var_s1];
            *(u8 *)((unsigned char *)var_s0 + (3539)) = (u8) (*(u8 *)((unsigned char *)var_s0 + (3539)) + 1);
        }
        var_s1 += 0x88;
    } while (var_s1 != 0x880);
}
