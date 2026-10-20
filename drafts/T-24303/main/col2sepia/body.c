void col2sepia(arg0, arg1, dr, dg, db)
Pix2In *arg0;
Pix2In *arg1;
s32 dr;
s32 dg;
s32 db;
{
    Pix2In *p;
    s32 i;
    u32 v;

    func_8004B590(arg0, arg1);
    p = arg1;
    for (i = 0; i != 0x80; i++) {
        if (p->r0 + p->g0 + p->b0 != 0) {
            v = p->r0 + dr;
            if (v >= 0x20) {
                p->r0 = 0x1F;
            } else {
                p->r0 = v;
            }
            v = p->g0 + dg;
            if (v >= 0x20) {
                p->g0 = 0x1F;
            } else {
                p->g0 = v;
            }
            v = p->b0 + db;
            if (v >= 0x20) {
                p->b0 = 0x1F;
            } else {
                p->b0 = v;
            }
        }
        if (p->r1 + p->g1 + p->b1 != 0) {
            v = p->r1 + dr;
            if (v >= 0x20) {
                p->r1 = 0x1F;
            } else {
                p->r1 = v;
            }
            v = p->g1 + dg;
            if (v >= 0x20) {
                p->g1 = 0x1F;
            } else {
                p->g1 = v;
            }
            v = p->b1 + db;
            if (v >= 0x20) {
                p->b1 = 0x1F;
            } else {
                p->b1 = v;
            }
        }
        p++;
    }
}
