/* model_3d.c: src/main/80058D20.c; T-27059: object [0x80058D20,0x80059710); 3D view, light and coordinate setup; TMD model-data linking, model slots, display sizing and model updates (reuse h-cdio candidate). Original object 0x80058D20-0x80059710 (config/objects/main.txt). */
/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this file was matched with. */
#define MAIN_API_OVERRIDE_func_8009B430 /* implicit declaration: called with 3 arguments, the original leaves $a3 unset (main_api.h: 4) */

#include "common.h"
#include "common.h"
#include "game/model_records.h"
#include "game_enums.h"
#include "libetc.h"
#include "libgpu.h"
#include "libgs.h"
#include "libgte.h"
#include "libspu.h"
#include "main/actor.h"
#include "main/draw.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/entry_ui_scenes_school_events.h"
#include "main/import_views/scenes.h"
#include "main/import_views/scenes_school_events.h"
#include "main/state.h"
#include "main/view.h"

/* Sets projection distance 1000, calls two setters with 100 and 0x10000 (HYPOTHESIS: near and far clip), and sets the reference view with the eye at z=1000 looking at the origin. */
void initView(void) {
    func_8009AD30(0x3E8);
    D_80122740 = 0;
    D_80122744 = 0;
    D_80122748 = 0x3E8;
    D_8012274C = 0;
    D_80122750 = 0;
    D_80122754 = 0;
    D_80122758 = 0;
    D_8012275C = 0;
    func_80099540(&D_80122740);
    func_8009AD50(0x64);
    func_8009AD60(0x10000);
}

/* FAKE: retain the shared scalar light views until the libgs light aggregate
 * is available in project headers. T-9550 */
/* Sets three directional lights (white, green, red) and zero ambient; no caller in src. */
void initLight(void) {
    view_light_0 = 0;
    D_80122764 = 0;
    D_80122768 = 0x64;
    D_8012276C = 0xFF;
    D_8012276D = 0xFF;
    D_8012276E = 0xFF;
    func_8009AD70(0, (s32) &view_light_0);
    view_light_1 = 0;
    D_80122774 = 0x64;
    D_80122778 = 0;
    D_8012277C = 0;
    D_8012277D = 0xFF;
    D_8012277E = 0;
    func_8009AD70(1, (s32) &view_light_1);
    view_light_2 = 0x64;
    D_80122784 = 0;
    D_80122788 = 0;
    D_8012278C = 0xFF;
    D_8012278D = 0;
    D_8012278E = 0;
    func_8009AD70(2, (s32) &view_light_2);
    GsSetAmbient(0, 0, 0);
    func_8009B340(0);
}

/* Initializes the 16 model coordinate records, each at z=-4000; no caller in src. */
void initCoordinate(void) {
    GraphicsCoordinate2 *p;

    /* FAKE: init and loop head on one source line give the original's addiu order (as1 schedules by line, T-7020) */
    p = view_coordinates; do {
        func_80099E30(0, p);
        p->translate_z = -0xFA0;
        p->flag = 0;
        p++;
    } while (p != D_80122CA0);
}

/* HYPOTHESIS: the TMD walk of the PsyQ libgs samples (func_8009B3C0 =
 * GsMapModelingData, func_8009B430 = GsLinkObject4): skip the ID word, map the
 * data, skip the flags and object-count words. The separate `dop++` steps give
 * the original's `move s2,s3; addiu s2,s2,8`; the indexed loop gives its
 * address setup order. T-11610 */
/* Maps model data and links its objects to the 16 model slots. */
void initModelingData_init(s32 *arg0) {
    s32 *dop;
    s32 i;

    dop = arg0;
    dop++;
    func_8009B3C0((s32)dop);
    dop++;
    dop++;
    for (i = 0; i < 16; i++) {
        func_8009B430((s32)dop, (u8 *)&model_objects[i * 4], 0);
        model_objects[i * 4 + 1] = (u32)&view_coordinates[i];
        model_objects[i * 4] = 0x80000000;
    }
}

/* Same TMD walk as initModelingData_init for one object slot (count clamped
 * to 15). FAKE: the slot is reached by byte offset from model_objects (u32[], 16
 * bytes per slot) because the record type is not declared yet; the repeated
 * slot address, not a local, gives the original's shared spill slot (as in
 * TAIIKU func_80140738), and `&model_objects[arg1 * 4]` costs one more ugen
 * temporary. T-11610 */
s32 view_link_object(u32 *model_data_address, s32 destination_slot, u32 object_index) {
    u32 *dop;
    u32 n;

    dop = model_data_address;
    dop++;
    func_8009B3C0((s32)dop);
    dop++;
    n = *dop;
    if (n > 16) {
        n = 15;
    }
    dop++;
    if (object_index < n) {
        func_8009B430((s32)dop, (u8 *)model_objects + destination_slot * 0x10, object_index);
        *(u32 *)((u8 *)model_objects + destination_slot * 0x10 + 4) = (u32)&view_coordinates[destination_slot];
        *(u32 *)((u8 *)model_objects + destination_slot * 0x10) = 0;
        return 1;
    }
    return -1;
}

