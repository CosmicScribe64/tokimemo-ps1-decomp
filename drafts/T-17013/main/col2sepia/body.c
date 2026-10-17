/* T-17013 draft, 171 differing rows; single pointer, writes through a (Pix2Out *) cast of it. Pix2In/Pix2Out are the types above func_8004B590 in src/main/8004AD60.c. */
void col2sepia(arg0, arg1, arg2, arg3, arg4)
s32 arg0;
Pix2In *arg1;
s32 arg2;
s32 arg3;
s32 arg4;
{
    Pix2In *src;
    s32 i;
    u32 t;

    func_8004B590(arg0, arg1);
    src = arg1;
    for (i = 0; i != 0x80; i++) {
        if (src->r0 + src->g0 + src->b0 != 0) {
            t = src->r0 + arg2;
            if (t >= 0x20) {
                ((Pix2Out *)src)->r0 = 0x1F;
            } else {
                ((Pix2Out *)src)->r0 = t;
            }
            t = src->g0 + arg3;
            if (t >= 0x20) {
                ((Pix2Out *)src)->g0 = 0x1F;
            } else {
                ((Pix2Out *)src)->g0 = t;
            }
            t = src->b0 + arg4;
            if (t >= 0x20) {
                ((Pix2Out *)src)->b0 = 0x1F;
            } else {
                ((Pix2Out *)src)->b0 = t;
            }
        }
        if (((Pix2Out *)src)->r1 + src->g1 + src->b1 != 0) {
            t = ((Pix2Out *)src)->r1 + arg2;
            if (t >= 0x20) {
                ((Pix2Out *)src)->r1 = 0x1F;
            } else {
                ((Pix2Out *)src)->r1 = t;
            }
            t = src->g1 + arg3;
            if (t >= 0x20) {
                ((Pix2Out *)src)->g1 = 0x1F;
            } else {
                ((Pix2Out *)src)->g1 = t;
            }
            t = src->b1 + arg4;
            if (t >= 0x20) {
                ((Pix2Out *)src)->b1 = 0x1F;
            } else {
                ((Pix2Out *)src)->b1 = t;
            }
        }
        src++;
    }
}
