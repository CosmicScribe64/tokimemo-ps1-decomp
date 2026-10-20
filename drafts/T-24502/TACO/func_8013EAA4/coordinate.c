void func_8013EAA4(s32 arg0) {
    MATRIX matrix;
    SVECTOR v;
    s32 pt[4];
    TcPos pos;
    s32 depth;
    s32 flag;
    s32 kind;
    u32 phase;
    Tc14 *particle;
    POLY_FT4 *poly;
    s32 dist;
    s32 o;
    s32 j;
    s32 y0;
    s32 y1;
    s32 y2;
    s32 size;
    s32 height;
    s32 clut;
    u32 frame;

    matrix = D_80129C40;
    particle = &D_8015EC70[arg0];
    pos = *(TcPos *)particle;
    pos.z -= 0x200;
    kind = particle->unk11 & 7;
    phase = particle->unk12 >> 1;
    func_800A1100(&matrix);
    func_800A1070(&matrix);
    v.vz = pos.z;
    y0 = 0;
    for (o = 0; o < 2; o++) {
        v.vy = pos.y + y0 - 0x40;
        for (j = 0; j < 2; j++) {
            v.vx = pos.x + j * 0x80 - 0x40;
            func_800AB170(&v, &pt[j * 2 + o], &depth, &flag);
        }
        y0 += 0x80;
    }
    if (((s16 *)pt)[4] >= -0x2C0 && ((s16 *)pt)[0] < 0x2C1) {
        if (((s16 *)pt)[3] >= D_800E6280.unk_018[D_8011ECA0] - 0x200 && D_800E6280.unk_018[D_8011ECA0] + 0x200 >= ((s16 *)pt)[7]) {
            dist = (D_80122754 - pos.z) / 127;
            if (dist >= 0x100) {
                dist = 0xFF;
            } else if (dist <= 0) {
                dist = 1;
            }
            poly = GetWorkBase(0x28, D_8011ECA0);
            func_8009F040(poly);
            SetSemiTrans(poly, 0);
            poly->r0 = 0x80;
            poly->g0 = 0x80;
            poly->b0 = 0x80;
            poly->tpage = func_8009ECB0(0, 1, 0x380, 0x100);
            if (D_8015F560[kind] != 0) {
                frame = phase % D_8015F560[kind];
            } else {
                frame = 0;
            }
            poly->clut = func_8009ED74(frame * 0x10, D_8015F568[kind] + 0x1E0);
            poly->u0 = D_8015F570[kind] + 1;
            poly->v0 = D_8015F578[kind] + 1;
            poly->u1 = D_8015F570[kind] + 0xF;
            poly->v1 = D_8015F578[kind] + 1;
            poly->u2 = D_8015F570[kind] + 1;
            poly->v2 = D_8015F578[kind] + 0xF;
            poly->u3 = D_8015F570[kind] + 0xF;
            poly->v3 = D_8015F578[kind] + 0xF;
            poly->x0 = ((s16 *)pt)[4];
            poly->y0 = ((s16 *)pt)[5];
            poly->x1 = ((s16 *)pt)[0];
            poly->y1 = ((s16 *)pt)[1];
            poly->x2 = ((s16 *)pt)[6];
            poly->y2 = ((s16 *)pt)[7];
            poly->x3 = ((s16 *)pt)[2];
            poly->y3 = ((s16 *)pt)[3];
            AddPrim((D_8011ECA0 << 10) + dist * 4 + D_800E8CA0, poly);
            height = (func_800A0070(particle->unk12 * 0x10) << 7) / 4096;
            v.vz = pos.z;
            for (o = 0; o < 2; o++) {
                y1 = o * height * 2;
                v.vy = pos.y + y1 - height;
                for (j = 0; j < 2; j++) {
                    v.vx = pos.x + j * 0x100 - 0x80;
                    func_800AB170(&v, &pt[j * 2 + o], &depth, &flag);
                }
            }
            poly = GetWorkBase(0x28, D_8011ECA0);
            func_8009F040(poly);
            SetSemiTrans(poly, 1);
            poly->r0 = 0x80;
            poly->g0 = 0x80;
            poly->b0 = 0x80;
            poly->tpage = func_8009ECB0(0, 1, 0x380, 0x100);
            clut = ((s32)phase % 14) * 0x10;
            poly->clut = func_8009ED74(clut, 0x1E7);
            poly->u0 = 0x61, poly->v0 = 0x21, poly->u1 = 0x7F, poly->v1 = 0x21, poly->u2 = 0x61, poly->v2 = 0x3F, poly->u3 = 0x7F, poly->v3 = 0x3F;
            poly->x0 = ((s16 *)pt)[4];
            poly->y0 = ((s16 *)pt)[5];
            poly->x1 = ((s16 *)pt)[0];
            poly->y1 = ((s16 *)pt)[1];
            poly->x2 = ((s16 *)pt)[6];
            poly->y2 = ((s16 *)pt)[7];
            poly->x3 = ((s16 *)pt)[2];
            poly->y3 = ((s16 *)pt)[3];
            AddPrim((D_8011ECA0 << 10) + dist * 4 + D_800E8CA0, poly);
            size = (func_800A0140(particle->unk12 * 0x10) << 7) / 4096;
            v.vz = pos.z;
            o = 0;
            for (y2 = 0; y2 != 0x200; y2 += 0x100) {
                v.vy = pos.y + y2 - 0x80;
                for (j = 0; j < 2; j++) {
                    v.vx = pos.x + j * size * 2 - size;
                    func_800AB170(&v, &pt[j * 2 + o], &depth, &flag);
                }
                o++;
            }
            poly = GetWorkBase(0x28, D_8011ECA0);
            func_8009F040(poly);
            SetSemiTrans(poly, 1);
            poly->r0 = 0x80;
            poly->g0 = 0x80;
            poly->b0 = 0x80;
            poly->tpage = func_8009ECB0(0, 1, 0x380, 0x100);
            poly->clut = func_8009ED74(clut, 0x1E7);
            poly->u0 = 0x61, poly->v0 = 0x21, poly->u1 = 0x7F, poly->v1 = 0x21, poly->u2 = 0x61, poly->v2 = 0x3F, poly->u3 = 0x7F, poly->v3 = 0x3F;
            poly->x0 = ((s16 *)pt)[4];
            poly->y0 = ((s16 *)pt)[5];
            poly->x1 = ((s16 *)pt)[0];
            poly->y1 = ((s16 *)pt)[1];
            poly->x2 = ((s16 *)pt)[6];
            poly->y2 = ((s16 *)pt)[7];
            poly->x3 = ((s16 *)pt)[2];
            poly->y3 = ((s16 *)pt)[3];
            AddPrim((D_8011ECA0 << 10) + dist * 4 + D_800E8CA0, poly);
        }
    }
}