void view_model_slots_clear(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        model_objects[i * 4] = 0x80000000;
    }
}

void view_display_reinit(void) {
    func_80098380();
    func_8009B560(0, 0, 0, 0xF0);
    func_80098530();
    InitGeom();
    draw_buffer = func_80098370();
}

/* FAKE: the call arguments re-read the three halfwords just stored through a `u16 *` view of
 * game_state; uopt then passes the stored registers (a0/a1) instead of folding the constants,
 * which plain field reads do (18 lines). The else arm stores unk_000/unk_002 before unk_014 so
 * their `li` lead the arm and as1 hoists them above the branch, as in the original. T-24206 */
void view_display_set_narrow_or_standard(s32 arg0) {
    func_8009C674(0);
    func_8009C210(1);
    func_800AD950(0);
    func_8009C5E0(0);
    if (arg0 != 0) {
        game_state.draw_origin_x[0] = 0x80;
        game_state.draw_origin_x[1] = 0x80;
        game_state.video_width = 0x100;
        game_state.video_height = 0xF0;
        game_state.video_interlace = 0;
        func_80097F58(((u16 *)&game_state)[0], ((u16 *)&game_state)[1], ((u16 *)&game_state)[2], 0, 0);
        view_display_reinit();
        back_clear_switch(1);
    } else {
        game_state.video_width = 0x140;
        game_state.video_height = 0xF0;
        game_state.draw_origin_x[0] = 0xA0;
        game_state.draw_origin_x[1] = 0xA0;
        game_state.video_interlace = 0;
        func_80097F58(((u16 *)&game_state)[0], ((u16 *)&game_state)[1], ((u16 *)&game_state)[2], 0, 0);
        view_display_reinit();
        back_clear_switch(1);
    }
    func_800AD950(0);
    func_8009C5E0(1);
}

/* FAKE: the call re-reads the stored halfwords through a `u16 *` view of game_state so uopt
 * forwards the stored registers into a0-a2; each arm writes unk_002 (and the else arm unk_000)
 * before the unk_014 stores so as1 hoists their li into the branch delay slots, as in
 * view_display_set_narrow_or_standard (T-24206). T-24210 */
void view_display_set_wide_or_standard(s32 arg0) {
    view_frame_buffers_clear();
    func_8009C674(0);
    func_8009C210(1);
    func_8009C5E0(0);
    if ((arg0 != 0) && (game_state.video_width != 0x200)) {
        game_state.video_height = 0xF0;
        game_state.draw_origin_x[0] = 0x100;
        game_state.draw_origin_x[1] = 0x100;
        game_state.video_width = 0x200;
        game_state.video_interlace = 0;
        func_80097F58(((u16 *)&game_state)[0], ((u16 *)&game_state)[1], ((u16 *)&game_state)[2], 0, 0);
        view_display_reinit();
        back_clear_switch(1);
    } else if ((arg0 == 0) && (game_state.video_width != 0x140)) {
        game_state.video_width = 0x140;
        game_state.video_height = 0xF0;
        game_state.draw_origin_x[0] = 0xA0;
        game_state.draw_origin_x[1] = 0xA0;
        game_state.video_interlace = 0;
        func_80097F58(((u16 *)&game_state)[0], ((u16 *)&game_state)[1], ((u16 *)&game_state)[2], 0, 0);
        view_display_reinit();
        back_clear_switch(1);
    }
    func_800AD950(0);
    func_8009C5E0(1);
}

/* FAKE: the RECT field stores sharing a source line are scheduled as a unit by as1, which gives the original's store order (h before w). T-8010 */
void view_frame_buffers_clear(void) {
    RECT rect;

    rect.w = 0x100; rect.h = 0x1E0;
    rect.x = 0;
    rect.y = 0;
    func_8009C7F8(&rect, 0, 0, 0);
    func_8009C674(0);
    rect.x = 0x100; rect.w = 0x100; rect.h = 0x1E0;
    rect.y = 0;
    func_8009C7F8(&rect, 0, 0, 0);
    func_8009C674(0);
}

