/* T-17017 hand draft of main func_8005938C, not a match: frame, stack offsets and the loop-invariant
 * address registers differ (see wiki/matching-notes.md, section T-17017). Needs ModelRec88 and the
 * three-scalar/pad locals below. */
/* Model record of D_8011ECD0 walked two display slots (0x88 bytes) at a time. T-17017 */
typedef struct ModelRec88 {
    /* 0x00 */ u8 active;
    /* 0x01 */ u8 flags;
    /* 0x02 */ u8 pad_02[2];
    /* 0x04 */ u8 obj[4];       /* object passed by address (GsDOBJ2-like) */
    /* 0x08 */ s32 handle;
    /* 0x0C */ u8 pad_0C[8];
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ MATRIX coord;
    /* 0x38 */ u8 pad_38[0x2C];
    /* 0x64 */ SVECTOR rot;
    /* 0x6C */ SVECTOR scale;
    /* 0x74 */ SVECTOR pos;
    /* 0x7C */ SVECTOR vel;
    /* 0x84 */ u8 pad_84[4];
} ModelRec88; /* size 0x88 */

void func_8005938C(void) {
    SVECTOR *p;
    s32 v1;
    s32 v2;
    SVECTOR rot;
    VECTOR scale;
    MATRIX m, sm, ls, lw;
    ModelRec88 *r;
    s32 pad[1];

    if (*(s32 *)&D_800E6280.unk_1095[7] < 0xA0) {
        r = (ModelRec88 *)(D_8011ECD0 + *(s32 *)&D_800E6280.unk_1095[7] * 0x44);
        do {
            if ((r->active != 0) && (r->flags & 0x80)) {
                p = &rot;
                p->vz = r->rot.vz;
                p->vx = r->rot.vx;
                p->vy = r->rot.vy;
                m = *(MATRIX *)D_80125C60;
                sm = *(MATRIX *)D_80125C60;
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
                if (r->flags & 0x10) {
                    r->pos.vx += r->vel.vx;
                    r->pos.vy += r->vel.vy;
                    r->pos.vz += r->vel.vz;
                }
                m.t[0] = r->pos.vx;
                m.t[1] = r->pos.vy;
                m.t[2] = r->pos.vz;
                r->coord = m;
                r->unk_14 = 0;
                func_80099B10(r->handle, &lw, &ls);
                func_80099F04(&lw);
                func_8009A200(r->handle, &ls);
                func_80099ED8(&ls);
                func_8009A4D0(r->obj, (s32)&D_800E8C70 + D_8011ECA0 * 0x14, 6, 0x1F800000);
            }
            r++;
        } while ((u8 *)r < D_80121750);
    }
}
