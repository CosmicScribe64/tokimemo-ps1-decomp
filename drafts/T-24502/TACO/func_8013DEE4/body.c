s32 func_8013DEE4(s32 arg0) {
    /* FAKE: donor frame pad and local order reproduce frame0xD8 and geometry homes. T-24502 */
    s32 pad[2];
    MATRIX m;
    SVECTOR w;
    s32 pt[4];
    POLY_FT4 *poly;
    s32 dist;
    s32 depth;
    s32 flag;
    s32 o;
    s32 kind;
    s32 mode;
    s32 j;
    s32 rot;
    SVECTOR v;
    s32 step;
    s32 c;
    s32 q;

    v = *(SVECTOR *)&D_8015EB30[arg0];
    kind = D_8015EB30[arg0].unk11 & 0xF;
    m = D_80129C40;
    v.vz -= 0x200;
    func_800A1100(&m);
    func_800A1070(&m);
    switch (kind) {
    case 3:
    case 5:
        step = 0x100;
        break;
    case 0:
        step = 0x40;
        break;
    default:
    case 1:
    case 2:
    case 4:
        step = 0x80;
        break;
    }
    rot = 0;
    o = 0;
    q = 0;
    mode = kind;
    w.vz = v.vz;
    do {
        w.vy = v.vy + rot - step / 2;
        for (j = 0; j < 2; j++) {
            w.vx = v.vx + j * step - step / 2;
            func_800AB170(&w, (u8 *)pt + q + j * 8, &depth, &flag);
        }
        o++;
        q += 4;
        rot += step;
    } while (o != 2);
    if (((s16 *)pt)[4] >= -0x2C0 && ((s16 *)pt)[0] < 0x2C1) {
        if (((s16 *)pt)[3] >= D_800E6280.unk_018[D_8011ECA0] - 0x200
            && D_800E6280.unk_018[D_8011ECA0] + 0x200 >= ((s16 *)pt)[7]) {
            dist = (D_80122754 - v.vz) / 127;
            if (dist >= 0x100) {
                dist = 0xFF;
            } else if (dist <= 0) {
                return -1;
            }
            poly = func_800490F0(0x28, D_8011ECA0);
            func_8009F040(poly);
            if (kind != 5) {
                c = 0xFF;
                if (kind != 6) {
                    c = 0x80;
                }
            } else {
                c = 0xFF;
            }
            poly->r0 = c;
            poly->g0 = c;
            poly->b0 = c;
            switch (mode) {
            case 3:
            case 5:
            case 6:
                SetSemiTrans(poly, 1);
                break;
            default:
            case 0:
            case 1:
            case 2:
            case 4:
                SetSemiTrans(poly, 0);
                break;
            }
            if (kind != 4) {
                poly->tpage = func_8009ECB0(1, 1, 0x3C0, 0x100);
            } else {
                poly->tpage = func_8009ECB0(0, 1, 0x380, 0x100);
            }
            if (kind != 4) {
                if (kind == 5) {
                    poly->clut = func_8009ED74(0, 0x1EA);
                } else {
                    poly->clut = func_8009ED74(0x100, 0x1F8);
                }
            } else {
                poly->clut = func_8009ED74(0, 0x1E5);
            }
            switch (mode) {
            default:
            case 0:
            case 1:
                poly->u0 = 1, poly->v0 = 0x41, poly->u1 = 0x1F, poly->v1 = 0x41, poly->u2 = 1, poly->v2 = 0x5F, poly->u3 = 0x1F, poly->v3 = 0x5F;
                break;
            case 2:
                poly->u0 = 0x41, poly->v0 = 1, poly->u1 = 0x7F, poly->v1 = 1, poly->u2 = 0x41, poly->v2 = 0x3F, poly->u3 = 0x7F, poly->v3 = 0x3F;
                break;
            case 3:
                poly->u0 = 1, poly->v0 = 1, poly->u1 = 0x3F, poly->v1 = 1, poly->u2 = 1, poly->v2 = 0x3F, poly->u3 = 0x3F, poly->v3 = 0x3F;
                break;
            case 4:
                poly->u0 = 0x40, poly->v0 = 0x80, poly->u1 = 0x7F, poly->v1 = 0x80, poly->u2 = 0x40, poly->v2 = 0xBF, poly->u3 = 0x7F, poly->v3 = 0xBF;
                break;
            }
            poly->x0 = ((s16 *)pt)[4];
            poly->y0 = ((s16 *)pt)[5];
            poly->x1 = ((s16 *)pt)[0];
            poly->y1 = ((s16 *)pt)[1];
            poly->x2 = ((s16 *)pt)[6];
            poly->y2 = ((s16 *)pt)[7];
            poly->x3 = ((s16 *)pt)[2];
            poly->y3 = ((s16 *)pt)[3];
            func_8009EED0((D_8011ECA0 << 10) + dist * 4 + D_800E8CA0, poly);
        }
    }
}