/* Model record of actor_pool walked two display slots (0x88 bytes) at a time. T-17017 */
typedef struct ModelRecord {
    /* 0x00 */ u8 active;
    /* 0x01 */ u8 model_flags;
    /* 0x02 */ u8 pad_02[2];
    /* 0x04 */ u8 obj[4];       /* object passed by address (GsDOBJ2-like) */
    /* 0x08 */ s32 handle;
    /* 0x0C */ u8 pad_0C[8];
    /* 0x14 */ s32 coordinate_flag;
    /* 0x18 */ MATRIX coord;
    /* 0x38 */ u8 pad_38[0x2C];
    /* 0x64 */ SVECTOR rot;
    /* 0x6C */ SVECTOR scale;
    /* 0x74 */ SVECTOR pos;
    /* 0x7C */ SVECTOR vel;
    /* 0x84 */ u8 pad_84[4];
} ModelRecord; /* size 0x88 */

/* 20-byte ordering-table header (GsOT-like) of the draw_ot_table pair, indexed by draw_buffer. T-17017 */
typedef struct OrderingTableHeader { s32 w[5]; } OrderingTableHeader;

/* The index loop (i += 2 over 0x44-byte slots) is strength-reduced to the 0x88 pointer walk of the
 * original and keeps r+4 and r+0x18 in s5/s4; the OrderingTableHeader element address keeps &draw_ot_table out of
 * a saved register; the scalars p, i and r declared before rot give the 0x108 frame. T-17017 */
void view_models_update_draw(void) {
    SVECTOR *p;
    s32 i;
    ModelRecord *r;
    SVECTOR rot;
    VECTOR scale;
    MATRIX m, sm, ls, lw;

    /* FAKE: T-26000: never-true arm that ugen drops (as1 deletes the dead byte load). Its
     * indirect store puts the two MATRIX copies at the top of the loop in block-copy state F
     * (tools/ugen_pass.py), as in the original (one scratch register before each copy's data
     * register; 62 rows without it). IDO 4.1's ugen gives this body F without the arm, because
     * it also sets F at a branch on a value (`r->flags & 0x80`); the pass does not model that and
     * counts the store of an arm ugen drops, which 4.1 ignores
     * (tools/ido_eval_cases/ugen_copy_state/copy_dropped_arm_store.c). Still needed in T-26002. */
    if ((u32)D_800B0896 < 0) {
        *(u8 *)0 = 0;
    }
    for (i = *(s32 *)&game_state.menu_hit[7]; i < 0xA0; i += 2) {
        r = (ModelRecord *)(actor_pool + i * 0x44);
        if (r->active && (r->model_flags & 0x80)) {
            p = &rot;
            p->vz = r->rot.vz;
            p->vx = r->rot.vx;
            p->vy = r->rot.vy;
            m = *(MATRIX *)view_base_matrix;
            sm = *(MATRIX *)view_base_matrix;
            RotMatrix(p, &m);
            if (r->scale.vx != 0) {
                scale.vx = r->scale.vx;
            } else {
                scale.vx = 0x1000;
            }
            if (r->scale.vy != 0) {
                scale.vy = r->scale.vy;
            } else {
                scale.vy = 0x1000;
            }
            if (r->scale.vz != 0) {
                scale.vz = r->scale.vz;
            } else {
                scale.vz = 0x1000;
            }
            ScaleMatrix(&sm, &scale);
            MulMatrix(&m, &sm);
            if (r->model_flags & 0x10) {
                r->pos.vx += r->vel.vx;
                r->pos.vy += r->vel.vy;
                r->pos.vz += r->vel.vz;
            }
            m.t[0] = r->pos.vx;
            m.t[1] = r->pos.vy;
            m.t[2] = r->pos.vz;
            r->coord = m;
            r->coordinate_flag = 0;
            func_80099B10(r->handle, &lw, &ls);
            func_80099F04(&lw);
            func_8009A200(r->handle, &ls);
            func_80099ED8(&ls);
            func_8009A4D0(r->obj, &((OrderingTableHeader *)&draw_ot_table)[draw_buffer], 6, 0x1F800000);
        }
    }
}

void view_model_record_init(s32 arg0) {
    u8 *p;

    p = actor_pool + arg0 * 0x44;
    p[0] = 1;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    *(s16 *)(p + 0x64) = 0;
    *(s16 *)(p + 0x66) = 0;
    *(s16 *)(p + 0x68) = 0;
    *(s16 *)(p + 0x6A) = 0;
    *(s16 *)(p + 0x6C) = 0x1000;
    *(s16 *)(p + 0x6E) = 0x1000;
    *(s16 *)(p + 0x70) = 0x1000;
    *(s16 *)(p + 0x72) = 0;
    *(s16 *)(p + 0x74) = 0;
    *(s16 *)(p + 0x76) = 0;
    *(s16 *)(p + 0x78) = 0;
    *(s16 *)(p + 0x7A) = 0;
    *(s16 *)(p + 0x7C) = 0;
    *(s16 *)(p + 0x7E) = 0;
    *(s16 *)(p + 0x80) = 0;
    *(s16 *)(p + 0x82) = 0;
    p[0x84] = 0;
    p[0x85] = 1;
    p[0x86] = 0;
}
